#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS


#include "windows.h"

#include<iostream>

#include <winsock2.h>

#include <Ws2tcpip.h>

#include <vector>

#include <algorithm>

#include <thread>

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT 27015
int counter;
std::vector < int > mediana;
double CalcM(std::vector < int > v) {

    size_t size = v.size();
    if (size == 0)
        return 0; 
    else {
        sort(v.begin(), v.end());
        if (size % 2 == 0)
            return ((double)v[size / 2 - 1] + v[size / 2]) / 2;
        else
            return (double)v[size / 2];
    }

}
int VectorToInt(std::vector < int > v) {
    int result = 0;
    for (auto d : v) {
        result = result * 10 + d;
    }
    return result;
}
int go(int i) {
    std::vector < int > v1;
    int iResult;
    WSADATA wsaData;

    SOCKET ConnectSocket = INVALID_SOCKET;
    struct sockaddr_in clientService;
    bool flag = 0;
    bool flag2 = 0;
    do {
        if (flag2 == 1) {
            Sleep(3000);
            flag2 = 0;
        }
        flag = 0;
        v1.clear();
        int recvbuflen = DEFAULT_BUFLEN;
        char sendbuf[4];
        char recvbuf[DEFAULT_BUFLEN] = "";

        iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != NO_ERROR) {
       
            //return 1;
            flag2 = 1;
        }

        _itoa(i, sendbuf, 10);

        //----------------------
        // Create a SOCKET for connecting to server
        ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        while (ConnectSocket == INVALID_SOCKET) {
            flag2 = 1;
            ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            wprintf(L"socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();

            
        }

        //----------------------
        // The sockaddr_in structure specifies the address family,
        // IP address, and port of the server to be connected to.
        clientService.sin_family = AF_INET;
        clientService.sin_addr.s_addr = inet_addr("88.212.241.115");
        clientService.sin_port = htons(2013);

     
        iResult = connect(ConnectSocket, (SOCKADDR*)&clientService, sizeof(clientService));
        while (iResult == SOCKET_ERROR) {
            flag2 = 1;
            iResult = connect(ConnectSocket, (SOCKADDR*)&clientService, sizeof(clientService));
            wprintf(L"connect failed with error: %d\n", WSAGetLastError());
            closesocket(ConnectSocket);
            WSACleanup();

          
        }

        iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
        if (iResult == SOCKET_ERROR) {
            wprintf(L"send failed with error: %d\n", WSAGetLastError());
            flag2 = 1;
            closesocket(ConnectSocket);
            WSACleanup();
       
        }
        // printf("Bytes Sent: %d\n", iResult);
        // shutdown the connection since no more data will be sent
        iResult = shutdown(ConnectSocket, SD_SEND);
        if (iResult == SOCKET_ERROR) {
            flag2 = 1;
            wprintf(L"shutdown failed with error: %d\n", WSAGetLastError());
            closesocket(ConnectSocket);
            WSACleanup();
         
        }
        // Receive until the peer closes the connection

        int counter = 0;
        v1.clear();
        do {
            iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
          
            for (int i = 0; i < strlen(recvbuf); ++i) {
                if (recvbuf[i] == '\n')
                    flag = 1;

                if (isdigit(recvbuf[i]))
                    v1.push_back(recvbuf[i] - '0');

            }

        } while (iResult > 0);

        iResult = closesocket(ConnectSocket);
        if (iResult == SOCKET_ERROR) {
            std::cout << "shit" << ' ';
            flag2 = 1;
            wprintf(L"close failed with error: %d\n", WSAGetLastError());
            WSACleanup();
       
        }

    } while (flag == 0 || VectorToInt(v1) < 0 || VectorToInt(v1) > pow(10, 7));

 

    std::cout << VectorToInt(v1) << " " << i << " " << "\n";

    mediana.push_back(VectorToInt(v1));

    WSACleanup();
    return 1;

    WSACleanup();
}

int main() {
    char* pl = setlocale(LC_ALL, "Russian_Russia.20866"); // koi8-r
    std::cout.precision(12);

    std::vector < std::thread > th;
    int start, end;

    start = 1;
    end = 200;
    for (int m = 1; m <= 10; ++m) {

        for (int i = start; i <= end; ++i) {
            Sleep(500);
            th.push_back(std::thread(go, i));

        }

    
        for (auto& t : th) {
            t.join();

        }
        th.clear();
        start += 200;
        end += 200;
    }

    for (int i = 2001; i <= 2018; ++i) {
        Sleep(1000);
        th.push_back(std::thread(go, i));
    }

    for (auto& t : th) {
        t.join();

    }
    std::cout << "mediana " << CalcM(mediana);

    return 0;
}