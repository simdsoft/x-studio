// scicmds_autogen.cpp : This file contains the 'main' function. Program execution begins and ends there.
// v0.0.1

#include <iostream>
#include <fstream>
#include <sstream>
#include <string_view>
#include <algorithm>
#include <unordered_map>
#include "pugixml.hpp"

namespace internal {
#define _REDIRECT_DELIM_INTRI(_From,_To,_CStr,_Delim) \
    inline _CStr _To(_CStr s, _Delim d) \
    { \
        return _From(s, d); \
    }

    _REDIRECT_DELIM_INTRI(strchr, xstrchr, const char*, int)
        _REDIRECT_DELIM_INTRI(strchr, xstrchr, char*, int)
        _REDIRECT_DELIM_INTRI(wcschr, xstrchr, const wchar_t*, wchar_t)
        _REDIRECT_DELIM_INTRI(wcschr, xstrchr, wchar_t*, wchar_t)

        _REDIRECT_DELIM_INTRI(strstr, xstrstr, const char*, const char*)
        _REDIRECT_DELIM_INTRI(strstr, xstrstr, char*, const char*)
        _REDIRECT_DELIM_INTRI(wcsstr, xstrstr, const wchar_t*, const wchar_t*)
        _REDIRECT_DELIM_INTRI(wcsstr, xstrstr, wchar_t*, const wchar_t*)

        _REDIRECT_DELIM_INTRI(strpbrk, xstrpbrk, const char*, const char*)
        _REDIRECT_DELIM_INTRI(strpbrk, xstrpbrk, char*, const char*)
        _REDIRECT_DELIM_INTRI(wcspbrk, xstrpbrk, const wchar_t*, const wchar_t*)
        _REDIRECT_DELIM_INTRI(wcspbrk, xstrpbrk, wchar_t*, const wchar_t*)
}

namespace nzls { // non zero length string
    /*
    * nsc::fast_split_nzls API (override+3) ignore empty string
    * op prototype: [](const char* start, const char* end){}
    */
    template<typename _CStr, typename _Fn> inline
        void fast_split(_CStr s, size_t slen, typename std::remove_pointer<_CStr>::type delim, _Fn func)
    {
        auto _Start = s; // the start of every string
        auto _Ptr = s; // source string iterator
        auto _End = s + slen;
        while ((_Ptr = internal::xstrchr(_Ptr, delim)))
        {
            if (_Start < _Ptr)
                func(_Start, _Ptr);
            _Start = _Ptr + 1;
            ++_Ptr;
        }
        if (_Start < _End) {
            func(_Start, _End);
        }
    }

    template<typename _CStr, typename _Fn> inline
        void fast_split(_CStr s, size_t slen, const typename std::remove_pointer<_CStr>::type* delims, size_t dlen, _Fn func)
    {
        auto _Start = s; // the start of every string
        auto _Ptr = s; // source string iterator
        auto _End = s + slen;
        while ((_Ptr = internal::xstrstr(_Ptr, delims)))
        {
            if (_Start < _Ptr)
            {
                func(_Start, _Ptr);
            }
            _Start = _Ptr + dlen;
            _Ptr += dlen;
        }
        if (_Start < _End) {
            func(_Start, _End);
        }
    }

    template<typename _CStr, typename _Fn> inline
        void fast_split_of(_CStr s, size_t slen, const typename std::remove_pointer<_CStr>::type* delims, _Fn func)
    {
        auto _Start = s; // the start of every string
        auto _Ptr = s; // source string iterator
        auto _End = s + slen;
        auto _Delim = delims[0];
        while ((_Ptr = internal::xstrpbrk(_Ptr, delims)))
        {
            if (_Start < _Ptr)
            {
                func(_Start, _Ptr, _Delim);
                _Delim = *_Ptr;
            }
            _Start = _Ptr + 1;
            ++_Ptr;
        }
        if (_Start < _End) {
            func(_Start, _End, _Delim);
        }
    }

    /*
    * nsc::fast_split_nzls_brkif API (override+3) ignore empty string
    * op prototype: [](const char* start, const char* end){}
    */
    template<typename _CStr, typename _Fn> inline
        void fast_split_brkif(_CStr s, size_t slen, typename std::remove_pointer<_CStr>::type delim, _Fn func)
    {
        auto _Start = s; // the start of every string
        auto _Ptr = s; // source string iterator
        auto _End = s + slen;
        while ((_Ptr = internal::xstrchr(_Ptr, delim)))
        {
            if (_Start < _Ptr)
                if (func(_Start, _Ptr))
                    return;
            _Start = _Ptr + 1;
            ++_Ptr;
        }
        if (_Start < _End) {
            func(_Start, _End);
        }
    }

    template<typename _CStr, typename _Fn> inline
        void fast_split_brkif(_CStr s, size_t slen, const typename std::remove_pointer<_CStr>::type* delims, size_t dlen, _Fn func)
    {
        auto _Start = s; // the start of every string
        auto _Ptr = s; // source string iterator
        auto _End = s + slen;
        while ((_Ptr = internal::xstrstr(_Ptr, delims)))
        {
            if (_Start < _Ptr)
            {
                if (func(_Start, _Ptr))
                    return;
            }
            _Start = _Ptr + dlen;
            _Ptr += dlen;
        }
        if (_Start < _End) {
            func(_Start, _End);
        }
    }

    template<typename _CStr, typename _Fn> inline
        void fast_split_of_brkif(_CStr s, size_t slen, const typename std::remove_pointer<_CStr>::type* delims, _Fn func)
    {
        auto _Start = s; // the start of every string
        auto _Ptr = s; // source string iterator
        auto _End = s + slen;
        auto _Delim = delims[0];
        while ((_Ptr = internal::xstrpbrk(_Ptr, delims)))
        {
            if (_Start < _Ptr)
            {
                if (func(_Start, _Ptr, _Delim))
                    return;
                _Delim = *_Ptr;
            }
            _Start = _Ptr + 1;
            ++_Ptr;
        }
        if (_Start < _End) {
            func(_Start, _End, _Delim);
        }
    }

    /// C++ basic_string wrappers
    template<typename _StringCont, typename _Fn> inline
        void fast_split(_StringCont& s, typename _StringCont::value_type delim, _Fn func)
    {
        typename _StringCont::value_type dummy[1] = { 0 };
        return fast_split(!s.empty() ? &s.front() : &dummy[0], s.length(), delim, func);
    }

#if _HAS_CXX17
    /// C++17 basic_string_view wrappers
    template<typename _Elem, typename _Fn> inline
        void fast_split(std::basic_string_view<_Elem> s, _Elem delim, _Fn func)
    {
        return fast_split(s.data(), s.length(), delim, func);
    }
    template<typename _Elem, typename _Fn> inline
        void fast_split(std::basic_string_view<_Elem> s, std::basic_string_view<_Elem> delims, _Fn func)
    {
        return fast_split(s.data(), s.length(), delims.data(), delims.length(), func);
    }
    template<typename _Elem, typename _Fn> inline
        void fast_split_of(std::basic_string_view<_Elem> s, const _Elem* delims, _Fn func)
    {
        return fast_split_of(s.data(), s.length(), delims, func);
    }
#endif

    /// simple retun a vector<basic_string> as string array
#if _HAS_CXX17
    template<typename _Elem> inline
        std::vector<std::basic_string<_Elem>> split(std::basic_string_view<_Elem> s, _Elem delim)
    {
        std::vector<std::basic_string<_Elem>> results;
        fast_split(s, delim, [&](const _Elem* s, const _Elem* e) {
            results.push_back(std::basic_string<_Elem>(s, e));
            });
        return results;
    }
    template<typename _Elem> inline
        std::vector<std::basic_string<_Elem>> split(const std::basic_string<_Elem>& s, _Elem delim)
    {
        return split(std::basic_string_view<_Elem>(s.c_str(), s.length()), delim);
    }

    template<typename _Elem> inline
        std::vector<std::basic_string<_Elem>> split(std::basic_string_view<_Elem> s, std::basic_string_view<_Elem> delims)
    {
        std::vector<std::basic_string<_Elem>> results;
        fast_split(s, delims, [&](const _Elem* s, const _Elem* e) {
            results.push_back(std::basic_string<_Elem>(s, e));
            });
        return results;
    }
    template<typename _Elem> inline
        std::vector<std::basic_string<_Elem>> split(const std::basic_string<_Elem>& s, std::basic_string_view<_Elem> delims)
    {
        return split(std::basic_string_view<_Elem>(s.c_str(), s.length()), delims);
    }

    template<typename _Elem> inline
        std::vector<std::basic_string<_Elem>> split_of(std::basic_string_view<_Elem> s, const _Elem* delims)
    {
        std::vector<std::basic_string<_Elem>> results;
        fast_split_of(s, delims, [&](const _Elem* s, const _Elem* e, _Elem) {
            results.push_back(std::basic_string<_Elem>(s, e));
            });
        return results;
    }
    template<typename _Elem> inline
        std::vector<std::basic_string<_Elem>> split_of(const std::basic_string<_Elem>& s, const _Elem* delims)
    {
        return split_of(std::basic_string_view<_Elem>(s.c_str(), s.length()), delims);
    }
#endif
}

static void parseMacros(const char* cxxFile, std::unordered_map<std::string, std::string>& macros)
{
    std::ifstream fin(cxxFile);
    if (!fin.is_open()) return;

    char* line = new char[1024 * 1024 * 10];

    do {
        if (!fin.getline(line, 1024 * 1024 * 10 - 1).good())
            break;

        std::string_view lineview = line;
        auto defined = lineview._Starts_with("#define");
        if (defined)
        {
            auto words = nzls::split(lineview, ' ');
            if (words.size() == 3) {
                macros.emplace(std::move(words[1]), std::move(words[2]));
            }
        }

    } while (!fin.eof());
    fin.close();
}

int main()
{
    std::unordered_map<std::string, std::string> macros;
    std::vector<std::pair<std::string, std::string>> scicmds;

    /// Step.1: parse macros from Scintilla.h & ScintillaExt.h
    parseMacros("Scintilla.h", macros);
    parseMacros("ScintillaExt.h", macros);

    /// Step.2: parse supported commands from scicmds.xml and fill value by macros parsed from Scintilla.h & ScintillaExt.h
    pugi::xml_document d;
    d.load_file("scicmds.xml");
    auto cmds = d.select_nodes("/table/tbody/tr/td/code");
    for (auto cmd : cmds) {
        std::string macroName = cmd.node().child_value();
        auto it = macros.find(macroName);
        if (it != macros.end()) {
            scicmds.push_back(std::make_pair(std::move(macroName), it->second));
        }
    }
    scicmds.push_back(std::make_pair("SCI_FINDANDADDSELECTION", "2911"));

    /// Step.3: generate lua code
    std::stringstream ss;

    ss << "-- This file is generated by scicmds_autogen automatically.\n";

    ss << "scicmds = {\n";
    for (auto& cmd : scicmds) {
        ss << "    " << cmd.first << " = " << cmd.second << ",\n";
    }

    ss << "}";
    
    std::string code = ss.str();
    std::ofstream outf("scicmds.lua");
    if (outf.is_open()) {
        outf << code;
        outf.close();
    }
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
