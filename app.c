#include "app.h"
#include "gameEngine.h"

// All the variables required in the programs declared first.

//For canvas
PFbyte titleCanvasId, descCanvasId,helpCanvasId, settingCanvasId;
//For windows
PFbyte mainWindowId, helpWindowId, playWindowId, settingWindowId;
//For widgets
PFbyte playWidgetId, helpWidgetId, backWidgetId, settingWidgetId, keypadWidgetId, accelerometerWidgetId;
//For co-ordinates
PFdword x,y;
//Variable to store id of current window
PFbyte current_window;

// To get input from keypad
PFdword switchNumber=0;

PFword i=0;
int s=0,hs=0,b=0;
int count=0,mode=0;
char score[2],highScore[10];
#define CENTER_X 120
#define CENTER_Y 160

//Dimensions of the rectangles that alltogether form a car
#define BOTTOM_RECT_X 165
#define BOTTOM_RECT_Y 300
#define RECT_WIDTH 35
#define RECT_HEIGHT 10

//All structures required for player gameObject
RectangleProperties player[4];
RectangleProperties playerPrevConfig[4];
DynamicObjectCfg playerDynConfig[4];
ObjectCfg playerObject[4];
PFbyte playerObjectId[4]={0,0,0,0};

//All structures required for enemy1 game object
#define ENEMY_BOTTOM_RECT_X 40
#define ENEMY_BOTTOM_RECT_Y 80
RectangleProperties enemy[4];
RectangleProperties enemyPrevConfig[4];
DynamicObjectCfg enemyDynConfig[4];
ObjectCfg enemyObject[4];
PFbyte enemyObjectId[4]={0,0,0,0};

//All structures required for enemy2 game object
#define ENEMY_BOTTOM_RECT1_X 40
#define ENEMY_BOTTOM_RECT1_Y 80
RectangleProperties enemy1[4];
RectangleProperties enemyPrevConfig1[4];
DynamicObjectCfg enemyDynConfig1[4];
ObjectCfg enemyObject1[4];
PFbyte enemyObjectId1[4]={0,0,0,0};


//For road:
//Properties structure for object line
LineProperties lineObjectConfig[10];
ObjectCfg lineObject[10];
PFbyte lineObjectId[10]={0,0,0,0,0,0,0,0,0,0};
LineProperties lineObjectPrevConfig[10];
DynamicObjectCfg lineObjectDynConfig[10];

//Arrays that are used to create enemy gameObjects
//For enemy 1
PFword level1x[4]={ENEMY_BOTTOM_RECT_X,ENEMY_BOTTOM_RECT_X,ENEMY_BOTTOM_RECT_X+12,ENEMY_BOTTOM_RECT_X};
PFword level1y[4]={ENEMY_BOTTOM_RECT_Y,ENEMY_BOTTOM_RECT_Y-20,ENEMY_BOTTOM_RECT_Y-30,ENEMY_BOTTOM_RECT_Y-30};
//For enemy 2
PFword level2x[4]={ENEMY_BOTTOM_RECT1_X,ENEMY_BOTTOM_RECT1_X,ENEMY_BOTTOM_RECT1_X+12,ENEMY_BOTTOM_RECT1_X};
PFword level2y[4]={ENEMY_BOTTOM_RECT1_Y,ENEMY_BOTTOM_RECT1_Y-20,ENEMY_BOTTOM_RECT1_Y-30,ENEMY_BOTTOM_RECT1_Y-30};

//This is the main function that runs when the game reaches at playfield part
void playCallback();

//This callback is called whenever a touch is detected in the help region.
void helpCallback(void)
{
	setWindow(helpWindowId);

	//Value updated after setting the window
	current_window=helpWindowId;

	//Description of help window
	gfxDrawString(15,110,"Control the moving car to",enGfxFont_8X16,BLACK,WHITE);
	gfxDrawString(15,125,"avoid the collision",enGfxFont_8X16,BLACK,WHITE);
	gfxDrawString(15,150,"Controls:",enGfxFont_8X16,BLACK,WHITE);
	gfxDrawString(15,170,"Key 3: Start the car",enGfxFont_8X16,BLACK,WHITE);
	gfxDrawString(15,190,"Key 2: Move car to left",enGfxFont_8X16,BLACK,WHITE);
	gfxDrawString(15,210,"Key 4: Move car to right",enGfxFont_8X16,BLACK,WHITE);
	gfxDrawString(15,230,"Key 5: Quit the game",enGfxFont_8X16,BLACK,WHITE);
}

//This callback is called whenever a touch is detected in the back region.
void backCallback(void)
{
	setWindow(mainWindowId);

	current_window=mainWindowId;
}

//This callback is called whenever a touch is detected in the setting region.
void settingCallback(void)
{
	setWindow(settingWindowId);

	current_window=settingWindowId;
}

//This callback is called whenever a touch is detected in the keypad region.
void keypadCallback(void)
{
	mode=0;

	setWindow(mainWindowId);
	current_window=mainWindowId;
}

//This callback is called whenever a touch is detected in the accelerometer region.
void accelerometerCallback(void)
{
	mode=1;

	setWindow(mainWindowId);
	current_window=mainWindowId;
}

//canvas for title name "C A R S"
CanvasCfg titleCanvas =
{
   //Attributes of canvas
   {
    (char *)"Game name",		// Name of a Window, Canvas or a Widget
    {65,30},					// Top Left Coordinate of a Window, Canvas or a Widget
    {130,50},					// Width and Height of a Window, Canvas or a Widget
    WHITE,						// Background color of a Window, Canvas or a Widget
    enBooleanFalse				// To draw border for a Window, Canvas or a Widget
   },
   0,							//To be called every time there is an event with in the Canvas region
   "C A R S",					//String to be displayed on the the Canvas
   enGfxFont_16X24,				//Font of the text
   RED,							//FontColor of the text
   0							//Image to be displayed for a Canvas
};

// canvas for Description of window "Main menu"
CanvasCfg descCanvas =
{
   {
    (char *)"Window description",
    {80,70},
    {80,30},
    WHITE,
    enBooleanFalse
   },
   0,
   " Main Menu",
   enGfxFont_8X16,
   RED,
   0
};

//Window configuration structure for GUI
static WindowCfg mainWindow =
{
  {
    (char *)"GUI",
    {10,10},
    {219, 299},
    WHITE,
    enBooleanTrue
  }
};

//Window configuration structure for help
static WindowCfg helpWindow=
{
		{
				(char *)"HELP",
				    {10,10},
				    {219, 299},
				    WHITE,
				    enBooleanFalse
		}
};

//Window configuration structure for play
static WindowCfg playWindow=
{
		{
				(char *)"PLAY",
				    {30,0},
				    {180, 320},
				    BLACK,
				    enBooleanFalse
		}
};

//Window configuration structure for settings
static WindowCfg settingWindow=
{
		{
				(char *)"SETTING",
				    {10,10},
				    {219, 299},
				    WHITE,
				    enBooleanFalse
		}
};

//play widget structure
static WidgetCfg playWidget=
{
		{
				"Play",
				{80,120},
				{80,40},
				RED,
				enBooleanFalse
		},
		playCallback,
		"   Play",
		enGfxFont_8X16,
		WHITE,
		0
};

//help widget structure
static WidgetCfg helpWidget=
{
		{
				"Help",
				{80,190},
				{80,40},
				RED,
				enBooleanFalse
		},
		helpCallback,
		"   Help",
		enGfxFont_8X16,
		WHITE,
		0
};

//setting widget structure
static WidgetCfg settingWidget=
{
		{
				"Setting",
				{80,260},
				{80,40},
				RED,
				enBooleanFalse
		},
		settingCallback,
		" Settings",
		enGfxFont_8X16,
		WHITE,
		0
};

//back Widget structure
static WidgetCfg backWidget=
{
		{
				"Back",
				{140,265},
				{80,40},
				RED,
				enBooleanFalse
		},
		backCallback,
		"   Back",
		enGfxFont_8X16,
		WHITE,
		0
};

// canvas for Description of window "Help"
CanvasCfg helpCanvas =
{
   {
    (char *)"Window description",
    {95,70},
    {60,25},
    WHITE,
    enBooleanFalse
   },
   0,
   " Help",
   enGfxFont_8X16,
   RED,
   0
};

// canvas for Description of window "Setting"
CanvasCfg settingCanvas =
{
   {
    (char *)"Window description",
    {90,70},
    {80,25},
    WHITE,
    enBooleanFalse
   },
   0,
   "Controls",
   enGfxFont_8X16,
   RED,
   0
};


//keypad widget structure
static WidgetCfg keypadWidget=
{
		{
				"Keypad",
				{60,140},
				{120,40},
				RED,
				enBooleanFalse
		},
		keypadCallback,
		"    Keypad",
		enGfxFont_8X16,
		WHITE,
		0
};


//accelerometer widget structure
static WidgetCfg accelerometerWidget=
{
		{
				"Accelerometer",
				{60,210},
				{120,40},
				RED,
				enBooleanFalse
		},
		accelerometerCallback,
		" Accelerometer",
		enGfxFont_8X16,
		WHITE,
		0
};


//This function creates the player gameObject
void createPlayer()
{
	PFword level1x[4]={BOTTOM_RECT_X,BOTTOM_RECT_X,BOTTOM_RECT_X+12,BOTTOM_RECT_X};
	PFword level1y[4]={BOTTOM_RECT_Y,BOTTOM_RECT_Y-20,BOTTOM_RECT_Y-30,BOTTOM_RECT_Y-30};
	PFword i=0;
	for(i=0;i<4;i++)
	{
		player[i].topLeft.xValue=level1x[i];
		player[i].topLeft.yValue=level1y[i];
		if(i<2)
		{
			player[i].size.height=RECT_HEIGHT;
			player[i].size.width=RECT_WIDTH;
		}
		else if(i==2)
		{
			player[i].size.height=RECT_WIDTH;
			player[i].size.width=RECT_HEIGHT;
		}
		else
		{
			player[i].size.height=40;
			player[i].size.width=RECT_WIDTH;
		}

		playerDynConfig[i].prevObjProperties=&playerPrevConfig[i];
		playerDynConfig[i].speed=10;
		playerDynConfig[i].direction=enSouth;

		playerObject[i].name=(PFchar *)"Car object";
		playerObject[i].objShape=enRectangle;
		playerObject[i].objProperties=&player[i];
		if(i<3)
		{
			playerObject[i].color= WHITE;
			playerObject[i].colorFill=enBooleanTrue;
			playerObject[i].visible=enStateVisible;
		}
		else
		{
			playerObject[i].color= YELLOW;
			playerObject[i].colorFill=enBooleanFalse;
			playerObject[i].visible=enStateInvisible;
		}
		playerObject[i].type=enDynamic;
		playerObject[i].dynamicCfg=&playerDynConfig[i];


		createObject(&playerObjectId[i],&playerObject[i]);
	}
	drawAllObjects();
	renderFrame();
}

//This function creates enemy 1
void createEnemy(int side)
{


	PFword i=0;

	for(i=0;i<4;i++)
	{

		if(side==1)
			enemy[i].topLeft.xValue=level1x[i];
		else if(side==2)
		{
			enemy[i].topLeft.xValue=level1x[i]+60;
		}
		else if(side==3)
			enemy[i].topLeft.xValue=level1x[i]+120;

		enemy[i].topLeft.yValue=level1y[i];

		if(i<2)
		{
			enemy[i].size.height=RECT_HEIGHT;
			enemy[i].size.width=RECT_WIDTH;
		}
		else if(i==2)
		{
			enemy[i].size.height=RECT_WIDTH;
			enemy[i].size.width=RECT_HEIGHT;
		}
		else
		{
			enemy[i].size.height=40;
			enemy[i].size.width=RECT_WIDTH;
		}


		enemyDynConfig[i].prevObjProperties=&enemyPrevConfig[i];
		enemyDynConfig[i].speed=4;
		enemyDynConfig[i].direction=enSouth;

		enemyObject[i].name=(PFchar *)"Car object";
		enemyObject[i].objShape=enRectangle;
		enemyObject[i].objProperties=&enemy[i];
		if(i<3)
		{
			enemyObject[i].color=RED;
			enemyObject[i].colorFill=enBooleanTrue;
			enemyObject[i].visible=enStateVisible;
		}
		else
		{
			enemyObject[i].color= GREEN;
			enemyObject[i].colorFill=enBooleanFalse;
			enemyObject[i].visible=enStateInvisible;
		}
		enemyObject[i].type=enDynamic;
		enemyObject[i].dynamicCfg=&enemyDynConfig[i];


		createObject(&enemyObjectId[i],&enemyObject[i]);
	}
}

//This function moves enemy 1 down
void moveEnemy(int s)
{
	int i;
	for(i=0;i<4;i++)
	{
		if(s==0)
		enemy[i].topLeft.yValue+=5;
		else if(s==1)
			enemy[i].topLeft.yValue+=3;

		if(enemy[2].topLeft.yValue==320)
			{
				destroyObject(enemyObjectId[i]);

			}

	}
	while(lastFrameRendered()==enBooleanFalse)
		for(i=0;i<4;i++)
			updateObject(enemyObjectId[i]);
	renderFrame();


}

//This function creates enemy 2
void createEnemy1(int side)
{


	PFword i=0;

	for(i=0;i<4;i++)
	{
		if(side==1)
			enemy1[i].topLeft.xValue=level2x[i];
		else if(side==2){
			enemy1[i].topLeft.xValue=level2x[i]+60;

		}
		else if(side==3)
			enemy1[i].topLeft.xValue=level2x[i]+120;
		enemy1[i].topLeft.yValue=level2y[i];


		if(i<2)
		{
			enemy1[i].size.height=RECT_HEIGHT;
			enemy1[i].size.width=RECT_WIDTH;
		}
		else if(i==2)
		{
			enemy1[i].size.height=RECT_WIDTH;
			enemy1[i].size.width=RECT_HEIGHT;
		}
		else
		{
			enemy1[i].size.height=40;
			enemy1[i].size.width=RECT_WIDTH;
		}

		enemyDynConfig1[i].prevObjProperties=&enemyPrevConfig1[i];
		enemyDynConfig1[i].speed=4;
		enemyDynConfig1[i].direction=enSouth;

		enemyObject1[i].name=(PFchar *)"Car object";
		enemyObject1[i].objShape=enRectangle;
		enemyObject1[i].objProperties=&enemy1[i];
		if(i<3)
		{
			enemyObject1[i].color=GREEN;
			enemyObject1[i].colorFill=enBooleanTrue;
			enemyObject1[i].visible=enStateVisible;
		}
		else
		{
			enemyObject1[i].color= GREEN;
			enemyObject1[i].colorFill=enBooleanFalse;
			enemyObject1[i].visible=enStateInvisible;
		}
		enemyObject1[i].type=enDynamic;
		enemyObject1[i].dynamicCfg=&enemyDynConfig1[i];


		createObject(&enemyObjectId1[i],&enemyObject1[i]);
	}
}

//This function moves enemy 2
void moveEnemy1(int s)
{
	int i;
	for(i=0;i<4;i++)
	{
		if(s==0)
		enemy1[i].topLeft.yValue+=7;
		else if(s==1)
			enemy1[i].topLeft.yValue+=5;

		if(enemy1[2].topLeft.yValue==320)
			{
				destroyObject(enemyObjectId1[i]);
			}

	}
	while(lastFrameRendered()==enBooleanFalse)
		for(i=0;i<4;i++)
			updateObject(enemyObjectId1[i]);
	renderFrame();

}


/*This function creates road.
  It uses the same logic as above gameObjects like player and enemy, by using
  structures to store coordinates and other properties */
void createRoad()
{
	/*To create a line we need 2 points hence
	  point1x stores x1 of point 1 and point1y stores y1 of point 1.
	  point2x stores x2 of point 2 and point2y stores y2 of point 2.
	  Using these 2 points the renderer can now make a line.
	  */
	PFword point1x[10]={90,150,90,150,90,150,90,150,90,150};
	PFword point1y[10]={20,20,90,90,150,150,210,210,270,270};
	PFword point2x[10]={90,150,90,150,90,150,90,150,90,150};
	PFword point2y[10]={60,60,130,130,190,190,250,250,310,310};

	/*now i have created 6 lines using the for loop*/
	for(i=0;i<6;i++)
		{
			lineObjectDynConfig[i].prevObjProperties=&lineObjectPrevConfig[i];
			lineObjectDynConfig[i].speed=4;
			lineObjectDynConfig[i].direction=enSouth;

			lineObjectConfig[i].point1.xValue=point1x[i];
			lineObjectConfig[i].point1.yValue=point1y[i];

			lineObjectConfig[i].point2.xValue=point2x[i];
			lineObjectConfig[i].point2.yValue=point2y[i];

			lineObject[i].name=(PFchar*)"Line object";
			lineObject[i].objShape=enLine;
			lineObject[i].objProperties=&lineObjectConfig[i];
			lineObject[i].color=WHITE;
			lineObject[i].colorFill=enBooleanTrue;
			lineObject[i].visible=enStateVisible;
			lineObject[i].type=enDynamic;
			lineObject[i].dynamicCfg=&lineObjectDynConfig[i];

		createObject(&lineObjectId[i],&lineObject[i]);
	}

	drawAllObjects();
	renderFrame();

}

/*This function is used to move the road.
  i did this by incrementing the cordinates of each line by 1 in the game loop &
  i think the problem is somewhere here in moveRoad() & createRoad() functions.
  i tried to create less lines and viceversa but it didn't work.*/
void moveRoad(int n)
{

	int i=0;
	for(i=0;i<6;i++)
	{


		if(n==0){
			lineObjectConfig[i].point1.yValue+=8;
			lineObjectConfig[i].point2.yValue+=8;


			//here everytime the point 1 y1 value reaches the bottom of the screen i reset the point 1 cordinates.
			if(lineObjectConfig[i].point1.yValue==320)
			{
				lineObjectConfig[i].point1.yValue=0;
				lineObjectConfig[i].point2.yValue=40;
			}

		}
		else if(n==1)
		{
			lineObjectConfig[i].point1.yValue+=10;
			lineObjectConfig[i].point2.yValue+=10;

			if(lineObjectConfig[i].point1.yValue==320)
			{
				lineObjectConfig[i].point1.yValue=0;
				lineObjectConfig[i].point2.yValue=40;
			}

		}
	}
	while(lastFrameRendered()==enBooleanFalse)
		for(i=0;i<6;i++){
			updateObject(lineObjectId[i]);
		}
	renderFrame();

	//This small part is to display score and highscore
	s++;
	sprintf(score,"%d",s);

	sprintf(highScore,"%d",hs);

	gfxDrawString(1,1,"Score: ", enGfxFont_8X16,WHITE,BLACK);
	gfxDrawString(1,20,score, enGfxFont_8X16,WHITE,BLACK);
	gfxDrawString(1,40,"HS: ", enGfxFont_8X16,WHITE,BLACK);
	gfxDrawString(1,60,highScore, enGfxFont_8X16,WHITE,BLACK);

}

//This function is used to detect collision.
int rectangleCollideRectangle(int rect1Id,int rect2Id)
{
	RectangleProperties *rectangleObject1;
	RectangleProperties *rectangleObject2;

	pObjectCfg rect1Cfg = getObject(rect1Id);
	pObjectCfg rect2Cfg = getObject(rect2Id);

	rectangleObject1 = (RectangleProperties *) rect1Cfg->objProperties;
	rectangleObject2 = (RectangleProperties *) rect2Cfg->objProperties;

	if(		   rectangleObject1->topLeft.xValue			    < rectangleObject2->topLeft.xValue+RECT_WIDTH
			&& rectangleObject1->topLeft.xValue+RECT_WIDTH  > rectangleObject2->topLeft.xValue
			&& rectangleObject1->topLeft.yValue			    < rectangleObject2->topLeft.yValue+40
			&& rectangleObject1->topLeft.yValue+40		    > rectangleObject2->topLeft.yValue)
	{
		playBuzzer(40);
		return 1;
	}
	else
	{
		return 0;
	}
}

//This function contains the game loop
void playGame()
{
	int bool,bool1;
	int i,pick_side,number,pick_side1;
	PFchar accY=0;



	for(i=0;i<4;i++)
	{
		enemy[i].topLeft.yValue=level1y[i];
		enemy1[i].topLeft.yValue=level2y[i];
	}
	count=0;
	setBackgroundColor(BLACK);
	createPlayer();
	createRoad();


	//Game loop
	while(1)
	{
		//end lines used to display the end of left and right part of the road
		gfxDrawLine(30,0,30,320);
		gfxDrawLine(210,0,210,320);

		/*This little part is used to spawn the enemies randomly at any lane.
		I did the random spawning part using the rand() function provided by C.
		Whenever counter reaches 0, a new set of enemies are spawned.
		number variable is used to decide how many enemies to spawn. If 1 then pick the side of that
		enemy and create enemy.
		If 2 then pick sides for both of them and create enemy.
		*/
			if(count==0)
			{
				renderFrame();
				number=(rand()%2+1);
				if(number==1)
				{
					pick_side=(rand()%3)+1;
					createEnemy(pick_side);
				}
				if(number==2)
				{
					pick_side=(rand()%3)+1;
					pick_side1=(rand()%3)+1;

					if(pick_side!=pick_side1)
					{
						createEnemy(pick_side);
						createEnemy1(pick_side1);
					}
				}
			}
			//Move enemy down by 1 every single loop
			moveEnemy(0);						//0 because no key is pressed
			moveEnemy1(0);
			renderFrame();

			//Check for collision in every loop
			bool=rectangleCollideRectangle(enemyObjectId[3],playerObjectId[3]);
			bool1=rectangleCollideRectangle(enemyObjectId1[3],playerObjectId[3]);
			if(bool==1 || bool1==1)
			{
				break;
			}
			/*mode is used to determine how the user wants to give his input,
			  i.e via keypad or accelerometer if 0 then keypad if 1 then accelerometer*/
			if(mode==0)
			{
			if(keypadGetKeyCode(&switchNumber)==enBooleanTrue)
			{
				switch(switchNumber)
				{
				//If key 2 is pressed then move left if key 4 is pressed then move right.
				case KEY_2:
					moveRoad(1);
					moveEnemy(1);
					moveEnemy1(1);
					if(player[0].topLeft.xValue>40)
					{
						for(i=0;i<4;i++){
							player[i].topLeft.xValue-=playerDynConfig[i].speed;
						}
						while(lastFrameRendered()==enBooleanFalse)
							for(i=0;i<4;i++)
							updateObject(playerObjectId[i]);
							bool=rectangleCollideRectangle(enemyObjectId[3],playerObjectId[3]);
							bool1=rectangleCollideRectangle(enemyObjectId1[3],playerObjectId[3]);
							if(bool==1 || bool1==1)
							{
								break;
							}

							renderFrame();
							playBuzzer(2);

					}
					break;
				case KEY_4:
					moveRoad(1);
					moveEnemy(1);
					moveEnemy1(1);
					if(player[0].topLeft.xValue<165)
					{
						for(i=0;i<4;i++){
						player[i].topLeft.xValue+=playerDynConfig[i].speed;
						}

						while(lastFrameRendered()==enBooleanFalse)
							for(i=0;i<4;i++)
							updateObject(playerObjectId[i]);
							bool=rectangleCollideRectangle(enemyObjectId[3],playerObjectId[3]);
							bool1=rectangleCollideRectangle(enemyObjectId1[3],playerObjectId[3]);
							if(bool==1 || bool1==1)
							{
								break;
							}

						renderFrame();
						playBuzzer(2);

					}
					break;
				}
			}
	}
			if(mode==1)
			{
				  mma7660ReadY(&accY);
							    if(accY<-1)
							    {
									moveRoad(1);
									moveEnemy(1);
									moveEnemy1(1);
									if(player[0].topLeft.xValue>40)
									{
										for(i=0;i<4;i++){
											player[i].topLeft.xValue-=playerDynConfig[i].speed;
										}
										while(lastFrameRendered()==enBooleanFalse)
											for(i=0;i<4;i++)
											updateObject(playerObjectId[i]);
											bool=rectangleCollideRectangle(enemyObjectId[3],playerObjectId[3]);
											bool1=rectangleCollideRectangle(enemyObjectId1[3],playerObjectId[3]);
											if(bool==1 || bool1==1)
											{
												break;
											}

											renderFrame();
											playBuzzer(2);

									}
							    }
							    else if(accY>1)
							    {
									moveRoad(1);
									moveEnemy(1);
									moveEnemy1(1);
									if(player[0].topLeft.xValue<165)
									{
										for(i=0;i<4;i++){
										player[i].topLeft.xValue+=playerDynConfig[i].speed;
										}

										while(lastFrameRendered()==enBooleanFalse)
											for(i=0;i<4;i++)
											updateObject(playerObjectId[i]);
											bool=rectangleCollideRectangle(enemyObjectId[3],playerObjectId[3]);
											bool1=rectangleCollideRectangle(enemyObjectId1[3],playerObjectId[3]);
											if(bool==1 || bool1==1)
											{
												break;
											}

										renderFrame();
										playBuzzer(2);

									}
			}
			}
					if(count<50)
					{
						count++;
					}
					else if(count==50)
					{
						count=0;
					}

						moveRoad(0);

					renderFrame();

				}
}

//This function runs after player collides with the enemy. function call occurs in playCallback().
int gameOver()
{

	/*Game Over function displays the game over message when player collides with enemy and then
	  waits for input from user when 1 is pressed it exits the game and displays a msg.
	  when key 5 is pressed it resets the game & updates the highsore.
	  ALSO THERE IS ONE MORE PROBLEM HERE THAT THE GAME OVER MSG IS DISPLAYED ONLY THE FIRST TIME THE PLAYER COLLIDES WITH THE ENEMY OBJECT BUT 2ND TIME AFTER RESETTING THE GAME IT DOES NOT DISPLAY THE GAMEOVER MSG BUT WAITS FOR THE INPUT. WHY IS THIS HAPPENING?*/
	setBackgroundColor(BLACK);
	for(int i=0;i<3;i++)
	{
	gfxDrawString(50,120,"GAME OVER", enGfxFont_16X24,RED,BLACK);
	gfxDrawString(50,170,"Press 5 to restart",enGfxFont_8X16,WHITE,BLACK);
	gfxDrawString(50,190,"Press 1 to Quit",enGfxFont_8X16,WHITE,BLACK);
	renderFrame();
	}
	while(1)
	{
		if(keypadGetKeyCode(&switchNumber)==enBooleanTrue)
		{
			switch(switchNumber){
			case KEY_5:
			{
				setBackgroundColor(BLACK);
				if(s>hs)
				{
					hs=s;
				}
				s=0;
				playGame();
				b=0;
				break;
			}
			case KEY_1:
				setBackgroundColor(BLACK);
				gfxDrawString(0,80,"  Thanks for",enGfxFont_16X24,WHITE,BLACK);
				gfxDrawString(0,110,"   playing!",enGfxFont_16X24,WHITE,BLACK);
				gfxDrawString(60,150,"   made by:",enGfxFont_8X16,WHITE,BLACK);
				gfxDrawString(30,170,"    JASMEET SINGH",enGfxFont_8X16,WHITE,BLACK);
				renderFrame();
				b=1;
				break;
			}
		}
		if(b==1){break;}
	}
	return 0;
}

//This function is called whenever a touch is detected in the play widget region.
void playCallback(void)
{
	/*This function will start the game by setting a new window for playfield and waits for user to press 3 to start playing.*/
	setWindow(playWindowId);

	setBackgroundColor(BLACK);
	current_window=playWindowId;


	setBackgroundColor(BLACK);
	createPlayer();
	createRoad();


	gfxDrawString(1,CENTER_X,"   Press 3 to start playing",enGfxFont_8X16,RED,BLACK);
	while(1)
		{
			if(keypadGetKeyCode(&switchNumber)==enBooleanTrue)
			{
				if(switchNumber==KEY_3)
				{
					for(i=0;i<10;i++)
					{
						destroyObject(playerObjectId[i]);
						destroyObject(lineObjectId[i]);
					}
					renderFrame();
					break;
				}
			}
		}
	/*playgame function contains gameloop and whenever the player coolides the loop breaks and control reaches here and executes the gameover() function.*/
		playGame();

		/*I displayed this msg again here to see weather this "game over msg is displayed on the screen 2nd time but no it is not printing."*/
		setBackgroundColor(BLACK);
		gfxDrawString(50,120,"GAME OVER", enGfxFont_16X24,RED,BLACK);
		gfxDrawString(50,170,"Press 5 to restart",enGfxFont_8X16,WHITE,BLACK);
		gfxDrawString(50,190,"Press 1 to Quit",enGfxFont_8X16,WHITE,BLACK);

		gameOver();

		/*After executing gameOver() function if the user pressed key 1 it will display this msg*/
		if(b==1)
		{
		setBackgroundColor(BLACK);
		gfxDrawString(0,80,"  Thanks for",enGfxFont_16X24,WHITE,BLACK);
		gfxDrawString(0,110,"   playing!",enGfxFont_16X24,WHITE,BLACK);
		gfxDrawString(60,150,"   made by:",enGfxFont_8X16,WHITE,BLACK);
		gfxDrawString(30,170,"    JASMEET SINGH",enGfxFont_8X16,WHITE,BLACK);
		renderFrame();
		}
}

/*main function is basically used to create widgets, windows & canvas and also to detect touches.*/
int main()
{
   PFEnStatus status;
   appInit();
   status = gameEngineInit();

   status=createWindow(&mainWindowId,&mainWindow);
   if(status!=enStatusSuccess)
	   DEBUG_WRITE("ERROR\n");
   //Attributes of main window
   createCanvas(mainWindowId, &titleCanvasId,   &titleCanvas  );
   createCanvas(mainWindowId, &descCanvasId,    &descCanvas   );
   createWidget(mainWindowId, &playWidgetId,    &playWidget   );
   createWidget(mainWindowId, &helpWidgetId,    &helpWidget   );
   createWidget(mainWindowId, &settingWidgetId, &settingWidget);


   status=createWindow(&helpWindowId,&helpWindow);
   if(status!=enStatusSuccess)
   	   DEBUG_WRITE("ERROR\n");
   //Attributes of help window
   createWidget(helpWindowId, &backWidgetId,  &backWidget );
   createCanvas(helpWindowId, &titleCanvasId, &titleCanvas);
   createCanvas(helpWindowId, &helpCanvasId,  &helpCanvas );

   status=createWindow(&playWindowId,&playWindow);
   if(status!=enStatusSuccess)
	   DEBUG_WRITE("ERROR\n");

   status=createWindow(&settingWindowId,&settingWindow);
   if(status!=enStatusSuccess)
   	   DEBUG_WRITE("ERROR\n");
   createCanvas(settingWindowId, &settingCanvasId,       &settingCanvas      );
   createWidget(settingWindowId, &keypadWidgetId,        &keypadWidget       );
   createWidget(settingWindowId, &accelerometerWidgetId, &accelerometerWidget);



   setWindow(mainWindowId);
   current_window=mainWindowId;

   while(1)
   	{
   		if(touchAvailable(&x,&y) == enBooleanTrue)
   		{
   			if(current_window==mainWindowId)
   			{
   				windowEventHandler(mainWindowId, x,y);
   			}										//Passing touch coordinated to the function
   			else if(current_window==helpWindowId)
   			{										//to detect in which region the touch coordinates lies
   				windowEventHandler(helpWindowId, x,y);
   			}
   			else if(current_window==settingWindowId)
   			{
   				windowEventHandler(settingWindowId, x,y);
   			}
   		}
   	}

   return 0;
}
