/*
#include "task_1.h"

void c_reate_file()
{
    int row = 0, column = 0;
    double** matrix;

    srand((unsigned)time(nullptr));

    row = column = 10;

    matrix = new double* [row];
    for(size_t i = 0; i < row; i++)
    {
        matrix[i] = new double[column];
        for(size_t j = 0; j < column; j++)
        {
            matrix[i][j] = rand() % 10;
        }
    }

    FILE* out = fopen("./input_f.txt", "wt");
    for (size_t i = 0; i < row; i++)
    {
        fprintf(out, "\n");
        for(size_t j = 0; j < column; j++)
        {
            fprintf(out, "%f", matrix[i][j]);
            fprintf(out, " ");
        }
    }
    fclose(out);

    matrix = new double* [row];
    for(size_t i = 0; i < row; i++)
    {
        matrix[i] = new double[column];
        for(size_t j = 0; j < column; j++)
        {
            matrix[i][j] = rand() % 10;
        }
    }

    out = fopen("./input_s.txt", "wt");
    for (size_t i = 0; i < row; i++)
    {
        fprintf(out, "\n");
        for(size_t j = 0; j < column; j++)
        {
            fprintf(out, "%f", matrix[i][j]);
            fprintf(out, " ");
        }
    }
    fclose(out);
}

void print_matrix_(mtrx& obj)
{
    std::cout << "\nPRINT ABOUT MATRIX" << std::endl;
    std::cout << "Amount of rows - " << obj.get_size() << std::endl;
    std::cout << "Amount of columns - " << obj.get_size() << std::endl;
    std::cout << "Matrix: " << std::endl;
    for (int i = 0; i < obj.get_size(); i++)
    {
        std::cout << std::endl;
        for(int j = 0; j < obj.get_size(); j++)
        {
            std::cout << obj[i * 10 + j] << "\t";
        }
    }
}

size_t get_file_size(const std::string& path)
{
    std::fstream file(path);
    size_t size = 0;

    file.seekg (0, std::ios::end);
    size = file.tellg();
    file.close();

    return size;
}

mtrx multi_matrix_(mtrx& matrix_f, mtrx& matrix_s)
{
    mtrx obj(matrix_f.get_size());

    for (int i = 0; i < obj.get_size(); i++)
    {
        for (int j = 0; j < obj.get_size(); j++)
        {
            for (int k = 0; k < obj.get_size(); k++)
            {
                obj[i * 10 + j] += matrix_f[i * 10 + k] * matrix_s[k + 10 * j];
            }
        }
    }
    return obj;
}

double dtime()
{
    struct timeval tv{};
    struct timezone tz{};
    double t;
    gettimeofday(&tv, &tz);
    t = (double)tv.tv_sec + (double)tv.tv_usec*1e-6;
    return(t);
}

void* parallel_multi_matrix_(void *pargs)
{
    auto *args = (struct matrix_args *) pargs;

    double tmp = 0;
    double t0 = dtime();

    std::vector<double> f_matrix_l = args->f_matrix;
    std::vector<double> s_matrix_l = args->s_matrix;
    std::vector<double> res_matrix_l = args->res_matrix;

    unsigned long side_size = args->side_size;
    int start = args->start;
    unsigned long end = args->end;

    for(int i = start; i < end; i++)
    {
        int count = 0;
        for(int k = 0; k < side_size; k++)
        {
            int inc_str = 0;
            for(int j = 0; j < side_size; j++)
            {
                tmp += f_matrix_l[i * side_size + inc_str] * s_matrix_l[j + k * side_size]; //2 (1 4 7) (2 5 8) (3 6 9)
                inc_str++;
            }
            res_matrix_l[i * side_size + count] = tmp;
            count++;
            tmp = 0;
        }
    }
    printf("Time of one thread working: %fs\n", dtime() - t0);
    pthread_exit(nullptr);

}

int main()
{
    c_reate_file();

    const std::string f_path = "./input_f.txt";
    const std::string s_path = "./input_s.txt";

    size_t file_size = get_file_size(f_path);
    auto side_size = (unsigned long)sqrt((double)file_size/8);
    std::cout << "File size: " << file_size << "\nAmount of matrix elements: " << file_size/8 << "\nSide size: " << side_size << "\n" << std::endl;

    mtrx f_matrix(side_size);

    std::ifstream file(f_path, std::ios::binary);
    if (file)
    {
        for(int i = 0; i < side_size; i++)
        {
            for(int j = 0; j < side_size; j++)
            {
                file >> f_matrix[i * 10 + j];
            }
        }
    }
    else
        std::cout << "Error opening file!" << std::endl;

    file_size = get_file_size(s_path);
    side_size = (unsigned long)sqrt((double)file_size/8);
    std::cout << "File size: " << file_size << "\nAmount of matrix elements: " << file_size/8 << "\nSide size: " << side_size << "\n" << std::endl;

    mtrx s_matrix(side_size);

    std::ifstream file1(s_path, std::ios::binary);
    if (file1)
    {
        for(int i = 0; i < side_size; i++)
        {
            for(int j = 0; j < side_size; j++)
            {
                file1 >> s_matrix[i * 10 + j];
            }
        }
    }
    else
        std::cout << "Error opening file!" << std::endl;

    print_matrix_(f_matrix);
    print_matrix_(s_matrix);

    std::cout << "\nSequential matrix multiplication" << std::endl;
    double t = dtime();
    mtrx res_matrix = multi_matrix_(f_matrix, s_matrix);
    std::cout << "Matrix multiplication was successful" << std::endl;
    std::cout << "All time: " << dtime()-t << std::endl;
    print_matrix_(res_matrix);

    side_size = 10;
    int id;
    int THR_NUM;
    mtrx r_matrix(side_size);

    std::cout << "\nPlease enter amount of threads" << std::endl;
    std::cin >> THR_NUM;

    pthread_t threads[THR_NUM];
    pthread_attr_t attr;

    srand(time(nullptr));

    std::cout << "Multiplying parallel" << std::endl;
    double t0 = dtime();

    pthread_attr_init(&attr);

    int step = (int)((double)side_size/(double)THR_NUM);
    int pos = 0;

    ARG.f_matrix = f_matrix.get_mtrx();
    ARG.s_matrix = s_matrix.get_mtrx();
    ARG.res_matrix = r_matrix.get_mtrx();
    ARG.side_size = side_size;

    for(int k = 0; k < THR_NUM; k++)
    {
        ARG.start = pos;
        pos += step;
        ARG.end = (k == THR_NUM - 1) ? side_size : pos;

        id = pthread_create(&threads[k], &attr, parallel_multi_matrix_, (void *)&ARG);
        if(id)
        {
            fprintf(stderr, "Error creating thread\n");
            exit(1);
        }
    }

    pthread_attr_destroy(&attr);

    for(int k = 0; k < THR_NUM; k++)
        pthread_join(threads[k], nullptr);
    std::cout << "All time: " << dtime()-t0 << std::endl;
    print_matrix_(res_matrix);
    pthread_exit(nullptr);
}
*/






























/*
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/times.h>
#include <cstdlib>
#include <cstdio>
#include <pthread.h>
#include <iostream>

#define SIZE 50

int *alloc_matrix(int size)
{
    int *matrix = (int*)malloc(size * size * sizeof(int));
    printf("matrix %dx%d allocated\n", size, size);
    return matrix;
}

void del_matrix(int *matrix)
{
    free(matrix);
}

double dtime()
{
    struct timeval tv;
    struct timezone tz;
    double t;
    gettimeofday(&tv, &tz);
    t = (double)tv.tv_sec + (double)tv.tv_usec*1e-6;
    return(t);
}

struct matrix_args{
    int *m1;
    int *m2;
    int *ans;
    int size;
    int start;
    int end;
} ARG[100];

void *multiply_matrix(void *pargs)
{
    struct matrix_args *args = (struct matrix_args *) pargs;
    int i, j, k, l, m, tmp = 0;
    double t0 = dtime();
    int *m1 = args->m1;
    int *m2 = args->m2;
    int *ans = args->ans;
    int size = args->size;
    int start = args->start;
    int end = args->end;
    for(i = start; i < end; i++)
    {
        m = i * size;
        for(j = 0; j < size; j++)
        {
            l = 0;
            for(k = 0; k < size; k++){
                tmp += m1[m + k] * m2[j + l];
                l += size;
            }
            ans[m + j] = tmp;
            tmp = 0;
        }
    }
    printf("thread works %fs\n", dtime() - t0);
    pthread_exit(NULL);
}

int main(int argc, char** argv){
    int i, j, size, k, step, pos, res;
    int THR_NUM,THR,N;

    printf("Введите число потоков\n");
    scanf("%i",&THR);

    printf("Введите размер матрици\n");
    scanf("%i",&N);
    THR_NUM = THR;
    size = N;

    pthread_t threads[THR_NUM];
    pthread_attr_t attr;
    printf("allocating\n");
    int *m1 = alloc_matrix(size);
    int *m2 = alloc_matrix(size);
    int *ans = alloc_matrix(size);
    srand(time(NULL));

    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            m1[i + j * size] = rand() % 5;
            m2[i + j * size] = rand() % 5;
        }
    }

    printf("multiplying\n");
    double t0 = dtime();
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    step = (int)((double)size/(double)THR_NUM);
    pos = 0;
    for(k = 0; k < THR_NUM; k++){
        ARG[k].m1 = m1;
        ARG[k].m2 = m2;
        ARG[k].ans = ans;
        ARG[k].size = size;
        ARG[k].start = pos;
        pos += step;
        ARG[k].end = (k == THR_NUM - 1) ? size : pos;
        res = pthread_create(&threads[k], &attr, multiply_matrix, (void *)&ARG[k]);
        if(res){
            fprintf(stderr, "Error creating thread\n");
            exit(1);
        }
    }
    pthread_attr_destroy(&attr);
    for(k = 0; k < THR_NUM; k++)
        pthread_join(threads[k], NULL);
    printf("time for multiplying: %f\n", dtime()-t0);

    del_matrix(m1);
    del_matrix(m2);
    del_matrix(ans);
    pthread_exit(NULL);
}
 */