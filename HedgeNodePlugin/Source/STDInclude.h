/*
	This project is licensed under the GNU GPL 2.0 license. Please respect that.

	Initial author: (https://github.com/)Convery
	Started: 2014-10-01
	Notes
*/

#pragma once

#pragma region Warnings

// We know what we are doing.
#pragma warning(disable:4018)
#pragma warning(disable:4244)
#pragma warning(disable:4273)
#pragma warning(disable:4309)
#pragma warning(disable:4996)

#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS
#define STRSAFE_NO_DEPRECATE
#define STRSAFE_NO_CCH_FUNCTIONS

#pragma endregion

#pragma region Defines

// Platform information.
#define PLATFORM_WINDOWS 1
#define PLATFORM_UNIX 2
#define PLATFORM_MAC 3

#if defined(_WIN32) 
#define PLATFORM PLATFORM_WINDOWS 
#elif defined(__APPLE__) 
#define PLATFORM PLATFORM_MAC
#else 
#define PLATFORM PLATFORM_UNIX 
#endif

#define WIN32_LEAN_AND_MEAN
#define DEBUG // We are currently debugging this monstrosity.
#define PROJECTNAME "BP"

#pragma endregion

#pragma region Debug

// Debug info.
#define NET_DEBUG					// Winsock logging.
#define FS_DEBUG					// Filesystem logging.
#define PERF_DEBUG					// Performance logging.
#define PLG_DEBUG					// Log to a steam_api export.

// Debug functions.
#if !defined(NET_DEBUG) && !defined(FS_DEBUG) && !defined(PERF_DEBUG)
#define DBGPrint(fmt, ...)
#else
#ifdef PLG_DEBUG
#ifdef _WIN64
#define DBGPrint(fmt, ...) ((int32_t (__cdecl *)(const char *))GetProcAddress(GetModuleHandleA("steam_api64.dll"), "Debug_PrintDebugString"))(hString::va(fmt, ##__VA_ARGS__))
#else
#define DBGPrint(fmt, ...) ((int32_t (__cdecl *)(const char *))GetProcAddress(GetModuleHandleA("steam_api.dll"), "Debug_PrintDebugString"))(hString::va(fmt, ##__VA_ARGS__))
#endif
#else
#define DBGPrint(fmt, ...) printf(fmt"\n",  ##__VA_ARGS__)
#endif
#endif

#define HardDebugBreak() MessageBoxA(0, __FUNCTION__, 0, 0);
#define PrintCurrentFunction()	pDBGPrint("[DBG]: %s", __FUNCTION__)

#ifdef NET_DEBUG
#define nDBGPrint(fmt, ...) DBGPrint("[NET]: "fmt,  ##__VA_ARGS__)
#else
#define nDBGPrint(fmt, ...)
#endif
#ifdef FS_DEBUG
#define fDBGPrint(fmt, ...) DBGPrint("[FS ]: "fmt,  ##__VA_ARGS__)
#else
#define fDBGPrint(fmt, ...)
#endif
#ifdef PERF_DEBUG
#define pDBGPrint(fmt, ...) DBGPrint("[PRF]: "fmt,  ##__VA_ARGS__)
#else
#define pDBGPrint(fmt, ...)
#endif
#define HedgePrint(fmt, ...) DBGPrint("[HedgeNode]: "fmt,  ##__VA_ARGS__)

#pragma endregion

#pragma region Includes

// C++ Headers.
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <fstream>

// Project headers.
#include "Exports.h"
#include "GlobalVariables.h"

// Platform dependant includes.
#if PLATFORM == PLATFORM_WINDOWS
#pragma comment( lib, "wsock32.lib" )
#include <winsock2.h>

#elif PLATFORM == PLATFORM_UNIX || PLATFORM == PLATFORM_MAC
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <fcntl.h>
#endif

// Utilities
#include "Util\ByteBuffer.h"
#include "Util\hString.h"

// SQLite3
#include "ExternalIncludes\Sqlite3\sqlite3.h" 

// Networking.
#include "Networking\DataTypes.h"
#include "Networking\SocketManager.h"
#include "Networking\Model\ISerializable.h"
#include "Networking\Model\NetworkDataTypes.h"

// Database.
#include "Database\DataTypes.h"
#include "Database\DatabaseManager.h" 

//Models
#include "Models\PersistentEntity.h"

//Our beans
#include "Beans\HedgeClient.h"

// Node system
#include "Nodes\sNode.h"

#pragma endregion