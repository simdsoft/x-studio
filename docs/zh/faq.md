# x-studio - 常见问题

!!! question "在win11运行卡顿怎么办？"

    1. 请使用win11系统设置里回滚至系统win10
    2. 笔者曾经将自己的至尊配置i9-10980XE电脑和公司电脑升级至win11，在感受包括系统各项操作的极端卡顿后，于2021.11.23将公司电脑回滚至win10，终于丝滑
    3. 等win11迭代更多版本，观望稳定后，在考虑升级吧

!!! question "登录失败返回-26错误码怎么办?"

    请升级至x-studio 2022.1.0(build 9900.0)

!!! question "打开文件乱码怎么办?"

    1. 出现乱码原因，如果不是文件本身编码问题，就是软件检测编码失败了，属于正常现象，例如字符串 "fdsfdsfsd 你好吗啊啊啊" 如果保存为GBK，vscode打开自动编码检测也无法正确识别编码，详见: https://github.com/simdsoft/x-studio/issues/798
    2. 解决方案: 通过软件主菜单【文件】【以指定编码保存/重新加载】，选择编码，加载即可。
    3. x-studio将努力保证大多数代码文件编码识别的正确性，如发现不能识别，如果方便，请将无法识别的编码文件发给我们。
    
!!! question "启动x-studio报0x000000c1怎们办?"

    x-studio目前是32位软件，请务必确保安装[vc2019_x86 redist](https://x-studio.net/fdl2.php?file=VC_redist.x86.exe)运行库后再重试

!!! question "启动x-studio报0x0000007e错误怎么办?"
	
    1. 前往 https://dl.x-studio.net 下载最新微软vs2019运行库并安装
    2. 检查dx环境，百度搜索 **dx修复工具** 修复下

!!! question "启动x-studio启动报998错误码怎么办？"

    卸载软件重装。

!!! question "使用x-studio启动调试，调试断点窜行或者不命中怎么办?"

    这种情况99.9999%是由于调试过程中修改代码造成的，不必重启软件，按 **CTRL+W** 关闭代码文档，重新打开即可

!!! question "使用x-studio启动调试，调试目标程序闪退怎么办?"

    删除exe同目录下.dcache文件，再试试。

!!! question "x-studio csb导入如果出现拷贝资源失败怎么办?"

    如果导入过程中出现拷贝资源失败，首先确定原资源是否在RAM磁盘， 如果是，则先将原始资源拷贝到普通磁盘，再尝试导入，如果还是失败，请加通过如下途径联系我们解决
  
    1. [点击](https://jq.qq.com/?_wv=1027&k=5rgoVdQ)加入官方QQ群联系群主
    2. 发送问题到 [support@simdsoft.com](mailto:support@simdsoft.com)
