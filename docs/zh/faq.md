# x-studio - 常见问题

!!! question "在win11运行滚动条卡顿怎么办？"

    请升级至x-studio 2022.1.0+

!!! question "总是登录失败返回-26错误码怎么办?"

    请升级至x-studio 2022.1.0+

!!! question "打开文件乱码怎么办?"

    1. 出现乱码原因，如果不是文件本身编码问题，就是软件检测编码失败了，属于正常现象，例如字符串 "fdsfdsfsd 你好吗啊啊啊" 如果保存为GBK，vscode打开自动编码检测也无法正确识别编码，详见: https://github.com/simdsoft/x-studio/issues/798
    2. 解决方案: 通过软件主菜单【文件】【以指定编码保存/重新加载】，选择编码，加载即可。
    3. x-studio将努力保证大多数代码文件编码识别的正确性，如发现不能识别，如果方便，请将无法识别的编码文件发给我们。

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
