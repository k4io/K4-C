#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#include "shared.h"
#include <thread>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "42069"

int main() {
    WSADATA wsaData;
    int iResult;
    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;
    struct addrinfo* result = NULL;
    struct addrinfo hints;
    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for the server to listen for client connections.
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    printf("[~] Listening...\n");

    std::thread([&]() 
        {
            int count = 0;
            while (1) {
                system("cls");
                int lastcount = count;
                for (auto v : sharedPlayersMap) {
                    auto c = v.second;
                    printf(R"(
[====== %s ======]
[User id]: %i
[In game]: %s
[Server name]: %s
[Server ip]: %s:%i
[Position]: (%.2f, %.2f, %.2f)
[Velocity]: (%.2f, %.2f, %.2f)
[Renderer players]: Size (%i)
)", c->name, c->userid, c->ingame ? "true" : "false",
c->servername.c_str(), c->serverip, c->serverport,
c->x, c->y, c->z,
c->vx, c->vy, c->vz, 
c->rendered_players.size());
                }
                SleepEx(100, 0);
            }
        }
    ).detach();

    do {
        // Accept a client socket
        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
            printf("accept failed with error: %d\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            return 1;
        }
        std::thread(
            [&]() 
            {
                auto sockfd = ClientSocket;
                char hellobuffer[1];
                auto result = recv(sockfd, hellobuffer, 1, 0);
                if (result != 1 && hellobuffer[0] != 'M') return;
                char buffer[512]{};
                memset(&buffer, '\xA6', 512);
                buffer[1] = 'H';
                send(sockfd, buffer, 512, 0);
                printf("[~+] Accepted client!\n");
                shPlayerData* cl = nullptr;
                while (1) {
                    try {
                        SleepEx(300, 0);
                        //receive information about client
                        //if (result != 512)
                        //    break;

                        std::string cs = "";
                        for (size_t i = 0; i < 512; i += 8) {
                            char n[8];
                            result = recv(sockfd, n, 8, 0);
                            for (size_t j = 0; j < 8; j++) {
                                buffer[i + j] = n[j];
                            }
                            cs += n;
                        }

                        cl = shPlayerData::deserialize(buffer);

                        memset(&buffer, '\xA6', 512);
                        recv(sockfd, buffer, 512, 0);
                        std::string sz_c = "";
                        for (size_t i = 0; i < 512; i++) {
                            if (buffer[i] == '\xA6') break;
                            sz_c += buffer[i];
                        }
                        auto count = stoi(sz_c);

                        if (count > 0) {
                            for (size_t i = 0; i < count; i++) {
                                memset(&buffer, '\xA6', 512);
                                recv(sockfd, buffer, 512, 0);
                                cl->rendered_players.push_back(shPlayerData::deserialize(buffer));
                            }
                        } 
                        else 
                            cl->rendered_players = {};

                        if (sharedPlayersMap.count(cl->userid) < 1)
                            sharedPlayersMap.insert(std::make_pair(cl->userid, cl));
                        else sharedPlayersMap[cl->userid] = cl;
                        //send count of current clients
                        memset(&buffer, '\xA6', 512);
                        auto c_sz = std::to_string(sharedPlayersMap.size() - 1);
                        for (size_t i = 0; i < c_sz.size(); i++) {
                            buffer[i] = c_sz[i];
                        }
                        send(sockfd, buffer, 512, 0);
                        if (c_sz == "0") continue;

                        //send current clients
                        for (auto pair : sharedPlayersMap) {
                            if (pair.first != cl->userid) { //make sure not to send current client
                                send(sockfd, pair.second->serialize().c_str(), 512, 0);
                            }
                        }
                    }
                    catch (...) {
                        if (cl &&
                            sharedPlayersMap.count(cl->userid) > 0)
                            sharedPlayersMap.erase(cl->userid);
                        closesocket(sockfd);
                        return;
                    }
                }
                closesocket(sockfd);
                return;
            }
        ).detach();
    } while (1);


    // No longer need server socket
    closesocket(ListenSocket);

    // Receive until the peer shuts down the connection
    do {

        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            printf("Bytes received: %d\n", iResult);

            // Echo the buffer back to the sender
            iSendResult = send(ClientSocket, recvbuf, iResult, 0);
            if (iSendResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                return 1;
            }
            printf("Bytes sent: %d\n", iSendResult);
        }
        else if (iResult == 0)
            printf("Connection closing...\n");
        else {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }

    } while (iResult > 0);

    // shutdown the connection since we're done
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }

    // cleanup
    closesocket(ClientSocket);
    WSACleanup();

    return 0;
}