#pragma once

#define DLL_EXPORT extern "C" __declspec(dllexport)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <assert.h>

#include <string>
#include <memory>
#include <functional>
#include <utility>
#include <cstring>
#include <vector>
#include <cstdint>
#include <mutex>
#include <algorithm>
#include <sstream>
#include <chrono>
#include <iterator>
#include <thread>

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

#include <MinHook.h>