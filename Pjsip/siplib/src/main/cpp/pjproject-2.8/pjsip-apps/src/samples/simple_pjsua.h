//
// Created by liuzl15 on 2019/12/5.
//

#ifndef CSIPSDK_SIMPLE_PJSUA_H
#define CSIPSDK_SIMPLE_PJSUA_H

#ifdef __cplusplus
extern "C" {
#endif

#include <pjsua-lib/pjsua.h>
#include <pjsua-lib/pjsua_internal.h>

void on_incoming_call(pjsua_acc_id acc_id, pjsua_call_id call_id,
                      pjsip_rx_data *rdata);
void on_call_state(pjsua_call_id call_id, pjsip_event *e);

void on_call_media_state(pjsua_call_id call_id);

void error_exit(const char *title, pj_status_t status);

int _test_reg_init();

int _register(char* user_name,char* user_code,char* password,char* domain);
int __register(char* user_name,char* user_code,char* password);

int make_call(char* user_code);

int test_answer(unsigned code);

int test_call_destroy();

int i_hangup_call();

int o_hangup_call();

int test_i_send_dtmf(char* buf);

int test_o_send_dtmf(char* buf);

#ifdef __cplusplus
}
#endif

#endif //CSIPSDK_SIMPLE_PJSUA_H
