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

static const char android_default_begin[] =
    "LOCAL_PATH := $(call my-dir)\n" \
    "include $(CLEAR_VARS)\n";

static const char android_default_end[] =
    "LOCAL_SRC_FILES :=\n" \
    "include $(BUILD_EXECUTABLE)\n";

static const char *labels[] =
{
  static_cast<const char*>("LOCAL_CPP_EXTENSION :="),
  static_cast<const char*>("LOCAL_SRC_FILES :="),
  static_cast<const char*>("LOCAL_CFLAGS :="),
  static_cast<const char*>("LOCAL_LDFLAGS :="),
  static_cast<const char*>("LOCAL_LDLIBS :="),
  static_cast<const char*>("LOCAL_C_INCLUDES :="),
  static_cast<const char*>("LOCAL_MODULE :="),
  static_cast<const char*>("\n")
};

PBYTE get_resource(LPCSTR sid, size_t *sz)
{
    do
    {
        HMODULE hm;
        HRSRC hres;
        HGLOBAL hmem;
        PBYTE data;

        if ((hm = GetModuleHandleA(NULL)) == INVALID_HANDLE_VALUE)
            break;
        if (!(hres = FindResourceA(hm, sid, RT_RCDATA)))
            break;
        if (!(hmem = LoadResource(hm, hres)))
            break;
        if (!(*sz = SizeofResource(hm, hres)))
            break;
        if (!(data = static_cast<PBYTE>(LockResource(hmem))))
            break;

        return data;
    }
    while (0);

    return NULL;
}

const char * get_label(int32_t idx)
{
    if ((idx < 0) || (idx >= static_cast<int32_t>(__NELE(labels))))
        return labels[(__NELE(labels) - 1)];
    return labels[idx];
}

bool if_section(CbConf *pcnf, int32_t idx)
{
    return (pcnf->v[idx].size());
}

bool write_label(FILE *fp, int32_t idx)
{
    size_t sz = strlen(get_label(idx));
    if (fwrite(get_label(idx), 1, sz, fp) != sz)
        return false;

    return true;
}

bool write_section(FILE *fp, CbConf *pcnf, int32_t idx)
{
    for (auto &val : pcnf->v[idx])
    {
#       if defined(_DEBUG)
        if (pcnf->isverb)
            std::cout << "\t+ : " << val << std::endl;
#       endif
        if (fwrite(val.data(), 1, val.length(), fp) != val.length())
            return false;
    }

    size_t sz = strlen(get_label(elabels::LBL_END));
    if (fwrite(get_label(elabels::LBL_END), 1, sz, fp) != sz)
        return false;

    return true;
}

static void write_data(CbConf *pcnf, LPCSTR sid, int32_t fid)
{
    FILE __AUTO(__autofile) *fp = NULL;

    if (!(fp = fopen(pcnf->fname[fid].c_str(), "wt")))
        throw tinyxml2::XmlException("open file to write");

#   if defined(_DEBUG)
    if (pcnf->isverb)
        std::cout << " * Create (default): " << pcnf->fname[fid].c_str() << std::endl;
#   endif

    do
    {
        PBYTE data;
        size_t sz;

        if (
            (!(data = get_resource(sid, &sz))) ||
            (!sz)
        )
            break;

        fwrite(data, 1, sz, fp);
        fclose(fp); fp = NULL;

        return;
    }
    while (0);

    throw tinyxml2::XmlException("get/set resource");
}

void write_andmk(CbConf *pcnf)
{
    FILE __AUTO(__autofile) *fp = NULL;

    if (!(fp = fopen(pcnf->fname[1].c_str(), "wt")))
        throw tinyxml2::XmlException("open file Android.mk to write");

#   if defined(_DEBUG)
    if (pcnf->isverb)
        std::cout << " * Create (default): " << pcnf->fname[1].c_str() << std::endl;
#   endif

    std::string nmodule(get_label(elabels::LBL_NAME));
    nmodule.append(" " + pcnf->prjname + "\n");

    fwrite(android_default_begin, 1, __CSZ(android_default_begin), fp);
    fwrite(nmodule.data(), 1, nmodule.length(), fp);
    fwrite(android_default_end, 1, __CSZ(android_default_end), fp);
    fclose(fp); fp = NULL;
}

void write_appmk(CbConf *pcnf)
{
    write_data(pcnf, static_cast<LPCSTR>("APPDATA"), 3);
}

void write_makef(CbConf *pcnf)
{
    write_data(pcnf, static_cast<LPCSTR>("MKFDATA"), 4);
    if (!pcnf->isquiet)
        std::cout << " ? Warning : New Makefile - you need to edit the NDKROOT variable pointing to the path to the Android NDK on your system." << std::endl;
}
