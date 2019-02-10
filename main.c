#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define min(a,b) (((a) < (b)) ? (a) : (b))

typedef struct{
   pthread_mutex_t myMutex;
   int waitingPassengers;
   int lsaM23dosh;
   pthread_cond_t allSeated;
   pthread_cond_t fehAmakn;

}station;

void station_on_board(station *curStation){

     pthread_mutex_lock(&curStation->myMutex);
     curStation->lsaM23dosh--;
     pthread_mutex_unlock(&curStation->myMutex);

     if(curStation->lsaM23dosh == 0){
     /// all seats are taken or all sat down
         pthread_cond_signal(&curStation->allSeated);
     }
}

void station_wait_for_train(station *curStation){
     /// only one passenger can enter
     pthread_mutex_lock(&curStation->myMutex);
     curStation ->waitingPassengers++;

     pthread_cond_wait(&curStation->fehAmakn,&curStation->myMutex);

     curStation->waitingPassengers--;
     curStation ->lsaM23dosh++;

     pthread_mutex_unlock(&curStation->myMutex);
     station_on_board(curStation);
}

int train_number = 1;
void station_load_train(station *curStation, int count){
       int savedCount = count;

       pthread_mutex_lock(&curStation->myMutex);

       printf("wohoooooo train %d arrived with %d seat(s) \n\n" , train_number++,count);

       while(count >0 && curStation->waitingPassengers >0){
             pthread_cond_signal(&curStation->fehAmakn);
             count--;
       }


       if(curStation->lsaM23dosh >0)
           pthread_cond_wait(&curStation->allSeated , &curStation->myMutex);


       printf("\nthe train is leaving with %d passenger(s) on board\n\n" , savedCount-count);

       pthread_mutex_unlock(&curStation->myMutex);

       /// train leaves
}

station *curStation = NULL;

void station_init(){
     curStation = (station*)malloc(sizeof(station));
     pthread_mutex_init(&curStation->myMutex,NULL);
     pthread_cond_init(&curStation->allSeated,NULL);
     pthread_cond_init(&curStation->fehAmakn,NULL);
     curStation->lsaM23dosh  = curStation->waitingPassengers = 0;
}

void station_close(){
     pthread_mutex_destroy(&curStation->myMutex);
     free(curStation);
}

pthread_t passengers[100] , train;

void *add_passenger(void *args){
    int *cur = (int*)args;
    printf("\npassenger %d is in queue !\n",*cur);
    station_wait_for_train(curStation);
    printf("\npassenger %d is on board have a safe trip xD \n",*cur);
}


void *add_train(void *args){
    int *seats = (int*)args;
    station_load_train(curStation , *seats);
}

int dummy[100];

int main()
{

    /// contains random tests
    /// users are threads
    station_init();

    /// adding passengers
    int i;
    for(i = 0 ;i < 10 ;i++){
        dummy[i] = i;
        pthread_create(&passengers[i] , NULL , add_passenger , &dummy[i]);
    }

    /// a train arrives
    sleep(1);
    int seats = 2;
    pthread_create(&train,NULL,add_train,&seats);

    sleep(1);

    seats = 5;
    pthread_create(&train,NULL,add_train,&seats);

    sleep(1);

    for(i = 10 ;i < 20 ;i++){
        dummy[i] = i;
        pthread_create(&passengers[i] , NULL , add_passenger , &dummy[i]);
    }

    sleep(1);
    seats = 6;
    pthread_create(&train,NULL,add_train,&seats);

    sleep(1);

    seats = 3;
    pthread_create(&train,NULL,add_train,&seats);

    station_close();

    return 0;
}
