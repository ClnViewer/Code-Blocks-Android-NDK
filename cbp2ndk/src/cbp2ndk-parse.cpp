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

typedef struct _str_const_cppflag_s
{
    const char *str;
    const size_t sz;

} cppflag_s;

static cppflag_s cppflags[] =
{
#define _TOTXT(A) { A, __CSZ(A) },
#include "cbp2ndk-cppflags.h"
#undef  _TOTXT
};

void parse_section(
        CbConf *pcnf,
        const tinyxml2::XMLDocument & root,
        std::string const & sec,
        std::string const & attr,
        std::function<void(CbConf*, std::string&)> f
        )
{
    for (auto unit : tinyxml2::selection(
                        root,
                        sec
                        )
         )
    {
        std::string opt = attribute_value(unit, attr);
        if (opt.empty())
            continue;

        opt.insert(0, " ");
        f(pcnf, opt);
    }
}

void parse_section(
        CbConf *pcnf,
        const tinyxml2::XMLElement *root,
        std::string const & sec,
        std::string const & attr,
        std::function<void(CbConf*, std::string&)> f
        )
{
    for (auto unit : tinyxml2::selection(
                        root,
                        sec
                        )
         )
    {
        std::string opt = attribute_value(unit, attr);
        if (opt.empty())
            continue;

        opt.insert(0, " ");
        f(pcnf, opt);
    }
}

void parse_cflag(CbConf *pcnf, std::string & opt)
{
    elabels lb = elabels::LBL_CFLAG;

    do
    {
        if (string_begin(opt, " -I"))
        {
            opt.erase(1,2);
            lb = elabels::LBL_HINC;
            break;
        }
        if (opt.find('+') != std::string::npos)
        {
            lb = elabels::LBL_CPPFLAG;
            break;
        }
        for (uint32_t i = 0U; i < __NELE(cppflags); i++)
        {
            if ((opt.length() - 1) >= cppflags[i].sz)
                if (!opt.compare(1U, cppflags[i].sz, cppflags[i].str))
                {
                    lb = elabels::LBL_CPPFLAG;
                    break;
                }
        }
    }
    while (0);

    pcnf->v[lb].push_back(opt);
}

void parse_ldflag(CbConf *pcnf, std::string & opt)
{
    elabels lb;

    if (string_begin(opt, " -l"))
        lb = elabels::LBL_LDLIBS;
    else
        lb = elabels::LBL_LDFLAG;

    pcnf->v[lb].push_back(opt);
}

void parse_srclist(CbConf *pcnf, std::string & opt)
{
    if (
        (string_end(opt, ".h"))   ||
        (string_end(opt, ".hxx")) ||
        (string_end(opt, ".hpp"))
       )
    {
        size_t sp;
        if ((sp = opt.find_last_of("/\\")) != std::wstring::npos)
        {
            std::string incpath = opt.substr(0, sp);
            if (!incpath.empty())
            {
                if (pcnf->m[1].find(incpath) != pcnf->m[1].end())
                    pcnf->m[1].insert(std::make_pair(incpath, 1));
                else
                    pcnf->m[1][incpath] += 1;
            }
        }
    }
    else if (
        (string_end(opt, ".c"))   ||
        (string_end(opt, ".cc"))  ||
        (string_end(opt, ".cxx")) ||
        (string_end(opt, ".cpp"))
       )
    {
        if (!string_end(opt, ".c"))
        {
            size_t sp;
            if ((sp = opt.find_last_of(".")) != std::wstring::npos)
            {
                std::string ext = opt.substr(sp, opt.length() - sp);
                if (!ext.empty())
                {
                    if (pcnf->m[0].find(ext) != pcnf->m[0].end())
                        pcnf->m[0].insert(std::make_pair(ext, 1));
                    else
                        pcnf->m[0][ext] += 1;
                }
            }
        }
        pcnf->v[elabels::LBL_CSRC].push_back(opt);
    }
    else
        if (pcnf->isverb)
            std::cout << " ! Skip: not support file extension: " << opt.c_str() << std::endl;
}

void parse_prjname(CbConf *pcnf, std::string & opt)
{
    static const char rchars[] = "!@#$%^&*()+- ?><'\"";

    for (uint32_t i = 0U; i < __CSZ(rchars); i++)
    {
        while (opt.find(rchars[i]) != string::npos)
            opt.replace(opt.find(rchars[i]), 1, "_");
    }
    pcnf->prjname.assign(opt);
}
