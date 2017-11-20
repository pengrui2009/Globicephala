#include <stdio.h>
#include "error.h"
#include "MessageFrame.h"
#include "cv_msg_frame.h"
int main()
{
    int ret = 0;
#if 0
    uint8_t data_buf[] = {  0x00, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
                0x00, 0x00, 0x1A, 0xD2, 0x74, 0x80, 0x35, 0xA4, 0xE8, 0xFF, \
                0x80, 0x00, 0x00, 0x00, 0x03, 0xE8, 0x3E, 0x83, 0xFB, 0xFF, \
                0xF8, 0x00, 0x00, 0x00, 0x00, 0x04, 0xB0, 0x20, 0xC0, 0x80};
#endif
    uint8_t data_buf[] = {  0x07, 0xF8, 0x11, 0x22, 0x33, 0x44, 0x00, 0x00, 0x00, 0x00, \
                0x4A, 0xAB, 0xBC, 0xCD, 0xDE, 0xEF, 0xF9, 0x90, 0x01, 0x38, \
                0x87, 0x2A, 0x9F, 0x88, 0x05, 0x07, 0x75, 0xD7, 0xF8, 0x83, \
                0xD4, 0xD7, 0x82, 0x4C, 0x00, 0x04, 0x97, 0xDF, 0xBB, 0x8B, \
                0x54, 0xA7, 0x82, 0xBB, 0xFE, 0xFE, 0x78, 0x9F, 0x43, 0xE8, \
                0xF0, 0x14, 0x96, 0x04, 0x18, 0x10};
    uint16_t data_size= sizeof(data_buf);
    
    static MSG_MessageFrame_st msg_frame = {0};

    ret = frame_decode_msg(&msg_frame, data_buf, data_size);
    if(ret != 0)
    {
        printf("decode msg error\n");
        return 0;
    }

    
    printf("msg_frame->messageid:%d\n", msg_frame.messageid);    
    printf("msg_frame->msg.msg_bsm.msgCnt:%d\n", msg_frame.msg.msg_bsm.msgCnt);    
    printf("msg_frame->id:%02x %02x %02x %02x %02x %02x %02x %02x\n", \
        msg_frame.msg.msg_bsm.id[0], msg_frame.msg.msg_bsm.id[1], msg_frame.msg.msg_bsm.id[2], msg_frame.msg.msg_bsm.id[3], \
        msg_frame.msg.msg_bsm.id[4], msg_frame.msg.msg_bsm.id[5], msg_frame.msg.msg_bsm.id[6], msg_frame.msg.msg_bsm.id[7] );

    if(msg_frame.msg.msg_bsm.opt.msg_BasicSafetyMessage_opt_plateno == MSG_OPTIONAL_YES)
    {
        printf("msg_frame->plateNo->bufsize:%d\n", msg_frame.msg.msg_bsm.plateno.bufsize);
        printf("plateNo:%02X %02X %02X %02X\n", msg_frame.msg.msg_bsm.plateno.buf[0], msg_frame.msg.msg_bsm.plateno.buf[1], msg_frame.msg.msg_bsm.plateno.buf[2], msg_frame.msg.msg_bsm.plateno.buf[3]);    
    }else{
        printf("msg_frame->plateNo is NULL\n");    
    }

    printf("secMark:%d\n", msg_frame.msg.msg_bsm.secMark);    
    

    printf("latitude:%lf\n", msg_frame.msg.msg_bsm.pos.latitude);    
    printf("longitude:%lf\n", msg_frame.msg.msg_bsm.pos.longitude);    
    if(msg_frame.msg.msg_bsm.pos.opt.Position3D_opt_elev == MSG_OPTIONAL_YES)
    {
        printf("pos elevation:%lf\n", msg_frame.msg.msg_bsm.pos.elevation);
    }else{
        printf("pos elevation is NULL\n");
    }

    printf("accuracy pos:%d\n", msg_frame.msg.msg_bsm.accuracy.pos);    
    if(msg_frame.msg.msg_bsm.accuracy.opt.PositionConfidenceSet_opt_confidence_elev == MSG_OPTIONAL_YES)
    {
        printf("accuracy elevation:%d\n", msg_frame.msg.msg_bsm.accuracy.elevation);
    }else{
        printf("accuracy elevation is NULL\n");
    }
    
    printf("trans:%d\n", msg_frame.msg.msg_bsm.trans);    
    printf("speed:%f\n", msg_frame.msg.msg_bsm.speed);
    printf("heading:%f\n", msg_frame.msg.msg_bsm.heading);
        
    if(msg_frame.msg.msg_bsm.opt.msg_BasicSafetyMessage_opt_angle == MSG_OPTIONAL_YES)
    {
        printf("msg_frame->angle:%f\n", msg_frame.msg.msg_bsm.angle);    
    }else{
        printf("msg_frame->angle is NULL\n");    
    }
        
    if(msg_frame.msg.msg_bsm.opt.msg_BasicSafetyMessage_opt_motioncfd == MSG_OPTIONAL_YES)
    {
        if(msg_frame.msg.msg_bsm.motioncfd.opt.MotionConfidenceSet_opt_speedcfd == MSG_OPTIONAL_YES)
        {
            printf("speedcfd:%d\n", msg_frame.msg.msg_bsm.motioncfd.speedcfd);
        }else{
            printf("speedcfd is NULL\n");
        }

        if(msg_frame.msg.msg_bsm.motioncfd.opt.MotionConfidenceSet_opt_headingcfd == MSG_OPTIONAL_YES)
        {
            printf("headingcfd:%d\n", msg_frame.msg.msg_bsm.motioncfd.headingcfd);
        }else{
            printf("headingcfd is NULL\n");
        }

        if(msg_frame.msg.msg_bsm.motioncfd.opt.MotionConfidenceSet_opt_steercfd == MSG_OPTIONAL_YES)
        {
            printf("steercfd:%d\n", msg_frame.msg.msg_bsm.motioncfd.steercfd);
        }else{
            printf("steercfd is NULL\n");
        }
        
    }else{
        printf("msg_frame->motionCfd is NULL\n");    
    }

    printf("msg_frame->acceSet.longitudinal:%f\n", msg_frame.msg.msg_bsm.acceSet.longitudinal);    
    printf("msg_frame->acceSet.lateral:%f\n", msg_frame.msg.msg_bsm.acceSet.lateral);    
    printf("msg_frame->acceSet.vertical:%f\n", msg_frame.msg.msg_bsm.acceSet.vertical);    
    printf("msg_frame->acceSet.yaw_rate:%f\n", msg_frame.msg.msg_bsm.acceSet.yaw_rate);    

    if(msg_frame.msg.msg_bsm.brakes.opt.BrakeSystemStatus_opt_brakepadel == MSG_OPTIONAL_YES)
    {
        printf("brakepedal:%d\n", msg_frame.msg.msg_bsm.brakes.brakepedal);
    }else{
        printf("brakepedal is NULL\n");
    }

    if(msg_frame.msg.msg_bsm.brakes.opt.BrakeSystemStatus_opt_wheelBrakes == MSG_OPTIONAL_YES)
    {
        printf("wheelBrakes.brake_status_unavailable:%d\n", msg_frame.msg.msg_bsm.brakes.wheelBrakes.brake_status_unavailable);
        printf("wheelBrakes.brake_status_leftFront:%d\n", msg_frame.msg.msg_bsm.brakes.wheelBrakes.brake_status_leftFront);
        printf("wheelBrakes.brake_status_leftRear:%d\n", msg_frame.msg.msg_bsm.brakes.wheelBrakes.brake_status_leftRear);
        printf("wheelBrakes.brake_status_rightFront:%d\n", msg_frame.msg.msg_bsm.brakes.wheelBrakes.brake_status_rightFront);
        printf("wheelBrakes.brake_status_rightRear:%d\n", msg_frame.msg.msg_bsm.brakes.wheelBrakes.brake_status_rightRear);
    }else{
        printf("wheelBrakes is NULL\n");
    }

    if(msg_frame.msg.msg_bsm.brakes.opt.BrakeSystemStatus_opt_traction == MSG_OPTIONAL_YES)
    {
        printf("traction:%d\n", msg_frame.msg.msg_bsm.brakes.traction);
    }else{
        printf("traction is NULL\n");
    }

    if(msg_frame.msg.msg_bsm.brakes.opt.BrakeSystemStatus_opt_abs == MSG_OPTIONAL_YES)
    {
        printf("abs:%d\n", msg_frame.msg.msg_bsm.brakes.abs);
    }else{
        printf("abs is NULL\n");
    }

    if(msg_frame.msg.msg_bsm.brakes.opt.BrakeSystemStatus_opt_scs == MSG_OPTIONAL_YES)
    {
        printf("scs:%d\n", msg_frame.msg.msg_bsm.brakes.scs);
    }else{
        printf("scs is NULL\n");
    }

    if(msg_frame.msg.msg_bsm.brakes.opt.BrakeSystemStatus_opt_brakeBoost == MSG_OPTIONAL_YES)
    {
        printf("brakeBoost:%d\n", msg_frame.msg.msg_bsm.brakes.brakeBoost);
    }else{
        printf("brakeBoost is NULL\n");
    }

    if(msg_frame.msg.msg_bsm.brakes.opt.BrakeSystemStatus_opt_auxBrakes == MSG_OPTIONAL_YES)
    {
        printf("auxBrakes:%d\n", msg_frame.msg.msg_bsm.brakes.auxBrakes);
    }else{
        printf("auxBrakes is NULL\n");
    }

    printf("size.width:%f\n", msg_frame.msg.msg_bsm.size.width);
    printf("size.length:%f\n", msg_frame.msg.msg_bsm.size.length);
    if(msg_frame.msg.msg_bsm.size.opt.vehiclesize_opt_height == MSG_OPTIONAL_YES)
    {
        printf("size.height:%f\n", msg_frame.msg.msg_bsm.size.height);
    }else{
        printf("size.height is NULL\n");
    }

    printf("vehicleclass:%d\n", msg_frame.msg.msg_bsm.vehicleclass.classification);
    
        
    if(msg_frame.msg.msg_bsm.opt.msg_BasicSafetyMessage_opt_safetyExt == MSG_OPTIONAL_YES)
    {
        if(msg_frame.msg.msg_bsm.safetyExt.opt.VehicleSafetyExternsions_opt_events == MSG_OPTIONAL_YES)
        {
            printf("VehicleEventFlags_eventHazardLights:%d\n" , msg_frame.msg.msg_bsm.safetyExt.events.bit.VehicleEventFlags_eventHazardLights);
            printf("VehicleEventFlags_eventStopLineViolation:%d\n" , msg_frame.msg.msg_bsm.safetyExt.events.bit.VehicleEventFlags_eventStopLineViolation);
            printf("VehicleEventFlags_eventABSactivated:%d\n" , msg_frame.msg.msg_bsm.safetyExt.events.bit.VehicleEventFlags_eventABSactivated);
            printf("VehicleEventFlags_eventTractionControlLoss:%d\n" , msg_frame.msg.msg_bsm.safetyExt.events.bit.VehicleEventFlags_eventTractionControlLoss);
            printf("VehicleEventFlags_eventStabilityControlactivated:%d\n" , msg_frame.msg.msg_bsm.safetyExt.events.bit.VehicleEventFlags_eventStabilityControlactivated);
            printf("VehicleEventFlags_eventHazardousMaterials:%d\n" , msg_frame.msg.msg_bsm.safetyExt.events.bit.VehicleEventFlags_eventHazardousMaterials);
            printf("VehicleEventFlags_eventReserved1:%d\n" , msg_frame.msg.msg_bsm.safetyExt.events.bit.VehicleEventFlags_eventReserved1);
            printf("VehicleEventFlags_eventHardBraking:%d\n" , msg_frame.msg.msg_bsm.safetyExt.events.bit.VehicleEventFlags_eventHardBraking);
            printf("VehicleEventFlags_eventLightsChanged:%d\n" , msg_frame.msg.msg_bsm.safetyExt.events.bit.VehicleEventFlags_eventLightsChanged);
            printf("VehicleEventFlags_eventWipersChanged:%d\n" , msg_frame.msg.msg_bsm.safetyExt.events.bit.VehicleEventFlags_eventWipersChanged);
            printf("VehicleEventFlags_eventFlatTire:%d\n" , msg_frame.msg.msg_bsm.safetyExt.events.bit.VehicleEventFlags_eventFlatTire);
            printf("VehicleEventFlags_eventDisabledVehicle:%d\n" , msg_frame.msg.msg_bsm.safetyExt.events.bit.VehicleEventFlags_eventDisabledVehicle);
            printf("VehicleEventFlags_eventAirBagDeployment:%d\n" , msg_frame.msg.msg_bsm.safetyExt.events.bit.VehicleEventFlags_eventAirBagDeployment);
            
            
        }else{
            printf("events is NULL\n");
        }

        if(msg_frame.msg.msg_bsm.safetyExt.opt.VehicleSafetyExternsions_opt_pathHistory == MSG_OPTIONAL_YES)
        {
            printf("pathHistory is not NULL\n");
        }else{
            printf("pathHistory is NULL\n");
        }

        if(msg_frame.msg.msg_bsm.safetyExt.opt.VehicleSafetyExternsions_opt_pathPrediction == MSG_OPTIONAL_YES)
        {
            printf("pathPrediction is not NULL\n");
        }else{
            printf("pathPrediction is NULL\n");
        }

        if(msg_frame.msg.msg_bsm.safetyExt.opt.VehicleSafetyExternsions_opt_lights == MSG_OPTIONAL_YES)
        {
            printf("ExteriorLights_lowBeamHeadlightsOn:%d\n", msg_frame.msg.msg_bsm.safetyExt.lights.bit.ExteriorLights_lowBeamHeadlightsOn);
            printf("ExteriorLights_highBeamHeadlightsOn:%d\n", msg_frame.msg.msg_bsm.safetyExt.lights.bit.ExteriorLights_highBeamHeadlightsOn);
            printf("ExteriorLights_leftTurnSignalOn:%d\n", msg_frame.msg.msg_bsm.safetyExt.lights.bit.ExteriorLights_leftTurnSignalOn);
            printf("ExteriorLights_rightTurnSignalOn:%d\n", msg_frame.msg.msg_bsm.safetyExt.lights.bit.ExteriorLights_rightTurnSignalOn);
            printf("ExteriorLights_hazardSignalOn:%d\n", msg_frame.msg.msg_bsm.safetyExt.lights.bit.ExteriorLights_hazardSignalOn);
            printf("ExteriorLights_automaticLightControlOn:%d\n", msg_frame.msg.msg_bsm.safetyExt.lights.bit.ExteriorLights_automaticLightControlOn);
            printf("ExteriorLights_daytimeRunningLightsOn:%d\n", msg_frame.msg.msg_bsm.safetyExt.lights.bit.ExteriorLights_daytimeRunningLightsOn);
            printf("ExteriorLights_fogLightOn:%d\n", msg_frame.msg.msg_bsm.safetyExt.lights.bit.ExteriorLights_fogLightOn);
            printf("ExteriorLights_parkingLightsOn:%d\n", msg_frame.msg.msg_bsm.safetyExt.lights.bit.ExteriorLights_parkingLightsOn);
            
        }else{
            printf("lights is NULL\n");
        }
        
    }else{
        printf("msg_frame->plateNo is NULL\n");    
    }

    return 0;
#if 0
        MessageFrame_t          *frame_ptr = NULL;

        asn_dec_rval_t        decode_rval = { 0 };
        asn_codec_ctx_t        decode_ctx = { 0 };
    
    uint8_t data_buf[] = {  0x00, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
                0x00, 0x00, 0x1A, 0xD2, 0x74, 0x80, 0x35, 0xA4, 0xE8, 0xFF, \
                0x80, 0x00, 0x00, 0x00, 0x03, 0xE8, 0x3E, 0x83, 0xFB, 0xFF, \
                0xF8, 0x00, 0x00, 0x00, 0x00, 0x04, 0xB0, 0x20, 0xC0, 0x80};
    uint16_t data_size = sizeof(data_buf);
    /* Dncode message from UPER format. */
        decode_rval = uper_decode(&decode_ctx, &asn_DEF_MessageFrame, (void **)&frame_ptr, data_buf, data_size, 0, 0);
        if(decode_rval.code != 0)
        {
            ret = -ERR_SYS;
    }else{
        printf("msgCnt:%d\n", (uint8_t)frame_ptr->choice.bsmFrame.msgCnt);
        
        printf("id.size=:%d\n", (int)frame_ptr->choice.bsmFrame.id.size);
        
        if(frame_ptr->choice.bsmFrame.plateNo != NULL)
            printf("plateNo.size:%d\n", (int)frame_ptr->choice.bsmFrame.plateNo->size);
        else
            printf("plateNo NULL\n");

        printf("secMark:%d\n", (int)frame_ptr->choice.bsmFrame.secMark);
        
        printf("pos:%d %d\n", (int)frame_ptr->choice.bsmFrame.pos.lat, (int)frame_ptr->choice.bsmFrame.pos.Long);

        if(frame_ptr->choice.bsmFrame.pos.elevation != NULL)
            printf("elevation:%d\n", (int)*frame_ptr->choice.bsmFrame.pos.elevation);
        else
            printf("msg_frame.choice.bsmFrame.pos.elevation NULL\n");
        
        printf("accuracy:%d\n", (int)frame_ptr->choice.bsmFrame.accuracy.pos);
        
        if(frame_ptr->choice.bsmFrame.accuracy.elevation != NULL)
            printf("accuracy:%d\n", (int)*frame_ptr->choice.bsmFrame.accuracy.elevation);
        else
            printf("msg_frame.choice.bsmFrame.accuracy.elevation  NULL\n");
        
        printf("transmission:%d\n", (int)frame_ptr->choice.bsmFrame.transmission);
        printf("speed:%d\n", (int)frame_ptr->choice.bsmFrame.speed);
        printf("heading:%d\n", (int)frame_ptr->choice.bsmFrame.heading);
        
        if(frame_ptr->choice.bsmFrame.angle != NULL)
            printf("angle:%d\n", (int)*frame_ptr->choice.bsmFrame.angle);
        else
            printf("msg_frame.choice.bsmFrame.angle NULL\n");
        
        if(frame_ptr->choice.bsmFrame.motionCfd != NULL)
        {
            if(frame_ptr->choice.bsmFrame.motionCfd->speedCfd != NULL)
                printf("motionCfd.speedCfd:%d\n", (int)*frame_ptr->choice.bsmFrame.motionCfd->speedCfd);
            else
                printf("msg_frame.choice.bsmFrame.motionCfd->speedCfd NULL\n");
            
            if(frame_ptr->choice.bsmFrame.motionCfd->headingCfd != NULL)
                printf("motionCfd.headingCfd:%d\n", (int)*frame_ptr->choice.bsmFrame.motionCfd->headingCfd);
            else
                printf("msg_frame.choice.bsmFrame.motionCfd->headingCfd NULL\n");
            
            if(frame_ptr->choice.bsmFrame.motionCfd->steerCfd != NULL)
                printf("motionCfd.steerCfd:%d\n", (int)*frame_ptr->choice.bsmFrame.motionCfd->steerCfd);
            else
                printf("msg_frame.choice.bsmFrame.motionCfd->steerCfd NULL\n");
        }else{
            printf("msg_frame.choice.bsmFrame.motionCfd NULL\n");
        }
        
        printf("Long:%d\n", (int)frame_ptr->choice.bsmFrame.accelSet.Long);
        printf("lat:%d\n", (int)frame_ptr->choice.bsmFrame.accelSet.lat);
        printf("vert:%d\n", (int)frame_ptr->choice.bsmFrame.accelSet.vert);
        printf("yaw:%d\n", (int)frame_ptr->choice.bsmFrame.accelSet.yaw);

        if(frame_ptr->choice.bsmFrame.brakes.brakePadel != NULL)
        {
            printf("brakePadel:%d\n", (int)*frame_ptr->choice.bsmFrame.brakes.brakePadel);
        }else{
            printf("msg_frame.choice.bsmFrame.brakes.brakePadel NULL\n");
        }
        
        if(frame_ptr->choice.bsmFrame.brakes.wheelBrakes != NULL)
        {
            printf("wheelBrakes.size:%d\n", (int)frame_ptr->choice.bsmFrame.brakes.wheelBrakes->size);
        }else{
            printf("msg_frame.choice.bsmFrame.brakes.brakePadel NULL\n");
        }

        if(frame_ptr->choice.bsmFrame.brakes.traction != NULL)
        {
            printf("traction:%d\n", (int)*frame_ptr->choice.bsmFrame.brakes.traction);
        }else{
            printf("msg_frame.choice.bsmFrame.brakes.traction NULL\n");
        }

        
        if(frame_ptr->choice.bsmFrame.brakes.abs != NULL)
        {
            printf("abs:%d\n", (int)*frame_ptr->choice.bsmFrame.brakes.abs);
        }else{
            printf("msg_frame.choice.bsmFrame.brakes.abs NULL\n");
        }

        if(frame_ptr->choice.bsmFrame.brakes.scs != NULL)
        {
            printf("scs:%d\n", (int)*frame_ptr->choice.bsmFrame.brakes.scs);
        }else{
            printf("msg_frame.choice.bsmFrame.brakes.scs NULL\n");
        }

        
        if(frame_ptr->choice.bsmFrame.brakes.brakeBoost != NULL)
        {
            printf("brakeBoost:%d\n", (int)*frame_ptr->choice.bsmFrame.brakes.brakeBoost);
        }else{
            printf("msg_frame.choice.bsmFrame.brakes.brakeBoost NULL\n");
        }
        
        if(frame_ptr->choice.bsmFrame.brakes.auxBrakes != NULL)
        {
            printf("auxBrakes:%d\n", (int)*frame_ptr->choice.bsmFrame.brakes.auxBrakes);
        }else{
            printf("msg_frame.choice.bsmFrame.brakes.auxBrakes NULL\n");
        }

        printf("width:%d\n", (int)frame_ptr->choice.bsmFrame.size.width);
        
        printf("length:%d\n", (int)frame_ptr->choice.bsmFrame.size.length);
        
        if(frame_ptr->choice.bsmFrame.size.height != NULL)
        {
            printf("height:%d\n", (int)*frame_ptr->choice.bsmFrame.size.height);
        }else{
            printf("msg_frame.choice.bsmFrame.brakes.size.height NULL\n");
        }
        
        printf("classification:%d\n", (int)frame_ptr->choice.bsmFrame.vehicleClass.classification);

        if(frame_ptr->choice.bsmFrame.safetyExt != NULL)
        {
            if(frame_ptr->choice.bsmFrame.safetyExt->event != NULL)
            {
                printf("event[0]:%02x\n", frame_ptr->choice.bsmFrame.safetyExt->event->buf[0]);
                printf("event[1]:%02x\n", frame_ptr->choice.bsmFrame.safetyExt->event->buf[1]);
            }else{
                printf("event is null\n");
            }

            if(frame_ptr->choice.bsmFrame.safetyExt->pathHistory != NULL)
            {
                printf("pathHistory not null\n");
            }else{
                printf("event is null\n");
            }

            if(frame_ptr->choice.bsmFrame.safetyExt->pathPrediction != NULL)
            {
                printf("pathPrediction not null\n");
            }else{
                printf("event is null\n");
            }

            if(frame_ptr->choice.bsmFrame.safetyExt->lights != NULL)
            {
                printf("lights[0]:%02x\n", frame_ptr->choice.bsmFrame.safetyExt->lights->buf[0]);
                printf("lights[1]:%02x\n", frame_ptr->choice.bsmFrame.safetyExt->lights->buf[1]);
            }else{
                printf("event is null\n");
            }
        }else{
            printf("safetyExt: NULL\n");
        }
    }
    return ret;
    #endif
}
