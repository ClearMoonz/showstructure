#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
char x[2048]={"C:\\Users\\ClearMoon\\1"};
char y[3]={"\\"};
void newscreen()
{
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}
int FileRead(const char *file,char **buff) {
    const int BUFFSIZE=256;
    FILE *fp = fopen(file, "r");
    if (fp == NULL) {
        printf("file doesn't exist\n");
        return 0;
    } else {
        int curSize = 1 + BUFFSIZE;
        *buff = malloc(curSize * sizeof(char));
        char *anchor = *buff;
        int readB = 0;
        while ((readB = fread(anchor, sizeof(char), BUFFSIZE, fp)) != 0) {
            if (readB == BUFFSIZE) {
                curSize += BUFFSIZE;
                *buff = realloc(*buff, curSize * sizeof(char));
                anchor = *buff + curSize - BUFFSIZE - 1;
                *anchor=0;
            }
        }
    }
    return 1;
}
int checkFileType(const char *file){
    /**
 * @author 201701272
 * @brief check if file exist(not zero) or is a dir(1) other(2)
 * @param file
 * @return  0 for not exist, 1 for dir , 2 for other
 */
    int mode = 0;/// access check if file exist
    int not_exi = access(file,mode);
    if(not_exi){
        return 0;
    }
    struct stat file_stat ={0};
    stat(file,&file_stat);
    if(S_ISDIR(file_stat.st_mode)){
        return 1;
    } else{
        return 2;
    }
}
int tree(const char dir[],char *** dir_out)
{
    struct FileList
    {
        char filename[64];
        struct FileList *next;
    }start,*node;
    DIR *directory_pointer;
    struct dirent *entry;
    int cnt=0;
    int i=0;
    if ((directory_pointer=opendir(dir))==NULL)
        printf( "Error opening %s\n ",dir);
    else
    {
        start.next=NULL;
        node=&start;
        while ((entry=readdir(directory_pointer))!=NULL)
        {
            cnt++;
            node-> next=(struct FileList *)malloc(sizeof(struct FileList));
            node=node-> next;
            strcpy(node-> filename,entry-> d_name);
            node-> next=NULL;
        }
        closedir(directory_pointer);
        node=start.next;
        *dir_out = (char**)malloc(sizeof(char*)*cnt);
        while(node)
        {
            printf( " %s \n ",node-> filename);
            int lenth = strlen(node->filename);
            (*dir_out)[i] = (char*)malloc(sizeof(char)*(1+lenth));
            strcpy((*dir_out)[i],node->filename);
            node=node-> next;
            i++;
        }
        i--;
    }
    return i;
}
HANDLE hout;
void move(char *ppp[],int cnt) {
    hout = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hout, &csbi);
    coord.X = csbi.dwCursorPosition.X;
    coord.Y = csbi.dwCursorPosition.Y;
    coord.Y -= cnt;
    SetConsoleCursorPosition(hout, coord);
    int i = 1;
    int z;
    int ch;
    char **p=NULL;
    while (i >= 0 && i <= cnt) {
        printf("\r*");
        ch = _getch();
        while (ch == 0XE0 || ch == 0X0D)
        {
            printf("\r ");
            GetConsoleScreenBufferInfo(hout, &csbi);
            coord.X = csbi.dwCursorPosition.X;
            coord.Y = csbi.dwCursorPosition.Y;
            if(ch == 0x0d)
            {
                if(checkFileType(ppp[i])==1)
                {
                    strcat(x, y);
                    strcat(x, ppp[i]);
                    newscreen();
                    z=tree(x,&p);
                    move(p,z);
                }
                else if(checkFileType(ppp[i])==2)
                {
                    FileRead(ppp[i],NULL);
                }
            }
            ch = _getch();
            if (ch == 0x48)
            {
                if (coord.Y != 0)
                {
                    coord.Y--;
                    i--;
                }

            }
            else if (ch == 0x50)
            {
                coord.Y++;
                i++;
            }
            SetConsoleCursorPosition(hout, coord);
            printf("\r* %s",ppp[i]);
            ch = _getch();
        }
    }
}

int main()
{
    int z;
    char **ppp = NULL;
    //printf("the filepath:");
    //gets(dir);
    z=tree(x,&ppp);
    move(ppp,z);
    return 0;
}