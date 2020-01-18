#include "systest.h"
#include <stdio.h>
#include <pjlib.h>
#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

enum gui_key gui_msgbox(const char *title, const char *message, enum gui_flag flag);


pj_status_t gui_init(gui_menu *menu);


static void print_menu(const char *indent, char *menu_id, gui_menu *menu);

void menu_input(char *input);

pj_status_t gui_start(gui_menu *menu);


void gui_destroy(void);

void gui_sleep(unsigned sec);

int main();

int test_inits();

int _test_destroy();

/*******  PLAY *******/
int test_play_init(int play_type);

int test_play_stop();

int test_play_destroy();

/******  RECORD *****/
int test_record_init();

int test_record_stop();

int test_record_destroy();

/******** AEC ********/
int test_aec_init(int tail_length,int options,pj_bool_t is_aec);

int test_aec_stop();

int test_aec_destroy();

int setMicLevel(float level);

int setSpeakerLevel(float level);

int set_delat_time(int delat_time);

int setNoVad(pj_bool_t noVad);
#ifdef __cplusplus
}
#endif