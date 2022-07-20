#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <memory>
#include <functional>
#include "LoggerM.hpp"

struct Task;
typedef std::shared_ptr<Task> task_t;
typedef void task_f(task_t);
typedef std::function<task_f> task_ft;

class ThreadPool;
/// структура хранящая достаточно для Задачи
struct Task {
    ///< пул потоков для новых задач
    ThreadPool* pool;
    //std::shared_ptr<WorkTimeReport> wtr;
    ///< параметры
    void* obj;
    ///< логгер
    ILogger* logger;
    ///< Функтор
    task_ft func;

    template<typename T>
    Task(task_ft f, T* _obj) : obj(static_cast<void*>(_obj)), func(f) {}
    Task(task_ft f, void* _obj, ILogger* log) : obj(_obj), func(f), logger(log) {}

    /// привязать пул потоков
    void link(ThreadPool* _pool, ILogger* _logger) {
        pool = _pool;
        logger = _logger;
    }
    /// вставить аргумент
    template<typename T>
    void set(T* _obj) {
        obj = static_cast<void*>(_obj);
    }
    /// новую задачу в пулл потоков
    void queue(task_ft f, void* _obj);
    /*template<typename T>
    auto _set (T* _ptr) {
        return obj = static_cast<void*>(_ptr);
    }*/
    /// получить аргумент
    template<typename T>
    T* get () {
        return static_cast<T*>(obj);
    }
    /// проверка валидности таски
    void* test;
    auto ping() {
        test = obj;
        return test;
    }
};
struct isActive {
    bool isActive = true;
};
class ThreadPool {
    std::vector<std::thread> threads;
    std::vector<std::shared_ptr<isActive>> opened;
    size_t count_of_threads = 0;
    std::mutex mtx;
    std::condition_variable cond;
    DomainLogger& logm;
    ILogger& log;
    bool iswork = true;
    task_t defaultTask;
    struct testtttt { int counter; testtttt(int c) : counter(c) {} };
    static void TaskTest(task_t t) {
        Warning(t->logger) << "Hello from TaskTest " << t->get<testtttt>()->counter;
        delete t->get<testtttt>();
    }
    static void DefaultTask(task_t t) {
        Warning(t->logger) << "Executed Default Task";
    }
public:
    ThreadPool(DomainLogger& dlogger, ILogger& logger) : logm(dlogger), log(logger),
        defaultTask(new Task(DefaultTask, nullptr, &log)) {
        Success(logger) << "initialized";
    }
private:
    std::queue<std::shared_ptr<Task>> _tasks;
    struct ThreadWorkerData {
        ThreadPool* pool;
        ILogger& log;
        std::shared_ptr<isActive> isActive;
    };
    static void* ThreadPoolWorker(ThreadWorkerData* data);
public:
    task_t getTask() {
        task_t ret; // так как это shared_ptr - то всё нормально, если мы объявим её раньше времени..
        std::unique_lock lck(mtx);
        Info(log) << "Thread lock";
        try{
            //далее ожидаем пока не появится какая-либо задача
            try {
                if(!iswork) {
                    Note(log) << "thread handled that threads will closed";
                    return defaultTask;
                }
                while(_tasks.empty()) {
                    cond.wait(lck);
                    if(!iswork) {
                        Note(log) << "thread handled that threads will closed";
                        return defaultTask;
                    }
                }
            } catch (...) {
                Error(log) << "error incident in getTask while waiting to Task at " << __LINE__ << "in" << __FILE__;
            }
            
            //как узнаем что в очереди есть невыполненные задачи - выполнить следующее:
            try {
                ret = _tasks.front(); //получить первый элемент
                _tasks.pop(); //удалить первый элемент
            } catch (...) {
                Error(log) << "error incident in getTask when dropped the task from Tasks queue" << __LINE__ << "in" << __FILE__ 
                    << "\n\t\t\t we will recursively request to getTask()";
                ret = getTask();
            }
            try {
                ret->ping();
            } catch (...) {
                Error(log) << "error incident in getTask when we pinged a task. Maybe handled NULL task" << __LINE__ << "in" << __FILE__ 
                    << "\n\t\t\t we will recursively request to getTask()";
                ret = getTask();
            }
        } catch (...) {
            Error(log) << "unknown error incident in getTask in the getting Task" << __LINE__ << "in" << __FILE__ 
                << "\n\t\t\t we will recursively request to getTask()";
            ret = getTask();
        }
        Info(log) << "success handled an task";
        return ret;
    }
    template<typename T>
    int addTask(task_ft f, T* _obj) {
        Info(log) << "adding a task " << __LINE__;
        std::unique_lock lck(mtx);
        _tasks.emplace(
            task_t(new Task(f, _obj))
        );//добавить в конец
        cond.notify_one();
        return 0;
    }
    int addTask(task_t task) {
        Info(log) << "adding a task " << __LINE__;
        std::unique_lock lck(mtx);
        _tasks.emplace(task);//добавить в конец
        cond.notify_one();
        return 0;
    }
    void start(const size_t n) {
        for(int i = 0; i < n; ++i) {
            std::shared_ptr<isActive> a = opened.emplace_back(new isActive());
            threads.emplace_back(ThreadPoolWorker, 
                new ThreadWorkerData{ 
                    this, 
                    logm.createInstance("threadworker"+std::to_string(++count_of_threads)),
                    a
                }).detach();
        }
        for(int i = 0; i < n; ++i) {
            addTask(TaskTest, new testtttt(i));
        }
    }
    void join() {
        iswork = false;
        cond.notify_all();
        Warning(this->log) << threads.size() << " threads will canceled";
        for(auto& thr : threads) {
            try{
                thr.join();
            } catch (...) {
                Error(this->log) << "throwed exception on thr.join() at the " << __FILE__ << ':' << __LINE__-2;
            }
            cond.notify_all();
        }
        cond.notify_all();
        for(std::shared_ptr<isActive> isActive : opened) {
            cond.notify_all();
            Note(this->log) << "Waits for end of thread started";
            while(isActive->isActive);
            cond.notify_all();
        }
        Important(this->log) << "Closed all threads";
    }
};