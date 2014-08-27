// This is the main file for the game logic and function
//
//
#include "game.h"
#include "bullet.h"
#include "Framework\console.h"
#include "enemy.h"
#include "mainmenu.h"
#include <iostream>
#include <iomanip>
#include <stdlib.h>

double elapsedTime;
double deltaTime;
bool keyPressed[K_COUNT];
COORD charLocation;
COORD consoleSize;
COORD playerDirection;
vector<enemy> unit;


void init()
{
	srand (time(NULL));
    // Set precision for floating point output
    std::cout << std::fixed << std::setprecision(3);

    SetConsoleTitle(L"SP1 Framework");

    // Sets the console size, this is the biggest so far.
    setConsoleSize(80, 40);//original: X 79  Y 28

    // Get console width and height
    CONSOLE_SCREEN_BUFFER_INFO csbi; /* to get buffer info */     

    /* get the number of character cells in the current buffer */ 
    GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &csbi );
    consoleSize.X = csbi.srWindow.Right + 1;
    consoleSize.Y = csbi.srWindow.Bottom + 1;

    // set the character to be in the center of the screen.
    charLocation.X = consoleSize.X / 2;
    charLocation.Y = consoleSize.Y / 2;

    elapsedTime = 0.0;

	
	//initialize enemy unit spawn points first time round (total of 7 enemy units), read from text file
	//set all enemy units to true first time round
	for(int i=0; i<8; ++i)
	{
		unit.push_back(enemy());//push back empty enemy struct to populate it
		unit[i].active = true;
		unit[i].spawnLocation.Y = 1;//each unit Y is 0
		unit[i].location.Y = unit[i].spawnLocation.Y;
		unit[i].point.Y = 0;
		unit[i].stop_Timer = 0;
		unit[i].mobile = true;
	}

	//set the spawn location for now
	unit[0].spawnLocation.X = 5;
	unit[1].spawnLocation.X = 25;
	unit[2].spawnLocation.X = 45;
	unit[3].spawnLocation.X = 65;
	unit[4].spawnLocation.X = 15;
	unit[5].spawnLocation.X = 35;
	unit[6].spawnLocation.X = 55;
	unit[7].spawnLocation.X = 75;

	for(int i=0; i<8; ++i)
	{
		unit[i].location.X = unit[i].spawnLocation.X;
	}

	//set all the target points first

	//set the X and Y coor of each target points first
	for(int i=0; i<8; ++i)
	{
		unit[i].point.Y = rand() % 5 + 1;//increments by 5 each time target point is reached to form a new Y coor of a new target point
		targetPoint(unit[i]);//set units target points
	}
}

void shutdown()
{
    // Reset to white text on black background
	colour(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
}

void getInput()
{    
    keyPressed[K_UP] = isKeyPressed(VK_UP);
    keyPressed[K_DOWN] = isKeyPressed(VK_DOWN);
    keyPressed[K_LEFT] = isKeyPressed(VK_LEFT);
    keyPressed[K_RIGHT] = isKeyPressed(VK_RIGHT);
    keyPressed[K_ESCAPE] = isKeyPressed(VK_ESCAPE);
	keyPressed[K_SPACE] = isKeyPressed(VK_SPACE);
}

void update(double dt)
{
   // get the delta time
    elapsedTime += dt;
    deltaTime = dt;

    // Updating the location of the character based on the key press
    if (keyPressed[K_UP] && charLocation.Y > 0)
    {
        charLocation.Y-=1; 
    }
    if (keyPressed[K_LEFT] && charLocation.X > 1)
    {
        charLocation.X-=3; 
    }
    if (keyPressed[K_DOWN] && charLocation.Y < consoleSize.Y - 1)
    {
       charLocation.Y+=1; 
    }
    if (keyPressed[K_RIGHT] && charLocation.X < consoleSize.X - 4)
    {
        charLocation.X+=3; 
    }

	//player direction
    if (keyPressed[K_LEFT] && playerDirection.X >= 0)
    {
		playerDirection.X = charLocation.X - 1;
		playerDirection.Y = charLocation.Y;
    }
    else if (keyPressed[K_DOWN] && playerDirection.Y < consoleSize.Y - 1)
    {
        playerDirection.X = charLocation.X - 1;
		playerDirection.Y = charLocation.Y;
    }
    else if (keyPressed[K_RIGHT] && playerDirection.X < consoleSize.X - 1)
    {
		playerDirection.X = charLocation.X + 1;
		playerDirection.Y = charLocation.Y;
    }
    // quits the game if player hits the escape key
    if (keyPressed[K_ESCAPE])
        g_quitGame = true;    

	//updates enemy movement, parameter is enemy unit, double elapsedTime, COORD consoleSize
	enemyMovement(unit, elapsedTime, consoleSize);
}

void render()
{
     // clear previous screen
    colour(0x0F);
    cls();

    //render test screen code (not efficient at all)
    const WORD colors[] =   {
	                        0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F,
	                        0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6
	                        };
	
	for (int i = 0; i < 7; ++i)
	{
		gotoXY(i*12, 4);
		colour(colors[5]);
		std::cout << "Hit";
	}


    // render time taken to calculate this frame
    gotoXY(70, 0);
    colour(0x1A);
    std::cout << 1.0/deltaTime << "fps" << std::endl;
  
    gotoXY(0, 0);
    colour(0x59);
    std::cout << elapsedTime << "secs" << std::endl;

	
	// score appearing on screen
	gotoXY(60, 0);
	colour(0xffffff00);
	std::cout << "Score: " << score() << " ";

	//player character
	gotoXY(charLocation);
    colour(0x0C);
	user player ;//Player direction
	player.front = (char)192;
	player.back = (char)191;
	player.left = (char)170;
	player.right = (char)169;
	player.body =(char)233;

    gotoXY(charLocation);
    colour(0x0C);
	std::cout << player.body;

	if (GetAsyncKeyState(VK_UP) != 0)
	{
		std::cout << player.front;
	}
	else if (GetAsyncKeyState(VK_DOWN) != 0)
	{
		gotoXY(playerDirection);
		std::cout << player.back;
	}
	else if (GetAsyncKeyState(VK_LEFT) != 0)
	{
		gotoXY(playerDirection);
		std::cout << player.left;
	}
	else if (GetAsyncKeyState(VK_RIGHT) != 0)
	{
		gotoXY(playerDirection);
		std::cout << player.right;
	}
	else if (GetAsyncKeyState(VK_UP) == 0)//by default face up
	{
		std::cout << player.front;
	}

    //render the game
	for(int i=0; i<8; ++i)
	{//psss in the X and Y coor. of each unit
		if(unit[i].active == true)
		{
			printEnemy(unit[i].location);
		}
	}	
}


int score()
{
	int currentscore = 0;
	int points = 10;

	//while (g_quitGame !=true)
	//{
	//	if (insert bullet hitting enemy function here)
	//	{
	//		 score += points;
	//	}
	//	std::cout << "Score: " << currentscore << " ";
	//}

	return currentscore;
}

int highscore()
{
	std::string data;
	int a;
	int b = score();
	int higherscore;

	std::ifstream inData;
	inData.open ("Images/highscore.txt");
	inData >> a;

	if (a > b)
	{
		higherscore = a;
	}
	else
	{
		higherscore = b;
	}

	std::ofstream highscore;
	highscore.open ("Images/highscore.txt");
	highscore << higherscore;
	highscore.close();
	return 0;
}
