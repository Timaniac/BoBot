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

bool daemon, hlp;

int main(int argc, char** argv)
{
	ParseOpt(argc, argv);
	
	if(serv==null || chan==null || user==null || pass==null)
	   Hlp();
	
	if(hlp)
	   return 0;

	if(daemon)
	   Daemon();

	sqlite3_open("Bobot_db.db", &dBase);
	
	memset(&callbacks, 0, sizeof(callbacks));

	InitIRC();
	
	
	ctx.channel = chan;
	ctx.nick    = user;
	ctx.password = pass;

	s = irc_create_session(&callbacks);
	
	if (!s)	
	{
		printf("Session configuration fault!\n");
		return 1;
	}

	irc_set_ctx(s, &ctx);
	irc_option_set(s, LIBIRC_OPTION_STRIPNICKS);

	
	if ( irc_connect(s, server, 6667, 0, user, 0, 0))
	{
		printf("No connection!\n");
		return 1;
	}

	irc_run(s);

	return 0;
}

/**
 * uebernahme Optionen
 */
void ParseOpt(int argc, char** argv)
{
	int opt = 0;
	while((opt = getopt(argc, argv, "s:u:c:d::p:h::")) != -1)
	{
		switch(opt)
		{
			case 's': serv=optarg; break;
			case 'u': user=optarg; break;
			case 'c': chan=optarg; break;
			case 'p': pass=optarg; break;
			case 'd': daemon=true; break;
			case 'h': Hlp(); break;
		}
	}
}

/**
 * Help and error messages
 */
void Hlp()
{
	hlp = true;
	printf("Instructions: \n\n");
	printf("Needed files are: Server, Channel, Nickname, Password\n");
	printf("-u Irc-Name \n");
	printf("-p Password \n");
	printf("-s Server Name \n");
	printf("-c Channel Name \n");
	printf("-d Daemon?\n");
}

/**
 * Demonprocess
 */
void Daemon()
{
	pid_t pid, sid;
	if(getppid() == 1) return;
        pid = fork();
	if(pid<0) exit(1);

	if(pid>0) exit(0);

	umask(0);
	sid = setsid();
	if(sid < 0) exit(1);
	
	if((chdir("/") < 0)) exit(1);

	freopen("/dev/null", "r", stdin);
	freopen("/dev/null", "w", stdout);
	freopen("/dev/null", "w", stderr);
}

/**
 * Event handler
 */
void InitIRC()
{
	callbacks.event_connect = event_connect;
	callbacks.event_privmsg = event_privmsg;
	callbacks.event_nick = event_nick;
	callbacks.event_join = event_join;
	callbacks.event_part = event_part;
	callbacks.event_topic = event_topic;
	callbacks.event_channel = event_channel;
}

/**
 * Connect_Event
 */

void event_connect (irc_session_t* session, const char* event, const char* origin, const char** params, unsigned int count)
{
  irc_ctx_t* ctx = (irc_ctx_t*) irc_get_ctx(session);
  irc_cmd_join (session, ctx->channel, 0); 
}

/**
 * Join_Event
 */

void event_join (irc_session_t* session, const char* event, const char* origin, const char** params, unsigned int count)
{    
	irc_cmd_msg (session, params[0], "IRCBOT loaded");  
	char str_join[200];  
	sprintf(str_join, "INSERT INTO join_irc (Channel, User, Joindate, Jointime) Values ('%s', '%s', date('now'), time('now'))",params[0], origin);  
	sqlite3_exec(Database, str_join,0,0,0);  
	printf("User: '%s', connected\n",origin);
}

/**
 * Part_Event
 */

void event_part (irc_session_t* session, const char* event, const char* origin, const char** params, unsigned int count)
{
	char str_quit[500];
	printf("User: '%s', left\n", origin);	
	sprintf(str_quit, "INSERT INTO part_irc (user, channel, reason, partdate, parttime) VALUES('%s', '%s', '%s', date('now'), time('now'))",origin, params[0], params[1]);
	sqlite3_exec(Database, str_quit,0,0,0);	
}

/**
 * PrivMsg_Event - Orders
 */
void event_privmsg (irc_session_t* session, const char* event, const char* origin, const char** params, unsigned int count) 
{
	printf("'%s' said '%s' to you\n", origin, params[1]);	
	
	if (strcmp(params[1],"-hello")==0)	
	{
		printf("Private Message!\n");
		irc_cmd_msg(session, origin, "I respond to private messages ");		
	}
	
	if (strcmp(params[1],"-login")==0)	
	{
		strcpy(global_origin, origin);		
		sqlite3_exec(Database, "Select partdate From join_irc Order by rowid desc limit 1",SQLCallback,0,0);		
		printf(global_origin);		
		fflush(stdout);
	}
	
	if (strcmp(params[1],"-help")==0)
	{	
		irc_cmd_msg(session, origin, "-hallo : get back status");
		irc_cmd_msg(session, origin, "-login : days online");
		irc_cmd_msg(session, origin, "-help: show this message");
		irc_cmd_msg(session, origin, "-logfile: create log-file");
	}
	
	if (strcmp(params[1],"-logfile")==0)
	{
		log_file = fopen("log.txt", "w");
		sqlite3_exec(Database, "Select * From join_irc",SQLCallback_File,0,0);
		fclose(log_file);			
	}
		
}
/**
 * Channel_Event
 */
void event_channel(irc_session_t* session, const char* event, const char* origin, const char** params, unsigned int count)
{
	char str_insert[10000];	
	sprintf(str_insert, "INSERT INTO log_irc (User, Channel, Message, InsertDate, InsertTime) Values ('%s', '%s', '%s', date('now'), time('now'))", origin, params[0], params[1]);
	sqlite3_exec(Database, str_insert,0,0,0);
    	printf ("'%s' said in channel %s: %s\n", 
    	origin ? origin : "One in the channel", 
    	params[0], params[1]);	
}

/**
 * Topic_Event
 */
void event_topic(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
	if (count > 1) {
		Com_Printf("IRC: %s Topicchange %s to %s\n", origin, params[0], params[1]);
	}
	else {
		Com_Printf("IRC: %s Topicchange on %s\n", origin, params[0]);
	}
}

/**
 * Nick_Event
 */
void event_nick (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
	Com_Printf("IRC: Nich change!  %s to %s\n", params[0], params[1]);
}

/**
 * Callback from SQL
 */
static int SQLCallback(void *NotUsed, int argc, char **argv, char **azColName) 
{
  int i; 
  for(i=0; i<argc; i++) 
  { 	
	char str_sql[500];	
	sprintf(str_sql,"%s : %s\n", azColName[i], argv[i] ? argv[i] : "NULL"); 	
	irc_cmd_msg(s, global_origin, str_sql);
  } 
  printf("\n"); 
  fflush(stdout);
  return 0; 
}	

/**
 * File_Callback
 */
static int SQLCallback_File(void *NotUsed, int argc, char **argv, char **azColName) 
{
  int i;   
  for(i=0; i<argc; i++)
  { 	
	fprintf(log_file, "%s : %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	printf("%s : %s\n", azColName[i], argv[i] ? argv[i] : "NULL");	
  } 
  printf("\n");
  return 0; 
}
