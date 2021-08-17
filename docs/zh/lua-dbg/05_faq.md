# 调试常见问题

!!! Unity或Unreal Engine调试断点不命中怎么办？

    在输出窗口搜索 ``x-studio Debug System:`` 查看关于lua代码文件路径的打印，检查是否是相对于工程路径，理论上，只要是相对于工程的路径，都可以命中断点，否则就需要手动修改 ``xxx.xsxproj`` 文件，在 ``debug-options`` 节点下添加代码搜索路径, 如下配置表示将工程 ``Assets\MyLuaFolder`` 添加为搜索路径(其实就类似VS C++调试器里的sourceSearchPaths):
    <search-paths>
      <item>Assets\MyLuaFolder</item>
    </search-paths>

!!! 如何让调试器在指定dll中搜索Lua虚拟机核心API?

    通过添加 ``debug-options/lvm-dll-pattern`` 设置正则表达式搜索即可，默认值: ``((\b(lib)?lua5[1-9])|plainlua|xlua|slua|tolua|(UE4Editor\-(slua_unreal|UnLua)[\-A-Za-z0-9]*))\.dll`` ， 正则表达式语法可以参考: [C++11 ECMAScript](http://www.cplusplus.com/reference/regex/ECMAScript/)

!!! question "如何传递将设备尺寸参数传递给被调试程序?"

    在游戏开发过程中，通常都有设计尺寸和设备尺寸以及朝向，那么，使用x-studio启动调试时
    可将这些参数传递给被调试程序，首先在.xsxproj工程文件中添加 ``debug-options`` 节点下添加 ``passing-view-args``
    (10.0.5900.199及之前版本，请添加 ``pass-size-args``) 元素, 值为1，那么调试器会传递如下格式参数:
    ``--design-size=720x1280 --device-size=640x960 --orientation=landscape or --orientation=portrait``

    ```c++
      /* 解析C++代码参考 */
      namespace xscmdl
      {
        static inline bool check_arg(const char* arg, const char* name) { return 0 == stricmp(arg, name); }
        static inline bool check_arg(const wchar_t* arg, const wchar_t* name) { return 0 == wcsicmp(arg, name); }
        
        template <size_t _Size>
        static inline bool check_arg(const char* arg, const char (&name)[_Size], size_t& n)
        {
          n = _Size - 1;
          return 0 == strnicmp(arg, name, n);
        }
        template <size_t _Size>
        static inline bool check_arg(const wchar_t* arg, const wchar_t (&name)[_Size], size_t& n)
        {
          n = _Size - 1;
          return 0 == wcsnicmp(arg, name, n);
        }
      }

      int main(int argc, char** argv) {
        std::string_view value;
        size_t len = 0;
        int w = 0, h = 0;
        for (int i = 0; i < argc; ++i) {
          if (xscmdl::check_arg(argv[i], "--device-size=", len)) {
            value = argv[i] + len;
            char* mulp = nullptr; // the 'X' pos
            w = strtol(value.data(), &mulp, 10);
            if(mulp)
              h = strtol(mulp + 1, nullptr, 10);
          }
        }
      }
    ```

!!! question "断点混乱怎么办？"

    我们都知道，有不少调试器在调试过程中默认是禁用编辑的，例如VS C#等，为方便代码编写，
    x-studio是允许修改代码的，多数情况下，会自动映射断点行；但少数情况会映射失败，
    此时关闭文档（Ctrl+W），重新打开即可，再次启动调试，就可以解决混乱了。

!!! question "断点莫名其妙无法命中怎么办？"

    检查是否是大小写问题，由于Windows是不区分文件大小写的，因此即使代码中require路径大小写
    和实际磁盘不一致，依然可以成功加载脚本，但此时，不一致的代码文件断点是无法命中的。

!!! question "为什么在文档中查看代码已经运行，但却没有实际输出？"

    检查开发目录和exe所在目录是否都有相同文件名代码，cocos引擎默认会优先加载exe所在目录下的代码，
    开发过程中，始终保持工程根目录下的src目录一份代码，并删除exe所在目录下的代码是一个良好的开发习惯，这是
    早期cocos lua的设计缺陷，应该摒弃。
