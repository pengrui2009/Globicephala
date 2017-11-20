/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : drv_sound.c
 @brief  : play PCM sound data.
 @author : pengrui
 @history:
           2017-10-16    pengrui    Created file
           ...
******************************************************************************/
#include "drv_audio.h"
#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <fcntl.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <time.h>
#include <locale.h>
#include <alsa/asoundlib.h>
#include <assert.h>
#include <termios.h>
#include <sys/poll.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <endian.h>

#include "error.h"
#ifndef LLONG_MAX
#define LLONG_MAX    9223372036854775807LL
#endif

#define _(string)    (string)  

#ifndef le16toh
#include <asm/byteorder.h>
#define le16toh(x) __le16_to_cpu(x)
#define be16toh(x) __be16_to_cpu(x)
#define le32toh(x) __le32_to_cpu(x)
#define be32toh(x) __be32_to_cpu(x)
#endif

//mixer config structure
typedef struct{
    uint8_t left_volume;
    uint8_t right_volume;
    sound_pcm_chan_en channels;
}speaker_info_t;

//the pcm data node info
typedef struct _pcm_data_node_t
{
    struct list_head list;
    uint8_t *pcm_data;
    uint32_t pcm_len;
}pcm_data_node_t;

//pcm data write to the list and wait to play
typedef struct _pcm_play_list_info_t
{
    //the node of pcm data
    pcm_data_node_t pcm_node[MAX_PCM_NODE_NUM];
	//the mutex of pcm list
    pthread_mutex_t mutex;
	
    struct list_head list_idle;
	struct list_head list_normal;//normal sound data
	struct list_head list_real;//emergent sound data
}pcm_play_list_info_t;

/* global data */

static snd_pcm_sframes_t (*readi_func)(snd_pcm_t *handle, void *buffer, snd_pcm_uframes_t size);
static snd_pcm_sframes_t (*writei_func)(snd_pcm_t *handle, const void *buffer, snd_pcm_uframes_t size);
static snd_pcm_sframes_t (*readn_func)(snd_pcm_t *handle, void **bufs, snd_pcm_uframes_t size);
static snd_pcm_sframes_t (*writen_func)(snd_pcm_t *handle, void **bufs, snd_pcm_uframes_t size);

//pcm handle
static snd_pcm_t *pcm_handle = NULL;
//mixer handle
static snd_mixer_t *mixer_handle = NULL;
static int mmap_flag = 0;
static int nonblock = 1;
static int monotonic = 0;
static size_t bits_per_sample, bits_per_frame;
static snd_pcm_uframes_t chunk_size = 0;
static int vumeter = SOUND_PCM_CHANNEL_NONE;
static pcm_params_info_t hwparams = {0};



//pcm data stored list
static pcm_play_list_info_t pcm_play_list;
static osal_task_t task_audio;// audio task


/* needed prototypes */

#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 95)
#define error(...) do {\
    fprintf(stderr, "%s: %s:%d: ", "drv_audio", __FUNCTION__, __LINE__); \
    fprintf(stderr, __VA_ARGS__); \
    putc('\n', stderr); \
} while (0)
#else
#define error(args...) do {\
    fprintf(stderr, "%s: %s:%d: ", "drv_audio", __FUNCTION__, __LINE__); \
    fprintf(stderr, ##args); \
    putc('\n', stderr); \
} while (0)
#endif    
/* open pcm device. */
static int pcm_open(void)
{
    int result = 0;
    int open_mode = 0;
    char *pcm_name = "default";
    snd_pcm_info_t *info;
    snd_pcm_stream_t stream = SND_PCM_STREAM_PLAYBACK;

    if(nonblock)
    {
        open_mode |= SND_PCM_NONBLOCK;
    }

#ifdef ENABLE_NLS
    setlocale(LC_ALL, "");
    textdomain(PACKAGE);
#endif

    snd_pcm_info_alloca(&info);
    
    chunk_size = -1;

    result = snd_pcm_open(&pcm_handle, pcm_name, stream, open_mode);
    if (result < 0) 
    {
        fprintf(stderr, "snd_pcm_open() error: %s\n", snd_strerror(result));
        result = -ERR_SYS;
        goto ERR_EXIT;
    }

    result = snd_pcm_info(pcm_handle, info);
    if (result < 0) 
    {
        fprintf(stderr, "snd_pcm_info() error: %s\n", snd_strerror(result));
		snd_pcm_info_free(info);
        result = -ERR_SYS;
        goto ERR_EXIT;
    }

    if (nonblock) 
    {
        result = snd_pcm_nonblock(pcm_handle, 1);
        if (result < 0) {
            fprintf(stderr, "snd_pcm_nonblock() error: %s\n", snd_strerror(result));
            result = -ERR_SYS;
            goto ERR_EXIT;
        }
    }

    if (mmap_flag) {
        writei_func = snd_pcm_mmap_writei;
        readi_func = snd_pcm_mmap_readi;
        writen_func = snd_pcm_mmap_writen;
        readn_func = snd_pcm_mmap_readn;
    } else {
        writei_func = snd_pcm_writei;
        readi_func = snd_pcm_readi;
        writen_func = snd_pcm_writen;
        readn_func = snd_pcm_readn;
    }
	
	return result;
ERR_EXIT:

	if(pcm_handle)
	{
		snd_pcm_close(pcm_handle);  
		pcm_handle = NULL;
	}
	
    return result;    
}

void pcm_close(void)
{
	if(pcm_handle)
	{
		if (nonblock) 
	    {
	        snd_pcm_nonblock(pcm_handle, 0);              
            snd_pcm_drain(pcm_handle);
            snd_pcm_nonblock(pcm_handle, nonblock);
	    }
		snd_pcm_drain(pcm_handle);
		snd_pcm_close(pcm_handle);  
		pcm_handle = NULL;
	}
}

static int pcm_config(snd_pcm_t *handle, pcm_params_info_t hwparams)
{
    int result = 0;
    
    if(handle == NULL)
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
    snd_pcm_hw_params_t *params;
    snd_pcm_sw_params_t *swparams;
    snd_pcm_uframes_t buffer_size;
    size_t n;
    unsigned int rate;
    unsigned period_time = 0;
    unsigned buffer_time = 0;
    int start_delay = 0;
    int stop_delay = 0;
    snd_pcm_uframes_t period_frames = 0;
    snd_pcm_uframes_t buffer_frames = 0;
    snd_pcm_uframes_t start_threshold, stop_threshold;
    
    snd_pcm_hw_params_alloca(&params);
    snd_pcm_sw_params_alloca(&swparams);
    result = snd_pcm_hw_params_any(handle, params);
    if (result < 0) {
        fprintf(stderr, "Broken configuration for this PCM: no configurations available\n");
        result = -ERR_SYS;
        goto ERR_EXIT;
    }
    
    if (mmap_flag) {
        snd_pcm_access_mask_t *mask = alloca(snd_pcm_access_mask_sizeof());
        snd_pcm_access_mask_none(mask);
        snd_pcm_access_mask_set(mask, SND_PCM_ACCESS_MMAP_INTERLEAVED);
        snd_pcm_access_mask_set(mask, SND_PCM_ACCESS_MMAP_NONINTERLEAVED);
        snd_pcm_access_mask_set(mask, SND_PCM_ACCESS_MMAP_COMPLEX);
        result = snd_pcm_hw_params_set_access_mask(handle, params, mask);
    }else{
        result = snd_pcm_hw_params_set_access(handle, params,
                           SND_PCM_ACCESS_RW_INTERLEAVED);
    }
    
    if (result < 0) {
		fprintf(stderr, "Access type not available\n");
        result = -ERR_SYS;
        goto ERR_EXIT;
    }
    
    result = snd_pcm_hw_params_set_format(handle, params, hwparams.format);
    if (result < 0) {
		fprintf(stderr, "Sample format non available\n");
        result = -ERR_SYS;
        goto ERR_EXIT;
    }
    result = snd_pcm_hw_params_set_channels(handle, params, hwparams.channels);
    if (result < 0) {
		fprintf(stderr, "Channels count non available\n");
		result = -ERR_SYS;
        goto ERR_EXIT;
    }

#if 0
    result = snd_pcm_hw_params_set_periods_min(handle, params, 2);
    assert(err >= 0);
#endif
    rate = hwparams.rate;
    result = snd_pcm_hw_params_set_rate_near(handle, params, &hwparams.rate, 0);
    if(result < 0)
    {
		
        result = -ERR_SYS;
        goto ERR_EXIT;
    }
    
    if ((float)rate * 1.05 < hwparams.rate || (float)rate * 0.95 > hwparams.rate) {
        
    }
    rate = hwparams.rate;
    if (buffer_time == 0 && buffer_frames == 0) {
        result = snd_pcm_hw_params_get_buffer_time_max(params,
                                &buffer_time, 0);
        if(result < 0)
        {
			fprintf(stderr, "snd_pcm_hw_params_get_buffer_time_max\n");
            result = -ERR_SYS;
            goto ERR_EXIT;
        }
        if (buffer_time > 500000)
            buffer_time = 500000;
    }
    if (period_time == 0 && period_frames == 0) {
        if (buffer_time > 0)
            period_time = buffer_time / 4;
        else
            period_frames = buffer_frames / 4;
    }
    
    if (period_time > 0)
        result = snd_pcm_hw_params_set_period_time_near(handle, params,
                                 &period_time, 0);
    else
        result = snd_pcm_hw_params_set_period_size_near(handle, params,
                                 &period_frames, 0);
    if(result < 0)
    {
		fprintf(stderr, "snd_pcm_hw_params_set_period_size_near() error\n");
        result = -ERR_SYS;
        goto ERR_EXIT;
    }

    if (buffer_time > 0) {
        result = snd_pcm_hw_params_set_buffer_time_near(handle, params,
                                 &buffer_time, 0);
    } else {
        result = snd_pcm_hw_params_set_buffer_size_near(handle, params,
                                 &buffer_frames);
    }
    if(result < 0)
    {
		fprintf(stderr, "snd_pcm_hw_params_set_period_size_near() error \n");
        result = -ERR_SYS;
        goto ERR_EXIT;
    }
    
    monotonic = snd_pcm_hw_params_is_monotonic(params);
    //can_pause = snd_pcm_hw_params_can_pause(params);
    result = snd_pcm_hw_params(handle, params);
    if (result < 0) {
		fprintf(stderr, "Unable to install hw params \n");
        result = -ERR_SYS;
        goto ERR_EXIT;
    }
    snd_pcm_hw_params_get_period_size(params, &chunk_size, 0);
    snd_pcm_hw_params_get_buffer_size(params, &buffer_size);
    if (chunk_size == buffer_size) {
		fprintf(stderr, "Can't use period equal to buffer size (%lu == %lu)\n", chunk_size, buffer_size);
        result = -ERR_SYS;
        goto ERR_EXIT;
    }
    snd_pcm_sw_params_current(handle, swparams);
    
    n = chunk_size;
    
    result = snd_pcm_sw_params_set_avail_min(handle, swparams, n);

    /* round up to closest transfer boundary */
    n = buffer_size;
    if (start_delay <= 0) {
        start_threshold = n + (double) rate * start_delay / 1000000;
    } else
        start_threshold = (double) rate * start_delay / 1000000;
    if (start_threshold < 1)
        start_threshold = 1;
    if (start_threshold > n)
        start_threshold = n;
    result = snd_pcm_sw_params_set_start_threshold(handle, swparams, start_threshold);
    if (result < 0) 
    {
		fprintf(stderr, "snd_pcm_sw_params_set_start_threshold() error\n");
        result = -ERR_SYS;
        goto ERR_EXIT;
    }
    
    if (stop_delay <= 0) 
        stop_threshold = buffer_size + (double) rate * stop_delay / 1000000;
    else
        stop_threshold = (double) rate * stop_delay / 1000000;
    result = snd_pcm_sw_params_set_stop_threshold(handle, swparams, stop_threshold);
    if (result < 0) 
    {
		fprintf(stderr, "snd_pcm_sw_params_set_start_threshold() error.\n");
        result = -ERR_SYS;
        goto ERR_EXIT;
    }

    if (snd_pcm_sw_params(handle, swparams) < 0) {
		fprintf(stderr, "unable to install sw params \n");
        result = -ERR_SYS;
        goto ERR_EXIT;
    }

    bits_per_sample = snd_pcm_format_physical_width(hwparams.format);
    bits_per_frame = bits_per_sample * hwparams.channels;
    
    
    //audiobuf = realloc(audiobuf, chunkbytes);
    
    // fprintf(stderr, "real chunk_size = %i, frags = %i, total = %i\n", chunk_size, setup.buf.block.frags, setup.buf.block.frags * chunk_size);

    /* show mmap buffer arragment */
    if (mmap_flag) {
        const snd_pcm_channel_area_t *areas;
        snd_pcm_uframes_t offset, size = chunk_size;
        int i;
        result = snd_pcm_mmap_begin(handle, &areas, &offset, &size);
        if (result < 0) {
			fprintf(stderr, "snd_pcm_mmap_begin problem: %s\n", snd_strerror(result));
            result = -ERR_SYS;
            goto ERR_EXIT;
        }
		
        for (i = 0; i < hwparams.channels; i++)
            fprintf(stderr, "mmap_area[%i] = %p,%u,%u (%u)\n", i, areas[i].addr, areas[i].first, areas[i].step, snd_pcm_format_physical_width(hwparams.format));
        /* not required, but for sure */
        snd_pcm_mmap_commit(handle, offset, 0);
    }

    buffer_frames = buffer_size;    /* for position test */
    result = chunk_size * bits_per_frame / 8;

ERR_EXIT:    

    return result;
}


#ifndef timersub
#define    timersub(a, b, result) \
do { \
    (result)->tv_sec = (a)->tv_sec - (b)->tv_sec; \
    (result)->tv_usec = (a)->tv_usec - (b)->tv_usec; \
    if ((result)->tv_usec < 0) { \
        --(result)->tv_sec; \
        (result)->tv_usec += 1000000; \
    } \
} while (0)
#endif

#ifndef timermsub
#define    timermsub(a, b, result) \
do { \
    (result)->tv_sec = (a)->tv_sec - (b)->tv_sec; \
    (result)->tv_nsec = (a)->tv_nsec - (b)->tv_nsec; \
    if ((result)->tv_nsec < 0) { \
        --(result)->tv_sec; \
        (result)->tv_nsec += 1000000000L; \
    } \
} while (0)
#endif

/* I/O error handler */
static int xrun(snd_pcm_t *handle)
{
    int result = 0;
    snd_pcm_status_t *status;
    
    snd_pcm_status_alloca(&status);
    
    result = snd_pcm_status(handle, status);
    if (result < 0) {
        error(_("status error: %s"), snd_strerror(result));
        result = -ERR_SYS;
        goto ERR_EXIT;
    }
    if (snd_pcm_status_get_state(status) == SND_PCM_STATE_XRUN) {
        if (monotonic) {
#ifdef HAVE_CLOCK_GETTIME
            struct timespec now, diff, tstamp;
            clock_gettime(CLOCK_MONOTONIC, &now);
            snd_pcm_status_get_trigger_htstamp(status, &tstamp);
            timermsub(&now, &tstamp, &diff);
            fprintf(stderr, _("%s!!! (at least %.3f ms long)\n"),
                _("underrun"), diff.tv_sec * 1000 + diff.tv_nsec / 10000000.0);
#else
            //fprintf(stderr, "%s !!!\n", _("underrun"));
#endif
        } else {
            struct timeval now, diff, tstamp;
            gettimeofday(&now, 0);
            snd_pcm_status_get_trigger_tstamp(status, &tstamp);
            timersub(&now, &tstamp, &diff);
            fprintf(stderr, _("%s!!! (at least %.3f ms long)\n"),
                 _("underrun"), diff.tv_sec * 1000 + diff.tv_usec / 1000.0);
        }
        
        result = snd_pcm_prepare(handle);
        if (result < 0) {
            error(_("xrun: prepare error: %s"), snd_strerror(result));
            result = -ERR_SYS;
            goto ERR_EXIT;
        }
        return result;        /* ok, data should be accepted again */
    }
    
    if (snd_pcm_status_get_state(status) == SND_PCM_STATE_DRAINING) {
        return result;
    }

ERR_EXIT:

    return result;
}

/* I/O suspend handler */
static void suspend(snd_pcm_t *handle)
{
    int res;

    fflush(stderr);
    while ((res = snd_pcm_resume(handle)) == -EAGAIN)
        sleep(1);    /* wait until suspend flag is released */
    if (res < 0) {
    
    }
}

static void print_vu_meter_mono(int perc, int maxperc)
{
    const int bar_length = 50;
    char line[80];
    int val;

    for (val = 0; val <= perc * bar_length / 100 && val < bar_length; val++)
        line[val] = '#';
    for (; val <= maxperc * bar_length / 100 && val < bar_length; val++)
        line[val] = ' ';
    line[val] = '+';
    for (++val; val <= bar_length; val++)
        line[val] = ' ';
    if (maxperc > 99)
        sprintf(line + val, "| MAX");
    else
        sprintf(line + val, "| %02i%%", maxperc);
    fputs(line, stdout);
    if (perc > 100)
        printf(_(" !clip  "));
}

static void print_vu_meter_stereo(int *perc, int *maxperc)
{
    const int bar_length = 35;
    char line[80];
    int c;

    memset(line, ' ', sizeof(line) - 1);
    line[bar_length + 3] = '|';

    for (c = 0; c < 2; c++) {
        int p = perc[c] * bar_length / 100;
        char tmp[4];
        if (p > bar_length)
            p = bar_length;
        if (c)
            memset(line + bar_length + 6 + 1, '#', p);
        else
            memset(line + bar_length - p - 1, '#', p);
        p = maxperc[c] * bar_length / 100;
        if (p > bar_length)
            p = bar_length;
        if (c)
            line[bar_length + 6 + 1 + p] = '+';
        else
            line[bar_length - p - 1] = '+';
        if (maxperc[c] > 99)
            sprintf(tmp, "MAX");
        else
            sprintf(tmp, "%02d%%", maxperc[c]);
        if (c)
            memcpy(line + bar_length + 3 + 1, tmp, 3);
        else
            memcpy(line + bar_length, tmp, 3);
    }
    line[bar_length * 2 + 6 + 2] = 0;
    fputs(line, stdout);
}

static void print_vu_meter(signed int *perc, signed int *maxperc)
{
    if (vumeter == SOUND_PCM_CHANNEL_STEREO)
        print_vu_meter_stereo(perc, maxperc);
    else
        print_vu_meter_mono(*perc, *maxperc);
}

/* peak handler */
static void compute_max_peak(u_char *data, size_t count)
{
    signed int val, max, perc[2], max_peak[2];
    static    int    run = 0;
    
    int    format_little_endian = snd_pcm_format_little_endian(hwparams.format);    
    int ichans, c;

    if (vumeter == SOUND_PCM_CHANNEL_STEREO)
        ichans = 2;
    else
        ichans = 1;

    memset(max_peak, 0, sizeof(max_peak));
    switch (bits_per_sample) {
    case 8: {
        signed char *valp = (signed char *)data;
        signed char mask = snd_pcm_format_silence(hwparams.format);
        c = 0;
        while (count-- > 0) {
            val = *valp++ ^ mask;
            val = abs(val);
            if (max_peak[c] < val)
                max_peak[c] = val;
            if (vumeter == SOUND_PCM_CHANNEL_STEREO)
                c = !c;
        }
        break;
    }
    case 16: {
        signed short *valp = (signed short *)data;
        signed short mask = snd_pcm_format_silence_16(hwparams.format);
        signed short sval;

        count /= 2;
        c = 0;
        while (count-- > 0) {
            if (format_little_endian)
                sval = le16toh(*valp);
            else
                sval = be16toh(*valp);
            sval = abs(sval) ^ mask;
            if (max_peak[c] < sval)
                max_peak[c] = sval;
            valp++;
            if (vumeter == SOUND_PCM_CHANNEL_STEREO)
                c = !c;
        }
        break;
    }
    case 24: {
        unsigned char *valp = data;
        signed int mask = snd_pcm_format_silence_32(hwparams.format);

        count /= 3;
        c = 0;
        while (count-- > 0) {
            if (format_little_endian) {
                val = valp[0] | (valp[1]<<8) | (valp[2]<<16);
            } else {
                val = (valp[0]<<16) | (valp[1]<<8) | valp[2];
            }
            /* Correct signed bit in 32-bit value */
            if (val & (1<<(bits_per_sample-1))) {
                val |= 0xff<<24;    /* Negate upper bits too */
            }
            val = abs(val) ^ mask;
            if (max_peak[c] < val)
                max_peak[c] = val;
            valp += 3;
            if (vumeter == SOUND_PCM_CHANNEL_STEREO)
                c = !c;
        }
        break;
    }
    case 32: {
        signed int *valp = (signed int *)data;
        signed int mask = snd_pcm_format_silence_32(hwparams.format);

        count /= 4;
        c = 0;
        while (count-- > 0) {
            if (format_little_endian)
                val = le32toh(*valp);
            else
                val = be32toh(*valp);
            val = abs(val) ^ mask;
            if (max_peak[c] < val)
                max_peak[c] = val;
            valp++;
            if (vumeter == SOUND_PCM_CHANNEL_STEREO)
                c = !c;
        }
        break;
    }
    default:
        if (run == 0) {
            fprintf(stderr, _("Unsupported bit size %d.\n"), (int)bits_per_sample);
            run = 1;
        }
        return;
    }
    max = 1 << (bits_per_sample-1);
    if (max <= 0)
        max = 0x7fffffff;

    for (c = 0; c < ichans; c++) {
        if (bits_per_sample > 16)
            perc[c] = max_peak[c] / (max / 100);
        else
            perc[c] = max_peak[c] * 100 / max;
    }

    static int maxperc[2];
    static time_t t=0;
    const time_t tt=time(NULL);
    if(tt>t) {
        t=tt;
        maxperc[0] = 0;
        maxperc[1] = 0;
    }
    for (c = 0; c < ichans; c++)
        if (perc[c] > maxperc[c])
            maxperc[c] = perc[c];

    putchar('\r');
    print_vu_meter(perc, maxperc);
    fflush(stdout);
    
}

/*
 *  write function
 */

static ssize_t pcm_write(snd_pcm_t *handle, u_char *data, size_t count)
{
    ssize_t r;
    ssize_t result = 0;

    if (count < chunk_size) {
        snd_pcm_format_set_silence(hwparams.format, data + count * bits_per_frame / 8, (chunk_size - count) * hwparams.channels);
        count = chunk_size;
    }
    while (count > 0) {

        r = writei_func(handle, data, count);
        if (r == -EAGAIN || (r >= 0 && (size_t)r < count)) {
            snd_pcm_wait(handle, 100);
        } else if (r == -EPIPE) {
            xrun(handle);
        } else if (r == -ESTRPIPE) {
            suspend(handle);
        } else if (r < 0) {
            error(_("write error: %s"), snd_strerror(r));
            result = -ERR_SYS;
            goto ERR_EXIT;
        }
        if (r > 0) {
            if (vumeter)
                compute_max_peak(data, r * hwparams.channels);
            result += r;
            count -= r;
            data += r * bits_per_frame / 8;
        }
    }
    
ERR_EXIT:    
    
    return result;
}

/******************************************************************************
*    Function: drv_audio_write
*    Descriptions: play one piece of pcm data, the size of piece is chunk_bytes 
                  when we call drv_audio_open.
*    Paramters:
            info_ptr                - the info of sound module
            data_ptr                - the data of play
            data_len                - the len of data
*    Return:
            = 0                     - success
            < 0                     - failed
*    Comments: 
******************************************************************************/
static int drv_audio_write(uint8_t *data_ptr, uint16_t data_len)
{
    int result = 0;
    uint16_t len = 0;
    uint8_t *audiobuf_ptr = NULL;
    uint16_t chunk_bytes = chunk_size * bits_per_frame / 8;
    
    if(data_ptr == NULL)
    {
        result = -ERR_NOMEM;
        goto ERR_EXIT;
    }
    
    if(data_len > chunk_bytes)
    {
        data_len = chunk_bytes;
    }
    
    audiobuf_ptr = os_calloc(1, chunk_bytes);
    if(audiobuf_ptr == NULL)
    {
        fprintf(stderr, "calloc result:%d\n", result);
        result = -ERR_NOMEM;
        goto ERR_EXIT;
    }
    
    memcpy(audiobuf_ptr, data_ptr, data_len);
    
    
    len = data_len * 8 / bits_per_frame;
    result = pcm_write(pcm_handle, audiobuf_ptr, len);
    if (result != len) 
    {
        goto ERR_EXIT;
    }    
    
    result = 0;
ERR_EXIT:
    
    if(audiobuf_ptr)
    {
        os_free(audiobuf_ptr);
    }
    
    return result;
}
/* open mixer*/
static int mixer_open(void)
{
    int result = 0;
    char *mixer_name = "default";
    
    //open mixer
    if ((result = snd_mixer_open(&mixer_handle, 0)) < 0)
    {
        fprintf(stderr, "snd_mixer_open error\n");
        result = -ERR_SYS;
        goto ERR_EXIT;
    }
    // Attach an HCTL to an opened mixer
    result = snd_mixer_attach(mixer_handle, mixer_name);
    if (result < 0)
    {
        fprintf(stderr, "snd_mixer_attach error\n");
        result = -ERR_SYS;
        goto ERR_EXIT1;
    }
    //register mixer
    if ((result = snd_mixer_selem_register(mixer_handle, NULL, NULL)) < 0)
    {
        fprintf(stderr, "snd_mixer_selem_register error\n");
        result = -ERR_SYS;
        goto ERR_EXIT2;
    }
    //load mixer
    if ((result = snd_mixer_load(mixer_handle)) < 0)
    {
        fprintf(stderr, "snd_mixer_load error\n");
        result = -ERR_SYS;
        goto ERR_EXIT2;
    }

	return result;
ERR_EXIT2:
    snd_mixer_detach(mixer_handle, mixer_name);
ERR_EXIT1:
	snd_mixer_close(mixer_handle);
	mixer_handle = NULL;
ERR_EXIT:
    return result;
}
/*close mixer*/
void mixer_close(void)
{
	char *mixer_name = "default";
	
	if(mixer_handle)
	{
		snd_mixer_detach(mixer_handle, mixer_name);
		
		snd_mixer_close(mixer_handle);
		mixer_handle = NULL;
	}
	
}
/* config mixer */
static int mixer_config(snd_mixer_t *handle, uint8_t volume)
{
    int result = 0;
    snd_mixer_elem_t *elem = NULL;
    long minVolume = 0, maxVolume = 0;
    
    if(handle == NULL)
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }
    
   // 
    for(elem=snd_mixer_first_elem(handle); elem; elem=snd_mixer_elem_next(elem))
    {
        if (snd_mixer_elem_get_type(elem) == SND_MIXER_ELEM_SIMPLE &&
             snd_mixer_selem_is_active(elem)) // æ‰¾åˆ°å¯ä»¥ç”¨çš„, æ¿€æ´»çš„elem
        {
            if(strcmp(snd_mixer_selem_get_name(elem), "Speaker Boost") == 0)
            {
                
            }else if(strcmp(snd_mixer_selem_get_name(elem), "Speaker Mixer") == 0){
                
            }else if(strcmp(snd_mixer_selem_get_name(elem), "Speaker ZC") == 0){
                
            }else if(strcmp(snd_mixer_selem_get_name(elem), "Speaker") == 0)
            {
                snd_mixer_selem_get_playback_volume_range(elem, &minVolume, &maxVolume);
                
                if(volume < minVolume)
                {
                    volume = minVolume;
                }
                
                if(volume > maxVolume)
                {
                    volume = maxVolume;
                }
            
                result = snd_mixer_selem_set_playback_volume_all(elem, (long)volume);
                if(result != 0)
                {
                    goto ERR_EXIT;
                }
            }
        }
    }
    
ERR_EXIT:
    
    return result;
}

/******************************************************************************
*    Function: thread_play_entry
*    Descriptions: play the pcm data.
*    Paramters:
            arg                    - the arguments of the device
*    Return:
            = 0                    - success, return the chunk_size of pcm data
            < 0                    - failed
*    Comments: 
******************************************************************************/
static void *thread_play_entry(void *arg)
{
    int result = 0;
    
    while(1)
    {
        if(list_empty(&pcm_play_list.list_real) && list_empty(&pcm_play_list.list_normal))
        {
            //wait the pcm data list
            usleep(10000);
        }else{
            if (!list_empty(&pcm_play_list.list_real))
            { 
                uint16_t offset = 0;
                uint16_t pcm_len = 0;
                uint8_t *pcm_data = NULL;
                pcm_data_node_t *pnode = NULL;
                int chunk_bytes = chunk_size * bits_per_frame / 8;;
                //»ñµÃ»¥³âËø
                pthread_mutex_lock (&pcm_play_list.mutex);
                
                pnode = list_first_entry(&pcm_play_list.list_real, pcm_data_node_t, list);
                list_del (&pnode->list);
                pcm_data = pnode->pcm_data;
                pcm_len = pnode->pcm_len;
                pthread_mutex_unlock (&pcm_play_list.mutex);
            	
                for(offset=0; offset < pcm_len; offset += chunk_bytes )
                {
                    int len =0;
                    len = pcm_len - offset;
                    if(len > chunk_bytes)
                    {
                        len = chunk_bytes;
                    }
					
                    result = drv_audio_write(&(pcm_data[offset]), len);
                    if(result < 0)
                    {
                        printf("drv_audio_write error\n");
                        //goto ERR_EXIT;
                    }
                }
                //»ñµÃ»¥³âËø
                 pthread_mutex_lock (&pcm_play_list.mutex);
                //snd_pcm_nonblock(pcm_handle, 0);
                //snd_pcm_drain(pcm_handle);
                //snd_pcm_nonblock(pcm_handle, nonblock);
                if(pnode->pcm_data)
                {
                    os_free(pnode->pcm_data);
					pnode->pcm_data = NULL;
                }
                list_add (&pnode->list, &pcm_play_list.list_idle);
                //»ñµÃ»¥³âËø
                pthread_mutex_unlock (&pcm_play_list.mutex);
            
            }else if (!list_empty(&pcm_play_list.list_normal)){
                uint16_t offset = 0;
                uint16_t pcm_len = 0;
                uint8_t *pcm_data = NULL;
                pcm_data_node_t *pnode = NULL;
                int chunk_bytes = chunk_size * bits_per_frame / 8;;
                
                //»ñµÃ»¥³âËø
                pthread_mutex_lock (&pcm_play_list.mutex);
                pnode = list_first_entry(&pcm_play_list.list_normal, pcm_data_node_t, list);
                list_del (&pnode->list);
                pcm_data = pnode->pcm_data;
                pcm_len = pnode->pcm_len;
                pthread_mutex_unlock (&pcm_play_list.mutex);
                
                for(offset=0; offset < pcm_len ;offset += chunk_bytes )
                {
                    int len =0;
    
                    if (!list_empty(&pcm_play_list.list_real))
                    {    
                        break;
                    }
                    
                    len = pcm_len - offset;
                    if(len > chunk_bytes)
                    {
                        len = chunk_bytes;
                    }
                    
                    result = drv_audio_write(&(pcm_data[offset]), len);
                    if(result < 0)
                    {
                        //printf("drv_audio_write error\n");
                        //goto ERR_EXIT;
                    }
                }
                //»ñµÃ»¥³âËø
                 pthread_mutex_lock (&pcm_play_list.mutex);

                if(pnode->pcm_data)
                {
	                os_free(pnode->pcm_data);
					pnode->pcm_data = NULL;
                }
                list_add (&pnode->list, &pcm_play_list.list_idle);
                //»ñµÃ»¥³âËø
                 pthread_mutex_unlock (&pcm_play_list.mutex);
            
            }
        }
    }

    return (void *)NULL;
}

/******************************************************************************
*    Function: drv_audio_open
*    Descriptions: open the pcm sound device and mixer device.
*    Paramters:
            void
*    Return:
            = 0                    - success, return the chunk_size of pcm data
            < 0                    - failed
*    Comments: 
******************************************************************************/
int drv_audio_open(void)
{
    int i = 0;
    int result = 0;

    INIT_LIST_HEAD(&pcm_play_list.list_idle);
    INIT_LIST_HEAD(&pcm_play_list.list_normal);
    INIT_LIST_HEAD(&pcm_play_list.list_real);

    for(i=0; i< MAX_PCM_NODE_NUM; i++)
    {
         list_add(&pcm_play_list.pcm_node[i].list, &pcm_play_list.list_idle);
    }

    //»¥³âÁ¿³õÊ¼»¯
    if (pthread_mutex_init(&pcm_play_list.mutex, NULL)) 
    {
        result = -ERR_SYS;
        goto ERR_EXIT;
    }
    
    result = pcm_open();
    if(result < 0)
    {
        result = -ERR_INVAL;
        goto ERR_EXIT1;
    }

    result = mixer_open();
    if(result < 0)
    {
        result = -ERR_INVAL;
        goto ERR_EXIT2;
    }
	
    result = osal_task_create(&task_audio, "tk-audio", thread_play_entry, NULL, AUDIO_TASK_STACK_SIZE, AUDIO_TASK_PRIORITY);
    if(result)
    {
        goto ERR_EXIT3;
    }

	return result;
ERR_EXIT3:
	mixer_close();
ERR_EXIT2:
	pcm_close();
ERR_EXIT1:
	pthread_mutex_destroy(&pcm_play_list.mutex);
ERR_EXIT:

    return result;
}

/******************************************************************************
*    Function: drv_audio_config
*    Descriptions: config pcm and mixer device.
*    Paramters:
            config_ptr            - the config paramters of sound module
*    Return:
            > 0                    - success
            < 0                    - failed
*    Comments: 
******************************************************************************/
int drv_audio_config(drv_sound_config_t *config_ptr)
{
    int result = 0;
    int chunk_bytes = 0;
    
    if(config_ptr == NULL)
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    result = pcm_config(pcm_handle, config_ptr->pcm_params);
    if(result < 0)
    {
        goto ERR_EXIT;
    }
    chunk_bytes = result;
    
    result = mixer_config(mixer_handle, config_ptr->speaker_params.volume);
    if(result < 0)
    {
        goto ERR_EXIT;
    }
    
    result = chunk_bytes;
    
ERR_EXIT:    
    
    return result;
}

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
int drv_audio_play(const uint8_t *pcm_data, uint32_t pcm_len, pcm_play_type_en play_type)
{
    int result = 0;
    
    if(pcm_data == NULL)
    {
        result = -ERR_INVAL;
        goto ERR_EXIT;
    }

    switch(play_type)
    {
    case PCM_PLAY_TYPE_NORMAL:
    {
        pcm_data_node_t *pnode;
        //»ñµÃ»¥³âËø
         if (pthread_mutex_lock (&pcm_play_list.mutex)) 
        {
             result = -ERR_SYS;
             goto ERR_EXIT;
         }
        
        if (list_empty(&pcm_play_list.list_idle))
        {
             result = -ERR_NOMEM;
             goto ERR_EXIT;
         }
        
        pnode = list_first_entry(&pcm_play_list.list_idle, pcm_data_node_t, list);
        pnode->pcm_data = os_calloc(1, pcm_len);
        if(NULL == pnode->pcm_data)
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
        memcpy(pnode->pcm_data, pcm_data, pcm_len);
        pnode->pcm_len = pcm_len;
        list_del (&pnode->list);
        list_add (&pnode->list, &pcm_play_list.list_normal);

        //»ñµÃ»¥³âËø
         if (pthread_mutex_unlock (&pcm_play_list.mutex)) 
        {
             result = -ERR_SYS;
             goto ERR_EXIT;
         }
         break;
    }
    case PCM_PLAY_TYPE_REAL:
    {
        pcm_data_node_t *pnode;
        //»ñµÃ»¥³âËø
         if (pthread_mutex_lock (&pcm_play_list.mutex))
        {
             result = -ERR_SYS;
             goto ERR_EXIT;
         }
         
         if (list_empty(&pcm_play_list.list_idle)) 
         {
             result = -ERR_NOMEM;
             goto ERR_EXIT;
         }
        
        pnode = list_first_entry(&pcm_play_list.list_idle, pcm_data_node_t, list);
        pnode->pcm_data = calloc(1, pcm_len);
        if(NULL == pnode->pcm_data)
        {
            result = -ERR_NOMEM;
            goto ERR_EXIT;
        }
        
        memcpy(pnode->pcm_data, pcm_data, pcm_len);
        pnode->pcm_len = pcm_len;
        list_del (&pnode->list);
        list_add (&pnode->list, &pcm_play_list.list_real);
        //»ñµÃ»¥³âËø
        if (pthread_mutex_unlock (&pcm_play_list.mutex)) 
        {
             result = -ERR_SYS;
             goto ERR_EXIT;
        }
                break;
    }
    }
    
ERR_EXIT:

    return result;
}

/******************************************************************************
*    Function: drv_audio_close
*    Descriptions: close the pcm sound device
*    Paramters:
            void                    -    

*    Return:
            = 0                     - success
            < 0                     - failed
*    Comments: 
******************************************************************************/
int drv_audio_close()
{
    int result = 0;
    
    pcm_data_node_t *pnode;
    
    list_for_each_entry(pnode, &pcm_play_list.list_real, list)
    {
        if(pnode->pcm_data)
        {
            os_free(pnode->pcm_data);
			pnode->pcm_data = NULL;
        }
    }
    
    list_for_each_entry(pnode, &pcm_play_list.list_normal, list)
    {
        if(pnode->pcm_data)
        {
            os_free(pnode->pcm_data);
			pnode->pcm_data = NULL;
        }
    }

	pthread_mutex_destroy(&pcm_play_list.mutex);
    
    pcm_close();
	
    mixer_close();

	osal_task_del(task_audio);
	
    snd_config_update_free_global();
    return result;
}
