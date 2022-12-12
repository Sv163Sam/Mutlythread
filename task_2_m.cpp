/*
#include "task_2_l.h"

double dtime()
{
    struct timeval tv{};
    struct timezone tz{};
    double t;
    gettimeofday(&tv, &tz);
    t = (double)tv.tv_sec + (double)tv.tv_usec*1e-6;
    return(t);
}

void* find_value(void *pargs)
{
    auto *args = (struct args *) pargs;

    double t0 = dtime();

    std::vector<int> array = args->array;
    int search_value = args->search_value;
    int start = args->start;
    int end = args->end;

    pthread_mutex_lock(&mutex);

    for(int i = start; i < end; i++)
    {
        if(array[i] == search_value)
            ret_indx.push_back(i);
    }

    pthread_mutex_unlock(&mutex);

    printf("Time of one thread working %fs\n", dtime() - t0);
    pthread_exit(nullptr);
}

int main()
{
    const int size = 150;
    int search_val = 0;
    std::vector<int> array;

    std::cout << "Please enter a search value" << std::endl;
    std::cin >> search_val;

    for(int i = 0; i < 70; i++)
    {
        array.push_back(i);
    }
    for(int i = 70; i < size; i++)
    {
        array.push_back(i - 60);
    }

    int THR_NUM, id;
    std::cout << "Please enter amount of threads" << std::endl;
    std::cin >> THR_NUM;

    pthread_t threads[THR_NUM];
    pthread_attr_t attr;

    double t0 = dtime();

    pthread_attr_init(&attr);
    pthread_mutex_init(&mutex, nullptr);

    int step = (int)((double)size/(double)THR_NUM);
    int pos = 0;

    ARG.array = array;
    ARG.search_value = search_val;

    for(int k = 0; k < THR_NUM; k++)
    {
        ARG.start = pos;
        pos += step;
        ARG.end = (k == THR_NUM - 1) ? size : pos;

        id = pthread_create(&threads[k], &attr, find_value, (void *)&ARG);
        if(id)
        {
            fprintf(stderr, "Error creating thread\n");
            exit(1);
        }
    }

    pthread_attr_destroy(&attr);

    for(int k = 0; k < THR_NUM; k++)
        pthread_join(threads[k], nullptr);
    std::cout << "All time search: " << dtime()-t0 << std::endl;

    pthread_mutex_destroy(&mutex);

    sort(ret_indx.begin(), ret_indx.end(), std::greater<>());

    if(ret_indx.empty())
        std::cout << "NOT A NUMBER" << std::endl;


    std::cout << "Finded indexes: ";
    for(int i : ret_indx)
    {
        std::cout << "\t" << i;
    }

    pthread_exit(nullptr);
}
*/