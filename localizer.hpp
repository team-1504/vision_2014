/* 
 * File:   localizer.hpp
 * Author: gavaga
 *
 * Created on January 19, 2014, 7:41 PM
 */

#ifndef LOCALIZER_HPP
#define	LOCALIZER_HPP

#include "networktables.hpp"
#include "identifiers.hpp"
#include "utils.hpp"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

struct observation
{
    int time;
    int index;
    robot_state delta;
    vision_target target;
};

extern std::vector<observation> observations;
extern robot_state robot_state_int;
extern robot_state robot_state_obs;

void init_localizer();

robot_state get_robot_location();
void update_interpolation(const robot_state&);
void reset_cycle();

bool calc_robot_state();
int add_obs(const vision_target_observation&, const robot_state&);
vision_target adjust_target_observation(const vision_target&, const vision_target&);


vision_target convert_observation_to_target(const vision_target_observation&, const robot_state&, cv::Mat*);
double calc_distance(const vision_target_observation&);
robot_state generate_robot_delta(const generic_packet&);

#endif	/* LOCALIZER_HPP */

