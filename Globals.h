#ifndef GLOBALS_H
	#define GLOBALS_H

	ALCdevice *device;
	ALboolean enumeration;
	ALCcontext *context;
	ALuint mainMusic1, mainMusic2, mainMusic3, mainMusic4, mainMusic5, mainMusic6, mainMusic7;
	ALuint tankShellFire;
	ALuint tankBulletFire;
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
	const GLfloat BACKCOLOR[] = { 0., 0., 0., 1. };
	const GLfloat AXES_WIDTH = { 3. };
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
	int T29[2][2];
	int E100[2][2];
	int Type59[2][2];
	int IS3[3][2];
	int Track[2][2];
	int cube[2];
	int trees[8][2];
	int shell[2];
	int ammo[2];
	int smokeCrate[2][2];
	int hpCrate[2][2];
	int mineCrate[2];
	int grass[2];
	//float smokeBeginTime = 0;
	//bool smokeSet = false;
	/*
	float smokeIDBuffer[1000];
	float smokeCoordBuffer[1000][2];
	float smokeDurBuffer[1000];
	float smokeAngleBuffer[1000];
	bool  smokeIDBufferSet[1000];
	bool  smokeActive[1000];
	int smokeIndex = 0;*/
	
	struct Smoke {
		float smokeIDBuffer;
		float smokeCoordBuffer[2];
		float smokeDurBuffer;
		float smokeAngleBuffer;
		bool  smokeIDBufferSet = false;
		bool  smokeActive = false;
	};
	std::vector<Smoke>Smokes;
	int AbramSmokeBudget = 0;
	int IS3SmokeBudget = 0;
	float destructionTimeBuffer[24][14];

	float TANKSPEED = 0.3;

	//explosion and fire
	bool shake = false;
	bool shakeOnce = false;
	float shakeDuration = 0.005;
	float shakeStartTime = 0;
	bool playExplosionSound = false;
	bool fireAbram = false;
	bool fireIS3 = false;
	bool clicked = false;
	bool loading = false;
	float AbramLastShot = 0;
	float IS3LastShot = 0;
	float AbramLastShotB = 0;
	float IS3LastShotB = 0;

	int AbramShells = SHELLSTORAGE;
	int IS3Shells = SHELLSTORAGE;

	float AbramHP = TANKHP;
	float IS3HP = TANKHP;

	float AbramSmoke = SMOKECOUNT;
	float IS3Smoke = SMOKECOUNT;

	//texture
	#include "bmptotexture.h"
	int width, height;
	GLuint tex0 = 1;
	BTT myTexture;
	unsigned char *Texture = myTexture.BmpToTexture("textures\\camo.bmp", &width, &height);

	GLSLProgram *Pattern;
	GLSLProgram *PatternGrass;
	GLSLProgram *PatternTree;
	GLSLProgram *PatternCamo;
	GLSLProgram *PatternSilh;

	float AbramTurretAngle = 0;
	float AbramHullAngle = 180;
	float AbramInitCoord[2] = { 0,-SPAWN };
	float IS3InitCoord[2] = { 0,SPAWN };
	float AbramXY[2] = { AbramInitCoord[0],AbramInitCoord[1] };
	float IS3XY[2] = { IS3InitCoord[0],IS3InitCoord[1] };
	float IS3TurretAngle = 0;
	float IS3HullAngle = 0;

	//texture:
	GLuint noise;

	// Menu
	bool isInMenu;
	int backgroundRand = 0;
	int selectIndex = 0;
	bool run = false;
	int musicID = 0;

	float MoveTimeAbram = 0;
	float MoveTimeIS3 = 0;
	std::string mapName = " ";
	std::string lastMap = " ";
	int AbramScore = 0;
	int IS3Score = 0;
	char scoreText[5];
	bool ScoreSet = true;

	bool keyBuffer[256];
	char MapRaw[25 * 14];

	struct Map myMap;
	//Crate Crates[10];
	std::vector<Crate>Crates;

	struct Shell Shells[SHELLMAX];
	struct Bullet Bullets[BULLETMAX];

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
	int CrateIndex = 0;
	int shellSize = 0;
	int bulletSize = 0;
	void	Animate();
	void	Display();
//	void	DoAxesMenu(int);
//	void	DoColorMenu(int);
//	void	DoDepthBufferMenu(int);
//	void	DoDepthFightingMenu(int);
//	void	DoDepthMenu(int);
//	void	DoCameraMenu(int);
//	void	DoDebugMenu(int);
//	void	DoMainMenu(int);
//	void	DoProjectMenu(int);
	void	Quit();
	void	DoRasterString(float, float, float, char *);
	void	DoStringBox   (float, float, float, char *);
	void	DoStringBoxColor(float, float, float, char *, float, float, float);
	void	DoStringBoxColor(float, float, float, char *);
//	void	DoStrokeString(float, float, float, float, char *);
	float	ElapsedSeconds();
	void	InitGraphics();
	void	InitLists();
	//void	InitMenus();
	//void	keySpecial(int key, int x, int y);
	void	Keyboard(unsigned char, int, int);
	void	keySpecial(int key, int x, int y);
	void	keyUp(unsigned char, int, int);
	void	gamepad();
	void	MouseButton(int, int, int, int);
	void	MouseMotion(int, int);
	void	Reset();
	void	Resize(int, int);
	void	Visibility(int);
	void	Axes(float);
	void	HsvRgb(float[3], float[3]);
	int main(int argc, char *argv[]);
	const float LENFRAC = 0.10f;
	const float BASEFRAC = 1.10f;
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

	bool isSingle = false;

	int PlayerOne = 0;
	int PlayerTwo = 1;

	void(*DrawPointer1)(float X, float Y, float Z, float hullAngle, float turretAngle);
	void(*DrawPointer2)(float X, float Y, float Z, float hullAngle, float turretAngle);
	void(*DrawPointer1d)(float X, float Y, float Z, float hullAngle, float turretAngle);
	void(*DrawPointer2d)(float X, float Y, float Z, float hullAngle, float turretAngle);

	int menuState = 0;
	int userGrassMultiplier = 10;
	bool isSoundLoaded = false;
	//OS Compatibility
#ifndef WIN32
	char * itoa(int i, char * buffer, int radix)
	{
		snprintf(buffer, sizeof(buffer), "%d", i);
		return buffer;
	}
#endif
#endif
