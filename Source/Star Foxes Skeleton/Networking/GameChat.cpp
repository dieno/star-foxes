#include "GameChat.h"

void GameChat::DrawHistory()
{
}

void GameChat::DrawInputField()
{
}

void GameChat::Draw()
{
}

void GameChat::RenderChat()
{
   if(IsWritingOn())
   {
      string msg = "= ";
      msg.append(GetCurrentMsg());
      GetTextFormat()->draw(msg.c_str());
   }

   list<string>* hist = GetHistory();
   int x = 10;
   int y = 100;
   RECT* rct = new RECT;
   rct->top = y;
   rct->left = x;
   rct->bottom = 50;
   rct->right = 200;

   for (list<string>::iterator ci = hist->begin(); ci != hist->end(); ++ci)
   {
      GetTextFormat()->draw(ci->c_str(), rct);
      rct->top += 12;
      rct->bottom += 12;
   }
}

void GameChat::SetTextFormat(Text* text)
{
   _msgtext = text;
}

Text* GameChat::GetTextFormat()
{
   return _msgtext;
}

void GameChat::AddMsgToHistory(string msg)
{
   if(_history.size() >= _histlimit)   
      _history.pop_front();

   _history.push_back(msg);
}

void GameChat::DeleteLastChar()
{
   if(_msg.size() > 0)
      _msg.pop_back();
}

void GameChat::SendMsg()
{
   AddMsgToHistory(_msg);
   ClearCurrentMsg();
}

void GameChat::ClearCurrentMsg()
{
   _msg.clear();
}

void GameChat::ClearHistory()
{
   _history.clear();
}

void GameChat::Write(char a)
{
   _msg.push_back(a);
}

void GameChat::StartWrite()
{
   _writingOn = true;
}

void GameChat::EndWrite()
{
   _writingOn = false;
}

GameChat::GameChat(void)
{
   _writingOn = false;
   _histlimit = 30;
   list<string> _history;
}

bool GameChat::IsWritingOn()
{
   return _writingOn;
}

string GameChat::GetCurrentMsg()
{
   return _msg;
}

const char* GameChat::GetCurrentMsgC()
{
   //const int len = _msg.length();
   //const char* msg = new char[len];
   //msg = _msg.c_str();
   return _msg.c_str();
}

list<string>* GameChat::GetHistory()
{
   return &_history;
}

GameChat::~GameChat(void)
{
}
