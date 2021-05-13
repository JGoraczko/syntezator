#include "menu.h"
#include <iostream>
#include <ctime>
#include "../datachunk.h"
#include <mqueue.h>
using namespace std;

int main(int argc, char * argv[])
{
    if (argc != 5) return -1;

    char* arg[4];
    for (int i=0; i<4; ++i)
        arg[i]= argv[i+1];

    mq_unlink(FIRST_QUEUE_NAME);
    mqd_t mq;
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 50;
    attr.mq_msgsize = sizeof(DataChunk);
    attr.mq_curmsgs = 0;
    mq = mq_open(FIRST_QUEUE_NAME, O_CREAT | O_RDONLY | O_EXCL, 0777, &attr);
    if(mq < 0){
        fprintf(stderr, "%s:%d: ", __func__, __LINE__);
        perror("Błąd utworzenia kolejki");
    }
    Menu menu;
    menu.mainMenu(arg);
    mq_unlink(FIRST_QUEUE_NAME);
    return 0;
}
