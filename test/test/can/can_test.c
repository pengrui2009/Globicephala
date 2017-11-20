/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : can_test.c
 @brief  : This file contain Can_Test module driver fucntions.
 @author : pengrui
 @history:
           2017-9-28    pengrui      Created file
           
           ...
******************************************************************************/
#include "drv_tbox.h"

int main()
{
	int result = 0;

	tbox_shminfo_st tbox_info = {0};

	tbox_shmconfig_st tbox_config = {SHM_DIR_TBOX_TO_V2X, ".", 'B', ".", 'C', 0xFF};
	
	result = tbox_shm_init(&tbox_info, &tbox_config);
	
	return result;
}

