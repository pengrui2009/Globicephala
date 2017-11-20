/*****************************************************************************
 Copyright(C) Beijing Carsmart Technology Co., Ltd.
 All rights reserved.
 
 @file   : wireless.c
 @brief  : This file provider the driver of wireless.
 @author : pengrui
 @history:
           2016-08-30    pengrui      Created file
           ...
******************************************************************************/

//库配置头文件

//模块启用开关

//调试头文件

//驱动调用头文件

//C库头文件
#include <stdio.h>                        //printf
#include <fcntl.h>                        //open
#include <unistd.h>                        //read,write
#include <string.h>                        //bzero,memcyp
#include <errno.h>
#include <sys/ioctl.h>                    //ioctl
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
//#include <net/bpf.h>
#include <linux/filter.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netdb.h>

//#include "utils.h"
#include "nl80211.h"
#include "platform.h"
//netlink 库头文件
#include "netlink/netlink.h"
#include "netlink/genl/genl.h"
#include "netlink/genl/family.h"
#include "netlink/genl/ctrl.h"

#include "error.h"
#include "wireless.h"
//提供给用户的头文件
/*************************************************
  宏定义
*************************************************/
#define PASS            0xFF
#define FAIL            0xFE

/* For IEEE80211_RADIOTAP_FLAGS */
// sent/received during CFP
#define    IEEE80211_RADIOTAP_F_CFP        0x01
//sent/received with short preamble
#define    IEEE80211_RADIOTAP_F_SHORTPRE    0x02
//sent/received with WEP encryption
#define    IEEE80211_RADIOTAP_F_WEP        0x04
//sent/received with fragmentation
#define    IEEE80211_RADIOTAP_F_FRAG        0x08
//frame includes FCS
#define    IEEE80211_RADIOTAP_F_FCS        0x10
//frame has padding between 802.11 header and payload (to 32-bit boundary)
#define    IEEE80211_RADIOTAP_F_DATAPAD    0x20
//frame failed FCS check
#define IEEE80211_RADIOTAP_F_BADFCS        0x40

//For IEEE80211_RADIOTAP_RX_FLAGS
//bad PLCP
#define IEEE80211_RADIOTAP_F_RX_BADPLCP    0x0002

//For IEEE80211_RADIOTAP_TX_FLAGS
// failed due to excessive retries
#define IEEE80211_RADIOTAP_F_TX_FAIL    0x0001
//used cts 'protection'
#define IEEE80211_RADIOTAP_F_TX_CTS        0x0002
//used rts/cts handshake
#define IEEE80211_RADIOTAP_F_TX_RTS        0x0004
//don't expect an ACK
#define IEEE80211_RADIOTAP_F_TX_NOACK    0x0008

#define ARRAY_SIZE(ar)     (sizeof(ar)/sizeof(ar[0]))

/*************************************************
  结构类型定义
*************************************************/
struct nl80211_state
{
    struct nl_sock *nlsock;
    int nl80211_id;
};

struct wireless_info_t
{
    int fd;
    wireless_config_st    wireless_config;
    struct nl80211_state nl_state;
    int dev_ifidx;
    uint8_t     dev_flag;           /* 1: IFF_UP,  0:down */
    uint8_t    count;                //模块打开计数器
    uint8_t     mac[MACADDR_LENGTH];
    int monitor_refcount;
};

/*************************************************
  静态全局变量定义
*************************************************/
static int iw_debug = 0;
static struct wireless_info_t wireless_info;
//BPF网络数据包过滤数组
static struct sock_filter msock_filter_insns[] =
{
    /*
     * do a little-endian load of the radiotap length field
     */
    /* load lower byte into A */
    BPF_STMT(BPF_LD  | BPF_B | BPF_ABS, 2),
    /* put it into X (== index register) */
    BPF_STMT(BPF_MISC| BPF_TAX, 0),
    /* load upper byte into A */
    BPF_STMT(BPF_LD  | BPF_B | BPF_ABS, 3),
    /* left-shift it by 8 */
    BPF_STMT(BPF_ALU | BPF_LSH | BPF_K, 8),
    /* or with X */
    BPF_STMT(BPF_ALU | BPF_OR | BPF_X, 0),
    /* put result into X */
    BPF_STMT(BPF_MISC| BPF_TAX, 0),

    /*
     * Allow management frames through, this also gives us those
     * management frames that we sent ourselves with status
     */
    /* load the lower byte of the IEEE 802.11 frame control field */
    BPF_STMT(BPF_LD  | BPF_B | BPF_IND, 0),
    /* mask off frame type and version */
    BPF_STMT(BPF_ALU | BPF_AND | BPF_K, 0xF),
    /* accept frame if it's both 0, fall through otherwise */
    BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, 0, PASS, 0),

    /*
     * TODO: add a bit to radiotap RX flags that indicates
     * that the sending station is not associated, then
     * add a filter here that filters on our DA and that flag
     * to allow us to deauth frames to that bad station.
     *
     * For now allow all To DS data frames through.
     */
    /* load the IEEE 802.11 frame control field */
    BPF_STMT(BPF_LD  | BPF_H | BPF_IND, 0),
    /* mask off frame type, version and DS status */
    BPF_STMT(BPF_ALU | BPF_AND | BPF_K, 0x0F03),
    /* accept frame if version 0, type 2 and To DS, fall through otherwise
     */
    BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, 0x0801, PASS, 0),

#if 0
    /*
     * drop non-data frames
     */
    /* load the lower byte of the frame control field */
    BPF_STMT(BPF_LD   | BPF_B | BPF_IND, 0),
    /* mask off QoS bit */
    BPF_STMT(BPF_ALU  | BPF_AND | BPF_K, 0x0c),
    /* drop non-data frames */
    BPF_JUMP(BPF_JMP  | BPF_JEQ | BPF_K, 8, 0, FAIL),
#endif
    /* load the upper byte of the frame control field */
    BPF_STMT(BPF_LD   | BPF_B | BPF_IND, 1),
    /* mask off toDS/fromDS */
    BPF_STMT(BPF_ALU  | BPF_AND | BPF_K, 0x03),
    /* accept WDS frames */
    BPF_JUMP(BPF_JMP  | BPF_JEQ | BPF_K, 3, PASS, 0),

    /*
     * add header length to index
     */
    /* load the lower byte of the frame control field */
    BPF_STMT(BPF_LD   | BPF_B | BPF_IND, 0),
    /* mask off QoS bit */
    BPF_STMT(BPF_ALU  | BPF_AND | BPF_K, 0x80),
    /* right shift it by 6 to give 0 or 2 */
    BPF_STMT(BPF_ALU  | BPF_RSH | BPF_K, 6),
    /* add data frame header length */
    BPF_STMT(BPF_ALU  | BPF_ADD | BPF_K, 24),
    /* add index, was start of 802.11 header */
    BPF_STMT(BPF_ALU  | BPF_ADD | BPF_X, 0),
    /* move to index, now start of LL header */
    BPF_STMT(BPF_MISC | BPF_TAX, 0),

    /*
     * Accept empty data frames, we use those for
     * polling activity.
     */
    BPF_STMT(BPF_LD  | BPF_W | BPF_LEN, 0),
    BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_X, 0, PASS, 0),

    /*
     * Accept EAPOL frames
     */
    BPF_STMT(BPF_LD  | BPF_W | BPF_IND, 0),
    BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, 0xAAAA0300, 0, FAIL),
    BPF_STMT(BPF_LD  | BPF_W | BPF_IND, 4),
    BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, 0x0000888E, PASS, FAIL),

    /* keep these last two statements or change the code below */
    /* return 0 == "DROP" */
    BPF_STMT(BPF_RET | BPF_K, 0),
    /* return ~0 == "keep all" */
    BPF_STMT(BPF_RET | BPF_K, ~0),
};

static struct sock_fprog msock_filter =
{
    .len = ARRAY_SIZE(msock_filter_insns),
    .filter = msock_filter_insns,
};
static int (*registered_handler)(struct nl_msg *, void *);
static void *registered_handler_data;
/*************************************************
  API函数实现
*************************************************/
/******************************************************************************
*    函数:    ieee80211_channel_to_frequency
*    功能:    无线模块信道转换为频率
*    参数:
*            chan                    -    无线信道号
*            band                    -    无线频段
*    返回:
            > 0                        -    无线频率
            < 0                    -    数据无效
*    说明:
 ******************************************************************************/
int ieee80211_channel_to_frequency(int chan, enum nl80211_band band)
{
    int ret = 0x00;
    /* see 802.11 17.3.8.3.2 and Annex J
     * there are overlapping channel numbers in 5GHz and 2GHz bands */
    if (chan <= 0)
    {
        /* not supported */
        ret = -ERR_INVAL;
        goto error;
    }
    switch (band)
    {
    case NL80211_BAND_2GHZ:
        if (chan == 14)
        {
            ret = 2484;
        }else if (chan < 14){
            ret = 2407 + chan * 5;
        }
        break;
    case NL80211_BAND_5GHZ:
        if (chan >= 182 && chan <= 196)
        {
            ret = 4000 + chan * 5;
        }else{
            ret = 5000 + chan * 5;
        }
        break;
    case NL80211_BAND_60GHZ:
        if (chan < 5)
        {
            ret = 56160 + chan * 2160;
        }
        break;
    default:
        ;
    }
error:
    return ret; /* not supported */
}
/******************************************************************************
*    函数:    ieee80211_frequency_to_channel
*    功能:    无线频率转无线信道号
*    参数:
*            freq                        -    无线频率
*    返回:
*            > 0                            -    无线信道号
            -ERR_INVAL                    -     参数无效
*    说明:
 ******************************************************************************/
int ieee80211_frequency_to_channel(int freq)
{
    int ret;
    /* see 802.11-2007 17.3.8.3.2 and Annex J */
    if (freq == 2484)
        ret = 14;
    else if (freq < 2484)
        ret = (freq - 2407) / 5;
    else if (freq >= 4910 && freq <= 4980)
        ret = (freq - 4000) / 5;
    else if (freq <= 45000) /* DMG band lower limit */
        ret = (freq - 5000) / 5;
    else if (freq >= 58320 && freq <= 64800)
        ret = (freq - 56160) / 2160;
    else
        ret = -ERR_INVAL;
    return ret;
}
/******************************************************************************
*    函数:    error_handler
*    功能:    错误处理函数
*    参数:
*            nla                            -   netlink套接字
*            err                            -    错误返回值
*            arg                            -    参数
*    返回:
*            2                            -    成功

*    说明:
 ******************************************************************************/
static int error_handler(struct sockaddr_nl *nla, struct nlmsgerr *err, void *arg)
{
    int *ret = arg;
    *ret = err->error;
    return NL_STOP;
}
/******************************************************************************
*    函数:    finish_handler
*    功能:    结束处理函数
*    参数:
*            msg                            -   netlink消息结构体
*            arg                            -    参数
*    返回:
*            1                            -    成功

*    说明:
 ******************************************************************************/
static int finish_handler(struct nl_msg *msg, void *arg)
{
    int *ret = arg;
    *ret = 0;
    return NL_SKIP;
}
/******************************************************************************
*    函数:    ack_handler
*    功能:    应答处理函数
*    参数:
*            msg                            -   netlink消息结构体
*            arg                            -    参数
*    返回:
*            2                            -    成功

*    说明:
 ******************************************************************************/
static int ack_handler(struct nl_msg *msg, void *arg)
{
    int *ret = arg;
    *ret = 0;
    return NL_STOP;
}
/******************************************************************************
*    函数:    register_handler
*    功能:    注册处理函数
*    参数:
*            handler                        -   函数指针
*            data                        -    参数
*    返回:
*            无

*    说明:
 ******************************************************************************/
void register_handler(int (*handler)(struct nl_msg *, void *), void *data)
{
    registered_handler = handler;
    registered_handler_data = data;
}
/******************************************************************************
*    函数:    valid_handler
*    功能:    校验处理函数
*    参数:
*            msg                            -   netlink消息结构体
*            arg                            -    参数
*    返回:
*            0                            -    成功

*    说明:
 ******************************************************************************/
int valid_handler(struct nl_msg *msg, void *arg)
{
    if (registered_handler)
        return registered_handler(msg, registered_handler_data);

    return NL_OK;
}
/******************************************************************************
*    函数:    nl80211_init
*    功能:    netlink初始化
*    参数:
*            state                        -   netlink状态套接字
*    返回:
*            0                            -    成功
*            -ERR_NOFILE                    -    无设备文件
*            -ERR_NOMEM                    -    内存分配失败
*            -ERR_LINK                    -    建立连接失败

*    说明:
 ******************************************************************************/
int nl80211_init(struct nl80211_state *state)
{
    int ret = -1;

    state->nlsock = nl_socket_alloc();
    if (!state->nlsock)
    {
        fprintf(stderr, "Failed to allocate netlink socket.\n");
        ret = -ERR_NOMEM;
        goto error;
    }

    ret = nl_socket_set_buffer_size(state->nlsock, 8192, 8192);
//    if(ret < 0)
//    {
//        fprintf(stderr, "Failed to set buffer size.\n");
//        ret = -ERR_CFG;
//        goto error;
//    }

    ret = genl_connect(state->nlsock);
    if(ret < 0)
    {
        fprintf(stderr, "Failed to connect to generic netlink.\n");
        ret = -ERR_DISCONNECT;
        goto out_handle_destroy;
    }

    ret = genl_ctrl_resolve(state->nlsock, "nl80211");
    if (ret < 0)
    {
        fprintf(stderr, "nl80211 not found.\n");
        ret = -ERR_NOFILE;
        goto out_handle_destroy;
    }else{
        state->nl80211_id = ret;
    }
    ret = 0;
    return ret;
out_handle_destroy:
    nl_socket_free(state->nlsock);
error:
    return ret;
}
/******************************************************************************
*    函数:    nl80211_cleanup
*    功能:    关闭netlink接口函数
*    参数:
*            state                        -   netlink状态套接字
*    返回:
*            无

*    说明:
 ******************************************************************************/
void nl80211_cleanup(struct nl80211_state *state)
{
    nl_socket_free(state->nlsock);
}
/******************************************************************************
*    函数:    nl80211_set_interface_monitor
*    功能:    通过netlink接口配置无线网卡工作模式为monitor模式
*    参数:
*            无
*    返回:
*            0                            -    成功
*            -ERR_NOMEM                    -    内存分配失败
*            -ERR_SYS                    -    系统错误

*    说明:
 ******************************************************************************/
int nl80211_set_interface_monitor(void)
{
    struct nl_cb *cb;
    struct nl_cb *s_cb;
    struct nl_msg *msg;
    int ret = 0x00;

    msg = nlmsg_alloc();
    if (!msg)
    {
        fprintf(stderr, "failed to allocate netlink message\n");
        ret = -ERR_NOMEM;
        goto error;
    }

    cb = nl_cb_alloc(iw_debug ? NL_CB_DEBUG : NL_CB_DEFAULT);
    s_cb = nl_cb_alloc(iw_debug ? NL_CB_DEBUG : NL_CB_DEFAULT);
    if (!cb || !s_cb)
    {
        fprintf(stderr, "failed to allocate netlink callbacks\n");
        ret = -ERR_NOMEM;
        goto out;
    }

    genlmsg_put(msg, 0, 0, wireless_info.nl_state.nl80211_id, 0,
            0, NL80211_CMD_SET_INTERFACE, 0);

    NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, wireless_info.dev_ifidx);

    /* set wlan0 to monitor type */
    NLA_PUT_U32(msg, NL80211_ATTR_IFTYPE, NL80211_IFTYPE_MONITOR);


    nl_socket_set_cb(wireless_info.nl_state.nlsock, s_cb);
    ret = nl_send_auto_complete(wireless_info.nl_state.nlsock, msg);
    if (ret < 0)
    {
        ret = -ERR_SYS;
        goto out;
    }

    ret = 1;
    nl_cb_err(cb, NL_CB_CUSTOM, error_handler, &ret);
    nl_cb_set(cb, NL_CB_FINISH, NL_CB_CUSTOM, finish_handler, &ret);
    nl_cb_set(cb, NL_CB_ACK, NL_CB_CUSTOM, ack_handler, &ret);
    nl_cb_set(cb, NL_CB_VALID, NL_CB_CUSTOM, valid_handler, NULL);

    while (ret > 0)
    {
        nl_recvmsgs(wireless_info.nl_state.nlsock, cb);
    }
    ret = 0x00;
out:
    nl_cb_put(cb);
    nl_cb_put(s_cb);
nla_put_failure:
    nlmsg_free(msg);
error:
    return ret;
}
/******************************************************************************
*    函数:    nl80211_set_wiphy_channel
*    功能:    通过netlink设置无线信道
*    参数:
*            ch                            -   信道号
*    返回:
*            0                            -    成功
*            -ERR_NOMEM                    -    内存错误
*            -ERR_SYS                    -    系统错误
*    说明:
 ******************************************************************************/
int nl80211_set_wiphy_channel(int ch)
{
    struct nl_cb *cb;
    struct nl_cb *s_cb;
    struct nl_msg *msg;
//    signed long long devidx = 0;
    int ret = -1;
//    enum command_identify_by command_idby = CIB_NONE;
    unsigned int freq;

    /* set wlan0 to monitor type */
    msg = nlmsg_alloc();
    if (!msg)
    {
        fprintf(stderr, "failed to allocate netlink message\n");
        ret = -ERR_NOMEM;
        goto error;
    }

    cb = nl_cb_alloc(iw_debug ? NL_CB_DEBUG : NL_CB_DEFAULT);
    s_cb = nl_cb_alloc(iw_debug ? NL_CB_DEBUG : NL_CB_DEFAULT);
    if (!cb || !s_cb)
    {
        fprintf(stderr, "failed to allocate netlink callbacks\n");
        ret = -ERR_NOMEM;
        goto out;
    }

    genlmsg_put(msg, 0, 0, wireless_info.nl_state.nl80211_id, 0,
            0, NL80211_CMD_SET_WIPHY, 0);

    NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, wireless_info.dev_ifidx);

    enum nl80211_band band;
    band = ((ch <= 14) ? NL80211_BAND_2GHZ : NL80211_BAND_5GHZ);
    freq = ieee80211_channel_to_frequency(ch, band);

    NLA_PUT_U32(msg, NL80211_ATTR_WIPHY_FREQ, freq);

    nl_socket_set_cb(wireless_info.nl_state.nlsock, s_cb);

    ret = nl_send_auto_complete(wireless_info.nl_state.nlsock, msg);
    if (ret < 0)
    {
        ret = -ERR_SYS;
        goto out;
    }
    ret = 1;

    nl_cb_err(cb, NL_CB_CUSTOM, error_handler, &ret);
    nl_cb_set(cb, NL_CB_FINISH, NL_CB_CUSTOM, finish_handler, &ret);
    nl_cb_set(cb, NL_CB_ACK, NL_CB_CUSTOM, ack_handler, &ret);
    nl_cb_set(cb, NL_CB_VALID, NL_CB_CUSTOM, valid_handler, NULL);

    while (ret > 0)
        nl_recvmsgs(wireless_info.nl_state.nlsock, cb);

    ret = 0;
out:
    nl_cb_put(cb);
    nl_cb_put(s_cb);
    nlmsg_free(msg);
    return ret;
nla_put_failure:
    fprintf(stderr, "building message failed\n");
error:
    return ret;
}
/******************************************************************************
*    函数:    nl80211_set_wiphy_txpower
*    功能:    设置无线发送功率
*    参数:
*            mbm                            -   无线发送功率(dbm为最小单位)
*    返回:
*            0                            -    成功
*            -ERR_NOMEM                    -    内存错误
*            -ERR_SYS                    -    系统错误
*    说明:
 ******************************************************************************/
int nl80211_set_wiphy_txpower(int mbm)
{
    struct nl_cb *cb;
    struct nl_cb *s_cb;
    struct nl_msg *msg;
    int ret;

    msg = nlmsg_alloc();
    if (!msg)
    {
        fprintf(stderr, "failed to allocate netlink message\n");
        ret = -ERR_NOMEM;
        goto error;
    }

    cb = nl_cb_alloc(iw_debug ? NL_CB_DEBUG : NL_CB_DEFAULT);
    s_cb = nl_cb_alloc(iw_debug ? NL_CB_DEBUG : NL_CB_DEFAULT);
    if (!cb || !s_cb)
    {
        fprintf(stderr, "failed to allocate netlink callbacks\n");
        ret = -ERR_NOMEM;
        goto out;
    }

    genlmsg_put(msg, 0, 0, wireless_info.nl_state.nl80211_id, 0,
            0, NL80211_CMD_SET_WIPHY, 0);

    NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, wireless_info.dev_ifidx);

    NLA_PUT_U32(msg, NL80211_ATTR_WIPHY_TX_POWER_SETTING, NL80211_TX_POWER_FIXED);
    NLA_PUT_U32(msg, NL80211_ATTR_WIPHY_TX_POWER_LEVEL, mbm);

    nl_socket_set_cb(wireless_info.nl_state.nlsock, s_cb);

    ret = nl_send_auto_complete(wireless_info.nl_state.nlsock, msg);
    if (ret < 0)
    {
        ret = -ERR_SYS;
        goto out;
    }
    ret = 1;

    nl_cb_err(cb, NL_CB_CUSTOM, error_handler, &ret);
    nl_cb_set(cb, NL_CB_FINISH, NL_CB_CUSTOM, finish_handler, &ret);
    nl_cb_set(cb, NL_CB_ACK, NL_CB_CUSTOM, ack_handler, &ret);
    nl_cb_set(cb, NL_CB_VALID, NL_CB_CUSTOM, valid_handler, NULL);

    while (ret > 0)
        nl_recvmsgs(wireless_info.nl_state.nlsock, cb);

    ret = 0;
out:
    nl_cb_put(cb);
    nl_cb_put(s_cb);
    nlmsg_free(msg);
    return ret;
nla_put_failure:
    fprintf(stderr, "building message failed\n");
error:
    return ret;
}
/******************************************************************************
*    函数:    wireless_mac_header_len
*    功能:    无线mac头长度
*    参数:
*            无
*    返回:
*            > 0                            -    无线mac头长度

*    说明:
 ******************************************************************************/
//int wireless_mac_header_len(void)
//{
//    return  IEEE80211_HDRLEN + sizeof(BeaconFixedElement);
//}
/******************************************************************************
*    函数:    wireless_set_interface_flag
*    功能:    开启或者关闭无线网络
*    参数:
*            ifname                        -   网络接口名
*            flag                        -    开启或者关闭标志(0:关闭 1:开启)
*    返回:
*            0                            -    成功

*    说明:
 ******************************************************************************/
int wireless_set_interface_flag(char *ifname, short flag)
{
    int ret = -1;
    struct ifreq ifr;
    int fd;

    /* Create a channel to the NET kernel. */
    ret = socket(AF_INET,SOCK_DGRAM ,0);
    if (ret < 0) {
        perror("socket");
        ret = -ERR_SYS;
        goto error;
    }else{
        fd = ret;
    }

    strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
    ret = ioctl(fd, SIOCGIFFLAGS, &ifr);
    if (ret < 0)
    {
        fprintf(stderr, "%s: unknown interface: %s\n", ifname, strerror(errno));
        ret = -ERR_SYS;
        goto error;
    }

    strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
    if(flag == 0)
        ifr.ifr_flags &= ~IFF_UP;
    else if(flag == 1)
        ifr.ifr_flags |= IFF_UP;

    ret = ioctl(fd, SIOCSIFFLAGS, &ifr);
    if(ret < 0) {
        perror("SIOCSIFFLAGS");
        ret = -ERR_SYS;
        goto error;
    }
    ret = 0;
error:
    return ret;
}

/******************************************************************************
*    函数:    wireless_add_monitor_filter
*    功能:    开启网络数据包过滤
*    参数:
*            s                            -   socket套接字
*    返回:
*            0                            -    成功
*            -ERR_CFG                    -    配置失败
*    说明:
 ******************************************************************************/
static int wireless_add_monitor_filter(int s)
{
    int ret = -1;
    int idx;

    /* rewrite all PASS/FAIL jump offsets */
    for (idx = 0; idx < msock_filter.len; idx++)
    {
        struct sock_filter *insn = &msock_filter_insns[idx];

        if (BPF_CLASS(insn->code) == BPF_JMP)
        {
            if (insn->code == (BPF_JMP|BPF_JA))
            {
                if (insn->k == PASS)
                    insn->k = msock_filter.len - idx - 2;
                else if (insn->k == FAIL)
                    insn->k = msock_filter.len - idx - 3;
            }

            if (insn->jt == PASS)
                insn->jt = msock_filter.len - idx - 2;
            else if (insn->jt == FAIL)
                insn->jt = msock_filter.len - idx - 3;

            if (insn->jf == PASS)
                insn->jf = msock_filter.len - idx - 2;
            else if (insn->jf == FAIL)
                insn->jf = msock_filter.len - idx - 3;
        }
    }

    ret = setsockopt(s, SOL_SOCKET, SO_ATTACH_FILTER, &msock_filter, sizeof(msock_filter));
    if(ret != 0)
    {
        printf("nl80211: setsockopt(SO_ATTACH_FILTER) failed: %s\r\n", strerror(errno));
        ret = -ERR_CFG;
        goto error;
    }
    ret = 0;
error:
    return ret;
}
/******************************************************************************
*    函数:    wireless_remove_monitor_socket
*    功能:    关闭socket套接字
*    参数:
*            无
*    返回:
*            无
*    说明:
 ******************************************************************************/
int wireless_remove_monitor_socket(void)
{
    int    ret = 0x00;

//    if(wireless_info.count == 0x00)
//    {
//        ret = -ERR_NOINIT;
//        goto error;
//    }
//    wireless_info.count = 0;

    if (wireless_info.monitor_refcount > 0)
    {
        wireless_info.monitor_refcount--;
    }

    if (wireless_info.dev_ifidx >= 0)
    {
        wireless_info.dev_ifidx = -1;
    }

    if (wireless_info.fd >= 0)
    {
        close(wireless_info.fd);
        wireless_info.fd = -1;
    }

    ret = 0x00;

    return ret;
}
/******************************************************************************
*    函数:    wireless_create_monitor_socket
*    功能:    打开无线网卡套接字
*    参数:
*            无
*    返回:
*            > 0                            -    成功,返回socket套接字
*            < 0                            -    失败

*    说明:
 ******************************************************************************/
int wireless_create_monitor_socket(void)
{
    int ret = -1;
    struct sockaddr_ll ll;
    int optval;
    socklen_t optlen;

    /* create monitor socket */
    memset(&ll, 0, sizeof(ll));
    ll.sll_family = AF_PACKET;
    ll.sll_ifindex = wireless_info.dev_ifidx;
    ret = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (ret < 0)
    {
        printf("nl80211: socket[PF_PACKET,SOCK_RAW] failed: %s\r\n", strerror(errno));
        goto error;
    }else{
        wireless_info.fd = ret;
    }

    ret = wireless_add_monitor_filter(wireless_info.fd);
    if (ret < 0)
    {
        printf("Failed to set socket filter for monitor interface; do filtering in user space\r\n");
        /* This works, but will cost in performance. */
    }

    ret = bind(wireless_info.fd,(struct sockaddr *) &ll, sizeof(ll));
    if (ret < 0)
    {
        printf("nl80211: monitor socket bind failed: %s\r\n", strerror(errno));
        ret = -ERR_SYS;
        goto error;
    }

    optlen = sizeof(optval);
    optval = 20;
    ret = setsockopt(wireless_info.fd, SOL_SOCKET, SO_PRIORITY, &optval, optlen);
    if (ret != 0)
    {
        printf("nl80211: Failed to set socket priority: %s\r\n", strerror(errno));
        ret = -ERR_CFG;
        goto error;
    }
    wireless_info.monitor_refcount++;

    ret = wireless_info.fd;
    return ret;
error:
    wireless_remove_monitor_socket();
    return ret;
}
/******************************************************************************
*    函数:    nl80211_send_monitor
*    功能:    monitor模式下数据发送函数
*    参数:
*            data                        -   数据指针
*            len                            -    数据长度
*            encrypt                        -    是否加密(1:加密 0：不加密)
*            noack                        -    是否应答
*    返回:
*            0                            -    成功
*            -ERR_NOINIT                    -    未初始化
*            -ERR_SYS                    -    系统错误
*    说明:
 ******************************************************************************/
int nl80211_send_monitor(const void *data, size_t len, int encrypt, int noack)
{
    int ret = -1;
    uint16_t txflags = 0;
    uint8_t rtap_hdr[] =
    {
        0x00, 0x00, /* radiotap version */
        0x0e, 0x00, /* radiotap length */
        0x02, 0xc0, 0x00, 0x00, /* bmap: flags, tx and rx flags */
        IEEE80211_RADIOTAP_F_FRAG, /* F_FRAG (fragment if required) */
        0x00,       /* padding */
        0x00, 0x00, /* RX and TX flags to indicate that */
        0x00, 0x00, /* this is the injected frame directly */
    };

    struct iovec iov[2] =
    {
        {
            .iov_base = &rtap_hdr,
            .iov_len = sizeof(rtap_hdr),
        },
        {
            .iov_base = (void *) data,
            .iov_len = len,
        }
    };
    struct msghdr msg =
    {
        .msg_name = NULL,
        .msg_namelen = 0,
        .msg_iov = iov,
        .msg_iovlen = 2,
        .msg_control = NULL,
        .msg_controllen = 0,
        .msg_flags = 0,
    };

    if (encrypt)
        rtap_hdr[8] |= IEEE80211_RADIOTAP_F_WEP;

    if (wireless_info.count == 0)
    {
        printf("nl80211: No monitor socket available \r\n");
        ret = -ERR_NOINIT;
        goto error;
    }

    if (noack)
        txflags |= IEEE80211_RADIOTAP_F_TX_NOACK;
    WPA_PUT_LE16(&rtap_hdr[12], txflags);

    ret = sendmsg(wireless_info.fd, &msg, 0);
    if (ret < 0)
    {
        printf("nl80211: sendmsg: %s\r\n", strerror(errno));
        ret = -ERR_SYS;
        goto error;
    }
    ret = 0;
error:
    return ret;
}
/******************************************************************************
*    函数:    wireless_config
*    功能:    配置无线模块
*    参数:
*            cfg                        -    无线模块配置参数
*    返回:
            0                        -    成功
             -ERR_SYS                -    系统错误
            -ERR_BUSY                -     模块已初始化
*    说明:    此函数由framework_init调用。
 ******************************************************************************/
int wireless_config(wireless_config_st *cfg)
{
    int ret = -1;

//    ret = wireless_get_macaddr(wireless_info.mac);
//    if(ret < 0)
//    {
//        goto error;
//    }
    //ifconfig wlan0 down
    ret = wireless_set_interface_flag(cfg->ifname, 0);
    if(ret < 0)
    {
        goto error;
    }

    /* 1. set wlan0 to monitor type */
    ret = nl80211_set_interface_monitor();
    if(ret < 0)
    {
        goto error;
    }

    /* 2. ifconfig wlan0 up */
    ret = wireless_set_interface_flag(cfg->ifname, 1);
    if(ret < 0)
    {
        goto error;
    }

    /* 3. set channel */
    ret = nl80211_set_wiphy_channel(cfg->channel);
    if(ret < 0)
    {
        goto error;
    }

    /* 4. set txpower 3000 */
    ret = nl80211_set_wiphy_txpower(cfg->txpower);
    if(ret < 0)
    {
        goto error;
    }

    ret = 0;
error:
    return ret;
}
/******************************************************************************
*    函数:    wireless_init
*    功能:    无线模块初始化
*    参数:
*            cfg                        -     无线模块配置参数
*    返回:
            > 0                        -    成功, 返回socket套接字
             -ERR_SYS                -    系统错误
            -ERR_BUSY                -     模块已初始化
*    说明:    此函数由framework_init调用。
 ******************************************************************************/
int wireless_init(wireless_config_st *cfg)
{
    int ret= -1;
    char buf[IFNAMSIZ];
    memcpy(&wireless_info.wireless_config, cfg, sizeof(wireless_config_st));

    snprintf(buf, IFNAMSIZ, "%s", wireless_info.wireless_config.ifname);
    buf[IFNAMSIZ - 1] = '\0';
    wireless_info.dev_ifidx = if_nametoindex(buf);

    ret = nl80211_init(&wireless_info.nl_state);
    if(ret < 0)
    {
        printf("nl80211_init error\n");
        goto error;
    }

    ret = wireless_config(&wireless_info.wireless_config);
    if(ret < 0)
    {
        printf("wireless_config error\n");
        goto error;
    }

    /* creater monitor socket to receive frame */
    ret = wireless_create_monitor_socket();
    if(ret < 0)
    {
        printf("wireless_create_monitor_socket error\n");
        goto error;
    }

    wireless_info.count = 1;
error:
    return ret;
}
/******************************************************************************
*    函数:    wireless_send
*    功能:    无线模块发送数据
*    参数:
*            buf                        -    数据缓冲区
*            len                        -    数据长度
*    返回:
*            0                        -    成功
*             -ERR_SYS                -    系统错误
*            -ERR_BUSY                -     模块已初始化
*    说明:
 ******************************************************************************/
int wireless_send(uint8_t *buf, int32_t len)
{
    int ret = 0x00;
    if (buf == NULL)
    {
        ret = -ERR_INVAL;
        goto error;
    }

    if(wireless_info.count == 0)
    {
        ret = -ERR_NOINIT;        //没有打开
        goto error;
    }

    ret = nl80211_send_monitor(buf, len, 1, 0);
    if(ret < 0)
    {
        goto error;
    }

    ret = 0x00;
error:
    return ret;
}
/******************************************************************************
*    函数:    wireless_recv
*    功能:    无线模块接收数据
*    参数:
*            buf                        -    数据缓冲区
*            max_len                    -    接收数据最大长度
*            len                        -    数据长度
*    返回:
*            0                        -    成功
*             -ERR_SYS                -    系统错误
*            -ERR_BUSY                -     模块已初始化
*    说明:
 ******************************************************************************/
int wireless_recv(uint8_t *buf, uint16_t max_len, uint16_t *len)
{
    int ret = -1;

    if((buf == NULL) || (len == NULL))
    {
        ret = -ERR_NOMEM;
        printf("[%s %d] ret:%d\n", __FUNCTION__, __LINE__, ret);
        goto error;
    }
    if(wireless_info.count == 0)
    {
        ret = -ERR_NOINIT;        //没有打开
        printf("[%s %d] ret:%d\n", __FUNCTION__, __LINE__, ret);
        goto error;
    }
    ret = recv(wireless_info.fd, buf, max_len, 0);
    if(ret == 0)
    {
        ret = -ERR_TIMEOUT;
        printf("[%s %d] recv ret:%d\n", __FUNCTION__, __LINE__, ret);
        goto error;
    }
    if(ret < 0)
    {
        ret = -ERR_SYS;
        printf("[%s %d] recv ret:%d\n", __FUNCTION__, __LINE__, ret);
        //goto error;
        //信道变更时会返回一帧错误数据
    }
    *len = ret;
    ret = 0x00;
error:
    return ret;
}
/******************************************************************************
*    函数:    wireless_close
*    功能:    关闭无线网卡模块
*    参数:
*            无
*    返回:
*            0                        -    成功
*             -ERR_NOINIT                -    模块未初始化
*    说明:    无
 ******************************************************************************/
int wireless_close(void)
{
    int ret = 0x00;
    if(wireless_info.count == 0x00)
    {
        ret = -ERR_NOINIT;        //没有打开
        goto error;
    }

    ret = wireless_remove_monitor_socket();
    if(wireless_info.count == 0x00)
    {
        ret = -ERR_NOINIT;        //没有打开
        goto error;
    }

    wireless_info.count = 0;
    wireless_info.dev_ifidx = -1;
    memset(&wireless_info.wireless_config, 0x00, sizeof(wireless_config_st));
    nl80211_cleanup(&wireless_info.nl_state);
error:
    return ret;
}
