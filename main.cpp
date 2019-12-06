/*
 * Author: Zhekun Qi
 * Class: ECE 6122
 * Last Date Modified: Dec 3 2019
 * Description: main function, using MPI and OpenGl
 */

#include <iostream>
//#include <OpenGL/gl.h>
//#include <GLUT/glut.h>
#include <GL/glut.h>
#include "footballField.h"
#include <vector>
#include <math.h>
#include <chrono>
#include <thread>
#include "Bitmap.h"
#include "mpi.h"


//location for eye
static float eyex = 0.0;
static float eyey = -140.0;
static float eyez = 100.0;
#define GL_SILENCE_DEPRECATION

footballField theField;  //the object to contain all things
// initialize all the UAVs
UAV UAVs[15] = {UAV(-50, -35, 0), UAV(-50, 0, 0), UAV(-50, 35, 0),
                UAV(-25, -35, 0), UAV(-25, 0, 0), UAV(-25, 35, 0),
                UAV(0, -35, 0), UAV(0, 0, 0), UAV(0, 35, 0),
                UAV(25, -35, 0), UAV(25, 0, 0), UAV(25, 35, 0),
                UAV(50, -35, 0), UAV(50, 0, 0), UAV(50, 35, 0),};

//used for the first 5 seconds wait time
double haveWaited = 0;
//these four are used for change color
double steps = 0;
double colorNum = 0;
double color = 1;
bool decrease = true;
//used for bmp texture
GLuint texture[2];
float x_angle = 0;
float y_angle = 0;
float z_angle = 0;
struct Image {

    unsigned long sizeX;

    unsigned long sizeY;

    char *data;

};
typedef struct Image Image;
BMP inBitmap;


#define checkImageWidth 64

#define checkImageHeight 64
GLubyte checkImage[checkImageWidth][checkImageHeight][3];
const int numElements = 6; // x, y, z, vx, vy, vz

const int rcvSize = 16 * 6; // (Main task + 15 UAVs) * numElements

double *rcvbuffer = new double[rcvSize];

double sendBuffer[numElements];

/**
 * used for check Image
 */
void makeCheckImage(void) {

    int i, j, c;

    for (i = 0; i < checkImageWidth; i++) {

        for (j = 0; j < checkImageHeight; j++) {

            c = ((((i & 0x8) == 0) ^ ((j & 0x8) == 0))) * 255;

            checkImage[i][j][0] = (GLubyte) c;

            checkImage[i][j][1] = (GLubyte) c;

            checkImage[i][j][2] = (GLubyte) c;

        }

    }

}

/**
 * initialize the graph
 */
void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    glDepthFunc(GL_LESS);
    //glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    //glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
    inBitmap.read("ff.bmp");
    makeCheckImage();
    // Create Textures

    glGenTextures(1, texture);

    //Setup first texture
    glBindTexture(GL_TEXTURE_2D, texture[0]);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, inBitmap.bmp_info_header.width, inBitmap.bmp_info_header.height, 0,
                 GL_BGR_EXT, GL_UNSIGNED_BYTE, &inBitmap.data[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glEnable(GL_TEXTURE_2D);

}

/**
 * reshape the window
 * @param w
 * @param h
 */
void reshape(int w, int h) {

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat) w / (GLfloat) h, 0.1, 5000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);

}

/**
 * display all the field image and UAVs
 */
void display() {

    using namespace std::chrono;
    using namespace std::this_thread;
    init();
    gluLookAt((GLfloat) eyex, (GLfloat) eyey, (GLfloat) eyez, 0, 0, 0.0, 0.0, 0, 1);
    //the first wait time not use now
    if (haveWaited == 2) {
        sleep_for(seconds(5));
    }
    haveWaited++;
    //change the texture and draw the ball and the UAVs
    glPushMatrix();
    glRotatef(x_angle, 1.0, 0.0, 0.0);
    glRotatef(y_angle, 0.0, 1.0, 0.0);
    glRotatef(z_angle, 0.0, 0.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    theField.drawball();
    // draw all the UAVs including change the color
    for (int i = 0; i < 15; ++i) {
        double x1 = rcvbuffer[i * 6 + 6];
        double y1 = rcvbuffer[i * 6 + 7];
        double z1 = rcvbuffer[i * 6 + 8];
        if (colorNum == 20 && decrease) {
            color -= double(1.0 / 255);
            colorNum = 0;
        } else if (colorNum == 20 && !decrease) {
            color += double(1.0 / 255);
            colorNum = 0;
        }
        if (color <= 0.5)
            decrease = false;
        else if (color >= 1)
            decrease = true;
        colorNum++;
        glColor4f(color, 0, 0, 1);
        glPushMatrix();
        glTranslated(x1, y1, z1);
        glScaled(1.5, 1.5, 1.5);
        glutSolidTetrahedron();
        glPopMatrix();
        //std::cout << x1 << y1 << z1 << std::endl;
    }

    //draw the field using image
    glBindTexture(GL_TEXTURE_2D, texture[0]);

    glBegin(GL_QUADS);
    glTexCoord2f(1, 1);
    glVertex3f(64.0f, 40.0f, 0.0f);
    glTexCoord2f(0, 1);
    glVertex3f(-64.0f, 40.0f, 0.0f);
    glTexCoord2f(0, 0);
    glVertex3f(-64.0f, -40.0f, 0.0f);
    glTexCoord2f(1, 0);
    glVertex3f(64.0f, -40.0f, 0.0f);
    glEnd();
    glPopMatrix();
    glutSwapBuffers();
    MPI_Allgather(sendBuffer, numElements, MPI_DOUBLE, rcvbuffer, numElements, MPI_DOUBLE, MPI_COMM_WORLD);


}
/**
 * change time
 * @param id
 */
void timerFunction(int id) {
    glutPostRedisplay();
    glutTimerFunc(100, timerFunction, 0);
}

/**
 * change eye position
 * @param key
 * @param xx
 * @param yy
 */
void normalKey(unsigned char key, int xx, int yy) {

    if (key == 'r' || key == 'R') {
        double theta = atan2((eyey - 0), (eyex - 0));
        // Add 10 degree to the angle
        theta = theta - (5.0 / 180) * 3.14;
        // Find the distance from the center
        double r = sqrt((eyex - 0) * (eyex - 0) + (eyey - 0) * (eyey - 0));
        // Calculate the new position of the camera
        eyex = r * cos(theta);
        eyey = r * sin(theta);
    } else if (key == 'd' || key == 'D') {
        eyez = eyez - 2;

    } else if (key == 'u' || key == 'U') {
        eyez = eyez + 2;
    }
}
/**
 * main function for OPenGL
 * @param argc
 * @param argv
 * @return
 */
int mainOpenGL(int argc, char **argv) {

    int mode = GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH;
    glutInit(&argc, argv);
    glutInitDisplayMode(mode);
    glutInitWindowSize(400, 400);
    glutCreateWindow("project Zhekun Qi");
    //init();
    //glutIdleFunc(update); // incremental update
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutTimerFunc(100, timerFunction, 0);
    glutKeyboardFunc(normalKey);
    glutMainLoop();
    return 0;
}

int main(int argc, char **argv) {
    int numTasks, rank;
    int rc = MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numTasks);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int gsize = 0;

    MPI_Comm_size(MPI_COMM_WORLD, &gsize);


    if (rank == 0) {
        mainOpenGL(argc, argv);
    } else {
        int count = rank - 1;
        for (int ii = 0; ii < 6000; ii++) {
            theField.gotoGoal(UAVs[count], count);
            sendBuffer[0] = UAVs[count].getPosition()[0];
            sendBuffer[1] = UAVs[count].getPosition()[1];
            sendBuffer[2] = UAVs[count].getPosition()[2];
            sendBuffer[3] = UAVs[count].getSpeed()[0];
            sendBuffer[4] = UAVs[count].getSpeed()[1];
            sendBuffer[5] = UAVs[count].getSpeed()[2];

            MPI_Allgather(sendBuffer, numElements, MPI_DOUBLE, rcvbuffer, numElements, MPI_DOUBLE, MPI_COMM_WORLD);
            // chang speed if collision happen
            for (int i = 0; i < 15; ++i) {
                if (i == count)
                    break;
                if (UAVs[count].getPosition()[0] == rcvbuffer[6 * i + 6] &&
                    UAVs[count].getPosition()[1] == rcvbuffer[6 * i + 7] &&
                    UAVs[count].getPosition()[2] == rcvbuffer[6 * i + 8]) {
                    UAVs[count].setSpeed(rcvbuffer[6 * i + 9], rcvbuffer[6 * i + 10], rcvbuffer[6 * i + 11]);

                }
            }
        }
    }
    MPI_Finalize();
    return 0;
}
