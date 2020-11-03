#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


typedef struct myData{
    int num;
    int result;
}mydata;

void *printHello(void *value);
void *returnData(void *value);
void *returnStruct(void * myData);

/*
Example 1
Most basic Pthread, takes in a single argument (threadNum) and prints it
*/
void *printHello(void *value){
    long value2 = (long)value;
    printf("Hellow World, thread %ld\n", value2);
    pthread_exit(NULL);
}

/*
Example 2
Return a value from threads
3 common ways:
    1) make a struct with extra space to return the data
    2) return the data from the pointer returned from pthread_exit()
    3) thread creates a struct to hold return values
Returning a value requires pthread_join
*/
void *returnData(void* value){
    //always have to reassign value to avoid overrwriting a value another pointer uses
    long a = *(int*)value;
    printf("value of a: %ld\n", a);

    int *result = (int*)malloc(4);
    *result = a + a;

    //when moving data out of bloc scope, better to declare variables as correct pointer data type
    //when done properly the return is of a normal variable, not &result
    pthread_exit(result);
}
/*
Example 3
Pass and Return a struct
Pass by reference is much faster, returning a pointer is much smaller than the data itself
*/
void *returnStruct(void * input){

    mydata *ptr = (mydata *)input;
    ptr->result = ptr->num * ptr->num;
    pthread_exit(NULL);
}   


int main (int argc, char *argv[]){

    pthread_t pthread_1;


    //Example 1
    long value = 4;
    pthread_create(&pthread_1, NULL, printHello, (void *)value);

    //Example 2
    void *returnedValue;
    int number = 6;
    pthread_create(&pthread_1, NULL, returnData, &number);
    pthread_join(pthread_1, &returnedValue);
    int returned = *((int *)returnedValue);
    //says (int*) = we expect a int pointer
    //*returned = we derefence the pointer
    printf("Returned Value of a: %d", returned);
    
    //Example 3
    mydata d1;
    d1.num = 4;
    pthread_create(&pthread_1, NULL, returnStruct, (void *)&d1);
    pthread_join(pthread_1, NULL);
    printf("Struct returns value of %d\n", d1.result);

    

    pthread_exit(NULL);
}

