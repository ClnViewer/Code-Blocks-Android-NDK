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

