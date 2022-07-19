#pragma once
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <string_view>
#include <string>
#include <sstream>
#include <memory>
#define TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES
//#define TERMCOLOR_AUTODETECTED_IMPLEMENTATION
#include "termcolor.hpp"

/// типы вывода в лог
enum eLogType { lInfo=0, lWarning=1, lError=2, lSuccess=3 };
/// интерфейс логгера на случай если будет больше видов логгера (например в отдельный файл) (небудет)
class ILogger {
public:
    virtual void log(eLogType, std::string) = 0;

    ILogger& operator() (eLogType e, std::string Message) {
        log(e, Message);
        return *this;
    }
};
class StatedLog {
private:
    ILogger& m_log;
    const eLogType m_e;
    std::string m_mess = "";
public:
    StatedLog(ILogger& l_, eLogType e) : m_log(l_), m_e(e) {}
    StatedLog(ILogger* l_, eLogType e) : m_log(*l_), m_e(e) {}

    template<typename T>
    StatedLog& operator<< (T input) {
        m_mess += std::to_string(input);
        return *this;
    }
    
    StatedLog& operator<< (const char* input) {
        m_mess += std::string(input);
        return *this;
    }

    virtual ~StatedLog() {
        m_log.log(m_e,m_mess);
    }
};
class Success : public StatedLog {
public:
    Success(ILogger& l_) : StatedLog(l_,eLogType::lSuccess) {}
    Success(ILogger* l_) : StatedLog(l_,eLogType::lSuccess) {}
};
class Info : public StatedLog {
public:
    Info(ILogger& l_) : StatedLog(l_,eLogType::lInfo) {}
    Info(ILogger* l_) : StatedLog(l_,eLogType::lInfo) {}
};
class Error : public StatedLog {
public:
    Error(ILogger& l_) : StatedLog(l_,eLogType::lError) {}
    Error(ILogger* l_) : StatedLog(l_,eLogType::lError) {}
};
class Warning : public StatedLog {
public:
    Warning(ILogger& l_) : StatedLog(l_,eLogType::lWarning) {}
    Warning(ILogger* l_) : StatedLog(l_,eLogType::lWarning) {}
};


class LoggerInstance;
class DomainLogger {
public:
    std::mutex mtx;
    unsigned long int LoggerNidCounter = 1000;
    std::vector<std::unique_ptr<LoggerInstance>> instances;
    //static constexpr std::string_view TypeNames[] = { "\033[94m[INFO]\0", "\033[33m[WARNING]\0", "\033[31m[ERROR]\0", "\033[32m[SUCCESS]\0" };
    static constexpr std::string_view TypeNames[] = { "[INFO]\0", "[WARNING]\0", "[ERROR]\0", "[SUCCESS]\0" };

    DomainLogger() {
        log(0, eLogType::lWarning, "DomainLoggerConstuctor", "created logm");
    }
    //#define LogWithLoggerNidd
    #ifndef LogWithLoggerNidd
    void log(int LoggerNidCounter, eLogType e, const std::string Name, std::string Message) {
        std::unique_lock<std::mutex> lock(mtx);
        switch (e) {
        case eLogType::lError:
            std::cout << termcolor::red
                 << std::string{TypeNames[e]} << " " << Name << ": "
                 << Message << termcolor::reset <<"\n";
            break;
        case eLogType::lInfo:
            std::cout << termcolor::bright_blue
                 << std::string{TypeNames[e]} << " " << Name << ": "
                 << Message << termcolor::reset << "\n";
            break;
        case eLogType::lSuccess:
            std::cout << termcolor::green
                 << std::string{TypeNames[e]} << " " << Name << ": "
                 << Message << termcolor::reset << "\n";
            break;
        case eLogType::lWarning:
            std::cout << termcolor::yellow
                 << std::string{TypeNames[e]} << " " << Name << ": "
                 << Message << termcolor::reset << "\n";
            break;
        default:
            break;
        }
        //mtx.unlock();
        //printf("%.*s %s: %s \033[0m\n", static_cast<int>(TypeNames[e].length()), TypeNames[e].data(), Name.c_str(), Message.c_str());
    }
    #else
    void log(int LoggerNidCounter, eLogType e, const std::string Name, std::string Message) {
        std::unique_lock<std::mutex>(mtx);
        printf("%d %.*s %s: %s \033[0m\n", LoggerNidCounter, static_cast<int>(TypeNames[e].length()), TypeNames[e].data(), Name.c_str(), Message.c_str());
    }
    #endif
    ILogger& createInstance(std::string name);
};
class LoggerInstance : public ILogger {
public:
    int LoggerNid = -1;
    DomainLogger& main;
    const std::string name;

    LoggerInstance(DomainLogger& logm, int LoggerNid, std::string name) : main(logm), LoggerNid(LoggerNid), name(name) {}
    virtual void log(eLogType e, std::string Message) {
        main.log(LoggerNid, e, name, Message);
    }
};