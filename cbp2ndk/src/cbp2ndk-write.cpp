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

#include "cbp2ndk.h"

using namespace std;

bool if_section(int lb, CbConf *pcnf)
{
    return (pcnf->v[lb].size());
}

bool write_label(FILE *fp, const char **labels, int lb)
{
    size_t sz = strlen(labels[lb]);
    if (fwrite(labels[lb], 1, sz, fp) != sz)
        return false;

    return true;
}

bool write_section(FILE *fp, CbConf *pcnf, const char **labels, int lb)
{
    for (auto &val : pcnf->v[lb])
    {
#       if defined(_DEBUG)
        std::cout << "\t+ : " << val << std::endl;
#       endif
        if (fwrite(val.c_str(), 1, val.length(), fp) != val.length())
            return false;
    }

    size_t sz = strlen(labels[elabels::LBL_END]);
    if (fwrite(labels[elabels::LBL_END], 1, sz, fp) != sz)
        return false;

    return true;
}
