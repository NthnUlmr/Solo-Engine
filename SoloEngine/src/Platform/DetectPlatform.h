#pragma once


// Macros for detecting current build platform

#ifdef _WIN32

#ifdef _WIN64
#define SOLO_PLATFORM_WINDOWS
#else
#error "Windows 32 bit is not supported!"
#endif

#elif defined(__APPLE__) || defined(__MAC__)

#define SOLO_PLATFORM_APPLE_DESKTOP
#error "Unknown platforms are not supported!"

#elif defined(__ANDROID__)

#define SOLO_PLATFORM_ANDROID
#error "Unknown platforms are not supported!"

#elif definend(__linux__)

#define SOLO_PLATFORM_LINUX
#error "Unknown platforms are not supported!"

#else

#define SOLO_PLATFORM_IS_ERROR
#error "Unknown platforms are not supported!"



#endif
