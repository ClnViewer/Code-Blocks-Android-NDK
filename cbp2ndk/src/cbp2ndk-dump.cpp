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

void dump_CbConf(CbConf *pcnf)
{
    try
    {
        for (auto &val : pcnf->v[elabels::LBL_HINC])
        {
            std::cout << " + LBL_HEAD: " << val << std::endl;
        }
        for (auto &val : pcnf->v[elabels::LBL_CSRC])
        {
            std::cout << " + LBL_CSRC: " << val << std::endl;
        }
        for (auto &val : pcnf->v[elabels::LBL_COMP])
        {
            std::cout << " + LBL_COMP: " << val << std::endl;
        }
        for (auto &val : pcnf->v[elabels::LBL_LDFLAG])
        {
            std::cout << " + LBL_LDFLAG: " << val << std::endl;
        }
        for (auto &val : pcnf->v[elabels::LBL_LDLIBS])
        {
            std::cout << " + LBL_LDLIBS: " << val << std::endl;
        }
        for (auto &val : pcnf->v[elabels::LBL_CEXT])
        {
            std::cout << " + LBL_CEXT: " << val << std::endl;
        }
        for (uint32_t i = 0U; i < 5U; i++)
        {
            std::cout << " + Input files [" << i << "] : "<< pcnf->fname[i] << std::endl;
        }
        std::cout << " + Project name [title] : "<< pcnf->prjname << std::endl;
        std::cout << " i File present [Makefile]       : "<< pcnf->ismkf << std::endl;
        std::cout << " i File present [Application.mk] : "<< pcnf->isapp << std::endl;
        std::cout << " i File present [Android.mk]     : "<< pcnf->isand << std::endl;
        std::cout << " c Is Dump    : "<< pcnf->isdump << std::endl;
        std::cout << " c Is Verbose : "<< pcnf->isverb << std::endl;
        std::cout << " c Is Quiet   : "<< pcnf->isquiet << std::endl;
	}
	catch (std::exception & _ex)
	{
		std::cout << " ! Dump : " << _ex.what() << std::endl;
	}

    return;
}
