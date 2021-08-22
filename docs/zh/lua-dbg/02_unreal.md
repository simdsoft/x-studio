# 虚幻引擎 Lua调试

!!! attention "注意"
    - 本教程自V2021.4.2版本发布后可用
    - 调试虚幻引擎Lua项目，编译模式应该是 ``Development Editor`` 或 ``DebugGame Editor``，且默认支持为腾讯的 ``UnLua`` 和 ``sluaunreal``，若要支持其他Lua解决方案，修改 ``xxx.xsxproj`` 工程文件(xml格式) ``debug-options/lvm-dll-pattern`` 正则表达式匹配相应插件dll名称即可。

## 基本步骤

### 1步创建调试工程

  将虚幻引擎游戏项目目录拖入编辑器即可，如图所示：
  TODO: 

### 开始调试

  按F9打断点，软件会 **自动识别虚幻引擎游戏项目**,
  因此按F5会直接附加到UE4或者UE5的Editor进程，虚幻引擎编辑器中启动预览游戏，命中断点后可查看调用堆栈，鼠标悬浮变量监视，
  或者将变量拖入变量监视窗口，也可以按Shift+F9打开快速监视对话框，如图所示：

  TODO: 

!!! attention "注意事项"

    * 目前附加到虚幻引擎Editor进程后，退出软件会结束虚幻引擎Editor进程，请谨慎操作，后续软件更新会专注优化这个体验。