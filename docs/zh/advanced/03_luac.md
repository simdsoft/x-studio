## Lua字节码编译命令行

### 最低x-studio支持版本

*x-studio Pro 2021*

### 基本用法

`"%XS_INSTDIR%\tools\lua-compile.bat" IN_DIR OUT_DIR KEEP_EXT BC_TYPE TARGET_CPU`

### 参数说明

*IN_DIR*: 输入目录<br/>
*OUT_DIR*: 输出目录<br/>
*KEEP_EXT*: 输出文件保持`.lua`后缀，否则字节码文件后缀为`.luac`<br/>
*BC_TYPE*: 字节码类型， 取值`plain`(官方lua),`jit`(LuaJIT) <br/>
*TARGET_CPU*: 目标字节码CPU架构，可取值: `x86`或`arm64`， 仅字节码类型为`jit`时有效<br/>

### 其他说明

- 官方lua字节码格式为基于lua-5.3.6改造后的32/64位通用字节码 <br/>
- jit类型字节码位luajit-2.1生成的字节码，需要指定CPU平台，字节码不通用
