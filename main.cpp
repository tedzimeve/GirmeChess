#include <iostream>
#include "GirmeServer.h"
#include "GetterSetter.h"
#include "ThreadPool.h"

using namespace Girme;
class Program {
    DomainLogger logm;
    ThreadPool pool;
    ILogger& log;
public:

    Program() : pool(logm,logm.createInstance("ThreadPool")), log(logm.createInstance("Program")) {
        log *= "initialized";
        pool.start(10);
    }

    static void ClientsHandler(SOCKET) {

    }
    int start() {
        log *= "started";

        server serv(logm.createInstance("serverthread"));
        serv.start(ClientsHandler);
        //httplib::Server serv;

        //erv.set_mount_point("/", "./www");

        log /= "End of Program";
        return 0;
    }
};

int main() {
    Program prgr;
    prgr.start();
    return 0;
}