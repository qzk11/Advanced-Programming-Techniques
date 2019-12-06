/*
 * Author: Zhekun Qi
 * Class: ECE 6122
 * Last Date Modified: Dec 3 2019
 * Description: store all the info for UAVs
 */
#ifndef PROJECT_UAV_H
#define PROJECT_UAV_H


class UAV {
private:
    double position[3];
    double speed[3];
    double accelerate[3];

    double mass = 1;
    double color = 1;
public:
    bool haveClosed = false;

    UAV();
    /**
     * initialize the UAV by position
     */
    UAV(double, double, double);

    const double *getPosition() const {
        return position;
    }

    const double *getSpeed() const {
        return speed;
    }

    const double *getAccelerate() const {
        return accelerate;
    }


    const double getColor() const {
        return color;
    }

    double getMass() const {
        return mass;
    }

    double totalSpeed();

    void setPosition(double, double, double);


    void setSpeed(double, double, double);

    void setAccelerate(double, double, double);

    void setColor(double color);

};

#endif //PROJECT_UAV_H
