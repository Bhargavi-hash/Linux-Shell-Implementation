#include "headers.h"

char *list_com[100000], *temp[100000];
char ogcom[10000];
int s, x = 0;
ll ii = 0;
ll l, r, myid;

void del_process(int id)
{
  // printf("Entered killproc with job count = %lld and proc number=%d\n", job_count, id);
  int flag = 0;
  if (id == -1)
    job_count = 0;
  else
    for (ll i = 1; i <= job_count; i++)
    {
      if (job_arr[i].pid == id)
      {
        // printf("job killed\n");
        flag = 1;
        for (ll j = i; j < job_count; j++)
          job_arr[j] = job_arr[j + 1];
        job_count--;
      }
    }
  if (flag == 0 && id != -1)
    printf("Error: no such process found\n");
}

void done()
{
  pid_t p;
  int status;
  p = waitpid(-1, &status, WNOHANG);
  for (ll z = 1; z <= job_count; z++)
  {
    if (p < 0)
    {
      perror("\nwaitpid failed\n");
    }
    const int exit = WEXITSTATUS(status);

    // printf("WIFEXITED (STATUS) OF KJOB = %d\nWEXITSTATUS %d  p= %d  job_arr[z] %d\n", WIFEXITED(status), WEXITSTATUS(status),p,job_arr[z].pid);

    if (((WIFEXITED(status) && p == job_arr[z].pid) || (kjobkill == 1 && p == job_arr[z].pid)) && overkillflag == 0)
    {
      if (kjobkill == 1)
        kjobkill = 0;
      if (exit == 0)
        fprintf(stderr, "\nExitted normally with exit status: %d\n", exit);
      else
        fprintf(stderr, "\nExitted abnormally\n");

      fprintf(stderr, "%s with pid %d: exited\n", job_arr[z].name, p);
      promptprint();
      fflush(stdout);
      del_process(p);
    }
  }
  if (overkillflag == 1)
    del_process(-1);
}

void ctrl_c(int signo)
{
  printf("\n");

  pid_t p = getpid();
  if (p != myid)
    return;

  if (p == myid && current_fore.pid == -1)
  {
    promptprint();
    fflush(stdout);
  }
  if (current_fore.pid != -1)
  {
    kill(current_fore.pid, SIGINT);
  }
  signal(SIGINT, ctrl_c);
}

void ctrl_z(int signo)
{
  printf("\n");

  pid_t p = getpid();
  if (p != myid)
    return;
  if (current_fore.pid != -1)
  {
    kill(current_fore.pid, SIGTTIN);
    kill(current_fore.pid, SIGTSTP);
    job_count++;
    job_arr[job_count].pid = current_fore.pid;
    strcpy(job_arr[job_count].name, current_fore.name);
    return;
  }
  signal(SIGTSTP, ctrl_z);
  if (p == myid)
  {
    promptprint();
    fflush(stdout);
  }

  return;
}

// ****************************************** MAIN LOOP ***********************************************************//

void loop(void)
{
  char *command; //reading the command
  char **args;   //reading the arguments
  ssize_t size = 0;
  status = 1; //status of the executed command
  char dupl_Term_Input[1000];

  HistoryIndex = 0;
  do
  {
    redflag = 0;
    current_fore.pid = -1;
    //********************************************* SIGNALS ***********************************************
    signal(SIGCHLD, done);
    signal(SIGINT, ctrl_c);
    signal(SIGTSTP, ctrl_z);

    // CTRL + D
    if (feof(stdin))
    {
      printf("\n");
      exit(1);
    }
    //************************************* DISPLAYING THE PROMPT  ***********************************************

    promptprint();

    //*************************************************** COMMANDS ************************************************

    getline(&command, &size, stdin);

    if (HistoryIndex % 20 > 0)
    {
      if (strcmp(dupl_Term_Input, command) != 0)
      {
        strcpy(HistoryBox[HistoryIndex % 20], command);
        fprintf(fp1, "%s\n", command);
        HistoryIndex += 1;
      }
    }
    else
    {
      //fseek(fp1, 0, SEEK_SET);
      strcpy(HistoryBox[0], command);
      fprintf(fp1, "%s\n", command);
      HistoryIndex += 1;
    }
    strcpy(dupl_Term_Input, command);
    list_com[0] = strtok(command, ";\n"); //separating the commands
    ll i = 0;
    while (list_com[i] != NULL)
    {
      i++;
      list_com[i] = strtok(NULL, ";\n");
    }

    for (ll j = 0; j < i; j++)
    {
      strcpy(ogcom, list_com[j]);

      getcwd(cwd, sizeof(cwd));

      //********************************** CHECK *************************************//
      if (pipecheck(list_com[j]))
      {
        temp[0] = strtok(list_com[j], "|");
        k = 0;
        while (temp[k] != NULL)
        {
          k++;
          temp[k] = strtok(NULL, "|");
        }
        piping(temp, k);
      }

      else
      {
        token[0] = strtok(list_com[j], " \t\r\n");
        k = 0;

        while (token[k] != NULL) //Separating tokens within the command
        {
          k++;
          token[k] = strtok(NULL, " \t\r\n");
        }
        Implement_commands();
      }
    }

  } while (status);
}

void Implement_commands()
{
  for (ll j = 0; j < k; j++)
  {
    for (ll i = 0; token[j][i]; i++)
      if ((token[j][i] == '>') || token[j][i] == '<' || (token[j][i] == '>' && token[j][i + 1] == '>') && redflag == 0)
      {
        redflag = 1;
        if (token[j][i] == '>' && token[j][i + 1] == '>')
          i++;
      }
      else if ((token[j][i] == '>') || token[j][i] == '<' || (token[j][i] == '>' && token[j][i + 1] == '>') && redflag == 1)
      {
        redflag = 2;
        if (token[j][i] == '>' && token[j][i + 1] == '>')
          i++;
      }
  }

  if (strcmp(token[k - 1], "&") == 0) // FOR BACKGROUND PROCESSES
  {
    token[k - 1] = NULL;
    back(token);
  }
  else if (redflag == 1 || redflag == 2) // FOR REDIRECTION
    redirection(token, k, ogcom, redflag);

  else if (token[0][0] == '.') // FOR EXECUTABLES
    foreground(token);

  else if (strcmp(token[0], "quit") == 0) //EXIT
  {
    overkill();
    status = 0;
  }
  else if (strcmp(token[0], "pwd") == 0) //COMMAND : PWD
    pwd();
  else if (strcmp(token[0], "cd") == 0) //COMMAND : CD
    cd(token, k);
  else if (strcmp(token[0], "echo") == 0) //COMMAND : ECHO
    echo(token, k);
  else if (strcmp(token[0], "ls") == 0) // COMMAND :LS
    Trigger_ls(k, token);
  else if (strcmp(token[0], "pinfo") == 0) //COMMAND :PINFO
    pinfo(k, token);
  else if (strcmp(token[0], "repeat") == 0)
    repeat(k, token);
  else if (strcmp(token[0], "sleep") == 0)
    sleep(atoi(token[1]));
  else if (strcmp(token[0], "vi") == 0 || strcmp(token[0], "emacs") == 0 || strcmp(token[0], "gedit") == 0) // COMMAND : VI, EMACS, GEDIT FOREGROUND
    foreground(token);
  else if (strcmp(token[0], "code") == 0) // TRIAL COMMAND FOR VSCODE
    foreground(token);
  else if (strcmp(token[0], "history") == 0) //HISTORY
    history_command(k, token);
  else if (strcmp(token[0], "jobs") == 0) //JOBS
    alljobs();
  else if (strcmp(token[0], "kjob") == 0) //KJOBS
    kjob(token, k);
  else if (strcmp(token[0], "bg") == 0) //BG
    bg(token);
  else if (strcmp(token[0], "fg") == 0) //FG
    fg(token);
  else if (strcmp(token[0], "overkill") == 0) //OVERKILL
    overkill();
  else
  {
    if (strcmp(token[0], "setenv") == 0)
    {
      if (k != 3)
        printf("Error: Incorrect number of arguments\n");

      else
      {
        ll z = setenv(token[1], token[2], 1);
        if (z < 0)
          perror("Error:\n");
      }
    }
    else
    {
      if (strcmp(token[0], "unsetenv") == 0)
      {
        if (k != 2)
          printf("Error: Incorrect number of arguments\n");
        else
        {
          ll z = unsetenv(token[1]);
          if (z < 0)
            perror("Error:\n");
        }
      }
      else
        foreground(token);
    }
  }
}

//************************************************* MAIN *******************************************

int main(int argc, char const *argv[])
{
  myid = getpid();

  //**************************************** opening history file **********************************
  fp1 = fopen("history.txt", "a+");
  fseek(fp1, 0, SEEK_SET);

  // *************************************************** PROGRAM START *******************************

  char currentdir[100000], to[100000];
  int pos = 0, sl;
  child_flag = 0;
  curid = -1;
  job_count = 0;
  kjobkill = 0;
  overkillflag = 0;
  pipingflag = 0;

  getcwd(currentdir, sizeof(currentdir));
  // printf("%s\n", currentdir);

  while (argv[0][pos])
  {
    if (argv[0][pos] == '/')
      sl = pos;
    pos++;
  }

  for (ll i = 0; i < sl; i++)
    to[i] = argv[0][i];

  chdir(to);
  getcwd(pseudo_home, sizeof(pseudo_home));
  getlogin_r(Username, sizeof(Username)); //Getting the username of the current active user
  gethostname(Sysname, sizeof(Sysname));  //Getting the machine name
  chdir(currentdir);

  loop();
  return 0;
}
