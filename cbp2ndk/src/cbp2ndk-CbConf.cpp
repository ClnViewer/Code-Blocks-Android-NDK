/*
    MIT License

    Copyright (c) 2018 PS
    GitHub: https://github.com/ClnViewer/Code-Blocks-Android-NDK

    See https://developer.android.com/ndk/guides/android_mk for more information

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

#include "cbp2ndk.h"
#include "extern/argh.h"

#define __CONF_TAG       "-t", "--tag"
#define __CONF_CBP       "-c", "--cbp"
#define __CONF_AUTO      "-a", "--auto"
#define __CONF_DUMP      "-d", "--dump"
#define __CONF_QUIET     "-q", "--quiet"
#define __CONF_VERBOSE   "-v", "--verbose"
#define __CONF_NODEFAULT "-n", "--nodefault"
#define __CONF_CBP_EXT  ".cbp"

#if !defined(__OS_IS_WIN)
#define _access access
#endif

using namespace std;

CbConf::CbConf(const char **argv, int argc)
    : isarg(false), isverb(false), isquiet(false), isdump(false), isnodef(false),
      isapp(false), isand(false), ismkf(false)
    {
        cmdl(argv, argc);
        if (isarg)
            path();
    }

CbConf::~CbConf() {}

bool CbConf::findcbp()
{
#   if defined(__OS_IS_WIN)
    bool ret = false;
    WIN32_FIND_DATA fd{};
    HANDLE hf;

    if ((hf = ::FindFirstFileA(
                static_cast<LPCSTR>("*" __CONF_CBP_EXT),
                &fd)
         ) == INVALID_HANDLE_VALUE
        )
        return ret;

    do
    {
        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            if (string_end(fd.cFileName, __CONF_CBP_EXT))
            {
                fname[0].assign(fd.cFileName);
                ret = true;
                break;
            }
        }
    }
    while (::FindNextFile(hf, &fd));

    ::FindClose(hf);
    return true;
#   else
    throw tinyxml2::XmlException("Non-Windows not implemented auto find file..");
#   endif
}

void CbConf::cmdl(const char **argv, int argc)
{
    argh::parser lcmd({
            __CONF_TAG,
            __CONF_CBP,
            __CONF_DUMP,
            __CONF_AUTO,
            __CONF_QUIET,
            __CONF_VERBOSE,
            __CONF_NODEFAULT
            });

    lcmd.parse(argc, argv);

    bool isauto = (lcmd[{ __CONF_AUTO }]);
    bool istag  = !(!(lcmd({ __CONF_TAG }) >> tag));
    bool iscnf  = !(!(lcmd({ __CONF_CBP }) >> fname[0]));
    isdump      = (lcmd[{ __CONF_DUMP }]);
    isquiet     = (lcmd[{ __CONF_QUIET }]);
    isnodef     = (lcmd[{ __CONF_NODEFAULT }]);
    isverb      = ((isquiet) ? false : (lcmd[{ __CONF_VERBOSE }]));

    if (isauto)
        if ((iscnf = findcbp()))
            if (tag.empty())
            {
                istag = true;
                tag = "Release";
            }

    if ((istag) && (iscnf))
        isarg = true;
    else if (((!istag) || (!iscnf)) && (lcmd.pos_args().size() >= 3))
    {
        if (!istag)
            tag = lcmd(1).str();
        if (!iscnf)
            fname[0] = lcmd(2).str();
        if (tag.empty())
            return;
        if (fname[0].empty())
            if (!findcbp())
                return;
        isarg = true;
    }
}

void CbConf::path()
{
    if (::_access(fname[0].c_str(), F_OK) < 0)
        throw tinyxml2::XmlException("open cbp file: " + fname[0]);

    size_t pos = fname[0].find_last_of("/\\");

    if (pos == std::string::npos)
    {
        fname[1] = "Android.mk";
        fname[2] = "Android.mk.tmp";
        fname[3] = "Application.mk";
        fname[4] = "Makefile";
    }
    else
    {
        std::string fpath =  fname[0].substr(0,pos);
        fname[1] = fpath + __SEPARATOR_PATH "Android.mk";
        fname[2] = fpath + __SEPARATOR_PATH "Android.mk.tmp";
        fname[3] = fpath + __SEPARATOR_PATH "Application.mk";
        fname[4] = fpath + __SEPARATOR_PATH "Makefile";
    }

    if (::_access(fname[4].c_str(), F_OK) < 0)
        ismkf = false;
    else
        ismkf = true;

    if (::_access(fname[3].c_str(), F_OK) < 0)
        isapp = false;
    else
        isapp = true;

    if (::_access(fname[1].c_str(), F_OK) < 0)
        isand = false;
    else
        isand = true;
}
