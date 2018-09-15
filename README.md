# x-studio365 2014 - 2018
x-studio365 is a powerful,lightweight and fast development IDE. It support multi-programing languages Source Code Editing & Syntax Highlighting & Outlining & Auto-completation, Great Lua Debugging. In the future, it will support other languages debugging, such is PHP. It support Cocos2d-x Particle editing, Scene(UI) editing & Screen Adapting preview, CocosStudio .csb recovery.
Download the latest version: http://en-us.x-studio365.com  http://cn.x-studio365.com
 
# Great Text/Code editing features:
* Suport Auto detect UTF-8 encoding, and support mutli-encodings convertion.
* Suport vscode/sublime Open Folder… feature, and support multi-program-languages syntax highlight.
* Suport 5 code themes, Hotkey: Ctrl+Alt+NUM9
* Support search project file quickly with Hotkey: Ctrl+P
* Support search Lua functions of current document with Hotkey: Ctrl R
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
* Support Lua Syntax check.
* Support Comment lua source code with hotkey: Ctrl+/
* Support View file as binary Ascii-View.
* Support CTRL+RightClick to show shell context menu at 'Project Explorer'
* Support Open/Save Large file (more than 100M) fastly.
* Support capture adb log to Output View with filter feature.

#Code Editing Preview
![image](https://github.com/halx99/x-studio365/blob/master/showcase2.png)  

# Cocos2d-x scene editing features:
1. Using cocos2d-x to render the main editor UI directly.
2. Supports more than 20 1K-layout methods.
3. Supports ParticleSystem Editing, import & export plist file.
4. Supports view real adpating vision for any-size of devices.
5. Supports import CocosStudio ccs project, and json format ui published by CocosStudio.
6. Supports import .csb, and publish designed ui to cocosstudio(ccs), json, csd formats.
7. Integrated AES batch encrypt/decrypt tool which work on CBC mode, and support MD5 checksum.
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

# How to use this editor with cocos2d-x
1. Create a empty project by official cocos2d-x engine console command, such as x365app1
2. Overrite 'editor-support' & 'external/flatbuffers'
3. (Win32) Exclude 'cocostudio',GameNode3DReader.cpp,Light3DReader.cpp from libcocos2d project explorer.  
4. (Win32) Add 'editor-support/cocostudio/proj.win32/libxreader.vcxproj' and 'editor-support/dragonBones/proj.win32/libDragonBones.vcxproj' to solution  
5. (Win32) Add libxreader project as reference of x365app1 project

showcase  
![image](https://github.com/halx99/x-studio365/blob/master/showcase.png)

### Purchase Professional License
 
 If you are interest in this software, please purchase.
 
 <a href='http://cn.x-studio365.com/buy.php?buy_type=1001&lang=437'>Purchase Professional License</a>
