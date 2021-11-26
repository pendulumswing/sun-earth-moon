#include <stdio.h>
#include <stdlib.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#define _USE_MATH_DEFINES
#include <math.h>

#ifdef WIN32
#include <windows.h>
#pragma warning(disable:4996)
#endif

//#include "glew.h"
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include "src/glut.h"
#include "src/freeglut.h"
#include "src/colors.h"   // Custom Colors
#include "src/utils.h"    // Utility functions separated from sample.cpp
#include "src/osusphere.cpp"
#include "src/loadobjfile.cpp"
#include "src/constants.h"

//	The objective is to draw five 3D Bezier curves with animation
//
//	The left mouse button does rotation
//	The middle mouse button does scaling
//	The user interface allows:
//		1. The axes to be turned on and off
//		2. The color of the axes to be changed
//		3. Debugging to be turned on and off
//		4. Depth cueing to be turned on and off
//		5. The projection to be changed
//		6. The transformations to be reset
//		7. The program to quit
//
//	Author:			Jason Brown


//////////////////////////////////////////////////
// CONSTANTS - Imported from constants.h
//////////////////////////////////////////////////

// window background color (rgba):
const GLfloat BACKCOLOR[ ] = { Colors[OFFWHITE][0], Colors[OFFWHITE][1], Colors[OFFWHITE][2], 1. };


//////////////////////////////////////////////////
// COLORS - Imported from colors.h
//////////////////////////////////////////////////



//////////////////////////////////////////////////
// NON-CONSTANT GLOBAL VARIABLES
//////////////////////////////////////////////////
// TODO - Insert Custom Project variables HERE
GLuint	HeliBodyList;				// object display list
GLuint	BladeList;					// object display list
GLuint	CircleList;					// object display list
GLuint	PlaneList;					// object display list
int Angle = 0;				      // Angle of rotation (0-360)
bool	Frozen;								// Toggle Animation
bool Light0On, Light1On, Light2On, Light3On;    // Light switches
float Time;
bool Smooth_Shading = true;

// LIGHT COLORS
Rgba LC0 = SetColorWithAlpha( LIGHTRED );
Rgba LC1 = SetColorWithAlpha( BLUEGRAY);
Rgba LC2 = SetColorWithAlpha( ORANGE );
Rgba LC3 = SetColorWithAlpha( WHITE );

// OBJ CONSTANT VARIABLES
GLuint Banana;
GLuint Box;
GLuint Oatmeal;
GLuint Kitchen_Walls;
GLuint Kitchen_Props;
GLuint Kitchen_Floor;
GLuint Wisk;
GLuint Spoon;
GLuint Vanilla;

// TEXTURE GLOBAL VARIABLES
GLuint Tex0;                // To store texture
GLuint Tex1;                // To store texture
GLuint Tex2;                // To store texture
GLuint Tex3;                // To store texture
GLuint Tex4;                // To store texture
GLuint Tex5;                // To store texture
GLuint Tex6;                // To store texture
GLuint Tex7;                // To store texture
unsigned char *Texture0;     // Texture pointer
unsigned char *Texture1;     // Texture pointer
unsigned char *Texture2;     // Texture pointer
unsigned char *Texture3;     // Texture pointer
unsigned char *Texture4;     // Texture pointer
unsigned char *Texture5;     // Texture pointer
unsigned char *Texture6;     // Texture pointer
unsigned char *Texture7;     // Texture pointer
int tex0Width, tex0Height;   // Texture Dimensions
int tex1Width, tex1Height;   // Texture Dimensions
int tex2Width, tex2Height;   // Texture Dimensions
int tex3Width, tex3Height;   // Texture Dimensions
int tex4Width, tex4Height;   // Texture Dimensions
int tex5Width, tex5Height;   // Texture Dimensions
int tex6Width, tex6Height;   // Texture Dimensions
int tex7Width, tex7Height;   // Texture Dimensions

//////////////////////////////////////////////////
// DYNAMIC GLOBAL VARIABLES (default)
//////////////////////////////////////////////////
int		ActiveButton;			// current button that is down
GLuint	AxesList;				// list to hold the axes
int		AxesOn;					// != 0 means to draw the axes
int		DebugOn;				// != 0 means to print debugging info
int		DepthCueOn;				// != 0 means to use intensity depth cueing
int		DepthBufferOn;			// != 0 means to use the z-buffer
int		DepthFightingOn;		// != 0 means to force the creation of z-fighting
GLuint	ObjectList;				// object display list
int		MainWindow;				// window id for main graphics window
float	Scale;					// scaling factor
int		ShadowsOn;				// != 0 means to turn shadows on
int		WhichColor;				// index into Colors[ ]
int		WhichProjection;		// ORTHO or PERSP
int		Xmouse, Ymouse;			// mouse values
float	Xrot, Yrot;				// rotation angles in degrees


// function prototypes:
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
void	DoShadowMenu();
void	DoRasterString( float, float, float, char * );
void	DoStrokeString( float, float, float, float, char * );
float	ElapsedSeconds( );
void	InitGraphics( );
void	InitLists( );
void	InitMenus( );
void	Keyboard( unsigned char, int, int );
void	MouseButton( int, int, int, int );
void	MouseMotion( int, int );
void	Reset( );
void	Resize( int, int );
void	Visibility( int );


// main program:
int
main( int argc, char *argv[ ] )
{
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

	// glutMainLoop( ) never returns
	// this line is here to make the compiler happy:
	return 0;
}


// this is where one would put code that is to be called
// everytime the glut main loop has nothing to do
//
// this is typically where animation parameters are set
//
// do not call Display( ) from here -- let glutMainLoop( ) do it

void
Animate( )
{
	// put animation stuff in here -- change some global variables
	// for Display( ) to find:

	// force a call to Display( ) next time it is convenient:
	// TODO - Animation
  Angle = Angle < 360 ? Angle + 1 : 0;

  int ms = glutGet( GLUT_ELAPSED_TIME );
  ms %= MS_PER_CYCLE;
  Time = (float)ms / (float)MS_PER_CYCLE;  // Normalize to value: 0.0 <= x <= 1.0

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// DRAW the complete scene:
void
Display( )
{
	if( DebugOn != 0 )
	{
		fprintf( stderr, "Display\n" );
	}

	// set which window we want to do the graphics into:
	glutSetWindow( MainWindow );

	// erase the BACKGROUND:
	glDrawBuffer( GL_BACK );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glEnable( GL_DEPTH_TEST );


  glDisable( GL_LIGHTING );
	// specify SHADING to be flat:
	glShadeModel( GL_FLAT );

	// set the VIEWPORT to a square centered in the window:
	GLsizei vx = glutGet( GLUT_WINDOW_WIDTH );
	GLsizei vy = glutGet( GLUT_WINDOW_HEIGHT );
	GLsizei v = vx < vy ? vx : vy;			// minimum dimension
	GLint xl = ( vx - v ) / 2;
	GLint yb = ( vy - v ) / 2;
	glViewport( xl, yb,  v, v );

	// set the VIEWING VOLUME:
	// remember that the Z clipping values are actually
	// given as DISTANCES IN FRONT OF THE EYE
	// USE gluOrtho2D( ) IF YOU ARE DOING 2D!
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	if( WhichProjection == ORTHO )
		glOrtho( -3., 3.,     -3., 3.,     0.1, 1000. );
	else
		gluPerspective( 90., 1.,	0.1, 1000. );

	// PLACE OBJECTS into the scene:
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );


	// Set VIEW: eye position, look-at position, and up-vector:
  float lookMul = 1.f;
	gluLookAt( 0.11 * lookMul, 0.25 * lookMul, 0.25 * lookMul,     0., 0.15, 0.,     0., 1., 0. );

	// ROTATE the scene:
	glRotatef( (GLfloat)Yrot, 0., 1., 0. );
	glRotatef( (GLfloat)Xrot, 1., 0., 0. );

	// SCALE the scene uniformly:
	if( Scale < MINSCALE )
		Scale = MINSCALE;
	glScalef( (GLfloat)Scale, (GLfloat)Scale, (GLfloat)Scale );

	// set the fog parameters:
	if( DepthCueOn != 0 )
	{
		glFogi( GL_FOG_MODE, FOGMODE );
		glFogfv( GL_FOG_COLOR, FOGCOLOR );
		glFogf( GL_FOG_DENSITY, FOGDENSITY );
		glFogf( GL_FOG_START, FOGSTART );
		glFogf( GL_FOG_END, FOGEND );
		glEnable( GL_FOG );
	}
	else
	{
		glDisable( GL_FOG );
	}

  // AXES:
  if( AxesOn != 0 )
  {
    glColor3fv( &Colors[BLUEGRAY][0] );
    glCallList( AxesList );
  }

	// since we are using glScalef( ), be sure NORMALS get unitized:
	glEnable( GL_NORMALIZE );

  // LIGHTS

  // LIGHT 0
  glPushMatrix();
    glRotatef(Angle, 0., 1., 0.);
    glRotatef(Angle, 0.05, 1., 0.08);
    glTranslatef( 0.12, 0.1, 0.);
    SetPointLight( GL_LIGHT0, 0., 0., 0., LC0.r, LC0.g, LC0.b );
    // Create Light Geometry
    glColor3f( LC0.r, LC0.g, LC0.b );
    glutSolidSphere( 0.02, 50, 50);
  glPopMatrix();

  // // LIGHT 1
  // glPushMatrix();
  //   glRotatef(Angle, 0., 1., 0.);
  //   glRotatef(Time * 360, 0.05, 1., 0.02);
  //   glTranslatef( 0.15, 0.2, -0.2);
  //   SetPointLight( GL_LIGHT1, 0., 0., 0., LC1.r, LC1.g, LC1.b );
  //   // Create Light Geometry
  //   glColor3f( LC1.r, LC1.g, LC1.b );
  //   glutSolidSphere( 0.02, 50, 50);
  // glPopMatrix();
  //
  // // LIGHT 2 - SPOTLIGHT
  // glPushMatrix();
  //   glTranslatef( 0., 0.79, 0.);
  //   SetSpotLight(GL_LIGHT2, 0., 0., 0., 0., -1., 0., LC2.r, LC2.g, LC2.b );
  //   // Create Light Geometry
  //   glColor3f( LC2.r, LC2.g, LC2.b );
  //   glutSolidSphere( 0.03, 50, 50);
  // glPopMatrix();
  //
  // // LIGHT 3 - STATIONARY WHITE POINT
  // glPushMatrix();
  //   glTranslatef( 0.15, 0.01, -0.1);
  //   SetPointLight( GL_LIGHT3, 0., 0., 0., LC3.r, LC3.g, LC3.b );
  //   // Create Light Geometry
  //   glColor3f( LC3.r, LC3.g, LC3.b );
  //   glutSolidSphere( 0.02, 50, 50);
  // glPopMatrix();


  // SHADING TOGGLE
  if( Smooth_Shading ) {
    glShadeModel( GL_SMOOTH );
  } else {
    glShadeModel( GL_FLAT );
  }


  // ENABLE LIGHTING
  glEnable( GL_LIGHTING );

  if( Light0On )
    glEnable( GL_LIGHT0 );
  else
    glDisable( GL_LIGHT0 );
  // if( Light1On )
  //   glEnable( GL_LIGHT1 );
  // else
  //   glDisable( GL_LIGHT1 );
  // if( Light2On )
  //   glEnable( GL_LIGHT2 );
  // else
  //   glDisable( GL_LIGHT2 );
  // if( Light3On )
  //   glEnable( GL_LIGHT3 );
  // else
  //   glDisable( GL_LIGHT3 );


	// DRAW
  float kitchen_scale = 1.f;

  // Banana
  glPushMatrix();
    glTranslatef( -0.2, 0.2, 0. );
    glRotatef( 360. * Time, 0., 1., 0. );
    float bScale = kitchen_scale;
    glScalef( bScale, bScale, bScale );
    SetMaterial( 1.0, 1.0, 1.0, 15.0 );
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, Tex1 );     // Set current texture
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );  // How to use the texture
    glCallList( Banana );
    glDisable( GL_TEXTURE_2D );
  glPopMatrix();


  // // Sphere - Shiny
  // glPushMatrix();
  //   glTranslatef( -0.1, 0.1, 0. );
  //   SetMaterial( 0.8, 0.2, 0.1, 20.0 );
  //   // glEnable( GL_TEXTURE_2D );
  //   // glBindTexture( GL_TEXTURE_2D, Tex0 );     // Set current texture
  //   // glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );  // How to use the texture
  //   OsuSphere(0.02, 100, 100);
  //   // glDisable( GL_TEXTURE_2D );
  // glPopMatrix();
  //
  // // Sphere - Flat
  // glPushMatrix();
  //   glTranslatef( -0.14, 0.08, 0.01 );
  //   SetMaterial( 0.8, 0.2, 0.1, 1.0 );
  //   // glEnable( GL_TEXTURE_2D );
  //   // glBindTexture( GL_TEXTURE_2D, Tex0 );     // Set current texture
  //   // glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );  // How to use the texture
  //   OsuSphere(0.02, 100, 100);
  //   // glDisable( GL_TEXTURE_2D );
  // glPopMatrix();

  // // Banana - FLAT
  // glPushMatrix();
  //   glShadeModel( GL_FLAT );
  //   glTranslatef( -0.1, 0.21, -0.1 );
  //   glRotatef( 360. * Time, 0., 1., 0. );
  //   glScalef( bScale, bScale, bScale );
  //   SetMaterial( 1.0, 1.0, 1.0, 15.0 );
  //   glEnable( GL_TEXTURE_2D );
  //   glBindTexture( GL_TEXTURE_2D, Tex1 );     // Set current texture
  //   glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );  // How to use the texture
  //   glCallList( Banana );
  //   glDisable( GL_TEXTURE_2D );
  //
  //   // Toggle back to selected shading type
  //   if( Smooth_Shading ) {
  //     glShadeModel( GL_SMOOTH );
  //   } else {
  //     glShadeModel( GL_FLAT );
  //   }
  // glPopMatrix();

  glDisable( GL_LIGHTING );

	// draw some gratuitous text that is fixed on the screen:
	//
	// the projection matrix is reset to define a scene whose
	// world coordinate system goes from 0-100 in each axis
	//
	// this is called "percent units", and is just a convenience
	//
	// the modelview matrix is reset to identity as we don't
	// want to transform these coordinates

  // TEXT - Project name
  glDisable( GL_DEPTH_TEST );
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity( );
  gluOrtho2D( 0., 100.,     0., 100. );
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity( );
  glColor3f( Colors[BLUEGRAY][0], Colors[BLUEGRAY][1], Colors[BLUEGRAY][2] );
  DoRasterString( 5., 5., 0., (char *)"Project Final - Sun Earth Moon Model" );


	// SWAP the double-buffered FRAMEBUFFERS:
	glutSwapBuffers( );


	// be sure the graphics buffer has been sent:
	// note: be sure to use glFlush( ) here, not glFinish( ) !
	glFlush( );
}

//------------------------------------------------------------------------------
//  MENUS
//------------------------------------------------------------------------------

void
DoAxesMenu( int id )
{
	AxesOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoColorMenu( int id )
{
	WhichColor = id - RED;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoDebugMenu( int id )
{
	DebugOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoDepthBufferMenu( int id )
{
	DepthBufferOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoDepthFightingMenu( int id )
{
	DepthFightingOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoDepthMenu( int id )
{
	DepthCueOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// main menu callback:

void
DoMainMenu( int id )
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


void
DoProjectMenu( int id )
{
	WhichProjection = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoShadowsMenu(int id)
{
	ShadowsOn = id;

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}


// use glut to display a string of characters using a raster font:

void
DoRasterString( float x, float y, float z, char *s )
{
	glRasterPos3f( (GLfloat)x, (GLfloat)y, (GLfloat)z );

	char c;			// one character to print
	for( ; ( c = *s ) != '\0'; s++ )
	{
		glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, c );
	}
}


// use glut to display a string of characters using a stroke font:

void
DoStrokeString( float x, float y, float z, float ht, char *s )
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


// return the number of seconds since the start of the program:

float
ElapsedSeconds( )
{
	// get # of milliseconds since the start of the program:
	int ms = glutGet( GLUT_ELAPSED_TIME );

	// convert it to seconds:
	return (float)ms / 1000.f;
}


// initialize the glui window:

void
InitMenus( )
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

	int shadowsmenu = glutCreateMenu(DoShadowsMenu);
	glutAddMenuEntry("Off", 0);
	glutAddMenuEntry("On", 1);

	int mainmenu = glutCreateMenu( DoMainMenu );
	glutAddSubMenu(   "Axes",          axesmenu);
	glutAddSubMenu(   "Colors",        colormenu);
	glutAddSubMenu(   "Depth Cue",     depthcuemenu);
	glutAddSubMenu(   "Projection",    projmenu );
	glutAddMenuEntry( "Reset",         RESET );
	glutAddSubMenu(   "Debug",         debugmenu);
	glutAddMenuEntry( "Quit",          QUIT );

// attach the pop-up menu to the right mouse button:
	glutAttachMenu( GLUT_RIGHT_BUTTON );
}



// initialize the glut and OpenGL libraries:
//	also setup display lists and callback functions
void
InitGraphics( )
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

	glutSetWindow( MainWindow );
	glutDisplayFunc( Display );
	glutReshapeFunc( Resize );
	glutKeyboardFunc( Keyboard );
	glutMouseFunc( MouseButton );
	glutMotionFunc( MouseMotion );
	glutPassiveMotionFunc(MouseMotion);
	//glutPassiveMotionFunc( NULL );
	glutVisibilityFunc( Visibility );
	glutEntryFunc( NULL );
	glutSpecialFunc( NULL );
	glutSpaceballMotionFunc( NULL );
	glutSpaceballRotateFunc( NULL );
	glutSpaceballButtonFunc( NULL );
	glutButtonBoxFunc( NULL );
	glutDialsFunc( NULL );
	glutTabletMotionFunc( NULL );
	glutTabletButtonFunc( NULL );
	glutMenuStateFunc( NULL );
	glutTimerFunc( -1, NULL, 0 );
	glutIdleFunc( Animate );  // TODO - Set argument to "Animate" to animate scene

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

  // TEXTURE SETUP
  // Function transfers array of texels from CPU to GPU and makes it current active texture.
  // You can get away with specifying this ahead of time only if you are using a single texture.
  // If using multiple textures, you must make each current in Display( ) before you need it.

  // Setup Handles
  glGenTextures( 1, &Tex0 );                    // Create unique Texture "handle"
  glGenTextures( 1, &Tex1 );                    // Create unique Texture "handle"
  // glGenTextures( 1, &Tex2 );                    // Create unique Texture "handle"
  // glGenTextures( 1, &Tex3 );                    // Create unique Texture "handle"
  // glGenTextures( 1, &Tex4 );                    // Create unique Texture "handle"
  // glGenTextures( 1, &Tex5 );                    // Create unique Texture "handle"
  // glGenTextures( 1, &Tex6 );                    // Create unique Texture "handle"
  // glGenTextures( 1, &Tex7 );                    // Create unique Texture "handle"


  // Import *.bmp maps
  Texture0 = BmpToTexture("data/textures/worldtex.bmp", &tex0Width, &tex0Height);  // Load texture
  Texture1 = BmpToTexture("data/textures/Bake_Banana_Tex_Base_ALB-OP.bmp", &tex1Width, &tex1Height);
  // Texture2 = BmpToTexture("data/textures/Bake_Banana_Tex_Base_ALB-OP.bmp", &tex2Width, &tex2Height);
  // Texture3 = BmpToTexture("data/textures/Bake_Banana_Tex_Base_ALB-OP.bmp", &tex3Width, &tex3Height);
  // Texture4 = BmpToTexture("data/textures/Bake_Banana_Tex_Base_ALB-OP.bmp", &tex4Width,
  //                         &tex4Height);
  // Texture5 = BmpToTexture("data/textures/Bake_Banana_Tex_Base_ALB-OP.bmp", &tex5Width,
  //                         &tex5Height);
  // Texture6 = BmpToTexture("data/textures/Bake_Banana_Tex_Base_ALB-OP.bmp", &tex6Width,
  //                         &tex6Height);
  // Texture7 = BmpToTexture("data/textures/Bake_Banana_Tex_Base_ALB-OP.bmp", &tex7Width,
  //                         &tex7Height);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);   // Set pixel storage mode

  // Tex0
  glBindTexture( GL_TEXTURE_2D, Tex0 );           // Make Tex0 handle current
  glEnable( GL_TEXTURE_2D );                      // Set Texture parameters for handle
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D( GL_TEXTURE_2D, 0, 3, tex0Width, tex0Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Texture0 );

  // Tex1
  glBindTexture( GL_TEXTURE_2D, Tex1 );            // Make Tex1 handle current
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D( GL_TEXTURE_2D, 0, 3, tex1Width, tex1Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Texture1 );

  // // Tex2
  // glBindTexture( GL_TEXTURE_2D, Tex2 );            // Make Tex2 handle current
  // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  // glTexImage2D( GL_TEXTURE_2D, 0, 3, tex2Width, tex2Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Texture2 );
  //
  // // Tex3
  // glBindTexture( GL_TEXTURE_2D, Tex3 );            // Make Tex3 handle current
  // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  // glTexImage2D( GL_TEXTURE_2D, 0, 3, tex3Width, tex3Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Texture3 );
  //
  // // Tex4
  // glBindTexture( GL_TEXTURE_2D, Tex4 );            // Make Tex4 handle current
  // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  // glTexImage2D( GL_TEXTURE_2D, 0, 3, tex4Width, tex4Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Texture4 );
  //
  // // Tex5
  // glBindTexture( GL_TEXTURE_2D, Tex5 );            // Make Tex5 handle current
  // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  // glTexImage2D( GL_TEXTURE_2D, 0, 3, tex5Width, tex5Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Texture5 );
  //
  // // Tex6
  // glBindTexture( GL_TEXTURE_2D, Tex6 );            // Make Tex6 handle current
  // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  // glTexImage2D( GL_TEXTURE_2D, 0, 3, tex6Width, tex6Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Texture6 );
  //
  // // Tex6
  // glBindTexture( GL_TEXTURE_2D, Tex7 );            // Make Tex7 handle current
  // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  // glTexImage2D( GL_TEXTURE_2D, 0, 3, tex7Width, tex7Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Texture7 );

  // Disable Texture mode for now
  glDisable( GL_TEXTURE_2D );

  // Now when "glBindTexture(GL_TEXTURE_2D, Tex0)" is called, it will make Tex0 current

}


// initialize the display lists that will not change:
// (a display list is a way to store opengl commands in
//  memory so that they can be played back efficiently at a later time
//  with a call to glCallList( )

void
InitLists( )
{
	glutSetWindow( MainWindow );

  // BANANA
  Banana = glGenLists( 1 );
  glNewList( Banana, GL_COMPILE );
  LoadObjFile( "data/obj/Bake_Banana.obj" );
  glEndList();


  // create the axes:
	AxesList = glGenLists( 1 );
	glNewList( AxesList, GL_COMPILE );
		glLineWidth( AXES_WIDTH );
			Axes( 1.5 );
		glLineWidth( 1. );
	glEndList( );
}


// the keyboard callback:
void
Keyboard( unsigned char c, int x, int y )
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

    case 's':
    case 'S':
      Smooth_Shading = !Smooth_Shading;
      break;

    case '0':
      Light0On = !Light0On;
      break;

    case '1':
      Light1On = !Light1On;
      break;

    case '2':
      Light2On = !Light2On;
      break;

    case '3':
      Light3On = !Light3On;
      break;

    case 'f':
    case 'F':
      Frozen = ! Frozen;
      if ( Frozen )
        glutIdleFunc( NULL );
      else
        glutIdleFunc( Animate );
      break;

    case 'r':
    case 'R':
      DoMainMenu( RESET );
      break;

    case 'q':
    case 'Q':
    case ESCAPE:
      DoMainMenu( QUIT );	// will not return here
      break;				// happy compiler

    default:
      fprintf( stderr, "Don't know what to do with keyboard hit: '%c' (0x%0x)\n", c, c );
  }

  // force a call to Display( ):

  glutSetWindow( MainWindow );
  glutPostRedisplay( );
}

// called when the mouse button transitions down or up:
void
MouseButton( int button, int state, int x, int y )
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

		case SCROLL_WHEEL_UP:
			Scale += SCLFACT * SCROLL_WHEEL_CLICK_FACTOR;
			// keep object from turning inside-out or disappearing:
			if (Scale < MINSCALE)
				Scale = MINSCALE;
			break;

		case SCROLL_WHEEL_DOWN:
			Scale -= SCLFACT * SCROLL_WHEEL_CLICK_FACTOR;
			// keep object from turning inside-out or disappearing:
			if (Scale < MINSCALE)
				Scale = MINSCALE;
			break;

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

	glutSetWindow(MainWindow);
	glutPostRedisplay();

}


// called when the mouse moves while a button is down:
void
MouseMotion( int x, int y )
{
	if( true )
		fprintf( stderr, "MouseMotion: %d, %d\n", x, y );


	int dx = x - Xmouse;		// change in mouse coords
	int dy = y - Ymouse;

	if( ( ActiveButton & LEFT ) != 0 )
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

	Xmouse = x;			// new current position
	Ymouse = y;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// reset the transformations and the colors:
// this only sets the global variables --
// the glut main loop is responsible for redrawing the scene
void
Reset( )
{
	ActiveButton = 0;
	AxesOn = 1;
	DebugOn = 0;
	DepthBufferOn = 1;
	DepthFightingOn = 0;
	DepthCueOn = 0;
	Scale  = 1.0;
	ShadowsOn = 0;
	WhichColor = WHITE;
	WhichProjection = PERSP;
  // WhichView = OUTSIDE;  // TODO - Custom Views
	Xrot = Yrot = 0.;
  Light0On = Light1On = Light2On = Light3On = true;  // Reset Lights
}


// called when user resizes the window:
void
Resize( int width, int height )
{
	if( DebugOn != 0 )
		fprintf( stderr, "ReSize: %d, %d\n", width, height );

	// don't really need to do anything since window size is
	// checked each time in Display( ):
	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// handle a change to the window's visibility:
void
Visibility ( int state )
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

///////////////////////////////////////   HANDY UTILITIES:  //////////////////////////
// separated into "utils.h"

#pragma clang diagnostic pop