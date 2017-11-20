
#include "drv_audio.h"
#include <stdio.h>
#include <pthread.h>

#include "pcm_resource.h"


int main(int argc, char *argv[])
{
	//int offset = 0;
	int result = 0;
	
	drv_sound_config_t config = {{SOUND_PCM_FORMAT_S16_LE, SOUND_PCM_CHANNEL_MONO, SOUND_PCM_RATE_16000}, {100}};
	result = drv_audio_open();
	if(result < 0)
	{
		printf("drv_audio_open error\n");
		goto ERR_EXIT;
	}

	result = drv_audio_config(&config);
	if(result < 0)
	{
		printf("drv_audio_config error\n");
		goto ERR_EXIT;
	}
	
	while(1)
	{
		//result = drv_audio_play(DOWN_SLOP_16K_16BITS_PCM_DATA, DOWN_SLOP_16K_16BITS_PCM_DATA_LEN, PCM_PLAY_TYPE_REAL);
		result = drv_audio_play(GPS_FIXED_16K_16BITS_PCM_DATA, GPS_FIXED_16K_16BITS_PCM_DATA_LEN, PCM_PLAY_TYPE_NORMAL);
		if(result < 0)
		{

		}
		sleep(5);
	}
#if 0
    pthread_create(&pid, NULL, thread_play_entry, &chunk_bytes);


	while(1)
	{
            sleep(20);
	    if(flag)
	    {	
		flag = 0;
		drv_audio_play(DOWN_SLOP_16K_16BITS_PCM_DATA, DOWN_SLOP_16K_16BITS_PCM_DATA_LEN, PCM_PLAY_TYPE_REAL); 
	    }else{
		flag = 1;
		drv_audio_play(GPS_FIXED_16K_16BITS_PCM_DATA, GPS_FIXED_16K_16BITS_PCM_DATA_LEN, PCM_PLAY_TYPE_NORMAL); 
	    }
	}

#endif	
#if 0
	{
		int offset = 0;
		for(offset =0; offset < DD_16K_16BITS_PCM_DATA_LEN; offset += chunk_bytes)
		{
			int len =0;
			len = DD_16K_16BITS_PCM_DATA_LEN - offset;
			if(len > chunk_bytes)
			{
				len = chunk_bytes;
			}
			
			result = drv_audio_write((uint8_t *)&(DD_16K_16BITS_PCM_DATA[offset]), len);
			if(result < 0)
			{
				printf("drv_audio_write error\n");
				goto ERR_EXIT;
			}
		}
	}

#endif
#if 0
	for(offset =0; offset < 640 * 12; offset += chunk_bytes)
	{
		int len =0;
		len = test_gps_fix_16K_16bits_pcm_pcmLen - offset;
		if(len > chunk_bytes)
		{
			len = chunk_bytes;
		}
			
		result = drv_audio_write(&(test_gps_fix_16K_16bits_pcm[offset]), len);
		if(result < 0)
		{
			printf("drv_audio_write error\n");
			goto ERR_EXIT;
		}
	}

	for(offset=0; offset <test_ebd_16k_16bits_pcm_len ;offset += chunk_bytes )
	{
		int len =0;
		len = test_ebd_16k_16bits_pcm_len - offset;
		if(len > chunk_bytes)
		{
			len = chunk_bytes;
		}
			
		result = drv_audio_write(&(test_ebd_16k_16bits_pcm[offset]), len);
		if(result < 0)
		{
			printf("drv_audio_write error\n");
			goto ERR_EXIT;
		}
	}
#endif
	result = drv_audio_close();
	if(result != 0)
	{
		printf("drv_audio_close error\n");
		goto ERR_EXIT;
	}
ERR_EXIT:	
	return 0;
}
