class EmbeddedConsole
{
public:
  struct ColorIndex final
  {
    enum
    {
      BLACK,        // 黑色
      BLUE,         // 蓝色
      GREEN,        // 绿色
      AQUA,         // 浅绿色/水绿色
      RED,          // 红色
      PURPLE,       // 紫色
      YELLOW,       // 黄色
      WHITE,        // 白色
      GRAY,         // 灰色
      LIGHT_BLUE,   // 淡蓝色
      LIGHT_GREEN,  // 淡绿色
      LIGHT_AQUA,   // 淡浅绿色/淡水绿色
      LIGHT_RED,    // 淡红色
      LIGHT_PURPLE, // 淡紫色
      LIGHT_YELLOW, // 淡黄色
      BRIGHT_WHITE, // 亮白色
    };
  };

  struct TextColorMask final
  {
    enum
    {
      LIGHT_GREEN  = FOREGROUND_INTENSITY | FOREGROUND_GREEN,                  // 淡绿色
      LIGHT_RED    = FOREGROUND_INTENSITY | FOREGROUND_RED,                    // 淡红色
      LIGHT_YELLOW = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, // 淡黄色
      BRIGHT_WHITE = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
    };
  };

public:
  EmbeddedConsole() : _hConsole(nullptr) {}

  void create()
  {
    this->_bAttachToParent = AttachConsole(-1);
    if (!this->_bAttachToParent)
      AllocConsole();
    else if (!GetConsoleTitleA(this->_title, sizeof(this->_title)))
    {
      memcpy(this->_title, R"(C:\WINDOWS\system32\cmd.exe)", 28);
    }
    SetConsoleTitleA("x-studio EncryptSystem - " VERSION_STRING);
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
    this->_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    setupColorSchemes();
  }

  void destroy()
  {
    if (this->_hConsole != nullptr)
    {
      if (this->_bAttachToParent)
      {
        // SetConsoleScreenBufferInfoEx(this->_hConsole, &this->_info);
        SetConsoleTextAttribute(this->_hConsole, this->_info.wAttributes);
        SetConsoleTitleA(this->_title);
      }

      this->_hConsole = nullptr;
      fclose(stderr);
      fclose(stdout);
      fclose(stdin);
      FreeConsole();
    }
  }

  void setupColorSchemes()
  {
    CONSOLE_SCREEN_BUFFER_INFOEX info;
    info.cbSize = sizeof(info);

    GetConsoleScreenBufferInfoEx(this->_hConsole, &info);
    memcpy(&this->_info, &info, sizeof(info));

#if 0
        info.ColorTable[ColorIndex::LIGHT_GREEN] = RGB(0, 255, 0); // info
        info.ColorTable[ColorIndex::LIGHT_YELLOW] = RGB(255, 255, 0); // warn
        info.ColorTable[ColorIndex::LIGHT_RED] = RGB(255, 0, 0); // error
        SetConsoleScreenBufferInfoEx(this->_hConsole, &info);
#endif
  }

  void setTextColor(int textColorMask) { SetConsoleTextAttribute(this->_hConsole, textColorMask); }

  HANDLE getHandle() const { return this->_hConsole; }

  BOOL isAttachToParent() const { return this->_bAttachToParent; }

private:
  HANDLE _hConsole;
  char _title[4096];
  CONSOLE_SCREEN_BUFFER_INFOEX _info = {0};
  BOOL _bAttachToParent              = FALSE;
};
