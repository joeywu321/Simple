#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
#include <sys/types.h> 
#ifdef WIN32
#include <winsock.h>
typedef int				socklen_t;
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <err.h>
#endif
#ifdef WIN32
#pragma comment(lib, "wsock32")
#endif
char response[] = "HTTP/1.1 200 OK\r\n"
	"Content-Type: text/html; charset=UTF-8\r\n\r\n"
	"Content-Length: 320"
	"<!DOCTYPE html><html><head><title>Bye-bye baby bye-bye</title>"
	"<style>body { background-color: #111 }"
	"h1 { font-size:4cm; text-align: center; color: black;"
	" text-shadow: 0 0 2mm red}</style></head>"
	"<body><h1>Goodbye, world!</h1></body></html>\r\n";

//Hello world/Web server
//http://rosettacode.org/wiki/Hello_world/Web_server
int main()
{
	int one = 1, client_fd;
	struct sockaddr_in svr_addr, cli_addr;
	socklen_t sin_len = sizeof(cli_addr);

	WSADATA wsaData;
	//#define MAKEWORD(a,b) ((WORD) (((BYTE) (a)) | ((WORD) ((BYTE) (b))) << 8)) 
	WORD version = MAKEWORD(2, 0);
	int ret = WSAStartup(version, &wsaData);//win sock start up

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
		return 1;

	//setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));
	int opt =  1;
	if ( setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0 )
		return 0;

	int port = 80;
	svr_addr.sin_family = AF_INET;
	svr_addr.sin_addr.s_addr = INADDR_ANY;
	svr_addr.sin_port = htons(port);

	if (bind(sock, (struct sockaddr *) &svr_addr, sizeof(svr_addr)) == -1) {
		//close(sock);		
		return 0;
	}

	listen(sock, 5);
	while (1) 
	{
		client_fd = accept(sock, (struct sockaddr *) &cli_addr, &sin_len);
		printf("got connection\n");

		if (client_fd == -1) 
		{
			perror("Can't accept");
			continue;
		}

		int ret = send(client_fd,response,sizeof(response),0);
		do
		{
			int bufsize=1024;
			char Rec_Buf[1024];
			int rec=recv(client_fd,Rec_Buf,bufsize,0);
			if(rec > 0)
			{
				closesocket(client_fd);
			}
		} while ( ret < 0 );
	}
}
