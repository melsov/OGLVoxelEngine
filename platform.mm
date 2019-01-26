#include "platform.h"



#ifdef _WIN32

#include <windows.h>

std::string ResourcePath(std::string fileName)
{
	char executablePath[1024] = { '\0' };
	DWORD charsCopied = GetModuleFileName(NULL, executablePath, 1024);
	if (charsCopied > 0 && charsCopied < 1024)
		return std::string(executablePath) + "\\..\\" + fileName;
	else
		throw std::runtime_error("GetModuleFileName failed a bit");
}

#elif __APPLE__

#import <Foundation/Foundation.h>

// returns the full path to the file `fileName` in the resources directory of the app bundle
std::string ResourcePath(std::string fileName) {
    NSString* fname = [NSString stringWithCString:fileName.c_str() encoding:NSUTF8StringEncoding];
    NSString* path = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:fname];
    return std::string([path cStringUsingEncoding:NSUTF8StringEncoding]);
}


#elif __unix__ // all unices not caught above

// Unix
#elif defined(_POSIX_VERSION)

// POSIX
#else
#   error "Unknown compiler"
#endif
