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
}

#else
# define ZANY_ISWINDOWS

namespace zany {
static constexpr bool isUnix = false;
static constexpr bool isWindows = true;
}

#endif