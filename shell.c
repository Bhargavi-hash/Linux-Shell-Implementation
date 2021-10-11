#include "headers.h"

int DirectoryBase = 0;

//************************************ Prompt ****************************************************
void promptprint()
{
    if (DirectoryBase == 0)
    {
        // Mother directory name is obtained when the
        // program started execution and doesn't change
        getcwd(pseudo_home, sizeof(pseudo_home));
        DirectoryBase = 1;
    }
    getcwd(cwd, sizeof(cwd));
    if (strcmp(pseudo_home, cwd) == 0)
    {
        // If cwd is mother directory
        Directory_Present = 'M';
    }
    else
        Directory_Present = 'O';

    if (Directory_Present == 'M')
        printf("<\033[1;32m%s@%s:\033[0m\033[1;34m%c>\033[0m  ", Username, Sysname, MotherDirct_Symbl);
    else
    {
        if (strlen(cwd) > strlen(pseudo_home))
        {
            int start = strlen(pseudo_home);
            int end = strlen(cwd);
            int k;
            for (k = 0; k < end - start; k++)
            {
                Dest_Path[k] = cwd[start + k];
            }
            printf("<\033[1;32m%s@%s:\033[0m\033[1;34m%c%s>\033[0m  ", Username, Sysname, MotherDirct_Symbl, Dest_Path);
        }
        else
        {
            printf("<\033[1;32m%s@%s:\033[0m\033[1;34m%s>\033[0m  ", Username, Sysname, cwd);
        }
    }
}

//*************************************** repeat ***************************************************

void repeat(int k, char *token[])
{
    int times_to_repeat = atoi(token[1]);
    
    k = k - 2;
    
    while (times_to_repeat--)
    {
        if (strcmp(token[2], "pwd") == 0) 
            pwd();
        else if (strcmp(token[2], "cd") == 0) 
            cd(token+2, k);
        else if (strcmp(token[2], "echo") == 0) 
            echo(token+2, k);
        else if (strcmp(token[2], "ls") == 0) 
            Trigger_ls(k, token+2);
        else if (strcmp(token[2], "pinfo") == 0) 
            pinfo(k, token+2);
        else if (strcmp(token[2], "sleep") == 0)
            sleep(atoi(token[3]));
    }
}

//************************************* ECHO Command *********************************************
void echo(char *token[], ll k)
{
    for (ll ind = 1; ind <= k - 1; ind++)
        printf("%s ", token[ind]);

    printf("\n");
    return;
}

//*************************************** ls command **************************************************
int CheckForFile(char *file)
{
    FILE *filePointer;

    if (filePointer = fopen(file, "r"))
    {
        int z = stat(file, &fileDetails);
        if (flg_l)
        {
            off_t size = fileDetails.st_size;
            /* The S_ISREG() macro tests for regular files */

            Permissions[0] = '-';

            //Owner permissions:
            if (fileDetails.st_mode & S_IRUSR)
                Permissions[1] = 'r';
            else
                Permissions[1] = '-';

            if (fileDetails.st_mode & S_IWUSR)
                Permissions[2] = 'w';
            else
                Permissions[2] = '-';

            if (fileDetails.st_mode & S_IXUSR)
                Permissions[3] = 'x';
            else
                Permissions[3] = '-';

            //Group permissions:
            if (fileDetails.st_mode & S_IRGRP)
                Permissions[4] = 'r';
            else
                Permissions[4] = '-';

            if (fileDetails.st_mode & S_IWGRP)
                Permissions[5] = 'w';
            else
                Permissions[5] = '-';

            if (fileDetails.st_mode & S_IXGRP)
                Permissions[6] = 'x';
            else
                Permissions[6] = '-';

            //Others permissions:
            if (fileDetails.st_mode & S_IROTH)
                Permissions[7] = 'r';
            else
                Permissions[7] = '-';

            if (fileDetails.st_mode & S_IWOTH)
                Permissions[8] = 'w';
            else
                Permissions[8] = '-';

            if (fileDetails.st_mode & S_IXOTH)
                Permissions[9] = 'x';
            else
                Permissions[9] = '-';

            Permissions[10] = '\0';
            printf("%s:   \t%s  %ld  %s  %s  %ld\t\t%s", file, Permissions, fileDetails.st_nlink, Username, Sysname, size, ctime(&fileDetails.st_mtime));
        }
        else
            printf("%s\n", file);
        fclose(filePointer);
        return 1;
    }
    else
    {
        free(filePointer);
        perror("Directory/file not found");
        return 0;
    }
}
int ls_command_1(char *Dirct, int flg_a, int flg_l)
{

    //Here we will list the directory
    DIR *dh = opendir(Dirct);
    if (!dh)
    {
        if (errno = ENOENT)
        {
            CheckForFile(Dirct);
            //If the directory is not found
            //perror("Directory not found");
        }
        else
        {
            //If the directory is not readable then throw error and exit
            perror("Unable to read directory");
        }
        free(dh);
        return 0;
    }

    while ((FileList = readdir(dh)) != NULL)
    {
        int z = stat(FileList->d_name, &fileDetails);
        //If hidden files are found we continue
        if (!flg_a && FileList->d_name[0] == '.')
            continue;
        else
        {
            if (flg_l)
            {
                off_t size = fileDetails.st_size;

                /* The S_ISREG() macro tests for regular files */
                if (S_ISREG(fileDetails.st_mode))
                    Permissions[0] = '-';
                else
                    Permissions[0] = 'd';

                //Owner permissions:
                if (fileDetails.st_mode & S_IRUSR)
                    Permissions[1] = 'r';
                else
                    Permissions[1] = '-';

                if (fileDetails.st_mode & S_IWUSR)
                    Permissions[2] = 'w';
                else
                    Permissions[2] = '-';

                if (fileDetails.st_mode & S_IXUSR)
                    Permissions[3] = 'x';
                else
                    Permissions[3] = '-';

                //Group permissions:
                if (fileDetails.st_mode & S_IRGRP)
                    Permissions[4] = 'r';
                else
                    Permissions[4] = '-';

                if (fileDetails.st_mode & S_IWGRP)
                    Permissions[5] = 'w';
                else
                    Permissions[5] = '-';

                if (fileDetails.st_mode & S_IXGRP)
                    Permissions[6] = 'x';
                else
                    Permissions[6] = '-';

                //Others permissions:
                if (fileDetails.st_mode & S_IROTH)
                    Permissions[7] = 'r';
                else
                    Permissions[7] = '-';

                if (fileDetails.st_mode & S_IWOTH)
                    Permissions[8] = 'w';
                else
                    Permissions[8] = '-';

                if (fileDetails.st_mode & S_IXOTH)
                    Permissions[9] = 'x';
                else
                    Permissions[9] = '-';

                Permissions[10] = '\0';
                printf("%s:   \t%s  %ld  %s  %s  %ld\t\t%s", FileList->d_name, Permissions, fileDetails.st_nlink, Username, Sysname, size, ctime(&fileDetails.st_mtime));
            }
            else
                printf("%s\n", FileList->d_name);
        }
    }
    return 1;
}
void ls_command_2(ll k, char *token[])
{
    int n = 0;
    for (ll i = 1; i < k; i++)
    {
        if (token[i][0] != '-')
        {
            n = 1;
            ls_command_1(token[i], flg_a, flg_l);
        }
    }
    if (n == 0)
        ls_command_1(".", flg_a, flg_l);
    return;
}
void Trigger_ls(ll k, char *token[])
{
    if (k == 1)
        ls_command_1(".", 0, 0);

    flg_a = 0;
    flg_l = 0;

    for (ll i = 1; i < k; i++)
    {
        if (token[i][0] == '-')
        {
            if (token[i][2] == '\0')
            {
                char flg = token[i][1];
                switch (flg)
                {
                case 'a':
                    flg_a = 1;
                    break;
                case 'l':
                    flg_l = 1;
                    break;
                default:
                    printf("'-%c' flag not found\n", flg);
                }
            }
            else
            {
                if ((strcmp(token[i], "-al") && strcmp(token[i], "-la")) == 0)
                {
                    flg_a = 1;
                    flg_l = 1;
                }
                else
                {
                    printf("'%s' flag not found\n", token[i]);
                }
            }
        }
    }
    ls_command_2(k, token);
    return;
}

//************************************ PWD-command ************************************************

void pwd()
{
    printf("%s\n", cwd);
}

//************************************ background-process ***************************************

void back(char *token[])
{
    curid++;
    pid_t pid = fork();
    setpgid(0, 0);
    if (pid < 0)
        perror("Error: Fork Failed\n");
    else if (pid == 0)
    {
        pid_t p = getpid();
        int z = execvp(token[0], token);
        if (z < 0)
            perror("myshell:Error\n");

        if (strcmp(token[0], "vim") == 0)
            kill(p, 19);
        exit(0);
    }
    else
    {
        job_count++;
        printf("[%lld] %d %s\n", job_count, pid, token[0]); // DETAILS OF BG PROCESS
        job_arr[job_count].pid = pid;
        strcpy(job_arr[job_count].name, token[0]); // TO KILL PROCESSES
    }

    return;
}

//************************************ pinfo-command ************************************************
void pinfo(ll k, char *token[])
{
    ll memory;
    int f, f2;

    char buf[10000];
    char buf2[10000];
    char file[10000];
    char status;
    char c;

    pid_t pid;

    if (k > 2)
    {
        printf("Error: Atmost 2 arguments are allowed\n");
        return;
    }
    if (k == 1)
        pid = getpid(); //GETTING PID
    if (k == 2)
        pid = atoi(token[1]); // PID <= second argument

    sprintf(file, "/proc/%d/stat", pid); //Process details: status, memory

    f = open(file, O_RDONLY);
    f2 = open(file, O_RDONLY);
    if (f < 0)
        perror("Error: no such process exists\n");
    else
    {
        FILE *fd;
        fd = fopen(file, "r");
        fscanf(fd, "%*d %*s %c %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %lld %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d", &status, &memory);
        fclose(fd);

        printf("PID -- %d\n", pid);
        printf("Process Status -- %c\n", status);
        printf("Memory -- %lld\n", memory);

        sprintf(file, "/proc/%d/exe", pid); // For executable path
        //printf("%s\n", file);
        int x = readlink(file, buf, sizeof(buf) - 1);

        x = 0;
        ll ii = 0;

        if (strcmp(buf, pseudo_home) == 0)
            strcpy(buf, "~");
        else
        {
            while (pseudo_home[ii] != '\0')
            {
                if (pseudo_home[ii] != buf[ii])
                {
                    x = ii;
                    break;
                }
                ii = ii + 1;
            }
            
            if (x == 0)
            {
                buf2[0] = '~';
                ll p = 1;
                ll j = ii;
                while (buf[j] != '\0')
                {
                    buf2[p] = buf[j];
                    p = p + 1;
                    j = j + 1;
                }
                buf2[p] = '\0';
            }
            else
                strcpy(buf2, buf);
        }

        printf("Executable Path---  %s\n", buf2);
    }
}

//****************************************History-command****************************************

void history_command(ll k, char *token[])
{
    int No_of_prev_comnds;
    if (k == 2)
    {
        No_of_prev_comnds = atoi(token[1]);
        if (No_of_prev_comnds < HistoryIndex)
        {
            for (int hid = HistoryIndex - No_of_prev_comnds; hid < HistoryIndex; hid++)
            {
                printf("%s\n", HistoryBox[hid % 20]);
            }
        }
        else
        {
            int cmnds_from_file = No_of_prev_comnds - HistoryIndex;
            fseek(fp1, 0, SEEK_SET);
            int count_lines = 0;
            char chr;
            //extract character from file and store in chr
            chr = getc(fp1);
            while (chr != EOF)
            {
                //Count whenever new line is encountered
                if (chr == '\n')
                {
                    count_lines = count_lines + 1;
                }
                //take next character from file.
                chr = getc(fp1);
            }
            fseek(fp1, 0, SEEK_SET);
            if (count_lines != 0 && count_lines >= cmnds_from_file)
            {
                int startline = 0;
                chr = getc(fp1);
                while (chr != EOF)
                {
                    //Count whenever new line is encountered
                    if (chr == '\n')
                    {
                        startline = startline + 1;
                    }
                    if (startline >= count_lines - cmnds_from_file - HistoryIndex)
                    {
                        printf("%c", chr);
                    }
                    if (startline == count_lines - HistoryIndex)
                        break;
                    //take next character from file.
                    chr = getc(fp1);
                }
            }
            if (count_lines < cmnds_from_file)
            {
                chr = getc(fp1);
                while (chr != EOF)
                {
                    printf("%c", chr);
                    chr = getc(fp1);
                }
            }
            for (int num = 0; num < HistoryIndex; num++)
                printf("%s\n", HistoryBox[num % 20]);
        }
    }
    if (k == 1)
    {
        if (HistoryIndex >= 10)
        {
            for (int hid = HistoryIndex - 10; hid < HistoryIndex; hid++)
                printf("> %s\n", HistoryBox[hid % 20]);
        }
        else
        {
            int cmnds_from_file = 10 - HistoryIndex;
            fseek(fp1, 0, SEEK_SET);
            int count_lines = 0;
            char chr;
            //extract character from file and store in chr
            chr = getc(fp1);
            while (chr != EOF)
            {
                //Count whenever new line is encountered
                if (chr == '\n')
                {
                    count_lines = count_lines + 1;
                }
                //take next character from file.
                chr = getc(fp1);
            }
            fseek(fp1, 0, SEEK_SET);
            if (count_lines != 0 && count_lines >= cmnds_from_file)
            {
                int startline = 0;
                chr = getc(fp1);
                while (chr != EOF)
                {
                    //Count whenever new line is encountered
                    if (chr == '\n')
                    {
                        startline = startline + 1;
                    }
                    if (startline >= count_lines - cmnds_from_file - HistoryIndex)
                    {
                        printf("%c", chr);
                    }
                    if (startline == count_lines - HistoryIndex)
                        break;
                    //take next character from file.
                    chr = getc(fp1);
                }
            }
            if (count_lines < cmnds_from_file)
            {
                chr = getc(fp1);
                while (chr != EOF)
                {
                    printf("%c", chr);
                    chr = getc(fp1);
                }
            }
            for (int num = 0; num < HistoryIndex; num++)
                printf("> %s\n", HistoryBox[num]);
        }
    }
}

//*****************************************CD-command**********************************************

void cd(char *token[], ll k)
{
    if (k > 2)
    {
        perror("Error: Too many arguments\n");
        return;
    }

    char cur[1000];
    if (k == 1)
        return;
    if (strcmp(token[1], "~") == 0)
    {
        chdir(pseudo_home);
        return;
    }
    if (strcmp(token[1], "-") == 0)
    {
        chdir("..");
        getcwd(cur, sizeof(cur));
        printf("%s\n", cur);
        return;
    }
    DIR *cd_dh = opendir(token[1]);
    if (!cd_dh)
    {
        if (errno = ENOENT)
            perror("Directory doesn't exist");
        else
            perror("Unable to read directory");

        free(cd_dh);
        return;
    }
    chdir(token[1]);
    return;
}

//**************************************** Foreground **********************************************

void foreground(char *token[])
{
    pid_t pid = fork();
    current_fore.pid = pid;
    strcpy(current_fore.name, token[0]);

    if (pid < 0)
        perror("Error: Fork Failed\n");
    else if (pid == 0)
    {
        int f = execvp(token[0], token);
        if (f < 0)
        {
            perror("Error: exec failed\n");
            printf("myshell: command not found: %s\n", token[0]);
        }
        exit(0);
    }
    else
    {
        int status;
        waitpid(pid, &status, WUNTRACED);
    }
}

//*************************************** piping ************************************************

int pipecheck(char com[])
{
    ll flag = 0;
    for (ll i = 0; com[i]; i++)
    {
        if (com[i] == '|')
        {
            flag = 1;
            break;
        }
    }
    return flag;
}

int redirection_check(char ogcom[])
{
    char *token[100000];
    token[0] = strtok(ogcom, " \t\r\n");
    ll k = 0, r;

    while (token[k] != NULL) //Separating tokens within the command
    {
        k++;
        token[k] = strtok(NULL, " \t\r\n");
    }
    for (ll j = 0; j < k; j++)
    {
        for (ll i = 0; token[j][i]; i++)
            if ((token[j][i] == '>') || token[j][i] == '<' || (token[j][i] == '>' && token[j][i + 1] == '>') && r == 0)
            {
                r = 1;
                if (token[j][i] == '>' && token[j][i + 1] == '>')
                    i++;
            }
            else if ((token[j][i] == '>') || token[j][i] == '<' || (token[j][i] == '>' && token[j][i + 1] == '>') && r == 1)
            {
                r = 2;
                if (token[j][i] == '>' && token[j][i + 1] == '>')
                    i++;
            }
    }
    return r;
}

void piping(char *commands[], ll k)
{
    pipingflag = 1;
    pid_t pid;
    int mypipe[2], mypipe2[2], inp = 0, outp = 0;
    char *part[10000], buffer[100000], ogcom[10000], temp[10000];
    for (ll i = 0; i < k; i++)
    {
        char *part[10000], *part2[10000];
        strcpy(ogcom, commands[i]);
        int oldout, oldin, ofd, ifd, r = 0;

        part[0] = strtok(ogcom, " \t\n\r"); //separating the commands
        ll ii = 0;
        while (part[ii] != NULL)
        {
            ii++;
            part[ii] = strtok(NULL, " \t\r\n");
        }

        if (i % 2 != 0)
        {
            int z = pipe(mypipe2);
            if (z < 0)
                perror("Error: pipe could not be created\n");
        }
        else
        {
            int z = pipe(mypipe);
            if (z < 0)
                perror("Error: pipe could not be created\n");
        }

        pid_t pid = fork();
        if (pid == 0)
        {
            if (i == 0)
            {
                dup2(mypipe[1], 1); // output to pipe
                close(mypipe[0]);   //closing input end of pipe
                strcpy(temp, commands[i]);
            }
            else if (i == k - 1)
            {
                if (i % 2 == 1)
                {
                    // printf("came into odd end\n");
                    dup2(mypipe[0], 0); //Input from pipe
                }
                else
                {
                    // printf("came into even end\n");
                    dup2(mypipe2[0], 0); //input from transit
                }
            }

            else if (i % 2 == 0)
            {
                // printf("came into even middle\n");
                dup2(mypipe2[0], 0); //input from transit
                close(mypipe[0]);    // close input end of pipe
                dup2(mypipe[1], 1);  // output to pipe
            }

            else if (i % 2 == 1)
            {
                // printf("came into odd middle\n");
                dup2(mypipe[0], 0);  //input from pipe
                close(mypipe[1]);    // close output end of pipe
                dup2(mypipe2[1], 1); // output to transit
            }

            // printf("VALUEEE OFFF RR = %d\n", r);

            strcpy(temp, commands[i]);
            r = redirection_check(temp);
            if (r == 1 || r == 2)
                redirection(part, ii, commands[i], r);

            // printf("now from exec\n");
            else
            {
                int z = execvp(part[0], part); // exec
                if (z < 0)
                    perror("Error: command not found\n");

                if (r == 0)
                    exit(0);
            }
            exit(0);
        }
        else
        {
            wait(NULL);

            if (i == 0)
            {
                close(mypipe[1]);
                if (inp == 1)
                {
                    dup2(oldin, 0);
                    close(ifd);
                }
            }
            else if (i == k - 1)
            {
                if (i % 2 == 0)
                    close(mypipe2[0]);
                else
                    close(mypipe[0]);

                if (outp == 1)
                {
                    dup2(oldout, 1);
                    close(ofd);
                }
            }
            else if (i % 2 == 0)
            {
                close(mypipe2[0]);
                close(mypipe[1]);
            }
            else if (i % 2 == 1)
            {
                close(mypipe[0]);
                close(mypipe2[1]);
            }
        }
    }
}

//********************************** redirection *****************************************************

int status1;

void choice(int fds[], char *from[], int k, int oldin, int oldout)
{
    if (strcmp(from[k - 1], "&") == 0) // FOR BACKGROUND PROCESSES
    {
        from[k - 1] = NULL;
        back(from);
    }
    else if (from[0][0] == '.') // FOR EXECUTABLES
        foreground(from);
    else if (strcmp(from[0], "pwd") == 0) //COMMAND : PWD
        pwd();
    else if (strcmp(from[0], "cd") == 0) //COMMAND : CD
    {
        cd(from, k);
    }

    else if (strcmp(from[0], "echo") == 0) //COMMAND : ECHO
    {
        echo(token, k);
    }
    else if (strcmp(from[0], "ls") == 0) // COMMAND :LS
    {
        Trigger_ls(k, from);
    }
    else if (strcmp(from[0], "pinfo") == 0) //COMMAND :PINFO
        pinfo(k, from);
    else if (strcmp(from[0], "vi") == 0 || strcmp(from[0], "emacs") == 0 || strcmp(from[0], "gedit") == 0) // COMMAND : VI, EMACS, GEDIT FOREGROUND
        foreground(from);
    else if (strcmp(from[0], "code") == 0) // TRIAL COMMAND FOR VSCODE
        foreground(from);
    else if (strcmp(from[0], "history") == 0) //HISTORY
    {
        history_command(k, token);
        //history_print();
        //his_check(from[0]);
    }
    else if (strcmp(from[0], "jobs") == 0)
        alljobs();
    else
    {
        int z = execvp(from[0], from);
        if (z < 0)
        {
            perror("myshell:Error\n");
            dup2(oldout, 1);
            close(fds[0]);
        }
    }
}

void both(char com[], ll app)
{
    char *command[100000], to[10000], *from[10000], *temp1[100000], *temp2[100000], *temp3[100000], input_file[100000], delim[10], output_file[100000];
    strcpy(output_file, "");
    strcpy(input_file, "");
    int fds[10];
    for (ll i = 0; i < 10; i++)
        fds[i] = -1;
    ll k = 0;
    if (app == 1)
        strcpy(delim, ">>");
    else
        strcpy(delim, ">");
    char *chk = strtok(com, delim);
    while (chk != NULL)
    {
        command[k++] = chk;
        chk = strtok(NULL, delim);
    }

    if (k == 2)
    {
        strcpy(to, command[1]); //OUTPUT FILE
        temp1[0] = strtok(to, " \t\n\r");
        strcpy(output_file, temp1[0]);
    }
    // printf("output file %s\n", output_file);

    k = 0;
    temp2[0] = strtok(command[0], "<");
    while (temp2[k] != NULL)
    {
        k++;
        temp2[k] = strtok(NULL, "<");
    }
    if (k == 2)
    {
        strcpy(input_file, temp2[1]); //INPUT FILEEEE
        temp3[0] = strtok(input_file, " \t\n\r");
        strcpy(input_file, temp3[0]);
    }
    // printf("input file = %s\nk=%d\n", input_file, k);
    k = 0;
    from[0] = strtok(temp2[0], " \t\n\r");
    while (from[k] != NULL)
    {
        k++;
        from[k] = strtok(NULL, " \t\n\r");
    }

    // printf("%s-%s-%s-\n", from[0], input_file, output_file);

    if (strcmp(input_file, "") != 0 && strcmp(output_file, "") == 0) // HAS INPUT REDIRECTION ONLY
    {
        // printf("came into input\n");
        int oldin = dup(STDIN_FILENO);
        int pid = fork();
        fds[0] = open(input_file, O_RDONLY);
        if (fds[0] < 0)
            perror("File doesn't exist\n");
        if (pid == 0)
        {
            if (fds[0] < 0)
                ;
            else
            {
                dup2(fds[0], 0);
                choice(fds, from, k, oldin, -1);
                dup2(oldin, 0);
            }
            exit(0);
        }
        else
            while (wait(&status1) != pid)
                ;
    }
    else if (strcmp(output_file, "") != 0 && strcmp(input_file, "") == 0) //HAS OUTPUT REDIRECTION ONLY
    {
        // printf("Came into output\n");
        int oldout = dup(STDOUT_FILENO);
        int pid = fork();

        if (app == 0)
            fds[0] = open(output_file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
        else if (app == 1)
            fds[0] = open(output_file, O_APPEND | O_WRONLY | O_CREAT, 0644);
        dup2(fds[0], 1);

        if (pid == 0)
        {
            // printf("Going to choice\n");
            choice(fds, from, k, -1, oldout);
            // printf("did choice\n");
            // dup2(oldout, 1);
            // close(fds[0]);

            exit(0);
        }
        else
        {
            while (wait(&status1) != pid)
                ;

            dup2(oldout, 1);
            close(fds[0]);
        }
    }
    else
    {
        // printf("Came into both\n");
        int oldin = dup(STDIN_FILENO);
        int oldout = dup(STDOUT_FILENO);
        int pid = fork();

        fds[0] = open(input_file, O_RDONLY);
        if (app == 0)
            fds[1] = open(output_file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
        else if (app == 1)
            fds[1] = open(output_file, O_APPEND | O_WRONLY | O_CREAT, 0644);
        dup2(fds[1], 1);
        if (fds[0] < 0)
            perror("File doesn't exist\n");
        if (pid == 0)
        {
            if (fds[0] < 0)
                ;
            else
            {
                dup2(fds[0], 0);
                choice(fds, from, k, oldin, oldout);
                dup2(oldin, 0);
            }
            exit(0);
        }
        else
        {
            while (wait(&status1) != pid)
                ;

            dup2(oldout, 1);
            close(fds[0]);
        }
    }
}

void redirection(char *token[], ll k, char list_com[], int redflag)
{
    ll app = 0, inp = 0, out = 0, a = 0;
    for (ll i = 0; i < k; i++)
    {
        if (strcmp(token[i], ">") == 0)
            out += 1;
        else if (strcmp(token[i], "<") == 0)
            inp += 1;
        else if (strcmp(token[i], ">>") == 0)
        {
            app = 1;
            a += 1;
        }
    }
    both(list_com, app);
}

//*********************************** bg ************************************************************

void bg(char *token[])
{
    ll num = atoi(token[1]);
    ll flag = 0;
    for (ll i = 1; i <= job_count; i++)
    {
        if (i == num)
        {
            flag = 1;
            kill(job_arr[num].pid, SIGTTIN);
            kill(job_arr[num].pid, SIGCONT);
        }
    }
    if (flag == 0)
        printf("Error: No such job found\n");
}

//************************************* fg ***************************************************

void fg(char *token[])
{
    ll num = atoi(token[1]);
    ll flag = 0;
    for (ll i = 1; i <= job_count; i++)
    {
        if (i == num)
        {
            int status;

            signal(SIGTTIN, SIG_IGN);
            signal(SIGTTOU, SIG_IGN);

            tcsetpgrp(STDIN_FILENO, job_arr[num].pid);

            flag = 1;

            current_fore.pid = job_arr[num].pid;

            strcpy(current_fore.name, job_arr[num].name);
            kill(job_arr[num].pid, SIGCONT);
            del_process(job_arr[num].pid); // DELETING THE PROCESS FROM THE BACKGROUND PROC ARRAY
            waitpid(-1, NULL, WUNTRACED);  // WAITING FOR THAT PROCESS TO FINISH EXECUTION

            tcsetpgrp(STDIN_FILENO, getpgrp());

            signal(SIGTTIN, SIG_DFL);
            signal(SIGTTOU, SIG_DFL);
        }
    }
    if (flag == 0)
        printf("Error: No such process found\n");
}

//*********************************** kjob ***********************************************************

void kjob(char *token[], ll k)
{
    ll number, signo;

    number = atoi(token[1]);
    signo = atoi(token[2]);

    ll flag = 0;
    if (k != 3)
        printf("Error: Incorrect number of arguments\n");
    else
    {
        if (signo == 9)
            kjobkill = 1;
        if (number <= job_count && number > 0)
            kill(job_arr[number].pid, signo);
        else
            printf("Error : No such process found\n");
    }
}

//************************************ jobs *******************************************************

void alljobs()
{
    for (ll i = 1; i <= job_count; i++)
    {
        ll pid = job_arr[i].pid;
        char name[10000], file[10000], status, s[100000];
        strcpy(name, job_arr[i].name);

        sprintf(file, "/proc/%lld/stat", pid);
        FILE *fd;
        fd = fopen(file, "r");
        if (fd < 0)
            perror("Error: no such process exists\n");
        else
        {
            fscanf(fd, "%*d %*s %c %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d", &status);
            fclose(fd);
        }

        if (status == 'T')
            strcpy(s, "Stopped");
        else
            strcpy(s, "Running");
        if( k == 1 )
            printf("[%lld] %s %s [%lld]\n", i, s, name, pid);
        else if(strcmp(token[1], "-s") == 0 && strcmp(s, "Stopped") == 0)
            printf("[%lld] %s %s [%lld]\n", i, s, name, pid);
        else if(strcmp(token[1], "-r") == 0 && strcmp(s, "Running") == 0)
            printf("[%lld] %s %s [%lld]\n", i, s, name, pid);
    }
}

//**************************************** overkill **************************************************

void overkill()
{
    for (ll i = 1; i <= job_count; i++)
    {
        kill(job_arr[i].pid, 9);
    }

    overkillflag = 1;
}