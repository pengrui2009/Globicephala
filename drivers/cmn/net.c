/******************************************************************************
	项目名称	：E600G车载终端项目
	文件		：net.c
	描述		：
	版本		：0.1
	作者		：pengrui
	创建日期	：2016-10-25
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
  宏定义
*************************************************/


/*************************************************
  结构体
*************************************************/

/*************************************************
  变量
*************************************************/

/*************************************************
 功能函数
*************************************************/
/******************************************************************************
*	函数:	net_ip_get
*	功能:	获取IP
*	参数:
			ipaddr			-	IP地址(返回),4字节
*	返回:
			= 0				-	成功
			<0				-	失败
*	说明:	获取IP地址
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
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);        //配置接口属性
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
*	函数:	net_udp_init
*	功能:	UDP初始化
*	参数:
			port 			-	UDP端口号
			timeout			-	发送接收阻塞模式下超时时间
*	返回:
			>0				-	成功  返回UDP的socket号
			-ERR_SYS		-	系统错误
			-ERR_INVAL		-	接口错误
			-ERR_BUSY		-	已断开

*	说明:	无
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
        sockaddr_udp.sin_addr.s_addr = htonl(INADDR_ANY);   //INADDR_ANY 表示所有计算机都能连过来
		//memcpy(&sockaddr_udp.sin_addr.s_addr, localip, IPADDR_LENGTH);
		memset(&(sockaddr_udp.sin_zero), 0, 8);

		//设置超时
		if (timeout != 0xffff)
		{
			//设置网络阻塞接收、发送超时时间
			tout.tv_sec = timeout;
			tout.tv_usec = 0;
			//设置发送超时
			ret = setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &tout, sizeof(tout));
			if (ret)
			{
				ret = -ERR_SYS;
				goto error;
			}
			//设置接收超时
			ret = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tout, sizeof(tout));
			if(ret)
			{
				ret = -ERR_SYS;
				goto error;
			}
		}

		//设置端口复用
		//if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,(char *)&optval,sizeof(optval)) !=  0)
		//设置端口广播
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
*	函数:	net_udp_send
*	功能:	UDP发送数据
*	参数:
			sockfd 			-	UDP端口号
			buf				-	数据缓冲区
			length			-	要发送数据长度
			ip				-	目标IP地址,4字节
			port			-	目标端口号
*	返回:
			>0				-	成功发送数据字节数
			-ERR_SYS		-	系统错误
			-ERR_INVAL		-	接口错误
			-ERR_BUSY		-	已断开

*	说明:	无
******************************************************************************/
int net_udp_send(int sockfd, uint8_t *buf, uint16_t length, uint8_t *ip, uint16_t port)
{
	int ret;
	struct sockaddr_in dest_addr;	//要发送的地址

	if(length == 0)
	{
		ret = -ERR_INVAL;
		goto error;
	}

	//设置参数
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
*	函数:	net_udp_recv
*	功能:	UDP接收数据
*	参数:
			sockfd 			-	UDP端口号
			buf				-	数据缓冲区(返回)
			max_length		-	数据缓冲区最大长度
			length			-	实际接收数据长度(返回)
			ip				-	源地址IP地址(返回)
			port			-	源地址端口号(返回)
*	返回:
			0				-	成功
			-ERR_SYS		-	系统错误
			-ERR_INVAL		-	接口错误
			-ERR_BUSY		-	已断开

*	说明:	无
******************************************************************************/
int net_udp_recv(int sockfd, uint8_t *buf, uint16_t max_length, uint16_t *length, uint8_t flag, uint8_t *ip, uint16_t *port)
{
	int ret, recvbytes;
	struct sockaddr_in dest_addr;
	socklen_t len;
	len = sizeof(struct sockaddr);
	switch(flag)
	{
		case NET_NONBLOCK:			//非阻塞方式接收数据
			recvbytes = recvfrom(sockfd, buf, max_length, MSG_DONTWAIT, (struct sockaddr *)&dest_addr, &len);
			break;
		case NET_BLOCK:				//阻塞方式接收数据
			recvbytes = recvfrom(sockfd, buf, max_length, 0, (struct sockaddr *)&dest_addr, &len);
			break;
		default:
			recvbytes = 0x0;
			ret = -ERR_INVAL;
			break;
	}

	if((recvbytes != -ERR_SYS) && (recvbytes != 0))
	{
		*length = recvbytes;//接收数据长度
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

 	*port = ntohl(dest_addr.sin_port);			//源地址端口号
	//ipaddr = (char *)inet_ntoa(dest_addr.sin_addr.s_addr);
	memcpy(ip, &(dest_addr.sin_addr.s_addr), 4);//源地址IP地址

	ret = 0;
error:
	return (ret);
}
/******************************************************************************
*	函数:	net_udp_close
*	功能:	关闭udp连接
*	参数:
			sockfd 			-	UDP端口号
*	返回:
			无

*	说明:	无
******************************************************************************/
void net_udp_close(int sockfd)
{
	close(sockfd);
}
