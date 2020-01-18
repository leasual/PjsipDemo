//
// Created by liuzl15 on 2019/11/7.
//

#ifndef CSIPSDK_AECTEST_H
#define CSIPSDK_AECTEST_H


#include <pjmedia.h>
#include <pjlib-util.h>	/* pj_getopt */
#include <pjlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define THIS_FILE   "aectest.c"
#define PTIME	    20
#define TAIL_LENGTH 200

static const char *desc =
        " FILE		    						    \n"
        "		    						    \n"
        "  aectest.c	    						    \n"
        "		    						    \n"
        " PURPOSE	    						    \n"
        "		    						    \n"
        "  Test the AEC effectiveness.					    \n"
        "		    						    \n"
        " USAGE		    						    \n"
        "		    						    \n"
        "  aectest [options] <PLAY.WAV> <REC.WAV> <OUTPUT.WAV>		    \n"
        "		    						    \n"
        "  <PLAY.WAV>   is the signal played to the speaker.		    \n"
        "  <REC.WAV>    is the signal captured from the microphone.	    \n"
        "  <OUTPUT.WAV> is the output file to store the test result	    \n"
        "\n"
        " options:\n"
        "  -d  The delay between playback and capture in ms, at least 25 ms.\n"
        "      Default is 25 ms. See note below.                            \n"
        "  -l  Set the echo tail length in ms. Default is 200 ms	    \n"
        "  -r  Set repeat count (default=1)                                 \n"
        "  -a  Algorithm: 0=default, 1=speex, 2=echo suppress, 3=WebRtc	    \n"
        "  -i  Interactive						    \n"
        "\n"
        " Note that for the AEC internal buffering mechanism, it is required\n"
        " that the echoed signal (in REC.WAV) is delayed from the           \n"
        " corresponding reference signal (in PLAY.WAV) at least as much as  \n"
        " frame time + PJMEDIA_WSOLA_DELAY_MSEC. In this application, frame \n"
        " time is 20 ms and default PJMEDIA_WSOLA_DELAY_MSEC is 5 ms, hence \n"
        " 25 ms delay is the minimum value.                                 \n";




static void app_perror(const char *sender, const char *title, pj_status_t st);


int test_main(char *play_path,char *rec_path,char *out_path,int tail_length,int delay,int options);

#ifdef __cplusplus
}
#endif

#endif //CSIPSDK_AECTEST_H
