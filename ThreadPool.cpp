#include "ThreadPool.hpp"
    
    void* ThreadPool::ThreadPoolWorker(ThreadWorkerData* data) {
        try{
            ILogger& log = data->log;
            ThreadPool* _tpool = data->pool;
            try {
                Success(log) << "worker started success";
                //log * 't' % 'e' % 's' % 't';
                while(_tpool->iswork) {
                    auto task = _tpool->getTask();
                    Info(log) << "returned task\n";
                    task->link(_tpool, &log);
                    task->func(task);
                }
                Info(log) << "--endthread--";
                data->isActive->isActive = false;
            } catch (std::bad_function_call) {
                Error(log) << " WorkerThread crashed by bad_function_call";
            } catch (std::bad_alloc& e) {
                Error(log) << " WorkerThread crashed std::bad_alloc\n"
                    << "\te.what() \t" << e.what();
            } catch (std::bad_exception& e) {
                std::cout << "[ERROR] WorkerThread crashed std::bad_exception" << std::endl;
                std::cout << e.what();
            } catch (std::bad_typeid& e) {
                std::cout << "[ERROR] WorkerThread crashed std::bad_typeid" << std::endl;
                std::cout << e.what();
            } catch (std::domain_error& e) {
                std::cout << "[ERROR] WorkerThread crashed std::domain_error" << std::endl;
                std::cout << e.what();
            } catch (std::invalid_argument& e) {
                std::cout << "[ERROR] WorkerThread crashed std::invalid_argument" << std::endl;
                std::cout << e.what();
            } catch (std::length_error& e) {
                std::cout << "[ERROR] WorkerThread crashed std::length_error" << std::endl;
                std::cout << e.what();
            } catch (std::out_of_range& e) {
                std::cout << "[ERROR] WorkerThread crashed std::out_of_range" << std::endl;
                std::cout << e.what();
            } catch (std::runtime_error& e) {
                std::cout << "[ERROR] WorkerThread crashed std::runtime_error" << std::endl;
                std::cout << e.what();
            } catch (std::logic_error& e) {
                std::cout << "[ERROR] WorkerThread crashed std::logic_error" << std::endl;
                std::cout << e.what();
            } catch (...) {
                std::cout << "[ERROR] WorkerThread crashed " << std::endl;
            }
        } catch(...) {
            std::cout << "ERORR!!!!!!!!! in ThreadPoolWorker WorkerData Access invalid!!!!";
            //DomainLogger lol;
            //ILogger& log = lol.createInstance("INVALID ACCESS LOGGER");
        }
        std::cout << "---endthread--- " << std::endl;
        return nullptr;
    }
    void Task::queue(task_ft f, void* _obj) {
        pool->addTask(f, _obj);
    }