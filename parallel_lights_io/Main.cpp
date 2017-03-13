#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "Main.h"
#include <fstream>
#include <string>

#include <Windows.h>
#include <winsock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib,"WS2_32")


char Lights[4] = { 0,0,0,0 };
bool socketAlive = false;
bool dataChanged = false;
SOCKET SendSocket = INVALID_SOCKET;
sockaddr_in RecvAddr;

std::string ExePath() {
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	return std::string(buffer).substr(0, pos);
}

BOOLEAN WINAPI DllMain(IN HINSTANCE hDllHandle,
	IN DWORD     nReason,
	IN LPVOID    Reserved)
{
	if (nReason == DLL_PROCESS_ATTACH) {
		int iResult = 0;
		WSADATA wsaData;

		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != NO_ERROR) {
			MessageBox(NULL, "WSAStartup failed.", "Fuck.", MB_OK);
			return TRUE;
		}


		int port = 27015;

		std::ifstream conf = std::ifstream(ExePath() + std::string("\\socket_lights.txt"));
		std::string adr = "127.0.0.1";
		std::string strPort;

		if (conf.is_open()) {
			try {
				std::getline(conf, adr);
				std::getline(conf, strPort);
				port = std::stoi(strPort);
			}
			catch (...) {
				MessageBox(NULL, "socket_lights.txt bad format\nUsing defaults: \n127.0.0.1\n27015", "Bad config", MB_OK);
				adr = "127.0.0.1";
				port = 27015;
			}
		}
		else {
			MessageBox(NULL, "socket_lights.txt missing\nUsing defaults: \n127.0.0.1\n27015", "Missing config", MB_OK);
		}

		SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		
		RecvAddr.sin_family = AF_INET;
		RecvAddr.sin_port = htons(port);
		RecvAddr.sin_addr.s_addr = inet_addr(adr.c_str());

		if (SendSocket == INVALID_SOCKET) {
			MessageBox(NULL, "Socket failed to create.", "RIP", MB_OK);
			return TRUE;
		}
		socketAlive = true;
	}
	return TRUE;
}

void dispatchLights() {
	if (!socketAlive) return;
	if (!dataChanged) return;
	int iResult = 0;
	sendto(SendSocket,
		(Lights), 3, 0, (SOCKADDR *)& RecvAddr, sizeof(RecvAddr));
	if (iResult == SOCKET_ERROR) {
		MessageBox(NULL, "Socket error! :(", "Aww", MB_OK);
		socketAlive = false;
	}
	dataChanged = false;
}

short int WINAPI IsDriverInstalled(){
	#pragma EXPORT
	return (short int)1;
}

char WINAPI PortIn(short int port){
	#pragma EXPORT
	return '\0';
}

void WINAPI PortOut(short int port,   char data)
{
	#pragma EXPORT

	switch (port) {
	case 0x378:
		dataChanged = (Lights[0] != data) || dataChanged;
		Lights[0] = data;
		break;
	case 0x278:
		dataChanged = (Lights[1] != data) || dataChanged;
		Lights[1] = data;
		break;
	case 0x3bc:
		dataChanged = (Lights[2] != data) || dataChanged;
		Lights[2] = data;
	default:
		dispatchLights();
		break;
	}
}
