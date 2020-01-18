/* $Id: simple_pjsua.c 3553 2011-05-05 06:14:19Z nanang $ */
/* 
 * Copyright (C) 2008-2011 Teluu Inc. (http://www.teluu.com)
 * Copyright (C) 2003-2008 Benny Prijono <benny@prijono.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */

/**
 * simple_pjsua.c
 *
 * This is a very simple but fully featured SIP user agent, with the 
 * following capabilities:
 *  - SIP registration
 *  - Making and receiving call
 *  - Audio/media to sound device.
 *
 * Usage:
 *  - To make outgoing call, start simple_pjsua with the URL of remote
 *    destination to contact.
 *    E.g.:
 *	 simpleua sip:user@remote
 *
 *  - Incoming calls will automatically be answered with 200.
 *
 * This program will quit once it has completed a single call.
 */


#include "simple_pjsua.h"

#define THIS_FILE    "APP"

#define SIP_DOMAIN    "112.74.178.87"

pjsua_call_id i_call_id = PJSUA_INVALID_ID;
pjsua_call_id o_call_id = PJSUA_INVALID_ID;
pjsua_call_id call_id = PJSUA_INVALID_ID;
int ring_slot = PJSUA_INVALID_ID;
pjmedia_port *ring_port;
pj_pool_t *pool_ring=NULL;

int ringback_slot =-1;
pjmedia_port *ringback_port = NULL;
pj_pool_t *pool_ringback = NULL;

static void on_reg_state(pjsua_acc_id acc_id,pjsua_reg_info *info)
{
    PJ_UNUSED_ARG(acc_id);
    ALOGE("lzl %d\n",acc_id);

    if(pjsua_var.test_call_cb) {
        pjsua_var.test_call_cb(PJ_TRUE, 1, "test message");
    }
    // Log already written.
}


/* Callback called by the library upon receiving incoming call */
void on_incoming_call(pjsua_acc_id acc_id, pjsua_call_id call_id,
                      pjsip_rx_data *rdata) {
    pjsua_call_info ci;

    PJ_UNUSED_ARG(acc_id);
    PJ_UNUSED_ARG(rdata);

    pjsua_call_get_info(call_id, &ci);
    i_call_id = call_id;
    ALOGE("%s Incoming call from %.*s!!", THIS_FILE,
          (int) ci.remote_info.slen,
          ci.remote_info.ptr);
    if(pjsua_var.test_call_cb) {
        pjsua_var.test_call_cb(PJ_TRUE, call_id, ci.remote_info.ptr);
    }

    /* Automatically answer incoming calls with 200/OK */
//    pjsua_call_answer(call_id, 200, NULL, NULL);
    play_ring();
}

/* Callback called by the library when call's state has changed */
void on_call_state(pjsua_call_id call_id, pjsip_event *e) {
    pjsua_call_info ci;

    PJ_UNUSED_ARG(e);
    o_call_id = call_id;
    pjsua_call_get_info(call_id, &ci);
    ALOGE("%s Call %d state=%.*s,last state code=%d", THIS_FILE, call_id,
          (int) ci.state_text.slen,
          ci.state_text.ptr,ci.last_status);
    if(pjsua_var.test_call_cb) {
        pjsua_var.test_call_cb(PJ_TRUE, ci.last_status, ci.remote_info.ptr);
    }
}

/* Callback called by the library when call's media state has changed */
void on_call_media_state(pjsua_call_id call_id) {
    pjsua_call_info ci;

    pjsua_call_get_info(call_id, &ci);

    if (ci.media_status == PJSUA_CALL_MEDIA_ACTIVE) {
        // When media is active, connect call to sound device.
        pjsua_conf_connect(ci.conf_slot, 0);
        pjsua_conf_connect(0, ci.conf_slot);
    }
}

/* Display error and exit application */
void error_exit(const char *title, pj_status_t status) {
    ALOGE("%s %s %d", THIS_FILE, title, status);
    pjsua_destroy();
    return;
}

int init_ringback(void) {

    /* Ringtones  */
#define RINGBACK_FREQ1        440        /* 400 */
#define RINGBACK_FREQ2        480        /* 450 */
#define RINGBACK_ON            3000    /* 400 */
#define RINGBACK_OFF        4000    /* 200 */
#define RINGBACK_CNT        1        /* 2   */
#define RINGBACK_INTERVAL        4000    /* 2000 */

    unsigned i, samples_per_frame;
    pjmedia_tone_desc tone[RINGBACK_CNT];


    pj_str_t name;
    pj_status_t status;

    pool_ringback = pjsua_pool_create("ringback", 512, 512);
    samples_per_frame = pjsua_var.media_cfg.audio_frame_ptime *
                        pjsua_var.media_cfg.clock_rate *
                        pjsua_var.media_cfg.channel_count / 1000;

    /* Ringback tone (call is ringing) */
    name = pj_str("ringback");
    status = pjmedia_tonegen_create2(pool_ringback, &name,
                                     pjsua_var.media_cfg.clock_rate,
                                     pjsua_var.media_cfg.channel_count,
                                     samples_per_frame,
                                     16, PJMEDIA_TONEGEN_LOOP,
                                     &ringback_port);
    if (status != PJ_SUCCESS) {
        ALOGE("%s pjmedia_tonegen_create2 failed", THIS_FILE);
        return -1;
    }

    pj_bzero(&tone, sizeof(tone));
    for (i = 0; i < RINGBACK_CNT; ++i) {
        tone[i].freq1 = RINGBACK_FREQ1;
        tone[i].freq2 = RINGBACK_FREQ2;
        tone[i].on_msec = RINGBACK_ON;
        tone[i].off_msec = RINGBACK_OFF;
    }
    tone[RINGBACK_CNT - 1].off_msec = RINGBACK_INTERVAL;

    status = pjmedia_tonegen_play(ringback_port, RINGBACK_CNT, tone,
                                  PJMEDIA_TONEGEN_LOOP);
    if (status != PJ_SUCCESS){
        ALOGE("%s pjmedia_tonegen_play failed", THIS_FILE);
        return -1;
    }

    status = pjsua_conf_add_port(pool_ringback, ringback_port, &ringback_slot);
    if (status != PJ_SUCCESS){
        ALOGE("%s pjsua_conf_add_port failed", THIS_FILE);
        return -1;
    }

    status = pjsua_conf_connect(ringback_slot, 0);

    return 0;
}

int play_ringback(){
    pj_status_t status;
    status = pjsua_conf_connect(ringback_slot, 0);
    if(status != PJ_SUCCESS){
        ALOGE("%s pjsua_conf_connect failed",THIS_FILE);
        return -1;
    }
    return 0;
}

int ringback_stop(){
    if(ringback_slot != -1) {
        pjsua_conf_disconnect(ringback_slot, 0);
        pjmedia_tonegen_rewind(ringback_port);
    }
    return 0;
}

void destroy_ringback(){
    if (ringback_slot != -1) {
        pjsua_conf_remove_port(ringback_slot);
        ringback_slot = -1;
    }
    if (ringback_port) {
        pjmedia_port_destroy(ringback_port);
        ringback_port = NULL;
    }
    if (pool_ringback) {
        pj_pool_release(pool_ringback);
        pool_ringback = NULL;
    }
}

void init_ring(){
    pjmedia_tone_desc tone[4];
    pj_str_t name;
    pool_ring = pjsua_pool_create("ringback", 512, 512);
    pj_status_t status;
    name = pj_str("ring");
    status = pjmedia_tonegen_create2(pool_ring,&name,
                                     8000,
                                     1,
                                     160,
                                     16, PJMEDIA_TONEGEN_LOOP,
                                     &ring_port);
    if (status != PJ_SUCCESS)
        return;

    tone[0].freq1 = 800;
    tone[0].freq2 = 20;
    tone[0].on_msec = 2000;
    tone[0].off_msec = 1000;

    pjmedia_tonegen_play(ring_port, 1,
                         tone, PJMEDIA_TONEGEN_LOOP);

    status = pjsua_conf_add_port(pool_ring, ring_port,&ring_slot);

}

int play_ring(){
    pj_status_t status;
    status = pjsua_conf_connect(ring_slot, 0);
    if(status != PJ_SUCCESS){
        ALOGE("%s pjsua_conf_connect failed",THIS_FILE);
        return -1;
    }
    return 0;
}

int ring_stop(){
    if(ring_slot != -1) {
        pjsua_conf_disconnect(ring_slot, 0);
        pjmedia_tonegen_rewind(ring_port);
    }
    return 0;
}


void destroy_ring() {
    if (ring_slot != -1) {
        pjsua_conf_remove_port(ring_slot);
        ring_slot = -1;
    }
    if (ring_port) {
        pjmedia_port_destroy(ring_port);
        ring_port = NULL;
    }

    if (pool_ring) {
        pj_pool_release(pool_ring);
        pool_ring = NULL;
    }
}


/*
 * main()
 *
 * argv[1] may contain URL to call.
 */
int _test_reg_init() {

    pj_status_t status;

    /* Create pjsua first! */
    status = pjsua_create();
    if (status != PJ_SUCCESS) error_exit("Error in pjsua_create()", status);

    /* If argument is specified, it's got to be a valid SIP URL */
//    if (argc > 1) {
//	status = pjsua_verify_url(argv[1]);
//	if (status != PJ_SUCCESS) error_exit("Invalid URL in argv", status);
//    }

    /* Init pjsua */
    {
        pjsua_config cfg;
        pjsua_logging_config log_cfg;

        pjsua_config_default(&cfg);
        cfg.cb.on_incoming_call = &on_incoming_call;
        cfg.cb.on_call_media_state = &on_call_media_state;
        cfg.cb.on_call_state = &on_call_state;
        cfg.cb.on_reg_state = &on_reg_state;

        pjsua_logging_config_default(&log_cfg);
        log_cfg.console_level = 4;

        status = pjsua_init(&cfg, &log_cfg, NULL);
        if (status != PJ_SUCCESS) error_exit("Error in pjsua_init()", status);
    }

    /* Add UDP transport. */
    {
        pjsua_transport_config cfg;

        pjsua_transport_config_default(&cfg);
        cfg.port = 5060;
        status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &cfg, NULL);
        if (status != PJ_SUCCESS) error_exit("Error creating transport", status);
    }

    /* Initialization is done, now start pjsua */
    status = pjsua_start();
    if (status != PJ_SUCCESS) error_exit("Error starting pjsua", status);
    return 0;
}

int __register(char* user_name,char* user_code,char* password){

    pj_status_t status;

    /* Register to SIP server by creating SIP account. */
    {
        pjsua_acc_config cfg;

        pjsua_acc_config_default(&cfg);

        char tmp[80];
        snprintf(tmp, sizeof(tmp),"\"%s\" <sip:%s@%s>",user_name,user_code,SIP_DOMAIN);
        cfg.id = pj_str(tmp);
        cfg.reg_uri = pj_str("sip:" SIP_DOMAIN "5060");
        cfg.cred_count = 1;
        cfg.cred_info[0].realm = pj_str(SIP_DOMAIN);
        cfg.cred_info[0].scheme = pj_str("digest");
        cfg.cred_info[0].username = pj_str(user_code);
        cfg.cred_info[0].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
        cfg.cred_info[0].data = pj_str(password);
        pj_str_t input_str = pj_str("sip:" SIP_DOMAIN "5060");
        cfg.proxy_cnt =0;
        cfg.proxy[cfg.proxy_cnt].ptr = input_str.ptr;
        cfg.proxy[cfg.proxy_cnt++].slen = input_str.slen;
        cfg.use_timer = PJSUA_SIP_TIMER_INACTIVE;
        cfg.rtp_cfg.qos_type = PJ_QOS_TYPE_BEST_EFFORT;

        status = pjsua_acc_add(&cfg, PJ_TRUE, &call_id);
        if (status != PJ_SUCCESS) error_exit("Error adding account", status);
		init_ring();
    }
}

int _register(char* user_name,char* user_code,char* password,char* domain){

    pj_status_t status;

    /* Register to SIP server by creating SIP account. */
    {
        pjsua_acc_config cfg;

        pjsua_acc_config_default(&cfg);

        char tmp[40];
        snprintf(tmp, sizeof(tmp),"\"%s\" <sip:%s@%s>",user_name,user_code,domain);
        cfg.id = pj_str(tmp);
        char tmp1[40];
        snprintf(tmp1, sizeof(tmp1),"sip:%s:5060",domain);
        cfg.reg_uri = pj_str(tmp1);
        cfg.cred_count = 1;
        cfg.cred_info[0].realm = pj_str(domain);
        cfg.cred_info[0].scheme = pj_str("digest");
        cfg.cred_info[0].username = pj_str(user_code);
        cfg.cred_info[0].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
        cfg.cred_info[0].data = pj_str(password);
        pj_str_t input_str = pj_str(tmp1);
        cfg.proxy_cnt =0;
        cfg.proxy[cfg.proxy_cnt].ptr = input_str.ptr;
        cfg.proxy[cfg.proxy_cnt++].slen = input_str.slen;
        cfg.use_timer = PJSUA_SIP_TIMER_INACTIVE;
        cfg.rtp_cfg.qos_type = PJ_QOS_TYPE_BEST_EFFORT;

        status = pjsua_acc_add(&cfg, PJ_TRUE, &call_id);
        if (status != PJ_SUCCESS) error_exit("Error adding account", status);
        init_ring();
    }
}

int make_call(char* user_code){
    /* If argument is specified, it's got to be a valid SIP URL */
    pj_status_t status;
    char tmp[80];
    snprintf(tmp, sizeof(tmp),"sip:%s@%s",user_code,SIP_DOMAIN);
    status = pjsua_verify_url(tmp);
    if (status != PJ_SUCCESS)
        error_exit("Invalid URL in argv", status);

    /* If URL is specified, make call to the URL. */
    pj_str_t uri = pj_str(tmp);
    status = pjsua_call_make_call(call_id, &uri, 0, NULL, NULL, NULL);
    if (status != PJ_SUCCESS) error_exit("Error making call", status);
    return status;
}

int i_hangup_call(){
    ring_stop();
    return pjsua_call_hangup(i_call_id, 0, NULL, NULL);
}

int o_hangup_call(){
    return pjsua_call_hangup(o_call_id, 0, NULL, NULL);
}

int test_answer(unsigned code){
    ring_stop();
    return pjsua_call_answer(i_call_id, code, NULL, NULL);

}


int test_i_send_dtmf(char* buf){
    pjsua_call_send_dtmf_param dtmf_param;
    pjsua_call_send_dtmf_param_default(&dtmf_param);
    dtmf_param.digits = pj_str(buf);
    dtmf_param.method = PJSUA_DTMF_METHOD_SIP_INFO;
    return pjsua_call_send_dtmf(o_call_id,&dtmf_param);

}

int test_o_send_dtmf(char* buf){
    pjsua_call_send_dtmf_param dtmf_param;
    pjsua_call_send_dtmf_param_default(&dtmf_param);
    dtmf_param.digits = pj_str(buf);
    dtmf_param.method = PJSUA_DTMF_METHOD_SIP_INFO;
    return pjsua_call_send_dtmf(o_call_id,&dtmf_param);

}

int test_call_destroy(){
    destroy_ring();
//    destroy_ringback();
    return pjsua_destroy();
}
