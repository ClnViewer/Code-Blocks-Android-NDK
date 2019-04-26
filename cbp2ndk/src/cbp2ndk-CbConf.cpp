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

using namespace std;

CbConf::CbConf(std::string const & _tag, std::string const & _path)
    : tag(_tag), isapp(false), isand(false), ismkf(false)
    {
        path(_path.c_str());
    }

CbConf::~CbConf() {}

void CbConf::path(std::string const & _path)
{
    fname[0] = std::string(_path);

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
