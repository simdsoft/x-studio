#  此任务需要额外的权限

此错误，则会显示 x-studio 命令以帐户没有足够的权限执行操作的用户调用。

必须运行某些 x-studio 命令与具有足够用户访问权限启用所需的文件和注册表项的命令读写所有的帐户。  若要获得这些权限，您必须关闭再重新打开具有提升访问权限的帐户的 x-studio，例如控制器。  

有关权限的更多信息，也可以参阅 [Windows用户帐户控制](https://docs.microsoft.com/zh-cn/windows/security/identity-protection/user-account-control/user-account-control-overview)

## 更正此错误

在对话框中，单击**“使用另一个帐户重新启动”**。  
  
这将提示您保存当前加载的解决方案，然后关闭 Visual Studio 再重新启动，提示您切换到另一个帐户。  
  
出现登录提示时，请使用比前面的帐户具有更高权限的帐户（如管理员帐户）进行登录。  
  
在 Visual Studio 启动时，它重新加载最后一个解决方案和命令行。  
  
!!! Note "备注"

    登录使用具有更高权限的帐户不一定确保 Visual Studio 命令的工作方式。  某些命令可能未成功运行，即使使用了管理员帐户。  
