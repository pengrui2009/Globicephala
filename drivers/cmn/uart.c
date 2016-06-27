/******************************************************************************
	项目名称	：E600G车载终端项目
	文件		：uart.c
	描述		：
	版本		：0.1
	作者		：pengrui
	创建日期	：2016-6-13
******************************************************************************/

//C库头文件
#include <stdio.h>						//printf
#include <fcntl.h>						//open
#include <unistd.h>						//read,write
#include <termios.h>					//串口设置及宏定义
#include <string.h>						//bzero,memcyp
#include <sys/ioctl.h>					//ioctl

//提供给用户的头文件
#include "cv_ehm.h"
#include "uart.h"
#include "error.h"
/*************************************************
  结构类型定义
*************************************************/

/*************************************************
  静态全局变量定义
*************************************************/
static struct{
	int fd;
	uint8_t count;				//打开计数
} comport[DSTREAM_END];
/*************************************************
  API函数实现
*************************************************/
/*************************************************
  操作底层所用到的函数
*************************************************/

/******************************************************************************
*	函数:	comport_init
*	功能:	打开并初始化串口
*	参数:	无
*	返回:	0				-	成功
			-ERR_SYS		-	错误
			-ERR_INVAL		-	参数错误
			-ERR_BUSY		-	已经打开
*	说明:	无
 ******************************************************************************/
int comport_init (uint8_t port)
{
	int ret;
	struct termios opt;
	//ttyUSB0为usb虚拟串口设备
	char *dev[]={"/dev/ttyS0","/dev/ttyS1","/dev/ttyUSB0"};

	//已初始化
	if(comport[port].count == 1){
		ret = -ERR_BUSY;
		goto err;
	}
	//入口参数判断
	if(port < 0 || port >= DSTREAM_END){
		ret = -ERR_INVAL;
		goto err;
	}
	//打开串口
	comport[port].fd = open(dev[port], O_RDWR|O_NOCTTY|O_NDELAY);
	if (comport[port].fd < 0){
		ret = -ERR_SYS;
		goto err;
	}
	//恢复串口阻塞模式
	fcntl(comport[port].fd, F_SETFL,0);
	//测试是否为终端设备
	if(isatty(STDIN_FILENO)==0){
		ret = -ERR_SYS;
		goto err;
	}

	//配置默认串口属性

	memset(&opt, 0,sizeof(opt));

	opt.c_cflag |=  CLOCAL | CREAD;
	opt.c_cflag &= ~CSIZE;
	//设置数据位，默认8位
	opt.c_cflag |= CS8;
	//设置奇偶校验位，默认无
	opt.c_cflag &= ~PARENB;
	//设置波特率，默认115200
	cfsetispeed(&opt, B115200);
	cfsetospeed(&opt, B115200);
	//设置停止位，默认1位
	opt.c_cflag &=  ~CSTOPB;
	//设定超时时间，默认永久阻塞
	opt.c_cc[VTIME]  = 0;
	opt.c_cc[VMIN] = 1;
	//设置硬件流控，默认为无
	opt.c_cflag &= ~CRTSCTS;
	opt.c_iflag &= ~(IXON|IXOFF);
	//激活配置
	if((tcsetattr(comport[port].fd, TCSANOW, &opt)) != 0)	{
		ret = -ERR_SYS;
		goto err;
	}
	//处理未接收字符
	tcflush(comport[port].fd,TCIFLUSH);
	comport[port].count = 1;
	ret = 0;
err:
	return ret;
}

/******************************************************************************
*	函数:	comport_setconfig
*	功能:	配置串口
*	参数:
			cfg				-	配置项（数据传入）
*	返回:	0				-	成功
			-ESYS			-	系统错误
			-ERR_INVAL		-	参数无效
			-ERR_NOINIT		-	没有初始化
*	说明:	本串口支持的波特率为 50，110，150，300，600，1200，2400，4800，9600
 			115200，460800，4000000
 ******************************************************************************/
int comport_setconfig (uint8_t port,comport_config_t *cfg)
{
	int ret = -1;
	uint32_t baud;
	struct termios opt;
	memset(&opt, 0, sizeof(opt));

	if(port < 0 || port >= DSTREAM_END) {
		ret = -ERR_INVAL;
		goto err;
	}
	if(comport[port].count == 0) {
		ret = -ERR_NOINIT;		//没有初始化
		goto err;
	}

	opt.c_cflag |=  CLOCAL | CREAD;
	//设置数据位
	switch(cfg->ndata ){
		case 6:		//设置数据位6位
			opt.c_cflag |= CS6;
			break;
		case 7:		//设置数据位7位
			opt.c_cflag |= CS7;
			break;
		case 8:		//设置数据位8位
			opt.c_cflag |= CS8;
			break;
		default:
			return -ERR_INVAL;
			break;
	}
	//设置奇偶校验位
	switch(cfg->verify){
	case COMPORT_VERIFY_ODD://奇数
		opt.c_cflag |= PARENB;
		opt.c_cflag |= PARODD;
		opt.c_iflag |= INPCK;
		break;
	case COMPORT_VERIFY_EVEN: //偶数
		opt.c_iflag |= INPCK;
		opt.c_cflag |= PARENB;
		opt.c_cflag &= ~PARODD;
		break;
	case COMPORT_VERIFY_NO:  //无奇偶校验位
		opt.c_cflag &= ~PARENB;
		break;
	default:
		return -ERR_INVAL;
		break;
	}

	baud = cfg->baud;

	//设置波特率
	switch(baud){
		case 50:	//波特率
			cfsetispeed(&opt, B50);
			cfsetospeed(&opt, B50);
			break;
		case 110:	//波特率
			cfsetispeed(&opt, B110);
			cfsetospeed(&opt, B110);
			break;
		case 150:	//波特率
			cfsetispeed(&opt, B150);
			cfsetospeed(&opt, B150);
			break;
		case 300:	//波特率
			cfsetispeed(&opt, B300);
			cfsetospeed(&opt, B300);
			break;
		case 600:	//波特率
			cfsetispeed(&opt, B600);
			cfsetospeed(&opt, B600);
			break;
		case 1200:	//波特率
			cfsetispeed(&opt, B1200);
			cfsetospeed(&opt, B1200);
			break;
		case 2400:	//波特率
			cfsetispeed(&opt, B2400);
			cfsetospeed(&opt, B2400);
			break;
		case 4800:	//波特率
			cfsetispeed(&opt, B4800);
			cfsetospeed(&opt, B4800);
			break;
		case 9600:	//波特率
			cfsetispeed(&opt, B9600);
			cfsetospeed(&opt, B9600);
			break;
		case 19200:	//波特率
			cfsetispeed(&opt, B19200);
			cfsetospeed(&opt, B19200);
			break;
		case 38400:	//波特率
			cfsetispeed(&opt, B38400);
			cfsetospeed(&opt, B38400);
			break;
		case 57600:	//波特率
			cfsetispeed(&opt, B57600);
			cfsetospeed(&opt, B57600);
			break;
		case 115200:	//波特率
			cfsetispeed(&opt, B115200);
			cfsetospeed(&opt, B115200);
			break;
		case 230400:	//波特率
			cfsetispeed(&opt, B230400);
			cfsetospeed(&opt, B230400);
			break;
		case 460800:	//波特率
			cfsetispeed(&opt, B460800);
			cfsetospeed(&opt, B460800);
			break;
		case 500000:	//波特率
			cfsetispeed(&opt, B500000);
			cfsetospeed(&opt, B500000);
			break;
		case 576000:	//波特率
			cfsetispeed(&opt, B576000);
			cfsetospeed(&opt, B576000);
			break;
		case 921600:	//波特率
			cfsetispeed(&opt, B921600);
			cfsetospeed(&opt, B921600);
			break;
		case 1000000:	//波特率
			cfsetispeed(&opt, B1000000);
			cfsetospeed(&opt, B1000000);
			break;
		case 1152000:	//波特率
			cfsetispeed(&opt, B1152000);
			cfsetospeed(&opt, B1152000);
			break;

		case 1500000:	//波特率
			cfsetispeed(&opt, B1500000);
			cfsetospeed(&opt, B1500000);
			break;
		case 2000000:	//波特率
			cfsetispeed(&opt, B2000000);
			cfsetospeed(&opt, B2000000);
			break;
		case 2500000:	//波特率
			cfsetispeed(&opt, B2500000);
			cfsetospeed(&opt, B2500000);
			break;
		case 3000000:	//波特率
			cfsetispeed(&opt, B3000000);
			cfsetospeed(&opt, B3000000);
			break;
		case 3500000:	//波特率
			cfsetispeed(&opt, B3500000);
			cfsetospeed(&opt, B3500000);
			break;
		case 4000000:	//波特率
			cfsetispeed(&opt, B4000000);
			cfsetospeed(&opt, B4000000);
			break;
		default:
			ret = -ERR_INVAL;
			goto err;
			break;
	}
	//设置停止位
	if(cfg->nstop == 1)
		opt.c_cflag &=  ~CSTOPB;
	else if (cfg->nstop == 2)				//设置2位停止位
		opt.c_cflag |=  CSTOPB;
	else{
		ret = -ERR_INVAL;
		goto err;
	}
	//设定超时时间
	if( (cfg->timeout < 0) || (cfg->timeout > 255) ){
		ret = -ERR_INVAL;
		goto err;
	}
	if (cfg->timeout == 0) {
		//永久阻塞
		opt.c_cc[VTIME] 	= 0/*1*/;
		opt.c_cc[VMIN] 	= 1/*255*/;
	}
	else if (cfg->timeout == 0xff) {
		//不阻塞
		opt.c_cc[VTIME]	= 0;
		opt.c_cc[VMIN]	= 0;
	}
	else {
		//超时阻塞
		opt.c_cc[VTIME]	= cfg->timeout;
		opt.c_cc[VMIN] 	= 0;
	}
	//设置硬件流控
	if (cfg->rtscts == COMPORT_RTSCTS_ENABLE) {
		opt.c_cflag |= CRTSCTS;
		//usart_set_serial(port, COMPORT_RTSCTS_ENABLE);
	}
	else if (cfg->rtscts == COMPORT_RTSCTS_DISABLE) {
		opt.c_cflag &= ~CRTSCTS;
	}
	else {
		ret = -ERR_INVAL;
		goto err;
	}
	//激活配置
	if ((tcsetattr(comport[port].fd, TCSANOW, &opt)) != 0)	{
		ret = -ERR_SYS;
		goto err;
	}
	//处理未接收字符
	tcflush(comport[port].fd,TCIFLUSH);
//	memcpy(&com,cfg,sizeof(comport_config_t));

//	if (mode == COMPORT_MODE_7816) {
//		usart_set_7816(port, ATMEL_US_ISO7816_T0);
//	}

	ret = 0;
err:
	return ret;
}

/******************************************************************************
*	函数:	comport_send
*	功能:	串口发送
*	参数:
			buf				-	数据缓冲区（数据传入）
			count			-	请求字节数
*	返回:	>=0				-	发送成功的字节数
			-ESYS			-	系统错误
			-ERR_INVAL		-	参数无效
			-ERR_NOINIT		-	没有初始化
*	说明:	无
 ******************************************************************************/
int comport_send (uint8_t port,uint8_t *buf, uint32_t count)
{
	int ret = -1;

	if(port < 0 || port >= DSTREAM_END) {
			ret = -ERR_INVAL;
		goto err;
	}
	if(comport[port].count == 0) {
		ret = -ERR_NOINIT;		//没有初始化
		goto err;
	}

	ret = write(comport[port].fd, buf, count);
	if(ret < 0){
		ret = -ERR_SYS;
		goto err;
	}
err:
	return ret;
}

/******************************************************************************
*	函数:	comport_recv
*	功能:	串口接收
*	参数:
			buf				-	数据缓冲区（数据传出）
			count			-	请求字节数
*	返回:	>=0				-	接收成功的字节数
			-ERR_SYS		-	错误
			-ERR_INVAL		-	参数无效
			-ERR_NOINIT		-	没有初始化
*	说明:	无
 ******************************************************************************/
int comport_recv (uint8_t port,uint8_t *buf, uint32_t count)
{
	int ret = -1;

	if(port < 0 || port >= DSTREAM_END) {
			ret = -ERR_INVAL;
		goto err;
	}
	if(comport[port].count == 0) {
		ret = -ERR_NOINIT;		//没有初始化
		goto err;
	}

	ret = read(comport[port].fd, buf, count);
	if(ret == 0){
		ret = -ERR_TIMEOUT;
		goto err;
	}
	if(ret < 0){
		ret = -ERR_SYS;
		goto err;
	}

err:
	return ret;
}

/******************************************************************************
*	函数:	comport_flush
*	功能:	清串口缓冲区
*	参数:
			mode			-	清缓冲区模式
*	返回:	0				-	成功
			-ERR_INVAL		-	参数无效
			-ERR_NOINIT		-	没有初始化
*	说明:	无
 ******************************************************************************/
int comport_flush (uint8_t port,uint8_t mode)
{
	int ret = -1;

	if(mode == COMPORT_FLUSH_ALL)
		ret = tcflush(comport[port].fd,TCIOFLUSH);
	else if(mode == COMPORT_FLUSH_RD)
		ret = tcflush(comport[port].fd,TCIFLUSH);
	else if(mode == COMPORT_FLUSH_WR)
		ret = tcflush(comport[port].fd,TCOFLUSH);
	else{
		ret = -ERR_INVAL;
		goto err;
	}
	if(ret < 0){
		ret = -ERR_SYS;
		goto err;
	}
	ret = 0;
err:
	return ret;
}

/******************************************************************************
*	函数:	comport_close
*	功能:	关闭串口
*	参数:
*	返回:	0				-	成功
			-ERR_SYS		-	错误
			-ERR_INVAL		-	参数无效
			-ERR_NOINIT		-	没有初始化
*	说明:	无
 ******************************************************************************/
int comport_close (uint8_t port)
{
	int ret = -1;
	if(port < 0 || port >= DSTREAM_END) {
			ret = -ERR_INVAL;
		goto err;
	}
	if(comport[port].count == 0) {
		ret = -ERR_NOINIT;		//没有初始化
		goto err;
	}
	ret = close(comport[port].fd);
	if(ret < 0){
		ret = -ERR_SYS;
		goto err;
	}

	ret = 0;
err:
	return ret;
}
/******************************************************************************
*	函数:	cv_ehm_uart_init
*	功能:	关闭串口
*	参数:	port			-	串口端口号
*	返回:	0				-	成功
			-ERR_SYS		-	错误
			-ERR_INVAL		-	参数无效
			-ERR_NOINIT		-	没有初始化
*	说明:	无
 ******************************************************************************/
//串口初始化
//int cv_ehm_uart_init(ehm_envar_t *p_ehm)
//{
//	int ret;
//	comport_config_t *cfg;
//	ret = comport_init();
//	if(ret < 0 )
//	{
//		printf("comport_init error\n");
//		goto err;
//	}
//	cfg = &p_ehm->working_param.uart_cfg;
//	ret= comport_setconfig(cfg);
//	if(ret < 0 )
//	{
//		printf("comport_setconfig error\n");
//		goto err;
//	}
//	ret = 0;
//err:
//	return ret;
//}
////串口卸载
//int cv_ehm_uart_deinit()
//{
//	int ret;
//	ret = comport_close();
//	if(ret < 0)
//	{
//		printf("comport_init error\n");
//		goto err;
//	}
//err:
//	return ret;
//}
//dstream-0
static int dstream_0_open (void)
{
	int ret;

	ret = comport_init(DSTREAM_0);
	if(ret < 0)
	{
		goto err;
	}

	ret = 0;
err:
	return ret;
}

static int dstream_0_cfg (comport_config_t *cfg)
{
	int ret;

	ret = comport_setconfig(DSTREAM_0, cfg);
	if(ret < 0){
		goto error;
	}
	ret = 0;
error:
	return ret;
}

static int dstream_0_send (uint8_t *buf, uint32_t count)
{
	int ret;

	ret = comport_send(DSTREAM_0, buf, count);
	if(ret < 0){
		goto error;
	}
error:
	return ret;
}

static int dstream_0_recv (uint8_t *buf, uint32_t count)
{
	int ret;

	ret = comport_recv(DSTREAM_0, buf, count);
	if(ret < 0){
		goto error;
	}
error:
	return ret;
}

static int dstream_0_close (void)
{
	int ret;

	ret = comport_close(DSTREAM_0);
	if(ret < 0){
		goto error;
	}
	ret = 0;
error:
	return ret;
}

//dstream-1
static int dstream_1_open (void)
{
	int ret;

	ret = comport_init(DSTREAM_1);
	if(ret < 1)
	{
		goto err;
	}

	ret = 0;
err:
	return ret;
}

static int dstream_1_cfg (comport_config_t *cfg)
{
	int ret;

	ret = comport_setconfig(DSTREAM_1, cfg);
	if(ret < 1){
		goto error;
	}
	ret = 0;
error:
	return ret;
}

static int dstream_1_send (uint8_t *buf, uint32_t count)
{
	int ret;

	ret = comport_send(DSTREAM_1, buf, count);
	if(ret < 1){
		goto error;
	}
error:
	return ret;
}

static int dstream_1_recv (uint8_t *buf, uint32_t count)
{
	int ret;

	ret = comport_recv(DSTREAM_1, buf, count);
	if(ret < 1){
		goto error;
	}
error:
	return ret;
}

static int dstream_1_close (void)
{
	int ret;

	ret = comport_close(DSTREAM_1);
	if(ret < 1){
		goto error;
	}
	ret = 0;
error:
	return ret;
}

//dstream-2
static int dstream_2_open (void)
{
	int ret;

	ret = comport_init(DSTREAM_USBD);
	if(ret < 2)
	{
		goto err;
	}

	ret = 0;
err:
	return ret;
}

static int dstream_2_cfg (comport_config_t *cfg)
{
	int ret;

	ret = comport_setconfig(DSTREAM_USBD, cfg);
	if(ret < 2){
		goto error;
	}
	ret = 0;
error:
	return ret;
}

static int dstream_2_send (uint8_t *buf, uint32_t count)
{
	int ret;

	ret = comport_send(DSTREAM_USBD, buf, count);
	if(ret < 2){
		goto error;
	}
error:
	return ret;
}

static int dstream_2_recv (uint8_t *buf, uint32_t count)
{
	int ret;

	ret = comport_recv(DSTREAM_USBD, buf, count);
	if(ret < 2){
		goto error;
	}
error:
	return ret;
}

static int dstream_2_close (void)
{
	int ret;

	ret = comport_close(DSTREAM_USBD);
	if(ret < 2){
		goto error;
	}
	ret = 0;
error:
	return ret;
}

dstream_device_t dstream_device[DSTREAM_END] = {
	[DSTREAM_0] = {
			.open 	=	dstream_0_open,
			.config =	dstream_0_cfg,
			.send 	=	dstream_0_send,
			.recv   =	dstream_0_recv,
			.close	= 	dstream_0_close,
	},
	[DSTREAM_1] = {
			.open 	=	dstream_1_open,
			.config =	dstream_1_cfg,
			.send 	=	dstream_1_send,
			.recv   =	dstream_1_recv,
			.close	= 	dstream_1_close,
	},
	[DSTREAM_USBD] = {
			.open 	=	dstream_2_open,
			.config =	dstream_2_cfg,
			.send 	=	dstream_2_send,
			.recv   =	dstream_2_recv,
			.close	= 	dstream_2_close,
	},
};
