#pragma once
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <string_view>
#include <string>
#include <sstream>
#include <memory>
#include "termcolor.hpp"
/**
 * @brief 
 * передав его в stringbuildr ты можешь перевести его в строку, то есть
 * stringbuilder() << dostring(); вернёт уже строку а не stringbuilder& 
 */
class dostring {};
/// Составляет строку. stringbuilder % string1 % string2 ... % int1 % string... << dostring() 
class stringbuilder {
public:
    std::stringstream ss;

    stringbuilder& operator=(const stringbuilder& sb) = delete;
    stringbuilder& operator=(stringbuilder& sb) {
        ss.clear();
        ss << (std::string)sb;
        return *this;
    }
    std::string operator<<(dostring&&) {
        return (std::string)(*this);
    }
    template<typename T>
    stringbuilder& operator%(T o) {
        ss << o;
        return *this;
    }
    operator std::string () {
        std::string ret;
        ss >> ret;
        return ret;
    }
};
/// типы вывода в лог
enum eLogType { Info=0, Warning=1, Error=2, Success };
class LoggerStated;
class MLoggerStated;
/// интерфейс логгера на случай если будет больше видов логгера (например в отдельный файл) (небудет)
class ILogger {
public:
    virtual void log(eLogType, std::string) = 0;

    ILogger& operator() (eLogType e, std::string Message) {
        log(e, Message);
        return *this;
    }
    ILogger& operator+= (std::string Message) {
        log(eLogType::Info, Message);
        return *this;
    }
    ILogger& operator/= (std::string Message) {
        log(eLogType::Error, Message);
        return *this;
    }
    ILogger& operator*= (std::string Message) {
        log(eLogType::Success, Message);
        return *this;
    }

    LoggerStated operator% (eLogType e);
    LoggerStated operator+ (std::string a);
    LoggerStated operator/ (std::string a);
    LoggerStated operator* (std::string a);
    LoggerStated operator+ (char a);
    LoggerStated operator/ (char a);
    LoggerStated operator* (char a);
    LoggerStated operator+ (int a);
    LoggerStated operator/ (int a);
    LoggerStated operator* (int a);
    MLoggerStated operator^ (eLogType e);
};
/** 
 * @brief
 * Временный логгер для составления строки и по окончании отправки её в лог.
 * Сообщение отправляется только если объект удалится!
 * 
 * @example LoggerStatedExample.cpp
 */
class LoggerStated {
    ILogger* logger;
    eLogType _e;
    std::string _mess;
    bool _exec = true;
public:
    LoggerStated(ILogger* logger, eLogType e) : logger(logger), _e(e) {}
    LoggerStated(ILogger* logger, eLogType e, std::string Message): logger(logger), _e(e), _mess(Message) {}

    void log(eLogType e, std::string Message) {
        logger->log(e, Message);
    }
    template<typename T>
    LoggerStated operator%(T e) {
        _exec = false;
        return LoggerStated(logger, _e, stringbuilder() % _mess % e <<dostring());
    };
    template<typename T>
    LoggerStated operator<<(T e) {
        _exec = false;
        return LoggerStated(logger, _e, stringbuilder() % _mess % e << dostring());
    };
    template<typename T>
    LoggerStated operator+(T e) {
        _exec = false;
        return LoggerStated(logger, eLogType::Info, stringbuilder() % e << dostring());
    };
    template<typename T>
    LoggerStated operator/(T e) {
        _exec = false;
        return LoggerStated(logger, eLogType::Error, stringbuilder() % e << dostring());
    };
    template<typename T>
    LoggerStated operator*(T e) {
        _exec = false;
        return LoggerStated(logger, eLogType::Success, stringbuilder() % e << dostring());
    };
    virtual ~LoggerStated() {
        if(_exec)
            logger->log(_e, _mess);
    }
};
/** 
 * @brief
 * Временный логгер для составления строки и по окончании отправки её в лог.
 * Сообщение отправляется только если объект удалится!
 * 
 * @example LoggerStatedExample.cpp
 */
class MLoggerStated {
    ILogger* logger;
    eLogType _e;
    stringbuilder _mess;
    bool exec;
public:
    MLoggerStated(ILogger* logger, eLogType e) : logger(logger), _e(e) {}
    MLoggerStated(ILogger* logger, eLogType e, std::string Message): logger(logger), _e(e) {
        _mess % Message;
    }
    MLoggerStated(const MLoggerStated&) = delete;
    MLoggerStated& operator=(const MLoggerStated&) = delete;
    MLoggerStated& operator=(MLoggerStated& slogger) {
        slogger.exec = false;
        _mess = slogger._mess;
        _e = slogger._e;
        logger = slogger.logger;
        return *this;
    }
    MLoggerStated& operator=(MLoggerStated&& slogger) {
        slogger.exec = false;
        _mess = slogger._mess;
        _e = slogger._e;
        logger = slogger.logger;
        return *this;
    }

    void log(eLogType e, std::string Message) {
        logger->log(e, Message);
    }
    template<typename T>
    MLoggerStated& operator%(T e) {
        _mess << e;
        return *this;
    };
    template<typename T>
    MLoggerStated& operator<<(T e) {
        _mess % e;

        return *this;
    };
    virtual ~MLoggerStated() {
        logger->log(_e, _mess << dostring());
    }
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
        log(0, eLogType::Warning, "DomainLoggerConstuctor", "created logm");
    }
    //#define LogWithLoggerNidd
    #ifndef LogWithLoggerNidd
    void log(int LoggerNidCounter, eLogType e, const std::string Name, std::string Message) {
        std::unique_lock<std::mutex> lock(mtx);
        switch (e) {
        case eLogType::Error:
            std::cout << termcolor::red
                 << std::string{TypeNames[e]} << " " << Name << ": "
                 << Message << termcolor::reset <<"\n";
            break;
        case eLogType::Info:
            std::cout << termcolor::bright_blue
                 << std::string{TypeNames[e]} << " " << Name << ": "
                 << Message << termcolor::reset << "\n";
            break;
        case eLogType::Success:
            std::cout << termcolor::green
                 << std::string{TypeNames[e]} << " " << Name << ": "
                 << Message << termcolor::reset << "\n";
            break;
        case eLogType::Warning:
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