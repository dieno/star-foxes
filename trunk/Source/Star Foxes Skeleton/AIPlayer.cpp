#include "AIPlayer.h"

#include <math.h>

using namespace std;

// Sets the behaviour of the AI: Flee, attack, wander, seek.
void AIPlayer::SetBehaviour(EBehaviour beh)
{
   _behave = beh;   
}

// Sets the x and y bounds in which the AI can move around.
// Always set bounds of movement of AI by calling this function, otherwise it won't move.
void AIPlayer::SetBounds(D3DXVECTOR3 pos)
{
   _mv->top = 20;
   _mv->bottom = 0;
   _mv->left = -20;
   _mv->right = 20;
}

// Heart of the AI. Updates AI behaviour.
// TODO: Behaviour evaluator to evaluate what behaviour to activate.
void AIPlayer::Update(HWND hWnd, D3DXVECTOR3 pos)
{
   //srand((unsigned)time(0));
	//Seek(pos);
   //Move(hWnd, FWRD, NULL);
   switch(_behave)
   {
   case FLEE:
      Flee(hWnd, pos);
      break;
   case SEEK:
	  Seek(pos);
     //Flee(hWnd, pos);
     //Wander(hWnd);
	  break;
   default:
      Wander(hWnd);
      break;
   }
}

// AI Behaviour: makes AI wander around randomly.
void AIPlayer::Wander(HWND hWnd)
{   
   bool thing = false;

   if(_mv->count <= 0 )
   {
      this->down(false);
      this->up(false);
      this->right(false);
      this->left(false);
      _mv->count = std::rand() % 50;
      _mv->dir =  std::rand() % 2 + 2;//9;
   }

   Move(hWnd, _mv->dir, NULL);
   _mv->count--;
}

// AI Behaviour: makes the AI flee from a point in the screen.
// param pos: the location in screen to flee from.
void AIPlayer::Flee(HWND hWnd, D3DXVECTOR3 pos)
{
   float x = pos.x - getPositionVector().x;
   float y = pos.y - getPositionVector().y;
   float z = pos.z - getPositionVector().z;
   float dist = sqrt(x*x + y*y + z*z);

   static bool fleeing = false;

   if(dist <= 1 && _mv->count <=0)
   {
      up(false);
      down(false);
      left(false);
      right(false);
      _mv->dir = std::rand() % 9;
      _mv->count = 10;
   }

   if(_mv->count > 0)
   {
      Move(hWnd, _mv->dir, &fleeing);
      _mv->count--;
   }
}

// Moves AI to 8 determined directions: up, down, upleft, downright, etc.
// The movement happens within bounds set with SetBounds() function.
// If the AI stops whenever it tries to go outside the bounds.
EDir AIPlayer::Move(HWND hWnd, int dir, bool *outbound)
{
   switch(dir)
   {
   case UP: //0
      if(getPositionVector().y < _mv->top)
         this->down(true);//this->up(0.04f);
      else
      {
         this->down(false);
         this->up(true);
         _mv->dir = DWN;
      }
      return UP;
   case DWN: //1
      if(getPositionVector().y > _mv->bottom)
      //if(getPosition())
         this->up(true);//this->down(0.04f);
      else
      {
         this->down(true);
         this->up(false);
         _mv->dir = UP;
      }
      return DWN;
   case LFT: //2
      this->left(true);//this->left(0.05f);
      if(getPositionVector().x >= _mv->left)
         this->right(true);//this->right(0.05f);
      return LFT;
   case RGHT: //3
      this->right(true);//this->right(0.05f);
      if(getPositionVector().x <= _mv->right)
      {
         //this->left(0.05f);
         this->left(true);
      }
      return RGHT;
   case UPRGHT: //4
      Move(hWnd, UP, NULL);
      Move(hWnd, RGHT, NULL);
      return UPRGHT;
   case UPLEFT: //5
      Move(hWnd, UP, NULL);
      Move(hWnd, LFT, NULL);
      return UPLEFT;
   case DWNRGHT: //6
      Move(hWnd, DWN, NULL);
      Move(hWnd, RGHT, NULL);
      return DWNRGHT;
   case DWNLFT: //7
      Move(hWnd, DWN, NULL);
      Move(hWnd, LFT, NULL);
      return DWNLFT;
   case FWRD: //8
      this->boost(true);
      return FWRD;
   default:
      return DIR_NONE;
   }
}

void AIPlayer::Rotate2DvectorXZ(D3DXVECTOR3* pV2, float angle)
{
	// use local variables to find transformed components
	float Vx1 = cosf(angle)*pV2->x - sinf(angle)*pV2->z;
	float Vy1 = sinf(angle)*pV2->x + cosf(angle)*pV2->z;
	// store results thru the pointer
	pV2->x = Vx1;
	pV2->z = Vy1;

	return;
}

void AIPlayer::Rotate2DvectorYZ(D3DXVECTOR3* pV2, float angle)
{
	// use local variables to find transformed components
	float Vx1 = cosf(angle)*pV2->z - sinf(angle)*pV2->y;
	float Vy1 = sinf(angle)*pV2->z + cosf(angle)*pV2->y;
	// store results thru the pointer
	pV2->z = Vx1;
	pV2->y = Vy1;

	return;
}

void AIPlayer::Seek(D3DXVECTOR3 enemyPos) {

   float x = getPositionVector().x;//getPositionX();
	float z = getPositionVector().z;
	float y = getPositionVector().y;
	float xDif = enemyPos.x - x;
   float yDif = enemyPos.y - y;
   float zDif = enemyPos.z - z;
  
	float dist = sqrt(xDif*xDif + yDif*yDif + zDif*zDif);
   D3DXVECTOR3 d = enemyPos - getPositionVector();   
   d.x = -d.x / dist;
   d.y = d.y / dist;
   d.z = d.z / dist;

   float angle = atan2(getDirectionVector().z, getDirectionVector().x);
   Rotate2DvectorXZ(&d, -angle);
   float off = 0.2f;
   
   float angle2 = atan2(0, 1.0f) - atan2(d.z, d.x);
   if(angle2 > off)
   {
      left(true);
   }
   else if(angle2 < -off)
   {
      right(true);
   }
   else
   {
      left(false);
      right(false);
   }


   /*
   if(abs(angle) > off)
   {
      if(my.x > d.x)
      {
         if(d.z < 0)
            left(true);
         else
            right(true);
      }
      else
      {
         if(d.z < 0)
            right(true);
         else
            left(true);
      }
   }
   else
   {
      right(false);
      left(false);
   }*/

   /*if(angle > off && abs(angle))
   {
      left(true);
      //right(true);
   }
   else if (angle < -off && angle > D3DX_PI)
   {
      right(true);
   }
   else
   {
      right(false);
      left(false);
   }
   */

   /*double myangle = atan2(z, x);
   double enangle = atan2(d.z, d.x);
   double angle = myangle - enangle;*/


   //this->up(true);
   /*
	//if((enemyPos.x < x) && (offset)) {
   if((d.x < getDirectionVector().x) && (offset)) {
		this->left(true);//this->bankLeft(0.03f);
      //this->right(false);
		//directXClass::SetError(TEXT("SEEK: Moving [left] | from %f | to %f"), x, enemyPos.x);  
	} else if((d.x > getDirectionVector().x) && (offset)){
		this->right(true); //this->bankRight(0.03f);
      //this->left(false);
		//directXClass::SetError(TEXT("SEEK: Moving [right] | from %f | to %f"), x, enemyPos.x);  
	}
   */
   /*
   if(abs(d.y - getDirectionVector().y) < 0.1f)
   {
	
      if((d.y > getDirectionVector().y) && (offset)) {
		   this->down(true);//this->bankDown(0.02f);
         //this->up(false);
		   //directXClass::SetError(TEXT("SEEK: Moving [down] | from %f | to %f"), z, enemyPos.z);  
	   } else if(d.y < getDirectionVector().y && (offset)){
		   this->up(true);	//this->bankUp(0.02f);	
		   //this->down(false);
         //directXClass::SetError(TEXT("SEEK: Moving [up] | from %f | to %f"), z, enemyPos.z);  
	   }   
      else
      {
         down(false);
         up(false);
      }
   }
   */
   /*if(abs(d.x - getDirectionVector().x) > 0.1f)
   {
	   if((d.x < getDirectionVector().x) && (offset)) {
		   this->left(true);//this->bankDown(0.02f);
         //this->up(false);
		   //directXClass::SetError(TEXT("SEEK: Moving [down] | from %f | to %f"), z, enemyPos.z);  
	   } else if(d.x > getDirectionVector().x && (offset)){
		   this->right(true);	//this->bankUp(0.02f);	
		   //this->down(false);
         //directXClass::SetError(TEXT("SEEK: Moving [up] | from %f | to %f"), z, enemyPos.z);  
	   }
   }
   else
   {*/
   //   left(false);
     // right(false);
   
   /*
   if(abs(d.z - getDirectionVector().z) > 0.1f)
   {
	   if((d.z < getDirectionVector().z) && (offset)) {
		   this->right(true);//this->bankDown(0.02f);
         //this->up(false);
		   //directXClass::SetError(TEXT("SEEK: Moving [down] | from %f | to %f"), z, enemyPos.z);  
	   } else if(d.z > getDirectionVector().z && (offset)){
		   this->left(true);	//this->bankUp(0.02f);	
		   //this->down(false);
         //directXClass::SetError(TEXT("SEEK: Moving [up] | from %f | to %f"), z, enemyPos.z);  
	   }
   }
   else
   {
      left(false);
      right(false);
   }*/
   /*
	if((d.z < getDirectionVector().z) && (offset)) {
		this->down(true);//this->bankDown(0.02f);
      //this->up(false);
		//directXClass::SetError(TEXT("SEEK: Moving [down] | from %f | to %f"), z, enemyPos.z);  
	} else if(d.y > getDirectionVector().y && (offset)){
		this->up(true);	//this->bankUp(0.02f);	
		//this->down(false);
      //directXClass::SetError(TEXT("SEEK: Moving [up] | from %f | to %f"), z, enemyPos.z);  
	}*/
}


void AIPlayer::SeekIra(D3DXVECTOR3 enemyPos) {

   float x = getPositionVector().x;//getPositionX();
	float z = getPositionVector().z;
	float y = getPositionVector().y;
	float xDif = enemyPos.x - x;
   float yDif = enemyPos.y - y;
   float zDif = enemyPos.z - z;

   

	float dist = sqrt(xDif*xDif + yDif*yDif + zDif*zDif);
	bool offset = true; //dist >= 1;
   //this->up(true);
   
	if((enemyPos.x < x) && (offset)) {
		this->left(true);//this->bankLeft(0.03f);
      //this->right(false);
		//directXClass::SetError(TEXT("SEEK: Moving [left] | from %f | to %f"), x, enemyPos.x);  
	} else if((enemyPos.x > x) && (offset)){
		this->right(true); //this->bankRight(0.03f);
      //this->left(false);
		//directXClass::SetError(TEXT("SEEK: Moving [right] | from %f | to %f"), x, enemyPos.x);  
	}
   
	if((enemyPos.y > y) && (offset)) {
		this->down(true);//this->bankDown(0.02f);
      //this->up(false);
		//directXClass::SetError(TEXT("SEEK: Moving [down] | from %f | to %f"), z, enemyPos.z);  
	} else if((enemyPos.y < y) && (offset)){
		this->up(true);	//this->bankUp(0.02f);	
		//this->down(false);
      //directXClass::SetError(TEXT("SEEK: Moving [up] | from %f | to %f"), z, enemyPos.z);  
	}

}


// Initializes AI. Function usually called in constructor.
void AIPlayer::IniAI()
{
   //_mv = (PMovement) malloc (sizeof(PMovement));
   _mv = (Movement*) malloc (sizeof(Movement));
   _mv->dir = -1;
}

AIPlayer::~AIPlayer(void)
{
}
