#include <stdio.h>
#include <pthread.h>

#define ITERATIONS 10000  // Reduced for quick output demonstration

// Shared resource
int shared_counter = 0;

// Mutex to protect shared resource
pthread_mutex_t lock;

// Thread function WITHOUT mutex → demonstrates race condition
void* increment_no_mutex(void* arg) {
    for (int i = 0; i < ITERATIONS; i++) {
        shared_counter++;  // No protection → race condition
    }
    return NULL;
}

// Thread function WITH mutex → safe concurrent access
void* increment_mutex(void* arg) {
    for (int i = 0; i < ITERATIONS; i++) {
        pthread_mutex_lock(&lock);   // Enter critical section
        shared_counter++;
        pthread_mutex_unlock(&lock); // Exit critical section
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    // ----------------------------
    // Demonstrate race condition
    // ----------------------------
    printf("=== Running WITHOUT mutex (race condition) ===\n");
    shared_counter = 0;  // Reset counter

    pthread_create(&t1, NULL, increment_no_mutex, NULL);
    pthread_create(&t2, NULL, increment_no_mutex, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Final counter value WITHOUT mutex: %d\n", shared_counter);
    printf("Expected value: %d\n\n", ITERATIONS * 2);

    // ----------------------------
    // Demonstrate safe mutex usage
    // ----------------------------
    printf("=== Running WITH mutex (synchronization) ===\n");
    shared_counter = 0;  // Reset counter
    pthread_mutex_init(&lock, NULL);

    pthread_create(&t1, NULL, increment_mutex, NULL);
    pthread_create(&t2, NULL, increment_mutex, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&lock);

    printf("Final counter value WITH mutex: %d\n", shared_counter);
    printf("Expected value: %d\n", ITERATIONS * 2);

    return 0;
}

