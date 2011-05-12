#include "../directXClass.h"

/*#include <cstdlib>
#include <std::string>
#include <std::list>*/

#ifndef GAMECHAT
#define GAMECHAT

class GameChat
{
private:
   bool _writingOn;
   char _owner[128];
   std::string _msg;   
   std::list<std::string> _history;
   int _histlimit;
   Text* _msgtext;   

public:
   void RenderChat();
   Text* GetTextFormat();
   void SetTextFormat(Text *text);
   void StartWrite();
   void EndWrite();
   void Write(char a);
   void ClearHistory();
   void ClearCurrentMsg();
   void SendMsg();
   void Draw();
   void DrawInputField();
   void DrawHistory();
   void DeleteLastChar();
   void AddMsgToHistory(std::string msg);
   bool IsWritingOn();
   std::string GetCurrentMsg();
   const char* GetCurrentMsgC();
   std::list<std::string>* GetHistory();
   GameChat(void);
   ~GameChat(void);
};

#endif