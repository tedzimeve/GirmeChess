#pragma once
#include <stdio.h>
#include <tchar.h>
#include <SDKDDKVer.h>
#include <winsock2.h>
#include <iostream>
#include <string>
#include "LoggerM.hpp"
//странно, но в VSCode любой блок через #ifdef выделяется нерабочим

namespace Girme {
    class server {
        SOCKET sListen;
        SOCKADDR_IN addr;
        int sizeofaddr;
        ILogger& log;
    public:
        typedef void(*ClientConnectionHandler)(SOCKET);


        server(ILogger&);
        int start(ClientConnectionHandler);

    };
} // namespace Girme