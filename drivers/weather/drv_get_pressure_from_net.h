#ifndef _DRV_GET_PRESSURE_FROM_NET_H_
#define _DRV_GET_PRESSURE_FROM_NET_H_

#define JSON_DATA_BUF    8192  /*The buf size of the json number obtained from the network*/

/*****************************************************
func:drv_get_pressure_init
des:get the pressure from net init function
return:ERR_OK is successful, other value is failed
*****************************************************/
int drv_get_pressure_init();

/*****************************************************
func:drv_get_pressure_deinit
des:get the pressure from net deinit function
return:ERR_OK is successful, other value is failed
*****************************************************/
int drv_get_pressure_deinit();

/*****************************************************
func:get_pressure_from_net
des:get the pressure from net
para:longitude is longitude of the station,latitude is latitude of the station,
     pressure is the pressure of that getting from net
return:ERR_OK is successful, other value is failed
*****************************************************/
int get_pressure_from_net(double longitude, double latitude, double *pressure);

#endif