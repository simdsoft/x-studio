## Lua字节码编译命令行

### 最低x-studio支持版本

*x-studio 2022.1.0*

### 基本用法

`"%XS_INSTDIR%\tools\lua-compile.bat" IN_DIR OUT_DIR STRIP KEEP_EXT BC_TYPE NO_GC64`

### 参数说明

*IN_DIR*: 输入目录<br/>
*OUT_DIR*: 输出目录<br/>
*STRIP*: 是否去除调试符号, 默认去除, false:保留调试符号
*KEEP_EXT*: 输出文件保持`.lua`后缀，否则字节码文件后缀为`.luac`<br/>
*BC_TYPE*: 字节码类型， 取值`plain`(官方lua),`jit`(LuaJIT) <br/>
*NO_GC64*: 可选，默认值`false`，仅BC_TYPE为`jit`时有效

### 其他说明

- 官方lua字节码格式为lua-5.4.4通用字节码 <br/>
- jit类型字节码为luajit-2.1生成, `NO_GC64`为`true`时，32位程序可加载运行，否则所有平台64位(包括x64,ARM64)程序可加载运行
- 关于LuaJIT GC64模式：LuaJIT垃圾回收机制在64位程序中突破2GB的限制
