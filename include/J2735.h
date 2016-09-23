/******************************************************************************
	项目名称	：V2X项目
	文件		：J2735.h
	描述		：本文件定义了J2735协议接口函数
	版本		：0.1
	作者		：pengrui
	创建日期	：2012.6.24
******************************************************************************/

#ifndef _J2735_H_
#define _J2735_H_

#include "app_msg_format.h"
/*************************************************
  宏定义
*************************************************/

/*************************************************
  结构类型定义
*************************************************/

/*************************************************
  静态全局变量定义
*************************************************/

/*************************************************
  API
*************************************************/

inline int16_t encode_acceleration(float acceleration);
/******************************************************************************
*	函数:	decode_acceleration
*	功能:	将加速度数据从1m/s^2转换为以0.01m/s^2为单位
*	参数:	acceleration		- 	加速度数据
*	返回:	-20.00 - +20.00		-	正常数据
			+20.01				-	无效数据
*	说明:	纵向加速度指沿 X 轴（沿车辆从前到后的中线方向）的加速度，负值表示减速或刹车。
*			横向加速度指沿 Y 轴（垂直于车辆行驶方向从左到右的中线方向）的加速度。
 ******************************************************************************/
inline float decode_acceleration(int16_t acceleration);
/******************************************************************************
*	函数:	encode_angle
*	功能:	将角度数据从1°为单位转换为以0.0125°为单位
*	参数:	angle				- 	角度数据
*	返回:	0 - 28799			-	正常数据
*	 		28800				-	无效数据
*	说明:	表示目标节点的运动方向，即相对于正北方向(或指定某一基准方向)的以 0.0125 度
*			 为单位的顺时针夹角.数值 28799 表示 359.9875 度，数值28800 表示角度无效
 ******************************************************************************/
inline uint16_t encode_angle(float angle);
/******************************************************************************
*	函数:	encode_angle
*	功能:	将角度数据从0.0125°为单位转换为以1°为单位
*	参数:	angle				- 	角度数据
*	返回:	0 - 359.9875		-	正常数据
*	 		360.000				-	无效数据
*	说明:	表示目标节点的运动方向，即相对于正北方向(或指定某一基准方向)的以 0.0125 度
*			 为单位的顺时针夹角.数值 28799 表示 359.9875 度，数值28800 表示角度无效
 ******************************************************************************/
inline float decode_angle(uint16_t angle);
/******************************************************************************
*	函数:	encode_elevation
*	功能:	将海拔数据从1米为单位转换为以1分米为单位
*	参数:	elevation			- 	海拔数据
*	返回:	-4096 - +61439		-	正常数据
*	 		-4096				-	未知数据
*	说明:	海拔表示高于或低于参考平面的地理位置，以 1 分米为基本单位，高于 6143.9 米
*			 则表示为+61439，低于-409.5 米则表示为-4095，如果发送设备不知道自己的海
*			 拔数据，则表示为-4096
 ******************************************************************************/
inline int32_t encode_elevation(float elevation);
/******************************************************************************
*	函数:	decode_elevation
*	功能:	将海拔数据从1分米为单位转换为以1米为单位
*	参数:	elevation			- 	海拔数据
*	返回:	-409.6 - +6143.9	-	正常数据
*	 		-409.6				-	未知数据
*	说明:	海拔表示高于或低于参考平面的地理位置，以 1 分米为基本单位，高于 6143.9 米
*			 则表示为+61439，低于-409.5 米则表示为-4095，如果发送设备不知道自己的海
*			 拔数据，则表示为-4096
 ******************************************************************************/
inline float decode_elevation(int32_t elevation);
/******************************************************************************
*	函数:	encode_latitude
*	功能:	将纬度数据从1度为单位转换为以0.1个微度为单位
*	参数:	latitude					- 	纬度数据
*	返回:	-900000000 - +900000000		-	正常数据
*	 		900000001					-	无效数据
*	说明:	表示以 0.1 个微度为单位的物体位置，当值为 900000001 时，纬度数据无效
 ******************************************************************************/
inline int32_t encode_latitude(double latitude);
/******************************************************************************
*	函数:	decode_latitude
*	功能:	将纬度数据从0.1个微度为单位转换为以1度为单位
*	参数:	latitude				- 	纬度数据
*	返回:	-90.00 - +90.00			-	正常数据
*	 		>90.00					-	无效数据
*	说明:	表示以 0.1 个微度为单位的物体位置，当值为 900000001 时，纬度数据无效。
 ******************************************************************************/
inline double decode_latitude(int32_t latitude);
/******************************************************************************
*	函数:	encode_longitude
*	功能:	将经度数据从以1度为单位转换为以0.1微度为单位
*	参数:	angle							- 	经度数据
*	返回:	-1799999999 - +1800000000		-	正常数据
*	 		+1800000001						-	无效数据
*	说明:	表示以 0.1 个微度为单位的物体位置，当值为 1800000001 时，经度数据无效。
 ******************************************************************************/
inline int32_t encode_longitude(double longitude);
/******************************************************************************
*	函数:	decode_longitude
*	功能:	将经度数据从以0.1微度为单位转换为以1度为单位
*	参数:	angle							- 	经度数据
*	返回:	-180.00 - +180.00				-	正常数据
*	 		>180.00							-	无效数据
*	说明:	表示以 0.1 个微度为单位的物体位置，当值为 1800000001 时，经度数据无效。
 ******************************************************************************/
inline double decode_longitude(int32_t longitude);
/******************************************************************************
*	函数:	encode_semimajor_axis_accuracy
*	功能:	将长半轴准确度数据从1m为单位转换为以5cm为单位
*	参数:	accuracy			- 	长半轴准确度数据
*	返回:	0 - 255				-	正常数据
*	 		255					-	无效数据
*	说明:	描述椭圆的长半径的准确度，该参数以 5cm 为最小步进，可以从 GNSS 系统中得到
*	，典型的值为 1 个 sigma 的准确度。任何大于254的值用254标识，255代表无效值
 ******************************************************************************/
inline uint8_t encode_semimajor_axis_accuracy(float accuracy);
/******************************************************************************
*	函数:	decode_semimajor_axis_accuracy
*	功能:	将长半轴准确度数据从5cm为单位转换为以1m为单位
*	参数:	accuracy			- 	长半轴准确度数据
*	返回:	0 - 12.7			-	正常数据
*	 		12.75				-	无效数据
*	说明:	描述椭圆的长半径的准确度，该参数以 5cm 为最小步进，可以从 GNSS 系统中得到
*	，典型的值为 1 个 sigma 的准确度。任何大于254的值用254标识，255代表无效值
 ******************************************************************************/
inline float decode_semimajor_axis_accuracy(uint8_t accuracy);
/******************************************************************************
*	函数:	encode_semimajor_axis_orientation
*	功能:	将长半轴方向数据从1度为单位转换为以0.0054932479度为单位
*	参数:	orientation			- 	长半轴方向数据
*	返回:	0 - 65535			-	正常数据
*	 		65525				-	无效数据
*	说明:	主要描述椭圆长半轴相对于坐标系统的方向角，可以从 GNSS系统中得到。
*			65534表示任何大于65534的数据，65535表示未知数据用.
 ******************************************************************************/
inline uint16_t encode_semimajor_axis_orientation(float orientation);
/******************************************************************************
*	函数:	decode_semimajor_axis_orientation
*	功能:	将长半轴方向数据从1度为单位转换为以0.0054932479度为单位
*	参数:	orientation			- 	长半轴方向数据
*	返回:	0 - 359.9945078786	-	正常数据
*	 		360					-	无效数据
*	说明:	主要描述椭圆长半轴相对于坐标系统的方向角，可以从 GNSS系统中得到。
*			359.9945078786表示任何大于65534的数据，360表示未知数据用.
 ******************************************************************************/
inline float decode_semimajor_axis_orientation(uint16_t orientation);
/******************************************************************************
*	函数:	encode_semiminor_axis_accuracy
*	功能:	将短半轴准确度数据从1m为单位转换为以0.05m为单位
*	参数:	accuracy				- 	角度数据
*	返回:	0 - 255				-	正常数据
*	 		255					-	无效数据
*	说明:	主要描述椭圆的短半径的准确度，该参数以 5cm 为最小步进，可以从 GNSS 系统中得到，
*	典型的值为 1 个 sigma 的准确度.
*	说明：	254表示大于或者等于254的值,255表示无效数据值.
 ******************************************************************************/
inline uint8_t encode_semiminor_axis_accuracy(float accuracy);
/******************************************************************************
*	函数:	decode_semiminor_axis_accuracy
*	功能:	将短半轴准确度数据从0.05m为单位转换为以1m为单位
*	参数:	accuracy				- 	角度数据
*	返回:	0 - 12.70			-	正常数据
*	 		12.75				-	无效数据
*	说明:	主要描述椭圆的短半径的准确度，该参数以 5cm 为最小步进，可以从 GNSS 系统中得到，
*	典型的值为 1 个 sigma 的准确度.
*	说明：	12.70表示大于或者等于12.70的值,12.75表示无效数据值.
 ******************************************************************************/
inline float decode_semiminor_axis_accuracy(uint8_t accuracy);
/******************************************************************************
*	函数:	encode_steer_wheel_angle
*	功能:	将方向盘转角数据从1度为单位转换为以1.5度为单位
*	参数:	angle				- 	角度数据
*	返回:	-126 - +127			-	正常数据
*	 		+127				-	无效数据
*	说明:	表司机方向盘转动的角度，这是一个有符号数值（右转为正值），最小单位是 1.5 度。
*	说明:	+126表示大于等于+126，+127表示无效数据
 ******************************************************************************/
inline int8_t encode_steer_wheel_angle(float angle);
/******************************************************************************
*	函数:	decode_steer_wheel_angle
*	功能:	将方向盘转角数据从1.5度为单位转换为以1度为单位
*	参数:	angle				- 	角度数据
*	返回:	-189 - +189			-	正常数据
*	 		+190.5				-	无效数据
*	说明:	表司机方向盘转动的角度，这是一个有符号数值（右转为正值），最小单位是 1 度。
*	说明:	+189表示大于等于+189，+190.5表示无效数据
 ******************************************************************************/
inline float decode_steer_wheel_angle(int8_t angle);
/******************************************************************************
*	函数:	encode_vehicle_width
*	功能:	将车辆宽度数据从1m为单位转换为以1cm为单位
*	参数:	width				- 	车辆宽度数据
*	返回:	0 - 1023			-	正常数据
*	说明:	车辆宽度）以 cm 为单位，无符号数。宽度数值应该是车辆装配上所有出厂设备后的最宽
			的点， 0 表示数据无效
 ******************************************************************************/
inline uint16_t encode_vehicle_width(float width);
/******************************************************************************
*	函数:	decode_vehicle_width
*	功能:	将车辆宽度数据从1cm为单位转换为以1m为单位
*	参数:	width				- 	车辆宽度数据
*	返回:	0 - 10.23			-	正常数据
*	说明:	车辆宽度）以 cm 为单位，无符号数。宽度数值应该是车辆装配上所有出厂设备后的最宽
			的点， 0 表示数据无效
 ******************************************************************************/
inline float decode_vehicle_width(uint16_t width);
/******************************************************************************
*	函数:	encode_vehicle_length
*	功能:	将车辆长度数据从1m为单位转换为以1cm为单位
*	参数:	length				- 	车辆长度数据
*	返回:	0 - 4095			-	正常数据
*	说明:	从车辆前保险杠边缘到车辆后保险杠的边缘长度，以 cm 为单位，无符号数，
			0 表示数据无效。
 ******************************************************************************/
inline uint16_t encode_vehicle_length(float length);
/******************************************************************************
*	函数:	decode_vehicle_length
*	功能:	将车辆长度数据从1cm为单位转换为以1m为单位
*	参数:	length				- 	车辆长度数据
*	返回:	0 - 40.95			-	正常数据
*	说明:	从车辆前保险杠边缘到车辆后保险杠的边缘长度，以 m 为单位，无符号数，
			0 表示数据无效。
 ******************************************************************************/
inline float decode_vehicle_length(uint16_t length);
/******************************************************************************
*	函数:	encode_absolute_velocity
*	功能:	将车辆绝对速度数据从1km/h为单位转换为以0.02m/s为单位
*	参数:	velocity			- 	车辆绝对速度数据
*	返回:	0 - 8191			-	正常数据
*	 		8191				-	无效数据
*	说明:	指目标节点的行驶速度或者道路指导速度，以 1km/h 为单位，若速度无效则值为8191。
 ******************************************************************************/
inline uint16_t encode_absolute_velocity(float velocity);
/******************************************************************************
*	函数:	decode_absolute_velocity
*	功能:	将车辆绝对速度数据从0.02m/s为单位转换为以1km/h为单位
*	参数:	velocity			- 	车辆绝对速度数据
*	返回:	0 - 163.80			-	正常数据
*	 		163.82				-	无效数据
*	说明:	指目标节点的行驶速度或者道路指导速度，以1km/h 为单位，若速度无效则值为8191。
 ******************************************************************************/
inline float decode_absolute_velocity(uint16_t velocity);
/******************************************************************************
*	函数:	encode_relative_velocity
*	功能:	将车辆相对速度数据从1km/h为单位转换为以0.02m/s为单位
*	参数:	velocity			- 	车辆绝对速度数据
*	返回:	-8191 - 8191		-	正常数据
*	 		8191				-	无效数据
*	说明:	指目标节点的相对某一参照物下的行驶速度或者道路指导速度，以 0.02 m/s 为单位
*			，若速度无效则值为8191。
 ******************************************************************************/
inline int16_t encode_relative_velocity(float velocity);
/******************************************************************************
*	函数:	decode_relative_velocity
*	功能:	将车辆绝对速度数据从0.02m/s为单位转换为以1km/h为单位
*	参数:	velocity			- 	车辆绝对速度数据
*	返回:	-163.80 - 163.80	-	正常数据
*	 		163.82				-	无效数据
*	说明:	指目标节点的行驶速度或者道路指导速度，以 0.02 m/s 为单位，若速度无效则值为8191。
 ******************************************************************************/
inline float decode_relative_velocity(int16_t velocity);
/******************************************************************************
*	函数:	encode_vertical_acceleration
*	功能:	将垂直加速度数据从19.80665m/s^2为单位转换为以0.1962m/s^2为单位
*	参数:	acceleration				- 	角度数据
*	返回:	-127 - +127			-	正常数据
*	 		+127				-	无效数据
*	说明:	指车辆在垂直轴上的加速度值，单位 0.02 G（ G = 9.80665 m/s^2,
			0.02 G = 0.1962 m/s^2）。
			+127表示大于或者等于127，-126表示小于或者等于-126，-127表示无效数据
 ******************************************************************************/
inline int8_t encode_vertical_acceleration(float acceleration);
/******************************************************************************
*	函数:	decode_vertical_acceleration
*	功能:	将垂直加速度数据从0.1962m/s^2为单位转换为以19.80665m/s^2为单位
*	参数:	acceleration		- 	角度数据
*	返回:	-127 - +127			-	正常数据
*	 		+127				-	无效数据
*	说明:	指车辆在垂直轴上的加速度值，单位 0.02 G（ G = 9.80665 m/s^2,
			0.02 G = 0.1962 m/s^2）。
			+2.54G表示>=2.54G，-2.52G表示<=2.52G，-2.54G表示无效数据
 ******************************************************************************/
inline float decode_vertical_acceleration(int8_t acceleration);
/******************************************************************************
*	函数:	encode_yawrate
*	功能:	将偏航率度数据从1度为单位转换为以0.01度为单位
*	参数:	yawrate				- 	角度数据
*	返回:	-32767 - +32767		-	正常数据
*	说明:	表示车辆的偏航率，是一个以 0.01 度/秒为单位的有符号数值（向右为正），该值可
			以表示车辆在特定时间段内绕垂直轴 Z 的旋转速率
 ******************************************************************************/
inline int16_t encode_yawrate(float yawrate);
/******************************************************************************
*	函数:	decode_yawrate
*	功能:	将偏航率度数据从0.01度为单位转换为以1度为单位
*	参数:	yawrate				- 	角度数据
*	返回:	-327.67 - +327.67	-	正常数据
*	说明:	表示车辆的偏航率，是一个以 0.01 度/秒为单位的有符号数值（向右为正），该值可
			以表示车辆在特定时间段内绕垂直轴 Z 的旋转速率
 ******************************************************************************/
inline float decode_yawrate(int16_t yawrate);
/******************************************************************************
*	函数:	encode_vehicle_alert_flag
*	功能:	将告警标识数据进行转换
*	参数:	warning_id			- 	告警标识数据
*	返回:	0 - 0xFFFFFFFF		-	正常数据
*	说明:	AlertFlag（告警标记）中每 1 位代表 1 个告警状态，有效为 1 无效为 0,各告警位之间相互独立
			(‘00000000 00000000 00000000 00000000’B)		-- 无任何告警
			(‘00000000 00000000 00000000 00000001’B)		-- 前车近距离告警
			(‘00000000 00000000 00000000 00000010’B)		-- 后车近距离告警
			(‘00000000 00000000 00000000 00000100’B)		-- 车辆故障告警
			(‘00000000 00000000 00000000 00001000’B)		-- 紧急刹车告警
			(‘00000000 00000000 00000000 00010000’B)		-- 救护车告警
			(‘00000000 00000000 00000000 00100000’B)		-- 危险货物运输车告警
			(‘00000000 00000000 00000000 01000000’B)		-- 隧道告警
 ******************************************************************************/
inline uint32_t encode_vehicle_alert_flag(uint16_t warning_id);
/******************************************************************************
*	函数:	encode_vehicle_alert_flag
*	功能:	将告警标识数据进行转换
*	参数:	warning_id			- 	告警标识数据
*	返回:	0 - 0xFFFFFFFF		-	正常数据
*	说明:	AlertFlag（告警标记）中每 1 位代表 1 个告警状态，有效为 1 无效为 0,各告警位之间相互独立
			(‘00000000 00000000 00000000 00000000’B)		-- 无任何告警
			(‘00000000 00000000 00000000 00000001’B)		-- 前车近距离告警
			(‘00000000 00000000 00000000 00000010’B)		-- 后车近距离告警
			(‘00000000 00000000 00000000 00000100’B)		-- 车辆故障告警
			(‘00000000 00000000 00000000 00001000’B)		-- 紧急刹车告警
			(‘00000000 00000000 00000000 00010000’B)		-- 救护车告警
			(‘00000000 00000000 00000000 00100000’B)		-- 危险货物运输车告警
			(‘00000000 00000000 00000000 01000000’B)		-- 隧道告警
 ******************************************************************************/
inline uint16_t decode_vehicle_alert_flag(uint32_t x);

#endif /* _J2735_H_ */
