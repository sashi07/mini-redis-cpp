#include "server.h"

#include <iostream>
#include <string>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

MiniRedisServer::MiniRedisServer(int port)
    : serverSocket(INVALID_SOCKET), port(port) {
}

MiniRedisServer::~MiniRedisServer() {
    stop();
}

bool MiniRedisServer::start() {

    // Initialize Winsock
    WSADATA wsaData;

    int result = WSAStartup(
        MAKEWORD(2, 2),
        &wsaData
    );

    if (result != 0) {
        std::cerr << "WSAStartup failed\n";
        return false;
    }

    // Create TCP socket
    serverSocket = socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP
    );

    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed\n";
        WSACleanup();
        return false;
    }

    // Configure server address
    sockaddr_in serverAddress{};

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(port);

    // Bind socket to IP + port
    result = bind(
        serverSocket,
        reinterpret_cast<sockaddr*>(&serverAddress),
        sizeof(serverAddress)
    );

    if (result == SOCKET_ERROR) {
        std::cerr << "Bind failed\n";
        closesocket(serverSocket);
        WSACleanup();
        return false;
    }

    // Start listening
    result = listen(
        serverSocket,
        SOMAXCONN
    );

    if (result == SOCKET_ERROR) {
        std::cerr << "Listen failed\n";
        closesocket(serverSocket);
        WSACleanup();
        return false;
    }

    std::cout << "MiniRedis server started on port "
              << port
              << std::endl;

    return true;
}

void MiniRedisServer::run() {

    std::cout << "Waiting for client connection..." << std::endl;

    sockaddr_in clientAddress{};
    int clientAddressSize = sizeof(clientAddress);

    SOCKET clientSocket = accept(
        serverSocket,
        reinterpret_cast<sockaddr*>(&clientAddress),
        &clientAddressSize
    );

    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Accept failed" << std::endl;
        return;
    }

    std::cout << "Client connected!" << std::endl;

    char buffer[1024];

    while (true) {

        int bytesReceived = recv(
            clientSocket,
            buffer,
            sizeof(buffer) - 1,
            0
        );

        if (bytesReceived <= 0) {
            std::cout << "Client disconnected." << std::endl;
            break;
        }

        buffer[bytesReceived] = '\0';

        std::cout << "Received: " << buffer << std::endl;

        std::string response = "OK: " + std::string(buffer);

        send(
            clientSocket,
            response.c_str(),
            static_cast<int>(response.size()),
            0
        );
    }

    closesocket(clientSocket);
}

void MiniRedisServer::stop() {

    if (serverSocket != INVALID_SOCKET) {

        closesocket(serverSocket);

        serverSocket = INVALID_SOCKET;

        WSACleanup();
    }
}