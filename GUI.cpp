#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <iostream>
#define WIN32
#pragma warning(disable:4996)
#include "glew.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include "glut.h"
#include "loader.h"
#include <glm/glm.hpp>
#include <time.h>
#include "glslprogram.h"
GLSLProgram *Pattern;
using std::vector;
const char *WINDOWTITLE = { "Tanks 2017" };
const char *GLUITITLE   = { "User Interface Window" };

const int GLUITRUE  = { true  };
const int GLUIFALSE = { false };

#define ESCAPE		0x1b



const int INIT_WINDOW_SIZE = { 600 };


const float BOXSIZE = { 2.f };

const float ANGFACT = { 1. };
const float SCLFACT = { 0.005f };

const float MINSCALE = { 0.05f };

const int LEFT   = { 4 };
const int MIDDLE = { 2 };
const int RIGHT  = { 1 };


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
const GLfloat BACKCOLOR[ ] = { 0., 0., 0., 1. };
const GLfloat AXES_WIDTH   = { 3. };
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
char * ColorNames[ ] =
{
	"Red",
	"Yellow",
	"Green",
	"Cyan",
	"Blue",
	"Magenta",
	"White",
	"Black"
};
const GLfloat Colors[ ][3] = 
{
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
const GLenum  FOGMODE     = { GL_LINEAR };
const GLfloat FOGDENSITY  = { 0.30f };
const GLfloat FOGSTART    = { 1.5 };
const GLfloat FOGEND      = { 4. };
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
void	Animate( );
void	Display( );
void	DoAxesMenu( int );
void	DoColorMenu( int );
void	DoDepthBufferMenu( int );
void	DoDepthFightingMenu( int );
void	DoDepthMenu( int );
void	DoDebugMenu( int );
void	DoMainMenu( int );
void	DoProjectMenu( int );
void	DoRasterString( float, float, float, char * );
void	DoStrokeString( float, float, float, float, char * );
float	ElapsedSeconds( );
void	InitGraphics( );
void	InitLists( );
void	InitMenus( );
void	Keyboard( unsigned char, int, int );
void	keySpecial(int key, int x, int y);
void	MouseButton( int, int, int, int );
void	MouseMotion( int, int );
void	Reset( );
void	Resize( int, int );
void	Visibility( int );

void	Axes( float );
void	HsvRgb( float[3], float [3] );
#define NUMMODEL 2

int IS3[3][2];
int Abram[2][2];
int Track[2][2];
int ammo[2];
int smokeCrate[2][2];
int hpCrate[2][2];

#define START 0
#define END 1
bool res = false;
std::vector< glm::vec3 > vertices;
std::vector< glm::vec2 > uvs;
std::vector< glm::vec3 > normals; // Won't be used at the moment.
std::vector< glm::vec3 > verticesArray[NUMMODEL];
std::vector< glm::vec2 > uvsArray[NUMMODEL];
std::vector< glm::vec3 > normalsArray[NUMMODEL]; // Won't be used at the moment.
GLuint VertexVBOID = 0;
GLuint ModelIDList[NUMMODEL];
float Time = 0;
#define TOTAL_MS (180 * 1000)
#define TANKSCALE 0.175
#define CAMX 10
#define CAMY 10
#define CAMZ 0
float eyex = CAMX;
float eyey = CAMY;
float eyez = CAMZ;
float targetx = 0;
float targety = 3;
float targetz = -3;
float upx = 0;
float upy = 1;
float upz = 0;
bool run = false;
int camState = 0;
int backgroundRand = 0;
int selectIndex = 0;
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
	glLightf(ilight, GL_CONSTANT_ATTENUATION, 1);
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
	glMaterialfv(GL_FRONT, GL_AMBIENT, Array3(r / 2, g / 2, b / 2));
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Array3(r, g, b));
	glMaterialfv(GL_FRONT, GL_SPECULAR, MulArray3(.8f, White));
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}
void drawIS3(float X, float Y, float Z, float hullAngle, float turretAngle)
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
void drawAbram(float X, float Y, float Z, float hullAngle, float turretAngle)
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
	glTranslatef(-1, 0, 0.25);
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
void drawAmmo(float X, float Y)
{
	int beginPoint;
	int endPoint;

	glPushMatrix();
	glTranslatef(X, 1, Y);	//movement
	glRotatef((int)(Time * 5000) % 360, 0, 1, 0);
	glTranslatef(0, 0, 0);
	glScalef(250, 250, 250);
	beginPoint = ammo[START];
	endPoint = ammo[END] - ammo[START];
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
	endPoint = smokeCrate[1][END] - smokeCrate[1][START];

	SetMaterial(0.25, 0.25, 0.25, 1.0);
	glColor3f(0.25, 0.25, 0.25);
	glPushMatrix();
	glRotatef(270, 1, 0, 0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = smokeCrate[0][START];
	endPoint = smokeCrate[0][END] - smokeCrate[0][START];

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
int main( int argc, char *argv[ ] )
{
	srand(time(NULL));
	backgroundRand = rand() % 5;
	// turn on the glut package:
	// (do this before checking argc and argv since it might
	// pull some command line arguments out)

	glutInit( &argc, argv );


	// setup all the graphics stuff:

	InitGraphics( );


	// create the display structures that will not change:

	InitLists( );


	// init all the global variables used by Display( ):
	// this will also post a redisplay

	Reset( );


	// setup all the user interface stuff:

	InitMenus( );


	// draw the scene once and wait for some interaction:
	// (this will never return)

	glutSetWindow( MainWindow );
	glutMainLoop( );


	// this is here to make the compiler happy:

	return 0;
}
void Animate()
{
	// put animation stuff in here -- change some global variables
	// for Display( ) to find:
	int ms = glutGet(GLUT_ELAPSED_TIME);
	ms %= TOTAL_MS;
	Time = (float)ms / 180000;
	// animate with time here:
	// force a call to Display( ) next time it is convenient:

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}
void loadAll()
{
	std::cout << "Loading Modles ..." << std::endl;

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

	hpCrate[0][START] = vertices.size();
	res = loadOBJ("models/hp1.obj", vertices, uvs, normals);
	hpCrate[0][END] = vertices.size();

	hpCrate[1][START] = vertices.size();
	res = loadOBJ("models/hp2.obj", vertices, uvs, normals);
	hpCrate[1][END] = vertices.size();

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
void Display( )
{

	Animate(); 
	glutSetCursor(GLUT_CURSOR_NONE);
	if (DebugOn != 0)
	{
		fprintf(stderr, "Display\n");
	}


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

	if (AxesOn != 0)
	{
		glColor3fv(&Colors[WhichColor][0]);
		glCallList(AxesList);
	}


	// since we are using glScalef( ), be sure normals get unitized:
	// draw the current object:
	//glCallList(BoxList);
	// Costume polys for each frame (instapoly):__________________________________________________________________________________________________________________________
	Pattern->Use();
	Pattern->SetUniformVariable((char *)"uKa", (float)1);
	Pattern->SetUniformVariable((char *)"uKd", (float)1);
	Pattern->SetUniformVariable((char *)"uKs", (float)1);
	Pattern->SetUniformVariable((char *)"uShininess", (float)80);
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
		// Push the GL attribute bits so that we don't wreck any settings
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		// Enable polygon offsets, and offset filled polygons forward by 2.5
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(-2.5f, -2.5f);
		// Set the render mode to be line rendering with a thick line width
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(4.0f);
		// Set the colour to be white
		glColor3f(.5, .5, .5);
		// Render the object
		switch (backgroundRand)
		{
		case 0:
			drawHPCrate(0, 0);
			break;
		case 1:
			drawSmokeCrate(0, 0);
			break;
		case 2:
			drawAmmo(0, 0);
				break;
		case 3:
			drawIS3(0, 0, 0, (float)((int)(Time * 5000) % 360), -45);
			break;
		case 4:
			drawAbram(0, 0, 0, (float)((int)(Time * 5000) % 360), -45);
			break;
		}
		// Set the polygon mode to be filled triangles 
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glShadeModel(GL_FLAT);
		glEnable(GL_LIGHTING);
		Pattern->SetUniformVariable((char *)"uKa", (float)0.25);
		SetPointLight(GL_LIGHT0, 0, 50, 0, 0.15, 0.15, 0.15);
		// Set the colour to the background
		glColor3f(0.0f, 0.0f, 0.0f);
		// Render the object
		switch (backgroundRand)
		{
		case 0:
			drawHPCrate(0, 0);
			break;
		case 1:
			drawSmokeCrate(0, 0);
			break;
		case 2:
			drawAmmo(0, 0);
			break;
		case 3:
			drawIS3(0, 0, 0, (float)((int)(Time * 5000) % 360), -45);
			break;
		case 4:
			drawAbram(0, 0, 0, (float)((int)(Time * 5000) % 360), -45);
			break;
		}
		// Pop the state changes off the attribute stack
		// to set things back how they were
		glPopAttrib();
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);
		Pattern->SetUniformVariable((char *)"uKa", (float)1);
		Pattern->Use(0);
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
	// Texts
	// hint:
	glColor3f(1., 1., 1.);
	int x = 0;
	int y = -3;
	int z = 7;

	switch (backgroundRand)
	{
	case 0:
		DoRasterString(x,y,z, (char *)"Took a few hits? Try snatching one of these bad boys, and you'll be back in action in no time!");
		break;
	case 1:
		DoRasterString(x, y, z, (char *)"Do you like ninjas? These Ninjutsu certified smoke canisters can take your breath away!");
		break;
	case 2:
		DoRasterString(x, y, z, (char *)"Don't you hated when your finger gets stuck on the fire button? This will feed that addiction.");
		break;
	case 3:
		DoRasterString(x, y, z, (char *)"This tanks is called Joseph Stalin 3 or IS3 ... but trust me, nothing about this tank is stalin' ...");
		break;
	case 4:
		DoRasterString(x, y, z, (char *)"How Abram tanks moves so fast? Well ask the turbine jet engine behind it!");
		break;
	}

	// select level:

	// level list

	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0., 100., 0., 100.);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(1., 1., 1.);
	y = 70;
	int inc = 0;
	GLint m_viewport[4];
	glGetIntegerv(GL_VIEWPORT, m_viewport);

	float amount = 40;
	selectIndex = selectIndex % 10;
	DoRasterString(60, y     , 0, (char *)"Please select the map:");
	if (/*(Xmouse > 1100 && Xmouse < 1500 &&
		Ymouse > 160 + (float)(inc * amount) && Ymouse < 210 + (float)(inc * amount)) ||*/ selectIndex == 0)
	{
		glColor3f(1., 1., 0);
		//if ((ActiveButton & LEFT) != 0) 
		if(run)
		{
			run = false;
			system("./out 1");
			//Sleep(500);
		}
	}
	else
		glColor3f(1., 1., 1.);
	inc++;
	DoRasterString(60, y - 2 , 0, (char *)"Classic (Medium)");
	if (/*(Xmouse > 1100 && Xmouse < 1500 &&
		Ymouse > 160 + (float)(inc * amount) && Ymouse < 210 + (float)(inc * amount)) ||*/ selectIndex == 1)
	{
		glColor3f(1., 1., 0);
		if(run) 
		{
			run = false;
			system("./out 2");
			//Sleep(500);
		}
	}
	else
		glColor3f(1., 1., 1.);
	inc++;
	DoRasterString(60, y - 4 , 0, (char *)"Duel (Medium)");
	if (/*(Xmouse > 1100 && Xmouse < 1500 &&
		Ymouse > 160 + (float)(inc * amount) && Ymouse < 210 + (float)(inc * amount)) ||*/ selectIndex == 2)
	{
		glColor3f(1., 1., 0);
		if(run)
		{
			run = false;
			system("./out 7");
			//Sleep(500);
		}
	}
	else
		glColor3f(1., 1., 1.);
	inc++;
	DoRasterString(60, y - 6 , 0, (char *)"Field (Easy)");
	if (/*(Xmouse > 1100 && Xmouse < 1500 &&
		Ymouse > 160 + (float)(inc * amount) && Ymouse < 210 + (float)(inc * amount)) ||*/ selectIndex == 3)
	{
		glColor3f(1., 1., 0);
		if(run)
		{
			run = false;
			system("./out 9");
			//Sleep(500);
		}
	}
	else
		glColor3f(1., 1., 1.);
	inc++;
	DoRasterString(60, y - 8 , 0, (char *)"aim_map (Easy)");
	if (/*(Xmouse > 1100 && Xmouse < 1500 &&
		Ymouse > 160 + (float)(inc * amount) && Ymouse < 210 + (float)(inc * amount)) ||*/ selectIndex == 4)
	{
		glColor3f(1., 1., 0);
		if(run)
		{
			run = false;
			system("./out 5");
			//Sleep(500);
		}
	}
	else
		glColor3f(1., 1., 1.);
	inc++;
	DoRasterString(60, y - 10, 0, (char *)"Joe Graphics (Hard)");
	if (/*(Xmouse > 1100 && Xmouse < 1500 &&
		Ymouse > 160 + (float)(inc * amount) && Ymouse < 210 + (float)(inc * amount)) ||*/ selectIndex == 5)
	{
		glColor3f(1., 1., 0);
		if(run)
		{
			run = false;
			system("./out 8");
			//Sleep(500);
		}
	}
	else
		glColor3f(1., 1., 1.);
	inc++;
	DoRasterString(60, y - 12, 0, (char *)"Limbo (Medium)");
	if (/*(Xmouse > 1100 && Xmouse < 1500 &&
		Ymouse > 160 + (float)(inc * amount) && Ymouse < 210 + (float)(inc * amount)) ||*/ selectIndex == 6)
	{
		glColor3f(1., 1., 0);
		if(run)
		{
			run = false;
			system("./out 3");
			//Sleep(500);
		}
	}
	else
		glColor3f(1., 1., 1.);
	inc++;
	DoRasterString(60, y - 14, 0, (char *)"Jungle (Hard)");
	if (/*(Xmouse > 1100 && Xmouse < 1500 &&
		Ymouse > 160 + (float)(inc * amount) && Ymouse < 210 + (float)(inc * amount)) ||*/ selectIndex == 7)
	{
		glColor3f(1., 1., 0);
		if(run)
		{
			run = false;
			system("./out 6");
			//Sleep(500);
		}
	}
	else
		glColor3f(1., 1., 1.);
	inc++;
	DoRasterString(60, y - 16, 0, (char *)"Maze (Hard)");
	if (/*(Xmouse > 1100 && Xmouse < 1500 &&
		Ymouse > 160 + (float)(inc * amount) && Ymouse < 210 + (float)(inc * amount)) ||*/ selectIndex == 8)
	{
		glColor3f(1., 1., 0);
		if(run)
		{
			run = false;
			system("./out 4");
			//Sleep(500);
		}
	}
	else
		glColor3f(1., 1., 1.);
	inc++;
	DoRasterString(60, y - 18, 0, (char *)"Temple (Medium):");
	if (/*(Xmouse > 1100 && Xmouse < 1500 &&
		Ymouse > 160 + (float)(inc * amount) && Ymouse < 210 + (float)(inc * amount)) ||*/ selectIndex == 9)
	{
		glColor3f(1., 1., 0);
		if (run)
		{
			run = false;
			system("./out r");
			//Sleep(500);
		}
	}
	else
		glColor3f(1., 1., 1.);
	inc++;
	DoRasterString(60, y - 20, 0, (char *)"Random (Easy)");
	// swap the double-buffered framebuffers:
	glutSwapBuffers();


	// be sure the graphics buffer has been sent:
	// note: be sure to use glFlush( ) here, not glFinish( ) !

	glFlush();
}
void  DoAxesMenu( int id )
{
	AxesOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}
void DoColorMenu( int id )
{
	WhichColor = id - RED;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}
void DoDebugMenu( int id )
{
	DebugOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}
void DoDepthBufferMenu( int id )
{
	DepthBufferOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}
void DoDepthFightingMenu( int id )
{
	DepthFightingOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}
void DoDepthMenu( int id )
{
	DepthCueOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}
void DoMainMenu( int id )
{
	switch( id )
	{
		case RESET:
			Reset( );
			break;

		case QUIT:
			// gracefully close out the graphics:
			// gracefully close the graphics window:
			// gracefully exit the program:
			glutSetWindow( MainWindow );
			glFinish( );
			glutDestroyWindow( MainWindow );
			exit( 0 );
			break;

		default:
			fprintf( stderr, "Don't know what to do with Main Menu ID %d\n", id );
	}

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}
void DoProjectMenu( int id )
{
	WhichProjection = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}
void DoRasterString( float x, float y, float z, char *s )
{
	glRasterPos3f( (GLfloat)x, (GLfloat)y, (GLfloat)z );

	char c;			// one character to print
	for( ; ( c = *s ) != '\0'; s++ )
	{
		glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, c );
	}
}
void DoStrokeString( float x, float y, float z, float ht, char *s )
{
	glPushMatrix( );
		glTranslatef( (GLfloat)x, (GLfloat)y, (GLfloat)z );
		float sf = ht / ( 119.05f + 33.33f );
		glScalef( (GLfloat)sf, (GLfloat)sf, (GLfloat)sf );
		char c;			// one character to print
		for( ; ( c = *s ) != '\0'; s++ )
		{
			glutStrokeCharacter( GLUT_STROKE_ROMAN, c );
		}
	glPopMatrix( );
}
float ElapsedSeconds( )
{
	// get # of milliseconds since the start of the program:

	int ms = glutGet( GLUT_ELAPSED_TIME );

	// convert it to seconds:

	return (float)ms / 1000.f;
}
void InitMenus( )
{
	glutSetWindow( MainWindow );

	int numColors = sizeof( Colors ) / ( 3*sizeof(int) );
	int colormenu = glutCreateMenu( DoColorMenu );
	for( int i = 0; i < numColors; i++ )
	{
		glutAddMenuEntry( ColorNames[i], i );
	}

	int axesmenu = glutCreateMenu( DoAxesMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int depthcuemenu = glutCreateMenu( DoDepthMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int depthbuffermenu = glutCreateMenu( DoDepthBufferMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int depthfightingmenu = glutCreateMenu( DoDepthFightingMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int debugmenu = glutCreateMenu( DoDebugMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int projmenu = glutCreateMenu( DoProjectMenu );
	glutAddMenuEntry( "Orthographic",  ORTHO );
	glutAddMenuEntry( "Perspective",   PERSP );

	int mainmenu = glutCreateMenu( DoMainMenu );
	glutAddSubMenu(   "Axes",          axesmenu);
	glutAddSubMenu(   "Colors",        colormenu);
	glutAddSubMenu(   "Depth Buffer",  depthbuffermenu);
	glutAddSubMenu(   "Depth Fighting",depthfightingmenu);
	glutAddSubMenu(   "Depth Cue",     depthcuemenu);
	glutAddSubMenu(   "Projection",    projmenu );
	glutAddMenuEntry( "Reset",         RESET );
	glutAddSubMenu(   "Debug",         debugmenu);
	glutAddMenuEntry( "Quit",          QUIT );

// attach the pop-up menu to the right mouse button:

	glutAttachMenu( GLUT_RIGHT_BUTTON );
}
void InitGraphics( )
{
	// request the display modes:
	// ask for red-green-blue-alpha color, double-buffering, and z-buffering:

	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );

	// set the initial window configuration:

	glutInitWindowPosition( 0, 0 );
	glutInitWindowSize( INIT_WINDOW_SIZE, INIT_WINDOW_SIZE );

	// open the window and set its title:

	MainWindow = glutCreateWindow( WINDOWTITLE );
	glutSetWindowTitle( WINDOWTITLE );
	// set the framebuffer clear values:

	glClearColor( BACKCOLOR[0], BACKCOLOR[1], BACKCOLOR[2], BACKCOLOR[3] );
	glutSetWindow( MainWindow );
	glutDisplayFunc( Display );
	glutFullScreen();
	glutReshapeFunc( Resize );
	glutKeyboardFunc( Keyboard );
	glutMouseFunc( MouseButton );
	glutMotionFunc( MouseMotion );
	glutPassiveMotionFunc(MouseMotion);
	glutVisibilityFunc( Visibility );
	glutEntryFunc( NULL );
	glutSpecialFunc(keySpecial);
	glutSpaceballMotionFunc( NULL );
	glutSpaceballRotateFunc( NULL );
	glutSpaceballButtonFunc( NULL );
	glutButtonBoxFunc( NULL );
	glutDialsFunc( NULL );
	glutTabletMotionFunc( NULL );
	glutTabletButtonFunc( NULL );
	glutMenuStateFunc( NULL );
	glutTimerFunc( -1, NULL, 0 );
	glutIdleFunc( NULL );
	// init glew (a window must be open to do this):

#ifdef WIN32
	GLenum err = glewInit( );
	if( err != GLEW_OK )
	{
		fprintf( stderr, "glewInit Error\n" );
	}
	else
		fprintf( stderr, "GLEW initialized OK\n" );
	fprintf( stderr, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
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
void InitLists( )
{
	loadAll();
	float dx = BOXSIZE / 2.f;
	float dy = BOXSIZE / 2.f;
	float dz = BOXSIZE / 2.f;
	glutSetWindow( MainWindow );

	// create the object:

	BoxList = glGenLists( 1 );
	glNewList( BoxList, GL_COMPILE );

	glEndList( );


	// create the axes:

	AxesList = glGenLists( 1 );
	glNewList( AxesList, GL_COMPILE );
		glLineWidth( AXES_WIDTH );
			Axes( 1.5 );
		glLineWidth( 1. );
	glEndList( );
}
void keySpecial(int key, int x, int y) {
	switch (key)
	{
	case GLUT_KEY_UP:
		selectIndex--;
		break;
	case GLUT_KEY_DOWN:
		selectIndex++;
		break;
	}
}
void Keyboard( unsigned char c, int x, int y )
{
	if( DebugOn != 0 )
		fprintf( stderr, "Keyboard: '%c' (0x%0x)\n", c, c );

	switch( c )
	{
		case 'o':
		case 'O':
			WhichProjection = ORTHO;
			break;

		case 'p':
		case 'P':
			WhichProjection = PERSP;
			break;
		case 'w':
		case 'W':
			selectIndex--;
			break;
		case 's':
		case 'S':
			selectIndex++;
			break;
		case ' ':
		case 13:
			run = true;
			break;
		case 'q':
		case 'Q':
		case ESCAPE:
			DoMainMenu( QUIT );	// will not return here
			break;				// happy compiler

	}

	// force a call to Display( ):

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}
void MouseButton( int button, int state, int x, int y )
{
	int b = 0;			// LEFT, MIDDLE, or RIGHT

	if( DebugOn != 0 )
		fprintf( stderr, "MouseButton: %d, %d, %d, %d\n", button, state, x, y );

	
	// get the proper button bit mask:

	switch( button )
	{
		case GLUT_LEFT_BUTTON:
			b = LEFT;		break;

		case GLUT_MIDDLE_BUTTON:
			b = MIDDLE;		break;

		case GLUT_RIGHT_BUTTON:
			b = RIGHT;		break;

		default:
			b = 0;
			fprintf( stderr, "Unknown mouse button: %d\n", button );
	}


	// button down sets the bit, up clears the bit:

	if( state == GLUT_DOWN )
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
void MouseMotion( int x, int y )
{

	int dx = x - Xmouse;		// change in mouse coords
	int dy = y - Ymouse;
	//Xrot += (ANGFACT*dy);
	//Yrot += (ANGFACT*dx);
	/*if( ( ActiveButton & LEFT ) != 0 )
	{
		Xrot += ( ANGFACT*dy );
		Yrot += ( ANGFACT*dx );
	}


	if( ( ActiveButton & MIDDLE ) != 0 )
	{
		Scale += SCLFACT * (float) ( dx - dy );

		// keep object from turning inside-out or disappearing:

		if( Scale < MINSCALE )
			Scale = MINSCALE;
	}
	*/
	Xmouse = x;			// new current position
	Ymouse = y;
	if(DebugOn != 0)
		if ((ActiveButton & LEFT) != 0)
			std::cout << "DEBUG MOUSE: " << Xmouse << ", " << Ymouse << std::endl;
	glutSetWindow( MainWindow );
	glutPostRedisplay( );
	
}
void Reset( )
{
	ActiveButton = 0;
	AxesOn = 0;
	DebugOn = 0;
	DepthBufferOn = 1;
	DepthFightingOn = 0;
	DepthCueOn = 0;
	Scale  = 1.0;
	WhichColor = WHITE;
	WhichProjection = PERSP;
	Xrot = Yrot = 0.;
}
void Resize( int width, int height )
{
	if( DebugOn != 0 )
		fprintf( stderr, "ReSize: %d, %d\n", width, height );

	// don't really need to do anything since window size is
	// checked each time in Display( ):

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}
void Visibility ( int state )
{
	if( DebugOn != 0 )
		fprintf( stderr, "Visibility: %d\n", state );

	if( state == GLUT_VISIBLE )
	{
		glutSetWindow( MainWindow );
		glutPostRedisplay( );
	}
	else
	{
		// could optimize by keeping track of the fact
		// that the window is not visible and avoid
		// animating or redrawing it ...
	}
}
static float xx[ ] = {
		0.f, 1.f, 0.f, 1.f
	      };
static float xy[ ] = {
		-.5f, .5f, .5f, -.5f
	      };
static int xorder[ ] = {
		1, 2, -3, 4
		};
static float yx[ ] = {
		0.f, 0.f, -.5f, .5f
	      };
static float yy[ ] = {
		0.f, .6f, 1.f, 1.f
	      };
static int yorder[ ] = {
		1, 2, 3, -2, 4
		};
static float zx[ ] = {
		1.f, 0.f, 1.f, 0.f, .25f, .75f
	      };
static float zy[ ] = {
		.5f, .5f, -.5f, -.5f, 0.f, 0.f
	      };
static int zorder[ ] = {
		1, 2, 3, 4, -5, 6
		};
const float LENFRAC = 0.10f;
const float BASEFRAC = 1.10f;
void Axes( float length )
{
	glBegin( GL_LINE_STRIP );
		glVertex3f( length, 0., 0. );
		glVertex3f( 0., 0., 0. );
		glVertex3f( 0., length, 0. );
	glEnd( );
	glBegin( GL_LINE_STRIP );
		glVertex3f( 0., 0., 0. );
		glVertex3f( 0., 0., length );
	glEnd( );

	float fact = LENFRAC * length;
	float base = BASEFRAC * length;

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 4; i++ )
		{
			int j = xorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( base + fact*xx[j], fact*xy[j], 0.0 );
		}
	glEnd( );

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 5; i++ )
		{
			int j = yorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( fact*yx[j], base + fact*yy[j], 0.0 );
		}
	glEnd( );

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 6; i++ )
		{
			int j = zorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( 0.0, fact*zy[j], base + fact*zx[j] );
		}
	glEnd( );

}
void HsvRgb( float hsv[3], float rgb[3] )
{
	// guarantee valid input:

	float h = hsv[0] / 60.f;
	while( h >= 6. )	h -= 6.;
	while( h <  0. ) 	h += 6.;

	float s = hsv[1];
	if( s < 0. )
		s = 0.;
	if( s > 1. )
		s = 1.;

	float v = hsv[2];
	if( v < 0. )
		v = 0.;
	if( v > 1. )
		v = 1.;

	// if sat==0, then is a gray:

	if( s == 0.0 )
	{
		rgb[0] = rgb[1] = rgb[2] = v;
		return;
	}

	// get an rgb from the hue itself:
	
	float i = floor( h );
	float f = h - i;
	float p = v * ( 1.f - s );
	float q = v * ( 1.f - s*f );
	float t = v * ( 1.f - ( s * (1.f-f) ) );

	float r, g, b;			// red, green, blue
	switch( (int) i )
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
