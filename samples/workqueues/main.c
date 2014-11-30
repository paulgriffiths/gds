#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <pggds/queue.h>
#include <pggds/gds_util.h>

/*  Total number of worker threads  */
static const size_t num_threads = 4;

/*  Total number of jobs  */
static const size_t num_jobs = 20;

/*  Struct sent by a thread for a completed job  */
struct job_report {
    int sleep_time;             /*!<  Amount of time thread worked/slept    */
    int worker_id;              /*!<  ID of worker thread                   */
    bool done;                  /*!<  true if thread is finished            */
};

/*  Struct to contain a job queue  */
struct job_queue {
    Queue queue;                /*!<  The queue itself                      */
    pthread_mutex_t mutex;      /*!<  Mutex to access the queue             */
    pthread_cond_t cond;        /*!<  Condition variable for jobs in queue  */
};

/*  Struct containing info passed to a worker thread on creation  */
struct thread_info {
    struct job_queue * in_queue;    /*!<  Queue for incoming tasks          */
    struct job_queue * out_queue;   /*!<  Queue for outgoing job reports    */
    int worker_id;                  /*!<  ID of worker thread (self)        */
};

/*  Creates a new job queue  */

struct job_queue * job_queue_create(const enum gds_datatype type)
{
    struct job_queue * new_queue = xmalloc(sizeof *new_queue);

    /*  Queue could conceivably need more than num_jobs elements,
     *  since each thread will send an additional report when it's
     *  done, so we make it resizable just in case.                 */

    new_queue->queue = queue_create(num_jobs, type, GDS_RESIZABLE);
    if ( !new_queue->queue ) {
        fprintf(stderr, "couldn't create queue\n");
        exit(EXIT_FAILURE);
    }

    if ( pthread_mutex_init(&new_queue->mutex, NULL) != 0 ) {
        perror("couldn't initialize mutex");
        exit(EXIT_FAILURE);
    }

    if ( pthread_cond_init(&new_queue->cond, NULL) != 0 ) {
        perror("couldn't initialize condition variable");
        exit(EXIT_FAILURE);
    }

    return new_queue;
}

/*  Destroys a job queue  */

void job_queue_destroy(struct job_queue * queue)
{
    queue_destroy(queue->queue);

    if ( pthread_mutex_destroy(&queue->mutex) != 0 ) {
        fprintf(stderr, "couldn't destroy mutex\n");
        exit(EXIT_FAILURE);
    }

    if ( pthread_cond_destroy(&queue->cond) != 0 ) {
        fprintf(stderr, "couldn't destroy condition variable\n");
        exit(EXIT_FAILURE);
    }

    free(queue);
}

/*  Creates a job report for sending back to the main thread  */

struct job_report * job_report_create(const int sleep_time,
                                      const int worker_id,
                                      const bool done)
{
    struct job_report * new_report = xmalloc(sizeof *new_report);

    new_report->sleep_time = sleep_time;
    new_report->worker_id = worker_id;
    new_report->done = done;

    return new_report;
}

/*  Creates a new thread info object  */

struct thread_info * thread_info_create(const int worker_id,
                                        struct job_queue * in_queue,
                                        struct job_queue * out_queue)
{
    struct thread_info * tinfo = xmalloc(sizeof *tinfo);

    tinfo->worker_id = worker_id;
    tinfo->in_queue = in_queue;
    tinfo->out_queue = out_queue;

    return tinfo;
}

/*  Worker thread function  */

void * thread_func(void * arg)
{
    struct thread_info * tinfo = arg;
    struct job_queue * in_queue = tinfo->in_queue;
    struct job_queue * out_queue = tinfo->out_queue;
    const int id = tinfo->worker_id;
    free(arg);

    bool done = false;

    /*  Loop until done  */

    while ( !done ) {
        int job = -1;

        /*  Get next job from in queue  */

        if ( pthread_mutex_lock(&in_queue->mutex) != 0 ) {
            fprintf(stderr, "couldn't lock mutex\n");
            exit(EXIT_FAILURE);
        }

        if ( queue_is_empty(in_queue->queue) ) {

            /*  If queue is empty, we're finished  */

            done = true;
        }
        else {

            /*  Otherwise retrieve the next job  */

            queue_pop(in_queue->queue, &job);
        }

        if ( pthread_mutex_unlock(&in_queue->mutex) != 0 ) {
            fprintf(stderr, "couldn't unlock mutex\n");
            exit(EXIT_FAILURE);
        }
        
        /*  Do the work  */

        if ( !done ) {
            sleep(job);
        }

        /*  Report the job or done status to the out queue  */

        if ( pthread_mutex_lock(&out_queue->mutex) != 0 ) {
            fprintf(stderr, "couldn't lock mutex\n");
            exit(EXIT_FAILURE);
        }

        struct job_report * report = job_report_create(job, id, done);
        queue_push(out_queue->queue, report);

        if ( pthread_cond_signal(&out_queue->cond) != 0 ) {
            fprintf(stderr, "couldn't signal condition variable\n");
            exit(EXIT_FAILURE);
        }

        if ( pthread_mutex_unlock(&out_queue->mutex) != 0 ) {
            fprintf(stderr, "couldn't unlock mutex\n");
            exit(EXIT_FAILURE);
        }
    }

    return NULL;
}

int main(void)
{
    struct job_queue * out_queue = job_queue_create(DATATYPE_INT);
    struct job_queue * done_queue = job_queue_create(DATATYPE_POINTER);
    pthread_t tid[num_threads];
    int live_threads = 0;

    /*  Populate out queue with tasks, giving the PRNG
     *  pedants some ammunition in the process. Each
     *  'task' consists of merely sleeping (nice job!)
     *  for a specified number of seconds. Here we just
     *  randomly generate those numbers of seconds.      */

    srand( (unsigned) time(NULL));
    for ( size_t i = 0; i < num_jobs; ++i ) {
        queue_push(out_queue->queue, rand() % 10 + 1);
    }

    /*  Start threads  */

    for ( size_t i = 0; i < num_threads; ++i ) {
        struct thread_info * tinfo = thread_info_create(i + 1,
                                                        out_queue,
                                                        done_queue);
        if ( pthread_create(&tid[i], NULL, thread_func, tinfo) != 0 ) {
            fprintf(stderr, "couldn't create thread\n");
            exit(EXIT_FAILURE);
        }
        ++live_threads;
    }

    /*  Main event loop  */

    if ( pthread_mutex_lock(&done_queue->mutex) != 0 ) {
        fprintf(stderr, "couldn't lock mutex\n");
        exit(EXIT_FAILURE);
    }

    while ( live_threads ) {

        /*  If queue is empty, wait on the condition variable.
         *  This should never wait forever, since the last thing
         *  off the queue should be the report saying the last
         *  thread is finished, at which point the main event
         *  loop will end.                                        */

        while ( queue_is_empty(done_queue->queue) ) {
            if ( pthread_cond_wait(&done_queue->cond,
                                   &done_queue->mutex) != 0 ) {
                fprintf(stderr, "couldn't wait on condition variable\n");
                exit(EXIT_FAILURE);
            }
        }

        /*  Process any job reports on the queue  */

        while ( !queue_is_empty(done_queue->queue) ) {
            struct job_report * report;
            queue_pop(done_queue->queue, (void *) &report);

            if ( report->done ) {

                /*  Join thread and decrement live thread
                 *  counter if the thread has finished.    */

                if ( pthread_join(tid[report->worker_id - 1], NULL) != 0 ) {
                    fprintf(stderr, "couldn't join thread\n");
                    exit(EXIT_FAILURE);
                }

                --live_threads;
            }
            else {

                /*  Otherwise report on the finished job.  */

                printf("Thread %d worked hard and slept for %d seconds.\n",
                       report->worker_id, report->sleep_time);
            }

            free(report);
        }
    }

    if ( pthread_mutex_unlock(&done_queue->mutex) != 0 ) {
        fprintf(stderr, "couldn't unlock mutex\n");
        exit(EXIT_FAILURE);
    }

    /*  Tidy up and exit  */

    job_queue_destroy(out_queue);
    job_queue_destroy(done_queue);

    return 0;
}
