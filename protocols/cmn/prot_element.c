/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : prot_dataelem.c
 @brief  : Data Element routine set for J2735/ehmp protocols.
 @author : wangxianwen
 @history:
           2016-09-20    wangxianwen    Created file
           2017-10-12    pengrui        Modify for CSAE
           ...
******************************************************************************/

#include "prot_element.h"


/* Basic type transform data group. */
const static bsctyp_transf_st BasicTypeGroup[100] = 
{
    /* value_max,   value_min,   value_err,  encode_factor, decode_factor. */

    {       20.0,       -20.0,       20.01,        100.0,         0.01  }, /* DE_Acceleration. Unit: 1 m/s2 & 0.01 m/s2. */

    {     358.50,           0,       360.0, 0.6666666667,          1.5  }, /* DE_CoarseHeading. Unit: degree & 1.5 degree. */
    {        100,           0,         101,            2,          0.5  }, /* DE_Confidence. Unit: percent & 0.5 percent. */

    {         31,           1,           0,          1.0,          1.0  }, /* DE_DDay. Unit: 1 day & 1 day. */
    {         30,           0,          31,          1.0,          1.0  }, /* DE_DHour. Unit: 1 hour & 1 hour. */
    {         59,           0,          60,          1.0,          1.0  }, /* DE_DMinute. Unit: 1 minute & 1 minute. */
    {         12,           1,           0,          1.0,          1.0  }, /* DE_DMonth. Unit: 1 month & 1 month. */
    {        840,        -840,           0,          1.0,          1.0  }, /* DE_DOffset. Unit: 1 offset & 1 offset. */
    {      60999,           0,       65535,          1.0,          1.0  }, /* DE_DSecond. Unit: 1 millisecond & 1 millicecond. */
    {       4095,           1,           0,          1.0,          1.0  }, /* DE_DYear. Unit: 1 year & 1 year. */
    
    {     6143.9,      -409.5,      -409.6,         10.0,          0.1  }, /* DE_Elevation. Unit: meter & 10 cm. */ 

    {        255,           1,           0,          1.0,          1.0  }, /* DE_LaneID. Unit: id & id. */
    {     327.67,           0,      327.68,        100.0,         0.01  }, /* DE_LaneWidth. Unit: meter & 0.01 meter. */
    {       90.0,       -90.0,  90.0000001,   10000000.0,    0.0000001  }, /* DE_Latitude. Unit: degree & 0.1 micro degree. */
    {     327.67,     -327.67,     -327.68,        100.0,         0.01  }, /* DE_LatitudinalDis. Unit: meter & 1 cm. */
    {      180.0,-179.9999999, 180.0000001,   10000000.0,    0.0000001  }, /* DE_Longitude. Unit: degree & 0.1 micro degree. */
    {     327.67,     -327.67,     -327.68,        100.0,         0.01  }, /* DE_LongitudinalDis. Unit: meter & 1cm. */
    
    {   359.9875,           0,       360.0,         80.0,       0.0125  }, /* DE_Heading. Unit: degree & 0.0125 degree. */
   
    {     527040,           0,      527040,          1.0,          1.0  }, /* DE_MinuteOfTheYear. Uint: 1 minute & 1 minute */
    {        127,           0,         255,          1.0,          1.0  }, /* DE_MsgCount. Unit: 1 count & 1 count. */

    {       2.55,       -2.55,       -2.56,        100.0,         0.01  }, /* DE_Offset_B09. Unit: meter & 0.01 meter. */
    {       5.11,       -5.11,       -5.12,        100.0,         0.01  }, /* DE_Offset_B10. Unit: meter & 0.01 meter. */
    {      20.47,      -20.47,      -20.48,        100.0,         0.01  }, /* DE_Offset_B12. Unit: meter & 0.01 meter. */
    {  0.0002047,  -0.0002048,  -0.0002048,   10000000.0,    0.0000001  }, /* DE_OffsetLL_B12. Unit: degree & 0.01 microdegrees. */
    {  0.0008191,  -0.0008192,  -0.0008192,   10000000.0,    0.0000001  }, /* DE_OffsetLL_B14. Unit: degree & 0.01 microdegrees. */
    {  0.0032767,  -0.0032768,  -0.0032768,   10000000.0,    0.0000001  }, /* DE_OffsetLL_B16. Unit: degree & 0.01 microdegrees. */
    {  0.0131071,  -0.0131071,  -0.0131072,   10000000.0,    0.0000001  }, /* DE_OffsetLL_B18. Unit: degree & 0.1 microdegrees. */
    {  0.2097151,  -0.2097152,  -0.2097152,   10000000.0,    0.0000001  }, /* DE_OffsetLL_B22. Unit: degree & 0.1 microdegrees. */
    {  0.8388607,  -0.8388608,  -0.8388608,   10000000.0,    0.0000001  }, /* DE_OffsetLL_B24. Unit: degree & 0.1 microdegrees. */
  
    {        255,           0,           0,          1.0,          1.0  }, /* DE_PhaseID. Uint: */
  
    {     3276.7,     -3276.7,     -3276.8,         10.0,          0.1  }, /* DE_RadiusOfCurvature. Unit: meter & 10 cm. */
    {      102.4,           0,           0,         10.0,          0.1  }, /* DE_Radius. Unit: meter & 10 cm. */
    {      65535,           0,           0,          1.0,          1.0  }, /* DE_RoadRegulatorID. Uint: */
  
    {      12.70,           0,       12.75,         20.0,         0.05  }, /* DE_SemiMajorAxisAccuracy. Unit: meter & 0.05m. */
    {359.9945078786,        0,360.0000011265,182.0416660971,0.0054932479}, /* DE_SemiMajorAxisOrientation. Unit: degree & 0.0054932479 degree. */
    {      12.70,           0,       12.75,         20.0,         0.05  }, /* DE_SemiMinorAxisAccuracy. Unit: meter & 0.05m. */
    {    589.680,           0,     589.752,13.8888888889,        0.072  }, /* DE_Speed. Unit: km/h & 0.02m/s. */
    {        189,        -189,       190.5, 0.6666666666666667,    1.5  }, /* DE_SteeringWheelAngle. Unit: degree & 1.5degree. */

    {     3600.0,           0,      3600.1,         10.0,          0.1  }, /* DE_TimeMark. Unit:s & 0.1s  */
    {     655.34,        0.01,      655.35,        100.0,         0.01  }, /* DE_TimeOffset. Unit: second & 10 mSec. */

    {       6.35,        0.05,           0,         20.0,         0.05  }, /* DE_VehicleHeight. Unit: m & 0.05m. */
    {      40.95,        0.01,           0,        100.0,         0.01  }, /* DE_VehicleLength. Unit: m & 0.01m. */
    {      10.23,        0.01,           0,        100.0,         0.01  }, /* DE_VehicleWidth. Unit: m & 0.01m. */
    {    589.680,           0,     589.752, 13.8888888889,       0.072  }, /* DE_Velocity. Unit: km/h & 0.02m/s. */
    {  24.908891,  -24.712758,  -24.908891,5.098581064889641, 0.196133  }, /* DE_VerticalAcceleration. Unit: 1 m/s2 & (0.02G)0.196133 m/s2. */
    {        6.3,        -6.4,        -6.4,         10.0,          0.1  }, /* DE_VertOffset_B07. Unit: meter & 10 cm. */
    {       12.7,       -12.8,       -12.8,         10.0,          0.1  }, /* DE_VertOffset_B08. Unit: meter & 10 cm. */
    {       25.5,       -25.6,       -25.6,         10.0,          0.1  }, /* DE_VertOffset_B09. Unit: meter & 10 cm. */
    {       51.1,       -51.2,       -51.2,         10.0,          0.1  }, /* DE_VertOffset_B10. Unit: meter & 10 cm. */
    {      102.3,      -102.4,      -102.4,         10.0,          0.1  }, /* DE_VertOffset_B11. Unit: meter & 10 cm. */
    {      204.7,      -204.7,      -204.8,         10.0,          0.1  }, /* DE_VertOffset_B12. Unit: meter & 10 cm. */ 

    {     327.67,     -327.67,      327.68,        100.0,         0.01  }, /* DE_YawRate. Unit: degree & 0.01 degree. */ 
    
};


/* Encode basic type data and return the outer type data. */
outertyp_t bsctyp_encode(bsctyp_def_en type, innertyp_t inner_data)
{
    outertyp_t result = 0;

    
    if( ((BasicTypeGroup[type].value_err - 0.00000005) < inner_data) \
     && (inner_data < (BasicTypeGroup[type].value_err + 0.00000005)) )
    {
       inner_data = BasicTypeGroup[type].value_err; 
    }
    else
    {
        inner_data = (BasicTypeGroup[type].value_max <= inner_data)? BasicTypeGroup[type].value_max : inner_data;
        inner_data = (inner_data <= BasicTypeGroup[type].value_min)? BasicTypeGroup[type].value_min : inner_data; 
    }

    result = (outertyp_t)(inner_data * BasicTypeGroup[type].encode_factor);

    return result;
}


/* Decode basic type data and return the inner type data. */
innertyp_t bsctyp_decode(bsctyp_def_en type, outertyp_t outer_data)
{
    innertyp_t result = (innertyp_t)outer_data;
    

    result = result * BasicTypeGroup[type].decode_factor;
    return result; 
}


