//
// Copyright (c) 2014-2015 purelib - All Rights Reserved
//
#ifndef _XXFSUTILITY_H_
#define _XXFSUTILITY_H_
#include <string>
#include <vector>
#ifdef _WIN32
#include <io.h>
#include <direct.h> // for CRT function: access
#else
#include <unistd.h> // for CRT function: access
#include <sys/types.h> // for CRT function: mkdir
#include <sys/stat.h>  // for CRT function: mkdir
#endif
#include <vector>


#ifdef _WIN32
#include <Windows.h>
#include <shellapi.h>


#endif

#include "object_pool.h"

#include <list>

using namespace purelib;
using namespace purelib::gc;

namespace fsutil {

long             get_file_size(const char* filename);
long             get_file_size(FILE* fp);

std::string      read_file_data(const char* filename);
void             read_file_data(const char *filename, std::string& data);
void             read_file_data(const char *filename, std::vector<char>& data);

std::string      read_file_data_ex(const char* filename, size_t align = 16);
void             read_file_data_ex(const char *filename, std::string& data, size_t align = 16);
void             read_file_data_ex(const char *filename, std::vector<char>& data, size_t align = 16);
/// Read file data with null-terminated '\0'
void             read_file_data_as_string(const char *filename, std::vector<char>& output);
void             read_file_data_as_string(const char *filename, std::string& output);

bool             write_file_data(const char* filename, const char* data, size_t size);
bool             write_file_data(const char* filename, const std::string& content);

template<size_t _Size> inline
void             write_file_data(const char* filename, const char(&data)[_Size])
{
    if (_Size > 1)
        write_file_data(filename, data, _Size - 1);
}

void             append_file_data(const char* filename, const char* data, size_t size);
void             append_file_data(const char* filename, const std::string& content);

template<size_t _Size> inline
void             append_file_data(const char* filename, const char(&data)[_Size])
{
    if (_Size > 1) {
        append_file_data(filename, data, _Size - 1);
    }
}

bool             exists(const char* pszPath);
bool             is_dir_exists(const char* dir);
bool             is_file_exists(const char* filename);

bool             is_type_of(const std::string& filename, const char* type);

bool             is_type_of_v2(const std::string& filename, const char* type);

void             mkdir(const char* directory);
void             mkdir(std::string& directory);
void             mkdir(std::string&& directory);

std::pair<std::string, std::string> split_fullpath(const std::string& fullpath);

std::string      get_short_name(const std::string& complete_filename);
std::string      get_path_of(const std::string& complete_filename);

const char*      get_extend(const char* filename);
const char*      get_extend(const std::string& filename);

/* get extend with wildcard, such as *.png */
std::string      get_type(const std::string& filename);
std::string      get_name(const std::string& filename);
std::string      remove_extend(const std::string& filename);

/// win32 spec APIs
#ifdef _WIN32

std::string      to_absolute(const std::string& path);
bool             is_absolute(const std::string& path);

std::string      to_relative(const std::string& from, bool fromIsDir, const std::string& to, bool toIsDir);
std::string      to_relative(const std::string& target, bool dir);

std::string      get_current_directory(void);
void             set_current_directory(const std::wstring& cd);
void             set_current_directory(const std::string& cd);

std::string      get_module_directory(void);

#define OFN_FILTER(tips,filters) TEXT(tips) TEXT("\0") TEXT(filters) TEXT("\0")
#define OFN_FILTER_END()         TEXT("\0")

std::string      get_open_filename(TCHAR* dir, const TCHAR* filters, HWND hwndParent = nullptr);
bool             get_open_filename(TCHAR* output, const TCHAR* filters, TCHAR* history = nullptr, HWND hWnd = nullptr);
std::string      get_save_filename(TCHAR* dir, const TCHAR* filters, HWND = nullptr);
TCHAR*           get_open_path(TCHAR* out, TCHAR* history = TEXT(""), HWND hWnd = nullptr);

INT CALLBACK     _BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM pData);

struct search_file_info {
    std::string subtree;
    std::string filename;
    std::size_t extp; // extension index
};

void search_df(const std::string& root, 
    const TCHAR* dir, 
    std::vector<search_file_info>& filelist, 
    bool recursive = false, 
    const char* filter = "*.*",
    unsigned int deep = 0);

void search_df(const std::string& root, 
    const TCHAR* dir, 
    std::list<search_file_info, object_pool_allocator<search_file_info, 256>>& filelist, 
    bool recursive = false, 
    const char* filter = "*.*");


#endif /* end of win32 utils */

};

#endif

