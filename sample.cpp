#include "const.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <iostream>
#include <vector>
#include <math.h>
#ifdef WIN32
#include <windows.h>
#pragma warning(disable:4996)
#include "glew.h"
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include "glut.h"
#include <glm/glm.hpp>
#include "loader.h"
#include <math.h>       /* cos */
#include <fstream>
#include <time.h>       /* time */
#include "glslprogram.h"
#include <cstring>
#include <string>
#include <iostream>
#include <al.h>
#include <alc.h>
#include <alut.h>

#include "Map.h"
#include "AI.h"
#include "SimpleAI.h"
#include "neuron.h"

ALCdevice *device;
ALboolean enumeration;
ALCcontext *context;
ALuint mainMusic;
ALuint tankShellFire;
ALuint tankShellBounce;
ALuint tankExplode;
ALuint hpRegen;
ALuint AmmoSmoke;

ALuint Buffers[NUM_BUFFERS];
ALuint Sources[NUM_SOURCES];

// Position of the source sounds.
ALfloat SourcesPos[NUM_SOURCES][3];

// Velocity of the source sounds.
ALfloat SourcesVel[NUM_SOURCES][3];


// Position of the listener.
ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };

// Velocity of the listener.
ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };

// Orientation of the listener. (first 3 elements are "at", second 3 are "up")
ALfloat ListenerOri[] = { 0.0, 0.0, -1.0, 0.0, 1.0, 0.0 };

ALsizei size, freq;
ALenum format;
ALvoid *data;
ALboolean loop = AL_FALSE;

const char *WINDOWTITLE = { "Tanks 2017" };
const char *GLUITITLE = { "Tanks" };
const int GLUITRUE = { true };
const int INIT_WINDOW_SIZE = { 600 };
const float BOXSIZE = { 2.f };
const float ANGFACT = { 1. };
const float SCLFACT = { 0.005f };
const float MINSCALE = { 0.05f };
const int LEFT = { 4 };
const int MIDDLE = { 2 };
const int RIGHT = { 1 };
enum Projections
{
	ORTHO,
	PERSP
};
enum ButtonVals
{
	RESET,
	QUIT
};
const GLfloat BACKCOLOR[] = { 0., 0., 0., 1. };
const GLfloat AXES_WIDTH = { 3. };
enum Colors
{
	RED,
	YELLOW,
	GREEN,
	CYAN,
	BLUE,
	MAGENTA,
	WHITE,
	BLACK
};
char * ColorNames[] = {
	(char *)"Red",
	(char *)"Yellow",
	(char *)"Green",
	(char *)"Cyan",
	(char *)"Blue",
	(char *)"Magenta",
	(char *)"White",
	(char *)"Black"
};
const GLfloat Colors[][3] = {
	{ 1., 0., 0. },		// red
	{ 1., 1., 0. },		// yellow
	{ 0., 1., 0. },		// green
	{ 0., 1., 1. },		// cyan
	{ 0., 0., 1. },		// blue
	{ 1., 0., 1. },		// magenta
	{ 1., 1., 1. },		// white
	{ 0., 0., 0. },		// black
};
const GLfloat FOGCOLOR[4] = { .0, .0, .0, 1. };
const GLenum  FOGMODE = { GL_LINEAR };
const GLfloat FOGDENSITY = { 0.30f };
const GLfloat FOGSTART = { 1.5 };
const GLfloat FOGEND = { 4. };
int		ActiveButton;			// current button that is down
GLuint	AxesList;				// list to hold the axes
int		AxesOn;					// != 0 means to draw the axes
int		DebugOn;				// != 0 means to print debugging info
int		DepthCueOn;				// != 0 means to use intensity depth cueing
int		DepthBufferOn;			// != 0 means to use the z-buffer
int		DepthFightingOn;		// != 0 means to use the z-buffer
GLuint	BoxList;				// object display list
int		MainWindow;				// window id for main graphics window
float	Scale;					// scaling factor
int		WhichColor;				// index into Colors[ ]
int		WhichProjection;		// ORTHO or PERSP
int		Xmouse, Ymouse;			// mouse values
float	Xrot, Yrot;				// rotation angles in degrees
bool freeze = false;
float Time;

float factor = 0;
float eyex = CAMX;
float eyey = CAMY;
float eyez = 0;
float targetx = 0;
float targety = 0;
float targetz = 0;
float upx = 1;
float upy = 0;
float upz = 0;
int camState = 0;
bool res;

bool resArray[NUMMODEL];
std::vector< glm::vec3 > vertices;
std::vector< glm::vec2 > uvs;
std::vector< glm::vec3 > normals; // Won't be used at the moment.
std::vector< glm::vec3 > verticesArray[NUMMODEL];
std::vector< glm::vec2 > uvsArray[NUMMODEL];
std::vector< glm::vec3 > normalsArray[NUMMODEL]; // Won't be used at the moment.
GLuint VertexVBOID = 0;
GLuint ModelIDList[NUMMODEL];
int Abram[2][2];
int IS3[3][2];
int Track[2][2];
int cube[2];
int trees[8][2];
int shell[2];
int ammo[2];
int smokeCrate[2][2];
int hpCrate[2][2];
float smokeBeginTime = 0;
bool smokeSet = false;
float smokeIDBuffer[1000];
float smokeCoordBuffer[1000][2];
float smokeDurBuffer[1000];
float smokeAngleBuffer[1000];
bool  smokeIDBufferSet[1000];
bool  smokeActive[1000];
int smokeIndex = 0;
float destructionTimeBuffer[24][14];

float TANKSPEED = 0.4;

//explosion and fire
bool shake = false;
bool shakeOnce = false;
float shakeDuration = 0.005;
float shakeStartTime = 0;
bool fireAbram = false;
bool fireIS3 = false;

float AbramLastShot = 0;
float IS3LastShot = 0;

int AbramShells = SHELLSTORAGE;
int IS3Shells = SHELLSTORAGE;

float AbramHP = TANKHP;
float IS3HP = TANKHP;

float AbramSmoke = SMOKECOUNT;
float IS3Smoke = SMOKECOUNT;

GLSLProgram *Pattern;
float AbramTurretAngle = 0;
float AbramHullAngle = 180;
float AbramInitCoord[2] = { 0,-SPAWN };
float IS3InitCoord[2] = { 0,SPAWN };
float AbramXY[2] = { AbramInitCoord[0],AbramInitCoord[1] };
float IS3XY[2] = { IS3InitCoord[0],IS3InitCoord[1] };
float IS3TurretAngle = 0;
float IS3HullAngle = 0;

float MoveTimeAbram = 0;
float MoveTimeIS3 = 0;
std::string mapName = " ";

bool keyBuffer[256];
char MapRaw[25 * 14];

struct Shell {
	float x;
	float y;
	float angle;
	float startTime;
	int shooterId;
	bool active;
};
struct Map myMap;
struct Shell Shells[SHELLMAX];
struct Crate {
	int type;
	float X;
	float Y;
	int i;
	int j;
	bool isActive;
};
// AI knowledge Base
struct AIKB {
	bool isAI = false;
	char AIID = NULL;
	float * playerHP = NULL;
	float * playerPos = NULL;
	int * playerAmmo = NULL;
	float * playerHullAngle = NULL;
	float * playerTurretAngle = NULL;
	float * AIHP = NULL;
	float * AIPos = NULL;
	int * AIAmmo = NULL;
	float * AIHullAngle = NULL;
	float * AITurretAngle = NULL;
	SimpleAI * agent = new SimpleAI(&myMap);
};
struct AIKB myAIKB;
Crate Crates[10];
int CrateIndex = 0;
int shellSize = 0;
void	Animate();
void	Display();
void	DoAxesMenu(int);
void	DoColorMenu(int);
void	DoDepthBufferMenu(int);
void	DoDepthFightingMenu(int);
void	DoDepthMenu(int);
void	DoCameraMenu(int);
void	DoDebugMenu(int);
void	DoMainMenu(int);
void	DoProjectMenu(int);
void	DoRasterString(float, float, float, char *);
void	DoStrokeString(float, float, float, float, char *);
float	ElapsedSeconds();
void	InitGraphics();
void	InitLists();
void	InitMenus();
void	Keyboard(unsigned char, int, int);
void	keyUp(unsigned char, int, int);
void	MouseButton(int, int, int, int);
void	MouseMotion(int, int);
void	Reset();
void	Resize(int, int);
void	Visibility(int);
void	Axes(float);
void	HsvRgb(float[3], float[3]);

int main(int argc, char *argv[])
{
	// turn on the glut package:
	// (do this before checking argc and argv since it might
	// pull some command line arguments out)
	if (argc > 1)
		mapName = argv[1];
	if (argc < 2)
	{
		std::cout << "Would you like an AI?(a/t/n)" << std::endl;
		std::string ans = "no";
		std::cin >> ans;
		switch(ans[0])
		{
		case 'a':
		case 'A':
		case 't':
		case 'T':
			myAIKB.isAI = true;
			myAIKB.AIID = ans[0];
			break;
		}
	}
	if (argc > 2)
	{
		myAIKB.isAI = true;
		myAIKB.AIID = argv[2][0];
	}
	if (myAIKB.isAI)
	{
		switch (myAIKB.AIID)
		{
		case 'A':
		case 'a':
			myAIKB.playerHP = &IS3HP;
			myAIKB.playerPos = IS3XY;
			myAIKB.playerAmmo = &IS3Shells;
			myAIKB.playerHullAngle = &IS3HullAngle;
			myAIKB.playerTurretAngle = &IS3TurretAngle;

			myAIKB.AIHP = &AbramHP;
			myAIKB.AIPos = AbramXY;
			myAIKB.AIAmmo = &AbramShells;
			myAIKB.AIHullAngle = &AbramHullAngle;
			myAIKB.AITurretAngle = &AbramTurretAngle;
			break;
		case 'T':
		case 't':
			myAIKB.playerHP = &AbramHP;
			myAIKB.playerPos = AbramXY;
			myAIKB.playerAmmo = &AbramShells;
			myAIKB.playerHullAngle = &AbramHullAngle;
			myAIKB.playerTurretAngle = &AbramTurretAngle;

			myAIKB.AIHP = &IS3HP;
			myAIKB.AIPos = IS3XY;
			myAIKB.AIAmmo = &IS3Shells;
			myAIKB.AIHullAngle = &IS3HullAngle;
			myAIKB.AITurretAngle = &IS3TurretAngle;
			break;
		}
		myAIKB.agent->env = (SimpleAI::InnerAIKB*)&myAIKB;
	}
	glutInit(&argc, argv);


	// setup all the graphics stuff:

	InitGraphics();


	// create the display structures that will not change:

	InitLists();


	// init all the global variables used by Display( ):
	// this will also post a redisplay

	Reset();


	// setup all the user interface stuff:

	InitMenus();


	// draw the scene once and wait for some interaction:
	// (this will never return)

	glutSetWindow(MainWindow);
	glutMainLoop();


	// this is here to make the compiler happy:
	for(int i = 0; i < NUMMODEL; i++)
		glDeleteBuffers(1, &ModelIDList[i]);
	//glDeleteBuffers(1, &VertexVBOID);
	alDeleteSources(1, &mainMusic);
	alDeleteSources(1, &tankShellFire);
	alDeleteSources(1, &tankShellBounce);
	alDeleteSources(1, &tankExplode);
	alDeleteBuffers(1, Buffers);

	if (myAIKB.isAI)
	{
		delete(myAIKB.agent);
		myAIKB.agent = NULL;
	}
	device = alcGetContextsDevice(context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
	return 0;
}
void Animate()
{
	// put animation stuff in here -- change some global variables
	// for Display( ) to find:
	int ms = glutGet(GLUT_ELAPSED_TIME);
	ms %= TOTAL_MS;
	Time = (float)ms/180000;
	if (Time == 0)
	{
		std::cout << "Time restarted" <<std::endl;
		shakeStartTime = 0;
		smokeBeginTime = 0;
		AbramLastShot = 0;
		IS3LastShot = 0;
	}
	// animate with time here:
	// force a call to Display( ) next time it is convenient:

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}
void loadMap()
{
	for (int i = 0; i < CRATECAP; i++)
	{
		Crates[i].isActive = false;
		myMap.isCrate[Crates[i].i][Crates[i].j] = false;
	}
	// Shell test
	/*	Shells[0].x = 0;
	Shells[0].y = 0;
	Shells[0].angle = 45;
	Shells[0].id = shellSize;
	Shells[0].startTime = 0.08;
	if (shellSize < 100)
	shellSize++;
	else
	shellSize = 0;
	*/
	
	srand(time(NULL));
	for (int j = 0; j < 14; j++)
	{
		for (int i = 0; i < 24; i++)
		{
			myMap.color[i][j][0] = 0;
			myMap.color[i][j][1] = 0;
			myMap.color[i][j][2] = 0;
			myMap.coord[i][j][0] = 0;
			myMap.coord[i][j][1] = 0;
			myMap.coord[i][j][2] = 0;
			myMap.MCM[i][j] = false;
			myMap.isSolid[i][j] = false;
			myMap.isCrate[i][j] = false;
			destructionTimeBuffer[i][j] = -1;
		}
	}
	std::cout << "Enter map directory: " << std::endl;
	if(mapName == " ")
		std::cin >> mapName;
	std::string folder = "Maps/";
	std::string ext = ".txt";
	switch (mapName[0])
	{
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		mapName = folder + mapName[0] + ext;
		break;
	case'r':
	case'R':
		mapName = "RANDOM";
		break;
	}
	std::cout << "Loading Map ..." << std::endl;
	if (mapName == "RANDOM")
	{
		int AX = 0;
		int AY = 0;
		int TX = 0;
		int TY = 0;

		while (AX == TX)
		{
			AX = rand() % 24;
			AY = rand() % 14;
			TX = rand() % 24;
			TY = rand() % 14;
		}

		for (int j = 0; j < 14; j++)
		{
			for (int i = 0; i < 24; i++)
			{
				char e = '_';
				if (i == AX && j == AY)
				{
					e = 'A';
				}
				if (i == TX && j == TY)
				{
					e = 'T';
				}
				if ((!(i == AX && j == AY)) && (!(i == TX && j == TY)))
				{
					int element = rand() % 16;
					switch (element)
					{
					case 0:
						e = '#';
						break;
					case 10:
					case 5:
					case 1:
						e = '$';
						break;
					}
				}
				MapRaw[j * 24 + i] = e;
			}
		}
	}
	else
	{
		std::ifstream mapFile(mapName.c_str());
		char tmp;
		int k = 0;
		while (mapFile.get(tmp))
		{
			MapRaw[k] = tmp;
			k++;
		}
		mapFile.close();
	}
	int rockCount = 0;
	int rowRockCount = 0;
	for (int j = 0; j < 14; j++)
	{
		rowRockCount = 0;
		for (int i = 0; i < 24; i++)
		{
			std::cout << MapRaw[j * 25 + i];
			if (MapRaw[j * 25 + i] == 'A')
			{
				AbramInitCoord[0] = AbramXY[0] = REFLECT*(-MAPEDGEX - CUBESIZE / 4 + j * CUBESIZE + 2);
				AbramInitCoord[1] = AbramXY[1] = -MAPEDGEY + i * CUBESIZE;
				if (i <= 12)
					AbramHullAngle = 180;
				else
					AbramHullAngle = 0;
			}
			if (MapRaw[j * 25 + i] == 'T')
			{
				IS3InitCoord[0] = IS3XY[0] = REFLECT*(-MAPEDGEX - CUBESIZE / 4 + j * CUBESIZE + 2);
				IS3InitCoord[1] = IS3XY[1] = -MAPEDGEY + i * CUBESIZE;
				if (i <= 12)
					IS3HullAngle = 180;
				else
					IS3HullAngle = 0;
			}
			if (MapRaw[j * 25 + i] == '#')
			{
				myMap.color[i][j][0] = 0.35;
				myMap.color[i][j][1] = 0.25;
				myMap.color[i][j][2] = 0.18;
				myMap.coord[i][j][0] = REFLECT*(-MAPEDGEX - CUBESIZE / 4 + j * CUBESIZE + 2);
				myMap.coord[i][j][1] = -MAPEDGEY + i * CUBESIZE;
				myMap.HP[i][j] = BRICKHP;
				myMap.MCM[i][j] = true;
				myMap.isSolid[i][j] = true;
			}
			if (MapRaw[j * 25 + i] == '+')
			{
				myMap.color[i][j][0] = 7;
				myMap.color[i][j][1] = 0.5;
				myMap.color[i][j][2] = 0;
				myMap.coord[i][j][0] = REFLECT*(-MAPEDGEX - CUBESIZE / 4 + j * CUBESIZE + 2);
				myMap.coord[i][j][1] = -MAPEDGEY + i * CUBESIZE;
				myMap.MCM[i][j] = true;
				myMap.isSolid[i][j] = true;
			}
			if (MapRaw[j * 25 + i] == '$')
			{
				if (rockCount < ROCKTHRESH)
				{
					int selector = rand() % (8 - rowRockCount);
					if (selector == 7)
					{
						if (rand() % 2 == 0)
							selector = rand() % 7;
					}
					myMap.color[i][j][0] = selector;
					myMap.color[i][j][1] = 0.5;
					myMap.color[i][j][2] = 0;
					myMap.coord[i][j][0] = REFLECT*(-MAPEDGEX - CUBESIZE / 4 + j * CUBESIZE + 2);
					myMap.coord[i][j][1] = -MAPEDGEY + i * CUBESIZE;
					myMap.angle[i][j] = rand() % 360;
					myMap.MCM[i][j] = true;
					myMap.isSolid[i][j] = myMap.color[i][j][0] == 7;
					if (myMap.color[i][j][0] == 7)
					{
						rockCount++;
						rowRockCount++;
					}
				}
				else
				{
					myMap.color[i][j][0] = rand() % 7;
					myMap.color[i][j][1] = 0.5;
					myMap.color[i][j][2] = 0;
					myMap.coord[i][j][0] = REFLECT*(-MAPEDGEX - CUBESIZE / 4 + j * CUBESIZE + 2);
					myMap.coord[i][j][1] = -MAPEDGEY + i * CUBESIZE;
					myMap.MCM[i][j] = true;
					myMap.isSolid[i][j] = myMap.color[i][j][0] == 7;
				}
			}
			if (MapRaw[j * 25 + i] == 'B')
			{
				myMap.color[i][j][0] = 0.50;
				myMap.color[i][j][1] = 0.50;
				myMap.color[i][j][2] = 0.00;
				myMap.coord[i][j][0] = REFLECT*(-MAPEDGEX - CUBESIZE / 4 + j * CUBESIZE + 2);
				myMap.coord[i][j][1] = -MAPEDGEY + i * CUBESIZE;
				myMap.MCM[i][j] = true;
				myMap.isSolid[i][j] = true;
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	for (int j = 0; j < 14; j++)
	{
		for (int i = 0; i < 24; i++)
		{
			std::cout << myMap.color[i][j][0] << ", ";
			std::cout << myMap.color[i][j][1] << ", ";
			std::cout << myMap.color[i][j][2] << ", ";
			std::cout << myMap.coord[i][j][0] << ", ";
			std::cout << myMap.coord[i][j][1] << ", ";
			std::cout << myMap.MCM[i][j];
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	for (int i = 0; i < 1000; i++)
	{
		smokeIDBufferSet[i] = false;
	}
}
void loadAll()
{
	std::cout << "Loading Abrams ..." << std::endl;

	Abram[0][START] = vertices.size();
	res = loadOBJ("models/abram-turret.obj", vertices, uvs, normals);
	Abram[0][END] = vertices.size();

	Abram[1][START] = vertices.size();
	res = loadOBJ("models/abram-hull.obj", vertices, uvs, normals);
	Abram[1][END] = vertices.size();

	IS3[0][START] = vertices.size();
	res = loadOBJ("models/IS-3-turret.obj", vertices, uvs, normals);
	IS3[0][END] = vertices.size();

	IS3[1][START] = vertices.size();
	res = loadOBJ("models/IS-3-upper-hull.obj", vertices, uvs, normals);
	IS3[1][END] = vertices.size();

	IS3[2][START] = vertices.size();
	res = loadOBJ("models/IS-3-lower-hull.obj", vertices, uvs, normals);
	IS3[2][END] = vertices.size();

	Track[0][START] = vertices.size();
	res = loadOBJ("models/l-track.obj", vertices, uvs, normals);
	Track[0][END] = vertices.size();

	Track[1][START] = vertices.size();
	res = loadOBJ("models/r-track.obj", vertices, uvs, normals);
	Track[1][END] = vertices.size();

	cube[START] = vertices.size();
	res = loadOBJ("models/cube.obj", vertices, uvs, normals);
	cube[END] = vertices.size();

	trees[0][START] = vertices.size();
	res = loadOBJ("models/tree1.obj", vertices, uvs, normals);
	trees[0][END] = vertices.size();

	trees[1][START] = vertices.size();
	res = loadOBJ("models/tree2.obj", vertices, uvs, normals);
	trees[1][END] = vertices.size();

	trees[2][START] = vertices.size();
	res = loadOBJ("models/tree3.obj", vertices, uvs, normals);
	trees[2][END] = vertices.size();

	trees[3][START] = vertices.size();
	res = loadOBJ("models/tree4.obj", vertices, uvs, normals);
	trees[3][END] = vertices.size();

	trees[4][START] = vertices.size();
	res = loadOBJ("models/tree5.obj", vertices, uvs, normals);
	trees[4][END] = vertices.size();

	trees[5][START] = vertices.size();
	res = loadOBJ("models/tree6.obj", vertices, uvs, normals);
	trees[5][END] = vertices.size();

	trees[6][START] = vertices.size();
	res = loadOBJ("models/tree7.obj", vertices, uvs, normals);
	trees[6][END] = vertices.size();

	trees[7][START] = vertices.size();
	res = loadOBJ("models/rock.obj", vertices, uvs, normals);
	trees[7][END] = vertices.size();

	hpCrate[0][START] = vertices.size();
	res = loadOBJ("models/hp1.obj", vertices, uvs, normals);
	hpCrate[0][END] = vertices.size();

	hpCrate[1][START] = vertices.size();
	res = loadOBJ("models/hp2.obj", vertices, uvs, normals);
	hpCrate[1][END] = vertices.size();

	shell[START] = vertices.size();
	res = loadOBJ("models/shell.obj", vertices, uvs, normals);
	shell[END] = vertices.size();

	smokeCrate[0][START] = vertices.size();
	res = loadOBJ("models/smoke1.obj", vertices, uvs, normals);
	smokeCrate[0][END] = vertices.size();

	smokeCrate[1][START] = vertices.size();
	res = loadOBJ("models/smoke2.obj", vertices, uvs, normals);
	smokeCrate[1][END] = vertices.size();

	ammo[START] = vertices.size();
	res = loadOBJ("models/ammo.obj", vertices, uvs, normals);
	ammo[END] = vertices.size();


	if (res)
	{
		glGenBuffers(1, &VertexVBOID);
		glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	}
}
float White[4] = { 1,1,1,1 };
float *Array3(float a, float b, float c)
{
	static float array[4];
	array[0] = a;
	array[1] = b;
	array[2] = c;
	array[3] = 1.;
	return array;
}
float *MulArray3(float factor, float array0[3])
{
	static float array[4];
	array[0] = factor * array0[0];
	array[1] = factor * array0[1];
	array[2] = factor * array0[2];
	array[3] = 1.;
	return array;
}
void SetPointLight(int ilight, float x, float y, float z, float r, float g, float b)
{
	glLightfv(ilight, GL_POSITION, Array3(x, y, z));
	glLightfv(ilight, GL_AMBIENT, Array3(r, g, b));
	glLightfv(ilight, GL_DIFFUSE, Array3(r, g, b));
	glLightfv(ilight, GL_SPECULAR, Array3(r, g, b));
	glLightf(ilight, GL_CONSTANT_ATTENUATION, 1.);
	glLightf(ilight, GL_LINEAR_ATTENUATION, 0.);
	glLightf(ilight, GL_QUADRATIC_ATTENUATION, 0.);
	glEnable(ilight);
}
void SetSpotLight(int ilight, float x, float y, float z, float xdir, float ydir, float zdir, float r, float g, float b)
{
	glLightfv(ilight, GL_POSITION, Array3(x, y, z));
	glLightfv(ilight, GL_SPOT_DIRECTION, Array3(xdir, ydir, zdir));
	glLightf(ilight, GL_SPOT_EXPONENT, 1.);
	glLightf(ilight, GL_SPOT_CUTOFF, 45.);
	glLightfv(ilight, GL_AMBIENT, Array3(0., 0., 0.));
	glLightfv(ilight, GL_DIFFUSE, Array3(r, g, b));
	glLightfv(ilight, GL_SPECULAR, Array3(r, g, b));
	glLightf(ilight, GL_CONSTANT_ATTENUATION, 1.);
	glLightf(ilight, GL_LINEAR_ATTENUATION, 0.);
	glLightf(ilight, GL_QUADRATIC_ATTENUATION, 0.);
	glEnable(ilight);
}
void SetMaterial(float r, float g, float b, float shininess)
{
	glMaterialfv(GL_BACK, GL_EMISSION, Array3(0., 0., 0.));
	glMaterialfv(GL_BACK, GL_AMBIENT, MulArray3(.4f, White));
	glMaterialfv(GL_BACK, GL_DIFFUSE, MulArray3(1., White));
	glMaterialfv(GL_BACK, GL_SPECULAR, Array3(0., 0., 0.));
	glMaterialf(GL_BACK, GL_SHININESS, 2.f);
	glMaterialfv(GL_FRONT, GL_EMISSION, Array3(0., 0., 0.));
	glMaterialfv(GL_FRONT, GL_AMBIENT, Array3(r/2, g/2, b /2));
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Array3(r , g , b ));
	glMaterialfv(GL_FRONT, GL_SPECULAR, MulArray3(.8f, White));
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}
void drawSmoke(float X, float Y, float Z,float angle, float scale, float r, float g, float b, float beginTime,float duration)
{
	int beginPoint;
	int endPoint;
	if ((Time - beginTime) >= 0 && (Time - beginTime) <= duration)
	{
		//std::cout << (Time - beginTime) << ", " << duration << std::endl;
		glPushMatrix();
		glTranslatef(X, Z + 3 + (Time - beginTime) * 100, Y);
		glScalef(1 + (Time - beginTime)* 10000, 1 + (Time - beginTime) * 10000, 1 + (Time - beginTime) * 10000);
		glRotatef(angle, 0, 1, 0);
		glPushMatrix();
		glScalef(scale, scale, scale);
		glTranslatef(0.5, 0, 0.25);
		beginPoint = trees[7][START];
		endPoint = trees[7][END] - trees[7][START];
		glPushMatrix();
		glTranslatef(0.5, 0, 0.5);
		SetMaterial(r-0.10, g - 0.10, b - 0.10, 1);
		glColor3f(r, g, b);
		glRotatef(270, 1, 0, 0);
		glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();

	}
}
void drawExplosion(float X, float Y, float Z, float angle, float scale, float r, float g, float b, float beginTime, float duration)
{
	int beginPoint;
	int endPoint;
	if ((Time - beginTime) >= 0 && (Time - beginTime) <= duration)
	{
		//std::cout << (Time - beginTime) << ", " << duration << std::endl;
		glPushMatrix();
		glTranslatef(X, Z + 3 + (Time - beginTime) * 100, Y);
		glScalef(1 + (Time - beginTime) * 10000, 1 + (Time - beginTime) * 10000, 1 + (Time - beginTime) * 10000);
		glRotatef(angle, 0, 1, 0);
		glPushMatrix();
		glScalef(scale, scale, scale);
		glTranslatef(0.5, 0, 0.25);
		beginPoint = trees[7][START];
		endPoint = trees[7][END] - trees[7][START];
		glPushMatrix();
		glTranslatef(0.5, 0, 0.5);
		SetMaterial(r - 0.10, g - 0.10, b - 0.10, 1);
		glColor3f(r, g, b);
		glRotatef(270, 1, 0, 0);
		glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();

	}
}
void drawIS3(float X, float Y, float Z , float hullAngle ,float turretAngle)
{
	glPushMatrix();
	glTranslatef(X,Y,Z);	//movement
	glTranslatef(0,2.1,0);
	glRotatef(hullAngle, 0, 1, 0);
	glRotatef(270, 1, 0, 0);
	glScalef(TANKSCALE, TANKSCALE, TANKSCALE);

	int beginPoint = IS3[0][START];
	int endPoint = IS3[0][END] - IS3[0][START];
	glPushMatrix();
	glTranslatef(0, 3, 0);
	glRotatef(turretAngle, 0, 0, 1);
	glTranslatef(0, -3, 0);
	glColor3f(0, 0, 0.75);
	SetMaterial(0, 0, 0.5, 1.0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	//glColor3f(0, 0.25, 0);
	//glDrawArrays(GL_LINES, beginPoint, endPoint);
	//glColor3f(1, 0, 0);
	//glDrawArrays(GL_POINTS, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = IS3[1][START];
	endPoint = IS3[1][END] - IS3[1][START];
	glPushMatrix();
	glTranslatef(0.75, 0, 0);
	glColor3f(0, 0, 0.75);
	SetMaterial(0, 0, 0.5, 1.0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	//glColor3f(0, 0.25, 0);
	//glDrawArrays(GL_LINES, beginPoint, endPoint);
	//glColor3f(1, 0, 0);
	//glDrawArrays(GL_POINTS, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = IS3[2][START];
	endPoint = IS3[2][END] - IS3[2][START];
	glPushMatrix();
	glColor3f(0.2, 0.2, 0.2);
	SetMaterial(0.1, 0.1, 0.1, 1.0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	//glColor3f(0, 0, 0);
	//glDrawArrays(GL_LINES, beginPoint, endPoint);
	//glColor3f(1, 0, 0);
	//glDrawArrays(GL_POINTS, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = Track[0][START];
	endPoint = Track[0][END] - Track[0][START];
	glPushMatrix();
	glTranslatef(-13, 0, -3);
	glColor3f(0.3, 0.25, 0.18);
	SetMaterial(0.3, 0.25, 0.18, 1.0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	//glColor3f(0, 0, 0);
	//glDrawArrays(GL_LINES, beginPoint, endPoint);
	//glColor3f(1, 0, 0);
	//glDrawArrays(GL_POINTS, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = Track[1][START];
	endPoint = Track[1][END] - Track[1][START];
	glPushMatrix();
	glTranslatef(0, 0, -3);
	glColor3f(0.3, 0.25, 0.18);
	SetMaterial(0.3, 0.25, 0.18, 1.0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	//glColor3f(0, 0, 0);
	//glDrawArrays(GL_LINES, beginPoint, endPoint);
	//glColor3f(1, 0, 0);
	//glDrawArrays(GL_POINTS, beginPoint, endPoint);
	glPopMatrix();

	glPopMatrix();
}
void drawIS3Dead(float X, float Y, float Z, float hullAngle, float turretAngle)
{
	glPushMatrix();
	glTranslatef(X, Y, Z);	//movement
	glTranslatef(0, 2.1, 0);
	glRotatef(hullAngle, 0, 1, 0);
	glRotatef(270, 1, 0, 0);
	glScalef(TANKSCALE, TANKSCALE, TANKSCALE);

	int beginPoint = IS3[0][START];
	int endPoint = IS3[0][END] - IS3[0][START];
	glPushMatrix();
	glTranslatef(0, 3, 0);
	glRotatef(turretAngle, 0, 0, 1);
	glTranslatef(0, -3, 0);
	glColor3f(0, 0, 0.1);
	SetMaterial(0, 0, 0.1, 1.0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	//glColor3f(0, 0.25, 0);
	//glDrawArrays(GL_LINES, beginPoint, endPoint);
	//glColor3f(1, 0, 0);
	//glDrawArrays(GL_POINTS, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = IS3[1][START];
	endPoint = IS3[1][END] - IS3[1][START];
	glPushMatrix();
	glTranslatef(0.75, 0, 0);
	glColor3f(0, 0, 0.1);
	SetMaterial(0, 0, 0.1, 1.0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	//glColor3f(0, 0.25, 0);
	//glDrawArrays(GL_LINES, beginPoint, endPoint);
	//glColor3f(1, 0, 0);
	//glDrawArrays(GL_POINTS, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = IS3[2][START];
	endPoint = IS3[2][END] - IS3[2][START];
	glPushMatrix();
	glColor3f(0.2, 0.2, 0.2);
	SetMaterial(0.1, 0.1, 0.1, 1.0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	//glColor3f(0, 0, 0);
	//glDrawArrays(GL_LINES, beginPoint, endPoint);
	//glColor3f(1, 0, 0);
	//glDrawArrays(GL_POINTS, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = Track[0][START];
	endPoint = Track[0][END] - Track[0][START];
	glPushMatrix();
	glTranslatef(-13, 0, -3);
	glColor3f(0, 0, 0.1);
	SetMaterial(0, 0, 0.1, 1.0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	//glColor3f(0, 0, 0);
	//glDrawArrays(GL_LINES, beginPoint, endPoint);
	//glColor3f(1, 0, 0);
	//glDrawArrays(GL_POINTS, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = Track[1][START];
	endPoint = Track[1][END] - Track[1][START];
	glPushMatrix();
	glTranslatef(0, 0, -3);
	glColor3f(0, 0, 0.1);
	SetMaterial(0, 0, 0.1, 1.0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	//glColor3f(0, 0, 0);
	//glDrawArrays(GL_LINES, beginPoint, endPoint);
	//glColor3f(1, 0, 0);
	//glDrawArrays(GL_POINTS, beginPoint, endPoint);
	glPopMatrix();

	glPopMatrix();
}
void drawAbram(float X, float Y, float Z, float hullAngle,float turretAngle)
{
	glPushMatrix();
	glTranslatef(X, Y, Z);	//movement
	glTranslatef(0, 2.1, 0);
	glRotatef(hullAngle, 0, 1, 0);
	glRotatef(270, 1, 0, 0);
	glScalef(TANKSCALE, TANKSCALE, TANKSCALE);

	int beginPoint = Abram[0][START];
	int endPoint = Abram[0][END] - Abram[0][START];
	glPushMatrix();
	glTranslatef(0, 1, 0);
	glRotatef(turretAngle, 0, 0, 1);
	glTranslatef(0, -1, 0);
	glTranslatef(1.75, 0, 0.25);
	SetMaterial(0.5, 0.5, 0, 1.0);
	glColor3f(1, 0.5, 0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	//glColor3f(0.25, 0.25, 0);
	//glDrawArrays(GL_LINES, beginPoint, endPoint);
	//glColor3f(1, 0, 0);
	//glDrawArrays(GL_POINTS, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = Abram[1][START];
	endPoint = Abram[1][END] - Abram[1][START];
	glPushMatrix();
	glTranslatef(-1,0,0.25);
	SetMaterial(0.5, 0.5, 0, 1.0);
	glColor3f(1, 0.5, 0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	//glColor3f(0.25, 0.25, 0);
	//glDrawArrays(GL_LINES, beginPoint, endPoint);
	//glColor3f(1, 0, 0);
	//glDrawArrays(GL_POINTS, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = Track[0][START];
	endPoint = Track[0][END] - Track[0][START];
	glPushMatrix();
	glTranslatef(-12.75, 0, -3);
	SetMaterial(0.3, 0.25, 0.18, 1.0);
	glColor3f(0.3, 0.25, 0.18);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	//glColor3f(0, 0, 0);
	//glDrawArrays(GL_LINES, beginPoint, endPoint);
	//glColor3f(1, 0, 0);
	//glDrawArrays(GL_POINTS, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = Track[1][START];
	endPoint = Track[1][END] - Track[1][START];
	glPushMatrix();
	glTranslatef(-0.25, 0, -3);
	SetMaterial(0.3, 0.25, 0.18, 1.0);
	glColor3f(0.3, 0.25, 0.18);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	//glColor3f(0, 0, 0);
	//glDrawArrays(GL_LINES, beginPoint, endPoint);
	//glColor3f(1, 0, 0);
	//glDrawArrays(GL_POINTS, beginPoint, endPoint);
	glPopMatrix();

	glPopMatrix();
}
void drawAbramDead(float X, float Y, float Z, float hullAngle, float turretAngle)
{
	glPushMatrix();
	glTranslatef(X, Y, Z);	//movement
	glTranslatef(0, 2.1, 0);
	glRotatef(hullAngle, 0, 1, 0);
	glRotatef(270, 1, 0, 0);
	glScalef(TANKSCALE, TANKSCALE, TANKSCALE);

	int beginPoint = Abram[0][START];
	int endPoint = Abram[0][END] - Abram[0][START];
	glPushMatrix();
	glTranslatef(0, 1, 0);
	glRotatef(turretAngle, 0, 0, 1);
	glTranslatef(0, -1, 0);
	glTranslatef(1.75, 0, 0.25);
	SetMaterial(0.15, 0.15, 0, 1.0);
	glColor3f(0.1, 0.1, 0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	//glColor3f(0.25, 0.25, 0);
	//glDrawArrays(GL_LINES, beginPoint, endPoint);
	//glColor3f(1, 0, 0);
	//glDrawArrays(GL_POINTS, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = Abram[1][START];
	endPoint = Abram[1][END] - Abram[1][START];
	glPushMatrix();
	glTranslatef(-1, 0, 0.25);
	SetMaterial(0.15, 0.15, 0, 1.0);
	glColor3f(0.1, 0.1, 0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	//glColor3f(0.25, 0.25, 0);
	//glDrawArrays(GL_LINES, beginPoint, endPoint);
	//glColor3f(1, 0, 0);
	//glDrawArrays(GL_POINTS, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = Track[0][START];
	endPoint = Track[0][END] - Track[0][START];
	glPushMatrix();
	glTranslatef(-12.75, 0, -3);
	SetMaterial(0.15, 0.15, 0, 1.0);
	glColor3f(0.1, 0.1, 0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	//glColor3f(0, 0, 0);
	//glDrawArrays(GL_LINES, beginPoint, endPoint);
	//glColor3f(1, 0, 0);
	//glDrawArrays(GL_POINTS, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = Track[1][START];
	endPoint = Track[1][END] - Track[1][START];
	glPushMatrix();
	glTranslatef(-0.25, 0, -3);
	SetMaterial(0.15, 0.15, 0, 1.0);
	glColor3f(0.1, 0.1, 0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	//glColor3f(0, 0, 0);
	//glDrawArrays(GL_LINES, beginPoint, endPoint);
	//glColor3f(1, 0, 0);
	//glDrawArrays(GL_POINTS, beginPoint, endPoint);
	glPopMatrix();

	glPopMatrix();
}
void drawCube(float X, float Y, float Z,float r,float g, float b)
{
	glPushMatrix();				// 0
	glTranslatef(X, Z, Y);	//movement
	glTranslatef(-5.25, CUBESIZE / 2, 0);
	glScalef(CUBESIZE/2, CUBESIZE/2, CUBESIZE/2);

	int beginPoint = cube[START];
	int endPoint = cube[END] - cube[START];

	glPushMatrix();				// 1
	glTranslatef(1.75, 0, 0);

	glPushMatrix();				// 2
	glScalef(.9, .9, .9);
	//SetMaterial(0.25, 0.25, 0.25, 1.0);
	glColor3f(0.25, 0.25, 0.25);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();				// 2

	//SetMaterial(r, g, b, 1.0);
	glColor3f(r, g, b);
	glPushMatrix();				// 3
	glTranslatef(-0.55, -0.55, 0);
	glScalef(0.48, 0.48,.95);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();				// 3

	glPushMatrix();				// 4
	glTranslatef(0.55, -0.55, 0);
	glScalef(0.48, 0.48, .95);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();				// 4
	
	glPushMatrix();				// 5
	glTranslatef(0, 0.55, -0.55);
	glScalef(.95, 0.48, 0.48);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();				// 5

	glPushMatrix();				// 6
	glTranslatef(0, 0.55, 0.55);
	glScalef(.95, 0.48, 0.48);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();				// 6

	glPopMatrix();				// 1

	glPopMatrix();				// 0
}
void drawTreeCube(float X, float Y,float angle, int index)
{
	int beginPoint;
	int endPoint;
	switch (index)
	{
	case 0:
		glPushMatrix();
		glTranslatef(X, 0, Y);	//movement
		glRotatef(angle, 0, 1, 0);
		glTranslatef(-8.5, -0.5, 6);
		glScalef(TREESCALE, TREESCALE, TREESCALE);
		beginPoint = trees[0][START];
		endPoint = 3960 - 2250;
		glPushMatrix();
		glRotatef(270, 1, 0, 0);
		SetMaterial(0.4, 0.2, 0, 1.0);
		glColor3f(0.4, 0.2, 0);
		glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
		glPopMatrix();

		beginPoint = trees[0][START] + 3960 - 2250;
		endPoint = 2250;
		glPushMatrix();
		SetMaterial(0, 0.75, 0, 1.0);
		glColor3f(0, 0.75, 0);
		glRotatef(270, 1, 0, 0);
		glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
		glPopMatrix();
		glPopMatrix();
		break;
	case 1:
		glPushMatrix();
		glTranslatef(X, 0, Y);	//movement
		glRotatef(angle, 0, 1, 0);
		glTranslatef(1, -0.5, 4);
		glScalef(TREESCALE, TREESCALE, TREESCALE);
		beginPoint = trees[1][START];
		endPoint = 5688 - 2022;
		glPushMatrix();
		glRotatef(270, 1, 0, 0);
		SetMaterial(0.4, 0.2, 0, 1.0);
		glColor3f(0.4, 0.2, 0);
		glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
		glPopMatrix();

		beginPoint = trees[1][START] + 5688 - 2022;
		endPoint = 2022;
		glPushMatrix();
		SetMaterial(0, 0.75, 0, 1.0);
		glColor3f(0, 0.75, 0);
		glRotatef(270, 1, 0, 0);
		glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
		glPopMatrix();
		glPopMatrix();
		break;
	case 2:
		glPushMatrix();
		glTranslatef(X, 0, Y);	//movement
		glRotatef(angle, 0, 1, 0);
		glTranslatef(5.5, -0.5, 4);
		glScalef(TREESCALE, TREESCALE, TREESCALE);
		beginPoint = trees[2][START];
		endPoint = 4398 - 3525;
		glPushMatrix();
		glRotatef(270, 1, 0, 0);
		SetMaterial(0, 0.75, 0, 1.0);
		glColor3f(0, 0.75, 0);
		glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
		glPopMatrix();

		beginPoint = trees[2][START] + 4398 - 3525;
		endPoint = 3525;
		glPushMatrix();
		SetMaterial(0.4, 0.2, 0, 1.0);
		glColor3f(0.4, 0.2, 0);
		glRotatef(270, 1, 0, 0);
		glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
		glPopMatrix();
		glPopMatrix();
		break;
	case 3:
		glPushMatrix();
		glTranslatef(X, 0, Y);	//movement
		glRotatef(angle, 0, 1, 0);
		glTranslatef(8.5, -0.5, 4);
		glScalef(TREESCALE, TREESCALE, TREESCALE);
		beginPoint = trees[3][START];
		endPoint = 3522 - 3000;
		glPushMatrix();
		glRotatef(270, 1, 0, 0);
		SetMaterial(0, 0.75, 0, 1.0);
		glColor3f(0, 0.75, 0);
		glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
		glPopMatrix();

		beginPoint = trees[3][START] + 3522 - 3000;
		endPoint = 3000;
		glPushMatrix();
		SetMaterial(0.4, 0.2, 0, 1.0);
		glColor3f(0.4, 0.2, 0);
		glRotatef(270, 1, 0, 0);
		glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
		glPopMatrix();
		glPopMatrix();
		break;
	case 4:
		glPushMatrix();
		glTranslatef(X, 0, Y);	//movement
		glRotatef(angle, 0, 1, 0);
		glTranslatef(-6, 10.5, 4);
		glScalef(TREESCALE, TREESCALE, TREESCALE);
		beginPoint = trees[4][START];
		endPoint = 4752 - 3816;
		glPushMatrix();
		glRotatef(270, 1, 0, 0);
		SetMaterial(0.4, 0.2, 0, 1.0);
		glColor3f(0.4, 0.2, 0);
		glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
		glPopMatrix();

		beginPoint = trees[4][START] + 4752 - 3816;
		endPoint = 3816;
		glPushMatrix();
		SetMaterial(0, 0.75, 0, 1.0);
		glColor3f(0, 0.75, 0);
		glRotatef(270, 1, 0, 0);
		glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
		glPopMatrix();
		glPopMatrix();
		break;
	case 5:
		glPushMatrix();
		glTranslatef(X, 0, Y);	//movement
		glRotatef(angle, 0, 1, 0);
		glTranslatef(0, 10.5, 4);
		glScalef(TREESCALE, TREESCALE, TREESCALE);
		beginPoint = trees[5][START];
		endPoint = 6105 - 4668;
		glPushMatrix();
		glRotatef(270, 1, 0, 0);
		SetMaterial(0.4, 0.2, 0, 1.0);
		glColor3f(0.4, 0.2, 0);
		glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
		glPopMatrix();

		beginPoint = trees[5][START] + 6105 - 4668;
		endPoint = 4668;
		glPushMatrix();
		SetMaterial(0, 0.75, 0, 1.0);
		glColor3f(0, 0.75, 0);
		glRotatef(270, 1, 0, 0);
		glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
		glPopMatrix();
		glPopMatrix();
		break;
	case 6:
		glPushMatrix();
		glTranslatef(X, 0, Y);	//movement
		glRotatef(angle, 0, 1, 0);
		glTranslatef(6, 10.5, 4);
		glScalef(TREESCALE, TREESCALE, TREESCALE);
		beginPoint = trees[6][START];
		endPoint = 6030 - 4944;
		glPushMatrix();
		glRotatef(270, 1, 0, 0);
		SetMaterial(0.4, 0.2, 0, 1.0);
		glColor3f(0.4, 0.2, 0);
		glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
		glPopMatrix();

		beginPoint = trees[6][START] + 6030 - 4944;
		endPoint = 4944;
		glPushMatrix();
		SetMaterial(0, 0.75, 0, 1.0);
		glColor3f(0, 0.75, 0);
		glRotatef(270, 1, 0, 0);
		glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
		glPopMatrix();
		glPopMatrix();
		break;
	case 7:
		glPushMatrix();
		glTranslatef(X, 0, Y);	//movement
		glRotatef(angle, 0, 1, 0);
		glTranslatef(12, 0, 4.5);
		glScalef(TREESCALE/2, TREESCALE/2, TREESCALE/2);
		beginPoint = trees[7][START];
		endPoint = trees[7][END] - trees[7][START];
		glPushMatrix();
		SetMaterial(0.125, 0.125, 0.125, 1);
		glColor3f(0.25, 0.25, 0.25);
		glRotatef(270, 1, 0, 0);
		glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
		glPopMatrix();
		glPopMatrix();
		break;
	}
}
void drawShell(float X, float Y, float angle,float scale=1)
{
	int beginPoint;
	int endPoint;

	glPushMatrix();
	glTranslatef(X, 2.75, Y);	//movement
	glRotatef(angle, 0, 1, 0);
	glTranslatef(0, 0, -2.5);
	glScalef(SHELLSCALE*scale, SHELLSCALE*scale, SHELLSCALE*scale);
	beginPoint = shell[START];
	endPoint = shell[END]- shell[START];
	glPushMatrix();
	glRotatef(270, 1, 0, 0);
	SetMaterial(1, 1, 0, 1.0);
	glColor3f(1, 0.125, 0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();
	glPopMatrix();
}
void drawAmmo(float X, float Y)
{
	int beginPoint;
	int endPoint;

	glPushMatrix();
	glTranslatef(X, 1, Y);	//movement
	glRotatef((int)(Time * 5000) % 360, 0, 1, 0);
	glTranslatef(0, 0, 0);
	glScalef(200,200,200);
	beginPoint = ammo[START];
	endPoint = ammo[END]- ammo[START];
	glPushMatrix();
	glRotatef(270, 1, 0, 0);
	SetMaterial(1, 1, 0, 1.0);
	glColor3f(1, 0.125, 0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();
	glPopMatrix();
}
void drawSmokeCrate(float X, float Y, int angle = 0)
{
	int beginPoint;
	int endPoint;

	glPushMatrix();

	glTranslatef(X, 1, Y);	//movement
	glRotatef(angle, 1, 0, 0);
	glRotatef((int)(Time * 5000) % 360, 0, 1, 0);
	glScalef(0.5, 0.5, 0.5);

	beginPoint = smokeCrate[1][START];
	endPoint = smokeCrate[1][END]- smokeCrate[1][START];

	SetMaterial(0.25, 0.25, 0.25, 1.0);
	glColor3f(0.25, 0.25, 0.25);
	glPushMatrix();
	glRotatef(270, 1, 0, 0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = smokeCrate[0][START];
	endPoint = smokeCrate[0][END]- smokeCrate[0][START];

	SetMaterial(0, 1, 0, 1.0);
	glColor3f(0, 1, 0);
	glPushMatrix();
	glRotatef(270, 1, 0, 0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	glPopMatrix();
}
void drawHPCrate(float X, float Y)
{
	int beginPoint;
	int endPoint;

	glPushMatrix();

	glTranslatef(X, 1, Y);	//movement
	glRotatef((int)(Time * 5000) % 360, 0, 1, 0);
	glScalef(0.5, 0.5, 0.5);

	beginPoint = hpCrate[0][START];
	endPoint = hpCrate[0][END] - hpCrate[0][START];

	SetMaterial(0.25, 0.25, 0.25, 1.0);
	glColor3f(0.25, 0.25, 0.25);
	glPushMatrix();
	glRotatef(270, 1, 0, 0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = hpCrate[1][START];
	endPoint = hpCrate[1][END] - hpCrate[1][START];

	SetMaterial(1, 0.75, 0, 1.0);
	glColor3f(1, 0, 0);
	glPushMatrix();
	glRotatef(270, 1, 0, 0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	glPopMatrix();
}
bool MapCollisionModel(float AX, float AY, float Xstride, float Ystride, int sign,int * ival = NULL, int *jval = NULL)
{
	for (int j = 0; j < 14; j++)
	{
		for (int i = 0; i < 24; i++)
		{
			if (myMap.isSolid[i][j] && myMap.color[i][j][0] != 7)
			{
				if ((((AX + (Xstride * sign) < myMap.coord[i][j][0] + BODY) && (AX + (Xstride * sign) > myMap.coord[i][j][0] - BODY)) &&
					((AY + (Ystride * sign) < myMap.coord[i][j][1] + BODY) && (AY + (Ystride * sign) > myMap.coord[i][j][1] - BODY))))
				{
					if (ival != NULL && jval != NULL)
					{
						*ival = i;
						*jval = j;
					}
					return true;
				}
			}
			if (myMap.isSolid[i][j] && myMap.color[i][j][0] == 7)
			{
				if ((((AX + (Xstride * sign) < myMap.coord[i][j][0] + BODY/2) && (AX + (Xstride * sign) > myMap.coord[i][j][0] - BODY/2)) &&
					((AY + (Ystride * sign) < myMap.coord[i][j][1] + BODY/2) && (AY + (Ystride * sign) > myMap.coord[i][j][1] - BODY/2))))
					return true;
			}
		}
	}
	return false;
}
int CrateCollisionModel(float AX, float AY, float Xstride, float Ystride, int sign)
{
	for (int i = 0; i < CRATECAP; i++)
	{
		if (Crates[i].isActive)
		{
			if ((((AX + (Xstride * sign) < Crates[i].X + BODY) && (AX + (Xstride * sign) > Crates[i].X - BODY)) &&
				((AY + (Ystride * sign) <Crates[i].Y + BODY) && (AY + (Ystride * sign) > Crates[i].Y - BODY))))
			{
				return i;
			}
		}
	}
	return CRATECAP;
}
void KeyHandler() {
	
	float AbramTX = TANKSPEED * (float)sin(AbramHullAngle * (float)((float)PI / (float)180));
	float AbramTY = TANKSPEED * (float)cos(AbramHullAngle * (float)((float)PI / (float)180));
	float IS3TX = TANKSPEED * (float)sin(IS3HullAngle * (float)((float)PI / (float)180));
	float IS3TY = TANKSPEED * (float)cos(IS3HullAngle * (float)((float)PI / (float)180));
	if ((keyBuffer['w'] || keyBuffer['W']) && AbramHP > 0) {
		if (((AbramXY[0] - AbramTX) < MAPEDGEX && (AbramXY[0] - AbramTX) > -MAPEDGEX) &&
			(!MapCollisionModel(AbramXY[0], AbramXY[1], AbramTX, AbramTY, (-1))) &&
			!(((AbramXY[0] - AbramTX < IS3XY[0] + BODY) && (AbramXY[0] - AbramTX > IS3XY[0] - BODY)) &&
			((AbramXY[1] - AbramTY < IS3XY[1] + BODY) && (AbramXY[1] - AbramTY > IS3XY[1] - BODY))))
		{
			AbramXY[0] -= AbramTX;
		}
		if (((AbramXY[1] - AbramTY) < MAPEDGEY && (AbramXY[1] - AbramTY) > -MAPEDGEY) &&
			(!MapCollisionModel(AbramXY[0], AbramXY[1], AbramTX, AbramTY, (-1))) &&
			!(((AbramXY[0] - AbramTX < IS3XY[0] + BODY) && (AbramXY[0] - AbramTX > IS3XY[0] - BODY)) &&
			((AbramXY[1] - AbramTY < IS3XY[1] + BODY) && (AbramXY[1] - AbramTY > IS3XY[1] - BODY))))
		{
			AbramXY[1] -= AbramTY;
		}
		if (smokeIndex >= 1000)
			smokeIndex = 0;
		smokeIDBuffer[smokeIndex] = Time;
		smokeCoordBuffer[smokeIndex][0] = AbramXY[0];
		smokeCoordBuffer[smokeIndex][1] = AbramXY[1];
		smokeDurBuffer[smokeIndex] = 0.01;
		smokeAngleBuffer[smokeIndex] = rand() % 360;
		smokeIDBufferSet[smokeIndex] = !smokeIDBufferSet[smokeIndex];
		smokeActive[smokeIndex] = true;
		smokeIndex++;
		int cratecheck = CrateCollisionModel(AbramXY[0], AbramXY[1],
			AbramTX,
			AbramTY,
			-1
		);
		if (cratecheck != CRATECAP)
		{
			switch (Crates[cratecheck].type)
			{
			case 0:
				AbramShells = SHELLSTORAGE;
				alSourcePlay(Sources[5]);
				break;
			case 1:
				AbramSmoke = SMOKECOUNT;
				alSourcePlay(Sources[5]);
				break;
			case 2:
				AbramHP = TANKHP;
				alSourcePlay(Sources[4]);
				break;
			}
			Crates[cratecheck].isActive = false;
			myMap.isCrate[Crates[cratecheck].i][Crates[cratecheck].j] = false;
		}
	}
	if ((keyBuffer['s'] || keyBuffer['S']) && AbramHP > 0) {
		if (((AbramXY[0] + AbramTX) < MAPEDGEX && (AbramXY[0] + AbramTX) > -MAPEDGEX) &&
			(!MapCollisionModel(AbramXY[0], AbramXY[1], AbramTX, AbramTY, (1))) &&
			!(((AbramXY[0] + AbramTX < IS3XY[0] + BODY) && (AbramXY[0] + AbramTX > IS3XY[0] - BODY)) &&
			((AbramXY[1] + AbramTY < IS3XY[1] + BODY) && (AbramXY[1] + AbramTY > IS3XY[1] - BODY))))
		{
			AbramXY[0] += AbramTX;
		}
		if (((AbramXY[1] + AbramTY) < MAPEDGEY && (AbramXY[1] + AbramTY) > -MAPEDGEY) &&
			(!MapCollisionModel(AbramXY[0], AbramXY[1], AbramTX, AbramTY, (1))) &&
			!(((AbramXY[0] + AbramTX < IS3XY[0] + BODY) && (AbramXY[0] + AbramTX > IS3XY[0] - BODY)) &&
			((AbramXY[1] + AbramTY < IS3XY[1] + BODY) && (AbramXY[1] + AbramTY > IS3XY[1] - BODY))))
		{
			AbramXY[1] += AbramTY;
		}
		if (smokeIndex >= 1000)
			smokeIndex = 0;
		smokeIDBuffer[smokeIndex] = Time;
		smokeCoordBuffer[smokeIndex][0] = AbramXY[0];
		smokeCoordBuffer[smokeIndex][1] = AbramXY[1];
		smokeDurBuffer[smokeIndex] = 0.01;
		smokeAngleBuffer[smokeIndex] = rand() % 360;
		smokeIDBufferSet[smokeIndex] = !smokeIDBufferSet[smokeIndex];
		smokeActive[smokeIndex] = true;
		smokeIndex++;
		int cratecheck = CrateCollisionModel(AbramXY[0], AbramXY[1],
			AbramTX,
			AbramTY,
			1
		);
		if (cratecheck != CRATECAP)
		{
			switch (Crates[cratecheck].type)
			{
			case 0:
				AbramShells = SHELLSTORAGE;
				alSourcePlay(Sources[5]);
				break;
			case 1:
				AbramSmoke = SMOKECOUNT;
				alSourcePlay(Sources[5]);
				break;
			case 2:
				AbramHP = TANKHP;
				alSourcePlay(Sources[4]);
				break;
			}
			Crates[cratecheck].isActive = false;
			myMap.isCrate[Crates[cratecheck].i][Crates[cratecheck].j] = false;
		}
	}
	if ((keyBuffer['a'] || keyBuffer['A']) && AbramHP > 0) {
		if (keyBuffer['s'] || keyBuffer['S'])
			AbramHullAngle -= TANKSPEED * 5;
		else
			AbramHullAngle += TANKSPEED * 5;
		if (smokeIndex >= 1000)
			smokeIndex = 0;
		smokeIDBuffer[smokeIndex] = Time;
		smokeCoordBuffer[smokeIndex][0] = AbramXY[0];
		smokeCoordBuffer[smokeIndex][1] = AbramXY[1];
		smokeDurBuffer[smokeIndex] = 0.01;
		smokeAngleBuffer[smokeIndex] = rand() % 360;
		smokeIDBufferSet[smokeIndex] = !smokeIDBufferSet[smokeIndex];
		smokeActive[smokeIndex] = true;
		smokeIndex++;
	}
	if ((keyBuffer['d'] || keyBuffer['D']) && AbramHP > 0) {
		if (keyBuffer['s'] || keyBuffer['S'])
			AbramHullAngle += TANKSPEED * 5;
		else
			AbramHullAngle -= TANKSPEED * 5;
		if (smokeIndex >= 1000)
			smokeIndex = 0;
		smokeIDBuffer[smokeIndex] = Time;
		smokeCoordBuffer[smokeIndex][0] = AbramXY[0];
		smokeCoordBuffer[smokeIndex][1] = AbramXY[1];
		smokeDurBuffer[smokeIndex] = 0.01;
		smokeAngleBuffer[smokeIndex] = rand() % 360;
		smokeIDBufferSet[smokeIndex] = !smokeIDBufferSet[smokeIndex];
		smokeActive[smokeIndex] = true;
		smokeIndex++;
	}
	if ((keyBuffer['q'] || keyBuffer['Q']) && AbramHP > 0) {
		AbramTurretAngle += TANKSPEED * 5;
	}
	if ((keyBuffer['e'] || keyBuffer['E']) && AbramHP > 0) {
		AbramTurretAngle -= TANKSPEED * 5;
	}

	if ((keyBuffer['i'] || keyBuffer['I'] || keyBuffer['8']) && IS3HP > 0) {
		if (((IS3XY[0] - IS3TX) < MAPEDGEX && (IS3XY[0] - IS3TX) > -MAPEDGEX) &&
			(!MapCollisionModel(IS3XY[0], IS3XY[1], IS3TX, IS3TY, (-1))) &&
			!(((IS3XY[0] - IS3TX < AbramXY[0] + BODY) && (IS3XY[0] - IS3TX > AbramXY[0] - BODY)) &&
			((IS3XY[1] - IS3TY < AbramXY[1] + BODY) && (IS3XY[1] - IS3TY > AbramXY[1] - BODY))))
		{
			IS3XY[0] -= IS3TX;
		}
		if (((IS3XY[1] - IS3TY) < MAPEDGEY && (IS3XY[1] - IS3TY) > -MAPEDGEY) &&
			(!MapCollisionModel(IS3XY[0], IS3XY[1], IS3TX, IS3TY, (-1))) &&
			!(((IS3XY[0] - IS3TX < AbramXY[0] + BODY) && (IS3XY[0] - IS3TX > AbramXY[0] - BODY)) &&
			((IS3XY[1] - IS3TY < AbramXY[1] + BODY) && (IS3XY[1] - IS3TY > AbramXY[1] - BODY))))
		{
			IS3XY[1] -= IS3TY;
		}
		if (smokeIndex >= 1000)
			smokeIndex = 0;
		smokeIDBuffer[smokeIndex] = Time;
		smokeCoordBuffer[smokeIndex][0] = IS3XY[0];
		smokeCoordBuffer[smokeIndex][1] = IS3XY[1];
		smokeDurBuffer[smokeIndex] = 0.01;
		smokeAngleBuffer[smokeIndex] = rand() % 360;
		smokeIDBufferSet[smokeIndex] = !smokeIDBufferSet[smokeIndex];
		smokeActive[smokeIndex] = true;
		smokeIndex++;
		int cratecheck = CrateCollisionModel(IS3XY[0], IS3XY[1],
			IS3TX,
			IS3TX,
			-1
		);
		if (cratecheck != CRATECAP)
		{
			switch (Crates[cratecheck].type)
			{
			case 0:
				IS3Shells = SHELLSTORAGE;
				alSourcePlay(Sources[5]);
				break;
			case 1:
				IS3Smoke = SMOKECOUNT;
				alSourcePlay(Sources[5]);
				break;
			case 2:
				IS3HP = TANKHP;
				alSourcePlay(Sources[4]);
				break;
			}
			Crates[cratecheck].isActive = false;
			myMap.isCrate[Crates[cratecheck].i][Crates[cratecheck].j] = false;
		}
	}
	if ((keyBuffer['k'] || keyBuffer['K'] || keyBuffer['5']) && IS3HP > 0) {
		if (((IS3XY[0] + IS3TX) < MAPEDGEX && (IS3XY[0] + IS3TX) > -MAPEDGEX) &&
			(!MapCollisionModel(IS3XY[0], IS3XY[1], IS3TX, IS3TY, (1))) &&
			!(((IS3XY[0] + IS3TX < AbramXY[0] + BODY) && (IS3XY[0] + IS3TX > AbramXY[0] - BODY)) &&
			((IS3XY[1] + IS3TY < AbramXY[1] + BODY) && (IS3XY[1] + IS3TY > AbramXY[1] - BODY))))
		{
			IS3XY[0] += IS3TX;
		}
		if (((IS3XY[1] + IS3TY) < MAPEDGEY && (IS3XY[1] + IS3TY) > -MAPEDGEY) &&
			(!MapCollisionModel(IS3XY[0], IS3XY[1], IS3TX, IS3TY, (1))) &&
			!(((IS3XY[0] + IS3TX < AbramXY[0] + BODY) && (IS3XY[0] + IS3TX > AbramXY[0] - BODY)) &&
			((IS3XY[1] + IS3TY < AbramXY[1] + BODY) && (IS3XY[1] + IS3TY > AbramXY[1] - BODY))))
		{
			IS3XY[1] += IS3TY;
		}
		if (smokeIndex >= 1000)
			smokeIndex = 0;
		smokeIDBuffer[smokeIndex] = Time;
		smokeCoordBuffer[smokeIndex][0] = IS3XY[0];
		smokeCoordBuffer[smokeIndex][1] = IS3XY[1];
		smokeDurBuffer[smokeIndex] = 0.01;
		smokeAngleBuffer[smokeIndex] = rand() % 360;
		smokeIDBufferSet[smokeIndex] = !smokeIDBufferSet[smokeIndex];
		smokeActive[smokeIndex] = true;
		smokeIndex++;
	}
	if ((keyBuffer['j'] || keyBuffer['J'] || keyBuffer['4']) && IS3HP > 0) {
		if (keyBuffer['k'] || keyBuffer['K'] || keyBuffer['5'])
			IS3HullAngle -= TANKSPEED * 5;
		else
			IS3HullAngle += TANKSPEED * 5;
		if (smokeIndex >= 1000)
			smokeIndex = 0;
		smokeIDBuffer[smokeIndex] = Time;
		smokeCoordBuffer[smokeIndex][0] = IS3XY[0];
		smokeCoordBuffer[smokeIndex][1] = IS3XY[1];
		smokeDurBuffer[smokeIndex] = 0.01;
		smokeAngleBuffer[smokeIndex] = rand() % 360;
		smokeIDBufferSet[smokeIndex] = !smokeIDBufferSet[smokeIndex];
		smokeActive[smokeIndex] = true;
		smokeIndex++;
		int cratecheck = CrateCollisionModel(IS3XY[0], IS3XY[1],
			IS3TX,
			IS3TX,
			1
		);
		if (cratecheck != CRATECAP)
		{
			switch (Crates[cratecheck].type)
			{
			case 0:
				IS3Shells = SHELLSTORAGE;
				alSourcePlay(Sources[5]);
				break;
			case 1:
				IS3Smoke = SMOKECOUNT;
				alSourcePlay(Sources[5]);
				break;
			case 2:
				IS3HP = TANKHP;
				alSourcePlay(Sources[4]);
				break;
			}
			Crates[cratecheck].isActive = false;
			myMap.isCrate[Crates[cratecheck].i][Crates[cratecheck].j] = false;
		}
	}
	if ((keyBuffer['l'] || keyBuffer['L'] || keyBuffer['6']) && IS3HP > 0) {
		if (keyBuffer['k'] || keyBuffer['K'] || keyBuffer['5'])
			IS3HullAngle += TANKSPEED * 5;
		else
			IS3HullAngle -= TANKSPEED * 5;
		if (smokeIndex >= 1000)
			smokeIndex = 0;
		smokeIDBuffer[smokeIndex] = Time;
		smokeCoordBuffer[smokeIndex][0] = IS3XY[0];
		smokeCoordBuffer[smokeIndex][1] = IS3XY[1];
		smokeDurBuffer[smokeIndex] = 0.01;
		smokeAngleBuffer[smokeIndex] = rand() % 360;
		smokeIDBufferSet[smokeIndex] = !smokeIDBufferSet[smokeIndex];
		smokeActive[smokeIndex] = true;
		smokeIndex++;
	}
	if ((keyBuffer['u'] || keyBuffer['U'] || keyBuffer['7']) && IS3HP > 0) {
		IS3TurretAngle += TANKSPEED * 5;
	}
	if ((keyBuffer['o'] || keyBuffer['O'] || keyBuffer['9']) && IS3HP > 0) {
		IS3TurretAngle -= TANKSPEED * 5;
	}
	if (keyBuffer['g'] || keyBuffer['G']) {
		freeze = !freeze;
	}
	if (keyBuffer[ESCAPE] || keyBuffer['t'] || keyBuffer['T']) {
		DoMainMenu(QUIT);	// will not return here
	}
	if (keyBuffer['f'] || keyBuffer['F'] || keyBuffer[SPACEBAR])
	{
		if ((Time - AbramLastShot) < 0)
			AbramLastShot = 0;

		if (AbramShells > 0 && AbramHP > 0 && ((Time - AbramLastShot) >= RELOADTIME))
		{
			alSourcePlay(Sources[1]);
			AbramLastShot = Time;
			Shells[shellSize].x = AbramXY[0];
			Shells[shellSize].y = AbramXY[1];
			Shells[shellSize].angle = AbramTurretAngle + AbramHullAngle;
			Shells[shellSize].shooterId = ABRAMID;
			Shells[shellSize].startTime = Time;
			Shells[shellSize].active = true;
			if (shellSize < SHELLMAX - 1)
				shellSize++;
			else
				shellSize = 0;
			AbramShells--;
			if (smokeIndex >= 1000)
				smokeIndex = 0;
			for (int i = 0; i < 20; i++)
			{
				smokeIDBuffer[smokeIndex] = Time;
				smokeCoordBuffer[smokeIndex][0] = AbramXY[0] - 5 * sin((AbramTurretAngle + AbramHullAngle) * PI / 180);
				smokeCoordBuffer[smokeIndex][1] = AbramXY[1] - 5 * cos((AbramTurretAngle + AbramHullAngle) * PI / 180);
				smokeDurBuffer[smokeIndex] = 0.015;
				smokeAngleBuffer[smokeIndex] = rand() % 360;
				smokeIDBufferSet[smokeIndex] = !smokeIDBufferSet[smokeIndex];
				smokeActive[smokeIndex] = true;
				smokeIndex++;
			}
		}
	}
	if (keyBuffer['h'] || keyBuffer['H'] || keyBuffer[ENTER] || keyBuffer['0'])
	{
		if ((Time - IS3LastShot) < 0)
			IS3LastShot = 0;

		if (IS3Shells > 0 && IS3HP > 0 && ((Time - IS3LastShot) >= RELOADTIME))
		{
			alSourcePlay(Sources[1]);
			IS3LastShot = Time;
			Shells[shellSize].x = IS3XY[0];
			Shells[shellSize].y = IS3XY[1];
			Shells[shellSize].angle = IS3TurretAngle + IS3HullAngle;
			Shells[shellSize].shooterId = IS3ID;
			Shells[shellSize].startTime = Time;
			Shells[shellSize].active = true;
			if (shellSize < SHELLMAX - 1)
				shellSize++;
			else
				shellSize = 0;
			IS3Shells--;
			for (int i = 0; i < 20; i++)
			{
				smokeIDBuffer[smokeIndex] = Time;
				smokeCoordBuffer[smokeIndex][0] = IS3XY[0] - 5 * sin((IS3TurretAngle + IS3HullAngle) * PI / 180);
				smokeCoordBuffer[smokeIndex][1] = IS3XY[1] - 5 * cos((IS3TurretAngle + IS3HullAngle) * PI / 180);
				smokeDurBuffer[smokeIndex] = 0.015;
				smokeAngleBuffer[smokeIndex] = rand() % 360;
				smokeIDBufferSet[smokeIndex] = !smokeIDBufferSet[smokeIndex];
				smokeActive[smokeIndex] = true;
				smokeIndex++;
			}
		}
	}
	// force a call to :
	//glutSetWindow(MainWindow);
	//glutPostRedisplay();
}
void Display()
{
	if (DebugOn != 0)
	{
		fprintf(stderr, "Display\n");
	}

	glutSetCursor(GLUT_CURSOR_NONE);
	// set which window we want to do the graphics into:

	glutSetWindow(MainWindow);


	// erase the background:

	glDrawBuffer(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (DepthBufferOn != 0)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);


	// specify shading to be flat:

	glShadeModel(GL_FLAT);


	// set the viewport to a square centered in the window:

	GLsizei vx = glutGet(GLUT_WINDOW_WIDTH);
	GLsizei vy = glutGet(GLUT_WINDOW_HEIGHT);
	GLsizei v = vx < vy ? vx : vy;			// minimum dimension
	v = vx > vy ? vx : vy;
	GLint xl = (vx - v) / 2;
	GLint yb = (vy - v) / 2;
	glViewport(xl, yb, v, v);

	// set the viewing volume:
	// remember that the Z clipping  values are actually
	// given as DISTANCES IN FRONT OF THE EYE
	// USE gluOrtho2D( ) IF YOU ARE DOING 2D !

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (WhichProjection == ORTHO)
		glOrtho(-3., 3., -3., 3., 0.1, 1000.);
	else
		gluPerspective(90., 1., 0.1, 1000.);


	// place the objects into the scene:

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	// set the eye position, look-at position, and up-vector:

	gluLookAt(eyex, eyey, eyez, targetx, targety, targetz, upx, upy, upz);
	//gluLookAt( -0.4, 1.85, -4.85,     0., 0., -20.,     0., 1., 0. );

	// rotate the scene:

	glRotatef((GLfloat)Yrot, 0., 1., 0.);
	glRotatef((GLfloat)Xrot, 1., 0., 0.);


	// uniformly scale the scene:

	if (Scale < MINSCALE)
		Scale = MINSCALE;
	glScalef((GLfloat)Scale, (GLfloat)Scale, (GLfloat)Scale);


	// set the fog parameters:

	if (DepthCueOn != 0)
	{
		glFogi(GL_FOG_MODE, FOGMODE);
		glFogfv(GL_FOG_COLOR, FOGCOLOR);
		glFogf(GL_FOG_DENSITY, FOGDENSITY);
		glFogf(GL_FOG_START, FOGSTART);
		glFogf(GL_FOG_END, FOGEND);
		glEnable(GL_FOG);
	}
	else
	{
		glDisable(GL_FOG);
	}


	// possibly draw the axes:

	if (AxesOn == 0)
	{
		glColor3fv(&Colors[WhichColor][0]);
		glCallList(AxesList);
	}


	// since we are using glScalef( ), be sure normals get unitized:
	
	Pattern->Use();
	Pattern->SetUniformVariable((char *)"uKa", (float)0.05);
	Pattern->SetUniformVariable((char *)"uKd", (float)1);
	Pattern->SetUniformVariable((char *)"uKs", (float)1);
	Pattern->SetUniformVariable((char *)"uShininess", (float)80);
	glEnable(GL_NORMALIZE);
	glBegin(GL_QUADS);
	glPushMatrix();
	//SetMaterial(0.05, 0.05, 0, 1.0);
	glColor3f(0.05, 0.05, 0.0);
	glVertex3f(MAPEDGEX + CUBESIZE,0, MAPEDGEY + CUBESIZE);
	glVertex3f(MAPEDGEX + CUBESIZE, 0, -MAPEDGEY - CUBESIZE);
	glVertex3f(-MAPEDGEX - CUBESIZE, 0, -MAPEDGEY - CUBESIZE);
	glVertex3f(-MAPEDGEX - CUBESIZE, 0, MAPEDGEY + CUBESIZE);
	glPopMatrix();
	glEnd();
	Pattern->Use(0);
	// draw the current object:
	//glCallList(BoxList);
	// Costume polys for each frame (instapoly):__________________________________________________________________________________________________________________________
	if (res)
	{
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		if (myAIKB.isAI)
		{
			myAIKB.agent->getMove(myAIKB.AIID, keyBuffer);
		}
		KeyHandler();
		
		if (shake)
		{
			if ((Time - shakeStartTime) < shakeDuration)
			{
				alSourcePlay(Sources[3]);
				eyex += sin((Time - shakeStartTime) * 5000) / 2;
				eyey += sin((Time - shakeStartTime) * 5000) / 2;
				if (AbramHP <= 0)
				{
					// Push the GL attribute bits so that we don't wreck any settings
					glPushAttrib(GL_ALL_ATTRIB_BITS);
					// Enable polygon offsets, and offset filled polygons forward by 2.5
					glEnable(GL_POLYGON_OFFSET_FILL);
					glPolygonOffset(-2.5f, -2.5f);
					// Set the render mode to be line rendering with a thick line width
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					glLineWidth(3.0f);
					// Set the colour to be white
					glColor3f(.5, .5, .5);
					// Render the object
					drawExplosion(AbramXY[0], AbramXY[1],0, 0, 0.5, 1 - abs(sin((Time - shakeStartTime) * 500)), 0.75 - 3*abs(sin((Time - shakeStartTime) * 500))/4, 0, shakeStartTime, shakeDuration / 2);
					drawExplosion(AbramXY[0], AbramXY[1],0, 60, 0.5, 1 - abs(sin((Time - shakeStartTime) * 500)), 0.75 - 3*abs(sin((Time - shakeStartTime) * 500))/4, 0, shakeStartTime, shakeDuration / 2);
					drawExplosion(AbramXY[0], AbramXY[1], 0,120, 0.5, 1 - abs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * abs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					drawExplosion(AbramXY[0], AbramXY[1], 0,180, 0.5, 1 - abs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * abs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					drawExplosion(AbramXY[0], AbramXY[1], 0,240, 0.5, 1 - abs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * abs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					drawExplosion(AbramXY[0], AbramXY[1], 0, 300, 0.5, 1 - abs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * abs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					// Set the polygon mode to be filled triangles 
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glShadeModel(GL_FLAT);
					glEnable(GL_LIGHTING);
					SetPointLight(GL_LIGHT1, 0, 50, 0, 0.9, 0.9, 0.9);
					glColor3f(0.0f, 0.0f, 0.0f);
					drawExplosion(AbramXY[0], AbramXY[1], 0, 0, 0.5, 1 - abs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * abs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					drawExplosion(AbramXY[0], AbramXY[1], 0, 60, 0.5, 1 - abs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * abs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					drawExplosion(AbramXY[0], AbramXY[1], 0, 120, 0.5, 1 - abs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * abs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					drawExplosion(AbramXY[0], AbramXY[1], 0, 180, 0.5, 1 - abs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * abs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					drawExplosion(AbramXY[0], AbramXY[1], 0, 240, 0.5, 1 - abs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * abs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					drawExplosion(AbramXY[0], AbramXY[1], 0, 300, 0.5, 1 - abs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * abs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					glPopAttrib();
					glDisable(GL_LIGHT1);
					glDisable(GL_LIGHTING);
					
				}
				if (IS3HP <= 0)
				{
					// Push the GL attribute bits so that we don't wreck any settings
					glPushAttrib(GL_ALL_ATTRIB_BITS);
					// Enable polygon offsets, and offset filled polygons forward by 2.5
					glEnable(GL_POLYGON_OFFSET_FILL);
					glPolygonOffset(-2.5f, -2.5f);
					// Set the render mode to be line rendering with a thick line width
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					glLineWidth(3.0f);
					// Set the colour to be white
					glColor3f(.5, .5, .5);
					// Render the object
					drawExplosion(IS3XY[0], IS3XY[1], 0, 0, 0.5, 1 - abs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * abs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					drawExplosion(IS3XY[0], IS3XY[1], 0, 60, 0.5, 1 - abs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * abs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					drawExplosion(IS3XY[0], IS3XY[1], 0, 120, 0.5, 1 - abs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * abs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					drawExplosion(IS3XY[0], IS3XY[1], 0, 180, 0.5, 1 - abs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * abs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					drawExplosion(IS3XY[0], IS3XY[1], 0, 240, 0.5, 1 - abs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * abs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					drawExplosion(IS3XY[0], IS3XY[1], 0, 300, 0.5, 1 - abs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * abs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					// Set the polygon mode to be filled triangles 
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glShadeModel(GL_FLAT);
					glEnable(GL_LIGHTING);
					SetPointLight(GL_LIGHT1, 0, 50, 0, 0.9, 0.9, 0.9);
					glColor3f(0.0f, 0.0f, 0.0f);
					drawExplosion(IS3XY[0], IS3XY[1], 0, 0, 0.5, 1 - abs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * abs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					drawExplosion(IS3XY[0], IS3XY[1], 0, 60, 0.5, 1 - abs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * abs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					drawExplosion(IS3XY[0], IS3XY[1], 0, 120, 0.5, 1 - abs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * abs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					drawExplosion(IS3XY[0], IS3XY[1], 0, 180, 0.5, 1 - abs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * abs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					drawExplosion(IS3XY[0], IS3XY[1], 0, 240, 0.5, 1 - abs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * abs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					drawExplosion(IS3XY[0], IS3XY[1], 0, 300, 0.5, 1 - abs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * abs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					glPopAttrib();
					glDisable(GL_LIGHT1);
					glDisable(GL_LIGHTING);
				}
			}
			else
			{
				eyex = CAMX;
				eyey = CAMY;
				shake = false;
			}
		}
		// Draw shell UI
		// Push the GL attribute bits so that we don't wreck any settings
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		// Enable polygon offsets, and offset filled polygons forward by 2.5
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(-2.5f, -2.5f);
		// Set the render mode to be line rendering with a thick line width
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(3.0f);
		// Set the colour to be white
		glColor3f(.5, .5, .5);
		// Render the object
		glBegin(GL_LINE_STRIP);
		glColor3f(1, 1, 0);
		glVertex3f(MAPEDGEX + 20, 3, -MAPEDGEY);
		glVertex3f(MAPEDGEX + 20, 3, -MAPEDGEY + AbramSmoke * 7);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glColor3f(0, 0, 1);
		glVertex3f(MAPEDGEX + 20, 3, MAPEDGEY);
		glVertex3f(MAPEDGEX + 20, 3, MAPEDGEY - IS3Smoke * 7 );
		glEnd();
		glLineWidth(5.0f);
		if (AbramSmoke > 0)
			drawSmokeCrate(MAPEDGEX + 22, -MAPEDGEY + AbramSmoke * 7 + 1,90);
		if (IS3Smoke > 0)
			drawSmokeCrate(MAPEDGEX + 22, MAPEDGEY - IS3Smoke * 7 - 1,270);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glShadeModel(GL_FLAT);
		glEnable(GL_LIGHTING);
		SetPointLight(GL_LIGHT1, 0, 50, 0, 0.9, 0.9, 0.9);
		glColor3f(0.0f, 0.0f, 0.0f);
		if (AbramSmoke > 0)
			drawSmokeCrate(MAPEDGEX + 22, -MAPEDGEY + AbramSmoke * 7 + 1,90);
		if (IS3Smoke > 0)
			drawSmokeCrate(MAPEDGEX + 22, MAPEDGEY - IS3Smoke * 7 - 1,270);
		glPopAttrib();
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHTING);
		// Push the GL attribute bits so that we don't wreck any settings
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		// Enable polygon offsets, and offset filled polygons forward by 2.5
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(-2.5f, -2.5f);
		// Set the render mode to be line rendering with a thick line width
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(10.0f);
		// Set the colour to be white
		glColor3f(.5, .5, .5);
		// Render the object
		glBegin(GL_LINE_STRIP);
		glColor3f(1,1,0);
		glVertex3f(MAPEDGEX + 15,3, -MAPEDGEY);
		glVertex3f(MAPEDGEX + 15, 3 ,-MAPEDGEY + AbramShells * 2);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glColor3f(0,0,1);
		glVertex3f(MAPEDGEX + 15,3, MAPEDGEY);
		glVertex3f(MAPEDGEX + 15,3, MAPEDGEY - IS3Shells * 2);
		glEnd();
		glLineWidth(3.0f);
		if(AbramShells > 0)
			drawShell(MAPEDGEX + 15, -MAPEDGEY + AbramShells * 2, 180,4);
		if (IS3Shells > 0)
			drawShell(MAPEDGEX + 15, MAPEDGEY - IS3Shells * 2, 0,4);
		// Set the polygon mode to be filled triangles 
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glShadeModel(GL_FLAT);
		glEnable(GL_LIGHTING);
		SetPointLight(GL_LIGHT1, 0, 50, 0, 0.9, 0.9, 0.9);
		glColor3f(0.0f, 0.0f, 0.0f);
		if (AbramShells > 0)
			drawShell(MAPEDGEX + 15, -MAPEDGEY + AbramShells * 2, 180,4);
		if (IS3Shells > 0)
			drawShell(MAPEDGEX + 15, MAPEDGEY - IS3Shells * 2, 0,4);
		glPopAttrib();
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHTING);
		// Draw Tanks
		// Push the GL attribute bits so that we don't wreck any settings
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		// Enable polygon offsets, and offset filled polygons forward by 2.5
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(-2.5f, -2.5f);
		// Set the render mode to be line rendering with a thick line width
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(3.0f);
		// Set the colour to be white
		glColor3f(.5, .5, .5);
		// Render the object
		if(AbramHP > 0)
			drawAbram(AbramXY[0], -0.25, AbramXY[1], AbramHullAngle, AbramTurretAngle);
		else
		{
			drawAbramDead(AbramXY[0], -0.25, AbramXY[1], AbramHullAngle, AbramTurretAngle);
			if (!shakeOnce)
			{
				shake = true;
				shakeOnce = true;
				shakeStartTime = Time;
			}
		}
		if (IS3HP > 0)
			drawIS3  (IS3XY[0]  , -0.25, IS3XY[1]  , IS3HullAngle  , IS3TurretAngle);
		else
		{
			drawIS3Dead(IS3XY[0], -0.25, IS3XY[1], IS3HullAngle, IS3TurretAngle);
			drawAbramDead(AbramXY[0], -0.25, AbramXY[1], AbramHullAngle, AbramTurretAngle);
			if (!shakeOnce)
			{
				shake = true;
				shakeOnce = true;
				shakeStartTime = Time;
			}
			
		}
		// Set the polygon mode to be filled triangles 
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glShadeModel(GL_FLAT);
		glEnable(GL_LIGHTING);

		SetPointLight(GL_LIGHT0, 0 , 50, 0, 0.75, 0.75, 0.75);
		// Set the colour to the background
		glColor3f(0.0f, 0.0f, 0.0f);
		// Render the object
		if (AbramHP > 0)
			drawAbram(AbramXY[0], -0.25, AbramXY[1], AbramHullAngle, AbramTurretAngle);
		else
			drawAbramDead(AbramXY[0], -0.25, AbramXY[1], AbramHullAngle, AbramTurretAngle);
		if (IS3HP > 0)
			drawIS3  (IS3XY[0], -0.25, IS3XY[1], IS3HullAngle, IS3TurretAngle);
		else
			drawIS3Dead(IS3XY[0], -0.25, IS3XY[1], IS3HullAngle, IS3TurretAngle);
		// Pop the state changes off the attribute stack
		// to set things back how they were
		glPopAttrib();
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);

		Pattern->Use();
		// draw map border
		for(int i = 0; i < (2*MAPEDGEX)/CUBESIZE + 2; i++)
			drawCube(-MAPEDGEX - CUBESIZE / 2 - 2 + i*CUBESIZE,-MAPEDGEY - CUBESIZE,0,0.5,0.5,0.5);
		for (int i = 0; i < (2 * MAPEDGEX) / CUBESIZE + 2; i++)
			drawCube(-MAPEDGEX - CUBESIZE / 2 - 2 + i*CUBESIZE, MAPEDGEY + CUBESIZE,0, 0.5, 0.5, 0.5);
		for (int i = 0; i < (2 * MAPEDGEY) / CUBESIZE + 2; i++)
			drawCube( -MAPEDGEX - CUBESIZE, -MAPEDGEY - CUBESIZE + i*CUBESIZE,0, 0.5, 0.5, 0.5);
		for (int i = 0; i < (2 * MAPEDGEY) / CUBESIZE + 2; i++)
			drawCube( MAPEDGEX + CUBESIZE, -MAPEDGEY - CUBESIZE + i*CUBESIZE,0, 0.5, 0.5, 0.5);
		Pattern->Use(0);
		//test ammo power up
		
		
		
		// Draw Map
		for (int j = 0; j < 14; j++)
		{
			for (int i = 0; i < 24; i++)
			{
				if (myMap.MCM[i][j] && myMap.isSolid[i][j] && (myMap.color[i][j][0] != 7))
				{
					if (destructionTimeBuffer[i][j] > 0 && (Time - destructionTimeBuffer[i][j]) > 0 && (Time - destructionTimeBuffer[i][j]) < 0.0025)
						myMap.coord[i][j][2] -= (Time - destructionTimeBuffer[i][j]) * 500;
					if (destructionTimeBuffer[i][j] > 0 && (Time - destructionTimeBuffer[i][j]) >= 0.0025)
					{
						myMap.MCM[i][j] = false;
						myMap.isSolid[i][j] = false;
					}
					drawCube(myMap.coord[i][j][0], myMap.coord[i][j][1], myMap.coord[i][j][2], myMap.color[i][j][0], myMap.color[i][j][1], myMap.color[i][j][2]);
				}
				if ((myMap.MCM[i][j] && !myMap.isSolid[i][j]) || (myMap.color[i][j][0] == 7))
				{
					// Push the GL attribute bits so that we don't wreck any settings
					glPushAttrib(GL_ALL_ATTRIB_BITS);
					// Enable polygon offsets, and offset filled polygons forward by 2.5
					glEnable(GL_POLYGON_OFFSET_FILL);
					glPolygonOffset(-2.5f, -2.5f);
					// Set the render mode to be line rendering with a thick line width
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					glLineWidth(3.0f);
					// Set the colour to be white
					glColor3f(.5, .5, .5);
					// Render the object
					drawTreeCube(myMap.coord[i][j][0], myMap.coord[i][j][1],myMap.angle[i][j], myMap.color[i][j][0]);
					// Set the polygon mode to be filled triangles 
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glShadeModel(GL_FLAT);
					glEnable(GL_LIGHTING);
					SetPointLight(GL_LIGHT1, 0, 50, 0, 0.9, 0.9, 0.9);
					glColor3f(0.0f, 0.0f, 0.0f);
					drawTreeCube(myMap.coord[i][j][0], myMap.coord[i][j][1], myMap.angle[i][j], myMap.color[i][j][0]);
					glPopAttrib();
					glDisable(GL_LIGHT1);
					glDisable(GL_LIGHTING);
				}
			}
		}
		
		//Smoke
		for (int i = 0; i < 1000; i=i+10)
		{
			if (smokeActive[i]) {
				// Push the GL attribute bits so that we don't wreck any settings
				glPushAttrib(GL_ALL_ATTRIB_BITS);
				// Enable polygon offsets, and offset filled polygons forward by 2.5
				glEnable(GL_POLYGON_OFFSET_FILL);
				glPolygonOffset(-2.5f, -2.5f);
				// Set the render mode to be line rendering with a thick line width
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glLineWidth(3.0f);
				// Set the colour to be white
				glColor3f(.5, .5, .5);
				// Render the object
				drawSmoke(smokeCoordBuffer[i][0], smokeCoordBuffer[i][1], 0, smokeAngleBuffer[i], 0.05, 0.59, 0.52, 0.48, smokeIDBuffer[i], smokeDurBuffer[i]);
				// Set the polygon mode to be filled triangles 
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glShadeModel(GL_FLAT);
				glEnable(GL_LIGHTING);
				SetPointLight(GL_LIGHT1, 0, 50, 0, 0.9, 0.9, 0.9);
				glColor3f(0.0f, 0.0f, 0.0f);
				drawSmoke(smokeCoordBuffer[i][0], smokeCoordBuffer[i][1], 0, smokeAngleBuffer[i], 0.05, 0.59, 0.52, 0.48, smokeIDBuffer[i], smokeDurBuffer[i]);
				glPopAttrib();
				glDisable(GL_LIGHT1);
				glDisable(GL_LIGHTING);
				if ((Time - smokeIDBuffer[i]) > smokeDurBuffer[i])
					smokeActive[i] = false;
			}
		}
		//draw crates
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		// Enable polygon offsets, and offset filled polygons forward by 2.5
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(-2.5f, -2.5f);
		// Set the render mode to be line rendering with a thick line width
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(3.0f);
		// Set the colour to be white
		glColor3f(.5, .5, .5);
		// Render the object
		for (int i = 0; i < CRATECAP; i++)
		{
			if (Crates[i].isActive)
				switch (Crates[i].type)
				{
				case 0:
					drawAmmo(Crates[i].X, Crates[i].Y);
					break;
				case 1:
					drawSmokeCrate(Crates[i].X, Crates[i].Y);
					break;
				case 2:
					drawHPCrate(Crates[i].X, Crates[i].Y);
					break;
				}
		}
		// Set the polygon mode to be filled triangles 
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glShadeModel(GL_FLAT);
		glEnable(GL_LIGHTING);
		SetPointLight(GL_LIGHT1, 0, 50, 0, 0.9, 0.9, 0.9);
		glColor3f(0.0f, 0.0f, 0.0f);
		for (int i = 0; i < 9; i++)
		{
			if (Crates[i].isActive)
				switch (Crates[i].type)
				{
				case 0:
					drawAmmo(Crates[i].X, Crates[i].Y);
					break;
				case 1:
					drawSmokeCrate(Crates[i].X, Crates[i].Y);
					break;
				case 2:
					drawHPCrate(Crates[i].X, Crates[i].Y);
					break;
				}
		}
		glPopAttrib();
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHTING);
		// draw shell
		for (int i = 0; i < SHELLMAX; i++)
		{
			if ((Time - Shells[i].startTime) < SHELLDURATION && Shells[i].active)
			{
				// Push the GL attribute bits so that we don't wreck any settings
				glPushAttrib(GL_ALL_ATTRIB_BITS);
				// Enable polygon offsets, and offset filled polygons forward by 2.5
				glEnable(GL_POLYGON_OFFSET_FILL);
				glPolygonOffset(-2.5f, -2.5f);
				// Set the render mode to be line rendering with a thick line width
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glLineWidth(3.0f);
				// Set the colour to be white
				glColor3f(.5, .5, .5);
				// Render the object
				drawShell(Shells[i].x - ((Time - Shells[i].startTime) * SHELLSPEED * sin(Shells[i].angle * PI / 180)), Shells[i].y - ((Time - Shells[i].startTime) * SHELLSPEED * cos(Shells[i].angle * PI / 180)), Shells[i].angle);
				// Set the polygon mode to be filled triangles 
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glShadeModel(GL_FLAT);
				glEnable(GL_LIGHTING);
				SetPointLight(GL_LIGHT1, 0, 50, 0, 0.9, 0.9, 0.9);
				glColor3f(0.0f, 0.0f, 0.0f);
				drawShell(Shells[i].x - ((Time - Shells[i].startTime) * SHELLSPEED * sin(Shells[i].angle * PI / 180)), Shells[i].y - ((Time - Shells[i].startTime) * SHELLSPEED * cos(Shells[i].angle * PI / 180)), Shells[i].angle);
				glPopAttrib();
				glDisable(GL_LIGHT1);
				glDisable(GL_LIGHTING);
				// Calculate trajectory:
				if (
					((Shells[i].x - ((Time - Shells[i].startTime) * SHELLSPEED * sin(Shells[i].angle * PI / 180)) < IS3XY[0] + BODY) && (Shells[i].x - ((Time - Shells[i].startTime) * SHELLSPEED * sin(Shells[i].angle * PI / 180)) > IS3XY[0] - BODY)) &&
					((Shells[i].y - ((Time - Shells[i].startTime) * SHELLSPEED * cos(Shells[i].angle * PI / 180)) < IS3XY[1] + BODY) && (Shells[i].y - ((Time - Shells[i].startTime) * SHELLSPEED * cos(Shells[i].angle * PI / 180)) > IS3XY[1] - BODY)) &&
					Shells[i].shooterId == ABRAMID
				)
				{
					//Dammage:
					IS3HP -= 2 * abs(abs(sin((Shells[i].angle - IS3HullAngle)* PI / 180)) - abs(cos((Shells[i].angle - IS3HullAngle)* PI / 180)));
					// Bounce!
					if (abs(abs(sin((Shells[i].angle - IS3HullAngle)* PI / 180)) - abs(cos((Shells[i].angle - IS3HullAngle)* PI / 180))) < BOUNCETHRESH)
					{
						alSourcePlay(Sources[2]);
						if (IS3XY[0] > Shells[i].x)
						{
							if (IS3XY[1] > Shells[i].y)
								if (IS3XY[0] - Shells[i].x >= IS3XY[1] - Shells[i].y)
									if(abs(sin((Shells[i].angle - IS3HullAngle)* PI / 180)) - abs(cos((Shells[i].angle - IS3HullAngle)* PI / 180)) >=0)
										Shells[i].angle += 90;
									else
										Shells[i].angle -= 90;
								else
									if (abs(sin((Shells[i].angle - IS3HullAngle)* PI / 180)) - abs(cos((Shells[i].angle - IS3HullAngle)* PI / 180)) >= 0)
										Shells[i].angle -= 90;
									else
										Shells[i].angle += 90;
							else
								if (IS3XY[0] - Shells[i].x >= IS3XY[1] - Shells[i].y)
									if (abs(sin((Shells[i].angle - IS3HullAngle)* PI / 180)) - abs(cos((Shells[i].angle - IS3HullAngle)* PI / 180)) >= 0)
										Shells[i].angle -= 90;
									else
										Shells[i].angle += 90;
								else
									if (abs(sin((Shells[i].angle - IS3HullAngle)* PI / 180)) - abs(cos((Shells[i].angle - IS3HullAngle)* PI / 180)) >= 0)
										Shells[i].angle += 90;
									else
										Shells[i].angle -= 90;
						}
						else
						{
							if (IS3XY[1] > Shells[i].y)
								if (IS3XY[0] - Shells[i].x >= IS3XY[1] - Shells[i].y)
									Shells[i].angle -= 90;
								else
									Shells[i].angle += 90;
							else
								if (IS3XY[0] - Shells[i].x >= IS3XY[1] - Shells[i].y)
									Shells[i].angle += 90;
								else
									Shells[i].angle -= 90;
						}
						Shells[i].startTime = Time;
						Shells[i].shooterId = IS3ID;
						Shells[i].x = IS3XY[0];
						Shells[i].y = IS3XY[1];
					}
					else
					{
						Shells[i].active = false;
					}
				}
				if (
					((Shells[i].x - ((Time - Shells[i].startTime) * SHELLSPEED * sin(Shells[i].angle * PI / 180)) < AbramXY[0] + BODY) && (Shells[i].x - ((Time - Shells[i].startTime) * SHELLSPEED * sin(Shells[i].angle * PI / 180)) > AbramXY[0] - BODY)) &&
					((Shells[i].y - ((Time - Shells[i].startTime) * SHELLSPEED * cos(Shells[i].angle * PI / 180)) < AbramXY[1] + BODY) && (Shells[i].y - ((Time - Shells[i].startTime) * SHELLSPEED * cos(Shells[i].angle * PI / 180)) > AbramXY[1] - BODY)) &&
					Shells[i].shooterId == IS3ID
				)
				{
					//Dammage:
					AbramHP -= 2 * abs(abs(sin((Shells[i].angle - AbramHullAngle)* PI / 180)) - abs(cos((Shells[i].angle - AbramHullAngle)* PI / 180)));
					// Bounce!
					if (abs(abs(sin((Shells[i].angle - AbramHullAngle)* PI / 180)) - abs(cos((Shells[i].angle - AbramHullAngle)* PI / 180))) < BOUNCETHRESH)
					{
						alSourcePlay(Sources[2]);
						if (AbramXY[0] > Shells[i].x)
						{
							if (AbramXY[1] > Shells[i].y)
								if (AbramXY[0] - Shells[i].x >= AbramXY[1] - Shells[i].y)
									if (abs(sin((Shells[i].angle - AbramHullAngle)* PI / 180)) - abs(cos((Shells[i].angle - AbramHullAngle)* PI / 180)) >= 0)
										Shells[i].angle += 90;
									else
										Shells[i].angle -= 90;
								else
									if (abs(sin((Shells[i].angle - AbramHullAngle)* PI / 180)) - abs(cos((Shells[i].angle - AbramHullAngle)* PI / 180)) >= 0)
										Shells[i].angle -= 90;
									else
										Shells[i].angle += 90;
							else
								if (AbramXY[0] - Shells[i].x >= AbramXY[1] - Shells[i].y)
									if (abs(sin((Shells[i].angle - AbramHullAngle)* PI / 180)) - abs(cos((Shells[i].angle - AbramHullAngle)* PI / 180)) >= 0)
										Shells[i].angle -= 90;
									else
										Shells[i].angle += 90;
								else
									if (abs(sin((Shells[i].angle - AbramHullAngle)* PI / 180)) - abs(cos((Shells[i].angle - AbramHullAngle)* PI / 180)) >= 0)
										Shells[i].angle += 90;
									else
										Shells[i].angle -= 90;
						}
						else
						{
							if (AbramXY[1] > Shells[i].y)
								if (AbramXY[0] - Shells[i].x >= AbramXY[1] - Shells[i].y)
									Shells[i].angle -= 90;
								else
									Shells[i].angle += 90;
							else
								if (AbramXY[0] - Shells[i].x >= AbramXY[1] - Shells[i].y)
									Shells[i].angle += 90;
								else
									Shells[i].angle -= 90;
						}
						Shells[i].startTime = Time;
						Shells[i].shooterId = ABRAMID;
						Shells[i].x = AbramXY[0];
						Shells[i].y = AbramXY[1];
					}
					else
					{
						Shells[i].active = false;
					}
				}
				if(
					(Shells[i].x - ((Time - Shells[i].startTime) * SHELLSPEED * sin(Shells[i].angle * PI / 180)) < -MAPEDGEX) ||
					(Shells[i].x - ((Time - Shells[i].startTime) * SHELLSPEED * sin(Shells[i].angle * PI / 180)) >  MAPEDGEX) ||
					(Shells[i].y - ((Time - Shells[i].startTime) * SHELLSPEED * cos(Shells[i].angle * PI / 180)) < -MAPEDGEY) ||
					(Shells[i].y - ((Time - Shells[i].startTime) * SHELLSPEED * cos(Shells[i].angle * PI / 180)) >  MAPEDGEY)
				)
					Shells[i].active = false;
				int tmpi;
				int tmpj;
				//CrateCollisionModel
				int cratecheck = CrateCollisionModel(Shells[i].x, Shells[i].y,
					(Time - Shells[i].startTime) * SHELLSPEED * sin(Shells[i].angle * PI / 180),
					(Time - Shells[i].startTime) * SHELLSPEED * cos(Shells[i].angle * PI / 180),
					-1
				);
				if (cratecheck != CRATECAP)
				{
					Crates[cratecheck].isActive = false;
					myMap.isCrate[Crates[cratecheck].i][Crates[cratecheck].j] = false;
				}
				if (
					MapCollisionModel(Shells[i].x, Shells[i].y,
					(Time - Shells[i].startTime) * SHELLSPEED * sin(Shells[i].angle * PI / 180),
						(Time - Shells[i].startTime) * SHELLSPEED * cos(Shells[i].angle * PI / 180),
						-1,
						&tmpi,
						&tmpj
					)
				)
				{
					CrateIndex++;	// randomly generate crates
					if (CrateIndex > CRATECAP)
						CrateIndex = 0;
					int wallState = rand() % 8;
					myMap.isCrate[tmpi][tmpj] = true;
					switch (wallState)
					{
					case 0:
					case 1:
					case 2:
						Crates[CrateIndex].type = AMMOCRATE;
						Crates[CrateIndex].X = myMap.coord[tmpi][tmpj][0] ;
						Crates[CrateIndex].Y = myMap.coord[tmpi][tmpj][1];
						Crates[CrateIndex].isActive = true;
						break;
					case 3:
						Crates[CrateIndex].type = SMOKECRATE;
						Crates[CrateIndex].X = myMap.coord[tmpi][tmpj][0];
						Crates[CrateIndex].Y = myMap.coord[tmpi][tmpj][1];
						Crates[CrateIndex].isActive = true;
						break;
					case 4:
						Crates[CrateIndex].type = HPCRATE;
						Crates[CrateIndex].X = myMap.coord[tmpi][tmpj][0];
						Crates[CrateIndex].Y = myMap.coord[tmpi][tmpj][1];
						Crates[CrateIndex].isActive = true;
						break;
					/*case 5:
						Crates[CrateIndex].type = HPCRATE;// RELOADCRATE;
						Crates[CrateIndex].X = myMap.coord[tmpi][tmpj][0];
						Crates[CrateIndex].Y = myMap.coord[tmpi][tmpj][1];
						Crates[CrateIndex].isActive = true;
						break;
					case 6:
						Crates[CrateIndex].type = HPCRATE;// DAMAGECRATE;
						Crates[CrateIndex].X = myMap.coord[tmpi][tmpj][0];
						Crates[CrateIndex].Y = myMap.coord[tmpi][tmpj][1];
						Crates[CrateIndex].isActive = true;
						break;
					case 7:
						Crates[CrateIndex].type = HPCRATE;// RAMSPEEDCRATE;
						Crates[CrateIndex].X = myMap.coord[tmpi][tmpj][0];
						Crates[CrateIndex].Y = myMap.coord[tmpi][tmpj][1];
						Crates[CrateIndex].isActive = true;
						break;*/
					}
					for (int i = 0; i < 50; i++)
					{
						if (smokeIndex >= 1000)
							smokeIndex = 0;
						smokeIDBuffer[smokeIndex] = Time;
						smokeCoordBuffer[smokeIndex][0] = myMap.coord[tmpi][tmpj][0];
						smokeCoordBuffer[smokeIndex][1] = myMap.coord[tmpi][tmpj][1];
						smokeAngleBuffer[smokeIndex] = rand() % 360;
						smokeDurBuffer[smokeIndex] = 0.02;
						smokeIDBufferSet[smokeIndex] = !smokeIDBufferSet[smokeIndex];
						smokeActive[smokeIndex] = true;
						smokeIndex++;
					}
					destructionTimeBuffer[tmpi][tmpj] = Time;
					Shells[i].active = false;
				}
			}
		}
		glDisableVertexAttribArray(0);
	}

	if (DepthFightingOn != 0)
	{
		glPushMatrix();
		glRotatef(90., 0., 1., 0.);
		glCallList(BoxList);
		glPopMatrix();
	}


	// draw some gratuitous text that just rotates on top of the scene:

	glDisable(GL_DEPTH_TEST);
	// Winner Text:
	if (AbramHP <= 0 && IS3HP <= 0)
	{
		glColor3f(1., 1., 1.);
		DoRasterString(0., 1., -1, (char *)"Draw!");
	}
	else 
	{
		if (AbramHP <= 0)
		{
			glColor3f(0, 0, 1.);
			DoRasterString(0., 1., -1, (char *)"Blue player Wins!");
		}
		if (IS3HP <= 0)
		{
			glColor3f(1, 1, 0);
			DoRasterString(0., 1., -1, (char *)"Yellow player Wins!");
		}
	}
	if (AbramShells == 0)
	{
		glColor3f(1-sin(Time*1000), 1 - sin(Time * 1000), 0);
		DoRasterString(MAPEDGEX + 15, 3, -MAPEDGEY, (char *)"OUT OF AMMO!");
	}
	if (IS3Shells == 0)
	{
		glColor3f(0,0, 1 - sin(Time * 1000));
		DoRasterString(MAPEDGEX + 15, 3, MAPEDGEY - 15, (char *)"OUT OF AMMO!");
	}
	if (AbramSmoke == 0)
	{
		glColor3f(1 - sin(Time * 1000), 1 - sin(Time * 1000), 0);
		DoRasterString(MAPEDGEX + 22, 3, -MAPEDGEY, (char *)"OUT OF SMOKE!");
	}
	if (IS3Smoke == 0)
	{
		glColor3f(0, 0, 1 - sin(Time * 1000));
		DoRasterString(MAPEDGEX + 22, 3, MAPEDGEY - 15, (char *)"OUT OF SMOKE!");
	}
	// draw some gratuitous text that is fixed on the screen:
	//
	// the projection matrix is reset to define a scene whose
	// world coordinate system goes from 0-100 in each axis
	//
	// this is called "percent units", and is just a convenience
	//
	// the modelview matrix is reset to identity as we don't
	// want to transform these coordinates

	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0., 100., 0., 100.);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(1., 1., 1.);
	//DoRasterString(500., 400, 0., (char *)"Final Project: Tank 2017");


	// swap the double-buffered framebuffers:

	glutSwapBuffers();


	// be sure the graphics buffer has been sent:
	// note: be sure to use glFlush( ) here, not glFinish( ) !

	glFlush();
}
void DoAxesMenu(int id)
{
	AxesOn = id;

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}
void DoColorMenu(int id)
{
	WhichColor = id - RED;

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}
void DoDebugMenu(int id)
{
	DebugOn = id;

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}
void DoDepthBufferMenu(int id)
{
	DepthBufferOn = id;

	glutSetWindow(MainWindow);
	glutPostRedisplay();
} 
void DoDepthFightingMenu(int id)
{
	DepthFightingOn = id;

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}
void DoDepthMenu(int id)
{
	DepthCueOn = id;

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}
void DoCameraMenu(int id)
{
	if (id == 0)
	{
		eyex = 10;
		eyey = 10;
		eyez = 10;
		targetx = 0;
		targety = 0;
		targetz = 0;
		upx = 0;
		upy = 1;
		upz = 0;
	}
	if (id == 1)
	{
		eyex = -0.4;
		eyey = 1.85;
		eyez = -4.85;
		targetx = 0;
		targety = 0;
		targetz = -20;
		upx = 0;
		upy = 1;
		upz = 0;
	}
	if (id == 2)
	{
		eyex = 0;
		eyey = 7;
		eyez = 12;
		targetx = 0;
		targety = 0;
		targetz = -20;
		upx = 0;
		upy = 1;
		upz = 0;
	}
}
void DoMainMenu(int id)
{
	switch (id)
	{
	case RESET:
		Reset();
		break;

	case QUIT:
		// gracefully close out the graphics:
		// gracefully close the graphics window:
		// gracefully exit the program:
		glutSetWindow(MainWindow);
		glFinish();
		glutDestroyWindow(MainWindow);
		exit(0);
		break;

	default:
		fprintf(stderr, "Don't know what to do with Main Menu ID %d\n", id);
	}

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}
void DoProjectMenu(int id)
{
	WhichProjection = id;

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}
void DoRasterString(float x, float y, float z, char *s)
{
	glRasterPos3f((GLfloat)x, (GLfloat)y, (GLfloat)z);

	char c;			// one character to print
	for (; (c = *s) != '\0'; s++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
	}
}
void DoStrokeString(float x, float y, float z, float ht, char *s)
{
	glPushMatrix();
	glTranslatef((GLfloat)x, (GLfloat)y, (GLfloat)z);
	float sf = ht / (119.05f + 33.33f);
	glScalef((GLfloat)sf, (GLfloat)sf, (GLfloat)sf);
	char c;			// one character to print
	for (; (c = *s) != '\0'; s++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, c);
	}
	glPopMatrix();
}
float ElapsedSeconds()
{
	// get # of milliseconds since the start of the program:

	int ms = glutGet(GLUT_ELAPSED_TIME);

	// convert it to seconds:

	return (float)ms / 1000.f;
}
void InitMenus()
{
	glutSetWindow(MainWindow);

	int numColors = sizeof(Colors) / (3 * sizeof(int));
	int colormenu = glutCreateMenu(DoColorMenu);
	for (int i = 0; i < numColors; i++)
	{
		glutAddMenuEntry(ColorNames[i], i);
	}

	int axesmenu = glutCreateMenu(DoAxesMenu);
	glutAddMenuEntry("Off", 0);
	glutAddMenuEntry("On", 1);

	int depthcuemenu = glutCreateMenu(DoDepthMenu);
	glutAddMenuEntry("Off", 0);
	glutAddMenuEntry("On", 1);

	int depthbuffermenu = glutCreateMenu(DoDepthBufferMenu);
	glutAddMenuEntry("Off", 0);
	glutAddMenuEntry("On", 1);

	int depthfightingmenu = glutCreateMenu(DoDepthFightingMenu);
	glutAddMenuEntry("Off", 0);
	glutAddMenuEntry("On", 1);

	int debugmenu = glutCreateMenu(DoDebugMenu);
	glutAddMenuEntry("Off", 0);
	glutAddMenuEntry("On", 1);

	int projmenu = glutCreateMenu(DoProjectMenu);
	glutAddMenuEntry("Orthographic", ORTHO);
	glutAddMenuEntry("Perspective", PERSP);

	int cameramenu = glutCreateMenu(DoCameraMenu);
	glutAddMenuEntry("Main", 0);
	glutAddMenuEntry("Secondary 1", 1);
	glutAddMenuEntry("Secondary 2", 2);

	int mainmenu = glutCreateMenu(DoMainMenu);
	glutAddSubMenu("Axes", axesmenu);
	glutAddSubMenu("Camera", cameramenu);
	glutAddSubMenu("Colors", colormenu);
	glutAddSubMenu("Depth Buffer", depthbuffermenu);
	glutAddSubMenu("Depth Fighting", depthfightingmenu);
	glutAddSubMenu("Depth Cue", depthcuemenu);
	glutAddSubMenu("Projection", projmenu);
	glutAddMenuEntry("Reset", RESET);
	glutAddSubMenu("Debug", debugmenu);
	glutAddMenuEntry("Quit", QUIT);

	// attach the pop-up menu to the right mouse button:

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
void InitGraphics()
{
	// request the display modes:
	// ask for red-green-blue-alpha color, double-buffering, and z-buffering:

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	// set the initial window configuration:

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(INIT_WINDOW_SIZE, INIT_WINDOW_SIZE);

	// open the window and set its title:

	MainWindow = glutCreateWindow(WINDOWTITLE);
	glutSetWindowTitle(WINDOWTITLE);
	glutFullScreen();           // making the window full screen
	// set the framebuffer clear values:

	glClearColor(BACKCOLOR[0], BACKCOLOR[1], BACKCOLOR[2], BACKCOLOR[3]);

	// setup the callback functions:
	// DisplayFunc -- redraw the window
	// ReshapeFunc -- handle the user resizing the window
	// KeyboardFunc -- handle a keyboard input
	// MouseFunc -- handle the mouse button going down or up
	// MotionFunc -- handle the mouse moving with a button down
	// PassiveMotionFunc -- handle the mouse moving with a button up
	// VisibilityFunc -- handle a change in window visibility
	// EntryFunc	-- handle the cursor entering or leaving the window
	// SpecialFunc -- handle special keys on the keyboard
	// SpaceballMotionFunc -- handle spaceball translation
	// SpaceballRotateFunc -- handle spaceball rotation
	// SpaceballButtonFunc -- handle spaceball button hits
	// ButtonBoxFunc -- handle button box hits
	// DialsFunc -- handle dial rotations
	// TabletMotionFunc -- handle digitizing tablet motion
	// TabletButtonFunc -- handle digitizing tablet button hits
	// MenuStateFunc -- declare when a pop-up menu is in use
	// TimerFunc -- trigger something to happen a certain time from now
	// IdleFunc -- what to do when nothing else is going on

	glutSetWindow(MainWindow);
	glutDisplayFunc(Display);
	glutReshapeFunc(Resize);
	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(keyUp);
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(NULL);
	glutVisibilityFunc(Visibility);
	glutEntryFunc(NULL);
	glutSpecialFunc(NULL);
	glutSpaceballMotionFunc(NULL);
	glutSpaceballRotateFunc(NULL);
	glutSpaceballButtonFunc(NULL);
	glutButtonBoxFunc(NULL);
	glutDialsFunc(NULL);
	glutTabletMotionFunc(NULL);
	glutTabletButtonFunc(NULL);
	glutMenuStateFunc(NULL);
	glutTimerFunc(-1, NULL, 0);
	glutIdleFunc(Animate);

	// init glew (a window must be open to do this):

#ifdef WIN32
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		fprintf(stderr, "glewInit Error\n");
	}
	else
		fprintf(stderr, "GLEW initialized OK\n");
	fprintf(stderr, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif
	Pattern = new GLSLProgram();
	bool valid = Pattern->Create((char *)"shaders/lighting2.vert", (char *)"shaders/lighting2.frag");
	if (!valid)
	{
		fprintf(stderr, "Shader cannot be created!\n");
		DoMainMenu(QUIT);
	}
	else
	{
		fprintf(stderr, "Shader created.\n");
	}
	Pattern->SetVerbose(false);

}
void InitializeVertexBuffer(GLuint &theBuffer, GLenum target, GLenum usage, const void* data, int size)
{
	glGenBuffers(1, &theBuffer);
	glBindBuffer(target, theBuffer);
	glBufferData(target, size, data, usage);
	glBindBuffer(target, 0);
}
static void list_audio_devices(const ALCchar *devices)
{
	const ALCchar *device = devices, *next = devices + 1;
	size_t len = 0;

	fprintf(stdout, "Devices list:\n");
	fprintf(stdout, "----------\n");
	while (device && *device != '\0' && next && *next != '\0') {
		fprintf(stdout, "%s\n", device);
		len = strlen(device);
		device += (len + 1);
		next += (len + 2);
	}
	fprintf(stdout, "----------\n");
}
void InitLists()
{
	float dx = BOXSIZE / 2.f;
	float dy = BOXSIZE / 2.f;
	float dz = BOXSIZE / 2.f;
	glutSetWindow(MainWindow);
	device = alcOpenDevice(NULL);
	if (!device)
		exit(0);
	else
		std::cout << "Audio device was created." << std::endl;
	enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
	if (enumeration == AL_FALSE)
		std::cout << "enumeration not supported." << std::endl; // enumeration not supported
	else
		std::cout << "enumeration supported." << std::endl;// enumeration supported
	list_audio_devices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));
	context = alcCreateContext(device, NULL);
	if (!alcMakeContextCurrent(context))
		exit(0);
	else
		std::cout << "Context was created." << std::endl;
	alGenSources((ALuint)1, &mainMusic);
	alGenSources((ALuint)1, &tankShellFire);
	alGenSources((ALuint)1, &tankShellBounce);
	alGenSources((ALuint)1, &tankExplode);
	alGenSources((ALuint)1, &hpRegen);
	alGenSources((ALuint)1, &AmmoSmoke);
	// check for errors

	alSourcef(mainMusic, AL_PITCH, 1);
	// check for errors
	alSourcef(mainMusic, AL_GAIN, 1);
	// check for errors
	alSource3f(mainMusic, AL_POSITION, 0, 0, 0);
	// check for errors
	alSource3f(mainMusic, AL_VELOCITY, 0, 0, 0);
	// check for errors
	alSourcei(mainMusic, AL_LOOPING, AL_FALSE);
	// check for errros

	alSourcef(tankShellFire, AL_PITCH, 1);
	// check for errors
	alSourcef(tankShellFire, AL_GAIN, 1);
	// check for errors
	alSource3f(tankShellFire, AL_POSITION, 0, 0, 0);
	// check for errors
	alSource3f(tankShellFire, AL_VELOCITY, 0, 0, 0);
	// check for errors
	alSourcei(tankShellFire, AL_LOOPING, AL_FALSE);
	// check for errros

	alSourcef(tankShellBounce, AL_PITCH, 1);
	// check for errors
	alSourcef(tankShellBounce, AL_GAIN, 1);
	// check for errors
	alSource3f(tankShellBounce, AL_POSITION, 0, 0, 0);
	// check for errors
	alSource3f(tankShellBounce, AL_VELOCITY, 0, 0, 0);
	// check for errors
	alSourcei(tankShellBounce, AL_LOOPING, AL_FALSE);
	// check for errros

	alSourcef(tankExplode, AL_PITCH, 1);
	// check for errors
	alSourcef(tankExplode, AL_GAIN, 1);
	// check for errors
	alSource3f(tankExplode, AL_POSITION, 0, 0, 0);
	// check for errors
	alSource3f(tankExplode, AL_VELOCITY, 0, 0, 0);
	// check for errors
	alSourcei(tankExplode, AL_LOOPING, AL_FALSE);
	// check for errros

	alSourcef(hpRegen, AL_PITCH, 1);
	// check for errors
	alSourcef(hpRegen, AL_GAIN, 1);
	// check for errors
	alSource3f(hpRegen, AL_POSITION, 0, 0, 0);
	// check for errors
	alSource3f(hpRegen, AL_VELOCITY, 0, 0, 0);
	// check for errors
	alSourcei(hpRegen, AL_LOOPING, AL_FALSE);
	// check for errros

	alSourcef(AmmoSmoke, AL_PITCH, 1);
	// check for errors
	alSourcef(AmmoSmoke, AL_GAIN, 1);
	// check for errors
	alSource3f(AmmoSmoke, AL_POSITION, 0, 0, 0);
	// check for errors
	alSource3f(AmmoSmoke, AL_VELOCITY, 0, 0, 0);
	// check for errors
	alSourcei(AmmoSmoke, AL_LOOPING, AL_FALSE);
	// check for errros

	alGenBuffers(NUM_BUFFERS, Buffers);

	alutLoadWAVFile("sound/song.wav", &format, &data, &size, &freq, &loop);
	alBufferData(Buffers[0], format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);

	alutLoadWAVFile("sound/shot.wav", &format, &data, &size, &freq, &loop);
	alBufferData(Buffers[1], format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);

	alutLoadWAVFile("sound/bounce.wav", &format, &data, &size, &freq, &loop);
	alBufferData(Buffers[2], format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);

	alutLoadWAVFile("sound/explosion.wav", &format, &data, &size, &freq, &loop);
	alBufferData(Buffers[3], format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);

	alutLoadWAVFile("sound/hp.wav", &format, &data, &size, &freq, &loop);
	alBufferData(Buffers[4], format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);

	alutLoadWAVFile("sound/ammo.wav", &format, &data, &size, &freq, &loop);
	alBufferData(Buffers[5], format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);

	// Bind buffers into audio sources.

	alGenSources(NUM_SOURCES, Sources);

	alSourcei(Sources[0], AL_BUFFER, Buffers[0]);
	alSourcef(Sources[0], AL_PITCH, 1.0);
	alSourcef(Sources[0], AL_GAIN, 1.0);
	alSource3f(Sources[0], AL_POSITION, 0, 0, 0);
	alSource3f(Sources[0], AL_VELOCITY, 0, 0, 0);
	alSourcei(Sources[0], AL_LOOPING, AL_TRUE);

	alSourcei(Sources[1], AL_BUFFER, Buffers[1]);
	alSourcef(Sources[1], AL_PITCH, 1.0);
	alSourcef(Sources[1], AL_GAIN, 1.0);
	alSource3f(Sources[1], AL_POSITION, 0, 0, 0);
	alSource3f(Sources[1], AL_VELOCITY, 0, 0, 0);
	alSourcei(Sources[1], AL_LOOPING, AL_FALSE);

	alSourcei(Sources[2], AL_BUFFER, Buffers[2]);
	alSourcef(Sources[2], AL_PITCH, 1.0);
	alSourcef(Sources[2], AL_GAIN, 1.0);
	alSource3f(Sources[2], AL_POSITION, 0, 0, 0);
	alSource3f(Sources[2], AL_VELOCITY, 0, 0, 0);
	alSourcei(Sources[2], AL_LOOPING, AL_FALSE);

	alSourcei(Sources[3], AL_BUFFER, Buffers[3]);
	alSourcef(Sources[3], AL_PITCH, 1.0);
	alSourcef(Sources[3], AL_GAIN, 1.0);
	alSource3f(Sources[3], AL_POSITION, 0, 0, 0);
	alSource3f(Sources[3], AL_VELOCITY, 0, 0, 0);
	alSourcei(Sources[3], AL_LOOPING, AL_FALSE);

	alSourcei(Sources[4], AL_BUFFER, Buffers[4]);
	alSourcef(Sources[4], AL_PITCH, 1.0);
	alSourcef(Sources[4], AL_GAIN, 1.0);
	alSource3f(Sources[4], AL_POSITION, 0, 0, 0);
	alSource3f(Sources[4], AL_VELOCITY, 0, 0, 0);
	alSourcei(Sources[4], AL_LOOPING, AL_FALSE);

	alSourcei(Sources[5], AL_BUFFER, Buffers[5]);
	alSourcef(Sources[5], AL_PITCH, 1.0);
	alSourcef(Sources[5], AL_GAIN, 1.0);
	alSource3f(Sources[5], AL_POSITION, 0, 0, 0);
	alSource3f(Sources[5], AL_VELOCITY, 0, 0, 0);
	alSourcei(Sources[5], AL_LOOPING, AL_FALSE);

	//load graphics
	loadMap();
	loadAll();

	// load audio
	alSourcePlay(Sources[0]);
	// create the object:
	BoxList = glGenLists(1);
	glNewList(BoxList, GL_COMPILE);
	// Random Objects placed here (polyStack)
	glEndList();


	// create the axes:

	AxesList = glGenLists(1);
	glNewList(AxesList, GL_COMPILE);
	glLineWidth(AXES_WIDTH);
	Axes(1.5);
	glLineWidth(1.);
	glEndList();
}
void Keyboard(unsigned char c, int x, int y)
{
	keyBuffer[c] = true;
	switch (c)
	{
	case 'c':
	case 'C':
		if (AbramSmoke > 0 && AbramHP > 0) {
			AbramSmoke--;
			if (smokeIndex >= 1000)
				smokeIndex = 0;
			smokeIDBuffer[smokeIndex] = Time;
			smokeCoordBuffer[smokeIndex][0] = AbramXY[0];
			smokeCoordBuffer[smokeIndex][1] = AbramXY[1];
			smokeDurBuffer[smokeIndex] = 0.09;
			smokeAngleBuffer[smokeIndex] = rand() % 360;
			smokeIDBufferSet[smokeIndex] = !smokeIDBufferSet[smokeIndex];
			smokeActive[smokeIndex] = true;
			smokeIndex++;
			for (int i = 0; i < 120; i++)
			{
				if (smokeIndex >= 1000)
					smokeIndex = 0;
				smokeIDBuffer[smokeIndex] = Time;
				smokeCoordBuffer[smokeIndex][0] = AbramXY[0] + 10 * (sin(i *  PI / 6));
				smokeCoordBuffer[smokeIndex][1] = AbramXY[1] + 10 * (cos(i *  PI / 6));
				smokeAngleBuffer[smokeIndex] = rand() % 360;
				smokeDurBuffer[smokeIndex] = 0.09;
				smokeIDBufferSet[smokeIndex] = !smokeIDBufferSet[smokeIndex];
				smokeActive[smokeIndex] = true;
				smokeIndex++;
			}
		}
		break;
	case '.':
	case '1':
	case 'n':
	case 'N':
		if (IS3Smoke > 0 && IS3HP > 0) {
			IS3Smoke--;
			if (smokeIndex >= 1000)
				smokeIndex = 0;
			smokeIDBuffer[smokeIndex] = Time;
			smokeCoordBuffer[smokeIndex][0] = IS3XY[0];
			smokeCoordBuffer[smokeIndex][1] = IS3XY[1];
			smokeDurBuffer[smokeIndex] = 0.09;
			smokeAngleBuffer[smokeIndex] = rand() % 360;
			smokeIDBufferSet[smokeIndex] = !smokeIDBufferSet[smokeIndex];
			smokeActive[smokeIndex] = true;
			smokeIndex++;
			for (int i = 0; i < 120; i++)
			{
				if (smokeIndex >= 1000)
					smokeIndex = 0;
				smokeIDBuffer[smokeIndex] = Time;
				smokeCoordBuffer[smokeIndex][0] = IS3XY[0] + 10 * (sin(i *  PI / 6));
				smokeCoordBuffer[smokeIndex][1] = IS3XY[1] + 10 * (cos(i *  PI / 6));
				smokeDurBuffer[smokeIndex] = 0.09;
				smokeAngleBuffer[smokeIndex] = rand() % 360;
				smokeIDBufferSet[smokeIndex] = !smokeIDBufferSet[smokeIndex];
				smokeActive[smokeIndex] = true;
				smokeIndex++;
			}
		}
		break;
	case '=':
	case '+':
		TANKSPEED *= 2.0;
		std::cout << "Speed set to:" << TANKSPEED << std::endl;
		break;
	case '-':
	case '_':
		TANKSPEED /= 2.0;
		std::cout << "Speed set to:" << TANKSPEED << std::endl;
		break;
	case 'g':
	case 'G':
		AbramHP = TANKHP;
		IS3HP = TANKHP;
		shake = false;
		smokeIndex = 0;
		shakeOnce = false;
		AbramShells = SHELLSTORAGE;
		IS3Shells = SHELLSTORAGE;
		AbramSmoke = SMOKECOUNT;
		IS3Smoke = SMOKECOUNT;
		AbramTurretAngle = 0;
		IS3TurretAngle = 0;
		AbramLastShot = 0;
		IS3LastShot = 0;
		for (int i = 0; i < 1000; i++)
			smokeIDBuffer[i] = 0;
		loadMap();
		break;
	}
}
void keyUp(unsigned char c, int x, int y)
{
	keyBuffer[c] = false;

}
void MouseButton(int button, int state, int x, int y)
{
	int b = 0;			// LEFT, MIDDLE, or RIGHT

	if (DebugOn != 0)
		fprintf(stderr, "MouseButton: %d, %d, %d, %d\n", button, state, x, y);


	// get the proper button bit mask:

	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		b = LEFT;		break;

	case GLUT_MIDDLE_BUTTON:
		b = MIDDLE;		break;

	case GLUT_RIGHT_BUTTON:
		b = RIGHT;		break;

	default:
		b = 0;
		fprintf(stderr, "Unknown mouse button: %d\n", button);
	}


	// button down sets the bit, up clears the bit:

	if (state == GLUT_DOWN)
	{
		Xmouse = x;
		Ymouse = y;
		ActiveButton |= b;		// set the proper bit
	}
	else
	{
		ActiveButton &= ~b;		// clear the proper bit
	}
}
void MouseMotion(int x, int y)
{
/*	if (DebugOn != 0)
		fprintf(stderr, "MouseMotion: %d, %d\n", x, y);


	int dx = x - Xmouse;		// change in mouse coords
	int dy = y - Ymouse;

	if ((ActiveButton & LEFT) != 0)
	{
		Xrot += (ANGFACT*dy);
		Yrot += (ANGFACT*dx);
	}


	if ((ActiveButton & MIDDLE) != 0)
	{
		Scale += SCLFACT * (float)(dx - dy);

		// keep object from turning inside-out or disappearing:

		if (Scale < MINSCALE)
			Scale = MINSCALE;
	}

	Xmouse = x;			// new current position
	Ymouse = y;
	*/
	glutSetWindow(MainWindow);
	glutPostRedisplay();
}
void Reset()
{
	ActiveButton = 0;
	AxesOn = 1;
	DebugOn = 0;
	DepthBufferOn = 1;
	DepthFightingOn = 0;
	DepthCueOn = 0;
	Scale = 1.0;
	WhichColor = WHITE;
	WhichProjection = PERSP;
	Xrot = Yrot = 0.;
}
void Resize(int width, int height)
{
	if (DebugOn != 0)
		fprintf(stderr, "ReSize: %d, %d\n", width, height);

	// don't really need to do anything since window size is
	// checked each time in Display( ):

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}
void Visibility(int state)
{
	if (DebugOn != 0)
		fprintf(stderr, "Visibility: %d\n", state);

	if (state == GLUT_VISIBLE)
	{
		glutSetWindow(MainWindow);
		glutPostRedisplay();
	}
	else
	{
		// could optimize by keeping track of the fact
		// that the window is not visible and avoid
		// animating or redrawing it ...
	}
}
static float xx[] = {
	0.f, 1.f, 0.f, 1.f
};
static float xy[] = {
	-.5f, .5f, .5f, -.5f
};
static int xorder[] = {
	1, 2, -3, 4
};
static float yx[] = {
	0.f, 0.f, -.5f, .5f
};
static float yy[] = {
	0.f, .6f, 1.f, 1.f
};
static int yorder[] = {
	1, 2, 3, -2, 4
};
static float zx[] = {
	1.f, 0.f, 1.f, 0.f, .25f, .75f
};
static float zy[] = {
	.5f, .5f, -.5f, -.5f, 0.f, 0.f
};
static int zorder[] = {
	1, 2, 3, 4, -5, 6
};
const float LENFRAC = 0.10f;
const float BASEFRAC = 1.10f;
void Axes(float length)
{
	glBegin(GL_LINE_STRIP);
	glVertex3f(length, 0., 0.);
	glVertex3f(0., 0., 0.);
	glVertex3f(0., length, 0.);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex3f(0., 0., 0.);
	glVertex3f(0., 0., length);
	glEnd();

	float fact = LENFRAC * length;
	float base = BASEFRAC * length;

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 4; i++)
	{
		int j = xorder[i];
		if (j < 0)
		{

			glEnd();
			glBegin(GL_LINE_STRIP);
			j = -j;
		}
		j--;
		glVertex3f(base + fact*xx[j], fact*xy[j], 0.0);
	}
	glEnd();

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 5; i++)
	{
		int j = yorder[i];
		if (j < 0)
		{

			glEnd();
			glBegin(GL_LINE_STRIP);
			j = -j;
		}
		j--;
		glVertex3f(fact*yx[j], base + fact*yy[j], 0.0);
	}
	glEnd();

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 6; i++)
	{
		int j = zorder[i];
		if (j < 0)
		{

			glEnd();
			glBegin(GL_LINE_STRIP);
			j = -j;
		}
		j--;
		glVertex3f(0.0, fact*zy[j], base + fact*zx[j]);
	}
	glEnd();

}
// function to convert HSV to RGB
// 0.  <=  s, v, r, g, b  <=  1.
// 0.  <= h  <=  360.
// when this returns, call:
//		glColor3fv( rgb );
void HsvRgb(float hsv[3], float rgb[3])
{
	// guarantee valid input:

	float h = hsv[0] / 60.f;
	while (h >= 6.)	h -= 6.;
	while (h <  0.) 	h += 6.;

	float s = hsv[1];
	if (s < 0.)
		s = 0.;
	if (s > 1.)
		s = 1.;

	float v = hsv[2];
	if (v < 0.)
		v = 0.;
	if (v > 1.)
		v = 1.;

	// if sat==0, then is a gray:

	if (s == 0.0)
	{
		rgb[0] = rgb[1] = rgb[2] = v;
		return;
	}

	// get an rgb from the hue itself:

	float i = floor(h);
	float f = h - i;
	float p = v * (1.f - s);
	float q = v * (1.f - s*f);
	float t = v * (1.f - (s * (1.f - f)));

	float r, g, b;			// red, green, blue
	switch ((int)i)
	{
	case 0:
		r = v;	g = t;	b = p;
		break;

	case 1:
		r = q;	g = v;	b = p;
		break;

	case 2:
		r = p;	g = v;	b = t;
		break;

	case 3:
		r = p;	g = q;	b = v;
		break;

	case 4:
		r = t;	g = p;	b = v;
		break;

	case 5:
		r = v;	g = p;	b = q;
		break;
	}


	rgb[0] = r;
	rgb[1] = g;
	rgb[2] = b;
}