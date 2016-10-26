/******************************************************************************
	��Ŀ����	��E600G�����ն���Ŀ
	�ļ�		��net.c
	����		��
	�汾		��0.1
	����		��pengrui
	��������	��2016-10-25
******************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "cv_osal.h"
#include "cv_wnet.h"
#include "cv_vam.h"
#include "cv_osal_dbg.h"
#include "net.h"
#include "error.h"
/*************************************************
  �궨��
*************************************************/


/*************************************************
  �ṹ��
*************************************************/

/*************************************************
  ����
*************************************************/

/*************************************************
 ���ܺ���
*************************************************/
/******************************************************************************
*	����:	net_ip_get
*	����:	��ȡIP
*	����:
			ipaddr			-	IP��ַ(����),4�ֽ�
*	����:
			= 0				-	�ɹ�
			<0				-	ʧ��
*	˵��:	��ȡIP��ַ
******************************************************************************/
int net_ip_get(char *ifname,uint8_t *ipaddr)
{
	int fd;
	int ret = 0;
	struct ifreq ifr;
	struct sockaddr_in *our_ip;
//	char *interface = "eth0";
//	u32 nip = 0;
//	struct in_addr temp_addr;
	memset(&ifr, 0, sizeof(ifr));
	fd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
	if (fd == -ERR_SYS) {
		ret = -ERR_SYS;
		goto err;
	}
	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);        //���ýӿ�����
	if (ipaddr)
	{
		if (ioctl(fd, SIOCGIFADDR, &ifr))
		{
			ret = -ERR_SYS;
			goto err;
		}
		our_ip = (struct sockaddr_in *) &ifr.ifr_addr;
		//nip = our_ip->sin_addr.s_addr;
	}
	//temp_addr.s_addr = nip;
	//strcpy(ipaddr, inet_ntoa(temp_addr));
	memcpy( ipaddr, &(our_ip->sin_addr.s_addr), IPADDR_LENGTH);

	ret = 0;
err:
	close(fd);
	return ret;
}
/******************************************************************************
*	����:	net_udp_init
*	����:	UDP��ʼ��
*	����:
			port 			-	UDP�˿ں�
			timeout			-	���ͽ�������ģʽ�³�ʱʱ��
*	����:
			>0				-	�ɹ�  ����UDP��socket��
			-ERR_SYS		-	ϵͳ����
			-ERR_INVAL		-	�ӿڴ���
			-ERR_BUSY		-	�ѶϿ�

*	˵��:	��
******************************************************************************/
int net_udp_init(uint16_t port, uint8_t *localip,uint16_t timeout)
{
	int ret;
	int sockfd;
	struct sockaddr_in sockaddr_udp;
	int optval = 1;
	struct timeval tout;

	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -ERR_SYS)
	{
		ret = -ERR_SYS;
		goto error;
	}else
	{
		sockaddr_udp.sin_family = AF_INET;
		sockaddr_udp.sin_port = htons(port);
        sockaddr_udp.sin_addr.s_addr = htonl(INADDR_ANY);   //INADDR_ANY ��ʾ���м��������������
		//memcpy(&sockaddr_udp.sin_addr.s_addr, localip, IPADDR_LENGTH);
		memset(&(sockaddr_udp.sin_zero), 0, 8);

		//���ó�ʱ
		if (timeout != 0xffff)
		{
			//���������������ա����ͳ�ʱʱ��
			tout.tv_sec = timeout;
			tout.tv_usec = 0;
			//���÷��ͳ�ʱ
			ret = setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &tout, sizeof(tout));
			if (ret)
			{
				ret = -ERR_SYS;
				goto error;
			}
			//���ý��ճ�ʱ
			ret = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tout, sizeof(tout));
			if(ret)
			{
				ret = -ERR_SYS;
				goto error;
			}
		}

		//���ö˿ڸ���
		//if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,(char *)&optval,sizeof(optval)) !=  0)
		//���ö˿ڹ㲥
		if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST,(char *)&optval,sizeof(optval)) !=  0)
		{
			ret = -ERR_INVAL;
			goto error;
		}

		if (bind(sockfd, (struct sockaddr *)&sockaddr_udp, sizeof(struct sockaddr)) == -ERR_SYS)
		{
			ret = -ERR_INVAL;
			goto error;
		}
	}
	ret = sockfd;
error:
	return ret;
}

/******************************************************************************
*	����:	net_udp_send
*	����:	UDP��������
*	����:
			sockfd 			-	UDP�˿ں�
			buf				-	���ݻ�����
			length			-	Ҫ�������ݳ���
			ip				-	Ŀ��IP��ַ,4�ֽ�
			port			-	Ŀ��˿ں�
*	����:
			>0				-	�ɹ����������ֽ���
			-ERR_SYS		-	ϵͳ����
			-ERR_INVAL		-	�ӿڴ���
			-ERR_BUSY		-	�ѶϿ�

*	˵��:	��
******************************************************************************/
int net_udp_send(int sockfd, uint8_t *buf, uint16_t length, uint8_t *ip, uint16_t port)
{
	int ret;
	struct sockaddr_in dest_addr;	//Ҫ���͵ĵ�ַ

	if(length == 0)
	{
		ret = -ERR_INVAL;
		goto error;
	}

	//���ò���
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(port);
	//dest_addr.sin_addr.s_addr = inet_addr("255.255.255.255");
	memcpy(&dest_addr.sin_addr.s_addr , ip , 4);
	memset(&(dest_addr.sin_zero), 0, 8);
	ret = sendto(sockfd, buf, length, 0, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr));
	if(ret == -ERR_SYS)
	{
		if((errno == EPIPE) || (errno == EDESTADDRREQ) || (errno == EBADF))
		{
			ret = -ERR_DISCONNECT;
		}else
		{
			ret = -ERR_TIMEOUT;
		}
		goto error;
	}
	ret = 0;
error:
	return (ret);
}

/******************************************************************************
*	����:	net_udp_recv
*	����:	UDP��������
*	����:
			sockfd 			-	UDP�˿ں�
			buf				-	���ݻ�����(����)
			max_length		-	���ݻ�������󳤶�
			length			-	ʵ�ʽ������ݳ���(����)
			ip				-	Դ��ַIP��ַ(����)
			port			-	Դ��ַ�˿ں�(����)
*	����:
			0				-	�ɹ�
			-ERR_SYS		-	ϵͳ����
			-ERR_INVAL		-	�ӿڴ���
			-ERR_BUSY		-	�ѶϿ�

*	˵��:	��
******************************************************************************/
int net_udp_recv(int sockfd, uint8_t *buf, uint16_t max_length, uint16_t *length, uint8_t flag, uint8_t *ip, uint16_t *port)
{
	int ret, recvbytes;
	struct sockaddr_in dest_addr;
	socklen_t len;
	len = sizeof(struct sockaddr);
	switch(flag)
	{
		case NET_NONBLOCK:			//��������ʽ��������
			recvbytes = recvfrom(sockfd, buf, max_length, MSG_DONTWAIT, (struct sockaddr *)&dest_addr, &len);
			break;
		case NET_BLOCK:				//������ʽ��������
			recvbytes = recvfrom(sockfd, buf, max_length, 0, (struct sockaddr *)&dest_addr, &len);
			break;
		default:
			recvbytes = 0x0;
			ret = -ERR_INVAL;
			break;
	}

	if((recvbytes != -ERR_SYS) && (recvbytes != 0))
	{
		*length = recvbytes;//�������ݳ���
	}else if(recvbytes == 0)
	{
		ret = -ERR_DISCONNECT;
		goto error;
	}else
	{
		if(errno == EAGAIN)
		{
			ret = -ERR_TIMEOUT;
		}else
		{
			ret = -ERR_SYS;
		}
		goto error;
	}

 	*port = ntohl(dest_addr.sin_port);			//Դ��ַ�˿ں�
	//ipaddr = (char *)inet_ntoa(dest_addr.sin_addr.s_addr);
	memcpy(ip, &(dest_addr.sin_addr.s_addr), 4);//Դ��ַIP��ַ

	ret = 0;
error:
	return (ret);
}
/******************************************************************************
*	����:	net_udp_close
*	����:	�ر�udp����
*	����:
			sockfd 			-	UDP�˿ں�
*	����:
			��

*	˵��:	��
******************************************************************************/
void net_udp_close(int sockfd)
{
	close(sockfd);
}
