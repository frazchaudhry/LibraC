//
// Created by Fraz Mahmud on 6/1/2025.
//
#ifdef __linux__
#include <unistd.h>


#include <linux/libraC-linux.h>

#include <libraC.h>

void LC_Linux_GetCurrentWorkingDirectory(char* buffer, const size_t size) {
    getcwd(buffer, (int32)size);
}

#endif
