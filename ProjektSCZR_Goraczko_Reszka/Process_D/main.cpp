#include "menu.h"
#include <iostream>
#include <ctime>
#include "../datachunk.h"
#include <mqueue.h>
using namespace std;

int main(int argc, char * argv[])	//argv[1] - priorytet A; argv[2] - priorytet B; argv[3] - priorytet C; argv[4] - co ile logów następuje ich zapis do plików testowych
{
    if (argc != 5) return -1;

    char* arg[4];
    for (int i=0; i<4; ++i)
        arg[i]= argv[i+1];

    mq_unlink(PROCESS_B_QUEUE_NAME);
    mqd_t mqB;
    struct mq_attr attrB;
    attrB.mq_flags = 0;
    attrB.mq_maxmsg = 50;
    attrB.mq_msgsize = sizeof(DataChunk);
    attrB.mq_curmsgs = 0;
    mqB = mq_open(PROCESS_B_QUEUE_NAME, O_CREAT | O_EXCL, 0777, &attrB);
    if(mqB < 0){
        fprintf(stderr, "%s:%d: ", __func__, __LINE__);
        perror("Błąd utworzenia kolejki B");
    }
    mq_unlink(PROCESS_C_QUEUE_NAME);
    mqd_t mqC;
    struct mq_attr attrC;
    attrC.mq_flags = 0;
    attrC.mq_maxmsg = 50;
    attrC.mq_msgsize = sizeof(DataChunk);
    attrC.mq_curmsgs = 0;
    mqC = mq_open(PROCESS_C_QUEUE_NAME, O_CREAT | O_EXCL, 0777, &attrC);
    if(mqC < 0){
        fprintf(stderr, "%s:%d: ", __func__, __LINE__);
        perror("Błąd utworzenia kolejki C");
    }
    Menu menu;
    menu.mainMenu(arg);
    mq_unlink(PROCESS_B_QUEUE_NAME);
    mq_unlink(PROCESS_C_QUEUE_NAME);
    return 0;
}
