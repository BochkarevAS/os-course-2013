//������������ 6
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define RESEARCHES_COUNT 4
#define RESEARCH_NAME_LENGTH 32
#define RESEARCH_DATE_LENGTH 10
#define RESEARCH_LENGTH (RESEARCH_NAME_LENGTH + RESEARCH_DATE_LENGTH + 2)

int main(int argc, char* argv[], char* env[]) {
    if (argc != 2 || (strcmp(argv[1], "set") != 0 && strcmp(argv[1], "find") != 0)){
        printf("������� ������ ���� ������� � ���������� set ��� find\n");
        return -1;
    }
        
    char *array;
    int shmid;
    int new = 1;
    char pathname[] = "lab06.c";

    key_t key;   
    if((key = ftok(pathname,0)) < 0){
        printf("Can\'t generate key\n");
        exit(-1);
    }

    if((shmid = shmget(key, RESEARCHES_COUNT*RESEARCH_LENGTH*sizeof(char), 0666|IPC_CREAT|IPC_EXCL)) < 0){
        if(errno != EEXIST){
            printf("Can\'t create shared memory\n");
            exit(-1);
        } else {
            if((shmid = shmget(key, RESEARCHES_COUNT*RESEARCH_LENGTH*sizeof(char), 0)) < 0){
                printf("Can\'t find shared memory\n");
                exit(-1);
            }
            new = 0;
        }
    }


    if((array = (char*)shmat(shmid, NULL, 0)) == (char*)(-1)){
        printf("Can't attach shared memory\n");
        exit(-1);
    }

    if (strcmp(argv[1], "find") == 0) {
        if (new){
            printf("���� �������� �� ���������. ����� ������ ��������� � ����� ����������.\n");
            strcpy(argv[1], "set");
        } else {
            char name[RESEARCH_NAME_LENGTH];
            memset(name, '\0', RESEARCH_NAME_LENGTH);
            printf("������� �������� ��������: ");
            scanf("\n%[^\r\n]",name);
            int i;
            int result = 0;
            for(i = 0; i < RESEARCHES_COUNT && !result; i++){
                char tempname[RESEARCH_NAME_LENGTH];
                memset(tempname, '\0', RESEARCH_NAME_LENGTH);
                strncpy(tempname, array+(i*RESEARCH_LENGTH)*sizeof(char), RESEARCH_NAME_LENGTH);
                if (strcmp(tempname, name) == 0) {
                    result = 1;
                    char tempdate[RESEARCH_DATE_LENGTH];
                    memset(tempdate, '\0', RESEARCH_DATE_LENGTH);
                    strncpy(tempdate, array+(i*RESEARCH_LENGTH+RESEARCH_NAME_LENGTH+1)*sizeof(char), RESEARCH_DATE_LENGTH+1);
                    printf(": %s\n",tempdate);
                    printf("���� �������� [%s] : %s\n",tempname,tempdate);
                }
            }
            if (!result)
                printf("�������� �� �������.\n");                
        }
    }
    
    if (strcmp(argv[1], "set") == 0) {
        char name[RESEARCH_NAME_LENGTH];
        char date[RESEARCH_DATE_LENGTH];
        int i;
        for(i = 0; i < RESEARCHES_COUNT; i++){
            /*���� ���������� ����%*/
            memset(name, '\0', RESEARCH_NAME_LENGTH);
            memset(date, '\0', RESEARCH_DATE_LENGTH);
            printf("������� �������� �������� %d: ", i);
            scanf("\n%[^\r\n]",name);
            printf("������� ���� �������� [%s]: ", name);
            scanf("\n%[^\r\n]",date);
            strncpy(array+(i*RESEARCH_LENGTH)*sizeof(char), name, RESEARCH_NAME_LENGTH);
            strncpy(array+(i*RESEARCH_LENGTH+RESEARCH_NAME_LENGTH)*sizeof(char), "\0", RESEARCH_NAME_LENGTH);
            strncpy(array+(i*RESEARCH_LENGTH+RESEARCH_NAME_LENGTH+1)*sizeof(char), date, RESEARCH_DATE_LENGTH);
            strncpy(array+((i+1)*RESEARCH_LENGTH-1)*sizeof(char), "\0", RESEARCH_DATE_LENGTH);
            /*%���� ���������� ����*/
        }
        printf("������������ ������� ���������!\n");
    }
    
    printf("---\n������ ����:\n");
    int j = 0;
    for (j = 0; j < RESEARCHES_COUNT*RESEARCH_LENGTH; j++) {
        if (j % RESEARCH_LENGTH == 0)
            printf("\n");
        if(array[j] == 0)
            printf("*");
        else printf("%c",array[j]);
    }printf("\n");
    

    if(shmdt(array) < 0){
        printf("Can't detach shared memory\n");
        exit(-1);
    }
    return 0;
}