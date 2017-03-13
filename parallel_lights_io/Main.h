#pragma once
#define EXPORT comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
#include <Windows.h>

BOOLEAN WINAPI DllMain(IN HINSTANCE, IN DWORD, IN LPVOID);

short int WINAPI IsDriverInstalled();
char WINAPI PortIn(short int);
void WINAPI PortOut(short int,   char);