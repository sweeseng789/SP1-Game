// This is the main file for the game logic and function
//
//
#include "game.h"
#include "enemy.h"
#include "bullet.h"
#include "Framework\console.h"
#include <dos.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <float.h>
using std::vector;
extern COORD consoleSize;
extern vector<enemy> unit;


//enemy passed in by reference so that updated X and Y coor. is updated to unit.X[i] and unit.Y[i]
void enemyMovement(vector<enemy>& unit, double elapsedTime, COORD consoleSize)
{
	//pattern for waves, spawn 4 followed by 3 ...
	 //check if enemy is still alive and pass in spawn time to check if the unit's wave period has expired, also pass in unit to edit its active status
	//waves starts when spawnTime is 4
	if(elapsedTime > 4)
	{
		
		double spawnTime = isEnemyAlive(elapsedTime, unit);//get the new spawn time
		//wave 1
		if(elapsedTime >=spawnTime)
		{
			for(int i=0; i<4; ++i)
			{
				if(unit[i].active == true)//if the unit is active
				{
					enemyUnitAi(elapsedTime, unit[i]);//calls enemyAi and update its direction(pass as pointer)
				
				}
			}
		}
		//wave 2
		if(elapsedTime >= spawnTime + 4)
		{
			for(int i=4; i<8; ++i)
			{
				if(unit[i].active == true)//if the unit is active
				{
					
					enemyUnitAi(elapsedTime, unit[i]);//calls enemyAi and update its direction(pass as pointer)
					
				}
			}
		}
	}
}

void enemyUnitAi(double time, enemy& unit)
{
	/**
	//if touch player line(bottom of screen, Y coor) or sides of console(X coor)
	int num = rand() % 2;//generate a number between 0 and 1

	if(num == 0)
	{
		if(body.Y <= 21)//if body is not reach
		{
			body.Y+=2;
		}
	}

	else if(num == 1)
	{
		int num2 = rand() % 2;//generate a number between 0 and 1 to move left or right
		//collision detection with other enemy units
		if(num2 == 0 && body.X <= 75 && body.Y <= 21)//move right
		{
			body.X++;
			body.Y++;
		}
		else if(num2 == 1  && body.X >= 5 && body.Y <= 21)//move left
		{
			body.X--;
			body.Y++;
		}
	}/**/
	if(unit.location.Y != unit.point.Y)
	{
		unit.location.Y++;
	}

	//if reached the X coor but not the Y coor of the target point and not the player line
	//if not reach the X coor of the target point and not reach player line
	if(unit.location.X != unit.point.X  && unit.location.Y == unit.point.Y && unit.point.Y != 35)
	{
		//if the target point X coor is bigger than the spawn point X coor
		if(unit.location.X < unit.point.X)
		{
			unit.location.X++;
		}
		if(unit.location.X > unit.point.X)
		{
			unit.location.X--;
		}
	}

		//if reached target point, call to set new target point and not reach player line
	if(unit.location.X == unit.point.X && unit.location.Y == unit.point.Y && unit.point.Y != 35)
	{
		unit.mobile = false;//if false, can fire bullets
		//unit.stop_Timer = time + (rand() % 3 + 1);//can stop from 1 to 3 seconds;
		targetPoint(unit);

	}
}

double isEnemyAlive(double currentTime, vector<enemy>& unit)
{
	//simulate if all enemy unit died when reach Y = 30 (add score function to count score)
	for(int i=0; i<8; ++i)
	{
		if(unit[i].location.Y >= 30)
		{
			unit[i].active = false;
		}
	}


	static double respawn = 4;//set to 4 first
	
	bool reset = false;//if true, reset all x and y of units (all units died)

	for(int i=0; i<8; ++i)
	{
		if(unit[i].active == false)//if unit is false
		{
			reset = true;
		}
		else if(unit[i].active == true)//else if unit is true
		{
			reset = false;
			break;//break since still got one unit alive
		}
	}

	//if reset true, all units are dead and reset their spawn points
	if(reset == true)
	{
		for(int i=0; i<8; ++i)
		{
			//if wave preiod ended and unit died before
				//reset enemy active to true
				unit[i].active = true;
				//reset each enemy unit spawn point Y
				unit[i].location.Y = unit[i].spawnLocation.Y;
				//reset each enemy unit spawn point X
				unit[i].location.X = unit[i].spawnLocation.X;
		}
		//reset the target points (including the Y coor individually)
		
		for(int i=0; i<8; ++i)
		{
			unit[i].point.Y = rand() % 5 + 1;
			unit[i].mobile = true;
			targetPoint(unit[i]);
		}

		respawn = currentTime + 2;//respawn time reset for further references
	}

	return respawn;//return start spawn time
}

void targetPoint(enemy& unit)
{
	//Grid 1: 1px - 5px 
	//Grid 2: 6 px - 10px
	//Grid 3: 11px - 15px
	//Grid 4: 16px - 20px
	//Grid 5: 21px - 25px
	int gridX = consoleSize.X / 8;//the length of each grid
	//loop through all units
	int X = unit.spawnLocation.X + gridX / 2;//X is the rightmost point of the grid of the respective enemy unit
	//store the target point for each enemy unit
	unit.point.X = rand() % 9 + (X - 10);//does not need
	unit.point.Y += 3;//everytime set new target point, enemy unit will move 5 pixels towards player

}

void printEnemy(COORD body)
{
	//print body
	gotoXY(body.X, body.Y);
	std::cout << (char)54;

	//print right wing
	gotoXY(body.X - 1, body.Y);
	std::cout << (char)195;

	//print left wing
	gotoXY(body.X + 1, body.Y);
	std::cout << (char)180;
}