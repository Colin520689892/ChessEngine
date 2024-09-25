#pragma once

// 包括 WinSDKVer.h 并定义 _WIN32_WINNT 宏来设置支持的最低 Windows 版本
#include <WinSDKVer.h>

// 定义支持的最低 Windows 版本
// 例如，定义为 Windows 7 (0x0601) 或其他版本
#define _WIN32_WINNT 0x0601 // Windows 7

// 包括 SDKDDKVer.h 将定义可用的最高版本的 Windows 平台
#include <SDKDDKVer.h>
