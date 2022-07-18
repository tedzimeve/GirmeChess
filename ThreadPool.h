#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <memory>
#include <functional>
#include "LoggerM.h"

struct Task;
typedef std::shared_ptr<Task> task_t;
typedef void task_f(task_t);
typedef std::function<task_f> task_ft;

class ThreadPool;
struct Task {
    ThreadPool* pool;
    //std::shared_ptr<WorkTimeReport> wtr;
    void* obj;
    task_ft func;

    Task(task_ft f, void* _obj) : obj(_obj), func(f) {}

    void link(ThreadPool* _pool) {
        pool = _pool;
    }
    void set(void* _obj) {
        obj = _obj;
    }
    void queue(task_ft f, void* _obj);
    template<typename T>
    auto _set (T* _ptr) {
        return obj = static_cast<void*>(_ptr);
    }
    template<typename T>
    T* get () {
        return static_cast<T*>(obj);
    }
    void* test;
    void* ping() {
        test = obj;
        return test;
    }
};
class ThreadPool {
    std::vector<std::thread> threads;
    size_t count_of_threads = 0;
    std::mutex mtx;
    std::condition_variable cond;
    DomainLogger& logm;
    ILogger& log;
public:
    ThreadPool(DomainLogger& dlogger, ILogger& logger) : logm(dlogger), log(logger) {
        logger *= "initialized";
    }
private:
    std::queue<std::shared_ptr<Task>> _tasks;
    struct ThreadWorkerData {
        ThreadPool* pool;
        ILogger& log;
    };
    static void* ThreadPoolWorker(ThreadWorkerData* data);
public:
    task_t getTask() {

        task_t ret; // так как это shared_ptr - то всё нормально
        //далее ожидаем пока не появится какая-либо задача
        std::unique_lock lck(mtx);
        log += "Thread lock";
        try{
            try {
                while(_tasks.empty()) {
                    cond.wait(lck);
                }
            } catch (...) {
                log / "error incident in getTask while waiting to Task at " / __LINE__ / "in" / __FILE__;
            }
            
            //как отловим - выполнить следующее:
            ret = _tasks.front(); //получить первый элемент
            ret->ping();
        } catch (...) {
            ret = getTask();
                log / "error incident in getTask in the getting Task" / __LINE__ / "in" / __FILE__;
        }

        _tasks.pop();//удалить первый элемент
        return ret;
    }
    int addTask(task_ft f, void* _obj) {
        /*std::cout << "Add task" << std::endl;
        pthread_mutex_lock(&pmt);
        _tasks.emplace(new Task(f, _wtr, _obj));//добавить в конец
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&pmt);*/
        return 0;
    }
    int addTask(task_t task) {
        /*std::cout << "Add task" << std::endl;
        pthread_mutex_lock(&pmt);
        _tasks.emplace(task);//добавить в конец
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&pmt);*/
        return 0;
    }

    void start(size_t n) {
        for(; n > 0; --n) {
            threads.emplace_back(ThreadPoolWorker, new ThreadWorkerData{ this, logm.createInstance(stringbuilder()%"threadworker"% ++count_of_threads)}).detach();
        }
    }
};