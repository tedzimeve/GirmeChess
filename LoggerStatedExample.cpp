#include "LoggerM.hpp"
int LoggerStatedExample() {
    DomainLogger logm;
    ILogger& log = logm.createInstance("Example");

    // 1.0 разбитый вывод в лог
    //log % eLogType::Info % "Эта " % "строка " % "разбита";
    /* 
     * выводиться будет:
     * [INFO] Example: Эта строка разбита
     */

    // 2.0 разбитый вывод с преднастроенным типом вывода
    //log + "Эта " + "строка " + "информационная";
    //log / "Эта " / "строка " / "ошибочная";
    //log * "Эта " * "строка " * "успешная";
    /* 
     * выводиться будет:
     * [INFO] Example: Эта строка разбита
     * [INFO] Example: Эта строка разбита
     * [INFO] Example: Эта строка разбита
     */

    // 3.0 можно заварить следующую кашу:
    std::vector<std::string> strings;
    //strings.emplace_back("string1");
    //strings.emplace_back("string2");
    //strings.emplace_back("string3");
    {
        ///< создаём экземляр который составит строку для вывода
        //MLoggerStated mlog = log ^ eLogType::Info;
        //составляем её
        //mlog << "Strings in vector: {\n";
        //for(std::string str : strings) {
        //    mlog << "\t" << str << "\n";
        //}
        //mlog << '}';
    }/*// дойдя до границ блока mlog по правилам времени жизни объектов самоуничтожается и выводит строку
    /* 
     * выводиться будет:
     * [INFO] Example: Strings in vector: {
     *      string1
     *      string2
     *      string3
     *  }
     */
    /* 
     * вместо кастыля:
     * [INFO] Example: Strings in vector: {
     * [INFO] Example:      string1
     * [INFO] Example:      string2
     * [INFO] Example:      string3
     * [INFO] Example: }
     * 
     * который к тому же при многопоточности растворится построчно между выводами других Логгеров
     */
    return 0;
}