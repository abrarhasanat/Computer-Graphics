///////////////////////////////////////////////////////////////////////////////
// main.cpp
// ========
// drawing cubespheres using vertex array (glDrawElements)
// dependency: freeglut/glut
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2018-07-17
// UPDATED: 2023-03-14
///////////////////////////////////////////////////////////////////////////////

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include "glExtension.h"
#include "Bmp.h"
#include "Cubesphere.h"


// GLUT CALLBACK functions
void displayCB();
void reshapeCB(int w, int h);
void timerCB(int millisec);
void keyboardCB(unsigned char key, int x, int y);
void mouseCB(int button, int stat, int x, int y);
void mouseMotionCB(int x, int y);

void initGL();
int  initGLUT(int argc, char **argv);
bool initSharedMem();
void clearSharedMem();
void initLights();
void setCamera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ);
void drawString(const char *str, int x, int y, float color[4], void *font);
void drawString3D(const char *str, float pos[3], float color[4], void *font);
void toOrtho();
void toPerspective();
GLuint loadTexture(const char* fileName, bool wrap=true);
GLuint loadCubemap(const char* fileName);


// constants
const int   SCREEN_WIDTH    = 1500;
const int   SCREEN_HEIGHT   = 500;
const float CAMERA_DISTANCE = 3.7f;
const int   TEXT_WIDTH      = 8;
const int   TEXT_HEIGHT     = 13;


// global variables
void *font = GLUT_BITMAP_8_BY_13;
int screenWidth;
int screenHeight;
bool mouseLeftDown;
bool mouseRightDown;
bool mouseMiddleDown;
float mouseX, mouseY;
float cameraAngleX;
float cameraAngleY;
float cameraDistance;
int drawMode;
GLuint texId1, texId2;                  // texture object
int imageWidth;
int imageHeight;

// for automatic texture generation modes: GL_OBJECT_LINEAR and GL_EYE_LINEAR
float planeS[] = {1.0f, 0.0f, 0.0f, 0.0f};  // +X
float planeT[] = {0.0f, 1.0f, 0.0f, 0.0f};  // +Y
float planeR[] = {0.0f, 0.0f, 1.0f, 0.0f};  // +Z

// for cubed sphere
bool cubemapSupported = false;
int subdivision = 3;
//Cubesphere sphere;                          // create default sphere(1, 3, true)
Cubesphere sphere(1.0f, subdivision, false); // radius, subdivision, smooth



///////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    // init global vars
    initSharedMem();

    // init GLUT and GL
    initGLUT(argc, argv);
    initGL();

    // load 2D texture
    texId1 = loadTexture("grid512.bmp", true);

    // check GL_ARB_texture_cube_map extension
    glExtension& ext = glExtension::getInstance();
    cubemapSupported = ext.isSupported("GL_ARB_texture_cube_map");
    if(cubemapSupported)
    {
        // load cube map
        texId2 = loadCubemap("cubemap.bmp");
        std::cout << "Video card supports GL_ARB_texture_cube_map" << std::endl;
    }
    else
    {
        texId2 = 0;
        std::cout << "[WARNING] Video card does NOT support GL_ARB_texture_cube_map." << std::endl;
    }

    // the last GLUT call (LOOP)
    // window will be shown and display callback is triggered by events
    // NOTE: this call never return main().
    glutMainLoop(); /* Start GLUT event-processing loop */

    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// initialize GLUT for windowing
///////////////////////////////////////////////////////////////////////////////
int initGLUT(int argc, char **argv)
{
    // GLUT stuff for windowing
    // initialization openGL window.
    // it is called before any other GLUT routine
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);   // display mode

    glutInitWindowSize(screenWidth, screenHeight);  // window size

    glutInitWindowPosition(100, 100);               // window location

    // finally, create a window with openGL context
    // Window will not displayed until glutMainLoop() is called
    // it returns a unique ID
    int handle = glutCreateWindow(argv[0]);     // param is the title of window

    // register GLUT callback functions
    glutDisplayFunc(displayCB);
    glutTimerFunc(33, timerCB, 33);             // redraw only every given millisec
    glutReshapeFunc(reshapeCB);
    glutKeyboardFunc(keyboardCB);
    glutMouseFunc(mouseCB);
    glutMotionFunc(mouseMotionCB);

    return handle;
}



///////////////////////////////////////////////////////////////////////////////
// initialize OpenGL
// disable unused features
///////////////////////////////////////////////////////////////////////////////
void initGL()
{
    glShadeModel(GL_SMOOTH);                    // shading mathod: GL_SMOOTH or GL_FLAT
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);      // 4-byte pixel alignment

    // enable /disable features
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    //glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // track material ambient and diffuse from surface color, call it before glEnable(GL_COLOR_MATERIAL)
    //glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    //glEnable(GL_COLOR_MATERIAL);

    glClearColor(0, 0, 0, 0);                   // background color
    glClearStencil(0);                          // clear stencil buffer
    glClearDepth(1.0f);                         // 0 is near, 1 is far
    glDepthFunc(GL_LEQUAL);

    initLights();
}



///////////////////////////////////////////////////////////////////////////////
// write 2d text using GLUT
// The projection matrix must be set to orthogonal before call this function.
///////////////////////////////////////////////////////////////////////////////
void drawString(const char *str, int x, int y, float color[4], void *font)
{
    glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT); // lighting and color mask
    glDisable(GL_LIGHTING);     // need to disable lighting for proper text color
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);

    glColor4fv(color);          // set text color
    glRasterPos2i(x, y);        // place text position

    // loop all characters in the string
    while(*str)
    {
        glutBitmapCharacter(font, *str);
        ++str;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glPopAttrib();
}



///////////////////////////////////////////////////////////////////////////////
// draw a string in 3D space
///////////////////////////////////////////////////////////////////////////////
void drawString3D(const char *str, float pos[3], float color[4], void *font)
{
    glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT); // lighting and color mask
    glDisable(GL_LIGHTING);     // need to disable lighting for proper text color
    glDisable(GL_TEXTURE_2D);

    glColor4fv(color);          // set text color
    glRasterPos3fv(pos);        // place text position

    // loop all characters in the string
    while(*str)
    {
        glutBitmapCharacter(font, *str);
        ++str;
    }

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glPopAttrib();
}



///////////////////////////////////////////////////////////////////////////////
// initialize global variables
///////////////////////////////////////////////////////////////////////////////
bool initSharedMem()
{
    screenWidth = SCREEN_WIDTH;
    screenHeight = SCREEN_HEIGHT;

    mouseLeftDown = mouseRightDown = mouseMiddleDown = false;
    mouseX = mouseY = 0;

    cameraAngleX = cameraAngleY = 0.0f;
    cameraDistance = CAMERA_DISTANCE;

    drawMode = 1; // 0:without lines, 1:with lines

    //DEBUG
    sphere.printSelf();
    std::cout << std::endl;

    return true;
}



///////////////////////////////////////////////////////////////////////////////
// clean up global vars
///////////////////////////////////////////////////////////////////////////////
void clearSharedMem()
{
}



///////////////////////////////////////////////////////////////////////////////
// initialize lights
///////////////////////////////////////////////////////////////////////////////
void initLights()
{
    // set up light colors (ambient, diffuse, specular)
    GLfloat lightKa[] = {.3f, .3f, .3f, 1.0f};  // ambient light
    GLfloat lightKd[] = {.8f, .8f, .8f, 1.0f};  // diffuse light
    GLfloat lightKs[] = {1, 1, 1, 1};           // specular light
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightKa);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightKd);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightKs);

    // position the light
    float lightPos[4] = {0, 0, 1, 0}; // directional light
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glEnable(GL_LIGHT0);                        // MUST enable each light source after configuration
}



///////////////////////////////////////////////////////////////////////////////
// set camera position and lookat direction
///////////////////////////////////////////////////////////////////////////////
void setCamera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(posX, posY, posZ, targetX, targetY, targetZ, 0, 1, 0); // eye(x,y,z), focal(x,y,z), up(x,y,z)
}



///////////////////////////////////////////////////////////////////////////////
// load raw image as a texture
///////////////////////////////////////////////////////////////////////////////
GLuint loadTexture(const char* fileName, bool wrap)
{
    Image::Bmp bmp;
    if(!bmp.read(fileName))
        return 0;     // exit if failed load image

    // get bmp info
    int width = bmp.getWidth();
    int height = bmp.getHeight();
    const unsigned char* data = bmp.getDataRGB();
    GLenum type = GL_UNSIGNED_BYTE;    // only allow BMP with 8-bit per channel

    // We assume the image is 8-bit, 24-bit or 32-bit BMP
    GLenum format;
    int bpp = bmp.getBitCount();
    if(bpp == 8)
        format = GL_LUMINANCE;
    else if(bpp == 24)
        format = GL_RGB;
    else if(bpp == 32)
        format = GL_RGBA;
    else
        return 0;               // NOT supported, exit

    // gen texture ID
    GLuint texture;
    glGenTextures(1, &texture);

    // set active texture and configure it
    glBindTexture(GL_TEXTURE_2D, texture);

    // select modulate to mix texture with color for shading (default mode)
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

    // if wrap is true, the texture wraps over at the edges (repeat)
    //       ... false, the texture ends at the edges (clamp)
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap ? GL_REPEAT : GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap ? GL_REPEAT : GL_CLAMP);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // copy texture data
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, type, data);
    //glGenerateMipmap(GL_TEXTURE_2D);

    /*
    // build our texture mipmaps
    switch(bpp)
    {
    case 8:
        gluBuild2DMipmaps(GL_TEXTURE_2D, 1, width, height, GL_LUMINANCE, type, data);
        break;
    case 24:
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, type, data);
        break;
    case 32:
        gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, type, data);
        break;
    }
    */

    return texture;
}



///////////////////////////////////////////////////////////////////////////////
// load 6 cubemap textures by inserting number 0...6 right after the input file
// For example, if input is "image.bmp", it will load the following image files
// image0.bmp: GL_TEXTURE_CUBE_MAP_POSITIVE_X
// image1.bmp: GL_TEXTURE_CUBE_MAP_NEGATIVE_X
// image2.bmp: GL_TEXTURE_CUBE_MAP_POSITIVE_Y
// image3.bmp: GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
// image4.bmp: GL_TEXTURE_CUBE_MAP_POSITIVE_Z
// image5.bmp: GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
///////////////////////////////////////////////////////////////////////////////
GLuint loadCubemap(const char* fileStr)
{
    std::string inputFile = fileStr;
    std::size_t pos = inputFile.find_last_of('.');
    if(pos == std::string::npos)
    {
        std::cout << "Incorrect file name (no extension)" << std::endl;
        return 0;
    }

    std::string fileName = inputFile.substr(0, pos);
    std::string fileExt = inputFile.substr(pos);    // with .
    //std::cout << fileName << fileExt << std::endl;

    // gen texture ID
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    // set texture parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP, GL_TRUE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // load 6 bitmaps by appending 0...6 in the file names
    for(int i = 0; i < 6; ++i)
    {
        std::string file = fileName + std::to_string(i) + fileExt;
        Image::Bmp bmp;
        if(!bmp.read(file.c_str()))
        {
            std::cout << "Failed to open " << file << std::endl;
            return 0;     // exit if failed load image
        }

        // get bmp info
        int width = bmp.getWidth();
        int height = bmp.getHeight();
        const unsigned char* data = bmp.getDataRGB();
        GLenum type = GL_UNSIGNED_BYTE;    // only allow BMP with 8-bit per channel
        //bmp.printSelf();

        // We assume the image is 8-bit, 24-bit or 32-bit BMP
        GLenum format;
        int bpp = bmp.getBitCount();
        if(bpp == 8)
            format = GL_LUMINANCE;
        else if(bpp == 24)
            format = GL_RGB;
        else if(bpp == 32)
            format = GL_RGBA;
        else
        {
            std::cout << "Not supported bitmap format" << std::endl;
            return 0;   // NOT supported, exit
        }

        // copy bitmap to each face of cubemap
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,  // target (0~5)
                     0,                                 // level
                     format,                            // internal format
                     width,                             // image width
                     height,                            // image height
                     0,                                 // border
                     format,                            // format
                     type,                              // type
                     data);                             // bitmap data
    }

    // unbind
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    return texture;
}



///////////////////////////////////////////////////////////////////////////////
// display info messages
///////////////////////////////////////////////////////////////////////////////
void showInfo()
{
    // backup current model-view matrix
    glPushMatrix();                     // save current modelview matrix
    glLoadIdentity();                   // reset modelview matrix

    // set to 2D orthogonal projection
    glMatrixMode(GL_PROJECTION);        // switch to projection matrix
    glPushMatrix();                     // save current projection matrix
    glLoadIdentity();                   // reset projection matrix
    //gluOrtho2D(0, screenWidth, 0, screenHeight); // set to orthogonal projection
    glOrtho(0, screenWidth, 0, screenHeight, -1, 1); // set to orthogonal projection

    float color[4] = {1, 1, 1, 1};

    std::stringstream ss;
    ss << std::fixed << std::setprecision(3);

    ss << "Subdivision: " << sphere.getSubdivision() << std::ends;
    drawString(ss.str().c_str(), 1, screenHeight-TEXT_HEIGHT, color, font);
    ss.str("");

    ss << "Radius: " << sphere.getRadius() << std::ends;
    drawString(ss.str().c_str(), 1, screenHeight-(2*TEXT_HEIGHT), color, font);
    ss.str("");

    ss << "Triangle Count: " << sphere.getTriangleCount() << std::ends;
    drawString(ss.str().c_str(), 1, screenHeight-(3*TEXT_HEIGHT), color, font);
    ss.str("");

    ss << "Vertex Count: " << sphere.getVertexCount() << std::ends;
    drawString(ss.str().c_str(), 1, screenHeight-(4*TEXT_HEIGHT), color, font);
    ss.str("");

    ss << "Index Count: " << sphere.getIndexCount() << std::ends;
    drawString(ss.str().c_str(), 1, screenHeight-(5*TEXT_HEIGHT), color, font);
    ss.str("");

    ss << "Press SPACE key to change subdivision level"  << std::ends;
    drawString(ss.str().c_str(), 1, 1, color, font);
    ss.str("");

    // unset floating format
    ss << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);

    // restore projection matrix
    glPopMatrix();                   // restore to previous projection matrix

    // restore modelview matrix
    glMatrixMode(GL_MODELVIEW);      // switch to modelview matrix
    glPopMatrix();                   // restore to previous modelview matrix
}



///////////////////////////////////////////////////////////////////////////////
// set projection matrix as orthogonal
///////////////////////////////////////////////////////////////////////////////
void toOrtho()
{
    // set viewport to be the entire window
    glViewport(0, 0, (GLsizei)screenWidth, (GLsizei)screenHeight);

    // set orthographic viewing frustum
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, 0, screenHeight, -1, 1);

    // switch to modelview matrix in order to set scene
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}



///////////////////////////////////////////////////////////////////////////////
// set the projection matrix as perspective
///////////////////////////////////////////////////////////////////////////////
void toPerspective()
{
    // set viewport to be the entire window
    glViewport(0, 0, (GLsizei)screenWidth, (GLsizei)screenHeight);

    // set perspective viewing frustum
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0f, (float)(screenWidth)/screenHeight, 1.0f, 1000.0f); // FOV, AspectRatio, NearClip, FarClip

    // switch to modelview matrix in order to set scene
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}









//=============================================================================
// CALLBACKS
//=============================================================================

void displayCB()
{
    // clear buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // save the initial ModelView matrix before modifying ModelView matrix
    glPushMatrix();

    // tramsform modelview matrix
    glTranslatef(0, 0, -cameraDistance);

    // set material
    float ambient[]  = {0.5f, 0.5f, 0.5f, 1};
    float diffuse[]  = {0.8f, 0.8f, 0.8f, 1};
    float specular[] = {1.0f, 1.0f, 1.0f, 1};
    float shininess  = 128;
    glMaterialfv(GL_FRONT, GL_AMBIENT,   ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);

    // line color
    float lineColor[] = {0.3f, 0.3f, 0.3f, 1};

    // draw a flat sphere without texture on left
    glPushMatrix();
    glTranslatef(-2.5f, 0, 0);
    glRotatef(cameraAngleX, 1, 0, 0);   // pitch
    glRotatef(cameraAngleY, 0, 1, 0);   // heading
    glBindTexture(GL_TEXTURE_2D, 0);
    if(drawMode == 0)
        sphere.draw();
    else
        sphere.drawWithLines(lineColor);
    glPopMatrix();

    // draw a flat sphere with 2D texture on center
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse); // reset diffuse
    glPushMatrix();
    glRotatef(cameraAngleX, 1, 0, 0);   // pitch
    glRotatef(cameraAngleY, 0, 1, 0);   // heading
    glBindTexture(GL_TEXTURE_2D, texId1);
    if(drawMode == 0)
        sphere.draw();
    else
        sphere.drawWithLines(lineColor);
    glPopMatrix();


    if(cubemapSupported)
    {
        glEnable(GL_TEXTURE_CUBE_MAP);
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glEnable(GL_TEXTURE_GEN_R);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texId2);

        // draw a flat sphere with cubemap on right
        // generate texture coords in object space
        glTexGenfv(GL_S, GL_OBJECT_PLANE, planeS);
        glTexGenfv(GL_T, GL_OBJECT_PLANE, planeT);
        glTexGenfv(GL_R, GL_OBJECT_PLANE, planeR);
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        //glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
        //glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
        //glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
        //glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP);
        //glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP);
        //glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse); // reset diffuse
        glPushMatrix();
        glTranslatef(2.5f, 0, 0);
        glRotatef(cameraAngleX, 1, 0, 0);
        glRotatef(cameraAngleY, 0, 1, 0);
        if(drawMode == 0)
            sphere.draw();
        else
            sphere.drawWithLines(lineColor);
        glPopMatrix();

        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_GEN_R);
        glDisable(GL_TEXTURE_CUBE_MAP);
    }

    showInfo();     // print max range of glDrawRangeElements

    glPopMatrix();

    glutSwapBuffers();
}


void reshapeCB(int w, int h)
{
    screenWidth = w;
    screenHeight = h;
    toPerspective();
    std::cout << "window resized: " << w << " x " << h << std::endl;

#ifdef _WIN32
    HWND handle = ::GetActiveWindow();
    RECT rect;
    ::GetWindowRect(handle, &rect);
    std::cout << "window size: " << (rect.right - rect.left) << "x" << (rect.bottom - rect.top) << std::endl;
#endif
}


void timerCB(int millisec)
{
    glutTimerFunc(millisec, timerCB, millisec);
    glutPostRedisplay();
}


void keyboardCB(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 27: // ESCAPE
        clearSharedMem();
        exit(0);
        break;

    case ' ': // space
        ++subdivision;
        subdivision %= 7; // from 0 to 6
        sphere.setSubdivision(subdivision);
        break;

    case 'd': // switch rendering modes (fill -> wire -> point)
    case 'D':
        ++drawMode;
        drawMode %= 2;
        break;

    default:
        ;
    }
}


void mouseCB(int button, int state, int x, int y)
{
    mouseX = x;
    mouseY = y;

    if(button == GLUT_LEFT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            mouseLeftDown = true;
        }
        else if(state == GLUT_UP)
            mouseLeftDown = false;
    }

    else if(button == GLUT_RIGHT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            mouseRightDown = true;
        }
        else if(state == GLUT_UP)
            mouseRightDown = false;
    }

    else if(button == GLUT_MIDDLE_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            mouseMiddleDown = true;
        }
        else if(state == GLUT_UP)
            mouseMiddleDown = false;
    }
}


void mouseMotionCB(int x, int y)
{
    if(mouseLeftDown)
    {
        cameraAngleY += (x - mouseX);
        cameraAngleX += (y - mouseY);
        mouseX = x;
        mouseY = y;
    }
    if(mouseRightDown)
    {
        cameraDistance -= (y - mouseY) * 0.2f;
        mouseY = y;
    }
}
