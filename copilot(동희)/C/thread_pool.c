// 쓰레드 풀을 구현한 C언어 코드를 작성해줘
// 쓰레드 풀은 쓰레드를 미리 생성해두고, 작업을 할당받으면 생성된 쓰레드에게 작업을 할당하는 방식으로 동작한다.
// 쓰레드 풀은 쓰레드를 생성하고 삭제하는데 드는 비용을 줄일 수 있어서, 쓰레드를 반복적으로 생성하고 삭제하는 작업을 할 때 유용하다.
// 쓰레드 풀은 다음과 같은 함수를 구현해야 한다.
// void thread_pool_init(int num_threads) : 쓰레드 풀을 초기화한다. num_threads 개수만큼 쓰레드를 생성한다.
// void thread_pool_submit(void (*func)(void*), void* arg) : 작업을 쓰레드 풀에 제출한다. func은 작업을 수행할 함수의 포인터이고, arg는 func에 전달할 인자이다.
// void thread_pool_shutdown() : 쓰레드 풀을 종료한다. 쓰레드 풀에 있는 모든 쓰레드를 종료하고, 쓰레드 풀을 해제한다.
// 쓰레드 풀은 다음과 같은 제약을 가진다.
// 쓰레드 풀은 최대 100개의 쓰레드를 생성할 수 있다.
// 쓰레드 풀은 최대 100개의 작업을 대기열에 가질 수 있다.
// 쓰레드 풀은 쓰레드가 작업을 수행할 때까지 대기하도록 해야 한다.
// 쓰레드 풀은 쓰레드가 작업을 수행한 후에 다음 작업을 수행하도록 해야 한다.
// 쓰레드 풀은 쓰레드가 종료될 때까지 대기하도록 해야 한다.
// 쓰레드 풀은 쓰레드가 종료된 후에 새로운 쓰레드를 생성해야 한다.
// 쓰레드 풀은 쓰레드가 종료된 후에 새로운 작업을 수행할 쓰레드를 생성해야 한다.
// 쓰레드 풀은 쓰레드가 종료된 후에 새로운 작업을 수행할 쓰레드를 생성할 때, 쓰레드가 생성될 때까지 대기하도록 해야 한다.
// 쓰레드 풀은 쓰레드가 종료된 후에 새로운 작업을 수행할 쓰레드를 생성할 때, 쓰레드가 생성된 후에 작업을 수행하도록 해야 한다.
// 쓰레드 풀은 쓰레드가 종료된 후에 새로운 작업을 수행할 쓰레드를 생성할 때, 쓰레드가 생성된 후에 다음 작업을 수행하도록 해야 한다.
// 쓰레드 풀은 쓰레드가 종료된 후에 새로운 작업을 수행할 쓰레드를 생성할 때, 쓰레드가 생성된 후에 새로운 작업을 수행하도록 해야 한다.
// 쓰레드 풀은 쓰레드가 종료된 후에 새로운 작업을 수행할 쓰레드를 생성할 때, 쓰레드가 생성된 후에 새로운 작업을 수행할 쓰레드를 생성하도록 해야 한다.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_THREADS 100
#define MAX_QUEUE 100

typedef struct {
    void (*func)(void*);
    void* arg;
} task_t;

typedef struct {
    pthread_t thread;
    int id;
} thread_t;

typedef struct {
    thread_t threads[MAX_THREADS];
    task_t queue[MAX_QUEUE];
    sem_t queue_sem;
    sem_t queue_count;
    sem_t shutdown;
    int num_threads;
    int queue_front;
    int queue_back;
    int queue_size;
    int shutdown_flag;
} thread_pool_t;

void* thread_func(void* arg) {
    thread_pool_t* pool = (thread_pool_t*)arg;
    while (1) {
        sem_wait(&pool->queue_count);
        if (pool->shutdown_flag) {
            break;
        }
        task_t task;
        sem_wait(&pool->queue_sem);
        task = pool->queue[pool->queue_front];
        pool->queue_front = (pool->queue_front + 1) % MAX_QUEUE;
        pool->queue_size--;
        sem_post(&pool->queue_sem);
        task.func(task.arg);
    }
    return NULL;
}

void thread_pool_init(thread_pool_t* pool, int num_threads) {
    pool->num_threads = num_threads;
    pool->queue_front = 0;
    pool->queue_back = 0;
    pool->queue_size = 0;
    pool->shutdown_flag = 0;
    sem_init(&pool->queue_sem, 0, 1);
    sem_init(&pool->queue_count, 0, 0);
    sem_init(&pool->shutdown, 0, 0);
    for (int i = 0; i < num_threads; i++) {
        pool->threads[i].id = i;
        pthread_create(&pool->threads[i].thread, NULL, thread_func, pool);
    }
}

void thread_pool_submit(thread_pool_t* pool, void (*func)(void*), void* arg) {
    sem_wait(&pool->queue_sem);
    pool->queue[pool->queue_back].func = func;
    pool->queue[pool->queue_back].arg = arg;
    pool->queue_back = (pool->queue_back + 1) % MAX_QUEUE;
    pool->queue_size++;
    sem_post(&pool->queue_sem);
    sem_post(&pool->queue_count);
}

void thread_pool_shutdown(thread_pool_t* pool) {
    pool->shutdown_flag = 1;
    for (int i = 0; i < pool->num_threads; i++) {
        sem_post(&pool->queue_count);
    }
    for (int i = 0; i < pool->num_threads; i++) {
        pthread_join(pool->threads[i].thread, NULL);
    }
    sem_destroy(&pool->queue_sem);
    sem_destroy(&pool->queue_count);
    sem_destroy(&pool->shutdown);
}

void task_func(void* arg) {
    int id = *(int*)arg;
    printf("task %d\n", id);
    free(arg); // 동적 메모리 해제
}

int main() {
    thread_pool_t pool;
    thread_pool_init(&pool, 10);
    for (int i = 0; i < 100; i++) {
        int* arg = malloc(sizeof(int)); // 동적 메모리 할당
        *arg = i;
        thread_pool_submit(&pool, task_func, arg);
    }
    thread_pool_shutdown(&pool);
    return 0;
}

