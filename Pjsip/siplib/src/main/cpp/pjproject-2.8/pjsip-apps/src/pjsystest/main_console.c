/* $Id: main_console.c 5035 2015-03-27 06:17:27Z nanang $ */
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
#include "main_console.h"

static pj_bool_t console_quit;
static pj_bool_t inited;

#if 0
enum gui_key gui_msgbox(const char *title, const char *message, enum gui_flag flag) {
    ALOGE("%s",title);
    ALOGE("%s",message);

    for (;;) {
        char input[10], *ret;

        if (flag == WITH_YESNO)
            ALOGE("%c:Yes  %c:No  ", KEY_YES, KEY_NO);
        else if (flag == WITH_OK)
            ALOGE("%c:OK  ", KEY_OK);
        else if (flag == WITH_OKCANCEL)
            ALOGE("%c:OK  %c:Cancel  ", KEY_OK, KEY_CANCEL);
        ALOGE("");

        ret = fgets(input, sizeof(input), stdin);
        if (!ret)
            return KEY_CANCEL;

        if (input[0] == KEY_NO || input[0] == KEY_YES || input[0] == KEY_CANCEL)
        return KEY_YES;
    }
}
#endif

pj_status_t gui_init(gui_menu *menu) {
    PJ_UNUSED_ARG(menu);
    return PJ_SUCCESS;
}

static void print_menu(const char *indent, char *menu_id, gui_menu *menu) {
    char child_indent[16];
    unsigned i;

    pj_ansi_snprintf(child_indent, sizeof(child_indent), "%s  ", indent);

    ALOGE("%s%s: %s\n", indent, menu_id, menu->title);

    for (i=0; i<menu->submenu_cnt; ++i) {
	char child_id[10];

	pj_ansi_sprintf(child_id, "%s%u", menu_id, i);

	if (!menu->submenus[i])
	    ALOGE("");
	else
	    print_menu(child_indent, child_id, menu->submenus[i]);
    }
}

#if 0
pj_status_t gui_start(gui_menu *menu)
{
    while (!console_quit) {
	unsigned i;
	char input[10], *p;
	gui_menu *choice;

	ALOGE("M E N U :");
        ALOGE("---------");
	for (i=0; i<menu->submenu_cnt; ++i) {
	    char menu_id[4];
	    pj_ansi_sprintf(menu_id, "%u", i);
	    print_menu("", menu_id, menu->submenus[i]);
	}
        ALOGE("");
        ALOGE("Enter the menu number: ");

	if (!fgets(input, sizeof(input), stdin))
	    break;

	p = input;
	choice = menu;
	while (*p && *p!='\r' && *p!='\n') {
	    unsigned d = (*p - '0');
	    if (d >= choice->submenu_cnt) {
            ALOGE("Invalid selection");
		choice = NULL;
		break;
	    }

	    choice = choice->submenus[d];
	    ++p;
	}

	if (choice && *p!='\r' && *p!='\n') {
        ALOGE("Invalid characters entered");
	    continue;
	}

	if (choice && choice->handler)
	    (*choice->handler)();
    }

    return PJ_SUCCESS;
}
#endif

#if 1

void menu_input(char *inputs){

    gui_menu *choice;
    char *p;

    // 输入为 NULL 或者 exit 时退出测试
    if(!inputs ){
        systest_deinit();
    }

    p = inputs;
    choice = &root_menu;
    while (*p && *p != '\r' && *p != '\n') {
        unsigned d = (*p - '0');
        if (d >= choice->submenu_cnt) {
            ALOGE("Invalid selection");
            choice = NULL;
            break;
        }

        choice = choice->submenus[d];
        ++p;
    }

    if (choice&& !p) {
        ALOGE("Invalid characters entered");
    } else{
        if (choice && choice->handler)
            (*choice->handler)();
    }

    //修改嵌套循环
    if(!console_quit){
        gui_start(choice);
    }
}

pj_status_t gui_start(gui_menu *menu) {
    unsigned i;
    char input[10], *p;

    ALOGE("M E N U :");
    ALOGE("---------");
    for (i = 0; i < menu->submenu_cnt; ++i) {
        char menu_id[4];
        pj_ansi_sprintf(menu_id, "%d", i);
        print_menu("", menu_id, menu->submenus[i]);
    }
    ALOGE("");
    ALOGE("Enter the menu number: ");

    return PJ_SUCCESS;
}

#endif

void gui_destroy(void) {
    console_quit = PJ_TRUE;
}

void gui_sleep(unsigned sec) {
    pj_thread_sleep(sec * 1000);
}

int main() {
    if (systest_init() != PJ_SUCCESS)
        return 1;
    systest_run();
    systest_deinit();
    return 0;
}


int test_inits() {
    pjsua_var.is_aecm = PJ_FALSE;
    pjsua_var.delat_time = 0;
    if (systest_init() != PJ_SUCCESS)
        return 1;
    inited = PJ_TRUE;
    return 0;
}

int _test_destroy(){
    if(inited) {
        systest_deinit();
        inited = PJ_FALSE;
    }
    return 0;
}

pjsua_player_id play_id = PJSUA_INVALID_ID;
pjsua_recorder_id *write_id =PJSUA_INVALID_ID;
pjsua_conf_port_id slot_id = PJSUA_INVALID_ID;
pj_pool_t *pool =NULL;
pjsua_recorder_id rec_id = PJSUA_INVALID_ID;

/*******  PLAY *******/
int test_play_init(int play_type){
    char *path;
    if(play_type == 0){     //aec处理之后
        path = "/sdcard/out.wav";
    } else if(play_type == 1){  //回采
        path ="/sdcard/rec.wav";
    } else{     //default
        path = "/sdcard/play.wav";
    }
    play_id = play_init(path,&slot_id);
    return play_id;
}

int test_play_stop(){
    return play_stop();
}

int test_play_destroy(){
    play_destroy(play_id);
    play_id = PJSUA_INVALID_ID;
    return 0;
}


/******  RECORD *****/
int test_record_init(){
    char* out_path = "/sdcard/play.wav";
    rec_id = record_init(out_path,&slot_id,&pool);
    return rec_id;
}

int test_record_stop(){
    return record_stop();
}

int test_record_destroy() {
    if (rec_id != PJSUA_INVALID_ID && slot_id != PJSUA_INVALID_ID && pool != NULL) {
        record_destroy(rec_id, slot_id, pool);
    }
    rec_id = PJSUA_INVALID_ID;
    slot_id = PJSUA_INVALID_ID;
    pool = NULL;
}
int tail =0;
/******** AEC ********/
pj_bool_t isAec = PJ_FALSE;
int test_aec_init(int tail_length,int options,pj_bool_t is_aec){

    is_open_aec = is_aec;
    char *play_path = "/sdcard/play.wav";
    char *aec_rec_path = "/sdcard/rec.wav";
    int ec_tail = 200;
    tail = tail_length;
    aec_init(play_path,aec_rec_path,tail_length,options,&play_id,&write_id,is_aec);
    ALOGE("--------test_aec_init %d,%d",play_id,write_id);
    return 0;
}

int test_aec_stop(){

    return aec_stop(play_id,write_id);
}

int test_aec_destroy(){
    isAec = PJ_FALSE;
    if(play_id != PJSUA_INVALID_ID && write_id != PJSUA_INVALID_ID) {
        aec_destroy(play_id, write_id, tail);
        tail =0;
    }
    play_id = PJSUA_INVALID_ID;
    write_id = PJSUA_INVALID_ID;
    return 0;
}


/************ 参数设置 ***************/

int setMicLevel(float level){
    if(isAec){
        return adjMicLevel(level,write_id);
    } else {
        return adjMicLevel(level, slot_id);
    }
}

int setSpeakerLevel(float level){
    if(isAec) {
        return adjSpeakerLevel(level, play_id);
    } else{
        return adjSpeakerLevel(level, slot_id);
    }
}

int set_delat_time(int delat_time)
{
    if(delat_time >0) {
        pjsua_var.delat_time = delat_time;
    }
    return 0;
}

int setNoVad(pj_bool_t noVad){
    return adjNoVad(noVad);
}

