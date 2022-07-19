#include "LoggerM.hpp"

ILogger& DomainLogger::createInstance(std::string name) {
    LoggerInstance* logger = new LoggerInstance(*this, LoggerNidCounter++, name);
    instances.emplace_back(logger);
    return *static_cast<ILogger*>(logger);
};
/*
StatedLog& StatedLog::operator<<(T input) {
    m_mess += std::to_string(input);
    return *this;
}
template<typename T>
StatedLog& StatedLog::operator<< <int>(int input) {
    m_mess += std::to_string(input);
    return *this;
}
template<typename T>
StatedLog& StatedLog::operator<< <const char*>(const char* input){
    m_mess += std::to_string(input);
    return *this;
}
*/
/*

LoggerStated ILogger::operator%(eLogType e) {
    return LoggerStated(this, e);
};

LoggerStated ILogger::operator+(std::string e) {
    return this->operator%(eLogType::Info) % e;
};
LoggerStated ILogger::operator/(std::string e) {
    return this->operator%(eLogType::Error) % e;
};
LoggerStated ILogger::operator*(std::string e) {
    return this->operator%(eLogType::Success) % e;
};

LoggerStated ILogger::operator+(char e) {
    return this->operator%(eLogType::Info) % e;
};
LoggerStated ILogger::operator/(char e) {
    return this->operator%(eLogType::Error) % e;
};
LoggerStated ILogger::operator*(char e) {
    return this->operator%(eLogType::Success) % e;
};

LoggerStated ILogger::operator+(int e) {
    return this->operator%(eLogType::Info) % e;
};
LoggerStated ILogger::operator/(int e) {
    return this->operator%(eLogType::Error) % e;
};
LoggerStated ILogger::operator*(int e) {
    return this->operator%(eLogType::Success) % e;
};


MLoggerStated ILogger::operator^(eLogType e) {
    return MLoggerStated(this, e);
};*/
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