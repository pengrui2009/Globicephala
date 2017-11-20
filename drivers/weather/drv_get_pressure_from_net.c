/*
  Copyright (c) 2009 Dave Gamble
 
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
 
  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.
 
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <curl/curl.h>
#include <error.h>

#include "cJSON.h"
#include "drv_get_pressure_from_net.h"

/*global json data buf*/
static char g_json_data[JSON_DATA_BUF];
static char drv_get_pressure_init_flag = 0; // 0:drv_get_pressure uninit, 1:drv_get_pressure init 
 
#if 0
/*****************************************************
func:get_xml_key_value
des:Gets the corresponding value in xml according to key
para:xml is the xml data,key is the keyword you need to find
return:Returns the value corresponding to the keyword
*****************************************************/
char * get_xml_key_value (const char * xml, char * key)
{
    char * head;
    char * tail;
    char stamp[1024];
    char * value;

    sprintf(stamp, "<%s>", key);
    if ((head = strstr (xml, stamp)) == NULL)
        return NULL;

    head = head + strlen (stamp);
    //Here the meaning of the front of the key + 2 characters such as <city> Beijing </ city> get rid of the city + 2 characters left Beijing </ city>
    while (isspace (head[0]))//To determine whether the first is a null character
        head ++;

    sprintf(stamp, "< / %s", key);

    if ((tail = strstr (head, stamp)) == NULL)
        return NULL;

    while (isspace(tail[-1]) && (tail > head))
        tail --;

    if (tail > head){
        value = calloc (tail - head + 1, 1);
        memcpy (value, head, tail - head);//Here is very important, because here the use of the tail = strstr (head, stamp) can be used to get the extra subtraction of the number of characters
        value[tail - head] = 0x00;//The last character is 0x0
    }

    return value;
}
#endif 

/*****************************************************
func:cJSON_weather_to_pressure
des:Get the pressure from the json data
para:text is the json data,pressure is the pressure that getting from json data
return:ERR_OK is successful, other value is failed
*****************************************************/
static int cJSON_weather_to_pressure(char *text, double *pressure)  
{  
    cJSON *json,*arrayItem,*item,*object,*object1;  
    int i;  
  
    json = cJSON_Parse(text);  /* Default options for cJSON_Parse */
    if (!json)  
    {  
        printf("Error before: [%s]\n",cJSON_GetErrorPtr());  
        return -ERR_INVAL;
    }  
    else  
    {  
        item = cJSON_GetObjectItem(json,"status");  /*Find the 'status' object*/
        if(item != NULL)
        {
            if(strcmp(item->valuestring, "OK"))  /*Determine the corresponding value*/
            {
                printf("status error:[%s]",item->valuestring);
                return -ERR_SYS;
            }
        }
        arrayItem = cJSON_GetObjectItem(json,"weather"); /*Find the 'weather' object*/
        if(arrayItem != NULL)  
        {  
            int size = cJSON_GetArraySize(arrayItem);  /*Get the size of the array*/
            //printf("cJSON_GetArraySize: size=%d\n",size);  

            /*According to the size of the array cycle*/
            for(i = 0;i < size;i++)  
            {  
                //printf("i=%d\n",i);  
                object = cJSON_GetArrayItem(arrayItem,i);  /*Get the corresponding object based on index i*/
  
                object1 = cJSON_GetObjectItem(object,"now");  /*Find the 'now' object*/
                if(object1 != NULL)  
                {  
                    //printf("cJSON_GetObjectItem: type=%d, string is %s\n",object1->type,object1->string);  
                    item = cJSON_GetObjectItem(object1,"pressure"); /*Find the 'pressure' object*/
                    if(item != NULL)
                    {
                        *pressure = atoi(item->valuestring);
                        //printf("cJSON_GetObjectItem: type=%d, string is %s\n",item->type,item->string);
                        //printf("valueint=%d,valuedouble=%f,valuestring=%s\n",item->valueint,item->valuedouble,item->valuestring);
                    }
                }  
            }  
        }    
  
        cJSON_Delete(json);  /*release resource*/
    }  
    return ERR_OK;  
}  

/*****************************************************
func:cJson_coor_to_cityID
des:Get the city id from the json data
para:text is the json data,cityID is the cityID that getting from json data
return:ERR_OK is successful, other value is failed
*****************************************************/
/* example:
{
    "results":[
        {
            "id":"WX4G17JWZEK7",
            "name":"朝阳",
            "country":"CN",
            "path":"朝阳,北京,中国",
            "timezone":"Asia/Shanghai",
            "timezone_offset":"+08:00"
        }
    ]
}
*/
static int cJson_coor_to_cityID(char *text,char *cityID)
{
    cJSON *json,*arrayItem,*item,*object;
    int i;  
  
    json = cJSON_Parse(text);  /* Default options for cJSON_Parse */
    if (!json)  
    {  
        printf("Error before: [%s]\n",cJSON_GetErrorPtr());  
        return -ERR_INVAL;
    }  
    else  
    {  
        arrayItem = cJSON_GetObjectItem(json,"results"); /*Find the 'results' object*/
        if(arrayItem != NULL)  
        {  
            int size=cJSON_GetArraySize(arrayItem);  /*Get the size of the array*/
            //printf("cJSON_GetArraySize: size=%d\n",size);  

            /*According to the size of the array cycle*/
            for(i = 0;i < size;i++)  
            {  
                //printf("i=%d\n",i);  
                object = cJSON_GetArrayItem(arrayItem, i);  /*Get the corresponding object based on index i*/
  
                item = cJSON_GetObjectItem(object,"id");  /*Find the 'id' object*/
                if(item != NULL)  
                {  
                    //printf("cJSON_GetObjectItem: type=%d, string is %s\n",item->type,item->string);  
                    strcpy(cityID, item->valuestring);
                } 
                item = cJSON_GetObjectItem(object,"name");  /*Find the 'name' object*/
                if(item != NULL)  
                {  
                    //printf("cJSON_GetObjectItem: type=%d, string is %s\n",item->type,item->string);  
                    printf("cityName:[%s]\n",item->valuestring);
                }
            }  
        } 
        else
        {
            printf("can't find query result\n");
            cJSON_Delete(json); /*release resource*/
            return -ERR_SYS;
        }
        cJSON_Delete(json);  /*release resource*/
    }  
    return ERR_OK;  
}

/*****************************************************
func:down_data_callback
des:Download the data callback function
para:fixed format,size is the length of one block,nmemb is the number of blocks
return:Returns the data length 
*****************************************************/
static size_t down_data_callback(void* buffer,size_t size,size_t nmemb,void *stream)
{      
    printf("debug:%s\n",(char *)buffer);  
    //printf("size=%d,nmenb=%d\n",size, nmemb);
    memset(g_json_data, 0, sizeof(g_json_data));
    memcpy(g_json_data, buffer, size*nmemb);
    return size*nmemb;  //Returns the data length
}

/*****************************************************
func:curl_get_content
des:get the content of the net
para:url is the site of content
return:ERR_OK is successful, other value is failed
*****************************************************/
static uint32_t curl_get_content (const char * url)
{
    if (url == NULL)
        return -ERR_INVAL;

    CURL * curl = curl_easy_init (); //init libcurl 
    curl_easy_setopt (curl, CURLOPT_URL, url); //Set the target URL address
    curl_easy_setopt (curl, CURLOPT_TIMEOUT, 15); //Download timeout
    curl_easy_setopt (curl, CURLOPT_NOSIGNAL, 1); //Shielding other signals
    curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION, down_data_callback); //Set download callback function
    //curl_easy_setopt (curl, CURLOPT_WRITEDATA, content); //Set download data save buffer
    CURLcode retval = curl_easy_perform (curl); //Execute the data request
    if (retval == CURLE_OK) {
        double down_length = 0;
        curl_easy_getinfo (curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &down_length); //Get download data length
        curl_easy_cleanup (curl); //Release resources
        return (uint32_t)down_length; //Return length
    }
    curl_easy_cleanup (curl); //Release resources
    return -ERR_SYS;
}

/*****************************************************
func:get_pressure_from_net
des:get the pressure from net
para:longitude is longitude of the station,latitude is latitude of the station,
     pressure is the pressure of that getting from net
return:ERR_OK is successful, other value is failed
*****************************************************/
int get_pressure_from_net(double longitude, double latitude, double *pressure)
{
    int ret = ERR_OK;
    /*the site is that translating coordinate to cityid*/
    char url_cityId[] = "weixin.jirengu.com/weather/cityid?location=39.9852:116.4995";
    /*the site is that getting the information of pressure*/
    char url_weather[512] = "weixin.jirengu.com/weather/now?cityid=";//"weixin.jirengu.com/weather";WX4FBXXFKE4F
    char cityID[50]; //city id

    if(pressure == NULL)
    {
        printf("pressure is invalid\n");
        return -ERR_INVAL;
    }

    /*get the city ID*/
    ret = curl_get_content (url_cityId);
    if(ret < 0)
    {
        printf("curl_get_content failed,ret=%d\n",ret);
        goto error;
    }

    /*translate json data to cityID*/
    ret = cJson_coor_to_cityID(g_json_data, cityID);
    if(ret < 0)
    {
        printf("cJson_coor_to_cityID failed,ret=%d\n",ret);
        goto error;
    }
    strcat(url_weather, cityID);

    /*get the weather data*/
    ret = curl_get_content (url_weather);
    if(ret < 0)
    {
        printf("curl_get_content failed,ret=%d\n",ret);
        goto error;
    }

    /*translate json data to pressure*/
    ret = cJSON_weather_to_pressure(g_json_data, pressure);
    if(ret < 0)
    {
        printf("cJSON_to_struct_array failed,ret=%d\n",ret);
        goto error;
    }
    
    printf("pressure:%f[hPa]\n",*pressure);

error:
    return ret;
}

/*****************************************************
func:drv_get_pressure_init
des:get the pressure from net init function
return:ERR_OK is successful, other value is failed
*****************************************************/
int drv_get_pressure_init()
{
    int ret = ERR_OK;
    CURLcode return_code;

    if(drv_get_pressure_init_flag)
    {
        printf("drv_get_pressure_init has been initialized\n");
        return ERR_OK;
    }
    memset(g_json_data, 0, JSON_DATA_BUF);

    /*init libcurl*/
    return_code = curl_global_init(CURL_GLOBAL_ALL);
    if (CURLE_OK != return_code) {
        printf("initlibcurl failed.\n");
        /*release libcurl*/
        curl_global_cleanup();
           ret = -ERR_SYS;
    }

    drv_get_pressure_init_flag = 1;
    
    return ret;
}

/*****************************************************
func:drv_get_pressure_deinit
des:get the pressure from net deinit function
return:ERR_OK is successful, other value is failed
*****************************************************/
int drv_get_pressure_deinit()
{
    int ret = ERR_OK;

    if(!drv_get_pressure_init_flag)
    {
        printf("drv_get_pressure_init has been deinitialized\n");
        return ERR_OK;
    }
    memset(g_json_data, 0, JSON_DATA_BUF);
    
    /*release curl resource*/
    curl_global_cleanup();
    drv_get_pressure_init_flag = 0;
    
    return ret;
}


