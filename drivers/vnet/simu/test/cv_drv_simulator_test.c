/******************************************************************************
	项目名称	：E600G车载终端项目
	文件		：cv_drv_simulator_test.c
	描述		：
	版本		：0.1
	作者		：彭瑞
	创建日期	：2016-5-31
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cv_drv_simulator.h"
int main()
{
	int ret;
	u16 port,send_port;
	u16 max_len,recv_port;
	u32 recv_len;
	int sockfd;
	u16 timeout;
	u8 buf[] = "12345678";

	char send_ipaddr[] = "192.168.1.115";
	char recv_ipaddr[15];
	port = 8000;
	timeout = 0xFFFF;
	max_len =20;
	ret = net_udp_init(port, timeout);
	if(ret < 0)
	{
		goto err;
	}else{
		sockfd = ret;
	}
	send_port = 8000;
	ret = net_udp_send(sockfd, buf, strlen(buf), send_ipaddr, send_port);
	if(ret < 0)
	{
		goto err;
	}
	printf("success send %d bytes\n",ret);
	ret = net_udp_recv(sockfd, buf, max_len, &recv_len, NET_BLOCK, recv_ipaddr, &recv_port);
	if(ret < 0){
		goto err;
	}
	printf("recv %d bytes\n",recv_len);
	//printf("ipaddress:%s  port:%d buf:%s\n",inet_ntoa((recv_ipaddr[3]<<24)+(recv_ipaddr[2]<<16)+(recv_ipaddr[1]<<8) +recv_ipaddr[0]),recv_port,buf);
	//printf("ipaddress:%s  port:%d buf:%s\n",inet_ntoa(*(unsigned long *)recv_ipaddr) ,recv_port,buf);
	printf("ipaddress:%s  port:%d buf:%s\n",recv_ipaddr ,recv_port,buf);
	ret = 0;
err:
	return ret;
}
