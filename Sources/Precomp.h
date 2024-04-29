#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <stdexcept>

#ifdef WIN32
#define WIN32_MEAN_AND_LEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define NOMINMAX
#include <WinSock2.h>
#include <Windows.h>
#undef PlaySound
#undef DrawText
#endif

#if (defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_X64))) || ((defined(__i386__) || defined(__x86_64__) || defined(__e2k__)) && defined(__SSE2__))
#define USE_SSE
#endif
