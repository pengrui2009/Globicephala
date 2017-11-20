/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : nmea0183_parse.c
 @brief  : nema0183 parse api
 @author : wanghechao
 @history:
           2017-9-29	  wanghechao	Created file
           ...
******************************************************************************/

#include <stdint.h>
#include <stdlib.h>
#include <memory.h>

#include "../../include/error.h"
#include "nmea0183_parse.h"
#include "../../osal/linux/os_core.h"

/*
*********************************************************************************************************
*   函 数 名: nmea_hex2ascii
*   功能说明: 将hex码0x1f转换成'1'和'f'. 结尾填0.
*   形    参: ucpHex 输入缓冲区指针
*            ucp_ascii 输出缓冲区指针
*           len_asc ASCII的字符长度.
*   返 回 值: 二进制整数值
*********************************************************************************************************
*/
	/* Hex to ascii code. */
static void nmea_hex2ascii(uint8_t *ucp_hex, uint8_t *ucp_ascii, uint8_t len_asc)
{
	uint8_t    i = 0;
	uint8_t temp = 0;


	for (i = 0; i < len_asc; i++)
	{
		temp = * ucp_hex;
		
		if ((i & 0x01) == 0x00)
		{
			temp = temp >> 4;
		}	 
		else
		{
			temp = temp & 0x0f;
			ucp_hex++;
		}
		
		temp += (temp < 0x0a) ? 0x30 : 0x37;
		ucp_ascii[i] = temp;
	}
}


/* Check nmea frame xor. */
static int nmea_check_xor(uint8_t *data_ptr, uint16_t length)
{
    int              ret = ERR_OK;
    
    uint8_t      xor_sum = 0;
    uint8_t check_buf[2] = { 0 };
    uint16_t           i = 0;


    /* Check frame integrity. */
    if((length < 6) || (data_ptr[length - 5] != '*'))
    {
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Return error when have no-ascii charactor. */
    for(i = 1; i < (length - 5); i++)
    {
        if((data_ptr[i] & 0x80) || (data_ptr[i] == 0))
        {
            ret = -ERR_INVAL;
            goto ERR_EXIT;
        }
    }

    /* Get xor code. */
    for(i = 1; i < (length - 5); i++)
    {
        xor_sum = xor_sum ^ data_ptr[i];
    }

    /* Compare xor code. */
    nmea_hex2ascii(&xor_sum, check_buf, 2);
    if(memcmp(&data_ptr[length - 4], check_buf, 2) == 0)
    {
        ret = ERR_OK;
    }
    
ERR_EXIT:
    
    return ret;
}

/* Parse utc time in nmea frame. */
static int nmea_parse_utc_time(uint8_t *start_ptr, uint8_t *end_ptr, nmea_utc_time_st_ptr utc_ptr)
{
    int             ret = ERR_OK;
    uint8_t tmp_buff[5] = { 0 };


    /* Check validation. */
    if((*(start_ptr + 6) != '.') || ((end_ptr - start_ptr) < 9))
    {
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    /* Check the msec data length and parse the specific data. */
    if((end_ptr - start_ptr) == 10)
    {
        /* Old nmea0183 format hhmmss.sss. */
        memset( tmp_buff, 0, sizeof(tmp_buff));
        tmp_buff[0] = *(start_ptr + 7);
        tmp_buff[1] = *(start_ptr + 8);
        tmp_buff[2] = *(start_ptr + 9);
        utc_ptr->msec = atoi((char *)tmp_buff);
    }
    else if((end_ptr - start_ptr) == 9)
    {
        /* New nmea0183 format hhmmss.ss. */
        memset( tmp_buff, 0, sizeof(tmp_buff));
        tmp_buff[0] = *(start_ptr + 7);
        tmp_buff[1] = *(start_ptr + 8);
        utc_ptr->msec = atoi((char *)tmp_buff) * 10;
    }
    else
    {
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Hour. */
    memset(tmp_buff, 0, sizeof(tmp_buff));
    tmp_buff[0] = *(start_ptr);
    tmp_buff[1] = *(start_ptr + 1);
    utc_ptr->hour = (uint8_t) atoi((char *)tmp_buff);

    /* Minute. */
    memset( tmp_buff, 0, sizeof(tmp_buff));
    tmp_buff[0] = *(start_ptr + 2);
    tmp_buff[1] = *(start_ptr + 3);
    utc_ptr->min = (uint8_t) atoi((char *)tmp_buff);

    /* Second. */
    memset( tmp_buff, 0, sizeof(tmp_buff));
    tmp_buff[0] = *(start_ptr + 4);
    tmp_buff[1] = *(start_ptr + 5);
    utc_ptr->sec = (uint8_t) atoi((char *)tmp_buff);

    ret = ERR_OK;
    
ERR_EXIT:

    return ret;
}

/* Parse utc date in nmea frame. */
static int nmea_parse_utc_date(uint8_t *start_ptr, uint8_t *end_ptr, nmea_utc_date_st_ptr utc_ptr)
{
    int             ret = ERR_OK;
    uint8_t tmp_buff[5] = { 0 };


    /* Check validation. */
    if((end_ptr - start_ptr) < 6)
    {
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    /* day. */
    memset(tmp_buff, 0, sizeof(tmp_buff));
    tmp_buff[0] = *(start_ptr);
    tmp_buff[1] = *(start_ptr + 1);
    utc_ptr->day = (uint8_t) atoi((char *)tmp_buff);

    /* month. */
    memset( tmp_buff, 0, sizeof(tmp_buff));
    tmp_buff[0] = *(start_ptr + 2);
    tmp_buff[1] = *(start_ptr + 3);
    utc_ptr->mon = (uint8_t) atoi((char *)tmp_buff);

    /* year. */
    memset( tmp_buff, 0, sizeof(tmp_buff));
    tmp_buff[0] = *(start_ptr + 4);
    tmp_buff[1] = *(start_ptr + 5);
    utc_ptr->year = (uint8_t) atoi((char *)tmp_buff) + 2000;

    ret = ERR_OK;
    
ERR_EXIT:

    return ret;
}


/* Parse latitude in nmea frame. */
static int nmea_parse_latitude(uint8_t *start_ptr, uint8_t *end_ptr, double *latitude_ptr)
{
    int              ret = ERR_OK;
    uint8_t tmp_buff[15] = { 0 };
    uint8_t            i = 0;


    /* Check validation. */
    if(*(start_ptr + 4) != '.') //|| ((end_ptr - start_ptr) != 10)
    {
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }
    /* Degree. */
    memset(tmp_buff, 0, sizeof(tmp_buff));
    memcpy(tmp_buff, start_ptr, 2);
    *latitude_ptr = (double)atoi((char *)tmp_buff);
    
    /* Minute. */
    memset(tmp_buff, 0, sizeof(tmp_buff));
    for(i = 2; i < (end_ptr - start_ptr); i++) 
    {
        if(isdigit(*(start_ptr + i)) || *(start_ptr + i) == '.') 
        {
            tmp_buff[i - 2] = *(start_ptr + i);
        }
        else 
        {
            ret = -ERR_INVAL;
            goto ERR_EXIT;
            break;
        }
    }
    *latitude_ptr += (atof((char *)tmp_buff) / 60.0);
    ret = ERR_OK;
    
ERR_EXIT:
    
    return ret;
}

/* Parse latitude ns in nmea frame. */
static int nmea_parse_latitude_ns(uint8_t *start_ptr, uint8_t *end_ptr, nmea_latitude_em *ns_ptr)
{
    int    ret = ERR_OK;


    /* Check validation. */
    if((end_ptr - start_ptr) != 1)
    {
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Set N/S. */
    if(*start_ptr == 'S')
    {
        *ns_ptr = LATITUDE_S;
    }
    else if(*start_ptr == 'N')
    {
        *ns_ptr = LATITUDE_N;
    }
    else
    {
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }

    ret = ERR_OK;
    
ERR_EXIT:
    
    return ret;
}



/* Parse longitude in nmea frame. */
static int nmea_parse_longitude(uint8_t *start_ptr, uint8_t *end_ptr, double *longitude_ptr)
{
    int              ret = ERR_OK;
    uint8_t tmp_buff[15] = { 0 };
    uint8_t            i = 0;


    /* Check validation. */
    if(*(start_ptr + 5) != '.') //|| ((end_ptr - start_ptr) != 11)
    {
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Degree. */
    memset(tmp_buff, 0, sizeof(tmp_buff));
    memcpy(tmp_buff, start_ptr, 3);
    *longitude_ptr = (double)atoi((char *)tmp_buff);
    
    /* Minute. */
    memset(tmp_buff, 0, sizeof(tmp_buff));
    for(i = 3; i < (end_ptr - start_ptr); i++) 
    {
        if(isdigit(*(start_ptr + i)) || *(start_ptr + i) == '.') 
        {
            tmp_buff[i - 3] = *(start_ptr + i);
        }
        else 
        {
            ret = -ERR_INVAL;
            goto ERR_EXIT;
            break;
        }
    }
    *longitude_ptr += (atof((char *)tmp_buff) / 60.0);
    ret = ERR_OK;
    
ERR_EXIT:
    
    return ret;
}

/* Parse longitude ew in nmea frame. */
static int nmea_parse_longitude_ew(uint8_t *start_ptr, uint8_t *end_ptr, nmea_longitude_em *ew_ptr)
{
    int   ret = ERR_OK;


    /* Check validation. */
    if((end_ptr - start_ptr) != 1)
    {
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Set N/S. */
    if(*start_ptr == 'E')
    {
        *ew_ptr = LONGITUDE_E;
    }
    else if(*start_ptr == 'W')
    {
        *ew_ptr = LONGITUDE_W;
    }
    else
    {
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }

    ret = ERR_OK;
    
ERR_EXIT:
    
    return ret;
}


/* Parse quality in nmea frame. */
static int nmea_parse_quality(uint8_t *start_ptr, uint8_t *end_ptr, nmea_gps_quality_em *quality_ptr)
{
    int   ret = ERR_OK;


    /* Check validation. */
    if((end_ptr - start_ptr) != 1)
    {
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }

	switch(*start_ptr)
	{
		case '0':
			*quality_ptr = GPS_QUALITY_NO_FIX;
			break;
		case '1':
			*quality_ptr = GPS_QUALITY_AUTO_GNSS_FIX;
			break;
		case '2':
			*quality_ptr = GPS_QUALITY_DIFF_GNSS_FIX;
			break;
		case '4':
			*quality_ptr = GPS_QUALITY_RTK_FIX;
			break;
		case '5':
			*quality_ptr = GPS_QUALITY_RTK_FLOAT;
			break;
		case '6':
			*quality_ptr = GPS_QUALITY_ESTIMATED_FIX;
			break;
		default:
			ret = -ERR_INVAL;
        	goto ERR_EXIT;
			break;
	}
    ret = ERR_OK;
    
ERR_EXIT:
    
    return ret;
}

/* Parse operation_mode in nmea frame. */
static int nmea_parse_operation_mode(uint8_t *start_ptr, uint8_t *end_ptr, nmea_gps_opMode_em *opmode_ptr)
{
    int   ret = ERR_OK;


    /* Check validation. */
    if((end_ptr - start_ptr) != 1)
    {
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }

	switch(*start_ptr)
	{
		case 'A':
			*opmode_ptr = GPS_OPMODE_AUTO_2D_3D;
			break;
		case 'M':
			*opmode_ptr = GPS_OPMODE_MANUAL_2D_3D;
			break;
		default:
			ret = -ERR_INVAL;
        	goto ERR_EXIT;
			break;
	}
    ret = ERR_OK;
    
ERR_EXIT:
    
    return ret;
}


/* Parse navigation_mode in nmea frame. */
static int nmea_parse_navigation_mode(uint8_t *start_ptr, uint8_t *end_ptr, nmea_gps_navMode_em *navmode_ptr)
{
    int   ret = ERR_OK;


    /* Check validation. */
    if((end_ptr - start_ptr) != 1)
    {
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }

	switch(*start_ptr)
	{
		case '1':
			*navmode_ptr = GPS_NAVMODE_NOT_AVAILABLE;
			break;
		case '2':
			*navmode_ptr = GPS_NAVMODE_2D_FIX;
			break;
		case '3':
			*navmode_ptr = GPS_NAVMODE_3D_FIX;
			break;
		default:
			ret = -ERR_INVAL;
        	goto ERR_EXIT;
			break;
	}
    ret = ERR_OK;
    
ERR_EXIT:
    
    return ret;
}

/* Parse status in nmea frame. */
static int nmea_parse_status(uint8_t *start_ptr, uint8_t *end_ptr, nmea_gps_status_em *status_ptr)
{
    int   ret = ERR_OK;

    /* Check validation. */
    if((end_ptr - start_ptr) != 1)
    {
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }

	switch(*start_ptr)
	{
		case 'A':
			*status_ptr = GPS_STATUS_DATA_VALID;
			break;
		case 'V':
			*status_ptr = GPS_STATUS_DATA_INVALID;
			break;
		default:
			ret = -ERR_INVAL;
        	goto ERR_EXIT;
			break;
	}
    ret = ERR_OK;
    
ERR_EXIT:
    
    return ret;
}


/* Parse position_mode in nmea frame. */
static int nmea_parse_position_mode(uint8_t *start_ptr, uint8_t *end_ptr, nmea_gps_posMode_em *posmode_ptr)
{
    int   ret = ERR_OK;

    /* Check validation. */
    if((end_ptr - start_ptr) != 1)
    {
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }

	switch(*start_ptr)
	{
		case 'N':
			*posmode_ptr = GPS_POSMODE_NO_FIX;
			break;
		case 'E':
			*posmode_ptr = GPS_POSMODE_ESTIMATED_FIX;
			break;
		case 'A':
			*posmode_ptr = GPS_POSMODE_AUTO_GNSS_FIX;
			break;
		case 'D':
			*posmode_ptr = GPS_POSMODE_DIFF_GNSS_FIX;
			break;
		case 'F':
			*posmode_ptr = GPS_POSMODE_RTK_FLOAT;
			break;
		case 'R':
			*posmode_ptr = GPS_POSMODE_RTK_FIX;
			break;
		default:
			ret = -ERR_INVAL;
        	goto ERR_EXIT;
			break;
	}
    ret = ERR_OK;
    
ERR_EXIT:
    
    return ret;
}


/* Parse navigation_status in nmea frame. */
static int nmea_parse_navigation_status(uint8_t *start_ptr, uint8_t *end_ptr, nmea_gps_navStatus_em *navstatus_ptr)
{
    int   ret = ERR_OK;

    /* Check validation. */
    if((end_ptr - start_ptr) != 1)
    {
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }

	switch(*start_ptr)
	{
		case 'V':
			*navstatus_ptr = GPS_NAVSTATUS_NOT_PROVID;
			break;
		default:
			ret = -ERR_INVAL;
        	goto ERR_EXIT;
			break;
	}
    ret = ERR_OK;
    
ERR_EXIT:
    
    return ret;
}


/* Parse nmea frame: GxGSA. */
int nmea_parse_GxGSA(nmea_GxGSA_st_ptr gsa_ptr, uint8_t *data_ptr, uint16_t len)
{
	int            ret = ERR_OK;
	int 	   fun_ret = ERR_OK;
    uint16_t  *dot_ptr = NULL;
    uint16_t dot_index = 0;
    
    uint16_t  dot_addr = 0;


    /* Calloc dot indicator. */
    if((dot_ptr = os_calloc(1, len)) == NULL)
	{
	    printf("[%s %d]: os_calloc data buffer error. \n", __FUNCTION__, __LINE__);
        
        fun_ret = ERR_NOMEM;
		goto ERR_EXIT;
	}

    /* Find all the dot in buffer. */
    for(dot_addr = 0; dot_addr < len; dot_addr ++)
    {
        if((*(data_ptr + dot_addr) == ',') || (*(data_ptr + dot_addr) == '*'))
        {
            *(dot_ptr + dot_index) = dot_addr;
            dot_index ++;
        }
    }

    /* Reset all the zone. */
    memset(gsa_ptr, 0, sizeof(*gsa_ptr));

    /* Frame name. */
    if(dot_ptr[0] == 6)
    {
        memcpy(gsa_ptr->name, data_ptr, 6);
        gsa_ptr->opt.name = 1;
    }
    else
    {
    	fun_ret = -ERR_INVAL;
        goto ERR_EXIT;
    }

	ret = nmea_parse_operation_mode(data_ptr + dot_ptr[0] + 1, data_ptr + dot_ptr[1], &(gsa_ptr->op_mode));
	if(ret == ERR_OK)
	{
		gsa_ptr->opt.op_mode = 1;
	}

	ret = nmea_parse_navigation_mode(data_ptr + dot_ptr[1] + 1, data_ptr + dot_ptr[2], &(gsa_ptr->nav_mode));
	if(ret == ERR_OK)
	{
		gsa_ptr->opt.nav_mode = 1;
	}

	/*parse the satellite_id*/
	for(dot_addr = 0; dot_addr < 12; dot_addr++)
	{
		if(dot_ptr[3 + dot_addr] - dot_ptr[2 + dot_addr] > 1)
		{
			uint8_t tmp[10];
			switch(dot_addr)
			{
				case 0:
					gsa_ptr->opt.satellite_id1 = 1;
					break;
				case 1:
					gsa_ptr->opt.satellite_id2 = 1;
					break;
				case 2:
					gsa_ptr->opt.satellite_id3 = 1;
					break;
				case 3:
					gsa_ptr->opt.satellite_id4 = 1;
					break;
				case 4:
					gsa_ptr->opt.satellite_id5 = 1;
					break;
				case 5:
					gsa_ptr->opt.satellite_id6 = 1;
					break;
				case 6:
					gsa_ptr->opt.satellite_id7 = 1;
					break;
				case 7:
					gsa_ptr->opt.satellite_id8 = 1;
					break;
				case 8:
					gsa_ptr->opt.satellite_id9 = 1;
					break;
				case 9:
					gsa_ptr->opt.satellite_id10 = 1;
					break;
				case 10:
					gsa_ptr->opt.satellite_id11 = 1;
					break;
				case 11:
					gsa_ptr->opt.satellite_id12 = 1;
					break;
				default:
					ret = -ERR_INVAL;
					goto ERR_EXIT;
					break;
			}	
			memset(tmp, 0, sizeof(tmp));
			memcpy(tmp, data_ptr + dot_ptr[2 + dot_addr] + 1, dot_ptr[3 + dot_addr] - dot_ptr[2 + dot_addr] - 1);
			gsa_ptr->satellite_id[dot_addr] = atoi((char *)tmp);	
		}
	}

	/*parse the PDOP*/
	if(dot_ptr[3 + dot_addr] - dot_ptr[2 + dot_addr] > 1)
	{
		uint8_t tmp[10];
		gsa_ptr->opt.pdop = 1;	
		memset(tmp, 0, sizeof(tmp));
		memcpy(tmp, data_ptr + dot_ptr[2 + dot_addr] + 1, dot_ptr[3 + dot_addr] - dot_ptr[2 + dot_addr] - 1);
		gsa_ptr->pdop = atof((char *)tmp);	
		dot_addr++;
	}

	/*parse the HDOP*/
	if(dot_ptr[3 + dot_addr] - dot_ptr[2 + dot_addr] > 1)
	{
		uint8_t tmp[10];
		gsa_ptr->opt.hdop = 1;	
		memset(tmp, 0, sizeof(tmp));
		memcpy(tmp, data_ptr + dot_ptr[2 + dot_addr] + 1, dot_ptr[3 + dot_addr] - dot_ptr[2 + dot_addr] - 1);
		gsa_ptr->hdop = atof((char *)tmp);	
		dot_addr++;
	}

	/*parse the VDOP*/
	if(dot_ptr[3 + dot_addr] - dot_ptr[2 + dot_addr] > 1)
	{
		uint8_t tmp[10];
		gsa_ptr->opt.vdop = 1;	
		memset(tmp, 0, sizeof(tmp));
		memcpy(tmp, data_ptr + dot_ptr[2 + dot_addr] + 1, dot_ptr[3 + dot_addr] - dot_ptr[2 + dot_addr] - 1);
		gsa_ptr->vdop = atof((char *)tmp);	
		dot_addr++;
	}

	/*parse the system id*/
	if(dot_ptr[3 + dot_addr] > 0)
	{
		if(dot_ptr[3 + dot_addr] - dot_ptr[2 + dot_addr] > 1)
		{
			uint8_t tmp[10];
			gsa_ptr->opt.system_id = 1;	
			memset(tmp, 0, sizeof(tmp));
			memcpy(tmp, data_ptr + dot_ptr[2 + dot_addr] + 1, dot_ptr[3 + dot_addr] - dot_ptr[2 + dot_addr] - 1);
			gsa_ptr->system_id = atoi((char *)tmp);	
			dot_addr++;
		}
	}
	
	gsa_ptr->check_sum = (*(data_ptr + dot_ptr[2 + dot_addr] + 1)<<8)|(*(data_ptr + dot_ptr[2 + dot_addr] + 2));
	gsa_ptr->opt.check_sum = 1;


ERR_EXIT:

    if(dot_ptr != NULL)
    {
        os_free(dot_ptr);
    }

    return fun_ret;
}


/*
*********************************************************************************************************
*   函 数 名: gpsGxGGA
*   功能说明: 分析0183数据包中的 GPGGA 命令，结果存放到全局变量
*   形    参:  buff  收到的数据
*            len    数据长度
*   返 回 值: 无
*********************************************************************************************************
*/
/*
例：$GPGGA,092204.999,4250.5589,S,14718.5084,E,1,04,24.4,19.7,M,,,,0000*1F
字段0：$GPGGA，语句ID，表明该语句为Global Positioning System Fix Data（GGA）GPS定位信息
字段1：UTC 时间，hhmmss.sss，时分秒格式
字段2：纬度ddmm.mmmm，度分格式（前导位数不足则补0）
字段3：纬度N（北纬）或S（南纬）
字段4：经度dddmm.mmmm，度分格式（前导位数不足则补0）
字段5：经度E（东经）或W（西经）
字段6：GPS状态，0=未定位，1=非差分定位，2=差分定位，3=无效PPS，6=正在估算
字段7：正在使用的卫星数量（00 - 12）（前导位数不足则补0）
字段8：HDOP水平精度因子（0.5 - 99.9）
字段9：海拔高度（-9999.9 - 99999.9）
字段10：地球椭球面相对大地水准面的高度
字段11：差分时间（从最近一次接收到差分信号开始的秒数，如果不是差分定位将为空）
字段12：差分站ID号0000 - 1023（前导位数不足则补0，如果不是差分定位将为空）
字段13：校验值
*/

/* Parse nmea frame: GxGGA. */
static int nmea_parse_GxGGA(nmea_GxGGA_st_ptr gga_ptr, uint8_t *data_ptr, uint16_t len)
{
    int            ret = ERR_OK;
	int 	   fun_ret = ERR_OK;
    uint16_t  *dot_ptr = NULL;
    uint16_t dot_index = 0;
    
    uint16_t  dot_addr = 0;


    /* Calloc dot indicator. */
    if((dot_ptr = os_calloc(1, len)) == NULL)
	{
	    printf("[%s %d]: os_calloc data buffer error. \n", __FUNCTION__, __LINE__);
        
        fun_ret = -ERR_NOMEM;
		goto ERR_EXIT;
	}

    /* Find all the dot in buffer. */
    for(dot_addr = 0; dot_addr < len; dot_addr ++)
    {
        if((*(data_ptr + dot_addr) == ',') || (*(data_ptr + dot_addr) == '*'))
        {
            *(dot_ptr + dot_index) = dot_addr;
            dot_index ++;
        }
    }

    /* Reset all the zone. */
    memset(gga_ptr, 0, sizeof(*gga_ptr));

    /* Frame name. */
    if(dot_ptr[0] == 6)
    {
        memcpy(gga_ptr->name, data_ptr, 6);
        gga_ptr->opt.name = 1;
    }
    else
    {
        fun_ret = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    
	ret = nmea_parse_utc_time(data_ptr + dot_ptr[0] + 1, data_ptr + dot_ptr[1], &(gga_ptr->utc_time));
	if(ret == ERR_OK)
	{
		gga_ptr->opt.utc_time = 1;
	}
   
    /* Latitude. */

	ret = nmea_parse_latitude(data_ptr + dot_ptr[1] + 1, data_ptr + dot_ptr[2], &(gga_ptr->latitude));
	if(ret == ERR_OK)
	{
		gga_ptr->opt.latitude = 1;
	}

    /* Latitude_ns. */
  
	ret = nmea_parse_latitude_ns(data_ptr + dot_ptr[2] + 1, data_ptr + dot_ptr[3], &(gga_ptr->lat_ns));
	if(ret == ERR_OK)
	{
		gga_ptr->opt.lat_ns = 1;
	}

    /* Longitude. */
  
	ret = nmea_parse_longitude(data_ptr + dot_ptr[3] + 1, data_ptr + dot_ptr[4], &(gga_ptr->longitude));
	if(ret == ERR_OK)
	{
		gga_ptr->opt.longitude = 1;
	}


    /* Longitude_ew. */
	ret = nmea_parse_longitude_ew(data_ptr + dot_ptr[4] + 1, data_ptr + dot_ptr[5], &(gga_ptr->lon_ew));
	if(ret == ERR_OK)
	{
		gga_ptr->opt.lon_ew = 1;
	}
	
	ret = nmea_parse_quality(data_ptr + dot_ptr[5] + 1, data_ptr + dot_ptr[6], &(gga_ptr->gps_qa));
	if(ret == ERR_OK)
	{
		gga_ptr->opt.gps_qa = 1;
	}

	/*parse the satellite num*/
	if(dot_ptr[7] - dot_ptr[6] > 1)
	{
		uint8_t tmp[10];
		gga_ptr->opt.satellite_num = 1;	
		memset(tmp, 0, sizeof(tmp));
		memcpy(tmp, data_ptr + dot_ptr[6] + 1, dot_ptr[7] - dot_ptr[6] - 1);
		gga_ptr->satellite_num = atoi((char *)tmp);		
	}

	/*parse the HDOP*/
	if(dot_ptr[8] - dot_ptr[7] > 1)
	{
		uint8_t tmp[10];
		gga_ptr->opt.hdop = 1;	
		memset(tmp, 0, sizeof(tmp));
		memcpy(tmp, data_ptr + dot_ptr[7] + 1, dot_ptr[8] - dot_ptr[7] - 1);
		gga_ptr->hdop = atof((char *)tmp);		
	}

	/*parse the altitude*/
	if(dot_ptr[9] - dot_ptr[8] > 1)
	{
		uint8_t tmp[10];
		gga_ptr->opt.altitude = 1;	
		memset(tmp, 0, sizeof(tmp));
		memcpy(tmp, data_ptr + dot_ptr[8] + 1, dot_ptr[9] - dot_ptr[8] - 1);
		gga_ptr->altitude = atof((char *)tmp);		
	}

	/*dot_ptr[9] - dot_ptr[8]: uAlt (m)  */

	/*parse the geoidal separation*/
	if(dot_ptr[10] - dot_ptr[9] > 1)
	{
		uint8_t tmp[10];
		gga_ptr->opt.geoidal_separation = 1;	
		memset(tmp, 0, sizeof(tmp));
		memcpy(tmp, data_ptr + dot_ptr[9] + 1, dot_ptr[10] - dot_ptr[9] - 1);
		gga_ptr->geoidal_separation = atof((char *)tmp);		
	}

	/*dot_ptr[11] - dot_ptr[10]: uSep (m)*/

	/*parse the diff data age*/
	if(dot_ptr[12] - dot_ptr[11] > 1)
	{
		uint8_t tmp[10];
		gga_ptr->opt.diff_data_age = 1;	
		memset(tmp, 0, sizeof(tmp));
		memcpy(tmp, data_ptr + dot_ptr[11] + 1, dot_ptr[12] - dot_ptr[11] - 1);
		gga_ptr->diff_data_age = atoi((char *)tmp);		
	}

	/*parse the diff station id*/
	if(dot_ptr[13] - dot_ptr[12] > 1)
	{
		uint8_t tmp[10];
		gga_ptr->opt.diff_station_id = 1;	
		memset(tmp, 0, sizeof(tmp));
		memcpy(tmp, data_ptr + dot_ptr[12] + 1, dot_ptr[13] - dot_ptr[12] - 1);
		gga_ptr->diff_station_id = atoi((char *)tmp);		
	}

	gga_ptr->check_sum = (*(data_ptr + dot_ptr[13] + 1)<<8)|(*(data_ptr + dot_ptr[13] + 2));
	gga_ptr->opt.check_sum = 1;

ERR_EXIT:

    if(dot_ptr != NULL)
    {
        os_free(dot_ptr);
    }

    return fun_ret;
}



/*
*********************************************************************************************************
*	函 数 名: gpsGxRMC
*	功能说明: 分析0183数据包中的 GPGSV 命令，结果存放到全局变量
*	形    参:  buff  收到的数据
*			 len    数据长度
*	返 回 值: 无
*********************************************************************************************************
*/
/*
例：$GPRMC,024813.640,A,3158.4608,N,11848.3737,E,10.05,324.27,150706,,,A*50
字段0：$GPRMC，语句ID，表明该语句为Recommended Minimum Specific GPS/TRANSIT Data（RMC）推荐最小定位信息
字段1：UTC时间，hhmmss.sss格式
字段2：状态，A=定位，V=未定位
字段3：纬度ddmm.mmmm，度分格式（前导位数不足则补0）
字段4：纬度N（北纬）或S（南纬）
字段5：经度dddmm.mmmm，度分格式（前导位数不足则补0）
字段6：经度E（东经）或W（西经）
字段7：速度，节，Knots
字段8：方位角，度
字段9：UTC日期，DDMMYY格式
字段10：磁偏角，（000 - 180）度（前导位数不足则补0）
字段11：磁偏角方向，E=东W=西
字段16：校验值
*/
int nmea_parse_GxRMC(nmea_GxRMC_st_ptr rmc_ptr, uint8_t *data_ptr, uint16_t len)
{
    int            ret = ERR_OK;
	int 	   fun_ret = ERR_OK;
    uint16_t  *dot_ptr = NULL;
    uint16_t dot_index = 0;
    
    uint16_t  dot_addr = 0;


    /* Calloc dot indicator. */
    if((dot_ptr = os_calloc(1, len)) == NULL)
	{
	    printf("[%s %d]: Calloc data buffer error. \n", __FUNCTION__, __LINE__);
        
        fun_ret = -ERR_NOMEM;
		goto ERR_EXIT;
	}

    /* Find all the dot in buffer. */
    for(dot_addr = 0; dot_addr < len; dot_addr ++)
    {
        if((*(data_ptr + dot_addr) == ',') || (*(data_ptr + dot_addr) == '*'))
        {
            *(dot_ptr + dot_index) = dot_addr;
            dot_index ++;
        }
    }
    /* Reset all the zone. */
    memset(rmc_ptr, 0, sizeof(*rmc_ptr));

    /* Frame name. */
    if(dot_ptr[0] == 6)
    {
        memcpy(rmc_ptr->name, data_ptr, 6);
        rmc_ptr->opt.name = 1;
    }
    else
    {
        fun_ret = -ERR_INVAL;
        goto ERR_EXIT;
    }

	ret = nmea_parse_utc_time(data_ptr + dot_ptr[0] + 1, data_ptr + dot_ptr[1], &(rmc_ptr->utc_time));
	if(ret == ERR_OK)
	{
		rmc_ptr->opt.utc_time = 1;
	}

	ret = nmea_parse_status(data_ptr + dot_ptr[1] + 1, data_ptr + dot_ptr[2], &(rmc_ptr->status));
	if(ret == ERR_OK)
	{
		rmc_ptr->opt.status = 1;
	}
    /* Latitude. */

	ret = nmea_parse_latitude(data_ptr + dot_ptr[2] + 1, data_ptr + dot_ptr[3], &(rmc_ptr->latitude));
	if(ret == ERR_OK)
	{
		rmc_ptr->opt.latitude = 1;
	}

    /* Latitude_ns. */
  
	ret = nmea_parse_latitude_ns(data_ptr + dot_ptr[3] + 1, data_ptr + dot_ptr[4], &(rmc_ptr->lat_ns));
	if(ret == ERR_OK)
	{
		rmc_ptr->opt.lat_ns = 1;
	}

    /* Longitude. */
  
	ret = nmea_parse_longitude(data_ptr + dot_ptr[4] + 1, data_ptr + dot_ptr[5], &(rmc_ptr->longitude));
	if(ret == ERR_OK)
	{
		rmc_ptr->opt.longitude = 1;
	}

    /* Longitude_ew. */
	ret = nmea_parse_longitude_ew(data_ptr + dot_ptr[5] + 1, data_ptr + dot_ptr[6], &(rmc_ptr->lon_ew));
	if(ret == ERR_OK)
	{
		rmc_ptr->opt.lon_ew = 1;
	}

	/*parse the speed,km/h*/
	if(dot_ptr[7] - dot_ptr[6] > 1)
	{
		uint8_t tmp[10];
		rmc_ptr->opt.speed = 1; 
		memset(tmp, 0, sizeof(tmp));
		memcpy(tmp, data_ptr + dot_ptr[6] + 1, dot_ptr[7] - dot_ptr[6] - 1);
		rmc_ptr->speed = atof((char *)tmp)* 1.852f; 	
	}

	/*parse course,degree*/
	if(dot_ptr[8] - dot_ptr[7] > 1)
	{
		uint8_t tmp[10];
		rmc_ptr->opt.course = 1; 
		memset(tmp, 0, sizeof(tmp));
		memcpy(tmp, data_ptr + dot_ptr[7] + 1, dot_ptr[8] - dot_ptr[7] - 1);
		rmc_ptr->course = atof((char *)tmp); 	
	}

	 /* utc_date. */
	ret = nmea_parse_utc_date(data_ptr + dot_ptr[8] + 1, data_ptr + dot_ptr[9], &(rmc_ptr->utc_date));
	if(ret == ERR_OK)
	{
		rmc_ptr->opt.utc_date = 1;
	}

	/*parse the mv*/
	if(dot_ptr[10] - dot_ptr[9] > 1)
	{
		uint8_t tmp[10];
		rmc_ptr->opt.mv = 1; 
		memset(tmp, 0, sizeof(tmp));
		memcpy(tmp, data_ptr + dot_ptr[9] + 1, dot_ptr[10] - dot_ptr[9] - 1);
		rmc_ptr->mv = atoi((char *)tmp); 	
	}

	/*parse the mv ew*/
	if(dot_ptr[11] - dot_ptr[10] > 1)
	{
		uint8_t tmp[10];
		rmc_ptr->opt.mv_ew = 1; 
		memset(tmp, 0, sizeof(tmp));
		memcpy(tmp, data_ptr + dot_ptr[10] + 1, dot_ptr[11] - dot_ptr[10] - 1);
		rmc_ptr->mvEW = atoi((char *)tmp); 	
	}

	 /* position_mode. */
	ret = nmea_parse_position_mode(data_ptr + dot_ptr[11] + 1, data_ptr + dot_ptr[12], &(rmc_ptr->pos_mode));
	if(ret == ERR_OK)
	{
		rmc_ptr->opt.pos_mode = 1;
	}

	if(dot_ptr[13] > 0)
	{
		/* position_mode. */
		ret = nmea_parse_navigation_status(data_ptr + dot_ptr[12] + 1, data_ptr + dot_ptr[13], &(rmc_ptr->nav_status));
		if(ret == ERR_OK)
		{
			rmc_ptr->opt.nav_status = 1;
		}
		
		rmc_ptr->check_sum = (*(data_ptr + dot_ptr[13] + 1)<<8)|(*(data_ptr + dot_ptr[13] + 2));
		rmc_ptr->opt.check_sum = 1;
	}
	else
	{
		rmc_ptr->check_sum = (*(data_ptr + dot_ptr[12] + 1)<<8)|(*(data_ptr + dot_ptr[12] + 2));
		rmc_ptr->opt.check_sum = 1;
	}

ERR_EXIT:

    if(dot_ptr != NULL)
    {
        os_free(dot_ptr);
    }

    return fun_ret;
}


/*****************************************************************
func:nmea_parse_main
desc:Main function that parse nmea0183 frame to the specific structure. 
para:type_ptr is return that we needed frame type,xxx_ptr is return data struct,
	data_ptr is data that needed to deal,length is the length of data 
return:ERR_OK is successful, other value is failed
******************************************************************/
int nmea_parse_main(nmea_frametype_em_ptr type_ptr, nmea_GxXXX_st_ptr xxx_ptr, uint8_t *data_ptr, uint16_t length)
{
    int ret = ERR_OK;

    
    /* Error detection. */
    if((type_ptr == NULL) || (xxx_ptr == NULL) || (data_ptr == NULL) || (length == 0))
    {
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Check xor for frame. */
    if(nmea_check_xor(data_ptr, length) != ERR_OK)
    {
        printf("[%s %d]: Check nmea frame error. \n", __FUNCTION__, __LINE__);
        ret = -ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Parse nmea frame based on frame type. */
    if((memcmp(data_ptr, "$GPRMC", 6) == 0) || (memcmp(data_ptr, "$GNRMC", 6) == 0))
    {
		ret = nmea_parse_GxRMC((nmea_GxRMC_st_ptr)xxx_ptr, data_ptr, length);
        *type_ptr = NMEA_FRAME_RMC;
    }
    else if ((memcmp(data_ptr, "$GPGSA", 6) == 0) || (memcmp(data_ptr, "$GNGSA", 6) == 0))
    {
        ret = nmea_parse_GxGSA((nmea_GxGSA_st_ptr)xxx_ptr, data_ptr, length);
        *type_ptr = NMEA_FRAME_GSA;
    }
    else if((memcmp(data_ptr, "$GPGGA", 6) == 0) || (memcmp(data_ptr, "$GNGGA", 6) == 0))
    {
		ret = nmea_parse_GxGGA((nmea_GxGGA_st_ptr)xxx_ptr, data_ptr, length);
        *type_ptr = NMEA_FRAME_GGA;
    }

ERR_EXIT:

    return ret;
}


