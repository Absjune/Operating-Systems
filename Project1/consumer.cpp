#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){
    int Shared;
    int* table;

    //Allocate the shared memory
    Shared = shareOpen("table",create | O_RDWR, 0666); //Create the table
    ftruncate(Shared, size(int)); //Setting the size of the shared memory
    table = static_cast<int*>(mmap(0,size(int),protRead | protWrite, mapShared, Shared, 0)); //Mapping object to address
    sem_t* full = sem_open("full",create,0666,0);
    sem_t* empty = sem_open("empty",create,0666,3);
    sem_t* mutex = sem_open("mutex",create, 0666, 1);

    for (int i = 0; i < 5; ++i){
        semaphoreWait(full);
        sleep(1); //Sleep for 1 second
        SemaphoreWait(mutex); //Unlock mutex
        if(*table > 0){
            --(*table); //CONSUME
            std::cout << "Item consumed" << std::endl << "There are " << *table << "items in the table." <<std::endl;
        }
        else{
            std::cout << "The table is empty!" << std::endl;
        }
        semaphorePost(mutex); //Close our mutex
        semaphorePost(empty);
    }

    //Closing semaphores
    semaphoreClose(full);
    semaphoreClose(empty);
    semaphoreClose(mutex);

    //Unlink semaphores
    semaphoreUnlink("full");
    semaphoreUnlink("empty");
    semaphoreUnlink("mutex");

    //Deallocate the shared memory
    munmap(table, size(int));
    close(Shared);
    shm_unlink("table");
    return 0;

}
