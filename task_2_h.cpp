/*
#include <cstdio>
#include <random>
#include <ctime>
#include <iostream>
#include <pthread.h>
#include <fstream>
#include <sys/time.h>
#include <cstdlib>
#include <optional>
#include <vector>

template <typename T>
struct optional {
private:
    bool _has_value;
    T _value;
public:
    optional() : _has_value{false}, _value{} {}
    optional(T v) : _has_value{true}, _value{v} {}
    bool has_value() const {return _has_value;}
    T value() const {
        if (_has_value) return _value;
        throw std::bad_optional_access();
    }
    T value_or(T def) const {
        return _has_value ? _value : def;
    }
    optional<T>& operator=(T v) {
        _has_value = true;
        _value = v;
        return *this;
    }
    void reset() {_has_value = false;}
};

template <typename T>
class threadsafe_queue{
private:
    const size_t capacity;
    std::vector<T> array;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    bool INTERRUPT_FLAG;
public:

    void last_element_flag()
    {
        INTERRUPT_FLAG = true;
    }

    threadsafe_queue(size_t max_size): capacity(max_size), INTERRUPT_FLAG(false)
    {
        pthread_mutex_init(&mutex, nullptr);
        pthread_cond_init(&cond, nullptr);
    }

    threadsafe_queue(const threadsafe_queue&) = delete;//no queue copy
    threadsafe_queue(threadsafe_queue&&) = delete;     //no queue move

    ~threadsafe_queue()
    {
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&cond);
        array.erase(array.end() - 1);
    }

    void enqueue(const T& v)
    {
        pthread_mutex_lock(&mutex);
        if(!full())
        {
            array.push_back(v);
            pthread_mutex_unlock(&mutex);
            return;
        }
        else
        {
            pthread_mutex_unlock(&mutex);
            throw std::logic_error("ARRAY IS FULL");
        }
    }

    T dequeue()
    {
        if (INTERRUPT_FLAG && array.size() < 1)
        {
                pthread_mutex_unlock(&mutex);
                throw std::logic_error("END OF ACTION");
        }
        pthread_mutex_lock(&mutex);
        if (!empty())
        {
            T elem = array[0];
            std::cout << "Received: " << elem << std::endl;
            array.erase(array.begin());
            pthread_mutex_unlock(&mutex);
            return elem;
        }
        else
        {
            pthread_mutex_unlock(&mutex);
            throw std::invalid_argument("ARRAY EMPTY");
        }
    }

    bool full() const
    {
        return(array.size() == capacity);
    }

    bool empty() const
    {
        return(array.size() == 0);
    }

    optional<T> try_dequeue()
    {
        if (INTERRUPT_FLAG && array.size() == 0)
            return optional<T>{};
        pthread_mutex_lock(&mutex);
        if(!empty())
        {
            //do {
                pthread_cond_wait(&cond, &mutex);
                if (!empty())
                {
                    T elem = array[0];
                    std::cout << "Received: " << elem << std::endl;
                    array.erase(array.begin());
                    optional<T> opt(elem);
                    pthread_mutex_unlock(&mutex);
                    return opt;
                }
            //}while(empty());
        }
        else
        {
            pthread_mutex_unlock(&mutex);
            return optional<T>{};
        }
    }

    bool try_enqueue(const T& v)
    {
        pthread_mutex_lock(&mutex);
        if(!full())
        {
            array.push_back(v);
            pthread_cond_signal(&cond);
            pthread_mutex_unlock(&mutex);
            return true;
        }
        return false;
    }
};

struct TEMP{
    int m;
    double d;
} ARG[100];


threadsafe_queue<int> object(101);
void* riddler_main(void* pargs)
{
    try
    {
        object.dequeue();
        return nullptr;
    }
    catch(std::logic_error const &error)
    {
        std::cout << "THE WRITERS SAID STOP" << std::endl;
        pthread_cancel(nullptr);
        exit(EXIT_SUCCESS);
        pthread_exit(nullptr);
        //exit(EXIT_FAILURE);
    }
    catch(std::invalid_argument const &error)
    {
        std::cout << "ARRAY EMPTY, BUT NO END" << std::endl;
        return nullptr;
    }
}

void* writer_main(void* pargs)
{
    TEMP *args = (struct TEMP *) pargs;
    int a = args->m;
    //double d = args->d;

    try
    {
        object.enqueue(a);
        return nullptr;
    }
    catch(std::logic_error const &error)
    {
        std::cout << "ARRAY IS FULL" << std::endl;
        return nullptr;
    }
}
 */
/*
int main()
{
    int array[100];
    for(int i = 0; i < 100; i++)
    {
        array[i] = i;
    }

    int THR_NUM, THR_NUMS, id, ids;
    std::cout << "Please enter amount of threads-writers" << std::endl;
    std::cin >> THR_NUM;

    std::cout << "Please enter amount of threads-readers" << std::endl;
    std::cin >> THR_NUMS;

    pthread_t threads[THR_NUM];
    pthread_t threadsr[THR_NUMS];
    pthread_attr_t attr;
    pthread_attr_t attr1;

    pthread_attr_init(&attr);
    pthread_attr_init(&attr1);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_attr_setdetachstate(&attr1, PTHREAD_CREATE_JOINABLE);

   // for(int i = 0; i < 2; i++) {
        for (int k = 0; k < THR_NUM; k++) {
            ARG[k].m = k + 10;
            //ARG[k].d = (double) k;

            id = pthread_create(&threads[k], &attr, writer_main, (void *) &ARG[k]);
            if (id) {
                fprintf(stderr, "Error creating thread\n");
                exit(1);
            }
            if (k == THR_NUM - 2)
            {
                std::cout << "end\n";
                object.last_element_flag();
            }
        }
    //}
    //for(int i = 0; i < 4; i++)
   // {
   for (int k = 0; k < THR_NUMS; k++)
   {
       ids = pthread_create(&threadsr[k], &attr1, riddler_main, NULL);
       if (ids)
       {
           fprintf(stderr, "Error creating thread\n");
           exit(1);
       }
   }
   pthread_attr_destroy(&attr);
   pthread_attr_destroy(&attr1);

   pthread_exit(nullptr);
   return 0;
}
*/
/*
    ARG[0].m = 10;
    ARG[1].m = 11;
    ARG[2].m = 12;
    std::cout << "main - 3" << std::endl;
    id = pthread_create(&threads[0], &attr, writer_main, (void *) &ARG[0]);
    if (id) {
        fprintf(stderr, "Error creating thread\n");
        exit(1);
    }
    std::cout << "main - 3" << std::endl;
    id = pthread_create(&threads[1], &attr, writer_main, (void *) &ARG[1]);
    if (id) {
        fprintf(stderr, "Error creating thread\n");
        exit(1);
    }
    std::cout << "main - 4" << std::endl;
    ids = pthread_create(&threadsr[0], &attr1, riddler_main, NULL);
    if (ids) {
        fprintf(stderr, "Error creating thread\n");
        exit(1);
    }
    std::cout << "main - 3" << std::endl;
    id = pthread_create(&threads[2], &attr, writer_main, (void *) &ARG[2]);
    if (id) {
        fprintf(stderr, "Error creating thread\n");
        exit(1);
    }
    object.last_element_flag();
    std::cout << "main - 4" << std::endl;
    ids = pthread_create(&threadsr[1], &attr1, riddler_main, NULL);
    if (ids) {
        fprintf(stderr, "Error creating thread\n");
        exit(1);
    }
    std::cout << "main - 4" << std::endl;
    ids = pthread_create(&threadsr[2], &attr1, riddler_main, NULL);
    if (ids) {
        fprintf(stderr, "Error creating thread\n");
        exit(1);
    }
    std::cout << "main - 4" << std::endl;
    ids = pthread_create(&threadsr[3], &attr1, riddler_main, NULL);
    if (ids) {
        fprintf(stderr, "Error creating thread\n");
        exit(1);
    }

    pthread_attr_destroy(&attr);
    pthread_attr_destroy(&attr1);

    for(int k = 0; k < THR_NUM; k++)
        pthread_join(threads[k], nullptr);
    for(int k = 0; k < THR_NUMS; k++)
        pthread_join(threadsr[k], nullptr);

    pthread_exit(NULL);
    return 0;
}
        try
        {
            return optional<T> (try_dequeue());
        }
        catch(std::invalid_argument const &error)
        {
            return optional<T>{};
        }
*/

