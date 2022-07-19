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
    std::shared_ptr<void> obj;
    task_ft func;

    template<typename T>
    Task(task_ft f, std::shared_ptr<T> _obj) : obj(std::static_pointer_cast<void, T>(_obj)), func(f) {}

    void link(ThreadPool* _pool) {
        pool = _pool;
    }
    template<typename T>
    void set(std::shared_ptr<T> _obj) {
        obj = std::static_pointer_cast<void, T>(_obj);
    }
    void queue(task_ft f, void* _obj);
    template<typename T>
    auto _set (T* _ptr) {
        return obj = static_cast<void*>(_ptr);
    }
    template<typename T>
    std::shared_ptr<T> get () {
        return std::static_pointer_cast<T, void>(obj);
    }
    std::shared_ptr<void> test;
    auto ping() {
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
        task_t ret; // так как это shared_ptr - то всё нормально, если мы объявим её раньше времени..
        std::unique_lock lck(mtx);
        log += "Thread lock";
        try{
            //далее ожидаем пока не появится какая-либо задача
            try {
                while(_tasks.empty()) {
                    cond.wait(lck);
                }
            } catch (...) {
                log / "error incident in getTask while waiting to Task at " / __LINE__ / "in" / __FILE__;
            }
            
            //как узнаем что в очереди есть невыполненные задачи - выполнить следующее:
            try {
                ret = _tasks.front(); //получить первый элемент
                _tasks.pop(); //удалить первый элемент
            } catch (...) {
                log / "error incident in getTask when dropped the task from Tasks queue" / __LINE__ / "in" / __FILE__ 
                    / "\n\t\t\t we will recursively request to getTask()";
                ret = getTask();
            }
            try {
                ret->ping();
            } catch (...) {
                log / "error incident in getTask when we pinged a task. Maybe handled NULL task" / __LINE__ / "in" / __FILE__ 
                    / "\n\t\t\t we will recursively request to getTask()";
                ret = getTask();
            }
        } catch (...) {
            log / "unknown error incident in getTask in the getting Task" / __LINE__ / "in" / __FILE__ 
                / "\n\t\t\t we will recursively request to getTask()";
            ret = getTask();
        }
        log + "success handled an task";
        return ret;
    }
    template<typename T>
    int addTask(task_ft f, T* _obj) {
        log + "adding a task " + __LINE__;
        std::unique_lock lck(mtx);
        _tasks.emplace(new Task(f, std::make_shared<T>(_obj)));//добавить в конец
        cond.notify_one();
        return 0;
    }
    int addTask(task_t task) {
        log + "adding a task " + __LINE__;
        std::unique_lock lck(mtx);
        _tasks.emplace(task);//добавить в конец
        cond.notify_one();
        return 0;
    }
    struct testtttt {
        ILogger& log;
        int counter;
        testtttt(ILogger& log_, int c) : log(log_), counter(c) {}
        /*testtttt(ILogger& log_, int& c) : log(log_), counter(c) {}
        testtttt(ILogger& log_, const int& c) : log(log_), counter(c) {}
        testtttt(ILogger& log_, int&& c) : log(log_), counter(c) {}*/
    };
    static void TaskTest(task_t t) {
        std::shared_ptr<testtttt> v = t->get<testtttt>();
        v->log * "Hello from TaskTest" * v->counter;
    }
    void start(size_t n) {
        for(; n > 0; --n) {
            threads.emplace_back(ThreadPoolWorker, new ThreadWorkerData{ this, logm.createInstance(stringbuilder()%"threadworker"% ++count_of_threads)}).detach();
        }
        for(int i = 0; i < 2; ++i) {
            addTask(task_t(
                new Task(
                    task_ft(TaskTest), 
                    std::shared_ptr<testtttt>(
                        new testtttt(log, i)
                        )
                    )
                ));
        }
    }
};