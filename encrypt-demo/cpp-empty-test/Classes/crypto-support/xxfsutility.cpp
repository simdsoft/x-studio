#include "nsconv.h"
#include <fstream>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#ifdef _WIN32
#include <io.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#else
#define O_BINARY 0
#include <dirent.h>
#endif
#include "xxfsutility.h"
#include "nsconv.h"
#include <set>

USING_NS_PURELIB;

long fsutil::get_file_size(const char *path)
{
    long filesize = -1;
    struct stat statbuff;
    if (stat(path, &statbuff) < 0){
        return filesize;
    }
    else{
        filesize = statbuff.st_size;
    }
    return filesize;
}

long fsutil::get_file_size(FILE* fp)
{
    fseek(fp, 0, SEEK_END);
    long length = ftell(fp);
    if(length != 0)
    {
        fseek(fp, 0, SEEK_SET);
    }
    return length;
}

std::string fsutil::read_file_data(const char* filename)
{
    FILE* fp = fopen(filename, "rb");
    if(fp == nullptr) 
        return (const char*)"";

    size_t size = get_file_size(fp);
	if (size == 0)
		return "";

    std::string storage(size, '\0');

    size_t bytes_readed = fread(&storage.front(), 1, size, fp);

    fclose(fp);
    if (bytes_readed < size)
        storage.resize(bytes_readed);
    return std::move(storage);
}

/// Read data, store to std::string
void fsutil::read_file_data(const char *filename, std::string& data)
{
    using namespace std;

    // Open stream
    std::ifstream stream(filename, ios::binary);
    if (!stream)
        throw runtime_error(std::string("cannot open file ") + filename);
    stream.unsetf(ios::skipws);

    // Determine stream size
    stream.seekg(0, ios::end);
    size_t size = stream.tellg();
    stream.seekg(0);   

    // Load data and add terminating automatically by std::string
    data.resize(size);
    stream.read(&data.front(), static_cast<streamsize>(size));
}

std::string fsutil::read_file_data_ex(const char* filename, size_t align)
{
    FILE* fp = fopen(filename, "rb");
    if(fp == nullptr) 
        return (char*)"";

    size_t size = get_file_size(fp);

    std::string storage(size + align, '\0');

    size_t bytes_readed = fread(&storage, 1, size, fp);
    fclose(fp);

    size_t padding_size = 0;
    if(align > 0) {
        padding_size = align - size % align;
        for(size_t offst = 0; offst < padding_size; ++offst) 
        {
            *(&storage.front() + bytes_readed + offst) = padding_size;
        }
    }

    storage.resize(bytes_readed + padding_size);
    
    return std::move(storage);
}

/// Read data and add terminating 0
void fsutil::read_file_data_ex(const char *filename, std::string& data, size_t align)
{
    // Open stream
    std::ifstream stream(filename, std::ios::binary);
    if (!stream)
        throw std::runtime_error(std::string("cannot open file ") + filename);
    stream.unsetf(std::ios::skipws);

    // Determine stream size
    stream.seekg(0, std::ios::end);
    size_t size = stream.tellg();
    stream.seekg(0);   

    // Calc Padding size
    size_t padding_size = 0;
    if(align > 0)
        padding_size = align - size % align;

    // Load data
    data.resize(size + padding_size + 1);
    stream.read(&data.front(), static_cast<std::streamsize>(size));

    // Padding data
    for(size_t offst = 0; offst < padding_size; ++offst) 
    {
        *(&data.front() + size + offst) = padding_size;
    }

    //  And add terminating
    data[size] = 0;
}

/// Read data and add terminating 0
void fsutil::read_file_data_ex(const char *filename, std::vector<char>& data, size_t align)
{
    // Open stream
    std::ifstream stream(filename, std::ios::binary);
    if (!stream)
        throw std::runtime_error(std::string("cannot open file ") + filename);
    stream.unsetf(std::ios::skipws);

    // Determine stream size
    stream.seekg(0, std::ios::end);
    size_t size = stream.tellg();
    stream.seekg(0);   

    // Calc Padding size
    size_t padding_size = 0;
    if(align > 0) 
        padding_size = align - size % align;

    // Load data and add terminating 0
    data.resize(size + 1);
    stream.read(&data.front(), static_cast<std::streamsize>(size));

    // Padding data
    for(size_t offst = 0; offst < padding_size; ++offst) 
    {
        *(&data.front() + size + offst) = padding_size;
    }

    data[size] = 0;
}

void fsutil::read_file_data(const char *filename, std::vector<char>& output)
{
    FILE* fp = fopen(filename, "rb");
    if (fp == nullptr)
        return ;

    size_t size = get_file_size(filename);

    output.resize(size);
    size_t bytes_readed = fread(&output.front(), 1, size, fp);

    fclose(fp);
}

void  fsutil::read_file_data_as_string(const char *filename, std::vector<char>& output)
{
    FILE* fp = fopen(filename, "rb");
    if (fp == nullptr)
        return;

    size_t size = get_file_size(fp);

    output.resize(size + 1);
    size_t bytes_readed = fread(&output.front(), 1, size, fp);
	output[bytes_readed] = '\0';
    // output.resize(bytes_readed);

    fclose(fp);
}

void  fsutil::read_file_data_as_string(const char *filename, std::string& output)
{
    FILE* fp = fopen(filename, "rb");
    if (fp == nullptr)
        return;

    size_t size = get_file_size(filename);

    output.resize(size);
    size_t bytes_readed = fread(&output.front(), 1, size, fp);

    fclose(fp);
}

bool fsutil::write_file_data(const char* filename, const char* data, size_t size)
{
    FILE* fp = fopen(filename, "wb+");
    if(fp == nullptr)
        return false;
    fwrite(data, size, 1,  fp);
    fclose(fp);
    return true;
}

bool  fsutil::write_file_data(const char* filename, const std::string& content)
{
    return fsutil::write_file_data(filename, content.c_str(), content.size());
}

void  fsutil::append_file_data(const char* filename, const char* data, size_t size)
{
    FILE* fp = fopen(filename, "ab+");
    if (fp == nullptr)
        return;
    fwrite(data, size, 1, fp);
    fclose(fp);
}

void fsutil::append_file_data(const char* filename, const std::string& content)
{
    return fsutil::append_file_data(filename, content.c_str(), content.size());
}

bool fsutil::exists(const char* filename)
{
    return 0 == access(filename, 0);
}
#ifdef _WIN32
bool fsutil::is_dir_exists(const char* dir)
{
    return PathIsDirectoryA(dir) && fsutil::exists(dir);
}

bool fsutil::is_file_exists(const char* filename)
{
    return !PathIsDirectoryA(filename) && fsutil::exists(filename);
}
#endif

bool  fsutil::is_type_of(const std::string& filename, const char* type)
{
    size_t off = filename.find_last_of('.');
    if(off != filename.npos) // "abc.cc"
    {
        return 0 == memcmp(&filename.at(off), type, (std::min)(strlen(type), filename.length() - off));
    }
    return false;
}

template<typename _Elem, typename _Fty> inline
void split_breakif(const _Elem* s, const _Elem delim, _Fty op)
{
    const _Elem* _Start = s; // the start of every string
    const _Elem* _Ptr = s;   // source string iterator
    while (*_Ptr != '\0')
    {
        if (delim == *_Ptr/* && _Ptr != _Start*/)
        {
            if (_Ptr != _Start)
                if (op(_Start, _Ptr - _Start))
                    break; 
            _Start = _Ptr + 1;
        }
        ++_Ptr;
    }
    if (_Start != _Ptr) {
        op(_Start, _Ptr - _Start);
    }
}

template<typename _Elem, typename _Fty> inline
void split_breakif(const _Elem* s, const _Elem* delims, _Fty op)
{
    const _Elem* _Start = s; // the start of every string
    const _Elem* _Ptr = s;   // source string iterator
    size_t      _Lend = strlen(delims);
    while ((_Ptr = strstr(_Ptr, delims)) != nullptr)
    {
        if (_Ptr != _Start)
        {
            if (op(std::basic_string<_Elem>(_Start, _Ptr)))
                break;
        }
        _Start = _Ptr + _Lend;
        _Ptr += _Lend;
    }
    if (*_Start) {
        op(std::basic_string<_Elem>(_Start));
    }
}

bool  fsutil::is_type_of_v2(const std::string& filename, const char* type)
{
    static std::string alltype = "*.*";
    size_t off = filename.find_last_of('.');
    if(off != filename.npos)
    {
        std::string ext = "*" + filename.substr(off); // contains dot
        bool contains = false;
        split_breakif(type, '|', [&](const char* start, int len){
            return contains = (len == ext.length() && memcmp(ext.c_str(), start, len) == 0);
        });
        return type == alltype || contains;
    }
    else if(!filename.empty()){
        return type == alltype;
    }
    else {
        return false;
    }
}

void  fsutil::mkdir(const char* _Path)
{
	std::string dir = _Path;
	fsutil::mkdir(dir);
}

void fsutil::mkdir(std::string&& dir)
{
	fsutil::mkdir(dir);
}

void fsutil::mkdir(std::string& dir)
{
	nsc::dir_split(dir, [](const char* subdir) {
		bool should_brk = false;

		if (!fsutil::exists(subdir))
		{
#ifdef _WIN32
			should_brk = !(0 == ::mkdir(subdir));
#else
			should_brk = !(0 == ::mkdir(subdir, S_IRWXU | S_IRWXG | S_IRWXO));
#endif
		}

		return should_brk;
	});
}

std::pair<std::string, std::string> fsutil::split_fullpath(const std::string& fullpath)
{
    std::string tmp = fullpath;
    std::pair<std::string, std::string> pr;
    nsc::replace(tmp, "\\", std::string("/"));
    size_t pos = tmp.find_last_of("/");
    if(pos != std::string::npos) {
        try {
            pr.first = tmp.substr(0, pos);
            pr.second = tmp.substr(pos + 1);
        }
        catch(...) {
        }
    }
    return pr;
}

std::string fsutil::get_short_name(const std::string& complete_filename)
{
    size_t pos = complete_filename.find_last_of("\\");

    if(pos == std::string::npos)
        pos = complete_filename.find_last_of("/");

    if(pos != std::string::npos) {
        try {
            return complete_filename.substr(pos + 1);
        }
        catch(...) {
        }
    }
    return "";
}

std::string fsutil::get_path_of(const std::string& complete_filename)
{
    size_t pos = complete_filename.find_last_of("\\");

    if(pos == std::string::npos)
        pos = complete_filename.find_last_of("/");

    if(pos != std::string::npos) {
        try {
            return complete_filename.substr(0, pos);
        }
        catch(...) {
        }
    }
    return "";
}

const char* fsutil::get_extend(const char* source)
{
    int len = strlen(source);

    for (len = len - 1; len >= 0; --len)
    {
        if (source[len] == '.')
            return source + len;
    }

    return "";
}

const char* fsutil::get_extend(const std::string& filename)
{
    auto dot = filename.rfind('.');
    if (dot != std::string::npos)
    {
        return &filename.front() + dot;
    }
    return "";
}

std::string fsutil::get_type(const std::string& filename)
{
    static std::string _s_wildcard = "*";
    return _s_wildcard + fsutil::get_extend(filename);
}

static std::string::size_type rfind_op(const std::string& target, char ch1, char ch2)
{
    for (int p = static_cast<int>(target.size()) - 1; p >= 0; --p)
        if (target[p] == ch1 || target[p] == ch2)
            return static_cast<std::string::size_type>(p);
    return std::string::npos;
}

std::string fsutil::get_name(const std::string& filename)
{
    auto dot = filename.rfind('.');
    if (dot != std::string::npos)
    {
        auto slash = rfind_op(filename, '\\', '/');

        if (slash != std::string::npos)
            return filename.substr(slash + 1, dot - slash - 1);
        return filename.substr(0, dot);
    }
    return filename;
}

std::string fsutil::remove_extend(const std::string& filename)
{
    auto dot = filename.rfind('.');
    auto backslash = filename.rfind('\\');
    auto backslash2 = filename.rfind('/');
    if (dot != std::string::npos && (backslash < dot || backslash2 < dot) )
    {
        return filename.substr(0, dot);
    }
    return filename;
}

#if defined( _WIN32 ) && !defined(WP8)
#include <ShlObj.h>
bool fsutil::get_open_filename(TCHAR* output, const TCHAR* filters, TCHAR* history, HWND hwndParent)
{
    OPENFILENAME ofn;
    memset(&ofn,0,sizeof(OPENFILENAME));
    output[0] = '\0';
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.lpstrFilter = filters;
    ofn.lpstrFile = output;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrInitialDir = history;
    ofn.hwndOwner = hwndParent;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_ALLOWMULTISELECT;
    return GetOpenFileName(&ofn);
}

std::string fsutil::get_open_filename(TCHAR* dir, const TCHAR* filters, HWND hwndParent)
{
    TCHAR output[MAX_PATH + 1] = { 0 };

    OPENFILENAME ofn;
    memset(&ofn, 0, sizeof(OPENFILENAME));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.lpstrFilter = filters;
    ofn.lpstrFile = output;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrInitialDir = dir;
    ofn.hwndOwner = hwndParent;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_ALLOWMULTISELECT;
    if (GetOpenFileName(&ofn))
    {
        return nsc::transcode(output);
    }

    return "";
}

std::string fsutil::get_save_filename(TCHAR* dir, const TCHAR* filters, HWND hwndParent )
{
    TCHAR output[MAX_PATH + 1] = { 0 };

    OPENFILENAME ofn;
    memset(&ofn, 0, sizeof(OPENFILENAME));
    
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.lpstrFilter = filters;
    ofn.lpstrFile = output;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrInitialDir = dir;
    ofn.hwndOwner = hwndParent;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_ALLOWMULTISELECT;
    if (GetSaveFileName(&ofn))
    {
        return nsc::transcode(output);
    }

    return "";
}

TCHAR* fsutil::get_open_path(TCHAR* pBuffer, TCHAR* history, HWND   hWnd)
{  
    BROWSEINFO   bf;  
    LPITEMIDLIST   lpitem;  

    memset(&bf, 0, sizeof BROWSEINFO);  

    bf.hwndOwner = hWnd;  

    bf.lpszTitle = TEXT("Ñ¡ÔñÄ¿Â¼");  

    bf.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;

    bf.lParam = (LPARAM)history;

    bf.lpfn = _BrowseCallbackProc;

    lpitem = SHBrowseForFolder(&bf);  

    if(lpitem == NULL) 
        return   TEXT("");  

    SHGetPathFromIDList(lpitem, pBuffer);  

    CoTaskMemFree(lpitem);

    return    pBuffer;
}  

INT CALLBACK fsutil::_BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM pData) 
{
    TCHAR szDir[MAX_PATH];
    switch(uMsg) 
    {
    case BFFM_INITIALIZED: 
        // WParam is TRUE since you are passing a path.
        // It would be FALSE if you were passing a pidl.
        SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)pData);
        break;
    case BFFM_SELCHANGED: 
        // Set the status window to the currently selected path.
        if (SHGetPathFromIDList((LPITEMIDLIST)lParam ,szDir))
        {
            SendMessage(hwnd,BFFM_SETSTATUSTEXT,0,(LPARAM)szDir);
        }
        break;
    }
    return 0;
}

static bool is_support_type(const std::string& filename, const std::set<std::string>& filterset)
{
	auto t = fsutil::get_type(filename);
	bool bOK = filterset.find(t) != filterset.end();
	return bOK;
}

void fsutil::search_df(const std::string& root, const TCHAR* dir, std::vector<search_file_info>& filelist, bool recursive, const char* filter, unsigned int deep)
{
    //srcFiles = (char *)malloc(FILEEMAX * sizeof(char));
    WIN32_FIND_DATA fd;
    ZeroMemory(&fd, sizeof(WIN32_FIND_DATA));

    HANDLE hFile;
    TCHAR tmpPath[256];
    TCHAR subPath[256];
    ZeroMemory(tmpPath, 256);
    ZeroMemory(subPath, 256);

    BOOL bRet = TRUE;
    static int nFileSize = 0;
    bool ignoreFilter = (filter[0] == '\0') || ( (deep == 0) && strstr(filter, "*.*") != nullptr );

    //define the format of the basepath
    lstrcpy(tmpPath, dir);
    if (tmpPath[lstrlen(tmpPath) - 1] != '\\')
    {
        lstrcat(tmpPath, L"\\");
    }
    lstrcat(tmpPath, L"*");

    hFile = FindFirstFile(tmpPath, &fd);

    if (hFile != INVALID_HANDLE_VALUE) {
        do
        {
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
                !(fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) &&
                !(fd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
            {
                std::string tp = ::nsc::transcode(tmpPath);
                size_t pos = tp.find_first_of(root);

                std::string tree = tp.substr(pos + 1 + root.size());
                tree.pop_back();
                if (!tree.empty())
                    tree.pop_back();

                search_file_info sfi;
                sfi.filename = nsc::transcode(fd.cFileName);
                sfi.subtree = tree;

                // sfi.filename_without_ext = fsutil::get_name(sfi.filename);
                if (ignoreFilter)
                {
                    filelist.push_back(sfi);
                }
                else {
                    if (fsutil::is_type_of_v2(sfi.filename, filter))
                    {
                        filelist.push_back(sfi);
                    }
                }
            }
            else { // check whether directory
                if (recursive) {
                    if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && lstrcmp(fd.cFileName, L".") != 0 && lstrcmp(fd.cFileName, L"..") != 0)
                    {
                        lstrcpy(subPath, dir);
                        if (subPath[lstrlen(subPath) - 1] != '\\')
                        {
                            lstrcat(subPath, L"\\");
                        }
                        lstrcat(subPath, fd.cFileName);
                        if (ignoreFilter)
                            search_df(root, subPath, filelist, recursive, "", ++deep);
                        else
                            search_df(root, subPath, filelist, recursive, filter, ++deep);
                    }
                }
            }

            bRet = FindNextFile(hFile, &fd);

        } while (bRet);
    }

    FindClose(hFile);
}


void fsutil::search_df(const std::string& root, const TCHAR* dir, std::list<search_file_info, object_pool_allocator<search_file_info, 256>>& filelist, bool recursive, const char* filter)
{
    //srcFiles = (char *)malloc(FILEEMAX * sizeof(char));
    WIN32_FIND_DATA fd;
    ZeroMemory(&fd, sizeof(WIN32_FIND_DATA));

    HANDLE hFile;
    TCHAR tmpPath[256];
    TCHAR subPath[256];
    ZeroMemory(tmpPath, 256);
    ZeroMemory(subPath, 256);

    BOOL bRet = TRUE;
    static int nFileSize = 0;
    bool ignoreFilter = (strlen(filter) == 0) || strstr(filter, "*.*");

    //define the format of the basepath
    lstrcpy(tmpPath, dir);
    if (tmpPath[lstrlen(tmpPath) - 1] != '\\')
    {
        lstrcat(tmpPath, L"\\");
    }
    lstrcat(tmpPath, L"*");

    hFile = FindFirstFile(tmpPath, &fd);

    if (hFile != INVALID_HANDLE_VALUE) {
        do
        {
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
                !(fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) &&
                !(fd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
            {
                std::string tp = ::nsc::transcode(tmpPath);
                size_t pos = tp.find_first_of(root);

                std::string tree = tp.substr(pos + 1 + root.size());
                tree.pop_back();

                search_file_info sfi;
                sfi.filename = nsc::transcode(fd.cFileName);
                sfi.subtree = tree;

                // sfi.filename_without_ext = fsutil::get_name(sfi.filename);
                if (ignoreFilter)
                {
                    filelist.push_back(sfi);
                }
                else {
                    if (fsutil::is_type_of_v2(sfi.filename, filter))
                    {
                        filelist.push_back(sfi);
                    }
                }
            }
            else { // check whether directory
                if (recursive) {
                    if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && lstrcmp(fd.cFileName, L".") != 0 && lstrcmp(fd.cFileName, L"..") != 0)
                    {
                        lstrcpy(subPath, dir);
                        if (subPath[lstrlen(subPath) - 1] != '\\')
                        {
                            lstrcat(subPath, L"\\");
                        }
                        lstrcat(subPath, fd.cFileName);
                        search_df(root, subPath, filelist, recursive, filter);
                    }
                }
            }

            bRet = FindNextFile(hFile, &fd);

        } while (bRet);
    }

    FindClose(hFile);
}

std::string fsutil::get_current_directory(void)
{
    std::string temp(MAX_PATH, '\0');

    DWORD size = GetCurrentDirectoryA(temp.size(), &temp.front());
    temp.resize(size);
    return std::move(temp);
}

void fsutil::set_current_directory(const std::wstring& cd)
{
    SetCurrentDirectory(cd.c_str());
}

void fsutil::set_current_directory(const std::string& cd)
{
    SetCurrentDirectoryA(cd.c_str());
}

std::string  fsutil::get_module_directory(void)
{
    std::string s(MAX_PATH, '\0');

    DWORD n = GetModuleFileNameA(NULL, &s.front(), MAX_PATH);
    s.resize(n);

    for (; s.size() > 0 ;)
    {
        if (s.back() != '\\')
            s.pop_back();
        else
            break;
    }

    if (!s.empty() && s.back() == '\\')
    {
        s.pop_back();
    }

    return std::move(s);
}

std::string fsutil::to_absolute(const std::string& path)
{
    if (is_absolute(path))
    {
        return path;
    }

    wchar_t buffer[256];
    GetCurrentDirectory(256, buffer);

    std::string cd = nsc::transcode(buffer);

    return cd + "\\" + path;
}

bool fsutil::is_absolute(const std::string& path)
{
    return path.find(':') != std::string::npos;
}

std::string  fsutil::to_relative(const std::string& target, bool dir)
{
    auto module_dir = fsutil::get_module_directory();
    return fsutil::to_relative(module_dir, true, target, dir);
}

std::string fsutil::to_relative(const std::string& from, bool fromIsDir, const std::string& to, bool toIsDir)
{
    std::string relative(MAX_PATH, '\0');

    if (PathRelativePathToA(&relative.front(), from.c_str(),
        fromIsDir ? FILE_ATTRIBUTE_DIRECTORY : 0,
        to.c_str(),
        toIsDir ? FILE_ATTRIBUTE_DIRECTORY : 0))
    {
        relative.resize(std::strlen(relative.c_str()));
    }
    else {
        relative = to;
    }

    return std::move(relative);
}

#endif


