/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : uart.c
 @brief  : This file provider the driver of uart.
 @author : pengrui
 @history:
           2016-08-30    pengrui      Created file
           2017-10-24    wangxianwen  Optimize the structure.
           ...
******************************************************************************/

#include <stdio.h>
#include <fcntl.h>						
#include <unistd.h>						
#include <termios.h>					
#include <string.h>						
#include <sys/ioctl.h>					
#include "uart.h"
#include "error.h"


/******************************************************************************
*	Function: comport_config
*	Descriptions: config the specific uart fd driver.
*	Paramters:
			fd		        -	the file descriptor.
			cfg_ptr			-	the config paramters pointer.
*	Return:
			= 0				-	success
			< 0				-	failed
*	Comments: 
******************************************************************************/
int comport_config (int fd, uart_config_st_ptr cfg_ptr)
{
	int   ret = ERR_OK;
	struct termios opt;


    /* Error detection. */
    if((fd == 0)||(cfg_ptr == NULL))
    {
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    /* Get the current options for the uart(Init the options). */ 
	if((tcgetattr(fd, &opt)) != 0)
	{
		ret = -ERR_SYS;
		goto ERR_EXIT;
	}
    
    /* Enable the receiver and set local mode. */
	opt.c_cflag |= (CLOCAL | CREAD);

    /* Set uart data bit number. */
    switch(cfg_ptr->ndata)
	{
	    case 5: { opt.c_cflag |= CS5; break; }
		case 6: { opt.c_cflag |= CS6; break; }
		case 7: { opt.c_cflag |= CS7; break; }
		case 8: { opt.c_cflag |= CS8; break; }
        
		default:{ ret = -ERR_INVAL;  
                  goto ERR_EXIT;     break; }
	}
    
    /* Set uart verify mode. */
	switch(cfg_ptr->verify)
    {
    	case COMPORT_VERIFY_ODD:
        {
            /* Enable parity bit and use odd mode. */
            opt.c_cflag |= PARENB;
            opt.c_cflag |= PARODD;

            /* Enable parity check for input control. */
    		opt.c_iflag |= INPCK;
            break;
        }
    	case COMPORT_VERIFY_EVEN:
        {
            /* Enable parity bit and use even mode. */
            opt.c_cflag |= PARENB;
            opt.c_cflag &= ~PARODD;

            /* Enable parity check for input control. */
            opt.c_iflag |= INPCK;
    		break;
        }
    	case COMPORT_VERIFY_NO:
        {
            /* Disable parity bit. */
            opt.c_cflag &= ~PARENB;
    		break;
        }
    	default:
        {
            /* Set error code. */
            ret = -ERR_INVAL;
            goto ERR_EXIT;
            break;
        }
	}

    /* Set uart bandrate. */
	switch(cfg_ptr->baud)
    {
		case 50:  { 
                    cfsetispeed(&opt, B50);
                    cfsetospeed(&opt, B50);  break; }
        case 110: {
                    cfsetispeed(&opt, B110);
                    cfsetospeed(&opt, B110); break; }
		case 150: {
        			cfsetispeed(&opt, B150);
        			cfsetospeed(&opt, B150); break; }
		case 300: {
        			cfsetispeed(&opt, B300);
        			cfsetospeed(&opt, B300); break; }
		case 600: {
        			cfsetispeed(&opt, B600);
        			cfsetospeed(&opt, B600); break; }            
		case 1200: {
        			cfsetispeed(&opt, B1200);
        			cfsetospeed(&opt, B1200); break; }
		case 2400: {
        			cfsetispeed(&opt, B2400);
        			cfsetospeed(&opt, B2400); break; }            
		case 4800: {
        			cfsetispeed(&opt, B4800);
        			cfsetospeed(&opt, B4800); break; }
		case 9600: {
        			cfsetispeed(&opt, B9600);
        			cfsetospeed(&opt, B9600); break; }            
		case 19200: {
        			cfsetispeed(&opt, B19200);
        			cfsetospeed(&opt, B19200); break; }
		case 38400: {
        			cfsetispeed(&opt, B38400);
        			cfsetospeed(&opt, B38400); break; }            
		case 57600: {
        			cfsetispeed(&opt, B57600);
        			cfsetospeed(&opt, B57600); break; }
		case 115200: {
        			cfsetispeed(&opt, B115200);
        			cfsetospeed(&opt, B115200); break; }    
		case 230400: {
        			cfsetispeed(&opt, B230400);
        			cfsetospeed(&opt, B230400); break; }    
		case 460800: {
        			cfsetispeed(&opt, B460800);
        			cfsetospeed(&opt, B460800); break; }    
		case 500000: {
        			cfsetispeed(&opt, B500000);
        			cfsetospeed(&opt, B500000); break; }    
		case 576000: {
        			cfsetispeed(&opt, B576000);
        			cfsetospeed(&opt, B576000); break; }    
		case 921600: {
        			cfsetispeed(&opt, B921600);
        			cfsetospeed(&opt, B921600); break; }    
		case 1000000: {
        			cfsetispeed(&opt, B1000000);
        			cfsetospeed(&opt, B1000000); break; }    
		case 1152000: {
        			cfsetispeed(&opt, B1152000);
        			cfsetospeed(&opt, B1152000); break; }    
		case 1500000: {
        			cfsetispeed(&opt, B1500000);
        			cfsetospeed(&opt, B1500000); break; }    
		case 2000000: {
        			cfsetispeed(&opt, B2000000);
        			cfsetospeed(&opt, B2000000); break; }    
		case 2500000: {
        			cfsetispeed(&opt, B2500000);
        			cfsetospeed(&opt, B2500000); break; }    
		case 3000000: {
        			cfsetispeed(&opt, B3000000);
        			cfsetospeed(&opt, B3000000); break; }    
		case 3500000: {
        			cfsetispeed(&opt, B3500000);
        			cfsetospeed(&opt, B3500000); break; }    
		case 4000000: {
        			cfsetispeed(&opt, B4000000);
        			cfsetospeed(&opt, B4000000); break; }    
		default: {
                    ret = -ERR_INVAL;
                    goto ERR_EXIT;             break;  }    
	}
    
    /* Set uart stop bit. */
    if(cfg_ptr->nstop == 1)
    {
        /* 1 stop bit. */
        opt.c_cflag &= ~CSTOPB;
    }
	else if (cfg_ptr->nstop == 2)
	{
	    /* 2 stop bits. */
	    opt.c_cflag |= CSTOPB;
	}	
	else
    {
        /* Set error code. */
		ret = -ERR_INVAL;
		goto ERR_EXIT;
	}
    
    /* Set uart receive timeout. */    
	if (cfg_ptr->timeout == 0)
	{
	    /* Set the uart into read block mode. */
	    fcntl(fd, F_SETFL, 0);

		opt.c_cc[VTIME] = 0;
		opt.c_cc[VMIN] 	= 1;
	}
	else if (cfg_ptr->timeout == 0xff)
	{
	    /* Set the uart into read noblock mode. */
        fcntl(fd, F_SETFL, FNDELAY);

        /* Have no effects when in noblock mode. */
		opt.c_cc[VTIME]	= 0;
		opt.c_cc[VMIN]	= 0;
	}
    else 
	{
		/* Set the uart into read block mode. */
	    fcntl(fd, F_SETFL, 0);

	    /* Waiting every character for "timeout" time when VMIN is 0. */
		opt.c_cc[VTIME]	= cfg_ptr->timeout;
		opt.c_cc[VMIN] 	= 0;
	}

    /* Set uart hardware flow control(not support for all platforms). */
	if (cfg_ptr->rtscts == COMPORT_RTSCTS_ENABLE)
	{
		opt.c_cflag |= CRTSCTS;
	}
    else if (cfg_ptr->rtscts == COMPORT_RTSCTS_DISABLE)
    {
		opt.c_cflag &= ~CRTSCTS;
	}
    else 
    {
		ret = -ERR_INVAL;
		goto ERR_EXIT;
	}
    
    /* Set the new options for the uart. */ 
	if((tcsetattr(fd, TCSANOW, &opt)) != 0)
	{
		ret = -ERR_SYS;
		goto ERR_EXIT;
	}
    
    /* Flush the input buffer data. */
	tcflush(fd, TCIFLUSH);

    ret = ERR_OK;
    
ERR_EXIT:
    
	return ret;
}


/******************************************************************************
*	Function: comport_send
*	Descriptions: Send data to the specific uart driver.
*	Paramters:
			fd		        -	the file descriptor.
			buff_ptr        -   buffer head address for data storage.
			data_len        -   the data count that user want to send.
*	Return:
			>= 0			-	the data count that have received.
			< 0				-	failed
*	Comments: 
******************************************************************************/
int comport_send (int fd, uint8_t *buff_ptr, uint32_t data_len)
{
	int ret = ERR_OK;


    /* Write the data to uart device. */
    ret = write(fd, buff_ptr, data_len);
	if(ret < 0)
	{
		ret = -ERR_SYS;
		goto ERR_EXIT;
	}
    
ERR_EXIT:
    
	return ret;
}


/******************************************************************************
*	Function: comport_receive
*	Descriptions: Receive data from the specific uart driver.
*	Paramters:
			fd		        -	the file descriptor.
			buff_ptr        -   buffer head address for data storage.
			data_len        -   the data count that user want to receive.
*	Return:
			>= 0			-	the data count that have received.
			< 0				-	failed
*	Comments: 
******************************************************************************/
int comport_receive (int fd, uint8_t *buff_ptr, uint32_t data_len)
{
	int ret = ERR_OK;


    /* Error detection. */
    if((buff_ptr == NULL) || (data_len == 0))
    {
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Read count data into buffer. */
	ret = read(fd, buff_ptr, data_len);
	if(ret == 0)
	{
		ret = -ERR_TIMEOUT;
        goto ERR_EXIT;
	}
	else if(ret < 0)
	{
		ret = -ERR_SYS;
        goto ERR_EXIT;
	}

ERR_EXIT:
    
	return ret;
}


/******************************************************************************
*	Function: comport_flush
*	Descriptions: Flush the specific uart buffer.
*	Paramters:
			fd		        -	the file descriptor.
			mode            -   buffer mode.
*	Return:
			= 0				-	success
			< 0				-	failed
*	Comments: 
******************************************************************************/
int comport_flush (int fd, comport_buff_em mode)
{
	int ret = -1;


    /* Flush the specific buffer. */
	if(mode == COMPORT_BUFF_ALL)
	{
	    ret = tcflush(fd, TCIOFLUSH);
	}
	else if(mode == COMPORT_BUFF_RD)
	{
	    ret = tcflush(fd, TCIFLUSH);
	}
	else if(mode == COMPORT_BUFF_WR)
	{
	    ret = tcflush(fd, TCOFLUSH);
	}	
	else
    {
		ret = -ERR_INVAL;
	}
    
	return ret;
}


/******************************************************************************
*	Function: comport_init
*	Descriptions: open the specific uart device.
*	Paramters:
            fd_ptr          -   the pointer that hold uart file descriptor.
			name_ptr	    -	the device character pointer.
			cfg_ptr			-	the config paramters pointer.
*	Return:
			= 0				-	success
			< 0				-	failed
*	Comments: 
******************************************************************************/
int comport_init (int *fd_ptr, const char *name_ptr, uart_config_st_ptr cfg_ptr)
{
	int   ret = ERR_OK;
	int         fd = 0;


    /* Error detection. */
    if((fd_ptr == NULL)||(name_ptr == NULL)||(cfg_ptr == NULL))
    {
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    /* Open the uart device. */
	if((fd = open(name_ptr, O_RDWR|O_NOCTTY|O_NDELAY)) < 0)
	{
		ret = -ERR_SYS;
		goto ERR_EXIT;
	}

    /* Return error when current uart is terminal device. */
	if(isatty(STDIN_FILENO) == 0)
	{
		ret = -ERR_SYS;
		goto ERR_EXIT;
	}

    /* Config the uart device. */
    if((ret = comport_config(fd, cfg_ptr)) != ERR_OK)
    {
        goto ERR_EXIT; 
    }

    /* Set the file descriptor. */
    *fd_ptr = fd;
    ret = ERR_OK;
    
    return ret;
    
ERR_EXIT:

    /* Deinit the device when init error. */
    if(fd != 0)
    {
        comport_deinit(fd);
    }
	return ret;
}


/******************************************************************************
*	Function: comport_deinit
*	Descriptions: Deinit the uart fd device.
*	Paramters:
			fd		        -	the file descriptor.
*	Return:
			= 0				-	success
			< 0				-	failed
*	Comments: 
******************************************************************************/
int comport_deinit(int fd)
{
    int ret = ERR_OK;

    
    if(close(fd) < 0)
    {
        ret = -ERR_INVAL;
    }
    else
    {
        ret = ERR_OK;
    }

    return ret;
}


