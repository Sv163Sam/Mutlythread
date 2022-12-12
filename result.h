#ifndef _3_RESULT_H
#define _3_RESULT_H

#include <cstdio>
#include <random>
#include <ctime>
#include <iostream>
#include <pthread.h>
#include <fstream>
#include <sys/time.h>
#include <cstdlib>
#include <optional>
#include <queue>
#include <unistd.h>
#include <sys/errno.h>
#define MAX_SIZE 5

template <typename T>
struct optional {
private:
    bool _has_value;
    T _value;
public:
    optional() : _has_value{false}, _value{} {}
    explicit optional(T v) : _has_value{true}, _value{v} {}
    bool has_value() const {return _has_value;}
    T value() const
    {
        if (_has_value)
            return _value;
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

struct TEMP
{
    int m;
} ARG;

#endif //_3_RESULT_H

/*
 * void enqueue(const T &v)
    {
        T a = v;
        while (!full())
        {
            if (!INTERRUPT_FLAG)
            {
                pthread_mutex_lock(&mutex);
                if (!full())
                {
                    queue.push(a++);
                    pthread_cond_signal(&cond);
                    pthread_mutex_unlock(&mutex);
                }
                else
                {
                    pthread_cond_signal(&cond);
                    pthread_cond_wait(&cond_w, &mutex);
                    pthread_mutex_unlock(&mutex);
                    //throw std::logic_error("ARRAY IS FULL");
                }
            }
            else
            {
                pthread_cond_signal(&cond);
                pthread_mutex_unlock(&mutex);
                return;
            }
        }
        pthread_mutex_unlock(&mutex);
    }

    T dequeue()
    {
        T elem = -1;
        pthread_mutex_lock(&mutex);
        while (!empty())
        {
            if (INTERRUPT_FLAG && !empty())
            {
                while (!empty())
                {
                    elem = queue.front();
                    std::cout << "GET: " << elem << std::endl;
                    queue.pop();
                }
                pthread_mutex_unlock(&mutex);
                break;
            }
            if (!empty())
            {
                elem = queue.front();
                std::cout << "Received: " << elem << std::endl;
                queue.pop();
                pthread_cond_signal(&cond_w);
                pthread_mutex_unlock(&mutex);
                //return elem;
            }
            else
            {
                std::cout << "wait\n" << std::endl;
                //pthread_cond_signal(&cond_w);
                pthread_cond_wait(&cond, &mutex);
                pthread_mutex_unlock(&mutex);
            }
        }
        if (empty() && INTERRUPT_FLAG)
            return -2;
        //optional<T> opt(elem);
        //return opt;
        //pthread_mutex_unlock(&mutex);
        //optional<T> op(-1);
        //return op;
        pthread_mutex_unlock(&mutex);
        return elem;
        //return optional<T>();
    }

    optional<T> try_dequeue()
    {
        T elem = -1;
        pthread_mutex_lock(&mutex);
        while (!empty())
        {
            if (INTERRUPT_FLAG && !empty())
            {
                while (!empty())
                {
                    elem = queue.front();
                    std::cout << "Received: " << elem << std::endl;
                    queue.pop();
                }
                pthread_mutex_unlock(&mutex);
                return optional<T>{};
            }
            if (!empty())
            {
                elem = queue.front();
                std::cout << "Received: " << elem << std::endl;
                queue.pop();
                pthread_cond_signal(&cond_w);
                pthread_mutex_unlock(&mutex);
            }
            else
            {
                pthread_cond_signal(&cond_w);
                pthread_cond_wait(&cond, &mutex);
                pthread_mutex_unlock(&mutex);
            }
        }
        if (empty() && INTERRUPT_FLAG)
        {
            optional<T> op(-1);
            return op;
        }
        pthread_mutex_unlock(&mutex);
        optional<T> opt(elem);
        return opt;
    }

    bool try_enqueue(const T &v)
    {
        T a = v;
        pthread_mutex_lock(&mutex);
        while (!full())
        {
            if (!INTERRUPT_FLAG)
            {
                if (!full())
                {
                    queue.push(a++);
                    pthread_cond_signal(&cond);
                    pthread_mutex_unlock(&mutex);
                }
                else
                {
                    pthread_cond_signal(&cond);
                    pthread_cond_wait(&cond_w, &mutex);
                    pthread_mutex_unlock(&mutex);
                    //throw std::logic_error("ARRAY IS FULL");
                }
            }
            else
            {
                pthread_cond_signal(&cond);
                pthread_mutex_unlock(&mutex);
                return false;
            }
        }
        pthread_mutex_unlock(&mutex);
        return true;
    }*/