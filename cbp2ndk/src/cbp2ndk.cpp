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
#include <io.h>

using namespace std;

static const char aconf_default_begin[] =
    "LOCAL_PATH := $(call my-dir)\n" \
    "include $(CLEAR_VARS)\n";

static const char aconf_default_end[] =
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

inline bool string_end(std::string const & val, std::string const & ending)
{
    if (ending.size() > val.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), val.rbegin());
}

inline bool string_begin(std::string const & val, std::string const & start)
{
    if (start.size() > val.size()) return false;
    return std::equal(start.begin(), start.end(), val.begin());
}

bool set_path(CbConf *pcnf, const char *argv)
{
    pcnf->fname[0] = std::string(argv);

    if (::_access(pcnf->fname[0].c_str(), F_OK) < 0)
        throw tinyxml2::XmlException("open cbp file: " + pcnf->fname[0]);

    size_t pos = pcnf->fname[0].find_last_of("/\\");

    if (pos == std::string::npos)
    {
        pcnf->fname[1] = "Android.mk";
        pcnf->fname[2] = "Android.mk.tmp";
    }
    else
    {
        std::string fpath =  pcnf->fname[0].substr(0,pos);
        pcnf->fname[1] = fpath + __SEPARATOR_PATH "Android.mk";
        pcnf->fname[2] = fpath + __SEPARATOR_PATH "Android.mk.tmp";
    }

    if (::_access(pcnf->fname[1].c_str(), F_OK) < 0)
        return false;
    return true;
}

int main(int argc, const char *argv[])
{
    if (argc < 3)
    {
        const char *exebin = strrchr(argv[0], '\\');
        exebin = ((exebin) ? (exebin + 1) : argv[0]);

        std::cout << std::endl << "   Code::Blocks to Android NDK configuration converter v." << FULLVERSION_STRING << " (" << DATE << "." << MONTH << "." << YEAR << ")" << std::endl;
        std::cout << "   C::B to NDK HOWTO: https://clnviewer.github.io/Code-Blocks-Android-NDK/" << std::endl;
        std::cout << "   Android.mk  HOWTO: https://developer.android.com/ndk/guides/android_mk" << std::endl << std::endl;
        std::cout << "   Using: " << exebin << " <Debug|Release> <path\\project.cbp>" << std::endl;
        return 127;
    }

	try
	{
	    CbConf cnf{};
	    bool iscnf = set_path(&cnf, argv[2]);
	    std::string tag(argv[1]);

		auto doc = tinyxml2::load_xmlfile(cnf.fname[0]);

#       if defined(_DEBUG)
        std::cout << " * Open (xml):       " << cnf.fname[0].c_str() << std::endl;
#       endif

        /// begin XML configuration parse

        if (!iscnf)
        {
            parse_section(
                &cnf,
                static_cast<const tinyxml2::XMLDocument&>(*doc),
                "CodeBlocks_project_file/Project/Option"s,
                "title"s,
                [](CbConf *pcnf, std::string & opt)
                    {
                        pcnf->prjname.assign(opt);
                    }
            );

            if (cnf.prjname.empty())
                throw tinyxml2::XmlException("no project name in cbp configuration");

            FILE __AUTO(__autofile) *fp = NULL;

            if (!(fp = fopen(cnf.fname[1].c_str(), "wt")))
                throw tinyxml2::XmlException("open file Android.mk to write");

#           if defined(_DEBUG)
            std::cout << " * Create (default): " << cnf.fname[1].c_str() << std::endl;
#           endif

            std::string nmodule(labels[elabels::LBL_NAME]);
            nmodule.append(" " + cnf.prjname + "\n");

            fwrite(aconf_default_begin, 1, __CSZ(aconf_default_begin), fp);
            fwrite(nmodule.data(), 1, nmodule.length(), fp);
            fwrite(aconf_default_end, 1, __CSZ(aconf_default_end), fp);
            fclose(fp); fp = NULL;
        }

        parse_section(
            &cnf,
            static_cast<const tinyxml2::XMLDocument&>(*doc),
            "CodeBlocks_project_file/Project/Unit"s,
            "filename"s,
             [](CbConf *pcnf, std::string & opt)
                {
                    if (
                        (string_end(opt, ".h"))   ||
                        (string_end(opt, ".hxx")) ||
                        (string_end(opt, ".hpp"))
                    )
                    {
                        size_t sp;
                        if ((sp = opt.find("/")) != std::wstring::npos)
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
                        size_t sp;
                        if ((sp = opt.find(".")) != std::wstring::npos)
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
                        pcnf->v[elabels::LBL_CSRC].push_back(opt);
                    }
                    else
                        std::cout << " ! NOT support file extension: " << opt.c_str() << std::endl;
                }
        );
		for (auto root :
             tinyxml2::selection(
                static_cast<const tinyxml2::XMLDocument&>(*doc),
                "CodeBlocks_project_file/Project/Build/Target"
            )
        ) {
            std::string taged = attribute_value(root, "title");
            if (
                (taged.empty()) ||
                (!string_end(tag, taged))
               )
                continue;

            parse_section(
                &cnf,
                root,
                "Compiler/Add"s,
                "option"s,
                [](CbConf *pcnf, std::string & opt)
                    {
                        if (string_begin(opt, " -I"))
                        {
                            opt.erase(1,2);
                            pcnf->v[elabels::LBL_HINC].push_back(opt);
                        }
                        else
                            pcnf->v[elabels::LBL_COMP].push_back(opt);
                    }
            );
            parse_section(
                &cnf,
                root,
                "Linker/Add"s,
                "option"s,
                [](CbConf *pcnf, std::string & opt)
                    {
                        if (string_begin(opt, " -l"))
                            pcnf->v[elabels::LBL_LDLIBS].push_back(opt);
                        else
                            pcnf->v[elabels::LBL_LDFLAG].push_back(opt);
                    }
            );
        }
		for (auto root :
             tinyxml2::selection(
                static_cast<const tinyxml2::XMLDocument&>(*doc),
                "CodeBlocks_project_file/Project"
            )
        ) {
            parse_section(
                &cnf,
                root,
                "Compiler/Add"s,
                "option"s,
                [](CbConf *pcnf, std::string & opt)
                    {
                        if (string_begin(opt, " -I"))
                        {
                            opt.erase(1,2);
                            pcnf->v[elabels::LBL_HINC].push_back(opt);
                        }
                        else
                            pcnf->v[elabels::LBL_COMP].push_back(opt);
                    }
            );
            parse_section(
                &cnf,
                root,
                "Linker/Add"s,
                "option"s,
                [](CbConf *pcnf, std::string & opt)
                    {
                        if (string_begin(opt, " -l"))
                            pcnf->v[elabels::LBL_LDLIBS].push_back(opt);
                        else
                            pcnf->v[elabels::LBL_LDFLAG].push_back(opt);
                    }
            );
        }

        /// append default values
        cnf.v[elabels::LBL_LDLIBS].push_back(" -llog");
        cnf.v[elabels::LBL_HINC].push_back(" ./");


        /// map to values
        std::map<std::string, int>::iterator mit0 = cnf.m[0].begin();
        while(mit0 != cnf.m[0].end())
        {
            std::string opt = mit0->first;
            if (opt.empty())
                continue;

            opt.insert(0, " ");
            cnf.v[elabels::LBL_CEXT].push_back(opt);
            mit0++;
        }

        std::map<std::string, int>::iterator mit1 = cnf.m[1].begin();
        while(mit1 != cnf.m[1].end())
        {
            std::string opt = mit1->first;
            if (opt.empty())
                continue;

            opt.insert(0, " ");
            cnf.v[elabels::LBL_HINC].push_back(opt);
            mit1++;
        }

        /// end XML configuration parse

        FILE __AUTO(__autofile) *fpi = NULL;
        FILE __AUTO(__autofile) *fpo = NULL;

        if (!(fpi = fopen(cnf.fname[1].c_str(), "rt")))
            throw tinyxml2::XmlException("open file Android.mk to write");

#       if defined(_DEBUG)
        std::cout << " * Open (r):         " << cnf.fname[1].c_str() << std::endl;
#       endif

        ::fseek(fpi, 0L, SEEK_END);
        size_t sz = static_cast<size_t>(::ftell(fpi));
        if (!sz)
        {
            fclose(fpi); fpi = NULL;
            (void) remove(cnf.fname[1].c_str());
            throw tinyxml2::XmlException("file Android.mk is empty, deleted, re-run it..");
        }

        ::fseek(fpi, 0L, SEEK_SET);

        if (!(fpo = fopen(cnf.fname[2].c_str(), "wt")))
            throw tinyxml2::XmlException("open file Android.mk.tmp to write");

#       if defined(_DEBUG)
        std::cout << " * Open (w):         " << cnf.fname[2].c_str() << std::endl;
#       endif

        bool iswrite = false;
        char *buf = new char[sz]{};

        while(fgets(buf, sz, fpi))
        {
            bool isskip = false;
            for (int n = elabels::LBL_CEXT; n < _END_FOR_ARRAY; n++)
            {
                size_t sp;
                std::string needle(labels[n]);

                if ((sp = needle.find(" ")) != std::wstring::npos)
                {
                    needle = needle.substr(0, sp - 1);
                    if (!needle.empty())
                    {
                        if (string_begin(buf, needle))
                        {
                            isskip = true;
                            break;
                        }
                    }
                }
            }
            if (isskip)
            {
                if (!iswrite)
                {
                    for (int i = elabels::LBL_CEXT; i < _END_FOR_ARRAY; i++)
                    {
                        if (if_section(i, &cnf))
                        {
                            if (!write_label(fpo, labels, i))
                                break;
                            if (!write_section(fpo, &cnf, labels, i))
                                break;
                        }
                    }
                    iswrite = true;
                }
                continue;
            }
            fwrite(buf, 1, strlen(buf), fpo);
        }
        delete [] buf;

        fclose(fpi); fpi = NULL;
        fclose(fpo); fpo = NULL;

        if (remove(cnf.fname[1].c_str()))
            throw tinyxml2::XmlException("remove old file Android.mk");
        if (rename(cnf.fname[2].c_str(), cnf.fname[1].c_str()))
            throw tinyxml2::XmlException("move new file Android.mk");

        std::cout << " * Convertible configuration done." << std::endl;
	}
	catch (tinyxml2::XmlException & e)
	{
		std::cout << " ! Error : " << e.what() << std::endl;
		return 126;
	}

    return 0;
}
