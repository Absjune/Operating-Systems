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

    //Allocate shared memory
    Shared = shm_open("table",create | O_RDWR, 0666); //Create table
    ftruncate(Shared, sizeof(int)); //Setting the size
    table = static_cast<int*>(mmap(0,sizeof(int),protRead | protWrite, mapShareMem, Shared, 0)); //Mapping object to address
    sem_t* full = sem_open("full",create,0666,0);
    sem_t* empty = sem_open("empty",create,0666,3);
    sem_t* mutex = sem_open("mutex",create, 0666, 1);

    std::cout << "Process is running!" << std::endl;

    for (int i = 0; i < 5; ++i) {
        semaphoreWait(empty);
        sleep(1); //1 sec

        semaphoreWait(mutex); // unlock
        if (*table < 2) { // if table does not have 2 items
            ++(*table);    // add new item in the table
            std::cout << "Produced an item." << std::endl << "Table contains: " << *table << " items";
        }
        else {
            std::cout << "Table is full!";
        }
        semaphorePost(mutex); // close
        semaphorePost(full);
    }
    sleep(3); //1 sec
    std::cout << "Press enter to exit the completed  process." << std::endl;

    //Closing semaphores
    semaphoreClose(full);
    semaphoreClose(empty);
    semaphoreClose(mutex);

    //Unlink semaphores
    semaphoreUnlink("full");
    semaphoreUnlink("empty");
    semaphoreUnlink("mutex");

    //Deallocate the shared memory
    munmap(table, sizeof(int));
    close(Shared);
    shm_unlink("table");
    return 0;
}
