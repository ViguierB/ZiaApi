/*
** EPITECH PROJECT, 2018
** api
** File description:
** Platform.hpp
*/

#pragma once

#if !defined(_WIN32) && !defined(__WIN32__) && !defined(WIN32)
# define ZANY_ISUNIX

namespace zany {
static constexpr bool isUnix = true;
static constexpr bool isWindows = false;
using Socket = int;
}

#else
# define ZANY_ISWINDOWS
# include <winsock2.h>

namespace zany {
static constexpr bool isUnix = false;
static constexpr bool isWindows = true;
using Socket = SOCKET;
}

#endif

#if defined _WIN32 || defined __CYGWIN__
  #ifdef __GNUC__
    #define ZANY_DLL __attribute__ ((dllexport))
  #else
    #define ZANY_DLL __declspec(dllexport)
  #endif
  #pragma warning(disable: 4251)
#else
  #if __GNUC__ >= 4
    #define ZANY_DLL __attribute__ ((visibility ("default")))
  #else
    #define ZANY_DLL
  #endif
#endif