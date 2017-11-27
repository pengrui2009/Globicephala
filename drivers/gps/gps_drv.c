/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : gps_drv.c
 @brief  : gps api drvier
 @author : wanghechao
 @history:
           2017-9-28      wanghechao    Created file
           ...
******************************************************************************/
#include <stdlib.h>
#include <stdint.h>
#include <memory.h>
#include <osal_thread.h>
#include <osal_queue.h>
#include <error.h>
#include <osal_sem.h>
#include <uart.h>
#include <os_core.h>

#include "gps_drv.h"
#include "ublox.h"


#define    RED_COLOR    "\033[1;31;40m"  /*change the color of print character*/
#define DEFAULT_COLOR    "\033[0m" /*recovery the color of print character*/


static gps_config_t g_gps_config;  /*the global value of gps config*/
static gps_deal_handle_t g_gps_deal_handle; /*the global value of gps deal handle*/


/*****************************************************
func:gps_read_data
desc:read gps data
para:the fd of gps uart
return:NULL
*****************************************************/
static void gps_read_data(int port)
{
    uint8_t tmp = 0; // 
    t_gps_buff gps_buf;
    
    while(1)
    {
        //接收数据
        if(comport_receive(port, &tmp, 1) == 1)
        {
            switch(gps_buf.PpBuf[gps_buf.Pipe].Flag)
            {
                case 0:
                    if(tmp == '$')  /*NMEA frame head*/
                    {
                        memset(gps_buf.PpBuf[gps_buf.Pipe].Buf, 0x0, GPS_BUFF_SIZE);
                        gps_buf.PpBuf[gps_buf.Pipe].Len = 1;
                        gps_buf.PpBuf[gps_buf.Pipe].Buf[0] = '$';
                        gps_buf.PpBuf[gps_buf.Pipe].Flag = 1;
                    }
                    else if(tmp == 0xB5)  /*UBX frame head*/
                    {
                        memset(gps_buf.PpBuf[gps_buf.Pipe].Buf, 0x0, GPS_BUFF_SIZE);
                        gps_buf.PpBuf[gps_buf.Pipe].Len = 1;
                        gps_buf.PpBuf[gps_buf.Pipe].Buf[0] = tmp;
                        gps_buf.PpBuf[gps_buf.Pipe].Flag = 2;
                    }
                    else if(tmp == 0xD3)  /*RTCM frame head*/
                    {
                        memset(gps_buf.PpBuf[gps_buf.Pipe].Buf, 0x0, GPS_BUFF_SIZE);
                        gps_buf.PpBuf[gps_buf.Pipe].Len = 1;
                        gps_buf.PpBuf[gps_buf.Pipe].Buf[0] = tmp;
                        gps_buf.PpBuf[gps_buf.Pipe].Flag = 3;
                    }
                    break;
                case 1: /*NMEA frame body*/
                    gps_buf.PpBuf[gps_buf.Pipe].Buf[gps_buf.PpBuf[gps_buf.Pipe].Len] = tmp;
                    gps_buf.PpBuf[gps_buf.Pipe].Len++;
                    gps_buf.PpBuf[gps_buf.Pipe].Len %= GPS_BUFF_SIZE;
                    
                    if(tmp == '\n')
                    {
                        gps_buf.PpBuf[gps_buf.Pipe].Flag = 0;
                        if(gps_buf.PpBuf[gps_buf.Pipe].Len)
                        {   
                            //发送数据消息,callback function
                            if(g_gps_config.gps_deal_callback)
                            {
                                (g_gps_config.gps_deal_callback)(gps_buf.PpBuf[gps_buf.Pipe].Buf, gps_buf.PpBuf[gps_buf.Pipe].Len);
                            }
                            else
                                printf("g_gps_config.gps_deal_callback error\n");
                        }
                        gps_buf.Pipe++;
                        gps_buf.Pipe %= GPS_PIPE;
                    }
                    break;
                case 2: /*UBX frame body*/
                    gps_buf.PpBuf[gps_buf.Pipe].Buf[gps_buf.PpBuf[gps_buf.Pipe].Len] = tmp;
                    gps_buf.PpBuf[gps_buf.Pipe].Len++;
                    gps_buf.PpBuf[gps_buf.Pipe].Len %= GPS_BUFF_SIZE;
                    if((gps_buf.PpBuf[gps_buf.Pipe].Len >= 6)&&(gps_buf.PpBuf[gps_buf.Pipe].Len == (((gps_buf.PpBuf[gps_buf.Pipe].Buf[4])|(gps_buf.PpBuf[gps_buf.Pipe].Buf[5]<<8))+8)))
                    {
                        gps_buf.PpBuf[gps_buf.Pipe].Flag = 0;
                        if(gps_buf.PpBuf[gps_buf.Pipe].Len)
                        {   
                            int ret = ERR_OK;
                            //发送数据消息,add data to ubx queue
                            if(g_gps_deal_handle.gps_ubx_queue_valid)
                            {
                                ret = osal_queue_send(g_gps_deal_handle.gps_ubx_queue, gps_buf.PpBuf[gps_buf.Pipe].Buf, gps_buf.PpBuf[gps_buf.Pipe].Len, 0, OSAL_NO_WAIT);
                                if (ret != ERR_OK)
                                {
                                    printf("[%s][%d]osal_queue_send error\n",__FUNCTION__,__LINE__);
                                }
                            }
                        }
                        gps_buf.Pipe++;
                        gps_buf.Pipe %= GPS_PIPE;
                    }
                    break;
                case 3: /*RTCM frame body*/
                    gps_buf.PpBuf[gps_buf.Pipe].Buf[gps_buf.PpBuf[gps_buf.Pipe].Len] = tmp;
                    gps_buf.PpBuf[gps_buf.Pipe].Len++;
                    gps_buf.PpBuf[gps_buf.Pipe].Len %= GPS_BUFF_SIZE;
                    if((gps_buf.PpBuf[gps_buf.Pipe].Len >= 3)&&(gps_buf.PpBuf[gps_buf.Pipe].Len == (((gps_buf.PpBuf[gps_buf.Pipe].Buf[2])|(gps_buf.PpBuf[gps_buf.Pipe].Buf[1]<<8))+6)))
                    {
                        gps_buf.PpBuf[gps_buf.Pipe].Flag = 0;
                        if(gps_buf.PpBuf[gps_buf.Pipe].Len)
                        {   
                            //发送数据消息,callback function
                            if(g_gps_config.gps_rtcm_callback)
                            {
                                (g_gps_config.gps_rtcm_callback)(gps_buf.PpBuf[gps_buf.Pipe].Buf, gps_buf.PpBuf[gps_buf.Pipe].Len);
                            }
                            else
                                printf("g_gps_config.gps_rtcm_callback error\n");
                        }
                        gps_buf.Pipe++;
                        gps_buf.Pipe %= GPS_PIPE;
                    }
                    break;
                default:
                    break;
            }
        }
        else
        {
            printf("gps data recv error\n");
            usleep(200000);
            break;
        }
    }
}

/*****************************************************
func:ubx_nav_relposned
desc:deal the relposned of ubx,only for debug
para:buff is the data,len is the length of data
return:NULL
*****************************************************/
void ubx_nav_relposned(uint8_t *buff, uint32_t len)
{
    double relPosN,relPosE,relPosD,relPosHPN,relPosHPE,relPosHPD,accN,accE,accD;
    relPosN = (buff[8]|(buff[9]<<8)|(buff[10]<<16)|(buff[11]<<24));
    relPosN /= 100;
    relPosE = (buff[12]|(buff[13]<<8)|(buff[14]<<16)|(buff[15]<<24));
    relPosE /= 100;
    relPosD = (buff[16]|(buff[17]<<8)|(buff[18]<<16)|(buff[19]<<24));
    relPosD /= 100;
    relPosHPN = buff[20];
    relPosHPN /= 10;
    relPosHPE = buff[21];
    relPosHPE /= 10;
    relPosHPD = buff[22];
    relPosHPD /= 10;
    accN = (buff[24]|(buff[25]<<8)|(buff[26]<<16)|(buff[27]<<24));
    accN /= 10000;
    accE = (buff[28]|(buff[29]<<8)|(buff[30]<<16)|(buff[31]<<24));
    accE /= 10000;
    accD = (buff[32]|(buff[33]<<8)|(buff[34]<<16)|(buff[35]<<24));
    accD /= 10000;
    printf("[ROVER][refstationId]:%d\n",buff[2]|(buff[3]<<8));
    printf("[ROVER][relPosN]:%s%f(m)%s\n",RED_COLOR,relPosN, DEFAULT_COLOR);
    printf("[ROVER][relPosE]:%s%f(m)%s\n",RED_COLOR,relPosE, DEFAULT_COLOR);
    printf("[ROVER][relPosD]:%s%f(m)%s\n",RED_COLOR,relPosD, DEFAULT_COLOR);
    //PRINTF("[relPosHPN]:%f(mm)\n",relPosHPN);
    //PRINTF("[relPosHPE]:%f(mm)\n",relPosHPE);
    //PRINTF("[relPosHPD]:%f(mm)\n",relPosHPD);
    printf("[ROVER][accN]:%s%f(m)%s\n",RED_COLOR,accN, DEFAULT_COLOR);
    printf("[ROVER][accE]:%s%f(m)%s\n",RED_COLOR,accE, DEFAULT_COLOR);
    printf("[ROVER][accD]:%s%f(m)%s\n",RED_COLOR,accD, DEFAULT_COLOR);
    //PRINTF("[gnssFixOk]:%d\n",buff[36]&0x01);
    //PRINTF("[diffSoln]:%d\n",(buff[36]>>1)&0x01);
    //PRINTF("[relPosValid]:%d\n",(buff[36]>>2)&0x01);
    switch((buff[36]>>3)&0x03)
    {
        case 0:
            printf("[ROVER][carrSoln]:%s***No carrier phase range solution***%s\n",RED_COLOR, DEFAULT_COLOR);
            break;
        case 1:
            printf("[ROVER][carrSoln]:%s***Float solution***%s\n",RED_COLOR, DEFAULT_COLOR);
            break;
        case 2:
            printf("[ROVER][carrSoln]:%s***Fixed solution***%s\n",RED_COLOR, DEFAULT_COLOR);
            break;
        default:
            break;
    }
    //PRINTF("[carrSoln]:%d(0:No carrier phase range solution,1:Float solution,2:Fixed solution)\n",(buff[36]>>2)&0x03);
}

/*****************************************************
func:ubx_nav_svin
desc:deal the svin of ubx,only for debug
para:buff is the data,len is the length of data
return:NULL
*****************************************************/
void ubx_nav_svin(uint8_t *buff, uint32_t len)
{
    double accu = 0;
    static uint8_t print_flag = 0;
    accu = (buff[28]|(buff[29]<<8)|(buff[30]<<16)|(buff[31]<<24));
    accu /= 10000;
    if(!print_flag)
    {
        printf("survey-in observation time:[%d(s)],current position accuracy:[%f(m)]\n",buff[8]|(buff[9]<<8)|(buff[10]<<16)|(buff[11]<<24),accu);
        if(buff[36] == 1)
        {
            printf("[%ssuccessful finished%s]\n",RED_COLOR, DEFAULT_COLOR);
            print_flag = 1;
        }
        //PRINTF("[Survey-in position validity flag]:%d(1 = valid)\n",buff[36]);
        if(buff[37] == 1)
            printf("[%sin progress%s]\n",RED_COLOR, DEFAULT_COLOR);
    }
    
    //PRINTF("[Survey-in in progress flag]:%d(1 = in-progress)\n",buff[37]);
}

/*****************************************************
func:ubx_rxm_rtcm
desc:deal the rxm rtcm of ubx,only for debug
para:buff is the data,len is the length of data
return:NULL
*****************************************************/
void ubx_rxm_rtcm(uint8_t *buff, uint32_t len)
{
    printf("[ROVER][RTCM Message Type]:%d  ", buff[6]|(buff[7]<<8));
    if(buff[1]%0x01 == 1)
        printf("***CRC check failed***\n");
    else
        printf("***CRC check passed***\n");
}

/*****************************************************
func:ubx_rxm_rtcm
desc:deal the rxm rtcm of ubx,only for debug
para:buff is the data,len is the length of data
return:NULL
*****************************************************/
void ubx_nav_timegps(uint8_t *buff, uint32_t len)
{
    uint8_t gps_leap_second_flag = 0;;
    if(buff[11]&0x04)
    {
        printf("[TIMEGPS-LEAPSECOND][valid]:%d\n", buff[10]);
        gps_leap_second_flag = 1;
        ubx_nav_timegps_off(&g_gps_config);
    }
    else
    {
        gps_leap_second_flag = 0;
        printf("[TIMEGPS-LEAPSECOND][invalid]\n");
    }
    if(g_gps_config.gps_ublox_leapsecond_callback)
        (g_gps_config.gps_ublox_leapsecond_callback)(&gps_leap_second_flag, 1);
    else
        printf("gps_ublox_leapsecond_callback is invalid\n");
}

/*****************************************************
func:ubx_parse_main
desc:parse ubx data
para:buff is the data,len is the length of data
return:NULL
*****************************************************/
void ubx_parse_main(uint8_t *buff, uint32_t len)
{
    uint8_t ckA,ckB;
    if((buff[0] == UBX_SYNC_CHAR1)&&(buff[1] == UBX_SYNC_CHAR2))
    {
        ubx_pkt_checknum_calc(&buff[2], len-4, &ckA, &ckB);
        //printf("ckA:0x%02x,ckB:0x%02x,buff[len-2]:0x%02x,buff[len-1]:0x%02x",ckA,ckB,buff[len-2],buff[len-1]);
        if((ckA == buff[len-2])&&(ckB == buff[len-1]))
        {
            //PRINTF("[ubx] class:0x%02x,ID:0x%02x\n",buff[2],buff[3]);
            if((buff[2] == 0x01)&&(buff[3] == 0x3c))
            {
                ubx_nav_relposned(&buff[6], len-8);
            }
            else if((buff[2] == 0x01)&&(buff[3] == 0x3b))
            {
                ubx_nav_svin(&buff[6], len-8);
            }
            else if((buff[2] == 0x02)&&(buff[3] == 0x32))
            {
                ubx_rxm_rtcm(&buff[6], len-8);
            }
            else if((buff[2] == 0x01)&&(buff[3] == 0x20))
            {
                ubx_nav_timegps(&buff[6], len-8);
            }
        }
        else
        {
            printf("ubx checksum error\n");
        }
    }
    else
    {
        printf("Invalid ubx data\n");
    }    
}

/*****************************************************
func:gps_thread_entry
desc:gps read data thread
para:parameter is the fd of gps uart
return:NULL
*****************************************************/
void * gps_thread_entry(void *parameter)
{
    int com_fd = *((int*)parameter);
    
    if(com_fd <= 0)
    {
        printf("[%s][%d] com_fd error\n",__FUNCTION__,__LINE__);
        goto error;
    }

    while(1) /*recv gps data*/
    {
        gps_read_data(g_gps_config.port);
    }
error:
    return NULL;
}

/*****************************************************
func:gps_ubx_thread_entry
desc:deal gps ubx data thread
para:parameter is not used
return:NULL
*****************************************************/
void * gps_ubx_thread_entry(void *parameter)
{
    int ret = ERR_OK;
    uint32_t len = 0;
    uint8_t buf[GPS_UBX_QUEUE_MSG_SIZE];
   
    while(1){
        memset(buf, 0, GPS_UBX_QUEUE_MSG_SIZE);
        /*receive ubx queue data*/
        if(g_gps_deal_handle.gps_ubx_queue_valid == 0)
        {
            printf("gps_ubx_queue invalid\n");
            usleep(10*1000);
            continue;
        }
        ret = osal_queue_recv(g_gps_deal_handle.gps_ubx_queue, buf, &len, OSAL_BLOCK);
        if (ret == ERR_OK && len > 0){
            /*deal ubx data*/
            ubx_parse_main(buf, len);
        }
        else{
            printf("[%s][%d] osal_queue_recv error\n",__FUNCTION__,__LINE__);    
        }
    }
    return NULL;
}

/*****************************************************
func:drv_gps_init
desc:init gps device
para:gps_config is the config of gps
return:ERR_OK is successful, other value is failed
*****************************************************/
int drv_gps_init(gps_config_t gps_config)
{
    int ret = ERR_OK;
    uart_config_st gps_uarts_cfg;

    if((g_gps_deal_handle.task_gps_valid)&&(g_gps_deal_handle.task_ubx_valid))
    {
        printf("drv_gps_init has been init\n");
        return ret;
    }
    memset(&g_gps_config, 0, sizeof(gps_config_t));
    memcpy(&g_gps_config, &gps_config, sizeof(gps_config_t));

    memset(&g_gps_deal_handle, 0, sizeof(gps_deal_handle_t));

    /*init gps uart para*/
    gps_uarts_cfg.baud = 9600;
    gps_uarts_cfg.verify = COMPORT_VERIFY_NO;
    gps_uarts_cfg.ndata    = 8;
    gps_uarts_cfg.nstop    = 1;
    gps_uarts_cfg.timeout = 150;
    gps_uarts_cfg.rtscts = COMPORT_RTSCTS_DISABLE;
    ret = comport_init(&g_gps_config.port, GPS_DEV, &gps_uarts_cfg);
    if(ret < 0)
    {
        printf("comport %s init error\n", GPS_DEV);
        goto error;
    }
    //ret = ERR_OK;
    /*create gps receive data thread*/
     ret = osal_task_create(&(g_gps_deal_handle.task_gps), "tk_gps",gps_thread_entry, &(g_gps_config.port),GPS_THREAD_STACK_SIZE, GPS_THREAD_PRIORITY);
    if(ret < 0)
    {
        printf("[%s][%d] osal_task_create error\n",__FUNCTION__,__LINE__);
        goto error;
    }
    g_gps_deal_handle.task_gps_valid = 1;

    /*create gps ubx data queue*/
    ret = osal_queue_create(&(g_gps_deal_handle.gps_ubx_queue), "q-gps_ubx_data", GPS_UBX_QUEUE_NUM, GPS_UBX_QUEUE_MSG_SIZE);
    if(ret < 0)
    {
        printf("[%s][%d] osal_queue_create error\n",__FUNCTION__,__LINE__);
        goto error;
    }
    g_gps_deal_handle.gps_ubx_queue_valid = 1;

    
    /*create deal gps ubx data thread*/
    ret = osal_task_create(&(g_gps_deal_handle.task_ubx), "tk_ubx",gps_ubx_thread_entry, &(g_gps_config.port),GPS_THREAD_STACK_SIZE, GPS_UBX_THREAD_PRIORITY);
    if(ret < 0)
    {
        printf("[%s][%d] osal_task_create error\n",__FUNCTION__,__LINE__);
        goto error;
    }
    g_gps_deal_handle.task_ubx_valid = 1;
    
    /*configure gps*/
    gps_chip_config(&g_gps_config);
    gps_uarts_cfg.baud = 115200;
    /*set the configuration of host uart*/
    ret = comport_config(g_gps_config.port, &gps_uarts_cfg);
      if(ret < 0)
      {
          printf("[%s][%d] comport_setconfig error\n",__FUNCTION__,__LINE__);
        goto error;
      }
    
    return ret;    
error:
    drv_gps_deinit();
    return ret;
}

/*****************************************************
func:drv_gps_deinit
desc:deinit gps device
para:NULL
return:ERR_OK is successful, other value is failed
*****************************************************/
int drv_gps_deinit(void)
{
    int ret = ERR_OK;
    
    if(g_gps_deal_handle.task_gps_valid)
    {
        /*delete gps receive data task*/
        ret = osal_task_del(g_gps_deal_handle.task_gps);
        if(ret < 0)
            goto error;
        
        g_gps_deal_handle.task_gps_valid = 0;
    }

    if(g_gps_deal_handle.task_ubx_valid)
    {
        /*delete deal ubx data task*/
        ret = osal_task_del(g_gps_deal_handle.task_ubx);
        if(ret < 0)
            goto error;

        g_gps_deal_handle.task_ubx_valid = 0;
    }

    if(g_gps_deal_handle.gps_ubx_queue_valid)
    {
        /*delete ubx queue*/
        ret = osal_queue_delete(g_gps_deal_handle.gps_ubx_queue);
        if(ret < 0)
            goto error;
        
        g_gps_deal_handle.gps_ubx_queue_valid = 0;
    }
    
    if(g_gps_config.port > 0)
    {
        /*set gps baud to 9600*/
        ubx_cfg_uart_port(g_gps_config.port, 9600);
        close(g_gps_config.port);/*close the fd of gps uart*/    
    }
    
    memset(&g_gps_config, 0, sizeof(g_gps_config));
    memset(&g_gps_deal_handle, 0, sizeof(gps_deal_handle_t));
error:
    return ret;
}



