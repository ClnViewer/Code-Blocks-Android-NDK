/*
    MIT License

    Copyright (c) 2018 PS
    GitHub: https://github.com/ClnViewer/Code-Blocks-Android-NDK

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sub license, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
 */

#ifndef __CBP2NDK_LOCAL_H__
#define __CBP2NDK_LOCAL_H__

#if !defined(_WIN32_WINNT)
#   define _WIN32_WINNT 0x0501
#endif

#if (defined(_WIN32) || defined(__WIN32__) || \
     defined(_WIN64) || defined(__WIN64__) || \
     defined(__WINNT) || defined(__WINNT__) || defined(WINNT) || \
     defined(_Windows) || defined(_MSC_VER) || \
     defined(__CYGWIN__) || defined(__MINGW32__) || defined(__MINGW64__))
#   define __SEPARATOR_PATH "\\"
#else
#   define __SEPARATOR_PATH "/"
#endif

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <iostream>
#include "extern/tixml2ex.h"
#include "../version.h"
#include <stdio.h>

#define __CSZ(a)  static_cast<size_t>(sizeof(a)-1)
#define __NELE(a) (sizeof(a) / sizeof(a[0]))
#define __AUTO(x) __attribute__((cleanup(x)))

static inline void __attribute__((always_inline)) __autofree(void *v)
{
    if (v)
    {
#       if defined(__cplusplus)
        void *x = static_cast<void*>(*static_cast<void**>(v));
#       else
        void *x = (void*)(*(void**)v);
#       endif
        if (x)
        {
            free(x);
        }
    }
}

static inline void __attribute__((always_inline)) __autofile(void *v)
{
    if (v)
    {
#       if defined(__cplusplus)
        void *x = static_cast<void*>(*static_cast<void**>(v));
#       else
        void *x = (void*)(*(void**)v);
#       endif
        if (x)
        {
#           if defined(__cplusplus)
            fclose(static_cast<FILE*>(x));
#           else
            fclose((FILE*)x);
#           endif
            x = NULL;
        }
    }
}

enum elabels
{
    LBL_CEXT,
    LBL_CSRC,
    LBL_COMP,
    LBL_LDFLAG,
    LBL_LDLIBS,
    LBL_HINC,
    LBL_NAME,
    LBL_END
};

#define _END_FOR_ARRAY elabels::LBL_NAME

typedef struct l_CbConf
{
    std::vector<std::string> v[elabels::LBL_END];
    std::map<std::string, int32_t> m[2];
    std::string fname[3];
    std::string prjname;
} CbConf;

void dump_CbConf(CbConf*);
bool if_section(int, CbConf*);
bool write_label(FILE*, const char**, int);
bool write_section(FILE*, CbConf*, const char**, int);

void parse_section(
        CbConf*,
        const tinyxml2::XMLDocument&,
        std::string const&,
        std::string const&,
        std::function<void(CbConf*, std::string&)>);

void parse_section(
        CbConf*,
        const tinyxml2::XMLElement*,
        std::string const&,
        std::string const&,
        std::function<void(CbConf*, std::string&)>);

#endif
