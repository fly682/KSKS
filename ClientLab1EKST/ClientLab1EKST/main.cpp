// Приклад простого UDP-клієнта
#pragma comment(lib, "Ws2_32.lib")
#pragma warning( disable : 4996)

#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <iostream>

#define PORT 777
#define SERVERADDR "127.0.0.1"

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "Russian");
	char buff[1000];
	char colBuf[8];
	char servBuf[1024];
	int comLen;
	printf("UDP DEMO Client \nВведите quit для выхода \n");

	// Крок 1 - ініціалізація бібліотеки Winsock

	if (WSAStartup(0x202, (WSADATA*)&buff[0]))
	{
		printf("WSAStartup error : %d \n", WSAGetLastError());
		return -1;
	}
	// Крок 2 - відкриття сокета

	SOCKET my_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (my_sock == INVALID_SOCKET)
	{
		printf("socket() error: %d \n", WSAGetLastError());
		WSACleanup();
		return -1;
	}
	// Крок 3 - обмін повідомлень з сервером

	HOSTENT* hst;
	sockaddr_in dest_addr;
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(PORT);
	// Визначення IP_адреса вузла
	if (inet_addr(SERVERADDR))
		dest_addr.sin_addr.s_addr = inet_addr(SERVERADDR);
	else
		if (hst = gethostbyname(SERVERADDR))
			dest_addr.sin_addr.s_addr = ((unsigned long**)hst->h_addr_list)[0][0];
		else
		{
			printf("Unknown host : %d \n", WSAGetLastError());
			closesocket(my_sock);
			WSACleanup();
			return -1;
		}
	while (1)
	{
		comLen = 0;
		// Читання повідомлення з клавіатури

		printf("\n\nВведите команду: "); fgets(&buff[0], sizeof(buff) - 1, stdin);
		if (!strcmp(&buff[0], "clear display:\n")) {
			printf("\nВведите цвет в формате #XXXXXX, где X - 0-9 или A-F, или ok, произойдёт очистка цветом по умолчанию: ");
			scanf("%s", colBuf);
			if (strcmp(&colBuf[0], "ok")) {
				comLen = strlen(buff);
				for (int i = 0; i < 8; i++) {
					buff[i + comLen - 1] = colBuf[i];
				}
			}
		}
		if (!strcmp(&buff[0], "quit\n")) break;

		// Передача повідомлень на сервер
		sendto(my_sock, &buff[0], strlen(&buff[0]), 0, (sockaddr*)&dest_addr, sizeof(dest_addr));

		// Прийом повідомлення з сервера

		sockaddr_in server_addr;
		int server_addr_size = sizeof(server_addr);
		int n = recvfrom(my_sock, &servBuf[0], sizeof(servBuf) - 1, 0, (sockaddr*)&server_addr, &server_addr_size);
		if (n == SOCKET_ERROR)
		{
			printf("recvfrom() error: %d \n", WSAGetLastError());
			closesocket(my_sock);
			WSACleanup();
			return -1;
		}
		servBuf[n] = 0;

		// Висновок прийнятого з сервера повідомлення на екран

		printf("S = > C: %s", &servBuf[0]);
	}

	// Крок останній - вихід

	closesocket(my_sock);
	WSACleanup();
	return 0;
}