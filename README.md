# x-studio 2014 - 2021

[简体中文](https://github.com/simdsoft/x-studio/blob/master/README_CN.md)

[![Release](https://img.shields.io/badge/x﹣studio%20Pro%20-10.0.9000.28-blue.svg)](https://github.com/simdsoft/x-studio)

### Since v10.0.5900.299, the software name will be rename from ```x-studio365``` to ```x-studio```

### x-studio self development tools is: ```Microsoft Visual Studio Community 2019```

x-studio is a powerful,lightweight and fast development IDE. It support multi-programing languages Source Code Editing & Syntax Highlighting & Outlining & Auto-completation, Great Lua Debugging. In the future, it will support other languages debugging, such is PHP. It support Cocos2d-x Particle editing, Scene(UI) editing & Screen Adapting preview, CocosStudio .csb recovery.  
+ Official website(zh_CN): https://x-studio.net/  
+ Official documentation(zh_CN): https://docs.x-studio.net/  
+ Official website(en): https://en.x-studio.net/  (building)
+ Revisions download: https://dl.x-studio.net/  

# Software Framework
![image](https://github.com/simdsoft/x-studio/blob/master/showcase5.png)  
 
# Great Code editing features:
* Support Auto detect UTF-8 encoding, and support mutli-encodings convertion.
* Support vscode/sublime Open Folder… feature, and support multi-program-languages syntax highlight.
* Support 5 code themes, Hotkey: Ctrl+Alt+NUM9
* Support search project file quickly with Hotkey: Ctrl + P
* Support search Lua functions of current document with Hotkey: Ctrl + R
* Support find all at current document with hotkey: Ctrl + F
* Support replace all at current document with hotkey: Ctrl + H
* Support find content at all project files with hotkey: CtrlShift+F
* Support find content at any directory, also can find any folder of project at project view, just right click to open context menu, then click [Find In Files] menu-item
* Support Visual Studio like Ctrl+Tab feature.
* Support Navigate Backward/Forward with hotkey Ctrl+-/Ctrl+Shift+-
* Support Change all occurrences with hotkey: Ctrl+F2
* Support multi-selection, use Ctrl+D to select next occurrence.
* Support block selection with Alt key pressed, and move mouse.
* Support Current document auto-completation
* Support format current Lua document with hotkey: Ctrl+I
* Support Lua Syntax check: Ctrl+B
* Support Comment lua source code with hotkey: Ctrl+/
* Support View file as binary Ascii-View.
* Support CTRL+RightClick to show shell context menu at 'Project Explorer'
* Support Open/Save Large file (more than 100M~1GB) at a few of seconds, when I try to open a 700MB file with vscode, just got failed.
* Support capture adb log to Output View with filter feature.
* High-performance Output view.
* Integrated with clang-fotmat 11.0, so format cpp, java, objc, js is very easy.
* Chould config a custom tool for android logcat to redirect to OutputView.
* Shell integrated support to calculate digest with usually algorithms, shuch as md5,sha1,sha512 and etc.

#Code Editing Preview
![image](https://github.com/simdsoft/x-studio/blob/master/showcase2.png)  
![image](https://github.com/simdsoft/x-studio/blob/master/showcase3.png)  
![image](https://github.com/simdsoft/x-studio/blob/master/showcase4.png)  

# Cocos2d-x scene editing features:
1. Using cocos2d-x to render the main editor UI directly.
2. Supports more than 20 1K-layout methods.
3. Supports ParticleSystem Editing, import & export plist file.
4. Supports view real adpating vision for any-size of devices.
5. Supports import CocosStudio ccs project, and json format ui published by CocosStudio.
6. Supports import .csb, and publish designed ui to cocosstudio(ccs), json, csd formats.
7. Provide command line common assets(.png,.lua,.json and any other files) encrypt solution.(since 10.0.3300.1001)
   usage: "%XS_INSTDIR%\x-studio.exe" -c -enc -i=d:\projects\project0\res -o=d:\projects\project0\res-enc
    will encrypt all files with aes256 cbc and compress with zlib, the enctypt config will be stored to C:\Users\%USERNAME%\AppData\Local\x-studio\encrypt-cfg.xml whitch contains encrypt key & ivec, it's reqiured by the runtime: https://github.com/simdsoft/x-studio/tree/master/encrypt-demo/cpp-empty-test/Classes
8. Supports Chinese and English.
9. Supports SpriteSheet creation.


# Editor shortcuts
|Shortcut         | Description     |
| ------------- |:----------------:|
|Shift+Alt+Enter	|FullScreen|
|Alt+ MouseWheel	|Zoom In/Out Node|
|Ctrl+Alt+MouseWhell	|Zoom In/Out Scene|
|Alt + MouseMove	|Move Node|
|Ctrl + Alt + MouseMove / Alt + MouseMiddleButton	|Move Scene|
|↑↓←→	|Adjust postion 1px, continue press: 2px|
|Press Shift, |Change Node Size without width/height factor change.|
|Press Ctrl, |Select Node Additional|
|F5 |Start Debugging|
|F10	|Step Over|
|Ctrl+F10	|Run To Cursor|
|F11	Step |Into|
|Shift+F5	|Stop Debugging|
|Ctrl+Shift+F5 |Restart Debugging|
|Ctrl+F5	|Start without debugging|
|Ctrl+F	|Open Find Dialog|
|F3 |Find Next|
|Shift+F3	|Find Prev|
|Ctrl+Shift+U	|Convert selected text to uppercase |
|Ctrl+U	|Convert selected text to lowercase|
|Alt + MouseMove| block select text|
|Ctrl + Shift + F	|Find All in files|
|Ctrl + D	|Select Next occurency text|
|Ctrl + E	|copy selected text and paste at end of current line|
|Ctrl + I	|Correct Lua Indent.|
|Ctrl + P	|Quick search file in project|
|Ctrl + R	|Quick search Lua function in current document.|
|Ctrl + Tab	|Quick switch opened document in stack|
|Ctrl + Q |Quick switch active x-studio365 process instances(project is open)|
|Ctrl + F2	|Change All Occurences of current document|
|Alt+→	|Display auto-completion window|
|Ctrl+Alt+Num9	|Choose Code Theme(Current support 5 themes)|
|Ctrl + Shift + D|temp copy&paste without clipboard occupation|
|Ctrl + RightClick Project Explorer| Display desktop shell context menu|
|Shift + F9|Quick Watch|

showcase  
![image](https://github.com/simdsoft/x-studio/blob/master/showcase.png)  
### Purchase Professional License
 
 If you are interest in this software, please purchase.
 
 <a href='http://x-studio.net/buy.php?buy_type=1001&lang=437'>Purchase Professional License</a>
