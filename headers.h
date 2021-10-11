#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <signal.h>
#include <sys/stat.h>
#include <grp.h>
#include <time.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/utsname.h>


#define ll long long int
#define MotherDirct_Symbl '~'

char cwd[20000];
char pseudo_home[1024];
char Dest_Path[290];
char Directory_Present;
char Sysname[30];
char Username[30];
char history[10][1000];
char *token[10000];
char transitfile[100000];
char Permissions[11];
char HistoryBox[21][256];

int pid_arr[100000];
int child_flag;
int curid;
int kjobkill;
int overkillflag;
int redflag;
int pipingflag;
int flg_a;
int flg_l;
int HistoryIndex;
int transitfd;
int childid;
int status;
int k;

FILE *fp1;

struct utsname SystemProperties;
struct dirent *FileList;
struct stat fileDetails;

typedef struct jobs{
    char name[10000];
    pid_t pid;
}job;

ll job_count;
struct jobs job_arr[100000];
struct jobs current_fore;

void loop(void);

void promptprint();
void Implement_commands();

void foreground(char *token[]);
void back(char *token[]);

void pwd();
void cd(char *token[], ll k);
void echo(char *token[], ll k);
void Trigger_ls(ll k, char *token[]);
void pinfo(ll k, char *token[]);
void repeat(int k, char *token[]);


void redirection(char *token[], ll k, char list_com[], int redflag);

void overkill();
void settingenv(char *token[],ll k);
void unsettingenv(char *token[], ll k) ;
void del_process(int id);

int pipecheck();
void piping(char *commands[], ll k);

void alljobs();
void kjob(char *token[],ll k);

void fg(char *token[]);
void bg(char *token[]);

void history_command(ll k, char *token[]);
extern int alphasort();

#endif