#include <iostream>
#include "service\GirmeServer.hpp"
#include "service\ThreadPool.hpp"

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

int main() {
    Program prgr;
    prgr.start();
    while(true);
    return 0;
}