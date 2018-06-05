/******************************************************************************
	项目名称	：E600G车载终端项目
	文件		：cv_drv_simulator.h
	描述		：
	版本		：0.1
	作者		：彭瑞
	创建日期	：2016-5-31
******************************************************************************/

#ifndef CV_DRV_SIMULATOR_H_
#define CV_DRV_SIMULATOR_H_
/*************************************************
  宏定义
*************************************************/
//网络接收阻塞
#define NET_BLOCK				0           //阻塞方式
#define NET_NONBLOCK			0xff		//非阻塞方式
#define NET_LISTEN_SELECT		1           //服务器监听单线程实现
#define NET_LISTEN				0			//服务器监听多线程实现

#define MAX_ADDR_INFO_NUM		5
/*************************************************
  结构体
*************************************************/
typedef struct _simu_addr_info{
	list_head_t list;
	wnet_rxinfo_t addr_info;
	int heart_life;
	int alert_life;
}simu_addr_info_t;
typedef struct _drv_simu_envar {
    //drv_wifi_config_t working_param;
    //struct nl80211_state nl_state;
	//int dev_ifidx;
	uint8_t dev_flag;             /* 1: IFF_UP,  0:down */
	uint8_t local_ipaddr[IPADDR_LENGTH];
	uint16_t local_port;

	int monitor_sock;
//	int monitor_refcount;
} drv_simu_envar_t;


/*************************************************
  变量
*************************************************/
extern drv_simu_envar_t g_simu_envar;

/*************************************************
 功能函数
*************************************************/

/******************************************************************************
*	函数:	drv_simu_init
*	功能:	初始化仿真接口
*	参数:		port 			-	UDP端口号
			timeout			-	发送接收阻塞模式下超时时间
*	返回:	=0				-	成功
			-ERR_SYS		-	系统错误
			-ERR_INVAL		-	接口错误
			-ERR_BUSY		-	已断开

*	说明:	无
******************************************************************************/
int drv_simu_init(void);
/******************************************************************************
*	函数:	drv_simu_send
*	功能:	仿真接口发送数据
*	参数:		port 			-	UDP端口号
			timeout			-	发送接收阻塞模式下超时时间
*	返回:	=0				-	成功
			-ERR_SYS		-	系统错误
			-ERR_INVAL		-	接口错误
			-ERR_BUSY		-	已断开

*	说明:	无
******************************************************************************/
int drv_simu_send(wnet_txinfo_t *txinfo, uint8_t *pdata, uint32_t length);
/******************************************************************************
*	函数:	drv_simu_recv
*	功能:	仿真接口接收数据
*	参数:		buf 			-	接收到字符(返回)
			len			-	接收到字符长度(返回)
*	返回:	=0				-	成功
			-ERR_SYS		-	系统错误
			-ERR_INVAL		-	接口错误
			-ERR_BUSY		-	已断开

*	说明:	无
******************************************************************************/
int drv_simu_recv(wnet_rxinfo_t *rxinfo,uint8_t *pdata, uint32_t *length);
/******************************************************************************
*	函数:	net_ip_get
*	功能:	获取IP
*	参数:	ipaddr			-	IP地址(返回),4字节
*	返回:	无
*	说明:	自动申请IP地址后获取IP地址
******************************************************************************/
#endif /* CV_DRV_SIMULATOR_H_ */
