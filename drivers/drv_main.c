#include "drv_main.h"


/******************************************************************************
*    Function: drv_main_init
*    Descriptions: Main routine that open the device group.
*    Paramters:
            drv_ptr_ptr        -    the pointer that hold driver point descriptor.
*    Return:
            = 0                -    success
            < 0                -    failed
*    Comments: 
******************************************************************************/
int drv_main_init(drv_main_st_ptr *drv_ptr_ptr)
{
    int              result = ERR_OK;
    drv_main_st_ptr drv_ptr = NULL;

    /* Audio configuration. */
    #if(DRV_ENABLE_AUDIO != 0)
    audio_config_st audio_cfg = DRV_AUDIO_CFG;
    #endif
    
    /* Barometer configuration. */
    #if(DRV_ENABLE_BAROMETER != 0)
    barometer_config_st barometer_cfg = DRV_BAROMETER_CFG;
    #endif

    /* Ehost configuration. */
    #if(DRV_ENABLE_EHOST != 0)
    net_config_st ehost_cfg = DRV_EHOST_NETCFG;
    #endif

    /* Ltev configuration. */
    #if(DRV_ENABLE_LTEV != 0)
    ltev_config_st ltev_cfg = DRV_LTEV_CFG;
    #endif


    /* Error detection. */
    if(drv_ptr_ptr == NULL)
    {
        osal_printf("[%s %d]: Invalid parameter. \n", __FUNCTION__, __LINE__);

        result = ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Allocate memory for main driver. */
    if((drv_ptr = os_calloc(1, sizeof(*drv_ptr))) == NULL)
    {
        osal_printf("[%s %d]: Allocate memory for main driver error. \n", __FUNCTION__, __LINE__);

        result = -ERR_NOMEM;
        goto ERR_EXIT;
    }

    /* Init audio driver. */
    #if(DRV_ENABLE_AUDIO != 0)
    if((result = drv_audio_init(&(drv_ptr->audio_fd), &audio_cfg)) != ERR_OK)
    {
        osal_printf("[%s %d]: Init audio driver error. ret = %d. \n", __FUNCTION__, __LINE__, result);
        goto ERR_EXIT;
    }
    #endif

    /* Init barometer driver. */
    #if(DRV_ENABLE_BAROMETER != 0)
    if((result = drv_barometer_init(&(drv_ptr->barometer_fd), &barometer_cfg)) != ERR_OK)
    {
        osal_printf("[%s %d]: Init barometer driver error. ret = %d. \n", __FUNCTION__, __LINE__, result);
        goto ERR_EXIT;
    }
    #endif

    /* Init vhost net driver. */
    #if(DRV_ENABLE_EHOST != 0)
    if((result = net_init(&(drv_ptr->ehost_fd), DRV_EHOST_NETIF, &ehost_cfg)) != ERR_OK)
    {
        osal_printf("[%s %d]: Init ehost driver error. ret = %d. \n", __FUNCTION__, __LINE__, result);
        goto ERR_EXIT;
    }
    #endif

    /* Init ltev driver. */
    #if(DRV_ENABLE_LTEV != 0)
    if((result = drv_ltev_init(&(drv_ptr->ltev_fd), &ltev_cfg)) != ERR_OK)
    {
        osal_printf("[%s %d]: Init ltev driver error. ret = %d. \n", __FUNCTION__, __LINE__, result);
        goto ERR_EXIT;
    }
    #endif

    /* Return driver pointer. */
    *drv_ptr_ptr = drv_ptr;
    result = ERR_OK;

    return result;
    
ERR_EXIT:

    if(drv_ptr != NULL)
    {
        drv_main_deinit(drv_ptr);
        os_free(drv_ptr);
    }

    return result;
}


/******************************************************************************
*    Function: drv_main_deinit
*    Descriptions: Main routine that close the device group.
*    Paramters:
            drv_ptr            -    the pointer that hold driver descriptor.
*    Return:
            = 0                -    success
            < 0                -    failed
*    Comments: 
******************************************************************************/
int drv_main_deinit(drv_main_st_ptr drv_ptr)
{
    int    result = ERR_OK;


    /* Error detection. */
    if(drv_ptr == NULL)
    {
        osal_printf("[%s %d]: Invalid parameter. \n", __FUNCTION__, __LINE__);

        result = ERR_INVAL;
        goto ERR_EXIT;
    }

    /* Deinit audio driver. */
    #if(DRV_ENABLE_AUDIO != 0)
    if((result = drv_audio_deinit(drv_ptr->audio_fd)) != ERR_OK)
    {
        osal_printf("[%s %d]: Deinit audio driver error. ret = %d. \n", __FUNCTION__, __LINE__, result);
        goto ERR_EXIT;
    }
    drv_ptr->audio_fd = 0;
    #endif

    /* Deinit barometer driver. */
    #if(DRV_ENABLE_BAROMETER != 0)
    if((result = drv_barometer_deinit(drv_ptr->barometer_fd)) != ERR_OK)
    {
        osal_printf("[%s %d]: Deinit barometer driver error. ret = %d. \n", __FUNCTION__, __LINE__, result);
        goto ERR_EXIT;
    }
    drv_ptr->barometer_fd = 0;
    #endif

    /* Deinit ltev driver. */
    #if(DRV_ENABLE_LTEV != 0)
    if((result = drv_ltev_deinit(drv_ptr->ltev_fd)) != ERR_OK)
    {
        osal_printf("[%s %d]: Deinit ltev driver error. ret = %d. \n", __FUNCTION__, __LINE__, result);
        goto ERR_EXIT;
    }
    drv_ptr->ltev_fd = 0;
    #endif

    /* Free the structure. */
    os_free(drv_ptr);
    result = ERR_OK;
        
ERR_EXIT:

    return result;
}


