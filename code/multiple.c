#include <stdio.h>
#include <stdlib.h>

#define PAGES 100
#define MAXPROC 10
//#define FRAMES 4

#define length 250
int numProc = 0;
int head = 0;
int tail = 0;

struct pcb
{
    char tmp[length];
    int origLen;
    int len;
    int pid;
    int numFaults;
    int completed;//1 means completed 0 means not completed
    int timeDone;

} proc[MAXPROC];

void CreateProcess(struct pcb *pcb, int num)
{
    if(num == 0)//part a
    {
        printf("PART A\n");
        int r;
        char c;

        int num;
        if(length < PAGES)
        {
                num = 1;
                //printf("Num: %d\n", num);
        }
        else
        {
                num = length/PAGES;
                //printf("Num: %d\n", num);
        }

        int i;
        int j=0;
        for(i=0; i<length; i++)
        {
                //ref[i] = (char)i;
                //ref[i] = itoa(i, ref[i], 10);

                if(i%num == 0)
                {
                j++;
                //printf("J: %d\n", j);
                }

                int r = (rand() % 15) + 1 + j;
                if(r > 100)
                {
                        r = 100;
                }

                c = (char)r;

                pcb->tmp[i] = c;
                //printf("Value: %d\n", pcb->tmp[i]);
        }
    }
    else if(num == 1)//part b
    {
        printf("PART B\n");
        char c;
        int r = 0;

        int i;
        for(i=0; i<length; i++)
        {
                int again = (rand()%3);
                if(again == 1)
                {
                        r = (rand()%100)+1;
                }
                if(again == 2 && r>5)
                {
                        r = r-again;
                }

                //check edge values
                if(r>100)
                {
                        r=100;
                }
                if(r==0)
                {
                        r=1;
                }

                c = (char)r;

                pcb->tmp[i] = c;
                //printf("Value: %d\n", pcb->tmp[i]);
        }
    }
    else if(num == 2)//part c
    {
        printf("PART C\n");
        int r = 0;
        char c;
    
        int num;
        if(length < PAGES)
        {
            num = 1;
        }
        else
        {
            num = length/PAGES;
        }
    
        int i;
        int j=0;
        for(i=0; i<length; i++)
        {  
            if(i%num == 0)
            {
                j++;
            }
        
            int again = (rand() % 3);
            if(again == 1)
            {
                r = (rand() % 15) + 1 + j;
            }
            if(again == 2 && r > 5)
            {
                r = r-again;
            }

            //check edge values
            if(r > 100)
            {
                r = 100;
            }
            if(r == 0)
            {
                r = 1;
            }

            c = (char)r;

            pcb->tmp[i] = c;
            //printf("Value: %d\n", pcb->tmp[i]);
        }
    }
    else if(num == 3)//part d
    {
        printf("PART D\n");
        char c;
        int r = 0;

        int i = 0;
        for(i=0; i<length; i++)
        {
            r = (rand() % 100);

            if(r > 100)
            {
                r = 100;
            }
            if(r == 0)
            {
                r = 1;
            }

            c = (char)r;

            pcb->tmp[i] = c;
            //printf("Value: %d\n", pcb->tmp[i]);
        }
    }

    pcb->pid = numProc;
    numProc++;
    pcb->completed = 0;
    pcb->numFaults = 0;
    pcb->origLen = length;
    pcb->len = length;

}

int FIFO(struct pcb *pcb, int frames, char queue[])
{
    struct pcb  stateCopy;
    stateCopy = *pcb; 

    //printf("PID: %d\n", stateCopy.pid);
    int cnt, loopCnt;
    typedef enum { false, true } bool;
    bool PgFault;

    //printf("len: %d\n", stateCopy.len);
    int i = ((stateCopy.origLen)-(stateCopy.len));
    //printf("I: %d\n", i);
    //printf("completed: %d\n", stateCopy.completed);
    if(i==0)
    {
        stateCopy.numFaults++;
        queue[head] = stateCopy.tmp[i];
        //printf("Page Fault\n");
    }

    cnt = head;
    PgFault = true;

    loopCnt = 0;
    while(loopCnt < frames && PgFault == true && i!=0)
    {
        //printf("In while loop\n");
        if(queue[cnt] == stateCopy.tmp[i])
        {
            //printf("No Page Fault\n");
            PgFault = false;
        }
        cnt = (cnt+1)%frames;
        loopCnt++;
    }

    if(PgFault == true && i!=0)
    {
        //add to tail and take from head
        if(stateCopy.numFaults>=frames)
        {
            //printf("Page Fault\n");
            //take from head
            head = (head+1)%frames;
            //add to tail
            tail = (tail+1)%frames;
            queue[tail] = stateCopy.tmp[i];
            stateCopy.numFaults++;
        }

        //add to tail
        if(stateCopy.numFaults<frames && i!=0)
        {
            //printf("Page Fault\n");
            tail = (tail+1)%frames;
            queue[tail] = stateCopy.tmp[i];
            stateCopy.numFaults++;
        }
           
    }
    stateCopy.len--;
    if(stateCopy.len == 0)
    {
        stateCopy.completed = 1;
    }
    //printf("Number of Faults: %d\n", stateCopy.numFaults);
    *pcb = stateCopy;
    return pcb->numFaults;
}

void printQueue(char ary[], int len)
{
    int i;
    for(i = 0; i<len; i++)
    {
        printf("Value: %d\n", ary[i]);
    }
}

main (int ac, char *av[])
{
    int totalProc = atoi(av[1]);
    int FRAMES = atoi(av[2]);
    char queue[FRAMES];
    
    int l;
    for(l=0; l<totalProc; l++)
    {
        struct pcb* MyProc = NULL;
        int type = (rand() % 4);
        CreateProcess(&proc[l], type);
    }

    int cSwitch = 0;
    int terminated = 0, ptr = -1;
    int myClock = 0;
    int totalTime = 0;
    //for(myClock = 0; terminated = 0; myClock++)
    while(terminated != 1)
    {
        if(myClock%10 == 0)
        {
            if(ptr == (totalProc-1))
            {
                ptr = 0;
            }
            else
            {
                ptr++;
            }
            cSwitch++;
        }
        else
        {
            if(proc[ptr].completed == 0)
            {
                //call fifo
                //printf("Fifo Called\n");
                //printQueue(queue, 3);
                FIFO(&proc[ptr], FRAMES, queue);
            }
            else
            {
                proc[ptr].timeDone = totalTime;
                myClock = 0;
                if(ptr == (totalProc-1))
                {
                    ptr = 0;
                }
                else
                {
                    ptr++;
                }
                cSwitch++;
            }
        }
        totalTime++;
        myClock++;
        int i = 0;
        int check = 0;
        //checks to see if all processes are completed
        while(i<numProc && check == 0)
        {
            if(proc[i].completed == 0)
            {
                check = 1;
            }
            if(proc[i].completed == 1 && i == (numProc-1))
            {
                terminated = 1;
            }
            i++;
        }
    }
    
    proc[ptr].timeDone = totalTime;
    printf("Total Time: %d\n", totalTime);
    printf("Total Number of Context Switches: %d\n", cSwitch);
    int k;
    for(k=0; k<totalProc; k++)
    {
        printf("Process: %d\n", proc[k].pid);
        printf("Number of Faults: %d\n", proc[k].numFaults);
        printf("Time Completed: %d\n", proc[k].timeDone);
    }
    
}