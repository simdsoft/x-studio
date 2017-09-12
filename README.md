# x-studio365
x-studio365 is a more powerful,lightweight and fast game develop IDE. It support Particle editing, Scene(UI) editing & Screen Adapting
preview, Lua Debug & Edit & Syntax Highlighting & Outlining & Auto-completation.
Download the latest version: http://en-us.x-studio365.com
 

# Editor major features:
1. Using cocos2d-x to render the main editor UI directly.
2. Supports more than 20 1K-layout methods.
3. Supports ParticleSystem Editing, import & export plist file.
4. Supports view real adpating vision for any-size of devices.
5. Supports import CocosStudio ccs project, and json format ui published by CocosStudio.
6. Supports import .csb, and publish designed ui to cocosstudio(ccs), json, csd formats.
7. Integrated AES batch encrypt/decrypt tool which work on CBC mode, and support MD5 checksum.
8. Supports Chinese and English.
9. Supports SpriteSheet creation.

# Code editing features:
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

# Editor shortcuts
|Shortcut         | Description     |
| ------------- |:----------------:|
|Shift+Alt+Enter	|FullScreen|
|Alt+ MouseWheel	|Zoom In/Out Node|
|Ctrl+Alt+MouseWhell	|Zoom In/Out Scene|
|Alt + MouseMove	|Move Node|
|Ctrl + Alt + MouseMove	|Move Scene|
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
|Alt + |MouseMove block select text|
|Ctrl + Shift + F	|Find All in files|
|Ctrl + D	|Select Next occurency text|
|Ctrl + E	|copy selected text and paste at end of current line|
|Ctrl + I	|Correct Lua Indent.|
|Ctrl + P	|Quick search file in project|
|Ctrl + R	|Quick search Lua function in current document.|
|Ctrl + Tab	|Quick switch opened document in stack|
|Ctrl + F2	|Change All Occurences of current document|
|Alt+→	|Display auto-completion window|
|Ctrl+Alt+Num9	|Choose Code Theme(Current support 5 themes)|

# How to use this editor with cocos2d-x
1. Download x-studio365 specific engine.  
github.com:  
engine: https://github.com/halx99/cocos2d-x  
3rd libs: https://github.com/halx99/cocos2d-x-external  
  
git.oschina.net:  
engine: http://git.oschina.net/halx99/cocos2d-x   
3rd libs: http://git.oschina.net/halx99/cocos2d-x-3rd-party-libs-bin  
  
clone the engine and 3rd, then copy or move all 3rd libs files to engine_root/external folder.  
currently, the build & run system have tested on win32&android, iOS have not test yet.  
  
2.Setup for new empty project:  
There are two scripts:  
https://github.com/halx99/cocos2d-x/blob/v3/copy_missing_files_cpp.bat for empty-cpp-project.  
Anothor script copy_missing_files_lua.bat for empty-lua-project.  
  
Config the system or user envirament COCOS2DX_ROOT to the path of engine_root, then copy the script file to the new empty project's root directory, then execute it, it will copy missing souce file automatically.  

3.remark: VS2015 is required.
