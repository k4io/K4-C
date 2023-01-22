#pragma once
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "42069"
#define SERVNAME "192.168.1.1"
// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#include "../../vars.hpp"

namespace sharedesp {
	bool connected = false;
	int sockfd = INVALID_SOCKET;

	void connectionthread() {
		struct addrinfo* result = NULL,
			* ptr = NULL,
			hints;

		WSAData dat;
		if (WSAStartup(MAKEWORD(2, 2), &dat) != 0)
			return;

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		if (getaddrinfo(0, DEFAULT_PORT, &hints, &result) != 0)
			return;	

		int iresult;
		connected = false;
		for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

			// Create a SOCKET for connecting to server
			sockfd = socket(ptr->ai_family, ptr->ai_socktype,
				ptr->ai_protocol);
			if (sockfd == INVALID_SOCKET) {
				printf("socket failed with error: %ld\n", WSAGetLastError());
				WSACleanup();
				return;
			}

			// Connect to server.
			iresult = connect(sockfd, ptr->ai_addr, (int)ptr->ai_addrlen);
			if (iresult == SOCKET_ERROR) {
				closesocket(sockfd);
				sockfd = INVALID_SOCKET;
				continue;
			}
			connected = true;
			break;
		}
		freeaddrinfo(result);
		//sockfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

		if (connected) {
			//send hello
			//send current client information
			//receive client count
			//if 0, start again
			//if more than 0, iterate N times and deserialize data

			char hellobuffer[1]{ 'M' };
			send(sockfd, hellobuffer, 1, 0);
			char buffer[512];
			recv(sockfd, buffer, 512, 0);
			if (buffer[1] != 'H') return;
			while (1) {
				try {
					SleepEx(300, 0);
					auto sb = vars->currentPlayerData->serialize();
					for (size_t i = 0; i < 512; i += 8) {
						char n[8];
						for (size_t j = 0; j < 8; j++) {
							n[j] = sb[i + j];
						}
						send(sockfd, n, 8, 0);
					}

					//send visible players list if any
					std::string sz_r = std::to_string(vars->currentPlayerData->rendered_players.size());
					memset(&buffer, '\xA6', 512);
					for (size_t i = 0; i < sz_r.size(); i++) {
						buffer[i] = sz_r[i];
					}
					send(sockfd, buffer, 512, 0);
					memset(&buffer, '\xA6', 512);
					for (size_t i = 0; i < vars->currentPlayerData->rendered_players.size(); i++) {
						send(sockfd, vars->currentPlayerData->rendered_players[i]->serialize().c_str(), 512, 0);
					}

					recv(sockfd, buffer, 512, 0);
					std::string c = "";
					for (size_t i = 0; i < 512; i++) {
						if (buffer[i] == '\xA6') break;
						c += buffer[i];
					}
					memset(&buffer, '\xA6', 512);
					int count = stoi(c);
					auto temp_map = std::map<ULONG, shPlayerData*>{};
					for (size_t i = 0; i < count; i++) {
						recv(sockfd, buffer, 512, 0);
						auto dat = shPlayerData::deserialize(buffer);
						temp_map.insert(std::make_pair(dat->userid, dat));
						memset(&buffer, '\xA6', 512);
					}
				}
				catch (...) {
					closesocket(sockfd);
					SleepEx(60000, 0);
					connectionthread();
				}
			}
		}
		else {
			SleepEx(60000, 0);
			connectionthread();
		}
		//if()
	}
}