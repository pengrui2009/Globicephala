/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file     : ehmh_element.c
 @brief  : ehmh protocol element encode and decode API
 @author : pengrui
 @history:
           2017-11-23       pengrui      Created file
           2017-11-27       pengrui      Modify the velocity encode and decode function
           2017-11-28       pengrui      Modify the byte convert bug
           ...
******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include "osal_cmn.h"
#include "ehmh_element.h"
#include "prot_element.h"
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
  API函数实现
*************************************************/
/*************************************************
  操作底层所用到的函数
*************************************************/

/******************************************************************************
*    函数:    encode_acceleration
*    功能:    将加速度数据从0.01m/s^2为单位转换为以1m/s^2为单位
*    参数:    acceleration        -     加速度数据
*    返回:    -2000 - +2000         -    正常数据
            +2001                -    无效数据
*    说明:    纵向加速度指沿 X 轴（沿车辆从前到后的中线方向）的加速度，负值表示减速或刹车。
*            横向加速度指沿 Y 轴（垂直于车辆行驶方向从左到右的中线方向）的加速度。
 ******************************************************************************/
inline int16_t encode_acceleration(float acceleration)
{
    long value = 0;
    int16_t result = 0;
    
    value = bsctyp_encode(DE_Acceleration, acceleration);
    
    /* unit 0.01m/s^2. */
    result = (int16_t)(value);

    return cv_ntohs(result);
}
/******************************************************************************
*    函数:    decode_acceleration
*    功能:    将加速度数据从1m/s^2转换为以0.01m/s^2为单位
*    参数:    acceleration        -     加速度数据
*    返回:    -20.00 - +20.00        -    正常数据
            +20.01                -    无效数据
*    说明:    纵向加速度指沿 X 轴（沿车辆从前到后的中线方向）的加速度，负值表示减速或刹车。
*            横向加速度指沿 Y 轴（垂直于车辆行驶方向从左到右的中线方向）的加速度。
 ******************************************************************************/
inline float decode_acceleration(int16_t acceleration)
{
    float result = 0;
    double value = 0;

    value = bsctyp_decode(DE_Acceleration, cv_ntohs(acceleration));
    /* unit 0.01m/s^2. */
    result = (float)value;

    return result;
}

/******************************************************************************
*    函数:    encode_angle
*    功能:    将角度数据从1°为单位转换为以0.0125°为单位
*    参数:    angle                -     角度数据
*    返回:    0 - 28799            -    正常数据
*             28800                -    无效数据
*    说明:    表示目标节点的运动方向，即相对于正北方向(或指定某一基准方向)的以 0.0125 度
*             为单位的顺时针夹角.数值 28799 表示 359.9875 度，数值28800 表示角度无效
 ******************************************************************************/
inline uint16_t encode_angle(float heading)
{
    uint16_t   result = 0;
    long value = 0;

    value = bsctyp_encode(DE_Heading, heading);

    result = (uint16_t)value;
    
    return cv_ntohs(result);
}


/******************************************************************************
*    函数:    encode_angle
*    功能:    将角度数据从0.0125°为单位转换为以1°为单位
*    参数:    angle                -     角度数据
*    返回:    0 - 359.9875        -    正常数据
*             360.000                -    无效数据
*    说明:    表示目标节点的运动方向，即相对于正北方向(或指定某一基准方向)的以 0.0125 度
*             为单位的顺时针夹角.数值 28799 表示 359.9875 度，数值28800 表示角度无效
 ******************************************************************************/
inline float decode_angle(uint16_t heading)
{
    float  result = heading;
    double value = 0;

    value = bsctyp_decode(DE_Heading, cv_ntohs(heading));

    result = (float)value;

    return result;
}

/******************************************************************************
*    函数:    encode_elevation
*    功能:    将海拔数据从1米为单位转换为以1分米为单位
*    参数:    elevation            -     海拔数据
*    返回:    -4096 - +61439        -    正常数据
*             -4096                -    未知数据
*    说明:    海拔表示高于或低于参考平面的地理位置，以 1 分米为基本单位，高于 6143.9 米
*             则表示为+61439，低于-409.5 米则表示为-4095，如果发送设备不知道自己的海
*             拔数据，则表示为-4096
 ******************************************************************************/
inline int32_t encode_elevation(float elevation)
{
    int32_t result = 0;
    long value = 0;

    /* unit 10 cm. */
    value = bsctyp_encode(DE_Elevation, elevation);
    
    result = (int32_t)(value);

    return cv_ntohl(result);
}
/******************************************************************************
*    函数:    decode_elevation
*    功能:    将海拔数据从1分米为单位转换为以1米为单位
*    参数:    elevation            -     海拔数据
*    返回:    -409.6 - +6143.9    -    正常数据
*             -409.6                -    未知数据
*    说明:    海拔表示高于或低于参考平面的地理位置，以 1 分米为基本单位，高于 6143.9 米
*             则表示为+61439，低于-409.5 米则表示为-4095，如果发送设备不知道自己的海
*             拔数据，则表示为-4096
 ******************************************************************************/
inline float decode_elevation(int32_t elevation)
{
    float result = 0;
    double value = 0;

    /* unit 10 cm. */
    value = bsctyp_decode(DE_Elevation, cv_ntohl(elevation));
    
    result = (float)(value);

    return result;
}
/******************************************************************************
*    函数:    encode_latitude
*    功能:    将纬度数据从1度为单位转换为以0.1个微度为单位
*    参数:    latitude                    -     纬度数据
*    返回:    -900000000 - +900000000        -    正常数据
*             900000001                    -    无效数据
*    说明:    表示以 0.1 个微度为单位的物体位置，当值为 900000001 时，纬度数据无效
 ******************************************************************************/
inline int32_t encode_latitude(double latitude)
{
    int32_t    result = 0;
    long value = 0;

    /* unit 0.1 micro degree. */
    value = bsctyp_encode(DE_Latitude, latitude);
    
    result = (int32_t)value;
    
    return cv_ntohl(result);
}
/******************************************************************************
*    函数:    decode_latitude
*    功能:    将纬度数据从0.1个微度为单位转换为以1度为单位
*    参数:    latitude                -     纬度数据
*    返回:    -90.00 - +90.00            -    正常数据
*             >90.00                    -    无效数据
*    说明:    表示以 0.1 个微度为单位的物体位置，当值为 900000001 时，纬度数据无效。
 ******************************************************************************/
inline double decode_latitude(int32_t latitude)
{
    double  result = latitude;

    /* unit 0.1 micro degree. */
    result = bsctyp_decode(DE_Latitude, cv_ntohl(latitude));

    return result;
}
/******************************************************************************
*    函数:    encode_longitude
*    功能:    将经度数据从以1度为单位转换为以0.1微度为单位
*    参数:    angle                            -     经度数据
*    返回:    -1799999999 - +1800000000        -    正常数据
*             +1800000001                        -    无效数据
*    说明:    表示以 0.1 个微度为单位的物体位置，当值为 1800000001 时，经度数据无效。
 ******************************************************************************/
inline int32_t encode_longitude(double longitude)
{
    int32_t    result = 0;
    long value = 0;

    /* unit 0.1 micro degree. */
    result = bsctyp_decode(DE_Longitude, longitude);

    result = (int32_t)value;

    return cv_ntohl(result);
}

/******************************************************************************
*    函数:    decode_longitude
*    功能:    将经度数据从以0.1微度为单位转换为以1度为单位
*    参数:    angle                            -     经度数据
*    返回:    -179.9999999f - +180.00            -    正常数据
*             >180.00                            -    无效数据
*    说明:    表示以 0.1 个微度为单位的物体位置，当值为 1800000001 时，经度数据无效。
 ******************************************************************************/
inline double decode_longitude(int32_t longitude)
{
    double  result = longitude;

    /* unit 0.1 micro degree. */
    result = bsctyp_decode(DE_Longitude, cv_ntohl(longitude));
    
    return result;
}


/******************************************************************************
*    函数:    encode_semimajor_axis_accuracy
*    功能:    将长半轴准确度数据从1m为单位转换为以5cm为单位
*    参数:    accuracy            -     长半轴准确度数据
*    返回:    0 - 255                -    正常数据
*             255                    -    无效数据
*    说明:    描述椭圆的长半径的准确度，该参数以 5cm 为最小步进，可以从 GNSS 系统中得到
*    ，典型的值为 1 个 sigma 的准确度。任何大于254的值用254标识，255代表无效值
 ******************************************************************************/
inline uint8_t encode_semimajor_axis_accuracy(float accuracy)
{
    uint8_t result = 0;

    result = bsctyp_encode(DE_SemiMajorAxisAccuracy, accuracy);

    result = (uint8_t)result;
    
    return result;
}
/******************************************************************************
*    函数:    decode_semimajor_axis_accuracy
*    功能:    将长半轴准确度数据从5cm为单位转换为以1m为单位
*    参数:    accuracy            -     长半轴准确度数据
*    返回:    0 - 12.7            -    正常数据
*             12.75                -    无效数据
*    说明:    描述椭圆的长半径的准确度，该参数以 5cm 为最小步进，可以从 GNSS 系统中得到
*    ，典型的值为 1 个 sigma 的准确度。任何大于254的值用254标识，255代表无效值
 ******************************************************************************/
inline float decode_semimajor_axis_accuracy(uint8_t accuracy)
{
    float result = 0;
    double value = 0;

    result = bsctyp_decode(DE_SemiMajorAxisAccuracy, accuracy);

    result = (float)value;
    
    return result;
}
/******************************************************************************
*    函数:    encode_semimajor_axis_orientation
*    功能:    将长半轴方向数据从1度为单位转换为以0.0054932479度为单位
*    参数:    orientation            -     长半轴方向数据
*    返回:    0 - 65535            -    正常数据
*             65525                -    无效数据
*    说明:    主要描述椭圆长半轴相对于坐标系统的方向角，可以从 GNSS系统中得到。
*            65534表示任何大于65534的数据，65535表示未知数据用.
 ******************************************************************************/
inline uint16_t encode_semimajor_axis_orientation(float orientation)
{
    uint16_t result = 0;
    long value = 0;

    value = bsctyp_encode(DE_SemiMajorAxisOrientation, orientation);

    result = (uint16_t)value;

    return cv_ntohs(result);
}
/******************************************************************************
*    函数:    decode_semimajor_axis_orientation
*    功能:    将长半轴方向数据从1度为单位转换为以0.0054932479度为单位
*    参数:    orientation            -     长半轴方向数据
*    返回:    0 - 359.9945078786    -    正常数据
*             360                    -    无效数据
*    说明:    主要描述椭圆长半轴相对于坐标系统的方向角，可以从 GNSS系统中得到。
*            359.9945078786表示任何大于65534的数据，360表示未知数据用.
 ******************************************************************************/
inline float decode_semimajor_axis_orientation(uint16_t orientation)
{
    float result = 0;
    double value = 0;

    value = bsctyp_decode(DE_SemiMajorAxisOrientation, cv_ntohs(orientation));

    result = (uint16_t)value;

    return result;
}
/******************************************************************************
*    函数:    encode_semiminor_axis_accuracy
*    功能:    将短半轴准确度数据从1m为单位转换为以0.05m为单位
*    参数:    accuracy                -     角度数据
*    返回:    0 - 255                -    正常数据
*             255                    -    无效数据
*    说明:    主要描述椭圆的短半径的准确度，该参数以 5cm 为最小步进，可以从 GNSS 系统中得到，
*    典型的值为 1 个 sigma 的准确度.
*    说明：    254表示大于或者等于254的值,255表示无效数据值.
 ******************************************************************************/
inline uint8_t encode_semiminor_axis_accuracy(float accuracy)
{
    uint8_t result = 0;
    long value = 0;

    value = bsctyp_encode(DE_SemiMinorAxisAccuracy, accuracy);

    result = (uint8_t)value;
    
    return result;
}
/******************************************************************************
*    函数:    decode_semiminor_axis_accuracy
*    功能:    将短半轴准确度数据从0.05m为单位转换为以1m为单位
*    参数:    accuracy                -     角度数据
*    返回:    0 - 12.70            -    正常数据
*             12.75                -    无效数据
*    说明:    主要描述椭圆的短半径的准确度，该参数以 5cm 为最小步进，可以从 GNSS 系统中得到，
*    典型的值为 1 个 sigma 的准确度.
*    说明：    12.70表示大于或者等于12.70的值,12.75表示无效数据值.
 ******************************************************************************/
inline float decode_semiminor_axis_accuracy(uint8_t accuracy)
{
    float result = 0;
    double value = 0;

    value = bsctyp_decode(DE_SemiMinorAxisAccuracy, accuracy);

    result = (float)value;
    
    return result;
}
/******************************************************************************
*    函数:    encode_steer_wheel_angle
*    功能:    将方向盘转角数据从1度为单位转换为以1.5度为单位
*    参数:    angle                -     角度数据
*    返回:    -126 - +127            -    正常数据
*             +127                -    无效数据
*    说明:    表司机方向盘转动的角度，这是一个有符号数值（右转为正值），最小单位是 1.5 度。
*    说明:    +126表示大于等于+126，+127表示无效数据
 ******************************************************************************/
inline int8_t encode_steer_wheel_angle(float angle)
{
    uint8_t result = 0;
    long value = 0;

    value = bsctyp_encode(DE_SteeringWheelAngle, angle);

    result = (int8_t)value;
    
    return result;
}
/******************************************************************************
*    函数:    decode_steer_wheel_angle
*    功能:    将方向盘转角数据从1.5度为单位转换为以1度为单位
*    参数:    angle                -     角度数据
*    返回:    -189 - +189            -    正常数据
*             +190.5                -    无效数据
*    说明:    表司机方向盘转动的角度，这是一个有符号数值（右转为正值），最小单位是 1 度。
*    说明:    +189表示大于等于+189，+190.5表示无效数据
 ******************************************************************************/
inline float decode_steer_wheel_angle(int8_t angle)
{
    int result = 0;
    double value = 0;

    value = bsctyp_decode(DE_SteeringWheelAngle, angle);

    result = (float)value;

    return result;
}
/******************************************************************************
*    函数:    encode_vehicle_width
*    功能:    将车辆宽度数据从1m为单位转换为以1cm为单位
*    参数:    width                -     车辆宽度数据
*    返回:    0 - 1023            -    正常数据
*    说明:    车辆宽度）以 cm 为单位，无符号数。宽度数值应该是车辆装配上所有出厂设备后的最宽
            的点， 0 表示数据无效
 ******************************************************************************/
inline uint16_t encode_vehicle_width(float width)
{
    uint16_t result = 0;
    long value = 0;

    value = bsctyp_encode(DE_VehicleWidth, width);
    result = (uint16_t)value;

    return cv_ntohs(result);
}
/******************************************************************************
*    函数:    decode_vehicle_width
*    功能:    将车辆宽度数据从1cm为单位转换为以1m为单位
*    参数:    width                -     车辆宽度数据
*    返回:    0 - 10.23            -    正常数据
*    说明:    车辆宽度）以 cm 为单位，无符号数。宽度数值应该是车辆装配上所有出厂设备后的最宽
            的点， 0 表示数据无效
 ******************************************************************************/
inline float decode_vehicle_width(uint16_t width)
{
    float result = 0;
    double value = 0;

    value = bsctyp_decode(DE_VehicleWidth, cv_ntohs(width));
    
    result = (float)value;

    return result;
}
/******************************************************************************
*    函数:    encode_vehicle_length
*    功能:    将车辆长度数据从1m为单位转换为以1cm为单位
*    参数:    length                -     车辆长度数据
*    返回:    0 - 4095            -    正常数据
*    说明:    从车辆前保险杠边缘到车辆后保险杠的边缘长度，以 cm 为单位，无符号数，
            0 表示数据无效。
 ******************************************************************************/
inline uint16_t encode_vehicle_length(float length)
{
    uint16_t result = 0;
    long value = 0;

    value = bsctyp_encode(DE_VehicleLength, length);
    
    result = (uint16_t)value;

    return cv_ntohs(result);
}
/******************************************************************************
*    函数:    decode_vehicle_length
*    功能:    将车辆长度数据从1cm为单位转换为以1m为单位
*    参数:    length                -     车辆长度数据
*    返回:    0 - 40.95            -    正常数据
*    说明:    从车辆前保险杠边缘到车辆后保险杠的边缘长度，以 m 为单位，无符号数，
            0 表示数据无效。
 ******************************************************************************/
inline float decode_vehicle_length(uint16_t length)
{
    float result = 0;
    long value = 0;

    value = bsctyp_decode(DE_VehicleLength, cv_ntohs(length));
    
    result = (float)value;

    return result;
}

/******************************************************************************
*    函数:    encode_absolute_velocity
*    功能:    将车辆绝对速度数据从1km/h为单位转换为以0.02m/s为单位
*    参数:    velocity            -     车辆绝对速度数据
*    返回:    0 - 8191            -    正常数据
*             8191                -    无效数据
*    说明:    指目标节点的行驶速度或者道路指导速度，以 0.02 m/s 为单位，若速度无效则值为8191。
 ******************************************************************************/
inline uint16_t encode_absolute_velocity(float velocity)
{
    uint16_t    result = 0;
    long value = 0;

    value = bsctyp_encode(DE_Velocity, velocity);
    
    result = (uint16_t)value;

    return cv_ntohs(result);
}

/******************************************************************************
*    函数:    decode_absolute_velocity
*    功能:    将车辆绝对速度数据从0.02m/s为单位转换为以1km/h为单位
*    参数:    velocity            -     车辆绝对速度数据
*    返回:    0 - 163.80            -    正常数据
*             163.82                -    无效数据
*    说明:    指目标节点的行驶速度或者道路指导速度，以 0.02 m/s 为单位，若速度无效则值为8191。
 ******************************************************************************/
inline float decode_absolute_velocity(uint16_t velocity)
{
    float result = 0;
    double value = 0;

    value = bsctyp_decode(DE_Velocity, cv_ntohs(velocity));
    
    result = (float)value;
    
    return result;
}

/******************************************************************************
*    函数:    encode_relative_velocity
*    功能:    将车辆相对速度数据从1km/s为单位转换为以0.02m/s为单位
*    参数:    velocity            -     车辆绝对速度数据
*    返回:    -8191 - 8191        -    正常数据
*             8191                -    无效数据
*    说明:    指目标节点的相对某一参照物下的行驶速度或者道路指导速度，以 0.02 m/s 为单位
*            ，若速度无效则值为8191。
 ******************************************************************************/
inline int16_t encode_relative_velocity(float velocity)
{
    int16_t result = 0;
    long value = 0;

    //velocity * 100000.0f / 3600 / 2
    value = bsctyp_encode(DE_Velocity, velocity);
    
    result = (int16_t)value;

    return cv_ntohs(result);
}
/******************************************************************************
*    函数:    decode_relative_velocity
*    功能:    将车辆绝对速度数据从0.02m/s为单位转换为以1km/h为单位
*    参数:    velocity            -     车辆绝对速度数据
*    返回:    -163.80 - 163.80    -    正常数据
*             163.82                -    无效数据
*    说明:    指目标节点的行驶速度或者道路指导速度，以 0.02 m/s 为单位，若速度无效则值为8191。
 ******************************************************************************/
inline float decode_relative_velocity(int16_t velocity)
{
    float result = 0;
    double value = 0;
    
    //velocity * 3600 *2 / 100000.0f
    value = bsctyp_decode(DE_Velocity, cv_ntohs(velocity));
    
    result = (float)value;

    return result;
}
/******************************************************************************
*    函数:    encode_vertical_acceleration
*    功能:    将垂直加速度数据从19.80665m/s^2为单位转换为以0.1962m/s^2为单位
*    参数:    acceleration                -     角度数据
*    返回:    -127 - +127            -    正常数据
*             +127                -    无效数据
*    说明:    指车辆在垂直轴上的加速度值，单位 0.02 G（ G = 9.80665 m/s^2,
            0.02 G = 0.1962 m/s^2）。
            +127表示大于或者等于127，-126表示小于或者等于-126，-127表示无效数据
 ******************************************************************************/
inline int8_t encode_vertical_acceleration(float acceleration)
{
    int8_t result = 0;
    long value = 0;

    /* Correct the acceleration data. */
    value = bsctyp_encode(DE_VerticalAcceleration, acceleration);
    
    result = (int8_t)value;

    return result;
}
/******************************************************************************
*    函数:    decode_vertical_acceleration
*    功能:    将垂直加速度数据从0.1962m/s^2为单位转换为以19.80665m/s^2为单位
*    参数:    acceleration        -     角度数据
*    返回:    -127 - +127            -    正常数据
*             +127                -    无效数据
*    说明:    指车辆在垂直轴上的加速度值，单位 0.02 G（ G = 9.80665 m/s^2,
            0.02 G = 0.1962 m/s^2）。
            +2.54G表示>=2.54G，-2.52G表示<=2.52G，-2.54G表示无效数据
 ******************************************************************************/
inline float decode_vertical_acceleration(int8_t acceleration)
{
    float result = 0;
    double value = 0;
    
    value = bsctyp_decode(DE_VerticalAcceleration, acceleration);
    
    result = (float)value;

    return result;
}
/******************************************************************************
*    函数:    encode_yawrate
*    功能:    将偏航率度数据从1度为单位转换为以0.01度为单位
*    参数:    yawrate                -     角度数据
*    返回:    -32767 - +32767        -    正常数据
*    说明:    表示车辆的偏航率，是一个以 0.01 度/秒为单位的有符号数值（向右为正），该值可
            以表示车辆在特定时间段内绕垂直轴 Z 的旋转速率
 ******************************************************************************/
inline int16_t encode_yawrate(float yawrate)
{
    int16_t result = 0;
    long value = 0;

    /* Unit 0.01 degree/s. */
    value = bsctyp_encode(DE_YawRate, yawrate);

    result = (int16_t)value;

    return cv_ntohs(result);
}
/******************************************************************************
*    函数:    decode_yawrate
*    功能:    将偏航率度数据从0.01度为单位转换为以1度为单位
*    参数:    yawrate                -     角度数据
*    返回:    -327.67 - +327.67    -    正常数据
*    说明:    表示车辆的偏航率，是一个以 0.01 度/秒为单位的有符号数值（向右为正），该值可
            以表示车辆在特定时间段内绕垂直轴 Z 的旋转速率
 ******************************************************************************/
inline float decode_yawrate(int16_t yawrate)
{
    float result = 0;
    double value = 0;

    /* Unit 0.01 degree/s. */
    value = bsctyp_decode(DE_YawRate, cv_ntohs(yawrate));
    
    result = (float)value;
    
    return result;
}

