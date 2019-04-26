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
#   if defined(_DEBUG)
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
        for (uint32_t i = 0U; i < 3U; i++)
        {
            std::cout << " + Input files [" << i << "] : "<< pcnf->fname[i] << std::endl;
        }
        std::cout << " + Project name [title] : "<< pcnf->prjname << std::endl;
	}
	catch (std::exception & e)
	{
		std::cout << " ! Dump : " << e.what() << std::endl;
	}

    return;
#   else
    (void)pcnf;
#   endif
}
