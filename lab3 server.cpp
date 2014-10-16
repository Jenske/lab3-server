// lab3 server.cpp: определяет точку входа для консольного приложения.
//
#include <stdio.h>
#include <iostream>
#include <WinSock2.h>

void main()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);
	if(err)
		return;

	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in local_addr;
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(1280);
	local_addr.sin_addr.s_addr = 0;
	bind(s, (sockaddr*) &local_addr, sizeof(local_addr));