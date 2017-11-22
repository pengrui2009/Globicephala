#include "main.h"
#include "drv_main.h"

#include "cv_msg_wsmp.h"
#include "cv_msg_frame.h"


/* Main driver pointer. */
drv_main_st_ptr DrvMainPtr = NULL;




MSG_MessageFrame_st MsgSt = { 0 };




/*FUNCTION*****************************************************************
* 
* Returned : No return normally.  
* Comments : Wnet receive data main handle thread entry.
*
*END*********************************************************************/
void * wnet_rx_thread_entry(void *param_ptr)
{
    int               result = ERR_OK;
    drv_main_st_ptr  drv_ptr = GET_DRVMAIN_PTR;

    /* Buffer address and message point. */
    uint8_t            * buffer_ptr = NULL;
    MSG_MessageFrame_st_ptr msg_ptr = NULL;
    
    /* Ltev message information and data length. */
    ltev_infor_st ltev_infor;
    uint16_t    ltev_datalen = 0;
    
    /* Wsmp message information and data length. */
    wsmp_infor_st wsmp_infor;
    uint16_t    wsmp_datalen = 0;

    
    /* Allocate message buffer for wnet module. */
    if((buffer_ptr = os_calloc(1, WNET_MSGBUF_SIZE)) == NULL)
    {
        osal_printf("[%s %d]: Allocate message buffer for wnet module error. \n", __FUNCTION__, __LINE__);

        result = -ERR_NOMEM;
        goto ERR_EXIT;
    }

    /* Allocate message for wnet module. */
    if((msg_ptr = os_calloc(1, sizeof(*msg_ptr))) == NULL)
    {
        osal_printf("[%s %d]: Allocate ads message for wnet module error. \n", __FUNCTION__, __LINE__);

        result = -ERR_NOMEM;
        goto ERR_EXIT;
    }
    
RX_LOOP:

    /* Get messgae from ltev device. */
    result = drv_ltev_receive(drv_ptr->ltev_fd, &ltev_infor, buffer_ptr, WNET_MSGBUF_SIZE); 
    if(result < ERR_OK)
    {
        osal_printf("[%s %d]: Receive data from ltev device error. ret = %d. \n", __FUNCTION__, __LINE__, result);
        goto RX_LOOP;
    }
    else
    {
        ltev_datalen = result;
    }

    /* Analyse wsmp header and get data. */
    wsmp_datalen = ltev_datalen;
    if((result = wsmp_analyse_msg(&wsmp_infor, buffer_ptr, &wsmp_datalen)) != ERR_OK)
    {
        osal_printf("[%s %d]: Analyse wsmp message header error. ret = %d. \n", __FUNCTION__, __LINE__, result);
        goto RX_LOOP;
    }

    /* Parse wsmp message based on type. */
    switch(wsmp_infor.dsmp_aid)
    {
        case WAVE_AID_vehicle_safety:
        {
            switch(wsmp_infor.dsmp_element_id)
            {
                case WAVE_ELEMENT_ID_WSM:
                {
                    /* Decode ADS frame into common message structure. */
                    if((result = frame_decode_msg(msg_ptr, buffer_ptr + (ltev_datalen - wsmp_datalen), wsmp_datalen)) != ERR_OK)
                    {
                        osal_printf("[%s %d]: Parse ADS message error. ret = %d. \n", __FUNCTION__, __LINE__, result);
                        goto RX_LOOP;
                    }

                    /* Add message structure into list. 这里暂时先用一个变量代替，后期会换成链表操作。 */
                    memcpy(&MsgSt, msg_ptr, sizeof(MsgSt));
                    break;
                }
                default:
                {
                    osal_printf("[%s %d]: Received not supposed WSMP message. dsmp_element_id = %d. \n", __FUNCTION__, __LINE__, wsmp_infor.dsmp_element_id);
                    break;
                }
            }
            break;
        }
        case WAVE_AID_private:
        {
            osal_printf("[%s %d]: Received private WSMP message. dsmp_aid = %d. \n", __FUNCTION__, __LINE__, wsmp_infor.dsmp_aid);
            break;
        }
        default:
        {
            osal_printf("[%s %d]: Received not supposed WSMP message. dsmp_aid = %d. \n", __FUNCTION__, __LINE__, wsmp_infor.dsmp_aid);
            break;
        }
    }

    goto RX_LOOP;

ERR_EXIT:

    /* Free buffer and message memory. */
    if(buffer_ptr != NULL)
    {
        os_free(buffer_ptr);
    }
    if(msg_ptr != NULL)
    {
        os_free(msg_ptr);
    }

    return (void*)result;
}


/*FUNCTION*****************************************************************
* 
* Returned : No return normally.  
* Comments : Wnet receive data main handle thread entry.
*
*END*********************************************************************/
void * ehm_rx_thread_entry(void *param_ptr)
{
#if 0
    int               result = ERR_OK;
    drv_main_st_ptr  drv_ptr = GET_DRVMAIN_PTR;

    /* Buffer address and message point. */
    uint8_t            * buffer_ptr = NULL;
    MSG_MessageFrame_st_ptr msg_ptr = NULL;
    
    /* Ltev message information and data length. */
    ltev_infor_st ltev_infor = { 0 };
    uint16_t    ltev_datalen = 0;
    
    /* Wsmp message information and data length. */
    wsmp_infor_st wsmp_infor = { 0 };
    uint16_t    wsmp_datalen = 0;

    
    /* Allocate message buffer for wnet module. */
    if((buffer_ptr = os_calloc(1, WNET_MSGBUF_SIZE)) == NULL)
    {
        osal_printf("[%s %d]: Allocate message buffer for wnet module error. \n", __FUNCTION__, __LINE__);

        result = -ERR_NOMEM;
        goto ERR_EXIT;
    }

    /* Allocate message for wnet module. */
    if((msg_ptr = os_calloc(1, sizeof(*MSG_MessageFrame_st_ptr))) == NULL)
    {
        osal_printf("[%s %d]: Allocate ads message for wnet module error. \n", __FUNCTION__, __LINE__);

        result = -ERR_NOMEM;
        goto ERR_EXIT;
    }
    
RX_LOOP:

    /* Get messgae from ltev device. */
    result = drv_ltev_receive(drv_ptr->ltev_fd, &ltev_infor, buffer_ptr, WNET_MSGBUF_SIZE); 
    if(result < ERR_OK)
    {
        osal_printf("[%s %d]: Receive data from ltev device error. ret = %d. \n", __FUNCTION__, __LINE__, result);
        goto RX_LOOP;
    }
    else
    {
        ltev_datalen = result;
    }

    /* Analyse wsmp header and get data. */
    wsmp_datalen = ltev_datalen;
    if((result = wsmp_analyse_msg(&wsmp_infor, buffer_ptr, &wsmp_datalen)) != ERR_OK)
    {
        osal_printf("[%s %d]: Analyse wsmp message header error. ret = %d. \n", __FUNCTION__, __LINE__, result);
        goto RX_LOOP;
    }

    /* Parse wsmp message based on type. */
    switch(wsmp_infor.dsmp_aid)
    {
        case WAVE_AID_vehicle_safety:
        {
            switch(wsmp_infor.dsmp_element_id)
            {
                case WAVE_ELEMENT_ID_WSM:
                {
                    /* Decode ADS frame into common message structure. */
                    if((result = frame_decode_msg(msg_ptr, buffer_ptr + (ltev_datalen - wsmp_datalen), wsmp_datalen)) != ERR_OK)
                    {
                        osal_printf("[%s %d]: Parse ADS message error. ret = %d. \n", __FUNCTION__, __LINE__, result);
                        goto RX_LOOP;
                    }

                    /* Add message structure into list. 这里暂时先用一个变量代替，后期会换成链表操作。 */
                    memcpy(&MsgSt, msg_ptr, sizeof(MsgSt));
                    break;
                }
                default:
                {
                    osal_printf("[%s %d]: Received not supposed WSMP message. dsmp_element_id = %d. \n", __FUNCTION__, __LINE__, wsmp_infor.dsmp_element_id);
                    break;
                }
            }
            break;
        }
        case WAVE_AID_private:
        {
            osal_printf("[%s %d]: Received private WSMP message. dsmp_aid = %d. \n", __FUNCTION__, __LINE__, wsmp_infor.dsmp_aid);
            break;
        }
        default:
        {
            osal_printf("[%s %d]: Received not supposed WSMP message. dsmp_aid = %d. \n", __FUNCTION__, __LINE__, wsmp_infor.dsmp_aid);
            break;
        }
    }

    goto RX_LOOP;

ERR_EXIT:

    /* Free buffer and message memory. */
    if(buffer_ptr != NULL)
    {
        os_free(buffer_ptr);
    }
    if(msg_ptr != NULL)
    {
        os_free(msg_ptr);
    }

    return result;

#endif

return NULL;

}




osal_task_t  WnetTask = 0;




/* Main routine for the project. */
int main(int argc, char *argv[])
{
    int result = ERR_OK;
    


    /* Init the driver groups. */
    if((result = drv_main_init(&DrvMainPtr)) != ERR_OK)
    {
        osal_printf("[%s %d]: Init driver main error. \n", __FUNCTION__, __LINE__);
        goto ERR_EXIT;
    }

    
    /* Create wnet message receive task. */
    result = osal_task_create(&WnetTask, "WnetRx", wnet_rx_thread_entry, NULL, WNET_RX_THREAD_STACK_SIZE, WNET_RX_THREAD_PRIORITY);
    if(result != ERR_OK)
    {
        goto ERR_EXIT;
    }






ERR_EXIT:

    return result;
}
