# x-studio - 常见问题

!!! question "启动x-studio报0x000000c1怎们办?"

    x-studio和VisualStudio一样是32位软件，请务必确保安装[vc2019_x86 redist](https://x-studio.net/fdl2.php?file=VC_redist.x86.exe)运行库后再重试

!!! question "启动x-studio报0x0000007e错误怎么办?"
	
    1. 前往 https://dl.x-studio.net 下载最新微软vs2019运行库并安装
    2. 检查dx环境，百度搜索 **dx修复工具** 修复下

!!! question "启动x-studio启动报998错误码怎么办？"

    卸载软件重装。

!!! question "使用x-studio启动调试，调试断点窜行或者不命中怎么办?"

    这种情况99.9999%是由于调试过程中修改代码造成的，不必重启软件，按 **CTRL+W** 关闭代码文档，重新打开即可

!!! question "如何使用最新版x-studio编译luajit arm64位字节码?"

    1. 首先下载最新版x-studio并安装
    2. 打开命令行输入 ``pushd "%XS_INSTDIR%\tools\lj-compiler\2.1"`` 并回车
    3. 使用命令编译字节码, 例如:

        - 编译arm64为字节码(iPhone5S及以上设备):
          ``lj-compile D:\dev\projects\LuaDemo\src D:\dev\projects\LuaDemo\jit-arm64 arm64``
        - 编译32为通用字节码(win32 iPhone5和所有Android设备):
          ``lj-compile D:\dev\projects\LuaDemo\src D:\dev\projects\LuaDemo\jit-cpu32 x86``

!!! question "使用x-studio启动调试，调试目标程序闪退怎么办?"

    删除exe同目录下.dcache文件，再试试。

!!! question "x-studio csb导入如果出现拷贝资源失败怎么办?"

    如果导入过程中出现拷贝资源失败，首先确定原资源是否在RAM磁盘， 如果是，则先将原始资源拷贝到普通磁盘，再尝试导入，如果还是失败，请加通过如下途径联系我们解决
  
    1. [点击](https://jq.qq.com/?_wv=1027&k=5rgoVdQ)加入官方QQ群联系群主
    2. 发送问题到 [support@simdsoft.com](mailto:support@simdsoft.com)
