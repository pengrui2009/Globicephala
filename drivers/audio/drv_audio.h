/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : drv_audio.h
 @brief  : The header file of audio .
 @author : pengrui
 @history:
           2017-10-16     pengrui        Modify for CSAE
           ...
******************************************************************************/
#include "list.h"
#include <stdio.h>
#include <stdint.h>
#include "os_core.h"
#include "osal_thread.h"

#define MAX_PCM_NODE_NUM        50 // max pcm data that we can stored

#define AUDIO_TASK_STACK_SIZE (1024 * 8192)  /*audio task stack size*/
#define AUDIO_TASK_PRIORITY   (TK_PRIO_DEFAULT)      /*audio task priority*/

/** PCM sample format */
typedef enum {
    /** Unknown */
    SOUND_PCM_FORMAT_UNKNOWN = -1,
    /** Signed 8 bit */
    SOUND_PCM_FORMAT_S8 = 0,
    /** Unsigned 8 bit */
    SOUND_PCM_FORMAT_U8,
    /** Signed 16 bit Little Endian */
    SOUND_PCM_FORMAT_S16_LE,
    /** Signed 16 bit Big Endian */
    SOUND_PCM_FORMAT_S16_BE,
    /** Unsigned 16 bit Little Endian */
    SOUND_PCM_FORMAT_U16_LE,
    /** Unsigned 16 bit Big Endian */
    SOUND_PCM_FORMAT_U16_BE,
    /** Signed 24 bit Little Endian using low three bytes in 32-bit word */
    SOUND_PCM_FORMAT_S24_LE,
    /** Signed 24 bit Big Endian using low three bytes in 32-bit word */
    SOUND_PCM_FORMAT_S24_BE,
    /** Unsigned 24 bit Little Endian using low three bytes in 32-bit word */
    SOUND_PCM_FORMAT_U24_LE,
    /** Unsigned 24 bit Big Endian using low three bytes in 32-bit word */
    SOUND_PCM_FORMAT_U24_BE,
    /** Signed 32 bit Little Endian */
    SOUND_PCM_FORMAT_S32_LE,
    /** Signed 32 bit Big Endian */
    SOUND_PCM_FORMAT_S32_BE,
    /** Unsigned 32 bit Little Endian */
    SOUND_PCM_FORMAT_U32_LE,
    /** Unsigned 32 bit Big Endian */
    SOUND_PCM_FORMAT_U32_BE,
    /** Float 32 bit Little Endian, Range -1.0 to 1.0 */
    SOUND_PCM_FORMAT_FLOAT_LE,
    /** Float 32 bit Big Endian, Range -1.0 to 1.0 */
    SOUND_PCM_FORMAT_FLOAT_BE,
    /** Float 64 bit Little Endian, Range -1.0 to 1.0 */
    SOUND_PCM_FORMAT_FLOAT64_LE,
    /** Float 64 bit Big Endian, Range -1.0 to 1.0 */
    SOUND_PCM_FORMAT_FLOAT64_BE,
    /** IEC-958 Little Endian */
    SOUND_PCM_FORMAT_IEC958_SUBFRAME_LE,
    /** IEC-958 Big Endian */
    SOUND_PCM_FORMAT_IEC958_SUBFRAME_BE,
    /** Mu-Law */
    SOUND_PCM_FORMAT_MU_LAW,
    /** A-Law */
    SOUND_PCM_FORMAT_A_LAW,
    /** Ima-ADPCM */
    SOUND_PCM_FORMAT_IMA_ADPCM,
    /** MPEG */
    SOUND_PCM_FORMAT_MPEG,
    /** GSM */
    SOUND_PCM_FORMAT_GSM,
    /** Special */
    SOUND_PCM_FORMAT_SPECIAL = 31,
    /** Signed 24bit Little Endian in 3bytes format */
    SOUND_PCM_FORMAT_S24_3LE = 32,
    /** Signed 24bit Big Endian in 3bytes format */
    SOUND_PCM_FORMAT_S24_3BE,
    /** Unsigned 24bit Little Endian in 3bytes format */
    SOUND_PCM_FORMAT_U24_3LE,
    /** Unsigned 24bit Big Endian in 3bytes format */
    SOUND_PCM_FORMAT_U24_3BE,
    /** Signed 20bit Little Endian in 3bytes format */
    SOUND_PCM_FORMAT_S20_3LE,
    /** Signed 20bit Big Endian in 3bytes format */
    SOUND_PCM_FORMAT_S20_3BE,
    /** Unsigned 20bit Little Endian in 3bytes format */
    SOUND_PCM_FORMAT_U20_3LE,
    /** Unsigned 20bit Big Endian in 3bytes format */
    SOUND_PCM_FORMAT_U20_3BE,
    /** Signed 18bit Little Endian in 3bytes format */
    SOUND_PCM_FORMAT_S18_3LE,
    /** Signed 18bit Big Endian in 3bytes format */
    SOUND_PCM_FORMAT_S18_3BE,
    /** Unsigned 18bit Little Endian in 3bytes format */
    SOUND_PCM_FORMAT_U18_3LE,
    /** Unsigned 18bit Big Endian in 3bytes format */
    SOUND_PCM_FORMAT_U18_3BE,
    SOUND_PCM_FORMAT_LAST = SOUND_PCM_FORMAT_U18_3BE,

#if __BYTE_ORDER == __LITTLE_ENDIAN
    /** Signed 16 bit CPU endian */
    SOUND_PCM_FORMAT_S16 = SOUND_PCM_FORMAT_S16_LE,
    /** Unsigned 16 bit CPU endian */
    SOUND_PCM_FORMAT_U16 = SOUND_PCM_FORMAT_U16_LE,
    /** Signed 24 bit CPU endian */
    SOUND_PCM_FORMAT_S24 = SOUND_PCM_FORMAT_S24_LE,
    /** Unsigned 24 bit CPU endian */
    SOUND_PCM_FORMAT_U24 = SOUND_PCM_FORMAT_U24_LE,
    /** Signed 32 bit CPU endian */
    SOUND_PCM_FORMAT_S32 = SOUND_PCM_FORMAT_S32_LE,
    /** Unsigned 32 bit CPU endian */
    SOUND_PCM_FORMAT_U32 = SOUND_PCM_FORMAT_U32_LE,
    /** Float 32 bit CPU endian */
    SOUND_PCM_FORMAT_FLOAT = SOUND_PCM_FORMAT_FLOAT_LE,
    /** Float 64 bit CPU endian */
    SOUND_PCM_FORMAT_FLOAT64 = SOUND_PCM_FORMAT_FLOAT64_LE,
    /** IEC-958 CPU Endian */
    SOUND_PCM_FORMAT_IEC958_SUBFRAME = SOUND_PCM_FORMAT_IEC958_SUBFRAME_LE
#elif __BYTE_ORDER == __BIG_ENDIAN
    /** Signed 16 bit CPU endian */
    SOUND_PCM_FORMAT_S16 = SOUND_PCM_FORMAT_S16_BE,
    /** Unsigned 16 bit CPU endian */
    SOUND_PCM_FORMAT_U16 = SOUND_PCM_FORMAT_U16_BE,
    /** Signed 24 bit CPU endian */
    SOUND_PCM_FORMAT_S24 = SOUND_PCM_FORMAT_S24_BE,
    /** Unsigned 24 bit CPU endian */
    SOUND_PCM_FORMAT_U24 = SOUND_PCM_FORMAT_U24_BE,
    /** Signed 32 bit CPU endian */
    SOUND_PCM_FORMAT_S32 = SOUND_PCM_FORMAT_S32_BE,
    /** Unsigned 32 bit CPU endian */
    SOUND_PCM_FORMAT_U32 = SOUND_PCM_FORMAT_U32_BE,
    /** Float 32 bit CPU endian */
    SOUND_PCM_FORMAT_FLOAT = SOUND_PCM_FORMAT_FLOAT_BE,
    /** Float 64 bit CPU endian */
    SOUND_PCM_FORMAT_FLOAT64 = SOUND_PCM_FORMAT_FLOAT64_BE,
    /** IEC-958 CPU Endian */
    SOUND_PCM_FORMAT_IEC958_SUBFRAME = SOUND_PCM_FORMAT_IEC958_SUBFRAME_BE
#else
#error "Unknown endian"
#endif
} sound_pcm_format_en;

/* pcm channel type. */
typedef enum {
    SOUND_PCM_CHANNEL_NONE,
    SOUND_PCM_CHANNEL_MONO,
    SOUND_PCM_CHANNEL_STEREO
}sound_pcm_chan_en;

/* pcm sample rate.*/
typedef enum {
    SOUND_PCM_RATE_8000  = 8000,
    SOUND_PCM_RATE_11025 = 11025,
    SOUND_PCM_RATE_16000 = 16000,
    SOUND_PCM_RATE_22050 = 22050,
    SOUND_PCM_RATE_32000 = 32000,
    SOUND_PCM_RATE_44100 = 44100,
    SOUND_PCM_RATE_48000 = 48000,
    SOUND_PCM_RATE_96000 = 96000
}sound_pcm_rate_en;

typedef enum
{
    PCM_PLAY_TYPE_NORMAL = 0,
    PCM_PLAY_TYPE_REAL
}pcm_play_type_en;

/* pcm param. */
typedef struct 
{
    sound_pcm_format_en format;
    sound_pcm_chan_en channels;
    sound_pcm_rate_en rate;
}pcm_params_info_t;

/* speaker volume param*/
typedef struct{
    uint8_t    volume;
}speaker_params_info_t;

/* sound info structure */
typedef struct 
{
    
}drv_sound_info_t;

/*sound config structure. */
typedef struct
{
    pcm_params_info_t      pcm_params;
    speaker_params_info_t  speaker_params;
}drv_sound_config_t;

/******************************************************************************
*    Function: drv_audio_open
*    Descriptions: open the pcm sound device and get the handle.
*    Paramters:
            void
*    Return:
            > 0                    -    success, return chunk_bytes of pcm data
            < 0                    -    failed
*    Comments: 
******************************************************************************/
extern int drv_audio_open(void);

/******************************************************************************
*    Function: drv_audio_config
*    Descriptions: config pcm and mixer device.
*    Paramters:
            config_ptr            -    the config paramters of sound module
*    Return:
            = 0                -    success
            < 0                -    failed
*    Comments: 
******************************************************************************/
extern int drv_audio_config(drv_sound_config_t *config_ptr);

/******************************************************************************
*    Function: drv_audio_play
*    Descriptions: Add pcm data to the list and wait for play
*    Paramters:
            pcm_data                - the data of pcm
            pcm_len                    - the length of pcm data
            play_type               - the play type
*    Return:
            = 0                     - success
            < 0                     - failed
*    Comments: 
******************************************************************************/
extern int drv_audio_play(const uint8_t *pcm_data, uint32_t pcm_len, pcm_play_type_en play_type);


/******************************************************************************
*    Function: drv_audio_close
*    Descriptions: close the pcm sound device
*    Paramters:
            void                -    

*    Return:
            = 0                     -    success
            < 0                    -    failed
*    Comments: 
******************************************************************************/
extern int drv_audio_close(void);
