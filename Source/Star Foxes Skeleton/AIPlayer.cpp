#include "AIPlayer.h"


void AIPlayer::Update(HWND hWnd, D3DXVECTOR3 pos)
{
   //srand((unsigned)time(0));
   //Wander(hWnd);
   Flee(hWnd, pos);
}

void AIPlayer::Flee(HWND hWnd, D3DXVECTOR3 pos)
{
   float x = pos.x - getPositionX();
   float y = pos.y - getPositionY();
   float z = pos.z - getPositionZ();
   float dist = sqrt(x*x + y*y + z*z);

   static int dir = 0;// = rand() % 5;
   static int count = 0;
   static bool fleeing = false;

   if(dist <= 1 && count <=0)
   {
      dir = rand() % 4;
      count = 10;
   }

   if(count > 0)
   {
      Move(hWnd, dir, &fleeing);      
      count--;
   }
}

EDir AIPlayer::Move(HWND hWnd, int dir, bool *outbound)
{
   static float top = -0.5;
   static float bottom = 0.5;
   static float left = 1;
   static float right = -1;

   switch(dir)
   {
   case UP:      
      if(getRotation().x < .2 && getPosition().z > -1.5)
         this->bankUp(0.04f);
      return UP;
   case DWN:      
      if(getRotation().x > -.2 && getPosition().z < 1.5)
         this->bankDown(0.04f);       
      return DWN;
   case LFT:      
      this->bankLeft(0.05f);
      if(getPositionX() >= left)      
         this->bankRight(0.05f);      
      return LFT;
   case RGHT:
      this->bankRight(0.05f);
      if(getPositionX() <= right)
      {
         this->bankLeft(0.05f);
      }
      return RGHT;
   default:
      return DIR_NONE;
   }
}


void AIPlayer::Wander(HWND hWnd)
{   
   static int dir = -1;
   static int count = 0;
   static int c = 0;
   bool thing = false;

   if(count <= 0 )
   {
      count = 0;      
      count = rand() % 50;
      dir =  rand() % 5;
   }

   Move(hWnd, dir, &thing);
   count--;
}


AIPlayer::~AIPlayer(void)
{
}
