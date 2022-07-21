#include <iostream>
#include "service\GirmeServer.hpp"
#include "service\ThreadPool.hpp"
#include "service\HTTPParseBase.hpp"

using namespace Girme;
class Program {
    DomainLogger logm;
    ThreadPool pool;
    ILogger& log;
public:

    Program() : pool(logm,logm.createInstance("ThreadPool")), log(logm.createInstance("Program")) {
        Success(log) << "initialized";
        pool.start(2);
    }

    static void ClientsHandler(SOCKET sock) {
        /*
         * create task to recieve contents from sock
         *
         */
    }
    int start() {
        Success(log) << "started";

        //server serv(logm.createInstance("serverthread"));
        //serv.start(ClientsHandler);
        
        //httplib::Server serv;
        //erv.set_mount_point("/", "./www");

        Important(log) << "MainThread goto line of end of Programm";
        Important(log) << "MainThread goto line of end of Programm";
        Note(log) << "we executed pool.join()";
        pool.join();
        Important(log) << "MainThread goto line of end of Programm";
        Important(log) << "MainThread goto line of end of Programm";
        Note(log) << "we executed while(true)";
        return 0;
    }
};
using namespace GirmeServer;
int main() {
    /*Program prgr;
    prgr.start();
    while(true);*/
    __RequestParseStated reqstated;
    std::vector<std::string> partOfHttpDocument = {
        "GET /index.html http/v1.1\r\n" 
        "Host: localhost:8080\r\n"
        "Content-Length: ","10\r\n"
        "h4: v1\r\n\r","\n",
        "1234567890"
    };
    for (auto send : partOfHttpDocument) {
        Girme::GirmeServer::HTTPParser::parse(reqstated, send);
    }
    
    return 0;
}