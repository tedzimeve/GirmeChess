#include "LoggerM.h"

ILogger& DomainLogger::createInstance(std::string name) {
    LoggerInstance* logger = new LoggerInstance(*this, LoggerNidCounter++, name);
    instances.emplace_back(logger);
    return *static_cast<ILogger*>(logger);
};



LoggerStated ILogger::operator%(eLogType e) {
    return LoggerStated(this, e);
};

LoggerStated ILogger::operator+(std::string e) {
    return this->operator%(eLogType::Info) % e;
};
LoggerStated ILogger::operator/(std::string e) {
    return this->operator%(eLogType::Info) % e;
};
LoggerStated ILogger::operator*(std::string e) {
    return this->operator%(eLogType::Info) % e;
};

LoggerStated ILogger::operator+(char e) {
    return this->operator%(eLogType::Info) % e;
};
LoggerStated ILogger::operator/(char e) {
    return this->operator%(eLogType::Info) % e;
};
LoggerStated ILogger::operator*(char e) {
    return this->operator%(eLogType::Info) % e;
};

LoggerStated ILogger::operator+(int e) {
    return this->operator%(eLogType::Info) % e;
};
LoggerStated ILogger::operator/(int e) {
    return this->operator%(eLogType::Info) % e;
};
LoggerStated ILogger::operator*(int e) {
    return this->operator%(eLogType::Info) % e;
};


MLoggerStated ILogger::operator^(eLogType e) {
    return MLoggerStated(this, e);
};
//Далее вариант которые почему-то не выполняются свою функцию даже после дедусинга
/*template<typename T>
LoggerStated ILogger::operator+(T e) {
    return LoggerStated(this, eLogType::Info, stringbuilder() << e);
};
template<typename T>
LoggerStated ILogger::operator/(T e) {
    return LoggerStated(this, eLogType::Error, stringbuilder() << e);
};
template<typename T>
LoggerStated ILogger::operator*(T e) {
    return LoggerStated(this, eLogType::Success, stringbuilder() << e);
};
//далее код который должен был придумать компилятор, но он этого не сделал
template<>
LoggerStated ILogger::operator*<int>(int e) {
    return LoggerStated(this, eLogType::Success, stringbuilder() << e);
};*/