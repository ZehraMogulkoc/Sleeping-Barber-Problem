#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/time.h>


void* barber();
void* customer(void*arg);
void set_duration(int time);

sem_t seats;
sem_t cus;
sem_t bar;
pthread_mutex_t cutting;

int num_customer;
int num_seats;
int max_arrival_time;;
int max_haricut_duration;
int haircut_repetition;
void* barber();
void* customer(void *arg);

int waiting_time;


int initialization(){
 pthread_mutex_init(&cutting, NULL);

    sem_init(&cus, 0, 0);
    sem_init(&bar, 0, 0);
    sem_init(&seats, 0, 1);
return 0;
}

void main(int argc,char *argv[]){

	 num_customer= atoi(argv[1]);
    if(num_customer%2==0 || num_customer>27){
        printf("Error: The number of philosopher is bigger than 27 or it is not an odd number.\n");
        exit(0);
    }
	max_arrival_time = atoi(argv[2]);
	max_haricut_duration = atoi(argv[3]);
	haircut_repetition = atoi(argv[4]);
	num_seats=atoi(argv[5]);

	
	pthread_t pid_barber;
        pthread_t customer_maker[num_customer];
		
	pthread_create(&pid_barber, NULL, (void*)barber,NULL);
	int id[num_customer];
	
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	for(int i =0; i< num_customer; i++){
		id[i] = i;
		pthread_create(&customer_maker[i], &attr, (void*)customer,(void*)&id[i]);
	}
	
	
	
	pthread_mutex_unlock(&cutting);
	pthread_join(pid_barber, NULL);
	
	
}
void* barber()
{

          sem_wait(&bar);     
          sem_wait(&seats);   
           
          num_seats++;  
         
         sem_post(&seats);
         sem_post(&cus);
         pthread_mutex_lock(&cutting);
         printf("Barber is working.\n");
         set_duration(max_haricut_duration);
         pthread_mutex_unlock(&cutting);
         sleep(3); 
     
    
}
void* customer(void *arg)
{
int time=0;
    int id = *((int *)arg);
        set_duration(max_arrival_time); //time it takes to arrive barber
        
         while(time<max_arrival_time)
    {
    if(num_seats == 0) { // if not, customer leaves
            printf("There is no available seats, customer leaving.\n");
            sem_post(&seats);
            break;
        }
        else if(num_seats>=1){
        num_seats=num_seats-1;
         printf("Customer %d is getting haircut now.\n",id);
         
         clock_t start = clock();
         sem_post(&bar);
         set_duration(max_haricut_duration);
          sem_post(&seats); 
          sem_wait(&bar);
          num_seats=num_seats+1;
          waiting_time=waiting_time+ (clock() - start) * 1000 / CLOCKS_PER_SEC;    
       
        sleep(1);
    }
    time=time+1;
    }
     printf("Customer %d is done with waiting time: %d\n",id,waiting_time);
    exit(0);
}

//crate random values for haircut duration and arrival time
void set_duration(int secs){
	srand(time(NULL));
	int arrive;
	arrive = rand()%(secs+1);
	arrive = arrive/1000;
	sleep(arrive);
}
