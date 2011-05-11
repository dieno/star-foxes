#include "AIPlayer.h"


void AIPlayer::Update(HWND hWnd, radar)
{
   Wander(hWnd);
}

void AIPlayer::Wander(HWND hWnd)
{   
   static int dir = -1;
   static int count = 0;
   static float top = -2;//getPositionY() ;
   static float bottom = 2;//= getPositionY();
   static float left = 1;//getPositionX();
   static float right = -1;//getPositionX();
   //MessageBox(this->
   //float shit;
   /*char* shitty = (char*) malloc(25);
   sprintf(shitty, "%f", getPositionZ());
   MessageBoxA(hWnd, shitty, "hi", 0);
   */

   if(count <= 0 )/*|| 
      getPositionX() >= left ||
      getPositionX() <= right ||
      getPositionZ() >= bottom ||
      getPositionZ() <= top)*/
   {
      count = 0;
      srand(time(NULL));
      count = rand() % 15;
      srand(time(NULL));
      dir = rand() % 5 - 1;
   }

   switch(dir)
   {
   case 0: //up 
      this->bankUp(0.03f);
      /*if(getPositionZ() <= top)
      {
         this->bankDown(0.05f);
         count = 0;
      }*/
      //count-=;
      break;
   case 1: //down
      this->bankUp(0.03f);/*
      this->bankDown(0.03f);
      if(getPositionZ() >= bottom)
      {
         this->bankUp(0.03f);
         count = 0;
      }    */
      //count-=10;
      //this->bankUp(0.03f);
      break;
   case 2: //left
      this->bankLeft(0.05f);
      this->bankUp(0.03f);
      if(getPositionX() >= left)
      {
         this->bankRight(0.05f);
         count = 0;
      }
      break;
   case 3: //right
      this->bankRight(0.05f);
      this->bankUp(0.03f);
      if(getPositionX() <= right)
      {
         this->bankLeft(0.05f);
         count = 0;
      }
   //case 4: // none      
     // break;
   default:
      count = 0;
      break;
   }
   count--;

}
/*
AIPlayer::AI(void)
{
}
*/

AIPlayer::~AIPlayer(void)
{
}
