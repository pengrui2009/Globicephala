/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : cv_vam_util.c
 @brief  : this file include some common function
 @author : wangyifeng
 @history:
           2014-6-24    wangyifeng    Created file
           ...
******************************************************************************/
#include <math.h>
#include "cv_osal.h"

#define OSAL_MODULE_DEBUG
#define OSAL_MODULE_DEBUG_LEVEL OSAL_DEBUG_INFO
#define MODULE_NAME "vam_util"
//#include "cv_osal_dbg.h"

#include "cv_vam.h"
#include "cv_cms_def.h"



/*****************************************************************************
 * declaration of variables and functions                                    *
*****************************************************************************/

#define EARTH_RADIUS  6371.004
#define RAD(d) ((d)*PI/180.0)


#define _COMPILE_INLINE__ inline






/*****************************************************************************
 * implementation of functions                                               *
*****************************************************************************/

#if 0
static float getDistanceVer1(float lat1, float lng1, float lat2, float lng2)

{

   float radLat1 = rad(lat1);

   float radLat2 = rad(lat2);

   float radLng1 = rad(lng1);

   float radLng2 = rad(lng2);

   float s = acos(sin(radLat1)*sin(radLat2)+cos(radLat1)*cos(radLat2)*cos(radLng1-radLng2));

   s = s * EARTH_RADIUS;

   return s;

}
#endif
void print_f(float* data_f)
{
    char strbuf[64] = {0};
    memset(strbuf, 0x0, sizeof(strbuf));
    sprintf(strbuf, "%3.8f", *data_f);
    osal_printf("dis2 is %s\n",strbuf);
}

float getDistanceVer2(double lat1, double lng1, double lat2, double lng2)
{
//    float pSrc;
//    float pResult1;
//    float pResult2;
//
//    float pOut;
    float s;
    double radLat1 = RAD(lat1);
    double radLat2 = RAD(lat2);
    double radLng1 = RAD(lng1);
    double radLng2 = RAD(lng2);

    double a = radLat1 - radLat2;//radLat1 - radLat2;

    double b = radLng1 - radLng2;//RAD(lng1) - RAD(lng2);

    s = 2 * asin(sqrt(pow(sin(a/2),2) + cos(radLat1)*cos(radLat2)*pow(sin(b/2),2)));

    s = s * EARTH_RADIUS;

    return s;
}

float vsm_get_distance(vam_position_t *p_src, vam_position_t *p_dest)
{
    float d = 1000.0;

    d *= getDistanceVer2(p_src->latitude, p_src->longitude, p_dest->latitude, p_dest->longitude);

    #if 0
    {
        char str[64];
        sprintf(str,"%f", d);
        osal_printf("distance:%s\n", str);
    }
    #endif

    return d;
}


const char *_directfromangle(int angle)
{
    static const char *dir[] = {
        "��",
        "����",
        "��",
        "����",
        "��",
        "����",
        "��",
        "����",
    };

    int i;

    if (angle <= 10){
        i = 0;
    }
    else if(angle < 80){
        i = 1;
    }
    else if(angle <= 100){
        i = 2;
    }
    else if(angle < 170){
        i = 3;
    }
    else if(angle <= 190){
        i = 4;
    }
    else if(angle < 260){
        i = 5;
    }
    else if(angle <= 280){
        i = 6;
    }
    else if(angle < 350){
        i = 7;
    }
    else{
        i = 0;
    }

    return dir[i];
}
vam_pos_data vsm_get_data(vam_stastatus_t *p_src, vam_stastatus_t *p_dest)
{
    double lat1, lng1, lat2, lng2, lat3, lng3;
    float distance_1_2, distance_2_3;
    double angle;
    vam_pos_data  pos_data;
    
    /* reference point */
    lat1 = p_src->pos.latitude;
    lng1 = p_src->pos.longitude;

    /* destination point */
    lat2 = p_dest->pos.latitude;
    lng2 = p_dest->pos.longitude;

    /* temp point */
    lat3 = lat1;
    lng3 = lng2;

    distance_1_2 = getDistanceVer2(lat1, lng1, lat2, lng2);
    distance_2_3 = getDistanceVer2(lat2, lng2, lat3, lng3);
    angle = acos(distance_2_3/distance_1_2)*180/PI;
    
    pos_data.distance_1_2 = distance_1_2;
    pos_data.distance_2_3 = distance_2_3;
    pos_data.angle = angle;
    return pos_data;

}

float vsm_get_pos(vam_stastatus_t *p_src, vam_stastatus_t *p_dest,vam_pos_data *pos_data)
{
    double lat1, lng1, lat2, lng2;
    float distance_1_2;
    float angle, delta;

    /* reference point */
    lat1 = p_src->pos.latitude;
    lng1 = p_src->pos.longitude;

    /* destination point */
    lat2 = p_dest->pos.latitude;
    lng2 = p_dest->pos.longitude;


    distance_1_2 = pos_data->distance_1_2;
    angle = pos_data->angle;

    /* calculate the relative angle against north, clockwise  */
    if (lat2 >= lat1){
    /* north */
        if (lng2 >= lng1){
        /* easts */
            //equal
        }
        else{
            angle = 360-angle;
        }
    }
    else{
    /* south */
        if (lng2 >= lng1){
        /* easts */
            angle = 180-angle;
        }
        else{
            angle = 180+angle;
        }
    }

    /* calculate the angle detra between local front and remote position  */
    if (angle > p_src->dir){
        delta = angle - p_src->dir;
    }
    else{
        delta = p_src->dir - angle;
    }

    if (delta > 180){
        delta = 360 - delta;
    }

    distance_1_2 *= 1000; /* convert from Km to m */            

    return (delta <= 45)? distance_1_2:(-distance_1_2);
}

float vsm_get_relative_pos(vam_stastatus_t *p_src, vam_stastatus_t *p_dest)
{
    double lat1, lng1, lat2, lng2, lat3, lng3;
    float distance_1_2, distance_2_3;
    float angle, delta;

    /* reference point */
    lat1 = p_src->pos.latitude;
    lng1 = p_src->pos.longitude;

    /* destination point */
    lat2 = p_dest->pos.latitude;
    lng2 = p_dest->pos.longitude;

    /* temp point */
    lat3 = lat1;
    lng3 = lng2;

    distance_1_2 = getDistanceVer2(lat1, lng1, lat2, lng2);
    distance_2_3 = getDistanceVer2(lat2, lng2, lat3, lng3);
    angle = acos(distance_2_3/distance_1_2)*180/PI;

    /* calculate the relative angle against north, clockwise  */
    if (lat2 >= lat1){
    /* north */
        if (lng2 >= lng1){
        /* easts */
            //equal
        }
        else{
            angle = 360-angle;
        }
    }
    else{
    /* south */
        if (lng2 >= lng1){
        /* easts */
            angle = 180-angle;
        }
        else{
            angle = 180+angle;
        }
    }

    /* calculate the angle detra between local front and remote position  */
    if (angle > p_src->dir){
        delta = angle - p_src->dir;
    }
    else{
        delta = p_src->dir - angle;
    }

    if (delta > 180){
        delta = 360 - delta;
    }

    distance_1_2 *= 1000; /* convert from Km to m */            

    return (delta <= 45)? distance_1_2:(-distance_1_2);
}

float vsm_get_relative_dir(const vam_stastatus_t *p_src, const  vam_stastatus_t *p_dest)
{
    float direction = 0;


    /* Calculate the angle between local and neighbour node.  */
    if( p_src->dir <= p_dest->dir)
    {
        direction = p_dest->dir - p_src->dir;
    }
    else
    {
        direction = 360 + p_dest->dir - p_src->dir;
    }

    return direction;
}

int8_t vsm_get_rear_dir(vam_stastatus_t *p_dest)
{
	if((p_dest->dir>270)||(p_dest->dir<90))
		return -1;
	else return 1;
	
}


/* calculate the real time current value */
int32_t vsm_get_dr_current(vam_stastatus_t *last, vam_stastatus_t *current)
{
    float deltaT = 0.0;
    double v, s, dR;
    double dir; /* Radians */
    double lon1,lat1,lon2,lat2;
  	uint32_t t = osal_get_systemtime();
    
	if(!last || !current)
    {
        return -1;
    }

    deltaT = ((t>=last->time) ? (t-last->time) : \
            (t+0xFFFFFFFF - last->time)) / 1000.0f;

    memcpy(current, last, sizeof(vam_stastatus_t));
    if((deltaT <= 20) || (last->speed < 10))
    {
        return 0;
    }
    
    /* deltaT != 0, the calculate the "current" value */
    lon1 = RAD(last->pos.longitude);
    lat1 = RAD(last->pos.latitude);
    dir = RAD(last->dir);
    
    /* uniform rectilinear motion */ 
    v = last->speed / 3.6f;
    s = v*deltaT; 
    
	/* lat2 = asin( sin lat1 * cos dR + cos lat1 * sin dR * cos �� )
    lon2 = lon1 + atan2( sin �� * sin dR * cos lat1, cos dR- sin lat1 * sin lat2 )
    where lat is latitude, lon is longitude, ��is the bearing (clockwise from north), 
    dR is the angular distance d/R; d being the distance travelled, R the earth��s radius */
    dR = s / EARTH_RADIUS / 1000.0;
    lat2 = asin(sin(lat1)*cos(dR) + cos(lat1)*sin(dR)*cos(dir));
    lon2 = lon1 + atan2(sin(dir)*sin(dR)*cos(lat1), cos(dR)-sin(lat1)*sin(lat2));


    current->time = t;

    current->pos.longitude = lon2 * 180.0 / PI;
    current->pos.latitude = lat2 * 180.0 / PI;

#if 0
    char buf[100];
    sprintf(buf, "(lon=%f,lat=%f),h=%f,d=%f,s=%f,v=%f", current->pos.lon, current->pos.lat, current->dir, 
                                      vsm_get_relative_pos(last, current, 0), s, v);
    osal_printf("c%d%s\r\n", t, buf);
#endif
    return 0;
}
