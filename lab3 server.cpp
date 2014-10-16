#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include <stdio.h>
#include <iostream>
#include <winsock2.h>

struct part
{
	char name[20];
	int number;
	int manufactorynumber;
};

struct list
{
	struct part info;
	struct list* next;
};

using namespace std;

int numcl = 0;

void formList(struct list**);
void clearList(struct list*);
void addToList(struct list**, struct part);

DWORD WINAPI ThreadFunc(LPVOID client_socket)
{
	SOCKET s2 = ((SOCKET *) client_socket)[0];
	char buf[100];
	char answer[100] = "There are no such parts.";
	struct list* head = NULL;
	while(recv(s2, buf, sizeof(buf), 0))
	{
		buf[strlen(buf)-1] = '\0';
		formList(&head);
		struct list* current = head;
		while(current)
		{
			if(!strcmp(current->info.name, buf))
			{
				_itoa(current->info.number, answer, 10);
			}
			current = current->next;
		}
		send(s2, answer, 100, 0);
	}
	clearList(head);
	closesocket(s2);
	return 0;
}

DWORD WINAPI ThreadFunc1(LPVOID client_socket)
{
	SOCKET s2 = ((SOCKET *) client_socket)[0];
	char buf[100];
	char answer[100] = "";
	"There are no such parts.";
	struct list* head = NULL;
	while(recv(s2, buf, sizeof(buf), 0))
	{
		buf[strlen(buf)-1] = '\0';
		formList(&head);
		struct list* current = head;
		int manufactory = atoi(buf);
		while(current)
		{
			if(manufactory == current->info.manufactorynumber)
			{
				strcpy(answer, current->info.name);
			}
			current = current->next;
		}
		send(s2, answer, 100, 0);
	}
	clearList(head);
	closesocket(s2);
	return 0;
}

void print()
{
	if (numcl) 
		printf("%d client connected\n", numcl);
	else 
		printf("No clients connected\n");
}


void main()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD( 2, 2 );
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 )
	{
		return;
	}

    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in local_addr;
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(1280);
    local_addr.sin_addr.s_addr = 0;
    bind(s, (sockaddr *) &local_addr, sizeof(local_addr));
    int c = listen(s, 5);
	cout<<"Server receive ready"<<endl;
	cout<<endl;
    SOCKET client_socket;  
    sockaddr_in client_addr; 
    int client_addr_size = sizeof(client_addr);
    while((client_socket=accept(s, (sockaddr *)&client_addr, &client_addr_size)))
    {
      numcl++;   
      print();
	  char buf[100];
	  recv(client_socket, buf, sizeof(buf), 0);
	  if(buf[0] == '1')
	  {
		  send(client_socket, "1", 100, 0);
		  DWORD thID;
		  CreateThread(NULL,NULL,ThreadFunc,&client_socket,NULL,&thID);
	  }
	  else if(buf[0] == '2')
	  {
		  send(client_socket, "2", 100, 0);
		  DWORD thID;
		  CreateThread(NULL,NULL,ThreadFunc1,&client_socket,NULL,&thID);
	  }
	  else send(client_socket, "Wrong choice", 100, 0);
    }
}


void addToList(struct list** head, struct part info)
{
	if(!(*head))
	{
		*head = new struct list;
		(*head)->info.manufactorynumber = info.manufactorynumber;
		(*head)->info.number = info.number;
		strcpy((*head)->info.name, info.name);
		(*head)->next = NULL;
	}
	else
	{
		struct list* current = *head;
		while(current->next)
		{
			current = current->next;
		}
		current->next = new struct list;
		current = current->next;
		current->info.manufactorynumber = info.manufactorynumber;
		current->info.number = info.number;
		strcpy(current->info.name, info.name);
		current->next = NULL;
	}
}

void clearList(struct list* head)
{
	if(head)
	{
		clearList(head->next);
	}
	else
		delete head;
}

void formList(struct list** head)
{
	struct part part[7] = {{"accumulator", 5, 560}, {"headlights", 6, 630}, {"mirrors", 7, 730}, {"condensers", 8, 480},{"speakers", 9, 630}, {"tires", 10, 580}, {"engines", 11, 760}};
	for(int i = 0; i < 7; i++)
	{
		addToList(head, part[i]);
	}
}