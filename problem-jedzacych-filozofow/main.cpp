#include <iostream>
#include <pthread.h>
#include <atomic>
#include <unistd.h>

int NUM_PHILOSOPHERS;

// critical section - before philosopher eats, two next to him have to think (picking up and putting down forks)
// thread represents a philosopher

pthread_mutex_t* forks;

void* philosopher(void* arg) {
    int id = *(int*)arg; // dereference to get id
    delete (int*)arg; // delete id
    int left = id;
    int right = (id + 1) % NUM_PHILOSOPHERS; // mod for N-1 case, i.e. N=5, 4's right fork is 0
    
    while (true) {
        printf("Philosopher %d is thinking...\n", id);
        usleep(1000000); // Sleep for 1000000 microseconds (1 second)

        // Asymetric solution preventing deadlock - odd philosophers pick left fork first, even pick right first
        // deadlock - all philosophers picking left/right fork at the same time
        if (id % 2 == 0) {
            pthread_mutex_lock(&forks[right]);
            pthread_mutex_lock(&forks[left]);
        } else {
            pthread_mutex_lock(&forks[left]);
            pthread_mutex_lock(&forks[right]);
        }

        printf("Philosopher %d is eating...\n", id);
        usleep(1000000);

        printf("Philosopher %d is done eating.\n", id);

        pthread_mutex_unlock(&forks[left]);
        pthread_mutex_unlock(&forks[right]);
    }
    return nullptr;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <number_of_philosophers>\n";
        return 1;
    }
    NUM_PHILOSOPHERS = std::stoi(argv[1]); // convert command line arg to int

    pthread_t* philosophers = new pthread_t[NUM_PHILOSOPHERS]; // array of philosopher threads

    forks = new pthread_mutex_t[NUM_PHILOSOPHERS]; // array of forks
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_init(&forks[i], nullptr);
    }

    // create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        int* id = new int(i); // dynamic id array to prevent race conditions
        // create a new thread for every philosopher in pthread_t* philosophers array. 
        // nullptr default attributes are used. 
        // thread will execute philosopher function.
        // &ids[i] allows thread to know which philosopher it represents
        pthread_create(&philosophers[i], nullptr, philosopher, id);
    }

    // join threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], nullptr);
    }

    // cleanup
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_destroy(&forks[i]);
    }

    delete[] philosophers;
    delete[] forks;

    return 0;
}
