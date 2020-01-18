/* $Id: systest.h 3553 2011-05-05 06:14:19Z nanang $ */
/* 
 * Copyright (C) 2008-2011 Teluu Inc. (http://www.teluu.com)
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
#ifndef __SYSTEST_H__
#define __SYSTEST_H__

#include <pjlib.h>
#include "gui.h"
#include <pjsua-lib/pjsua.h>
#include <pjsua-lib/pjsua_internal.h>
#include <pjmedia_audiodev.h>


//#ifdef __cplusplus
//extern "C" {
//#endif

/*
 * Overrideable parameters
 */
#define REC_DEV_ID            systest_cap_dev_id
#define PLAY_DEV_ID            systest_play_dev_id
//#define REC_DEV_ID			5
//#define PLAY_DEV_ID			5
#define OVERRIDE_AUDDEV_REC_LAT        0
#define OVERRIDE_AUDDEV_PLAY_LAT    0
#define OVERRIDE_AUD_FRAME_PTIME    20

/* Don't change this */
#define CHANNEL_COUNT            1

/* If you change CLOCK_RATE then the input WAV files need to be
 * changed, so normally don't need to change this.
 */
#define TEST_CLOCK_RATE			8000

/* You may change sound device's clock rate as long as resampling
 * is enabled.
 */
#define DEV_CLOCK_RATE            8000

#define TEST_SLEEP 10

#if 1

void systest_wizard(void);

void systest_list_audio_devs(void);

void systest_display_settings(void);

void systest_play_tone(void);

void systest_play_wav1(void);

void systest_play_wav2(void);

void systest_rec_audio(void);

void systest_audio_test(void);

void systest_latency_test(void);

void systest_aec_test(void);

void exit_app(void);

/* Menus */
static gui_menu menu_exit = {"Exit", &exit_app};

static gui_menu menu_wizard = {"Run test wizard", &systest_wizard};
static gui_menu menu_playtn = {"Play Tone", &systest_play_tone};
static gui_menu menu_playwv1 = {"Play WAV File1", &systest_play_wav1};
static gui_menu menu_playwv2 = {"Play WAV File2", &systest_play_wav2};
static gui_menu menu_recaud = {"Record Audio", &systest_rec_audio};
static gui_menu menu_audtest = {"Device Test", &systest_audio_test};
static gui_menu menu_calclat = {"Latency Test", &systest_latency_test};
static gui_menu menu_sndaec = {"AEC/AES Test", &systest_aec_test};

static gui_menu menu_listdev = {"View Devices", &systest_list_audio_devs};
static gui_menu menu_getsets = {"View Settings", &systest_display_settings};

static gui_menu menu_tests = {
        "Tests", NULL,
        10,
        {
                &menu_wizard,
                &menu_audtest,
                &menu_playtn,
                &menu_playwv1,
                &menu_playwv2,
                &menu_recaud,
                &menu_calclat,
                &menu_sndaec,
                NULL,
                &menu_exit
        }
};

static gui_menu menu_options = {
        "Options", NULL,
        2,
        {
                &menu_listdev,
                &menu_getsets,
        }
};

static gui_menu root_menu = {
        "Root", NULL, 2, {&menu_tests, &menu_options}
};

#endif


#if defined(PJ_WIN32_WINCE) && PJ_WIN32_WINCE
#define LOG_OUT_PATH		"\\PJSYSTEST.TXT"
#define RESULT_OUT_PATH		"\\PJSYSTEST_RESULT.TXT"
#define WAV_PLAYBACK_PATH		"\\Program Files\\pjsystest\\input.8.wav"
#define WAV_REC_OUT_PATH		"\\PJSYSTEST_TESTREC.WAV"
#define WAV_TOCK8_PATH		"\\Program Files\\pjsystest\\tock8.WAV"
#define WAV_LATENCY_OUT_PATH	"\\PJSYSTEST_LATREC.WAV"
#define ALT_PATH1			""
#define AEC_REC_PATH		"\\PJSYSTEST_AECREC.WAV"
#else
#define LOG_OUT_PATH        "/sdcard/PJSYSTEST.TXT"
#define RESULT_OUT_PATH        "/sdcard/PJSYSTEST_RESULT.TXT"
#define WAV_PLAYBACK_PATH        "/sdcard/input.WAV"
#define WAV_REC_OUT_PATH        "/sdcard/rec.WAV"
#define WAV_TOCK8_PATH        "/sdcard/tock8.wav"
#define WAV_LATENCY_OUT_PATH    "/sdcard/PJSYSTEST_LATREC.WAV"
#define ALT_PATH1            ""
#define AEC_REC_PATH        "/sdcard/aec_rec.WAV"
#endif

/* API, to be called by main() */
int systest_init(void);

int systest_set_dev(int cap_dev, int play_dev);

int systest_run(void);

void systest_save_result(const char *filename);

void systest_deinit(void);

/* Device ID to test */
extern int systest_cap_dev_id;
extern int systest_play_dev_id;

/* Test item is used to record the test result */
typedef struct test_item_t {
    char title[80];
    pj_bool_t skipped;
    pj_bool_t success;
    char reason[1024];
} test_item_t;

#define SYSTEST_MAX_TEST    32
extern unsigned test_item_count;
extern test_item_t test_items[SYSTEST_MAX_TEST];
#define PATH_LENGTH        PJ_MAXPATH
extern char doc_path[PATH_LENGTH];
extern char res_path[PATH_LENGTH];

test_item_t *systest_alloc_test_item(const char *title);

#if 1
void systest_perror(const char *title, pj_status_t status);

test_item_t *systest_alloc_test_item(const char *title);

pj_status_t create_player(unsigned path_cnt, const char *paths[],
                                 pjsua_player_id *p_id);

void systest_play_wav(unsigned path_cnt, const char *paths[]);

int calculate_latency(pj_pool_t *pool, pjmedia_port *wav,
                             unsigned *lat_sum, unsigned *lat_cnt,
                             unsigned *lat_min, unsigned *lat_max);

int systest_set_dev(int cap_dev, int play_dev);

void systest_save_result(const char *filename);


/*******  PLAY *******/

pjsua_player_id play_init(const char *paths,pjsua_conf_port_id *slot_id);

int play_stop();

int play_destroy(pjsua_player_id play_id);

/******  RECODE *****/

int record_init(char* out_path, pjsua_conf_port_id *slot_id, pj_pool_t **pool);

int record_stop();

int record_destroy(pjsua_recorder_id rec_id,pjsua_conf_port_id rec_slot,pj_pool_t *pool);

/******** AEC ********/

int aec_init(char* play_path,char* aec_rec_path,int ec_tail,int options,pjsua_player_id *play_id,pjsua_recorder_id *write_id,pj_bool_t is_aec);

int aec_stop(pjsua_player_id player_id,pjsua_recorder_id writer_id);

int aec_destroy(pjsua_player_id player_id,pjsua_recorder_id writer_id,int ec_tail);

int adjMicLevel(float level,pjsua_conf_port_id slot);

int adjSpeakerLevel(float level,pjsua_conf_port_id slot);

int adjNoVad(pj_bool_t noVad);

#endif

//#ifdef __cplusplus
//}
//#endif

#endif    /* __SYSTEST_H__ */