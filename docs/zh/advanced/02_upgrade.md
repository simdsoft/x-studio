# 软件升级

## 外网升级

联网状态下， 软件会自动检查更新，并下载升级，最新版本下载地址: https://x-studio.simdsoft.com/dl.php?host=local, 版本信息查询: https://x-studio.simdsoft.com/query_latest.php

## 内网升级

在局域网内网环境中，可通将任意一台机器上的x-studio作为更新服务器, 配置方法如下:

* 在注册表项: `HKEY_CURRENT_USER\Software\Simdsoft\x-studio\StableSettings` 新建DWORD项 `LANUpgradeRole` 并设置为1
* 编辑内容 `ftp://x-studio:awesomeide@$(HOST)/;软件安装程序exe;软件安装包大小;软件安装包MD5值` 保存至文件: `C:\Users\%USERNAME%\AppData\Local\x-studio\tmp\version.txt` , 同时，新版本安装包也拷贝至该目录即可。
