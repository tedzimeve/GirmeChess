#include "GirmeServer.h"

namespace Girme {
    
    server::server(ILogger& log) : log(log) {
        //WSAStartup initialization
        WSAData wsaData;
        WORD DLLVersion = MAKEWORD(2, 1);
        if(WSAStartup(DLLVersion, &wsaData) != 0) {
            log /= "Error in WSAStartup";
            exit(1);
        }
        // инициализация прослущивающего сокета
        sizeofaddr = sizeof(addr);
        addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        addr.sin_port = htons(8080);
        addr.sin_family = AF_INET;
        //само открытие и прослушивание сокета
        sListen = socket(AF_INET, SOCK_STREAM, 0);
        bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
        if(sListen == 0) {
            log /= "Error in create socket";
        }
        log += "initialized";
    }

    int server::start(server::ClientConnectionHandler clienthandler) {
        SOCKET newConnection;

        /*if(newConnection == 0) {
            std::cout << "Error #2\n";
        } else {
            std::cout << "Client Connected!\n";
            char msg[256] = "Hello. It`s my first network program!";
            //send(newConnection, msg, sizeof(msg), NULL);
        }*/
        listen(sListen, SOMAXCONN);
        log *= "clients acceptor started";
        while(true) {
            newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);
            if(newConnection == 0)
                clienthandler(newConnection);
            else
                log /= "Error in ";
        }
    }
} // namespace Girme