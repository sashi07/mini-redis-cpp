#pragma once

#include <winsock2.h>

class MiniRedisServer {
private:
    SOCKET serverSocket;
    int port;

public:
    MiniRedisServer(int port);
    ~MiniRedisServer();

    bool start();
    void run();
    void stop();
};