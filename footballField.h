/*
 * Author: Zhekun Qi
 * Class: ECE 6122
 * Last Date Modified: Dec 3 2019
 * Description: this class draw the UAVs and also calculate info for UAVs
 */
#ifndef PROJECT_FOOTBALLFIELD_H
#define PROJECT_FOOTBALLFIELD_H

#include "UAV.h"

#define GL_SILENCE_DEPRECATION

class footballField {
public:
    /**
     * used for draw original field, not used in the final code
     */
    void drawfield();

    /**
     * draw the ball
     */
    void drawball();

    /**
     * used for draw original UAVs
     */
    void drawUAVs(UAV);

    /**
     * calculate the info before close to the ball
     * @param u1
     */
    void gotoGoal(UAV &u1, int);

    /**
     * the distance between UAV and goal
     * @param u  the UAV
     * @param g  the goal position
     * @return
     */
    double getDistance(UAV u, double g[3]);

    /**
     * calculate info after close to the ball
     * @param u1
     */
    void flyAround(UAV &u1);

    /**
     * not used
     * @return distance
     */
    double UAVdistance(UAV, UAV);

};


#endif //PROJECT_FOOTBALLFIELD_H
