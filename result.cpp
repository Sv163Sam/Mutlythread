#include "result.h"

template <typename T>
class threadsafe_queue
        {
private:
    const size_t size;
    std::queue<T> queue;

    pthread_mutex_t mutex{};
    pthread_cond_t cond{};
    pthread_cond_t cond_w{};

    bool NO_WRITERS;
    bool NO_READERS;

public:

    explicit threadsafe_queue(size_t max_size) : size(max_size), NO_WRITERS(false), NO_READERS(false) {
        pthread_mutex_init(&mutex, nullptr);
        pthread_cond_init(&cond, nullptr);
        pthread_cond_init(&cond_w, nullptr);
    }

    threadsafe_queue(const threadsafe_queue &) = delete;//no queue copy
    threadsafe_queue(threadsafe_queue &&) = delete;     //no queue move

    ~threadsafe_queue()
    {
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&cond);
        pthread_cond_destroy(&cond_w);
        while (!empty()) queue.pop();
    }

    void end_w()
    {
        if(NO_WRITERS)
        {
            pthread_cond_broadcast(&cond);
            while(!empty())
                dequeue();
            pthread_mutex_unlock(&mutex);
        }
        if(NO_READERS)
        {
            pthread_cond_broadcast(&cond_w);
            pthread_mutex_unlock(&mutex);
        }
    }

    void last_element_flag()
    {
        NO_WRITERS = true;
    }

    void all_end() {
        NO_READERS = true;
    }

    bool full() const
    {
        return (queue.size() == MAX_SIZE);
    }

    bool empty() const
    {
        return (queue.empty());
    }

    void enqueue(const T& v)
    {
        pthread_mutex_lock(&mutex);
        while(full())
            pthread_cond_wait(&cond_w, &mutex);

        if (!full())
        {
            queue.push(v);
            std::cout << "I'm wrote: " << v << std::endl;
            pthread_cond_signal(&cond);
        }
        pthread_mutex_unlock(&mutex);
    }

    T dequeue()
    {
        T elem = -1;
        pthread_mutex_lock(&mutex);
        while(empty())
            pthread_cond_wait(&cond, &mutex);

        if (!empty())
        {
            elem = queue.front();
            std::cout << "Received: " << elem << std::endl;
            queue.pop();
            pthread_cond_signal(&cond_w);
        }
        else throw std::exception();

        pthread_mutex_unlock(&mutex);
        return elem;
    }

    optional<T> try_dequeue()
    {
        T elem = -1;
        int rc = 0;
        struct timespec ts{};
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += 5;
        pthread_mutex_lock(&mutex);
        while(empty()) {
            rc = pthread_cond_timedwait(&cond, &mutex, &ts);
            if(rc != 0) throw std::exception();
        }

        if (!empty())
        {
            elem = queue.front();
            std::cout << "Received: " << elem << std::endl;
            queue.pop();
            pthread_cond_signal(&cond_w);
            pthread_mutex_unlock(&mutex);
            optional<T> opt(elem);
            return opt;
        }
        pthread_mutex_unlock(&mutex);
        return optional<T>{};
    }

    bool try_enqueue(const T& v)
    {
        struct timespec ts{};
        int rc = 0;
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += 5;
        pthread_mutex_lock(&mutex);
        while(full()) {
            rc = pthread_cond_timedwait(&cond_w, &mutex, &ts);
            if(rc != 0) throw std::exception();
        }

        if (!full())
        {
            queue.push(v);
            std::cout << "I'm wrote: " << v << std::endl;
            pthread_cond_signal(&cond);
            pthread_mutex_unlock(&mutex);
            return true;
        }
        pthread_mutex_unlock(&mutex);
        return false;
    }


};

threadsafe_queue<int> object(5);

void* deq_read(void* pargs)
{
    try
    {
        object.dequeue();
    }
    catch(const std::exception& error)
    {
        std::cout << "NO WRITERS ANN QUEUE IS EMPTY!" << std::endl;
        exit(EXIT_SUCCESS);
    }
    return nullptr;
}
void* enq_writ(void* pargs)
{
    TEMP *args = (struct TEMP *) pargs;
    int a = args->m;
    object.enqueue(a);
    return nullptr;
}
void* try_deq_read(void* pargs)
{
    try
    {
        optional<int> a = object.try_dequeue();
        if (!a.has_value()) {
            std::cout << "WAS CALLED INTERRUPT FLAG" << std::endl;
            exit(EXIT_SUCCESS);
        }
        return nullptr;
    }
    catch(const std::exception& err)
    {
        std::cout << "TIMEOUT EXPIRED" << std::endl;
        exit(EXIT_FAILURE);
    }
}
void* try_enq_writ(void* pargs)
{
    try
    {
        TEMP *args = (struct TEMP *) pargs;
        int a = args->m;
        object.try_enqueue(a);
        return nullptr;
    }
    catch(const std::exception& err)
    {
        std::cout << "TIMEOUT EXPIRED" << std::endl;
        exit(EXIT_FAILURE);
    }
}
void* working_end(void* pargs)
{
    object.end_w();
    return nullptr;
}
void finish_step()
{
    int id;
    pthread_t thr[1];
    pthread_attr_t attr2;
    pthread_attr_init(&attr2);
    id = pthread_create(&thr[0], &attr2, working_end, (void *) &ARG);
    if (id)
    {
        fprintf(stderr, "Error creating thread\n");
        exit(1);
    }
    pthread_attr_destroy(&attr2);
    pthread_join(thr[0], nullptr);
}

int main()
{
    int THR_NUM = 40, THR_NUMS = 50, id, ids;

    //std::cout << "Please enter amount of threads-writers" << std::endl;
    //std::cin >> THR_NUM;
    //std::cout << "Please enter amount of threads-readers" << std::endl;
    //std::cin >> THR_NUMS;

    pthread_t threads[THR_NUM];
    pthread_t threadsr[THR_NUMS];

    pthread_attr_t attr;
    pthread_attr_t attr1;

    pthread_attr_init(&attr);
    pthread_attr_init(&attr1);

    int THR_NUM_COUNT = -1;
    int THR_NUMS_COUNT = -1;
    int SELECT_BUTTON = 0;
    int SELECT_MODE = 0;

    std::cout << "Please select a mode of executing program:\n1 - enqueue, dequeue\n2 - tryenqueue, trydequeue" << std::endl;
    std::cin >> SELECT_MODE;

    std::cout << "Please enter value:\n1 - Working one of all writers\n2 - Working one of all readers\nSend message that writers will stop\nSend message that readers will stop" << std::endl;
    std::cin >> SELECT_BUTTON;
    int inc = 0;

    while(SELECT_BUTTON != 4)
    {
        THR_NUM_COUNT++;
        THR_NUMS_COUNT++;
        inc++;
        switch(SELECT_BUTTON)
        {
            case 1:
            {
                ARG.m = inc;
                switch (SELECT_MODE)
                {
                    case 1:
                    {
                        id = pthread_create(&threads[THR_NUM_COUNT], &attr, enq_writ, (void *) &ARG);
                        break;
                    }
                    case 2:
                    {
                        id = pthread_create(&threads[THR_NUM_COUNT], &attr, try_enq_writ, (void *) &ARG);
                        break;
                    }
                    default:
                        exit(EXIT_FAILURE);
                }
                if (id)
                {
                    fprintf(stderr, "Error creating thread\n");
                    exit(1);
                }
                std::cin >> SELECT_BUTTON;
                break;
            }
            case 2:
            {
                switch (SELECT_MODE)
                {
                    case 1:
                    {
                        ids = pthread_create(&threadsr[THR_NUMS_COUNT], &attr1, deq_read, nullptr);
                        break;
                    }
                    case 2:
                    {
                        ids = pthread_create(&threadsr[THR_NUMS_COUNT], &attr1, try_deq_read, nullptr);
                        break;
                    }
                    default:
                        exit(EXIT_FAILURE);
                }
                if (ids)
                {
                    fprintf(stderr, "Error creating thread\n");
                    exit(1);
                }
                std::cin >> SELECT_BUTTON;
                break;
            }
            case 3:
            {
                object.last_element_flag();
                finish_step();
                exit(EXIT_SUCCESS);
            }
            case 4:
            {
                object.all_end();
                finish_step();
                break;
            }
            default: exit(EXIT_FAILURE);
        }
    }

    pthread_attr_destroy(&attr);
    pthread_attr_destroy(&attr1);

    for(auto & thread : threads)
    {
        pthread_join(thread, nullptr);
    }
    for(auto & k : threadsr)
    {
        pthread_join(k, nullptr);
    }

    pthread_exit(nullptr);
}



























//int iid = pthread_create(&t_clean[0], &attr2, cleaner, NULL);
/*
ARG[THR_NUM].m = THR_NUM;
//std::cout << k << std::endl;
id = pthread_create(&threads[THR_NUM], &attr, writer_main, (void *) &ARG[THR_NUM]);
if (id) {
    fprintf(stderr, "Error creating thread\n");
    exit(1);
}
ARG[THR_NUM+1].m = THR_NUM+1;
//std::cout << k << std::endl;
id = pthread_create(&threads[THR_NUM+1], &attr, writer_main, (void *) &ARG[THR_NUM+1]);
if (id) {
    fprintf(stderr, "Error creating thread\n");
    exit(1);
}
ids = pthread_create(&threadsr[THR_NUMS], &attr1, riddler_main, NULL);
if (ids) {
    fprintf(stderr, "Error creating thread\n");
    exit(1);
}
*/