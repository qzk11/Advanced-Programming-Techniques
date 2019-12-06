/*
 * Author: Zhekun Qi
 * Class: ECE 6122
 * Last Date Modified: Dec 3 2019
 * Description: functions for footballField.h
 */

#include <GL/glut.h>
#include "footballField.h"
#include <math.h>
#include "UAV.h"
#include<iostream>

#define GL_SILENCE_DEPRECATION

void footballField::drawfield() {
    glColor4f(0, 1, 0, 1);
    glBegin(GL_POLYGON);
    glPushMatrix();
    glVertex3f(-55, -40, 0);
    glVertex3f(-55, 40, 0);
    glVertex3f(55, 40, 0);
    glVertex3f(55, -40, 0);
    glEnd();
    glPopMatrix();


}


void footballField::drawUAVs(UAV u1) {
        glColor4f(u1.getColor(), 0, 0, 1);
        glPushMatrix();
        glTranslated(u1.getPosition()[0], u1.getPosition()[1], u1.getPosition()[2]);
        glScaled(2, 2, 2);
        glutSolidTetrahedron();
        glPopMatrix();


}


void footballField::gotoGoal(UAV &u1, int i) {
    double goal[3] = {0, 0, 50};
    double maxSpeed0 = 2;
    double maxNewton = 20;
        if (u1.totalSpeed() <= maxSpeed0 && getDistance(u1, goal) > 10 && !u1.haveClosed) {
            int num = (i >= 9) ? -1 : 1;
            double alpha1;
            double alpha2;
            if (i == 6)
                alpha1 = atan(1) * 2;
            else if (i == 8)
                alpha1 = -atan(1) * 2;
            else
                alpha1 = atan((goal[1] - u1.getPosition()[1]) / (goal[0] - u1.getPosition()[0])); //  y/x
            alpha2 = atan((goal[2] - u1.getPosition()[2]) /
                          sqrt((goal[0] - u1.getPosition()[0]) * (goal[0] - u1.getPosition()[0]) +
                               (goal[1] - u1.getPosition()[1]) * (goal[1] - u1.getPosition()[1])
                          ));                                                                                         // z/(x^2 + y^2)
            if (i == 7)
                u1.setAccelerate(0, 0, maxNewton);
            else {
                u1.setAccelerate(num * maxNewton * cos(alpha2) * cos(alpha1),
                                 num * maxNewton * cos(alpha2) * sin(alpha1),
                                 maxNewton * sin(alpha2));
            }

            u1.setSpeed(u1.getSpeed()[0] + 0.1 * u1.getAccelerate()[0],
                        u1.getSpeed()[1] + 0.1 * u1.getAccelerate()[1],
                        u1.getSpeed()[2] + 0.1 * u1.getAccelerate()[2]);
            u1.setPosition(u1.getPosition()[0] + 0.1 * u1.getSpeed()[0] +
                           0.5 * 0.1 * 0.1 * u1.getAccelerate()[0],
                           u1.getPosition()[1] + 0.1 * u1.getSpeed()[1] +
                           0.5 * 0.1 * 0.1 * u1.getAccelerate()[1],
                           u1.getPosition()[2] + 0.1 * u1.getSpeed()[2] +
                           0.5 * 0.1 * 0.1 * u1.getAccelerate()[2]
            );
        } else if (getDistance(u1, goal) > 15) {
            u1.setAccelerate(0, 0, 0);
            u1.setSpeed(u1.getSpeed()[0] + 0.1 * u1.getAccelerate()[0],
                        u1.getSpeed()[1] + 0.1 * u1.getAccelerate()[1],
                        u1.getSpeed()[2] + 0.1 * u1.getAccelerate()[2]);
            u1.setPosition(u1.getPosition()[0] + 0.1 * u1.getSpeed()[0] +
                           0.5 * 0.1 * 0.1 * u1.getAccelerate()[0],
                           u1.getPosition()[1] + 0.1 * u1.getSpeed()[1] +
                           0.5 * 0.1 * 0.1 * u1.getAccelerate()[1],
                           u1.getPosition()[2] + 0.1 * u1.getSpeed()[2] +
                           0.5 * 0.1 * 0.1 * u1.getAccelerate()[2]
            );
        } else if (getDistance(u1, goal) <= 15 && getDistance(u1, goal) > 12) {
            if(i == 2 || i == 5 || i ==8 || i == 11|| i ==14)
                u1.setSpeed(0, -2, 2);
            else if(i == 1)
                u1.setSpeed(2,0, 2);
            else if(i == 13)
                u1.setSpeed(-2,0,2);
            else
               u1.setSpeed(0, 2, 2);
//            u1.setSpeed(u1.getSpeed()[0] + 0.1 * u1.getAccelerate()[0],
//                        u1.getSpeed()[1] + 0.1 * u1.getAccelerate()[1],
//                        u1.getSpeed()[2] + 0.1 * u1.getAccelerate()[2]);
            u1.setPosition(u1.getPosition()[0] + 0.1 * u1.getSpeed()[0] +
                           0.5 * 0.1 * 0.1 * u1.getAccelerate()[0],
                           u1.getPosition()[1] + 0.1 * u1.getSpeed()[1] +
                           0.5 * 0.1 * 0.1 * u1.getAccelerate()[1],
                           u1.getPosition()[2] + 0.1 * u1.getSpeed()[2] +
                           0.5 * 0.1 * 0.1 * u1.getAccelerate()[2]);


        } else if (getDistance(u1, goal) <= 12 || u1.haveClosed) {
            flyAround(u1);
            u1.haveClosed = true;

        }

}
double footballField::getDistance(UAV u, double *g) {
    double distance = sqrt((g[0] - u.getPosition()[0]) * (g[0] - u.getPosition()[0]) +
                           (g[1] - u.getPosition()[1]) * (g[1] - u.getPosition()[1]) +
                           (g[2] - u.getPosition()[2]) * (g[2] - u.getPosition()[2]));
    return distance;

}





void footballField::flyAround(UAV &u1) {
    double goal[3] = {0, 0, 50};
    double xx = (goal[0] - u1.getPosition()[0]) /
                sqrt((goal[0] - u1.getPosition()[0]) * (goal[0] - u1.getPosition()[0]) +
                     (goal[1] - u1.getPosition()[1]) * (goal[1] - u1.getPosition()[1]) +
                     (goal[2] - u1.getPosition()[2]) * (goal[2] - u1.getPosition()[2]));

    double yy = (goal[1] - u1.getPosition()[1]) /
                sqrt((goal[0] - u1.getPosition()[0]) * (goal[0] - u1.getPosition()[0]) +
                     (goal[1] - u1.getPosition()[1]) * (goal[1] - u1.getPosition()[1]) +
                     (goal[2] - u1.getPosition()[2]) * (goal[2] - u1.getPosition()[2]));

    double zz = (goal[2] - u1.getPosition()[2]) /
                sqrt((goal[0] - u1.getPosition()[0]) * (goal[0] - u1.getPosition()[0]) +
                     (goal[1] - u1.getPosition()[1]) * (goal[1] - u1.getPosition()[1]) +
                     (goal[2] - u1.getPosition()[2]) * (goal[2] - u1.getPosition()[2]));
    double fx = xx * u1.totalSpeed() * u1.totalSpeed() / getDistance(u1, goal);
    double fy = yy * u1.totalSpeed() * u1.totalSpeed() / getDistance(u1, goal);
    double fz = zz * u1.totalSpeed() * u1.totalSpeed() / getDistance(u1, goal);
    double hooke = 2.5 * (10 - getDistance(u1, goal));
    double hx = -xx * hooke;
    double hy = -yy * hooke;
    double hz = -zz * hooke;
    u1.setAccelerate(fx + hx, fy + hy, fz + hz);
    u1.setSpeed(u1.getSpeed()[0] + 0.1 * u1.getAccelerate()[0],
                u1.getSpeed()[1] + 0.1 * u1.getAccelerate()[1],
                u1.getSpeed()[2] + 0.1 * u1.getAccelerate()[2]);
    u1.setPosition(u1.getPosition()[0] + 0.1 * u1.getSpeed()[0] +
                   0.5 * 0.1 * 0.1 * u1.getAccelerate()[0],
                   u1.getPosition()[1] + 0.1 * u1.getSpeed()[1] +
                   0.5 * 0.1 * 0.1 * u1.getAccelerate()[1],
                   u1.getPosition()[2] + 0.1 * u1.getSpeed()[2] +
                   0.5 * 0.1 * 0.1 * u1.getAccelerate()[2]);

}

void footballField::drawball() {


    glPushMatrix();
    glTranslated(0, 0, 50);
    glColor3f(1.0, 1.0, 1.0);
    glutWireSphere(10.0, 10, 8);   /* draw sun */
    glPopMatrix();
}



double footballField::UAVdistance(UAV a1, UAV a2) {
    double result = sqrt((a1.getPosition()[0] - a2.getPosition()[0]) * (a1.getPosition()[0] - a2.getPosition()[0]) +
                         (a1.getPosition()[1] - a2.getPosition()[1]) * (a1.getPosition()[1] - a2.getPosition()[1]) +
                         (a1.getPosition()[2] - a2.getPosition()[2]) * (a1.getPosition()[2] - a2.getPosition()[2]));
    return result;
}


