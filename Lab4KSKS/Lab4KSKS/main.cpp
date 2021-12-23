#pragma comment(lib, "Ws2_32.lib")
#pragma warning( disable : 4996)
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <SFML/Graphics.hpp>
#include <winsock2.h>
#include <iostream>
#include <cmath>
#include "HeaderKSKS4.h"
#include "GraphicsLib.h"

using namespace std;

#define PORT 777 // порт сервера

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");
    KSKS obj1;

    char buff[1024];
    printf("UDP DEMO echo_Server \n");
    // Крок 1 - підключення бібліотеки
    if (WSAStartup(0x202, (WSADATA*)&buff[0])) {
        printf("WSAStartup error : %d \n", WSAGetLastError());
        return -1;
    }
    // Крок 2 - створення сокета
    SOCKET my_sock;
    my_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (my_sock == INVALID_SOCKET) {
        printf("Socket() error: %d \n", WSAGetLastError());
        WSACleanup();
        return -1;
    }
    // Крок 3 - зв'язування сокета з локальною адресою
    sockaddr_in local_addr;
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = INADDR_ANY;
    local_addr.sin_port = htons(PORT);
    if (bind(my_sock, (sockaddr*)&local_addr, sizeof(local_addr))) {
        printf("bind error : %d \n", WSAGetLastError());
        closesocket(my_sock);
        WSACleanup();
        return -1;
    }
    // Крок 4 - обробка пакетів, надісланих клієнтами
    while (1) {
        sockaddr_in client_addr;
        int client_addr_size = sizeof(client_addr);
        int bsize = recvfrom(my_sock, &buff[0], sizeof(buff) - 1, 0, (sockaddr*)&client_addr, &client_addr_size);
        if (bsize == SOCKET_ERROR)
            printf("recvfrom() error: %d \n", WSAGetLastError());
        // Визначаємо IP-адреса клієнта і інші атрибути
        HOSTENT* hst;
        hst = gethostbyaddr((char*)&client_addr.sin_addr, 4, AF_INET);
        printf("\n + %s[%s:%d] new command!\n", (hst) ? hst->h_name : "Unknown host", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        // Додавання завершального нуля
        buff[bsize] = 0;
        cout << "Получены данные: " << buff;

        bool result = obj1.FinalResult(buff);
        // Посилка дейтаграми клієнту
        if (result == true) {
            sendto(my_sock, "Command correct!", sizeof("Command correct!"), 0, (sockaddr*)&client_addr, sizeof(client_addr));
        }
        else {
            sendto(my_sock, "Command incorrect!", sizeof("Command incorrect!"), 0, (sockaddr*)&client_addr, sizeof(client_addr));
        }
    }

    return 0;
}