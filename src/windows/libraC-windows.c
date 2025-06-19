//
// Created by Fraz Mahmud on 6/1/2025.
//
#ifdef __WIN32
#include <direct.h>


#include <windows/libraC-windows.h>

#include <libraC.h>

void LC_Win32_GetCurrentWorkingDirectory(char* buffer, const size_t size) {
     _getcwd(buffer, (int32)size);
}

#endif
