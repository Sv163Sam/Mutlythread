
#include <cstdio>
#include <random>
#include <ctime>
#include <iostream>
#include <pthread.h>
#include <fstream>
#include <sys/time.h>
#include <cstdlib>
#include <vector>

class mtrx
{
private:
    std::vector<double> matrix;
    unsigned long side_size = 0;
public:
    explicit mtrx(unsigned long size) : side_size(size)
    {
        matrix.resize(side_size * side_size);
    }
    ~mtrx()
    {
        matrix.clear();
    }
    void set_value(int index, int value)
    {
        matrix[index] = value;
    }
    unsigned long get_size() const
    {
        return side_size;
    }
    unsigned long get_mat_size()
    {
        return matrix.size();
    }
    std::vector<double>& get_mtrx()
    {
        return matrix;
    }
    double &operator[](int i)
    {
        return matrix[i];
    }
    mtrx& operator=(const mtrx& obj)
    {
        this->side_size = obj.side_size;
        this->matrix.resize(side_size);
        for(int i = 0; i < side_size; i++)
        {
            matrix[i] = obj.matrix[i];
        }
        return *this;
    }
};

struct matrix_args
{
    std::vector<double> s_matrix;
    std::vector<double> f_matrix;
    std::vector<double> res_matrix;
    int start;
    unsigned long end;
    unsigned long side_size = 0;
} ARG;

void c_reate_file();
void print_matrix_(mtrx& obj);
size_t get_file_size(const std::string& path);
mtrx multi_matrix_(mtrx& matrix_f, mtrx& matrix_s);
double dtime();
void* parallel_multi_matrix_(void *pargs);











/*
 * #include <iostream>
#include <pthread.h>
#include <fstream>
#include <sys/time.h>
#include <cstdlib>
#include "help.h"

struct mtrx
{
    double** matrix;
    unsigned long side_size = 0;
};

void print_matrix_(mtrx obj)
{
    std::cout << "\nPRINT ABOUT MATRIX" << std::endl;
    std::cout << "Amount of rows - " << obj.side_size << std::endl;
    std::cout << "Amount of columns - " << obj.side_size << std::endl;
    std::cout << "Matrix: " << std::endl;
    for (size_t i = 0; i < obj.side_size; i++)
    {
        std::cout << std::endl;
        for(size_t j = 0; j < obj.side_size; j++)
        {
            std::cout << obj.matrix[i][j] << "\t";
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

mtrx create_matrix(const std::string& path)
{
    mtrx obj;

    size_t file_size = get_file_size(path);
    auto side_size = (unsigned long)sqrt((double)file_size/8);
    std::cout << "File size: " << file_size << "\nAmount of matrix elements: " << file_size/8 << "\nSide size: " << side_size << "\n" << std::endl;

    double **matrix;
    matrix = new double*[side_size];
    for(int i = 0; i < side_size; i++)
        matrix[i] = new double[side_size];

    std::ifstream file(path, std::ios::binary);
    if (file)
    {
        for(int i = 0; i < side_size; i++)
        {
            for(int j = 0; j < side_size; j++)
            {
                file >> matrix[i][j];
            }
        }
    }
    else
        std::cout << "Error opening file!" << std::endl;

    obj.matrix = matrix;
    obj.side_size = side_size;
    return obj;
}

mtrx multi_matrix_(mtrx matrix_f, mtrx matrix_s)
{
    mtrx obj;
    obj.side_size = matrix_f.side_size;

    double** res_matrix = new double* [obj.side_size];
    for (size_t i = 0; i < obj.side_size; i++)
    {
        res_matrix[i] = new double[obj.side_size];
        for (size_t j = 0; j < obj.side_size; j++)
        {
            res_matrix[i][j] = 0;
            for (size_t k = 0; k < obj.side_size; k++)
            {
                res_matrix[i][j] += matrix_f.matrix[i][k] * matrix_s.matrix[k][j];
            }
        }
    }
    obj.matrix = res_matrix;

    return obj;
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
    double *f_matrix_line;
    double *s_matrix_line;
    double *res_matrix_line;
    int count;
    int side_size;
    int start;
    int end;
} ARG[100];

void* parallel_multi_matrix_(void *pargs)
{
    struct matrix_args *args = (struct matrix_args *) pargs;

    int l, m;
    double tmp = 0;
    double t0 = dtime();

    double *f_matrix_l = args->f_matrix_line;
    double *s_matrix_l = args->s_matrix_line;
    double *res_matrix_l = args->res_matrix_line;

    int count = args->count;
    int side_size = args->side_size;
    int start = args->start;
    int end = args->end;

    for(int i = start; i < end; i++)
    {
        m = i * side_size;
        for(int j = 0; j < side_size; j++)
        {
            l = 0;
            for(int k = 0; k < side_size; k++)
            {
                tmp += f_matrix_l[i + l] * s_matrix_l[j + k * side_size];
                l += side_size;
            }
            res_matrix_l[count++] = tmp;
            tmp = 0;
        }
    }
    for(int i = 0; i < side_size * side_size; i++)
    {
        std::cout << "\n" << res_matrix_l[i] << "\t";
    }

    args->res_matrix_line = res_matrix_l;
    printf("thread works %fs\n", dtime() - t0);
    pthread_exit(nullptr);
}
double *alloc_matrix(int size)
{
    double *matrix = (double*)malloc(size * size * sizeof(double));
    return matrix;
}

int main()
{
    c_reate_file();

    const std::string f_path = "/Users/vladimirskobcov/Desktop/lab/lab_sys_proga/lab_3/input_f.txt";
    const std::string s_path = "/Users/vladimirskobcov/Desktop/lab/lab_sys_proga/lab_3/input_s.txt";

    mtrx f_matrix = create_matrix(f_path);
    mtrx s_matrix = create_matrix(s_path);

    print_matrix_(f_matrix);
    print_matrix_(s_matrix);

    std::cout << "\nSequential matrix multiplication" << std::endl;
    mtrx res_matrix = multi_matrix_(f_matrix, s_matrix);
    std::cout << "Matrix multiplication was successful" << std::endl;
    print_matrix_(res_matrix);
    std::cout << "Parallel matrix multiplication" << std::endl;
    mtrx res_matrix_p = parallel_multi_matrix_(f_matrix, s_matrix);
    std::cout << "Matrix multiplication was successful" << std::endl;


mtrx res_p_matrix;
res_p_matrix.side_size = 3;

int side_size = 3;
int id;
int THR_NUM,THR;
double *f_line = alloc_matrix(side_size),
        *s_line= alloc_matrix(side_size),
        *res_line= alloc_matrix(side_size);

printf("Введите число потоков\n");
scanf("%i",&THR);
THR_NUM = THR;

pthread_t threads[THR_NUM];
pthread_attr_t attr;

printf("allocating\n");
srand(time(NULL));

for(int i = 0; i < side_size; i++)
{
for(int j = 0; j < side_size; j++)
{
f_line[i + j * side_size] = f_matrix.matrix[i][j];
s_line[i * side_size + j] = s_matrix.matrix[i][j];
}
}
printf("multiplying\n");
double t0 = dtime();
pthread_attr_init(&attr);
pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

int step = (int)((double)side_size/(double)THR_NUM);
int pos = 0;
int count = 0;

for(int k = 0; k < THR_NUM; k++)
{
ARG[k].f_matrix_line = f_line;
ARG[k].s_matrix_line = s_line;
ARG[k].res_matrix_line = res_line;
ARG[k].side_size = side_size;
ARG[k].count = count;
ARG[k].start = pos;
pos += step;
ARG[k].end = (k == THR_NUM - 1) ? side_size : pos;
id = pthread_create(&threads[k], &attr, parallel_multi_matrix_, (void *)&ARG[k]);
if(id){
fprintf(stderr, "Error creating thread\n");
exit(1);
}
}

pthread_attr_destroy(&attr);

for(int k = 0; k < THR_NUM; k++)
pthread_join(threads[k], nullptr);
printf("time for multiplying: %f\n", dtime()-t0);


// print_matrix_(res_p_matrix);

pthread_exit(nullptr);

return 0;
}
 */
