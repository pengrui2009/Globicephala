/******************************************************************************
	��Ŀ����	��E600G�����ն���Ŀ
	�ļ�		��uart.c
	����		��
	�汾		��0.1
	����		��pengrui
	��������	��2016-6-13
******************************************************************************/

//C��ͷ�ļ�
#include <stdio.h>						//printf
#include <fcntl.h>						//open
#include <unistd.h>						//read,write
#include <termios.h>					//�������ü��궨��
#include <string.h>						//bzero,memcyp
#include <sys/ioctl.h>					//ioctl

//�ṩ���û���ͷ�ļ�
#include "cv_ehm.h"
#include "uart.h"
#include "error.h"
/*************************************************
  �ṹ���Ͷ���
*************************************************/

/*************************************************
  ��̬ȫ�ֱ�������
*************************************************/
static struct{
	int fd;
	uint8_t count;				//�򿪼���
} comport[DSTREAM_END];
/*************************************************
  API����ʵ��
*************************************************/
/*************************************************
  �����ײ����õ��ĺ���
*************************************************/

/******************************************************************************
*	����:	comport_init
*	����:	�򿪲���ʼ������
*	����:	��
*	����:	0				-	�ɹ�
			-ERR_SYS		-	����
			-ERR_INVAL		-	��������
			-ERR_BUSY		-	�Ѿ���
*	˵��:	��
 ******************************************************************************/
int comport_init (uint8_t port)
{
	int ret;
	struct termios opt;
	//ttyUSB0Ϊusb���⴮���豸
	char *dev[]={"/dev/ttyS0","/dev/ttyS1","/dev/ttyUSB0"};

	//�ѳ�ʼ��
	if(comport[port].count == 1){
		ret = -ERR_BUSY;
		goto err;
	}
	//��ڲ����ж�
	if(port < 0 || port >= DSTREAM_END){
		ret = -ERR_INVAL;
		goto err;
	}
	//�򿪴���
	comport[port].fd = open(dev[port], O_RDWR|O_NOCTTY|O_NDELAY);
	if (comport[port].fd < 0){
		ret = -ERR_SYS;
		goto err;
	}
	//�ָ���������ģʽ
	fcntl(comport[port].fd, F_SETFL,0);
	//�����Ƿ�Ϊ�ն��豸
	if(isatty(STDIN_FILENO)==0){
		ret = -ERR_SYS;
		goto err;
	}

	//����Ĭ�ϴ�������

	memset(&opt, 0,sizeof(opt));

	opt.c_cflag |=  CLOCAL | CREAD;
	opt.c_cflag &= ~CSIZE;
	//��������λ��Ĭ��8λ
	opt.c_cflag |= CS8;
	//������żУ��λ��Ĭ����
	opt.c_cflag &= ~PARENB;
	//���ò����ʣ�Ĭ��115200
	cfsetispeed(&opt, B115200);
	cfsetospeed(&opt, B115200);
	//����ֹͣλ��Ĭ��1λ
	opt.c_cflag &=  ~CSTOPB;
	//�趨��ʱʱ�䣬Ĭ����������
	opt.c_cc[VTIME]  = 0;
	opt.c_cc[VMIN] = 1;
	//����Ӳ�����أ�Ĭ��Ϊ��
	opt.c_cflag &= ~CRTSCTS;
	opt.c_iflag &= ~(IXON|IXOFF);
	//��������
	if((tcsetattr(comport[port].fd, TCSANOW, &opt)) != 0)	{
		ret = -ERR_SYS;
		goto err;
	}
	//����δ�����ַ�
	tcflush(comport[port].fd,TCIFLUSH);
	comport[port].count = 1;
	ret = 0;
err:
	return ret;
}

/******************************************************************************
*	����:	comport_setconfig
*	����:	���ô���
*	����:
			cfg				-	��������ݴ��룩
*	����:	0				-	�ɹ�
			-ESYS			-	ϵͳ����
			-ERR_INVAL		-	������Ч
			-ERR_NOINIT		-	û�г�ʼ��
*	˵��:	������֧�ֵĲ�����Ϊ 50��110��150��300��600��1200��2400��4800��9600
 			115200��460800��4000000
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
		ret = -ERR_NOINIT;		//û�г�ʼ��
		goto err;
	}

	opt.c_cflag |=  CLOCAL | CREAD;
	//��������λ
	switch(cfg->ndata ){
		case 6:		//��������λ6λ
			opt.c_cflag |= CS6;
			break;
		case 7:		//��������λ7λ
			opt.c_cflag |= CS7;
			break;
		case 8:		//��������λ8λ
			opt.c_cflag |= CS8;
			break;
		default:
			return -ERR_INVAL;
			break;
	}
	//������żУ��λ
	switch(cfg->verify){
	case COMPORT_VERIFY_ODD://����
		opt.c_cflag |= PARENB;
		opt.c_cflag |= PARODD;
		opt.c_iflag |= INPCK;
		break;
	case COMPORT_VERIFY_EVEN: //ż��
		opt.c_iflag |= INPCK;
		opt.c_cflag |= PARENB;
		opt.c_cflag &= ~PARODD;
		break;
	case COMPORT_VERIFY_NO:  //����żУ��λ
		opt.c_cflag &= ~PARENB;
		break;
	default:
		return -ERR_INVAL;
		break;
	}

	baud = cfg->baud;

	//���ò�����
	switch(baud){
		case 50:	//������
			cfsetispeed(&opt, B50);
			cfsetospeed(&opt, B50);
			break;
		case 110:	//������
			cfsetispeed(&opt, B110);
			cfsetospeed(&opt, B110);
			break;
		case 150:	//������
			cfsetispeed(&opt, B150);
			cfsetospeed(&opt, B150);
			break;
		case 300:	//������
			cfsetispeed(&opt, B300);
			cfsetospeed(&opt, B300);
			break;
		case 600:	//������
			cfsetispeed(&opt, B600);
			cfsetospeed(&opt, B600);
			break;
		case 1200:	//������
			cfsetispeed(&opt, B1200);
			cfsetospeed(&opt, B1200);
			break;
		case 2400:	//������
			cfsetispeed(&opt, B2400);
			cfsetospeed(&opt, B2400);
			break;
		case 4800:	//������
			cfsetispeed(&opt, B4800);
			cfsetospeed(&opt, B4800);
			break;
		case 9600:	//������
			cfsetispeed(&opt, B9600);
			cfsetospeed(&opt, B9600);
			break;
		case 19200:	//������
			cfsetispeed(&opt, B19200);
			cfsetospeed(&opt, B19200);
			break;
		case 38400:	//������
			cfsetispeed(&opt, B38400);
			cfsetospeed(&opt, B38400);
			break;
		case 57600:	//������
			cfsetispeed(&opt, B57600);
			cfsetospeed(&opt, B57600);
			break;
		case 115200:	//������
			cfsetispeed(&opt, B115200);
			cfsetospeed(&opt, B115200);
			break;
		case 230400:	//������
			cfsetispeed(&opt, B230400);
			cfsetospeed(&opt, B230400);
			break;
		case 460800:	//������
			cfsetispeed(&opt, B460800);
			cfsetospeed(&opt, B460800);
			break;
		case 500000:	//������
			cfsetispeed(&opt, B500000);
			cfsetospeed(&opt, B500000);
			break;
		case 576000:	//������
			cfsetispeed(&opt, B576000);
			cfsetospeed(&opt, B576000);
			break;
		case 921600:	//������
			cfsetispeed(&opt, B921600);
			cfsetospeed(&opt, B921600);
			break;
		case 1000000:	//������
			cfsetispeed(&opt, B1000000);
			cfsetospeed(&opt, B1000000);
			break;
		case 1152000:	//������
			cfsetispeed(&opt, B1152000);
			cfsetospeed(&opt, B1152000);
			break;

		case 1500000:	//������
			cfsetispeed(&opt, B1500000);
			cfsetospeed(&opt, B1500000);
			break;
		case 2000000:	//������
			cfsetispeed(&opt, B2000000);
			cfsetospeed(&opt, B2000000);
			break;
		case 2500000:	//������
			cfsetispeed(&opt, B2500000);
			cfsetospeed(&opt, B2500000);
			break;
		case 3000000:	//������
			cfsetispeed(&opt, B3000000);
			cfsetospeed(&opt, B3000000);
			break;
		case 3500000:	//������
			cfsetispeed(&opt, B3500000);
			cfsetospeed(&opt, B3500000);
			break;
		case 4000000:	//������
			cfsetispeed(&opt, B4000000);
			cfsetospeed(&opt, B4000000);
			break;
		default:
			ret = -ERR_INVAL;
			goto err;
			break;
	}
	//����ֹͣλ
	if(cfg->nstop == 1)
		opt.c_cflag &=  ~CSTOPB;
	else if (cfg->nstop == 2)				//����2λֹͣλ
		opt.c_cflag |=  CSTOPB;
	else{
		ret = -ERR_INVAL;
		goto err;
	}
	//�趨��ʱʱ��
	if( (cfg->timeout < 0) || (cfg->timeout > 255) ){
		ret = -ERR_INVAL;
		goto err;
	}
	if (cfg->timeout == 0) {
		//��������
		opt.c_cc[VTIME] 	= 0/*1*/;
		opt.c_cc[VMIN] 	= 1/*255*/;
	}
	else if (cfg->timeout == 0xff) {
		//������
		opt.c_cc[VTIME]	= 0;
		opt.c_cc[VMIN]	= 0;
	}
	else {
		//��ʱ����
		opt.c_cc[VTIME]	= cfg->timeout;
		opt.c_cc[VMIN] 	= 0;
	}
	//����Ӳ������
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
	//��������
	if ((tcsetattr(comport[port].fd, TCSANOW, &opt)) != 0)	{
		ret = -ERR_SYS;
		goto err;
	}
	//����δ�����ַ�
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
*	����:	comport_send
*	����:	���ڷ���
*	����:
			buf				-	���ݻ����������ݴ��룩
			count			-	�����ֽ���
*	����:	>=0				-	���ͳɹ����ֽ���
			-ESYS			-	ϵͳ����
			-ERR_INVAL		-	������Ч
			-ERR_NOINIT		-	û�г�ʼ��
*	˵��:	��
 ******************************************************************************/
int comport_send (uint8_t port,uint8_t *buf, uint32_t count)
{
	int ret = -1;

	if(port < 0 || port >= DSTREAM_END) {
			ret = -ERR_INVAL;
		goto err;
	}
	if(comport[port].count == 0) {
		ret = -ERR_NOINIT;		//û�г�ʼ��
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
*	����:	comport_recv
*	����:	���ڽ���
*	����:
			buf				-	���ݻ����������ݴ�����
			count			-	�����ֽ���
*	����:	>=0				-	���ճɹ����ֽ���
			-ERR_SYS		-	����
			-ERR_INVAL		-	������Ч
			-ERR_NOINIT		-	û�г�ʼ��
*	˵��:	��
 ******************************************************************************/
int comport_recv (uint8_t port,uint8_t *buf, uint32_t count)
{
	int ret = -1;

	if(port < 0 || port >= DSTREAM_END) {
			ret = -ERR_INVAL;
		goto err;
	}
	if(comport[port].count == 0) {
		ret = -ERR_NOINIT;		//û�г�ʼ��
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
*	����:	comport_flush
*	����:	�崮�ڻ�����
*	����:
			mode			-	�建����ģʽ
*	����:	0				-	�ɹ�
			-ERR_INVAL		-	������Ч
			-ERR_NOINIT		-	û�г�ʼ��
*	˵��:	��
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
*	����:	comport_close
*	����:	�رմ���
*	����:
*	����:	0				-	�ɹ�
			-ERR_SYS		-	����
			-ERR_INVAL		-	������Ч
			-ERR_NOINIT		-	û�г�ʼ��
*	˵��:	��
 ******************************************************************************/
int comport_close (uint8_t port)
{
	int ret = -1;
	if(port < 0 || port >= DSTREAM_END) {
			ret = -ERR_INVAL;
		goto err;
	}
	if(comport[port].count == 0) {
		ret = -ERR_NOINIT;		//û�г�ʼ��
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
*	����:	cv_ehm_uart_init
*	����:	�رմ���
*	����:	port			-	���ڶ˿ں�
*	����:	0				-	�ɹ�
			-ERR_SYS		-	����
			-ERR_INVAL		-	������Ч
			-ERR_NOINIT		-	û�г�ʼ��
*	˵��:	��
 ******************************************************************************/
//���ڳ�ʼ��
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
////����ж��
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
