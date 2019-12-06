/*
 * Author: Zhekun Qi
 * Class: ECE 6122
 * Last Date Modified: Dec 3 2019
 * Description: functions for UAV.h
 */


#include "UAV.h"
#include <math.h>
void UAV::setPosition(double x, double y, double z) {
        this->position[0] = x;
        this->position[1] = y;
        this->position[2] = z;
}

void UAV::setSpeed(double x, double y, double z) {
    this->speed[0] = x;
    this->speed[1] = y;
    this->speed[2] = z;
}

UAV::UAV(double x, double y, double z)  {
    this->position[0] = x;
    this->position[1] = y;
    this->position[2] = z;
}

UAV::UAV() {

}

void UAV::setAccelerate(double x, double y, double z) {
    this->accelerate[0] = x;
    this->accelerate[1] = y;
    this->accelerate[2] = z;
}

double UAV::totalSpeed() {
    double result = sqrt(speed[0] * speed[0] + speed[1] * speed[1] + speed[2] * speed[2]);
    return result;
}

void UAV::setColor(double color) {
    UAV::color = color;
}





