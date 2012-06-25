/**
 * IRC-Bot by Boris Heier M1000661
 */

#include "ircbot.h"
#include "libircclient.h"
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sqlite3.h>
#include <unistd.h>
#include <stdbool.h>


struct sqlite3* dBase;
char* chan;
char* pass;
char* serv;
char* user;

static char g_origin[50];

irc_callbacks_t callbacks;
irc_ctx_t ctx;
irc_session_t *s;
FILE *log_file;

typedef struct
{
  char* chan;
  char* nickname;
} 

bool daemonize, help;

