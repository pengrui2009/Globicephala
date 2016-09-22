/******************************************************************************
	��Ŀ����	��E600G�����ն���Ŀ
	�ļ�		��cv_drv_simulator.c
	����		��
	�汾		��0.1
	����		������
	��������	��2016-5-31
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
#include "cv_drv_simu.h"
#include "error.h"
/*************************************************
  �궨��
*************************************************/
#define SIMU_PORT			9000		//������Խӿ�
#define MAX_BUF_LEN			2048
#define NET_BLOCK			0           	//������ʽ
#define NET_NONBLOCK		0xff		//��������ʽ

#define BTYPE_MGMT          0
#define BTYPE_CNTL          1
#define BTYPE_DATA          2

#define SUBTYPE_BEACON      8
#define MACADDR_LENGTH		6

#define PROTO_TYPE_UDP		1
/*************************************************
  �ṹ��
*************************************************/
#ifdef __CHECKER__
#define __force __attribute__((force))
#define __bitwise __attribute__((bitwise))
#else
#define __force
#define __bitwise
#endif

typedef uint16_t __bitwise be16;
typedef uint16_t __bitwise le16;
typedef uint32_t __bitwise be32;
typedef uint32_t __bitwise le32;
typedef uint64_t __bitwise be64;
typedef uint64_t __bitwise le64;

typedef struct _frame_control {
#ifndef __LITTLE_ENDIAN
    uint16_t SubType:4;    /* MSDU subtype */
    uint16_t Type:2;        /* MSDU type */
    uint16_t Ver:2;        /* Protocol version */
    uint16_t Order:1;        /* Strict order expected */
    uint16_t Wep:1;        /* Wep data */
    uint16_t MoreData:1;    /* More data bit */
    uint16_t PwrMgmt:1;    /* Power management bit */
    uint16_t Retry:1;        /* Retry status bit */
    uint16_t MoreFrag:1;    /* More fragment bit */
    uint16_t FrDs:1;        /* From DS indication */
    uint16_t ToDs:1;        /* To DS indication */
#else
    uint16_t Ver:2;        /* Protocol version */
    uint16_t Type:2;        /* MSDU type */
    uint16_t SubType:4;    /* MSDU subtype */
    uint16_t ToDs:1;        /* To DS indication */
    uint16_t FrDs:1;        /* From DS indication */
    uint16_t MoreFrag:1;    /* More fragment bit */
    uint16_t Retry:1;        /* Retry status bit */
    uint16_t PwrMgmt:1;    /* Power management bit */
    uint16_t MoreData:1;    /* More data bit */
    uint16_t Wep:1;        /* Wep data */
    uint16_t Order:1;        /* Strict order expected */
#endif    /* !__BIG_ENDIAN */
} frame_control_t;

struct ieee80211_hdr {
	frame_control_t fc;
	le16 duration_id;
	uint8_t addr1[6];
	uint8_t addr2[6];
	uint8_t addr3[6];
	le16 seq_ctrl;
	/* followed by 'uint8_t addr4[6];' if ToDS and FromDS is set in data frame
	 */
} __attribute__ ((packed));

/*************************************************
  ����
*************************************************/
drv_simu_envar_t g_simu_envar;
static uint8_t BroadcastAddr[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
static uint8_t BssidForV2V[] = {0x00, 0x63, 0x73, 0x76, 0x32, 0x76};
static uint8_t BeaconFixedElement[] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* timestamp */
    /* Reseverd */
    0x64, 0x00, /* beacon interval */
    0x22, 0x04, /* CapabilityInfo: IBSS, short preamble, short slot time */
    0x00, 0x00,
};

#define IEEE80211_HDRLEN (sizeof(struct ieee80211_hdr))
#define MAC_BEACON_FIX_LENGTH (sizeof(BeaconFixedElement))

/*************************************************
 ���ܺ���
*************************************************/
/******************************************************************************
*	����:	net_ip_get
*	����:	��ȡIP
*	����:	ipaddr			-	IP��ַ(����),4�ֽ�
*	����:	= 0				-	�ɹ�
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
*	����:		port 			-	UDP�˿ں�
			timeout			-	���ͽ�������ģʽ�³�ʱʱ��
*	����:	>0				-	�ɹ�  ����UDP��socket��
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
		//sockaddr_udp.sin_addr.s_addr = INADDR_ANY;                         //INADDR_ANY ��ʾ���м��������������
		memcpy(&sockaddr_udp.sin_addr.s_addr, localip, IPADDR_LENGTH);
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
*	����:	sockfd 			-	UDP�˿ں�
			buf				-	���ݻ�����
			length			-	Ҫ�������ݳ���
			ip				-	Ŀ��IP��ַ,4�ֽ�
			port			-	Ŀ��˿ں�
*	����:	>0				-	�ɹ����������ֽ���
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
*	����:	sockfd 			-	UDP�˿ں�
			buf				-	���ݻ�����(����)
			max_length		-	���ݻ�������󳤶�
			length			-	ʵ�ʽ������ݳ���(����)
			ip				-	Դ��ַIP��ַ(����)
			port			-	Դ��ַ�˿ں�(����)
*	����:	0				-	�ɹ�
			-ERR_SYS		-	ϵͳ����
			-ERR_INVAL		-	�ӿڴ���
			-ERR_BUSY		-	�ѶϿ�

*	˵��:	��
******************************************************************************/
int net_udp_recv(int sockfd, uint8_t *buf, uint16_t max_length, uint32_t *length, uint8_t flag, uint8_t *ip, uint16_t *port)
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
			ret = -ERR_INVAL;
			break;
	}

	if((recvbytes != -ERR_SYS) && (recvbytes != 0))
	{
		memcpy(length, &recvbytes, 4);//�������ݳ���
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
*	����:	drv_simu_init
*	����:	��ʼ������ӿ�
*	����:
*	����:	=0				-	�ɹ�
			-ERR_SYS		-	ϵͳ����
			-ERR_INVAL		-	�ӿڴ���
			-ERR_BUSY		-	�ѶϿ�

*	˵��:	��
******************************************************************************/
int drv_simu_init(void)
{
	int ret;
	uint16_t timeout;
	drv_simu_envar_t *p_simu = &g_simu_envar;
	memset(p_simu,0,sizeof(drv_simu_envar_t));
	ret = net_ip_get("eth0", p_simu->local_ipaddr);
	if(ret < 0)
	{
		osal_printf("net_ip_get error ret=%d\n",ret);
		goto err;
	}
	p_simu->local_port = SIMU_PORT;
	timeout =0xFFFF;//��������
	ret = net_udp_init(p_simu->local_port, p_simu->local_ipaddr, timeout);
	if(ret < 0){
		osal_printf("net_udp_init error ret=%d\n",ret);
		goto err;
	}else{
		p_simu->monitor_sock = ret;
	}
	ret = 0;
err:
	return ret;
}
/******************************************************************************
*	����:	drv_simu_send
*	����:	����ӿڷ�������
*	����:		port 			-	UDP�˿ں�
			timeout			-	���ͽ�������ģʽ�³�ʱʱ��
*	����:	=0				-	�ɹ�
			-ERR_SYS		-	ϵͳ����
			-ERR_INVAL		-	�ӿڴ���
			-ERR_BUSY		-	�ѶϿ�

*	˵��:	��
******************************************************************************/
int drv_simu_send(wnet_txinfo_t *txinfo, uint8_t *pdata, uint32_t length)
{
    int ret;
    struct ieee80211_hdr *p_hdr_80211;
    uint8_t *pPayload;
    uint8_t ipaddr[4];
    uint16_t port;
    uint32_t timestamp;
    drv_simu_envar_t *p_simu = &g_simu_envar;
    port = htons(p_simu->local_port);
    memcpy(ipaddr, p_simu->local_ipaddr, IPADDR_LENGTH);
    ipaddr[3] = 0xFF;
    pPayload = pdata - MAC_BEACON_FIX_LENGTH;
    /* fill the beacon fixed element */
    timestamp = cv_ntohl(txinfo->timestamp);
    memcpy(BeaconFixedElement, &timestamp, sizeof(txinfo->timestamp));
    memcpy(pPayload, BeaconFixedElement, MAC_BEACON_FIX_LENGTH);

    /* fill the 802.11 header */
    p_hdr_80211 = (struct ieee80211_hdr *)(pPayload - IEEE80211_HDRLEN);
    memset(p_hdr_80211, 0, IEEE80211_HDRLEN);

    p_hdr_80211->fc.Type = BTYPE_MGMT;
    p_hdr_80211->fc.SubType = SUBTYPE_BEACON;

	memcpy(p_hdr_80211->addr1, BroadcastAddr, MACADDR_LENGTH);
	memcpy(p_hdr_80211->addr2, p_simu->local_ipaddr,IPADDR_LENGTH);
	memcpy(p_hdr_80211->addr2 + IPADDR_LENGTH, &port, MACADDR_LENGTH-IPADDR_LENGTH);
	memcpy(p_hdr_80211->addr3, BssidForV2V, MACADDR_LENGTH);

	if(p_simu->monitor_sock)
	{
		ret =  net_udp_send(p_simu->monitor_sock, (uint8_t *)p_hdr_80211, (length+MAC_BEACON_FIX_LENGTH+IEEE80211_HDRLEN), ipaddr, p_simu->local_port);
		if(ret < 0)
		{
			goto err;
		}
	}

    ret = 0;
err:
	return ret;
}
/******************************************************************************
*	����:	drv_simu_recv
*	����:	����ӿڽ�������
*	����:		buf 			-	���յ��ַ�(����)
			len			-	���յ��ַ�����(����)
*	����:	=0				-	�ɹ�
			-ERR_SYS		-	ϵͳ����
			-ERR_INVAL		-	�ӿڴ���
			-ERR_BUSY		-	�ѶϿ�

*	˵��:	��
******************************************************************************/
int drv_simu_recv(wnet_rxinfo_t *rxinfo,uint8_t *pdata, uint32_t *length)
{
	int ret;
	int sockfd;
	uint16_t port;
	uint8_t recvipaddr[IPADDR_LENGTH];
	uint32_t len;
	uint8_t buf[MAX_BUF_LEN] = { 0 };
	struct ieee80211_hdr *hdr;
	int hdr_len = sizeof(struct ieee80211_hdr);
	drv_simu_envar_t *p_simu = &g_simu_envar;
	sockfd = p_simu->monitor_sock;
	ret = net_udp_recv(sockfd, buf, MAX_BUF_LEN, &len, NET_BLOCK, recvipaddr, &port);
	if(ret < 0)
	{
		goto err;
	}
	hdr = (struct ieee80211_hdr *)buf;

	if ((memcmp(hdr->addr3, BssidForV2V, sizeof(BssidForV2V))==0) && (memcmp(recvipaddr, p_simu->local_ipaddr, IPADDR_LENGTH) != 0)){
		/* Parse the element of V2V */
		#define SKIP_LEN 14  /* TimestampLLC  + Reserve */
		uint8_t * pPayload;
		uint8_t ElementID, ElementLen;
		*length = len;
#if 0
			osal_printf("Received from V2x module!......................\r\n");
			monitor_buf_print(hdr, *length);
#endif

		pPayload = buf + hdr_len;
		*length -= hdr_len;
		//osal_printf("len3=%d,",*length);
		pPayload += SKIP_LEN; /* skip fixed element in the beacon */

		ElementID = *pPayload++;
		ElementLen = *pPayload++;
		/* obu frame */
		if (ElementID == 0xDD) {
		   /* move LLC header */
		   pPayload -= 8;
		   memcpy(pPayload, buf + hdr_len, 8);
		   /* reduce 'reserved' length */
		   *length -= 8;
		}
		/* rsu frame |80211header|Timestrmp|Reserved|LLC+Dsmp+rcp| */
		if(ElementID == 0xAA){
			pPayload -= 2;
			*length -= SKIP_LEN;
		}
		if(*length <= MAX_BUF_LEN)
		{
			memcpy(pdata, pPayload, *length);
			memcpy(rxinfo->src.mac.addr, hdr->addr2, MACADDR_LENGTH);
			//memcpy(rxinfo->src.udp.addr, ipaddr, IPADDR_LENGTH);
			//rxinfo->src.udp.port = port;
			//osal_printf("len6 = %d\n",*length);
			return 0;
		}
		else
		{
			osal_printf("nl80211: Received data overflowed for current buffer size len:%d.\n",ElementLen);
			return -1;
		}
	}
	else
	{
		//monitor_buf_print(hdr, len);
		return -1;
	}
err:
	return ret;
}
