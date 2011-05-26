#include "AIPlayer.h"

#include <math.h>

using namespace std;

void FSM::setCurrentState(EState _state)
{
   currentState = _state;   
}

int FSM::getMasterIdx()
{
	return masterIdx;
}

string FSM::getCurrentStateName()
{
	switch(currentState){
		case FLEE:
			return string("FSM // Current State: Fleeing");
			break;
		case WAND:
			return string("FSM // Current State: Wandering");
			break;
		case ATCK:
			return string("FSM // Current State: Attacking");
			break;
		case SEEK:
			return string("FSM // Current State: Seeking");
			break;
		default:
			return "FSM // ERROR - Unable to evaluate current state";
	}
}

EState FSM::getCurrentState() 
{
	return currentState;
}

float FSM::evalDistToTarget(D3DXVECTOR3 target, D3DXVECTOR3 origin)
{
	float dist;
	D3DXVECTOR3 res;
	res = target - origin;
	dist = sqrt(res.x*res.x + res.y*res.y + res.z*res.z);
   
   res.x /= dist; // need to do this reflection first
   res.y /= dist; // Setting univector from this ship to enemy position.
   res.z /= dist;

   return dist;
}


/*
	Call appropriate evaluation function depending on current state.
*/
D3DXVECTOR3 FSM::eval()
{
	D3DXVECTOR3 target;
	
	directXClass::SetError(TEXT("FSM // Evaluating game state..."));

	switch(currentState){
		case FLEE:
			target = evalFlee();
			break;
		case WAND:
			target = evalWander();
			break;
		case ATCK:
			target = evalAttack();
			break;
		case SEEK:
			target = evalSeek();
			break;
	}
  /* currentState = SEEK;
   target = gamestate->getPlayer(0)->getPosition();*/
		
	return target;
}

/*
	Evaluate gamestate and determine transition (if any) when current state is FLEE. 
*/
D3DXVECTOR3 FSM::evalFlee()
{
	D3DXVECTOR3 tmp;
	return tmp;
}

/*
	Evaluate gamestate and determine transition (if any) when current state is WAND. 
*/
D3DXVECTOR3 FSM::evalWander()
{
	directXClass::SetError(TEXT("FSM // Evaluating WAND state..."));

	std::list<MainPlayerClass*> players;
	float shortestDist = RADAR_RADIUS; //the radar / 'sight' distance the AI can detect
	float currDist;
	int targetIdx;
	D3DXVECTOR3 target;
    MainPlayerClass* ePlayer;
	int i = 0;

 	for (i = 0; i < 8; i++) {
		if( ((ePlayer = gamestate->getPlayer(i)) != NULL) && (ePlayer->getID() != getMasterIdx())) {
			currDist = evalDistToTarget(ePlayer->getPosition(), gamestate->getPlayer(getMasterIdx())->getPosition());
			if(currDist < shortestDist) {
				shortestDist = currDist;
				targetIdx = ePlayer->getID();
				setCurrentState(SEEK);
			}
		}
	}
	/*
	for (std::list<MainPlayerClass*>::const_iterator pi = players.begin(); pi != players.end(); ++pi)
	{
		//Search through enemies until one is found within range. Begin seeking target.
		if((*pi) != NULL && ((*pi)->getID() != getMasterIdx())) {
			currDist = evalDistToTarget((*pi)->getPosition(), gamestate->getPlayer(getMasterIdx()).getPosition());
			if(currDist < shortestDist) {
				shortestDist = currDist;
				targetIdx = (*pi)->getID();
				setCurrentState(SEEK);
			}
		}  		
	}
	*/

   if(getCurrentState() == SEEK) {
		target = gamestate->getPlayer(targetIdx)->getPosition();
	}

	return target;
}

/*
	Evaluate gamestate and determine transition (if any) when current state is ATCK. 
		-Attacks player closest and with the lowest HP of all targets in shooting range.
*/
D3DXVECTOR3 FSM::evalAttack()
{/*
	directXClass::SetError(TEXT("FSM // Evaluating ATCK state..."));
   
	std::list<MainPlayerClass*> players;
	float shortestDist = RADAR_RADIUS; //the radar / 'sight' distance the AI can detect
	bool isShootingRange = false;
	float currDist;
	int targetIdx;
	int lastTargetIdx;
	D3DXVECTOR3 target;
	MainPlayerClass* ePlayer;
	int i = 0;

	for (int i = 0; i < 8; i++) {
		if( ((ePlayer = gamestate->getPlayer(i)) != NULL) && (ePlayer->getID() != getMasterIdx())) {
			shortestDist = evalDistToTarget(ePlayer->getPosition(), gamestate->getPlayer(getMasterIdx())->getPosition());
			if(shortestDist < RADAR_RADIUS) {
				targetIdx = ePlayer->getID();
				if((shortestDist <= SHOOT_RANGE) && 
					(gamestate->getPlayer(targetIdx)->getShipCurrentHealth() 
					< gamestate->getPlayer(lastTargetIdx)->getShipCurrentHealth())) {
					isShootingRange = true;
				}
			}
			lastTargetIdx = ePlayer->getID();
		} 		
	}

	if(shortestDist == RADAR_RADIUS) {
		setCurrentState(WAND);
	}else if(isShootingRange) {
		target = gamestate->getPlayer(targetIdx)->getPosition();
	}
	else {
		setCurrentState(SEEK);
		target = gamestate->getPlayer(targetIdx)->getPosition();
	}

	return target;*/
   return D3DXVECTOR3(0, 0, 0);
}

/*
	Evaluate gamestate and determine transition (if any) when current state is SEEK. 
*/
D3DXVECTOR3 FSM::evalSeek()
{
	directXClass::SetError(TEXT("FSM // Evaluating SEEK state..."));

	std::list<MainPlayerClass*> players;
	float shortestDist = RADAR_RADIUS; //the radar / 'sight' distance the AI can detect
	float currDist;
	int targetIdx;
	D3DXVECTOR3 target;
	MainPlayerClass* ePlayer;
	int i = 0;

	//Search through enemies until one is found within range. Begin seeking target.
	for (int i = 0; i < 8; i++) {
		if( ((ePlayer = gamestate->getPlayer(i)) != NULL) && (ePlayer->getID() != getMasterIdx())) {
			currDist = evalDistToTarget(ePlayer->getPosition(), gamestate->getPlayer(getMasterIdx())->getPosition());
			if(currDist < shortestDist) {
				shortestDist = currDist;
				targetIdx = ePlayer->getID();
			}
		}  		
	}

	if(shortestDist == RADAR_RADIUS) {
		setCurrentState(WAND);
	} else if(shortestDist <= SHOOT_RANGE) {
		setCurrentState(ATCK);
		target = gamestate->getPlayer(targetIdx)->getPosition();
	}
	else {
		target = gamestate->getPlayer(targetIdx)->getPosition();
	}

	return target;
}


//------------------------------------------------------------


// Sets the x and y bounds in which the AI can move around.
// Always set bounds of movement of AI by calling this function, otherwise it won't move.
void AIPlayer::SetBounds(D3DXVECTOR3* pos)
{
   // Bounds of movement for ship
   float xzBound = 60;
   _mv->top = 100;
   _mv->bottom = 5;
   _mv->left = -xzBound;
   _mv->right = xzBound;
   _mv->back = _mv->left;
   _mv->front = _mv->right;
   
   // considered when straightening the ship
   _mv->straightDownHi = _mv->top - 0.2f;
   _mv->straightDownLow = _mv->top - 0.4f;
   _mv->straightUpHi = 0.4f;
   _mv->straightUpLow = 0.2f;
}

// The AI starts moving and updating only if when you call this function first
void AIPlayer::Start()
{
   _boosting = true;
}

// Heart of the AI. Updates AI behaviour.
// TODO: Behaviour evaluator to evaluate what behaviour to activate.
void AIPlayer::Update(float timeDelta)
{
   /*if(!_boosting) // Have to call Start() for the AI to start moving
   return;*/
   D3DXVECTOR3 target = _fsm.eval();
   HWND hWnd = _fsm.getGameState()->getHWND();
   boost(true);

   if(KeepInBounds(hWnd))
   { 
	  switch(_fsm.getCurrentState())
	   {
		case FLEE:
			Flee(hWnd, target);
			break;
		case WAND:
			Wander(hWnd);
			break;
		case ATCK:
			Attack(target, timeDelta);
			break;
		case SEEK:
			Seek(target);
			break;
	  }
   }

   MainPlayerClass::Update(timeDelta);
}

// Returns true if the ship is inside the bounds.
// If it's not inside the bounds, it makes the ship seek the inside of the bound area.
bool AIPlayer::KeepInBounds(HWND hWnd)
{
   
   // If going outside xz-horizontal bound, make it seek the middle of map.
   if(getPositionVector().x > _mv->right ||
      getPositionVector().x < _mv->left ||
      getPositionVector().z > _mv->front ||
      getPositionVector().z < _mv->back)
   {
      //Move(hWnd, DIR_NONE, NULL);
      Seek(D3DXVECTOR3(1, 10, 1));
      return false;
   }
   

   // If it's out of y-bottom bound, straighten up.
   if(getPositionVector().y < _mv->bottom)
   {   
      StraightenUp();
      return false;
   }

   // If it's out of y-top bound, straighten down.
   if(getPositionVector().y > _mv->top)
   {
      StraightenDown();
      return false;
   }
   
   return true;
}

// AI Behaviour: makes AI wander around randomly.
void AIPlayer::Wander(HWND hWnd)
{   
   bool thing = false;

   if(_mv->count <= 0 )
   {  
      _mv->count = std::rand() % 20;
      _mv->dir =  std::rand() % 12;
      if(_mv->dir > 8)
         _mv->dir = 8;
      if(_mv->dir == UP )
         _mv->count *= 5;
   }   

   Move(hWnd, _mv->dir, &thing, true);
   _mv->count--;
}

// AI Behaviour: makes the AI flee from a point in the screen.
// param pos: the location in screen to flee from.
void AIPlayer::Flee(HWND hWnd, D3DXVECTOR3 pos)
{
   float x = pos.x - getPositionX();
   float y = pos.y - getPositionY();
   // getting vector from this ship position to enemy position.
   D3DXVECTOR3 vpos = D3DXVECTOR3(pos.x - getPositionVector().x,
      pos.y - getPositionVector().y,
      pos.z - getPositionVector().z);
   float dist = sqrt(vpos.x*vpos.x + vpos.y*vpos.y + vpos.z*vpos.z);

   //creating univector for vpos.
   vpos.x /= -dist;
   vpos.y /= dist;
   vpos.z /= dist;

   D3DXVECTOR3 vdir = getDirectionVector();
   float halfPi = 2.5f; // I know is not half pi. Radius to flee from.
   float angle = atan2(vdir.z, vdir.x);
   Rotate2DvectorXZ(&vpos, -angle);
   float angle2 = atan2(vpos.z, vpos.x) - atan2(0, 1.0f);

   if(abs(angle2) < halfPi)
   {
      if(angle2 > 0)
         left(true);
      else
         right(true);
   }
   else
   {
      right(false);
      left(false);
      if(_mv->count <= 0)
      {
         _mv->dir = std::rand() % 6;
         _mv->count = std::rand() % 5 + 5;
         if(_mv->dir > 2)
         {
            _mv->dir = DIR_NONE;
            _mv->count *= 10;
         }
      }
      else
      {
         Move(hWnd, _mv->dir, NULL, true);
         _mv->count--;
      }
   }
}

// Moves AI to 8 determined directions: up, down, upleft, downright, etc.
// The movement happens within bounds set with SetBounds() function.
// If the AI stops whenever it tries to go outside the bounds.
// param move: whether to active or cancel the move
EDir AIPlayer::Move(HWND hWnd, int dir, bool *outbound, bool move)
{
   if(directXClass::program->_IamClient){
      return DIR_NONE;
   }
   static char _netmsg[3];
   static char id[1];
   switch(dir)
   {
   case UP: //0
      //if(directXClass::program->_IamServer)
      if(directXClass::program->_IamServer){
         id[0] = 1;
         directXClass::program->_msgt.CreateMsg(_netmsg, MSG_CMD, id, "S");
         directXClass::program->_server.BroadcastMsg(_netmsg, 3);
      }
      else
         this->down(move);//this->up(0.04f);
      return UP;
   case DWN: //1
      if(directXClass::program->_IamServer){
         id[0] = 1;
         directXClass::program->_msgt.CreateMsg(_netmsg, MSG_CMD, id, "W");
         directXClass::program->_server.BroadcastMsg(_netmsg, 3);
      }
      else
         this->up(move);//this->down(0.04f);
      return DWN;
   case LFT: //2
      if(directXClass::program->_IamServer){
         id[0] = 1;
         directXClass::program->_msgt.CreateMsg(_netmsg, MSG_CMD, id, "A");
         directXClass::program->_server.BroadcastMsg(_netmsg, 3);
      }
      else
         this->left(move);//this->left(0.05f);
      return LFT;
   case RGHT: //3
      if(directXClass::program->_IamServer){
         id[0] = 1;
         directXClass::program->_msgt.CreateMsg(_netmsg, MSG_CMD, id, "D");
         directXClass::program->_server.BroadcastMsg(_netmsg, 3);
      }
      else
         this->right(move);//this->right(0.05f);
      return RGHT;
   case UPRGHT:
      Move(hWnd, UP, NULL, move);
      Move(hWnd, RGHT, NULL, move);
      return UPRGHT;
   case UPLEFT:
      Move(hWnd, UP, NULL, move);
      Move(hWnd, LFT, NULL, move);
      return UPLEFT;
   case DWNRGHT:
      Move(hWnd, DWN, NULL, move);
      Move(hWnd, RGHT, NULL, move);
      return DWNRGHT;
   case DWNLFT:
      Move(hWnd, DWN, NULL, move);
      Move(hWnd, LFT, NULL, move);
      return DWNLFT;
   default:
      if(directXClass::program->_IamServer){
         id[0] = 1;
         directXClass::program->_msgt.CreateMsg(_netmsg, MSG_CMD, id, "C");
         directXClass::program->_server.BroadcastMsg(_netmsg, 3);
      }
      left(false);
      right(false);
      up(false);
      down(false);
      return DIR_NONE;
   }
}

void AIPlayer::Seek(D3DXVECTOR3 enemyPos) {   
   float dist;// = sqrt(xDif*xDif + yDif*yDif + zDif*zDif);
   D3DXVECTOR3 d;// = enemyPos - getPositionVector();
   GetMeToPosVector(&enemyPos, &d, &dist);
   D3DXVECTOR3 d2 = d;
   float off = 0.02f;
   float angle, angle2;

   // Calculating horizontal rotation
   angle = atan2(getDirectionVector().z, getDirectionVector().x);
   Rotate2DvectorXZ(&d, -angle);
      
   angle2 = atan2(d.z, d.x) - atan2(0, 1.0f);
   if(angle2 > off)
      if(getUpVector().y > 0) // Check up vector to see if ship is upside down
         Move(NULL, LFT, NULL, true);//left(true);
      else
         Move(NULL, RGHT, NULL, true);//right(true);
   else if(angle2 < -off)
      if(getUpVector().y > 0) // Check up vector to see if ship is upside down
         Move(NULL, RGHT, NULL, true);//right(true);
      else 
         Move(NULL, LFT, NULL, true); //left(true);
   else
   {
      Move(NULL, LFT, NULL, false);
      Move(NULL, RGHT, NULL, false);
      //left(false);
      //right(false);
   }
   
   float offy = 0.1f;
   float yy = d.y - getDirectionVector().y;
   if(yy > offy)
      if(getUpVector().y > 0)
         Move(NULL, UP, NULL, true);//down(true);
      else
         Move(NULL, DWN, NULL, true);
   else if(yy < -offy)
      if(getUpVector().y > 0)
         Move(NULL, DWN, NULL, true);//up(true);
      else
         Move(NULL, UP, NULL, true);//down(true);
   else
   {
      Move(NULL, DWN, NULL, false);
      Move(NULL, UP, NULL, false);
      //up(false);
      //down(false);
   }
   //calculating vertical rotation
   //d2.z = -d2.z;
   //d2.y = -d2.y;
   /*angle = atan2(getDirectionVector().z, getDirectionVector().y);
   Rotate2DvectorYZ(&d2, -angle);
   
   
   angle2 = atan2(d2.z, d2.y) - atan2(0, 1.0f);
   if(angle2 > off)
     // if(getUpVector().y > 0)
      //   down(true);
     // else
         up(true);
      //else
         //down(true);
   else if(angle2 < -off)
      {//if(getUpVector().y > 0)
         down(true);
     // else
       //  up(true);
      //else
      //up(true);
      }
   else
   {
      up(false);
      down(false);
   }*/
   /*
      if(angle2 > off)
      //if(getUpVector().y > 0)
         up(true);
      //else
         //down(true);
   else if(angle2 < -off)
      {//if(getUpVector().y > 0)
         down(true);
      //else
      //up(true);
      }
   else
   {
      up(false);
      down(false);
   }
*/
}

// Rotates the X and Z of the vector to the given angle.
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

// Rotates Z and Y of the vector given the angle.
void AIPlayer::Rotate2DvectorYZ(D3DXVECTOR3* pV2, float angle)
{
	// use local variables to find transformed components
	float Vx1 = cosf(angle)*pV2->y - sinf(angle)*pV2->z;
	float Vy1 = sinf(angle)*pV2->y + cosf(angle)*pV2->z;
	// store results thru the pointer
	pV2->y = Vx1;
	pV2->z = Vy1;

	return;
}

// Straightens the ship setting it with an slight up-direction.
bool AIPlayer::StraightenUp()
{
   float off = 0.2f;
   Move(NULL, DIR_NONE, NULL, true);
   if(getDirectionVector().y > 0.4f || getDirectionVector().y < off)
   {
      if(getUpVector().y > 0)
         Move(NULL, UP, NULL, true);
      else
         Move(NULL, DWN, NULL, true);
      return false;
   }
   else
   {
      /*up(false);
      down(false);
      left(false);
      right(false);*/
      Move(NULL, DIR_NONE, NULL, false);
      return true;
   }
}

// Straightens the ship setting it with an slight down-direction.
bool AIPlayer::StraightenDown()
{
   float off = 0.2f;
   Move(NULL, DIR_NONE, NULL, false);
   if(getDirectionVector().y > 0)// || getDirectionVector().y < -0.4f)
   {
      if(getUpVector().y > 0)
         Move(NULL, DWN, NULL, true);
      else
         Move(NULL, UP, NULL, true);
      return false;
   }
   else
   {
      Move(NULL, DIR_NONE, NULL, false);
      /*up(false);
      down(false);
      left(false);
      right(false);*/
      return true;
   }
}

/*
	Attack state is a combination of SEEK state and shoot. 
*/
void AIPlayer::Attack(D3DXVECTOR3 target, float timeDelta)
{
	Seek(target);
	Shoot(target, timeDelta);
}

void AIPlayer::Shoot(D3DXVECTOR3 target, float timeDelta)
{
	D3DXVECTOR3 dist;
	float fDist;
	GetMeToPosVector(&target, &dist, &fDist);

	D3DXVECTOR3	comp = getPositionVector();
    D3DXVECTOR3 diff = comp - dist;

    if(abs(diff.x) < _shootArea.x && abs(diff.y) < _shootArea.y) {
       MainPlayerClass::shoot(timeDelta);
	}
}

// Gets a vector from this AI position to the position given in loc.
// Resulting vector 'res' is returned as an UNIVECTOR.
// Out pointer paramer 'dist' saves the distance between both locations.
void AIPlayer::GetMeToPosVector(D3DXVECTOR3* loc, D3DXVECTOR3* res, float* dist)
{
   *res = *loc - getPositionVector();
	*dist = sqrt(res->x*res->x + res->y*res->y + res->z*res->z);
   
   res->x /= *dist; // need to do this reflection first
   res->y /= *dist; // Setting univector from this ship to enemy position.
   res->z /= *dist;
}

// Initializes AI. Function usually called in constructor.
void AIPlayer::IniAI()
{
   //_mv = (PMovement) malloc (sizeof(PMovement));
   _boosting = false;
   float shootarea = 0.2f;
   srand(timeGetTime());
   _mv = (Movement*) malloc (sizeof(Movement));
   _mv->dir = -1;
   _shootArea.x = shootarea;
   _shootArea.y = shootarea;
}

AIPlayer::~AIPlayer(void)
{
}
