
/* header.h */
#define KEY  ((key_t)(12970)) /*change it to last five digits of your SID*/
#define SEGSIZE sizeof(struct StudentInfo)

#define NUM_SEMAPHS 5
#define SEMA_KEY   ((key_t)(44450)) /* change this to last four digits of SID */

#define STD_SEGSIZE 55*(sizeof(struct StudentInfo))
#define READ_SEGSIZE sizeof(struct ReadCounter)

struct StudentInfo {
    char Name[80];
    char telNumber[80];
    char address[80];
    char StudentId[80];
    char whoModified[10];
};

struct ReadCounter {
  int readCount;
};

void Wait(int semaph, int n);
void Signal(int semaph, int n);
int GetSemaphs(key_t k, int n);