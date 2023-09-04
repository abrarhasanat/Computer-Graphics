#define _USE_MATH_DEFINES

#include<bits/stdc++.h>
#include <cstdio>
#include <GL/glut.h>
using namespace std;

#include "1805023_classes.cpp" 
#include "1805023_bitmap_image.hpp"

#define pi acos(-1)


// here goes the data loaded from the input file 


  // the input file is named as description.txt 
    // there are 4 integers on the first line  
    // load  the first 2 as nearDistance and farDistance 
    // load the next one as fovY
    // load the next one as aspectRatio 
    // the next line contains one integer level of recursion
    // the next line contains one integer number of pixels along both axis
    // the next one is a blank line 
    // the next line contains an integer which is width of each cell in checker board  
    // the next line contains three double values ambient, diffuse, reflection coefficient of the infinite checkerboard (no specular for it)\

double nearDistance, farDistance, fovY, aspectRatio, widthOfEachCell;
int levelOfRecursion, pixelsAlongX, pixelsAlongY;
double ambientForCheckerBoard, diffuseForCheckerBoard, reflectionForCheckerBoard;
int numberOfObjects, numberOfNormalLigthSources, numberOfSpotLightSources;
// vector<Object*> objects;
// vector<Sphere*> spheres;
// vector<Cube*> cubes;
// vector<Pyramid*> pyramids;
// vector<Light*> Lights;
// vector<SpotLight*> SpotLights;
CheckerBoard* checkerBoard;
bitmap_image image; // the image to be generated
string  getLine(ifstream& inputFile) {
    string line;
    getline(inputFile, line);
    while (line.length() == 0) {
        getline(inputFile, line);
    }
    return  line;
}


double colorOfset = 30;
void readInputFile() {
    ifstream inputFile("description.txt");
    if (inputFile.is_open()) {
        string line;
        getline(inputFile, line);
        stringstream ss(line);
        ss >> nearDistance >> farDistance >> fovY >> aspectRatio;
        //cout << nearDistance << " " << farDistance << " " << fovY << " " << aspectRatio << endl;

        getline(inputFile, line);
        stringstream ss1(line);
        //int levelOfRecursion;
        ss1 >> levelOfRecursion;

        getline(inputFile, line);
        stringstream ss2(line);
        //int pixelsAlongX, pixelsAlongY;
        ss2 >> pixelsAlongX;
        pixelsAlongY = pixelsAlongX;

        // the next line is a blank line 
        getline(inputFile, line);

        getline(inputFile, line);
        stringstream ss3(line);
        //int widthOfEachCell;
        ss3 >> widthOfEachCell;


        getline(inputFile, line);
        stringstream ss4(line);
        //double ambientForCheckerBoard, diffuseForCheckerBoard, reflectionForCheckerBoard;
        ss4 >> ambientForCheckerBoard >> diffuseForCheckerBoard >> reflectionForCheckerBoard;
        cout << "here " << ambientForCheckerBoard << " " << diffuseForCheckerBoard << " " << reflectionForCheckerBoard << endl;

        line = getLine(inputFile);
        stringstream ss5(line);
        //int numberOfObjects;
        ss5 >> numberOfObjects;
        for (int i = 0; i < numberOfObjects; ++i) {
            line = getLine(inputFile);
            // lower case of the line 
            transform(line.begin(), line.end(), line.begin(), ::tolower);
            cout << line << endl;
            if (line == "sphere") {
                line = getLine(inputFile);
                stringstream ss6(line);
                double x, y, z;
                ss6 >> x >> y >> z;

                Point center(x, y, z);

                line = getLine(inputFile);
                stringstream ss7(line);
                double radius;
                ss7 >> radius;

                line = getLine(inputFile);
                stringstream ss8(line);
                double colorR, colorG, colorB;
                ss8 >> colorR >> colorG >> colorB;
                Color color(colorR, colorG, colorB);

                line = getLine(inputFile);
                stringstream ss9(line);
                double ambient, diffuse, specular, reflection;
                ss9 >> ambient >> diffuse >> specular >> reflection;

                line = getLine(inputFile);
                stringstream ss10(line);
                double shininess;
                ss10 >> shininess;

                Sphere* sphere = new Sphere(center, radius, color, ambient, diffuse, specular, reflection, shininess);
                objects.push_back(sphere);
                spheres.push_back(sphere);

                cout << "sphere" << endl;
                cout << x << " " << y << " " << z << endl;
                cout << radius << endl;
                cout << colorR << " " << colorG << " " << colorB << endl;
                cout << ambient << " " << diffuse << " " << specular << " " << reflection << endl;
                cout << shininess << endl;
                cout << endl;

            }
            else if (line == "cube") {
                Point bottomLowerLeftCorner;
                line = getLine(inputFile);
                stringstream ss6(line);
                double x, y, z;
                ss6 >> x >> y >> z;
                bottomLowerLeftCorner.x = x;
                bottomLowerLeftCorner.y = y;
                bottomLowerLeftCorner.z = z;

                line = getLine(inputFile);
                stringstream ss7(line);
                double side;
                ss7 >> side;

                line = getLine(inputFile);
                stringstream ss8(line);
                double colorR, colorG, colorB;
                ss8 >> colorR >> colorG >> colorB;
                Color color(colorR, colorG, colorB);

                line = getLine(inputFile);
                stringstream ss9(line);
                double ambient, diffuse, specular, reflection;
                ss9 >> ambient >> diffuse >> specular >> reflection;

                line = getLine(inputFile);
                stringstream ss10(line);
                double shininess;
                ss10 >> shininess;

                Cube* cube = new Cube(bottomLowerLeftCorner, side, color, ambient, diffuse, specular, reflection, shininess);
                objects.push_back(cube);
                cubes.push_back(cube);

                cout << "cube" << endl;
                cout << x << " " << y << " " << z << endl;
                cout << side << endl;
                cout << colorR << " " << colorG << " " << colorB << endl;
                cout << ambient << " " << diffuse << " " << specular << " " << reflection << endl;
                cout << shininess << endl;
                cout << endl;
            }
            else if (line == "pyramid") {
                Point  lowestPointCoordinate;
                line = getLine(inputFile);
                stringstream ss6(line);
                double x, y, z;
                ss6 >> x >> y >> z;
                lowestPointCoordinate.x = x;
                lowestPointCoordinate.y = y;
                lowestPointCoordinate.z = z;


                line = getLine(inputFile);
                stringstream ss7(line);
                double width, height;
                ss7 >> width >> height;

                line = getLine(inputFile);
                stringstream ss8(line);
                double colorR, colorG, colorB;
                ss8 >> colorR >> colorG >> colorB;
                Color color(colorR, colorG, colorB);

                line = getLine(inputFile);
                stringstream ss9(line);
                double ambient, diffuse, specular, reflection;
                ss9 >> ambient >> diffuse >> specular >> reflection;

                line = getLine(inputFile);
                stringstream ss10(line);
                double shininess;
                ss10 >> shininess;

                Pyramid* pyramid = new Pyramid(lowestPointCoordinate, width, height, color, ambient, diffuse, specular, reflection, shininess);
                objects.push_back(pyramid);
                pyramids.push_back(pyramid);

                cout << "pyramid" << endl;
                cout << x << " " << y << " " << z << endl;

                cout << width << " " << height << endl;

                cout << colorR << " " << colorG << " " << colorB << endl;
                cout << ambient << " " << diffuse << " " << specular << " " << reflection << endl;
                cout << shininess << endl;
                cout << endl;


            }

        }

        CheckerBoard* chk = new CheckerBoard(widthOfEachCell, ambientForCheckerBoard, diffuseForCheckerBoard, reflectionForCheckerBoard);
        checkerBoard = chk;
        objects.push_back(checkerBoard);
        // read the rest of the file and do nothing with them

        line = getLine(inputFile);
        stringstream ss11(line);
        int numberOfNormalLigthSources;
        ss11 >> numberOfNormalLigthSources;
        for (int i = 0; i < numberOfNormalLigthSources; ++i) {
            line = getLine(inputFile);
            stringstream ss12(line);
            double x, y, z;
            ss12 >> x >> y >> z;
            Point position(x, y, z);


            Color color(1, 1, 1);

            line = getLine(inputFile);
            stringstream ss14(line);
            double falloff;
            ss14 >> falloff;

            Light* light = new Light(position, color, falloff);
            lights.push_back(light);
        }
        line = getLine(inputFile);
        stringstream ss15(line);
        int numberOfSpotLightSources;
        ss15 >> numberOfSpotLightSources;
        for (int i = 0; i < numberOfSpotLightSources; ++i) {
            line = getLine(inputFile);
            stringstream ss16(line);
            double x, y, z;
            ss16 >> x >> y >> z;
            Point position(x, y, z);

            line = getLine(inputFile);
            stringstream ss17(line);
            double falloff;
            ss17 >> falloff;

            line = getLine(inputFile);
            stringstream ss18(line);
            double dx, dy, dz, angle;
            ss18 >> dx >> dy >> dz >> angle;
            Point direction(dx, dy, dz);

            Color color(1, 1, 1);

            SpotLight* spotLight = new SpotLight(position, color, direction, angle, falloff);
            spotLights.push_back(spotLight);

        }





        while (getline(inputFile, line)) {
            //cout << line << endl;
        }



    }
    else {
        cout << "Unable to open file";
    }


    cout << nearDistance << " " << farDistance << " " << fovY << " " << aspectRatio << endl;
    cout << levelOfRecursion << endl;
    cout << pixelsAlongX << " " << pixelsAlongY << endl;
    cout << widthOfEachCell << endl;
    cout << ambientForCheckerBoard << " " << diffuseForCheckerBoard << " " << reflectionForCheckerBoard << endl;
    cout << numberOfObjects << endl;
    cout << endl;

}



void drawCheckerBoard() {
    for (int y = -100; y <= 100; ++y) {
        for (int x = -100; x <= 100; ++x) {
            bool isWhite = (y + x) % 2 == 0;
            if (isWhite) {
                glColor3f(1, 1, 1); // White
            }
            else {
                glColor3f(0, 0, 0); // Black
            }

            // glBegin(GL_QUADS);
            // glVertex3f(x * widthOfEachCell, 0, z * widthOfEachCell);
            // glVertex3f((x + 1) * widthOfEachCell, 0, z * widthOfEachCell);
            // glVertex3f((x + 1) * widthOfEachCell, 0, (z + 1) * widthOfEachCell);
            // glVertex3f(x * widthOfEachCell, 0, (z + 1) * widthOfEachCell);
            // glEnd();

            glBegin(GL_QUADS);
            glVertex3f(x * widthOfEachCell, y * widthOfEachCell, 0);
            glVertex3f((x + 1) * widthOfEachCell, y * widthOfEachCell, 0);
            glVertex3f((x + 1) * widthOfEachCell, (y + 1) * widthOfEachCell, 0);
            glVertex3f(x * widthOfEachCell, (y + 1) * widthOfEachCell, 0);
            glEnd();

        }
    }
}

void drawObjects() {
    for (int i = 0; i < spheres.size(); ++i) {
        spheres[i]->draw();
    }
    for (int i = 0; i < cubes.size(); ++i) {
        cubes[i]->draw();
    }
    for (int i = 0; i < pyramids.size(); ++i) {
        pyramids[i]->draw();
    }

    for (int i = 0; i < spotLights.size(); ++i) {
        spotLights[i]->draw();
    }

    for (int i = 0; i < lights.size(); ++i) {
        lights[i]->draw();
    }

}





/* Initialize OpenGL Graphics */
void initGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovY, aspectRatio, nearDistance, farDistance);
    glMatrixMode(GL_MODELVIEW);
}
// Global variables
Point pos;   // position of the eye
Point l;     // look/forward direction
Point r;     // right direction
Point u;     // up direction

/* Draw axes: X in Red, Y in Green and Z in Blue */
void drawAxes() {
    glLineWidth(3);
    glBegin(GL_LINES);
    glColor3f(1, 0, 0);   // Red
    // X axis
    glVertex3f(0, 0, 0);
    glVertex3f(1000, 0, 0);

    glColor3f(0, 1, 0);   // Green
    // Y axis
    glVertex3f(0, 0, 0);
    glVertex3f(0, 1000, 0);

    glColor3f(0, 0, 1);   // Blue
    // Z axis
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1000);
    glEnd();
}

float scale = 0;
float scaleStep = 0.05f;
void drawOctahedron() {

    // Define the vertices of a triangle
    GLfloat vertices[] = {
        (1.0f * scale) / 3 , 1 - (2.0f * scale) / 3, (1.0f * scale) / 3 ,     // Top vertex
        1 - (2.0f * scale) / 3 ,(1.0f * scale) / 3  ,(1.0f * scale) / 3 ,    // Bottom left vertex
        (1.0f * scale) / 3, (1.0f * scale) / 3,  1 - (2.0f * scale) / 3    // Bottom right vertex
    };


    // Apply rotations and translations to create the octahedron

        // Draw the triangle
    for (int i = 1; i <= 8; ++i) {
        glPushMatrix();
        int j = max(i, i - 4);
        glRotatef((j - 1) * 90, 0.0f, 1.0f, 0.0f);  // Rotate 90 degrees around the Y-axis 
        if (i > 4) {
            // mirror it on xy plane
            glRotatef(180, 1.0f, 0.0f, 0.0f);

        }
        glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f);  // Red
        glVertex3fv(vertices);  // Top vertex
        glColor3f(0.0f, 1.0f, 0.0f);  // Green
        glVertex3fv(vertices + 3);  // Bottom left vertex
        glColor3f(0.0f, 0.0f, 1.0f);  // Blue
        glVertex3fv(vertices + 6);  // Bottom right vertex
        glEnd();

        // glRotatef(i * 90.0f, 0.0f, 1.0f, 0.0f);  // Rotate 90 degrees around the Y-axis

        glPopMatrix();

    }


}




std::vector<float> buildUnitPositiveX(int subdivision)
{
    const float DEG2RAD = acos(-1) / 180.0f;

    std::vector<float> vertices;
    float n1[3];        // normal of longitudinal plane rotating along Y-axis
    float n2[3];        // normal of latitudinal plane rotating along Z-axis
    float v[3];         // direction vector intersecting 2 planes, n1 x n2
    float a1;           // longitudinal angle along Y-axis
    float a2;           // latitudinal angle along Z-axis

    // compute the number of vertices per row, 2^n + 1
    int pointsPerRow = (int)pow(2, subdivision) + 1;

    // rotate latitudinal plane from 45 to -45 degrees along Z-axis (top-to-bottom)
    for (unsigned int i = 0; i < pointsPerRow; ++i) {
        // normal for latitudinal plane
        // if latitude angle is 0, then normal vector of latitude plane is n2=(0,1,0)
        // therefore, it is rotating (0,1,0) vector by latitude angle a2
        a2 = DEG2RAD * (45.0f - 90.0f * i / (pointsPerRow - 1));
        n2[0] = -sin(a2);
        n2[1] = cos(a2);
        n2[2] = 0;

        // rotate longitudinal plane from -45 to 45 along Y-axis (left-to-right)
        for (unsigned int j = 0; j < pointsPerRow; ++j) {
            // normal for longitudinal plane
            // if longitude angle is 0, then normal vector of longitude is n1=(0,0,-1)
            // therefore, it is rotating (0,0,-1) vector by longitude angle a1
            a1 = DEG2RAD * (-45.0f + 90.0f * j / (pointsPerRow - 1));
            n1[0] = -sin(a1);
            n1[1] = 0;
            n1[2] = -cos(a1);

            // find direction vector of intersected line, n1 x n2
            v[0] = n1[1] * n2[2] - n1[2] * n2[1];
            v[1] = n1[2] * n2[0] - n1[0] * n2[2];
            v[2] = n1[0] * n2[1] - n1[1] * n2[0];

            // normalize direction vector
            float scale = 1 / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
            v[0] *= scale;
            v[1] *= scale;
            v[2] *= scale;

            // add a vertex into array
            vertices.push_back(v[0]);
            vertices.push_back(v[1]);
            vertices.push_back(v[2]);
        }
    }


    return vertices;
}

void drawSphere() {

    float vertices[] = { 1 - scale , 0 , 0,
       0, 1 - scale, 0,
       0, 0, 1 - scale,
         -1 + scale, 0, 0,
            0, -1 + scale, 0,
            0, 0, -1 + scale
    };

    float radius = (1.0f * scale) / sqrt(3);
    //radius = radius * 2.0f;
    int subdivision = 5;
    for (int i = 0; i < 6; ++i) {
        glPushMatrix();
        glTranslatef(vertices[3 * i + 0], vertices[3 * i + 1], vertices[3 * i + 2]);
        glColor3f(1, 1, 0);
        if (i == 1) {
            // set the color to red
            glColor3f(1, 0, 0);
            glRotatef(90, 0, 0, 1);
        }
        if (i == 2) {
            // set the color to white
            glColor3f(1, 1, 1);
            glRotatef(-90, 0, 1, 0);
        }
        if (i == 3) {
            // set the color to blue
            glColor3f(0, 0, 1);
            glRotatef(180, 0, 1, 0);
        }
        if (i == 4) {
            // set the color to cyan
            glColor3f(0, 1, 1);
            glRotatef(-90, 0, 0, 1);
        }
        if (i == 5) {
            // set the color to magenta
            glColor3f(1, 0, 1);
            glRotatef(90, 0, 1, 0);
        }
        vector<float> vertices = buildUnitPositiveX(subdivision);

        for (int i = 0; i < vertices.size(); i++) {
            //  vertices[i] = vertices[i] * radius;
        }
        int pointsPerRow = (int)pow(2, subdivision) + 1;
        for (unsigned int i = 0; i < pointsPerRow - 1; ++i) {
            glBegin(GL_TRIANGLE_STRIP);
            for (unsigned int j = 0; j < pointsPerRow; ++j) {
                // vertex 1
                // yellow color

                glNormal3f(vertices[3 * (i * pointsPerRow + j) + 0],
                    vertices[3 * (i * pointsPerRow + j) + 1],
                    vertices[3 * (i * pointsPerRow + j) + 2]);
                glVertex3f(vertices[3 * (i * pointsPerRow + j) + 0] * radius,
                    vertices[3 * (i * pointsPerRow + j) + 1] * radius,
                    vertices[3 * (i * pointsPerRow + j) + 2] * radius);

                // vertex 2
                glNormal3f(vertices[3 * ((i + 1) * pointsPerRow + j) + 0],
                    vertices[3 * ((i + 1) * pointsPerRow + j) + 1],
                    vertices[3 * ((i + 1) * pointsPerRow + j) + 2]);
                glVertex3f(vertices[3 * ((i + 1) * pointsPerRow + j) + 0] * radius,
                    vertices[3 * ((i + 1) * pointsPerRow + j) + 1] * radius,
                    vertices[3 * ((i + 1) * pointsPerRow + j) + 2] * radius);
            }
            glEnd();
        }
        glPopMatrix();
    }

}

void drawCylinder() {

    float vertices[] = {
        (1.0f * scale) / 3 , 1 - (2.0f * scale) / 3, (1.0f * scale) / 3 ,     // Top vertex
        1 - (2.0f * scale) / 3 ,(1.0f * scale) / 3  ,(1.0f * scale) / 3 ,    // Bottom left vertex
        (1.0f * scale) / 3, (1.0f * scale) / 3,  1 - (2.0f * scale) / 3    // Bottom right vertex
    };
    float newposx = (vertices[0] + vertices[3]) / 2;
    float newposy = (vertices[1] + vertices[4]) / 2;
    float newposz = (vertices[2] + vertices[5]) / 2;
    float xheight = vertices[1] - vertices[0];
    float radius = vertices[0] * sqrt(3);
    float height = sqrt(2) * xheight;
    int segments = 50;
    double tempx = radius, tempy = 0;
    double currx, curry;
    for (int i = 0; i < 12; ++i) {
        glPushMatrix();
        glTranslatef((1 - scale) * 0.5f, (1 - scale) * 0.5f, 0);
        // here begins the transloation 
        if (i == 1) {
            glTranslatef(-(1 - scale) * 0.5f * 2.0f, 0, 0);
        }
        if (i == 2) {
            glTranslatef(-(1 - scale) * 0.5f, 0, -(1 - scale) * 0.5f);
        }

        if (i == 3) {
            glTranslatef(-(1 - scale) * 0.5f, 0, (1 - scale) * 0.5f);
        }

        if (i == 4) {
            glTranslatef(0, -(1 - scale) * 0.5f * 2.0f, 0);
        }

        if (i == 5) {
            glTranslatef(-(1 - scale) * 0.5f * 2.0f, -(1 - scale) * 0.5f * 2.0f, 0);
        }
        if (i == 6) {
            glTranslatef(-(1 - scale) * 0.5f, -(1 - scale) * 0.5f * 2.0f, (1 - scale) * 0.5f);
        }
        if (i == 7) {
            glTranslatef(-(1 - scale) * 0.5f, -(1 - scale) * 0.5f * 2.0f, -(1 - scale) * 0.5f);

        }
        if (i == 8) {
            glTranslatef(0, -(1 - scale) * 0.5f, (1 - scale) * 0.5f);
        }

        if (i == 9) {
            glTranslatef(0, -(1 - scale) * 0.5f, -(1 - scale) * 0.5f);
        }
        if (i == 10) {
            glTranslatef(-(1 - scale) * 0.5f * 2.0f, -(1 - scale) * 0.5f, -(1 - scale) * 0.5f);
        }

        if (i == 11) {
            glTranslatef(-(1 - scale) * 0.5f * 2.0f, -(1 - scale) * 0.5f, (1 - scale) * 0.5f);
        }

        glRotatef(90, 1, 0, 0);
        glRotatef(45, 0, 1, 0);
        if (i == 1) {
            glRotatef(180, 1, 0, 1);
        }
        if (i == 2 or i == 6) {
            glRotatef(-45, 0, 1, 0);
            glRotatef(45, 1, 0, 0);
        }
        if (i == 3 or i == 7) {
            glRotatef(-45, 0, 1, 0);
            glRotatef(-45, 1, 0, 0);
        }
        if (i == 4) {
            glRotatef(90, 0, 1, 0);
        }
        if (i == 8 or i == 10) {
            glRotatef(45, 0, 1, 0);
            glRotatef(45, 1, 0, 0);
        }
        if (i == 9 or i == 11) {
            glRotatef(45, 0, 1, 0);
            glRotatef(-45, 1, 0, 0);
        }
        glBegin(GL_QUADS);
        for (int i = 1; i <= segments; i++) {
            double theta = i * 2.0 * M_PI / segments;
            currx = radius * cos(theta);
            curry = radius * sin(theta);

            GLfloat c = (2 + cos(theta)) / 3;
            glColor3f(c, c, c);
            glVertex3f(currx, curry, height / 2);
            glVertex3f(currx, curry, -height / 2);

            glVertex3f(tempx, tempy, -height / 2);
            glVertex3f(tempx, tempy, height / 2);

            tempx = currx;
            tempy = curry;
        }
        glEnd();
        glPopMatrix();
    }
}


/*  Handler for window-repaint event. Call back when the window first appears and
    whenever the window needs to be re-painted. */
GLfloat x = 0, y = 0, z = 0;
GLfloat rotationAngle = 0.0f;
GLfloat rotationRadius = 0.01f;
void display() {
    // glClear(GL_COLOR_BUFFER_BIT);            // Clear the color buffer (background)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);             // To operate on Model-View matrix
    glLoadIdentity();                       // Reset the model-view matrix




    // control viewing (or camera)
    gluLookAt(pos.x, pos.y, pos.z,
        pos.x + l.x, pos.y + l.y, pos.z + l.z,
        u.x, u.y, u.z);

    // draw
    //drawSphere(5,100,100);
    //drawCone(10, 5, 10);
    glPushMatrix();
    glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f);

    // your draw fucntions goes here 

    drawCheckerBoard();
    drawObjects();
    // drawGrid();


    glPopMatrix();
    glutSwapBuffers();  // Render now
}


double windowWidth = 740, windowHeight = 740;
double viewAngle = 80;
bool isWLoaded = false, isBLoaded = false;
Color stretched_buff[2000][2000];
void loadTexture() {


    cout << "inside load texture" << endl;
    // load the "texture_w.bmp" file  using bitmap.h/.cpp\ 


    bitmap_image image_z("texture_w.bmp");

    if (!image_z) {
        isWLoaded = false;
        return;
    }
    int mndim = 256;
    int wz = image_z.width();
    int hz = image_z.height();
    w_height = mndim;
    w_width = mndim;
    bitmap_image image_w(mndim, mndim);
    for (int i = 0; i < w_width; i++) {
        for (int j = 0; j < w_height; j++) {
            rgb_t color;
            double x = (double)i / (double)w_width;
            double y = (double)j / (double)w_height;
            x *= (double)wz;
            y *= (double)hz;
            image_z.get_pixel(x, y, color);
            image_w.set_pixel(j, i, color.red, color.green, color.blue);
        }
    }



    w_height = image_w.height();
    w_width = image_w.width();
    cout << " w_height " << w_height << " w_width " << w_width << endl;
    w_width -= 10;
    w_height -= 10;
    for (std::size_t y = 0; y < w_height; ++y) {
        for (std::size_t x = 0; x < w_width; ++x) {
            rgb_t color;
            image_w.get_pixel(x, y, color);
            w_buff[x][y] = Color(color.red, color.green, color.blue);
            w_buff[x][y].r /= 255.0;
            w_buff[x][y].g /= 255.0;
            w_buff[x][y].b /= 255.0;
        }
    }

    bitmap_image image_x("texture_b.bmp");
    int mindim = min((int)image_x.height(), (int)image_x.width());
    mindim = 256 ;
    int ww = image_x.width();
    int hh = image_x.height();
    b_height = mindim;
    b_width = mindim;
    bitmap_image image_b(mindim, mindim);
    for (int i = 0; i < b_width; i++) {
        for (int j = 0; j < b_height; j++) {
            rgb_t color;
            double x = (double)i / (double)b_width;
            double y = (double)j / (double)b_height;
            x *= (double)ww;
            y *= (double)hh;
            image_x.get_pixel(x, y, color);
            image_b.set_pixel(j, i, color.red, color.green, color.blue);
        }
    }

    if (!image_b) {
        isBLoaded = false;
        return;
    }
    b_height = image_b.height();
    b_width = image_b.width();

    b_width -= 10;
    b_height -= 10;
    cout << " b_height " << b_height << " b_width " << b_width << endl;
    for (std::size_t y = 0; y < b_height; ++y) {
        for (std::size_t x = 0; x < w_width; ++x) {
            rgb_t color;
            image_b.get_pixel(y, x, color);
            b_buff[x][y] = Color(color.red, color.green, color.blue);
            b_buff[x][y].r /= 255.0;
            b_buff[x][y].g /= 255.0;
            b_buff[x][y].b /= 255.0;

        }
    }








    isBLoaded = true;
    isWLoaded = true;
    cout << "Successfully loaded the textures" << endl;
}
void generateImage() {
    cout << "Generating image..." << endl;
    objects.back()->isCheckerBoard = true;
    double imageWidth = pixelsAlongX + 500;
    double imageHeight = pixelsAlongY + 500;
    // set the image width and height 
    image.setwidth_height(imageWidth, imageHeight);
    // initialize bitmap image and set background color to black

    for (int i = 0;i < imageWidth;i++)
        for (int j = 0;j < imageHeight;j++)
            image.set_pixel(i, j, 0, 0, 0);

    double planeDistance = (windowHeight / 2.0) / tan((pi * 80) / (360.0));
    Point topLeft = pos + (l * planeDistance) + (u * (windowHeight / 2.0)) - (r * (windowWidth / 2.0));
    double du = windowWidth / (imageWidth * 1.0);
    double dv = windowHeight / (imageHeight * 1.0);
    topLeft = topLeft + (r * du / 2.0) - (u * dv / 2.0);
    int nearest = -1;
    ;
    vector <pair<pair<int, int>, Color>> pixels;
    for (int i = 0;i < imageWidth;i++) {
        for (int j = 0;j < imageHeight;j++) {

            Point pixel = topLeft + (r * du * i) - (u * dv * j);
            Ray ray(pos, pixel - pos);
            Color color; // = Color(0, 0, 0);
            double tMin = -1;
            nearest = -1;
            for (int k = 0;k < (int)objects.size();k++) {
                double t = objects[k]->intersect(ray, color, 0);

                if (t < tMin || nearest == -1) {
                    if (t > 0) {
                        tMin = t;
                        nearest = k;
                    }
                }
            }
            if (nearest != -1) {
                color = Color(0, 0, 0);

                double t = objects[nearest]->Illuminate(ray, color, levelOfRecursion);



                color.r = max(0.0, min(1.0, color.r));
                color.g = max(0.0, min(1.0, color.g));
                color.b = max(0.0, min(1.0, color.b));



                // }
                image.set_pixel(i, j, color.r * 255, color.g * 255, color.b * 255);
                pixels.push_back({ {i,j}, color });
                //cout << "intersection hoise" << endl;
                //cout << color.r * 255 << " " << color.g * 255 << " " << color.b * 255 << endl;

            }
        }
    }

    cout << zeroLambert << " " << nonZeroLambert << endl;
    cout << zeroPhong << " " << nonZeroPhong << endl;
    if (!isTextureEnabled) {
        image.save_image("1805023.bmp");
    }
    else {
        image.save_image("1805023_texture.bmp");
    }
    cout << "Image Generated" << endl;
}

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0) height = 1;                // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping area to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset the projection matrix
    /*if (width >= height) {
        // aspect >= 1, set the height from -1 to 1, with larger width
        gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
    } else {
        // aspect < 1, set the width to -1 to 1, with larger height
        gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
    }*/
    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}


void clockWise() {

    rotationAngle = 10.0f;
    GLfloat rotationAngleRad = rotationAngle * (M_PI / 180.0);

    // Calculate the distance between the camera position and the origin
    GLfloat distanceToOrigin = sqrt(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z);

    // Rotate the camera position around the origin
    GLfloat newX = cos(rotationAngleRad) * pos.x + sin(rotationAngleRad) * pos.z;
    GLfloat newZ = -sin(rotationAngleRad) * pos.x + cos(rotationAngleRad) * pos.z;
    pos.x = newX;
    pos.z = newZ;

    // Recalculate the camera's forward direction
    l.x = -pos.x / distanceToOrigin;
    l.y = -pos.y / distanceToOrigin;
    l.z = -pos.z / distanceToOrigin;

    // Recalculate the camera's up direction
    // u.x = l.y * r.z - l.z * r.y;
    // u.y = l.z * r.x - l.x * r.z;
    // u.z = l.x * r.y - l.y * r.x;

    // // Recalculate the camera's right direction
    // r.x = u.y * l.z - u.z * l.y;
    // r.y = u.z * l.x - u.x * l.z;
    // r.z = u.x * l.y - u.y * l.x;

    // // Normalize the right direction vector
    // GLfloat rLength = sqrt(r.x * r.x + r.y * r.y + r.z * r.z);
    // r.x /= rLength;
    // r.y /= rLength;
    // r.z /= rLength;

    // r.x = 0.7;
    // r.y = 0;
    // r.z = 0.7;





}


void counterClockwise() {
    rotationAngle = 10.0f;
    GLfloat rotationAngleRad = rotationAngle * (M_PI / 180.0);

    // Calculate the distance between the camera position and the origin
    GLfloat distanceToOrigin = sqrt(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z);

    // Rotate the camera position around the origin
    GLfloat newX = cos(rotationAngleRad) * pos.x - sin(rotationAngleRad) * pos.z;
    GLfloat newZ = sin(rotationAngleRad) * pos.x + cos(rotationAngleRad) * pos.z;
    pos.x = newX;
    pos.z = newZ;
    // Recalculate the camera's forward direction
    l.x = -pos.x / distanceToOrigin;
    l.y = -pos.y / distanceToOrigin;
    l.z = -pos.z / distanceToOrigin;

    // Recalculate the camera's up direction
    // u.x = l.y * r.z - l.z * r.y;
    // u.y = l.z * r.x - l.x * r.z;
    // u.z = l.x * r.y - l.y * r.x;

    // // Recalculate the camera's right direction
    // r.x = u.y * l.z - u.z * l.y;
    // r.y = u.z * l.x - u.x * l.z;
    // r.z = u.x * l.y - u.y * l.x;

    // // Normalize the right direction vector
    // GLfloat rLength = sqrt(r.x * r.x + r.y * r.y + r.z * r.z);
    // r.x /= rLength;
    // r.y /= rLength;
    // r.z /= rLength;

    // r.x = 0.7;
    // r.y = 0;
    // r.z = 0.7;


}
void keyboardListener(unsigned char key, int xx, int yy) {
    double rate = 0.01;
    switch (key) {

    case '1':
        r.x = r.x * cos(rate) + l.x * sin(rate);
        r.y = r.y * cos(rate) + l.y * sin(rate);
        r.z = r.z * cos(rate) + l.z * sin(rate);

        l.x = l.x * cos(rate) - r.x * sin(rate);
        l.y = l.y * cos(rate) - r.y * sin(rate);
        l.z = l.z * cos(rate) - r.z * sin(rate);
        break;

    case '2':
        r.x = r.x * cos(-rate) + l.x * sin(-rate);
        r.y = r.y * cos(-rate) + l.y * sin(-rate);
        r.z = r.z * cos(-rate) + l.z * sin(-rate);

        l.x = l.x * cos(-rate) - r.x * sin(-rate);
        l.y = l.y * cos(-rate) - r.y * sin(-rate);
        l.z = l.z * cos(-rate) - r.z * sin(-rate);
        break;

    case '3':
        l.x = l.x * cos(rate) + u.x * sin(rate);
        l.y = l.y * cos(rate) + u.y * sin(rate);
        l.z = l.z * cos(rate) + u.z * sin(rate);

        u.x = u.x * cos(rate) - l.x * sin(rate);
        u.y = u.y * cos(rate) - l.y * sin(rate);
        u.z = u.z * cos(rate) - l.z * sin(rate);
        break;

    case '4':
        l.x = l.x * cos(-rate) + u.x * sin(-rate);
        l.y = l.y * cos(-rate) + u.y * sin(-rate);
        l.z = l.z * cos(-rate) + u.z * sin(-rate);

        u.x = u.x * cos(-rate) - l.x * sin(-rate);
        u.y = u.y * cos(-rate) - l.y * sin(-rate);
        u.z = u.z * cos(-rate) - l.z * sin(-rate);
        break;

    case '5':
        u.x = u.x * cos(rate) + r.x * sin(rate);
        u.y = u.y * cos(rate) + r.y * sin(rate);
        u.z = u.z * cos(rate) + r.z * sin(rate);

        r.x = r.x * cos(rate) - u.x * sin(rate);
        r.y = r.y * cos(rate) - u.y * sin(rate);
        r.z = r.z * cos(rate) - u.z * sin(rate);
        break;

    case '6':
        u.x = u.x * cos(-rate) + r.x * sin(-rate);
        u.y = u.y * cos(-rate) + r.y * sin(-rate);
        u.z = u.z * cos(-rate) + r.z * sin(-rate);

        r.x = r.x * cos(-rate) - u.x * sin(-rate);
        r.y = r.y * cos(-rate) - u.y * sin(-rate);
        r.z = r.z * cos(-rate) - u.z * sin(-rate);
        break;
    case 'a':
        //rotate the camera clockwise
        rotationAngle += 10;
        //clockWise();


        break;
    case 'd':
        rotationAngle -= 10;
        //counterClockwise();
        break;
    case '7':

        break;
    case ',':
        scale += scaleStep;
        if (scale > 1.0f)
            scale = 1.0f;
        break;
    case '.':
        scale -= scaleStep;
        if (scale < 0.0f)
            scale = 0.0f;
        break;

    case '0':
        generateImage();
        break;

    case ' ':
        isTextureEnabled = !isTextureEnabled;
        if (isTextureEnabled) {
            cout << "Texture is enabled" << endl;
        }
        else {
            cout << "Texture is disabled" << endl;
        }
        break;

    default:
        break;
    }
    glutPostRedisplay();
}


void specialKeyListener(int key, int x, int y)
{
    switch (key) {
    case GLUT_KEY_UP:		//down arrow key
        pos.x += l.x;
        pos.y += l.y;
        pos.z += l.z;
        break;
    case GLUT_KEY_DOWN:		// up arrow key
        pos.x -= l.x;
        pos.y -= l.y;
        pos.z -= l.z;
        break;

    case GLUT_KEY_RIGHT:
        pos.x += r.x;
        pos.y += r.y;
        pos.z += r.z;
        break;
    case GLUT_KEY_LEFT:
        pos.x -= r.x;
        pos.y -= r.y;
        pos.z -= r.z;
        break;

    case GLUT_KEY_PAGE_UP:
        pos.x += u.x;
        pos.y += u.y;
        pos.z += u.z;
        break;
    case GLUT_KEY_PAGE_DOWN:
        pos.x -= u.x;
        pos.y -= u.y;
        pos.z -= u.z;
        break;

    case GLUT_KEY_INSERT:
        break;

    case GLUT_KEY_HOME:
        break;
    case GLUT_KEY_END:
        pos.x = 3;pos.y = 3; pos.z = 3;
        l.x = -1;l.y = -1;l.z = -1;
        u.x = 0;u.y = 1;u.z = 0;
        r.x = 0.3;r.y = 0;r.z = -0.3;

    default:
        break;
    }
    glutPostRedisplay();
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {


    readInputFile();
    cout << "file is read" << endl;
    loadTexture();
    pos.x = 0;pos.y = -150; pos.z = 60;
    l.x = 0;l.y = 1;l.z = 0;
    //determine the rand u vector from given posand l
    Point temp;
    temp.x = 0;temp.y = 1;temp.z = 0;
    // cross l and temp to get r
    r.x = l.y * temp.z - l.z * temp.y;
    r.y = l.z * temp.x - l.x * temp.z;
    r.z = l.x * temp.y - l.y * temp.x;

    // cross r and l to get u
    u.x = r.y * l.z - r.z * l.y;
    u.y = r.z * l.x - r.x * l.z;
    u.z = r.x * l.y - r.y * l.x;

    r.x = 1, r.y = 0, r.z = 0;
    u.x = 0, u.y = 0, u.z = 1;

    glutInit(&argc, argv);                  // Initialize GLUT
    glutInitWindowSize(740, 740);           // Set the window's initial width & height
    glutInitWindowPosition(50, 50);         // Position the window's initial top-left corner
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color
    glutCreateWindow("OpenGL 3D Drawing 2");          // Create a window with the given title
    glutDisplayFunc(display);               // Register display callback handler for window re-paint
   // glutReshapeFunc(reshape);               // Register callback handler for window re-shape

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);

    initGL();                               // Our own OpenGL initialization
    glutMainLoop();                         // Enter the event-processing loop
    return 0;
}
