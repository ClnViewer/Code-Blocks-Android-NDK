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

int main(int argc, const char *argv[])
{
    CbConf cnf(argv, argc);

    if (!cnf.isarg)
    {
        const char *exebin = strrchr(argv[0], '\\');
        exebin = ((exebin) ? (exebin + 1) : argv[0]);

        std::cout << std::endl << "   Code::Blocks to Android NDK configuration converter v." << CBP_FULLVERSION_STRING << " (" << CBP_DATE << "." << CBP_MONTH << "." << CBP_YEAR << ")" << std::endl;
        std::cout << "   C::B to NDK HOWTO: https://clnviewer.github.io/Code-Blocks-Android-NDK/" << std::endl;
        std::cout << "   Android.mk  HOWTO: https://developer.android.com/ndk/guides/android_mk" << std::endl << std::endl;
        std::cout << "   Options: " << std::endl;
        std::cout << "\t-a, --auto\tfind .cbp project file from current directory" << std::endl;
        std::cout << "\t-c, --cbp\tpath to .cbp project file" << std::endl;
        std::cout << "\t-d, --dump\tdump current configuration" << std::endl;
        std::cout << "\t-t, --tag\tbuilding tag: Debug|Release|OtherTag" << std::endl;
        std::cout << "\t-q, --quiet\tquiet all messages" << std::endl;
        std::cout << "\t-v, --verbose\tverbose output to console" << std::endl;
        std::cout << "\t-n  --nodefault\tno set default values" << std::endl;
        std::cout << "\t    --cbtmpl\tinstall C::B wizard template Makefile" << std::endl;
        std::cout << "\t    --api\tandroid API number (Application.mk)" << std::endl;
        std::cout << "\t    --abi\tandroid ABI platform (Application.mk)" << std::endl;
        std::cout << "\t    --ndkopt\tandroid NDK options (Application.mk)" << std::endl << std::endl;
        std::cout << "   Using: " << std::endl;
        std::cout << "\t" << exebin << " <BuildTag> <path\\project.cbp>" << std::endl;
        std::cout << "\t" << exebin << " -t <BuildTag> -c <path\\project.cbp> -v" << std::endl;
        std::cout << "\t" << exebin << " -a --api android-28 --abi armeabi-v7a --ndkopt debug" << std::endl;
        std::cout << "\t" << exebin << " -a" << std::endl;
        return 127;
    }

	try
	{
		auto doc = tinyxml2::load_xmlfile(cnf.fname[0]);

		if (cnf.isverb)
            std::cout << " * Open (xml):       " << cnf.fname[0].c_str() << std::endl;

        /// begin XML configuration parse

        if ((!cnf.ismkf) && (cnf.iscbtmpl))
            write_makefcb(&cnf);
        else if (!cnf.ismkf)
            write_makef(&cnf);

        if ((!cnf.isapp) && (!cnf.isabi))
            write_appmk(&cnf);
        else if (cnf.isabi)
            write_appmk_custom(&cnf);

        if (!cnf.isand)
        {
            parse_section(
                &cnf,
                static_cast<const tinyxml2::XMLDocument&>(*doc),
                "CodeBlocks_project_file/Project/Option"s,
                "title"s,
                [](CbConf *pcnf, std::string & opt)
                    {
                        parse_prjname(pcnf, opt);
                    }
            );

            if (cnf.prjname.empty())
                throw tinyxml2::XmlException("no project name in cbp configuration");

            write_andmk(&cnf);
        }

        parse_section(
            &cnf,
            static_cast<const tinyxml2::XMLDocument&>(*doc),
            "CodeBlocks_project_file/Project/Unit"s,
            "filename"s,
             [](CbConf *pcnf, std::string & opt)
                {
                    parse_srclist(pcnf, opt);
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
                (!string_end(cnf.tag, taged))
               )
                continue;

            parse_section(
                &cnf,
                root,
                "Compiler/Add"s,
                "option"s,
                [](CbConf *pcnf, std::string & opt)
                    {
                        parse_cflag(pcnf, opt);
                    }
            );
            parse_section(
                &cnf,
                root,
                "Linker/Add"s,
                "option"s,
                [](CbConf *pcnf, std::string & opt)
                    {
                        parse_ldflag(pcnf, opt);
                    }
            );
            parse_section(
                &cnf,
                root,
                "Option"s,
                "parameters"s,
                [](CbConf *pcnf, std::string & opt)
                    {
                        pcnf->v[elabels::LBL_CMDL].push_back(opt);
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
                        parse_cflag(pcnf, opt);
                    }
            );
            parse_section(
                &cnf,
                root,
                "Linker/Add"s,
                "option"s,
                [](CbConf *pcnf, std::string & opt)
                    {
                        parse_ldflag(pcnf, opt);
                    }
            );
        }

        /// append default values
        if (!cnf.isnodef)
        {
            cnf.v[elabels::LBL_LDLIBS].push_back(" -llog");
            cnf.v[elabels::LBL_CFLAG].push_back(" -D__ANDROID_API_PLATFORM__=\"$(TARGET_PLATFORM)\"");
            cnf.v[elabels::LBL_HINC].push_back(" ./");
        }

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

        if (cnf.isdump)
            dump_CbConf(&cnf);

        /// end XML configuration parse

        FILE __AUTO(__autofile) *fpi = NULL;
        FILE __AUTO(__autofile) *fpo = NULL;

        if (!(fpi = fopen(cnf.fname[1].c_str(), "rt")))
            throw tinyxml2::XmlException("open file Android.mk to write");

        if (cnf.isverb)
            std::cout << " * Open (r):         " << cnf.fname[1].c_str() << std::endl;

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

        if (cnf.isverb)
            std::cout << " * Open (w):         " << cnf.fname[2].c_str() << std::endl;

        bool iswrite = false;
        char *buf = new char[sz]{};

        while(fgets(buf, sz, fpi))
        {
            bool isskip = false;
            for (int n = elabels::LBL_CEXT; n < _END_FOR_ARRAY; n++)
            {
                size_t sp;
                std::string needle(get_label(n));

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
                        if (if_section(&cnf, i))
                        {
                            if (!write_label(fpo, i, false))
                                break;
                            if (!write_section(fpo, &cnf, i))
                                break;
                        }
                    }
                    if (!if_section(&cnf, elabels::LBL_CMDL))
                        (void) write_label(fpo, elabels::LBL_CMDL, true);
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

        if (!cnf.isquiet)
            std::cout << " * C::B -> Android NDK - convertible configuration done." << std::endl;
	}
	catch (tinyxml2::XmlException & _ex)
	{
	    if (!cnf.isquiet)
            std::cout << " ! Error : " << _ex.what() << std::endl;
		return 125;
	}
	catch (std::exception & _ex)
	{
	    if (!cnf.isquiet)
            std::cout << " ! Exception : " << _ex.what() << std::endl;
		return 126;
	}

    return 0;
}
