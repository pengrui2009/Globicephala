#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <error.h>
#include <gps_drv.h>
#include <uart.h>
#include <osal_thread.h>

#include "ntrip.h"

static ntrip_contrl_t g_ntrip_control; /*Global control variables*/
static ntrip_config_t g_ntrip_config; /*Global configuration variables*/
static uint8_t g_ntrip_gga[GGA_BUF_SIZE]; /*Global GGA buffer*/
static int stop = 0; /*ntrip network termination sign*/

/*URL username and password coding table*/
static const char encodingTable [64] = {
  'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P',
  'Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f',
  'g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v',
  'w','x','y','z','0','1','2','3','4','5','6','7','8','9','+','/'
};

/* CVS revision and version */
static char revisionstr[] = "$Revision: 1.50 $";

/*****************************************************
func:getargs
desc:Fill the ntrip_args_t structure function
para:args is a buf of type ntrip_args_t,gga_string is a GGA frame
return:Less than 0 failed,Greater than or equal to 0 indicates success
*****************************************************/
static int getargs(ntrip_args_t *args,char* gga_string)
{
  int res = 1;
  //int getoptr = 0;
  char *a;
  int i = 0;

  args->server = NTRIP_SERVER;//"ntrip.qxwz.com";
  args->port = NTRIP_PORT;//"8001";
  args->user = NTRIP_USER;//"beidou11";
  args->password = NTRIP_PASSWD;//"e78a7f1";

  args->nmea = gga_string;//"$GPGGA,023056.00,3957.34315,N,11621.53262,E,1,10,1.07,155.5,M,-8.6,M,,*43";//0;

  args->data = "RTCM32_GGB";
  args->bitrate = 0;
  args->proxyhost = 0;
  args->proxyport = "2101";
  args->mode = NTRIP1;
  args->initudp = 0;
  args->udpport = 0;

  for(a = revisionstr+11; *a && *a != ' '; ++a)
    revisionstr[i++] = *a;
  revisionstr[i] = 0;
  
  return res;
}

/*****************************************************
func:encode
desc:URL parameter encoding function
para:buf is data buffer,size is the length of this buffer,user is username,pwd is password
return:Less than 0 failed,Greater than or equal to 0 indicates success
*****************************************************/
static int encode(char *buf, int size, const char *user, const char *pwd)
{
  unsigned char inbuf[3];
  char *out = buf;
  int i, sep = 0, fill = 0, bytes = 0;

  while(*user || *pwd)
  {
    i = 0;
    while(i < 3 && *user) inbuf[i++] = *(user++);
    if(i < 3 && !sep)    {inbuf[i++] = ':'; ++sep; }
    while(i < 3 && *pwd)  inbuf[i++] = *(pwd++);
    while(i < 3)         {inbuf[i++] = 0; ++fill; }
    if(out-buf < size-1)
      *(out++) = encodingTable[(inbuf [0] & 0xFC) >> 2];
    if(out-buf < size-1)
      *(out++) = encodingTable[((inbuf [0] & 0x03) << 4)
               | ((inbuf [1] & 0xF0) >> 4)];
    if(out-buf < size-1)
    {
      if(fill == 2)
        *(out++) = '=';
      else
        *(out++) = encodingTable[((inbuf [1] & 0x0F) << 2)
                 | ((inbuf [2] & 0xC0) >> 6)];
    }
    if(out-buf < size-1)
    {
      if(fill >= 1)
        *(out++) = '=';
      else
        *(out++) = encodingTable[inbuf [2] & 0x3F];
    }
    bytes += 4;
  }
  if(out-buf < size)
    *out = 0;
  return bytes;
}

/*****************************************************
func:ntrip_main_proc
desc:ntrip core processing functions
para:NULL  
return:Less than 0 failed,Greater than or equal to 0 indicates success
*****************************************************/
static int ntrip_main_proc()
{
  ntrip_args_t args;
  //static int inter = 1;

  //  printf("recieve GGA is %s\n",p_msg->gga_buff);
  if(getargs(&args,(char *)&g_ntrip_gga))
  {
	FILE *ser = 0;
   // char nmeabuffer[200] = "$GNGGA,"; /* our start string */
	//size_t nmeabufpos = 0;
	//size_t nmeastarpos = 0;
	int sleeptime = 0;

	do
	{
	  int error = 0;
	  int sockfd = 0;
	  int numbytes;
	  char buf[MAXDATASIZE];
	  struct sockaddr_in their_addr; /* connector's address information */
	  struct hostent *he;
	  struct servent *se;
	  const char *server, *port, *proxyserver = 0;
	  char proxyport[6];
	  char *b;
	  long i;
	  
	  /*Set the sleep time*/
	  if(sleeptime)
	  {
		sleep(sleeptime);
		sleeptime += 2;
	  }
	  else
	  {
		sleeptime = 1;
	  }

	  /*Determine whether to use the proxy*/
	  if(args.proxyhost)
	  {
		int p;
		if((i = strtol(args.port, &b, 10)) && (!b || !*b))
		  p = i;
		else if(!(se = getservbyname(args.port, 0)))
		{
		  fprintf(stderr, "Can't resolve port %s.", args.port);
		  stop = 1;
		}
		else
		{
		  p = ntohs(se->s_port);
		}
		if(!stop && !error && (g_ntrip_control.ntrip_start_flag == 1))
		{
		  snprintf(proxyport, sizeof(proxyport), "%d", p);
		  port = args.proxyport;
		  proxyserver = args.server;
		  server = args.proxyhost;
		}
	  }
	  else
	  {
		server = args.server;
		port = args.port;
	  }
	  if(!stop && !error && (g_ntrip_control.ntrip_start_flag == 1))
	  {
		memset(&their_addr, 0, sizeof(struct sockaddr_in));
		if((i = strtol(port, &b, 10)) && (!b || !*b))
		  their_addr.sin_port = htons(i);
		else if(!(se = getservbyname(port, 0)))
		{
		  fprintf(stderr, "Can't resolve port %s.", port);
		  stop = 1;
		}
		else
		{
		  their_addr.sin_port = se->s_port;
		}
		if(!stop && !error && (g_ntrip_control.ntrip_start_flag == 1))
		{
		  if(!(he=gethostbyname(server)))
		  {
			//fprintf(stderr, "Server name lookup failed for '%s'.\n", server);
			error = 1;
		  }
		  else if((sockfd = socket(AF_INET, (args.mode == UDP ? SOCK_DGRAM:SOCK_STREAM), 0)) == -1)
		  {
			perror("socket");
			error = 1;
		  }
		  else
		  {
			their_addr.sin_family = AF_INET;
			their_addr.sin_addr = *((struct in_addr *)he->h_addr);
		  }
		}
	  }
	  if(!stop && !error && (g_ntrip_control.ntrip_start_flag == 1))
	  {
		if(args.mode == UDP)
		{}
		else if(args.data && *args.data != '%' && args.mode == RTSP)
		{}
		else  /*Use ntrip protocol*/
		{
		  //printf("%s,%u\n",__FILE__,__LINE__);
		  if(connect(sockfd, (struct sockaddr *)&their_addr,
		  sizeof(struct sockaddr)) == -1)
		  {
			perror("connect");
			error = 1;
		  }
		  if(!stop && !error && (g_ntrip_control.ntrip_start_flag == 1))
		  {
			//printf("%s,%u\n",__FILE__,__LINE__);
			if(!args.data)
			{
			  i = snprintf(buf, MAXDATASIZE,
			  "GET %s%s%s%s/ HTTP/1.1\r\n"
			  "Host: %s\r\n%s"
			  "User-Agent: %s/%s\r\n"
			  "Connection: close\r\n"
			  "\r\n"
			  , proxyserver ? "http://" : "", proxyserver ? proxyserver : "",
			  proxyserver ? ":" : "", proxyserver ? proxyport : "",
			  args.server, args.mode == NTRIP1 ? "" : "Ntrip-Version: Ntrip/2.0\r\n",
			  AGENTSTRING, revisionstr);
			}
			else
			{
			  const char *nmeahead = (args.nmea && args.mode == HTTP) ? args.nmea : 0;

			  i=snprintf(buf, MAXDATASIZE-40, /* leave some space for login */
			  "GET %s%s%s%s/%s HTTP/1.1\r\n"
			  "Host: %s\r\n%s"
			  "User-Agent: %s/%s\r\n"
			  "%s%s%s"
			  "Connection: close%s"
			  , proxyserver ? "http://" : "", proxyserver ? proxyserver : "",
			  proxyserver ? ":" : "", proxyserver ? proxyport : "",
			  args.data, args.server,
			  args.mode == NTRIP1 ? "" : "Ntrip-Version: Ntrip/2.0\r\n",
			  AGENTSTRING, revisionstr,
			  nmeahead ? "Ntrip-GGA: " : "", nmeahead ? nmeahead : "",
			  nmeahead ? "\r\n" : "",
			  (*args.user || *args.password) ? "\r\nAuthorization: Basic " : "");
			  if(i > MAXDATASIZE-40 || i < 0) /* second check for old glibc */
			  {
				fprintf(stderr, "Requested data too long\n");
				stop = 1;
			  }
			  else
			  {
				i += encode(buf+i, MAXDATASIZE-i-4, args.user, args.password);
				if(i > MAXDATASIZE-4)
				{
				  fprintf(stderr, "Username and/or password too long\n");
				  stop = 1;
				}
				else
				{
				  buf[i++] = '\r';
				  buf[i++] = '\n';
				  buf[i++] = '\r';
				  buf[i++] = '\n';
				  if(args.nmea && !nmeahead)
				  {
					int j = snprintf(buf+i, MAXDATASIZE-i, "%s\r\n", args.nmea);
					if(j >= 0 && j < MAXDATASIZE-i)
					  i += j;
					else
					{
					  fprintf(stderr, "NMEA string too long\n");
					  stop = 1;
					}
				  }
				}
			  }
			}
		  }
		  if(!stop && !error && (g_ntrip_control.ntrip_start_flag == 1))
		  {

		  //printf("%s,%u\n",__FILE__,__LINE__);
		  printf("send buf is %s",buf);
		  //printf("\nsend buff end\n");
			if(send(sockfd, buf, (size_t)i, 0) != i)
			{
			  perror("send");
			  error = 1;

			  printf("%s,%u",__FILE__,__LINE__);
			}
			else if(args.data && *args.data != '%')
			{

			//printf("%s,%u\n",__FILE__,__LINE__);
			  int k = 0;
			  int chunkymode = 0;
			  int starttime = time(0);
			  int lastout = starttime;
			  int totalbytes = 0;
			  int chunksize = 0;

			  while(!stop && !error && (g_ntrip_control.ntrip_start_flag == 1) && (numbytes=recv(sockfd, buf, MAXDATASIZE-1, 0)) > 0)
			  {
				//alarm(ALARMTIME);
				//printf("%s,%u\n",__FILE__,__LINE__);
            #if 0
				printf("\nrecive buf is %s,len is %d\n",buf,numbytes);
				printf("\nstart \n");
				int m =0;
				for(m=0;m < numbytes;m++){
					printf("0x%X ",buf[m]);
				}

				printf("\nend \n");
#endif
				if(!k)
				{
				  //printf("%s,%u\n",__FILE__,__LINE__);
				  printf("%s%d end\n",buf,numbytes);
				  if( numbytes > 17 &&
					!strstr(buf, "ICY 200 OK")	&&	/* case 'proxy & ntrip 1.0 caster' */
					(!strncmp(buf, "POP_GW_Ntrip_1.0_1457434850/1.0\r\n", 17) ||
					!strncmp(buf, "HTTP/1.0 200 OK\r\n", 17)) )
				  {
				  //printf("%s,%u\n",__FILE__,__LINE__);
					const char *datacheck = "Content-Type: gnss/data\r\n";
					const char *chunkycheck = "Transfer-Encoding: chunked\r\n";
					int l = strlen(datacheck)-1;
					int j=0;
					for(i = 0; j != l && i < numbytes-l; ++i)
					{
					  for(j = 0; j < l && buf[i+j] == datacheck[j]; ++j)
						;
					}
					if(i == numbytes-l)
					{
					  fprintf(stderr, "No 'Content-Type: gnss/data' found\n");
					  error = 1;
					}
					l = strlen(chunkycheck)-1;
					j=0;
					for(i = 0; j != l && i < numbytes-l; ++i)
					{
					  for(j = 0; j < l && buf[i+j] == chunkycheck[j]; ++j)
						;
					}
					if(i < numbytes-l)
					  chunkymode = 1;
				  }
				  else if(!strstr(buf, "ICY 200 OK"))
				  {
				  //printf("%s,%u\n",__FILE__,__LINE__);
					fprintf(stderr, "Could not get the requested data: ");
					for(k = 0; k < numbytes && buf[k] != '\n' && buf[k] != '\r'; ++k)
					{
					  fprintf(stderr, "%c", isprint(buf[k]) ? buf[k] : '.');
					}
					fprintf(stderr, "\n");
					error = 1;
				  }
				  else if(args.mode != NTRIP1)
				  {
				  //printf("%s,%u\n",__FILE__,__LINE__);
					fprintf(stderr, "NTRIP version 2 HTTP connection failed%s.\n",
					args.mode == AUTO ? ", falling back to NTRIP1" : "");
					if(args.mode == HTTP)
					  stop = 1;
				  }
				  ++k;
				}
				else
				{
				//printf("%s,%u\n",__FILE__,__LINE__);
				  sleeptime = 0;
				  if(chunkymode)
				  {
					int cstop = 0;
					int pos = 0;
					while(!stop && !cstop && !error && (pos < numbytes) && (g_ntrip_control.ntrip_start_flag == 1))
					{
					  switch(chunkymode)
					  {
					  case 1: /* reading number starts */
						chunksize = 0;
						++chunkymode; /* no break */
					  case 2: /* during reading number */
						i = buf[pos++];
						if(i >= '0' && i <= '9') chunksize = chunksize*16+i-'0';
						else if(i >= 'a' && i <= 'f') chunksize = chunksize*16+i-'a'+10;
						else if(i >= 'A' && i <= 'F') chunksize = chunksize*16+i-'A'+10;
						else if(i == '\r') ++chunkymode;
						else if(i == ';') chunkymode = 5;
						else cstop = 1;
						break;
					  case 3: /* scanning for return */
						if(buf[pos++] == '\n') chunkymode = chunksize ? 4 : 1;
						else cstop = 1;
						break;
					  case 4: /* output data */
						i = numbytes-pos;
						if(i > chunksize) i = chunksize;
						//if(args.device_num >= 0)
						if(g_ntrip_control.ntrip_start_flag == 1)
						{
						  int ofs = 0;
						  while(i > ofs && !cstop && !stop && !error && (g_ntrip_control.ntrip_start_flag == 1))
						  {
							int j;
							//int j = SerialWrite(&sx, buf+pos+ofs, i-ofs);
							//int j = dstream_device[args.device_num].send(buf+pos+ofs, i-ofs);//·¢ËÍ
							if(g_ntrip_config.mode == GPS_MODE_RSU)
							{
								//j = gps_add_event_queue(V2X_MSG_RAW_RTCM, i-ofs, 0, (uint8_t *)(buf+pos+ofs));
								if(g_ntrip_config.ntrip_rsu_callback != NULL)
								{
									(g_ntrip_config.ntrip_rsu_callback)((uint8_t *)(buf+pos+ofs),i-ofs);
									ofs = i;
								}
								else
								{
									fprintf(stderr, "ntrip rtcm ntrip_rsu_callback NULL\n");
									stop = 1;
								}
							}
							else
							{
								if(g_ntrip_config.gps_port > 0)
								{
									j = comport_send(g_ntrip_config.gps_port, (uint8_t *)(buf+pos+ofs),i-ofs);
									if(j < 0)
									{
										fprintf(stderr, "ntrip rtcm to gps failed,ret=%d\n",j);
										stop = 1;
									}
									else
										ofs += j;
								}
								else
									stop = 1;
							}
						  }
						}
						else
						  fwrite(buf+pos, (size_t)i, 1, stdout);

						//printf("%s,%u\n",__FILE__,__LINE__);
						totalbytes += i;
						chunksize -= i;
						pos += i;
						if(!chunksize)
						  chunkymode = 1;
						break;
					  case 5:
						if(i == '\r') chunkymode = 3;
						break;
					  }
					}
					if(cstop)
					{
					  fprintf(stderr, "Error in chunky transfer encoding\n");
					  error = 1;
					}
				  }
				  else
				  {
					totalbytes += numbytes;
					//if(args.device_num >= 0)
			if(g_ntrip_control.ntrip_start_flag == 1)
					{
					  int ofs = 0;
					  while(numbytes > ofs && !stop)
					  {
						//printf("\n\nbuf-serial is %s\n len is %d\n",buf+ofs,numbytes-ofs);
                    #if 0
						printf("\nserial start \n");
						int m =0;
						for(m=0;m < numbytes-ofs;m++){
							printf("0x%X,",buf[ofs+m]);
						}

						printf("\nserial end \n");
                    #endif
						//printf("%s,%u\n",__FILE__,__LINE__);
						//fflush(stdout);
						int i;
						//if(1 == inter){
							//i = SerialWrite(&sx, buf+ofs, numbytes-ofs);//numbytes-ofs;//
							//i = dstream_device[args.device_num].send(buf+ofs,numbytes-ofs);//·¢ËÍ,numbytes-ofs;//
							if(g_ntrip_config.mode == GPS_MODE_RSU)
							{
								//i = vam_add_event_queue(p_vam, V2X_MSG_RAW_RTCM, numbytes-ofs, 0, (uint8_t *)(buf+ofs));
								if(g_ntrip_config.ntrip_rsu_callback != NULL)
								{
									(g_ntrip_config.ntrip_rsu_callback)((uint8_t *)(buf+ofs),numbytes-ofs);
									ofs += numbytes-ofs;
								}
								else
								{
									fprintf(stderr, "ntrip rtcm ntrip_rsu_callback NULL\n");
									stop = 1;
								}
								
							}
							else
							{
								if(g_ntrip_config.gps_port > 0)
								{
									i = comport_send(g_ntrip_config.gps_port, (uint8_t *)(buf+ofs),numbytes-ofs);
									if(i < 0)
									{
										fprintf(stderr, "ntrip rtcm to gps failed,ret=%d\n",i);
										stop = 1;
									}
									else
										ofs += i;
								}
								else
									stop = 1;
							}	
							//inter = 0;
							//}
					  //  else{
						 //   i = numbytes-ofs;
						  //  inter = 1;
					  //  }
						//i = SerialWrite(&sx, buf+ofs, numbytes-ofs);
						// i = os_device_write(fd,buf+ofs,numbytes-ofs);
					  }
					}
					else{
					  fwrite(buf, (size_t)numbytes, 1, stdout);
					  //printf("%s,%u\n",__FILE__,__LINE__);
						}
				  }
				  fflush(stdout);
				  if(totalbytes < 0) /* overflow */
				  {
					totalbytes = 0;
					starttime = time(0);
					lastout = starttime;
				  }
				  //printf("%s,%u\n",__FILE__,__LINE__);
				  //printf("%s,%u\n",__FILE__,__LINE__);
				  if(args.bitrate)
				  {
					int t = time(0);
					if(t > lastout + 60)
					{
					  lastout = t;
					  fprintf(stderr, "Bitrate is %dbyte/s (%d seconds accumulated).\n",
					  totalbytes/(t-starttime), t-starttime);
					}
				  }
				}
				//printf("%s,%u\n",__FILE__,__LINE__);
				//memset(buf,0,MAXDATASIZE);
			  }
			}
			else
			{

			//printf("%s,%u\n",__FILE__,__LINE__);
			  sleeptime = 0;
			  while(!stop && (numbytes=recv(sockfd, buf, MAXDATASIZE-1, 0)) > 0)
			  {
			   // alarm(ALARMTIME);

				//printf("%s,%u\n",__FILE__,__LINE__);
				fwrite(buf, (size_t)numbytes, 1, stdout);
			  }
			}
		  }
		}
	  }
	  if(sockfd)
		closesocket(sockfd);
	} while(args.data && *args.data != '%' && !stop && (g_ntrip_control.ntrip_start_flag == 1));
	if(ser)
	  fclose(ser);
  }
  return 0;
}

/*****************************************************
func:ntrip_thread_entry
desc:ntrip task processing function
para:param is the argument passed in when creating the task  
return:NULL
*****************************************************/
void * ntrip_thread_entry (void *param)
{	
	while(1)
	{
        if((g_ntrip_control.gga_valid_flag)&&(g_ntrip_control.ntrip_start_flag))
		{
			/*Loop call ntrip core handler*/
            ntrip_main_proc();
        }
        else
		{
            sleep(1);
        }
	}
}

/*****************************************************
func:ntrip_start
desc:Start the NTRIP service
para:void
return:Less than 0 failed,Greater than or equal to 0 indicates success
*****************************************************/
int ntrip_start()
{
	int ret = ERR_OK;

	if(!(g_ntrip_control.gga_valid_flag))
		return -ERR_NOCFG;
	
	if(!(g_ntrip_control.ntrip_start_flag))
		g_ntrip_control.ntrip_start_flag = 1;

	return ret;
}

/*****************************************************
func:ntrip_stop
desc:stop the NTRIP service
para:void
return:Less than 0 failed,Greater than or equal to 0 indicates success
*****************************************************/
int ntrip_stop()
{
	int ret = ERR_OK;
	if(g_ntrip_control.ntrip_start_flag)
		g_ntrip_control.ntrip_start_flag = 0;

	return ret;
}

/*****************************************************
func:ntrip_gga_update
desc:Update gga data
para:buf is the data buffer,len is the length of this buffer 
return:Less than 0 failed,Greater than or equal to 0 indicates success
*****************************************************/
int ntrip_gga_update(uint8_t *buf, uint32_t len)
{
	int ret = ERR_OK;
	if(buf == NULL)
		return -ERR_INVAL;

	memset(g_ntrip_gga, 0, sizeof(g_ntrip_gga));
	memcpy(g_ntrip_gga, buf, len);
	if(g_ntrip_gga[0] == '$')
		g_ntrip_control.gga_valid_flag = 1;
	else
		g_ntrip_control.gga_valid_flag = 0;
	
	return ret;
}

/*****************************************************
func:ntrip_init
desc:init ntrip services
para:ntrip_config is the config of ntrip.
	When mode is RSU, you need to initialize the ntrip_rsu_callback parameter;When mode is normal, you need to initialize the gps_port parameter
return:Less than 0 failed,Greater than or equal to 0 indicates success
*****************************************************/
int ntrip_init(ntrip_config_t ntrip_config)
{
    int ret = ERR_OK;

	/*Has been initialized, returned directly*/
	if((g_ntrip_control.task_ntrip_valid)&&(g_ntrip_control.task_ntrip))
		return ret;
	
	/*Check normal mode, the incoming parameters are complete*/
	if((g_ntrip_config.mode == GPS_MODE_NORMAL)&&(g_ntrip_config.gps_port <= 0))
		return -ERR_INVAL;

	/*Check rsu mode, the incoming parameters are complete*/
	if((g_ntrip_config.mode == GPS_MODE_RSU)&&(g_ntrip_config.ntrip_rsu_callback == NULL))
		return -ERR_INVAL;
	
	memset(&g_ntrip_config, 0, sizeof(g_ntrip_config));
	memset(&g_ntrip_control, 0, sizeof(g_ntrip_control));
	memset(g_ntrip_gga, 0, sizeof(g_ntrip_gga));
	memcpy(&g_ntrip_config, &ntrip_config, sizeof(g_ntrip_config));

	/*Create NTRIP processing task*/
	ret = osal_task_create(&(g_ntrip_control.task_ntrip),"tk-ntrip",ntrip_thread_entry, NULL,NTRIP_THREAD_STACK_SIZE, NTRIP_THREAD_PRIORITY);
	if(ret < 0)
	{
		printf("task_ntrip create falied\n");
		return ret;
	}
	/*Create a task that is valid*/
	g_ntrip_control.task_ntrip_valid = 1;
	
	return ret;
}

/*****************************************************
func:ntrip_deinit
desc:deinit ntrip services
para:void
return:Less than 0 failed,Greater than or equal to 0 indicates success
*****************************************************/
int ntrip_deinit()
{
	int ret = ERR_OK;
	if(g_ntrip_control.task_ntrip_valid)
	{
		/*Delete NTRIP processing task*/
		ret = osal_task_del(g_ntrip_control.task_ntrip);
		if(ret < 0)
			goto error;

		g_ntrip_control.task_ntrip_valid = 0;
	}
	memset(&g_ntrip_config, 0, sizeof(g_ntrip_config));
	memset(&g_ntrip_control, 0, sizeof(g_ntrip_control));
	memset(g_ntrip_gga, 0, sizeof(g_ntrip_gga));
error:
	return ret;
}


