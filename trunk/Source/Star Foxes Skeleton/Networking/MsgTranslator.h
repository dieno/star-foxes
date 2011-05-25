//#pragma once
#include "../directXClass.h"
//#include <std::string.h>
//#include <Windows.h>
//using namespace std;

#ifndef MSGTRANSLATOR
#define MSGTRANSLATOR
enum EMSG_TYPE
{
   MSG_CMD = '0',
   MSG_MSC = '1',
   MSG_TXT = '2'
};

enum ECOMMAND
{
   CMD_NONE = '0',
   MV_LEFT = VK_LEFT,
   MV_RITE = VK_RIGHT,
   MV_UP = VK_UP,
   MV_DOWN = VK_DOWN,

   //MISC
   MSC_SETID  ='i'

   //TEXT
};
/*
enum MISC
{
   MSC_SETID  ='i'
};
*/
class Msg
{
private:
   int _ixType;
   int _ixCmd;
   int _ixBody;
   int _szType;
   int _szCmd;
   int _szBody;
   char* _msg;
   int body_size;

public:
   char* GetMsg();
   void SetMsg(char* msg);
   int CmpType(char* msg);
   int CmpID(char* msg);
   int CmpBody(char* msg);
   char GetType();
   char GetCmd();
   char* GetID();
   ECOMMAND GetCommand();
   char GetBody();
   Msg(char* msg, int sztype, int szid, int szmsg);
   Msg(int sztype, int szid, int szmsg);   
   void Initialize(char* msg, int sztype, int szid, int szmsg);
   ~Msg(void);
   Msg(void);
};


class MsgTranslator
{
private:
   static const int _szType = 1;
   static const int _szCmd = 1;
   static const int _szMsg = -1;
   Msg* _msg;
public:
   void SetMsg(char* msg);
   Msg* GetMsg();
   EMSG_TYPE TranslateMsg(char* msg);
   int CreateMsg(char* msg, EMSG_TYPE type, char* id, char *body);
   int CreateMsg(char* msg, EMSG_TYPE type, ECOMMAND cmd, char *body);
   int CreateMsgMsc(char* msg, ECOMMAND cmd, char* id, char *body);
   MsgTranslator(void);
   ~MsgTranslator(void);
};

#endif