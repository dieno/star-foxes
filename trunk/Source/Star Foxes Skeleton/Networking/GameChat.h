#include <cstdlib>
#include <string>
#include <list>
#include "Text.h"

using namespace std;

class GameChat
{
private:
   bool _writingOn;
   char _owner[128];
   string _msg;   
   list<string> _history;
   int _histlimit;
   Text* _msgtext;   

public:
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
   void AddMsgToHistory(string msg);
   bool IsWritingOn();
   string GetCurrentMsg();
   const char* GetCurrentMsgC();
   list<string>* GetHistory();
   GameChat(void);
   ~GameChat(void);
};
