#include <iostream>
#include <pthread.h>
#include <atomic>
#include <unistd.h>

int NUM_PHILOSOPHERS;

// critical section - before philosopher eats, two next to him have to think (picking up and putting down forks)
// thread represents a philosopher

// lock using atomic bool to prevent race condition - two philosophers picking up the same fork
struct Spinlock {
    std::atomic<bool> state; // true = taken

    Spinlock() { 
        state.store(false); 
    }

    void pick_up() {
        // true = fork taken
        // keeps being taken until it's put down
        while (state.exchange(true, std::memory_order_acquire)); 
    }

    void put_down() { 
        state.store(false, std::memory_order_release); 
    }
};

Spinlock* forks;

void* philosopher(void* arg) {
    int id = *(int*)arg; // dereference to get id
    int left = id;
    int right = (id + 1) % NUM_PHILOSOPHERS; // mod for N-1 case, i.e. N=5, 4's right fork is 0
    
    while (true) {
        printf("Philosopher %d is thinking...\n", id);
        usleep(1000000); // Sleep for 1000000 microseconds (1 second)

        // Asymetric solution preventing deadlock - odd philosophers pick left fork first, even pick right first
        // deadlock - all philosophers picking left/right fork at the same time
        if (id % 2 == 0) {
            forks[right].pick_up(); // Pick up right fork
            forks[left].pick_up();  // Pick up left fork
        } else {
            forks[left].pick_up();  // Pick up left fork
            forks[right].pick_up(); // Pick up right fork
        }

        printf("Philosopher %d is eating...\n", id);
        usleep(1000000);

        printf("Philosopher %d is done eating.\n", id);

        // Put down forks
        forks[left].put_down();
        forks[right].put_down();
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
    int* ids = new int[NUM_PHILOSOPHERS]; // philosopher's ids

    forks = new Spinlock[NUM_PHILOSOPHERS]; // array of forks informing which ones are locked

    // create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        ids[i] = i;
        // create a new thread for every philosopher in pthread_t* philosophers array. 
        // nullptr default attributes are used. 
        // thread will execute philosopher function.
        // &ids[i] allows thread to know which philosopher it represents
        pthread_create(&philosophers[i], nullptr, philosopher, &ids[i]);
    }

    // join threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], nullptr);
    }

    delete[] philosophers;
    delete[] ids;
    delete[] forks;

    return 0;
}
