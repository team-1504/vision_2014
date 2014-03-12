/* 
 * File:   identifiers.hpp
 * Author: gavaga
 *
 * Created on January 19, 2014, 4:04 PM
 */

#ifndef IDENTIFIERS_HPP
#define	IDENTIFIERS_HPP


#include <iostream>
#include <opencv2/core.hpp>

struct robot_bumper_observation
{
    int x, y, height_left, height_right, height_center, theta, team_number;   
};
struct ball_image_observation
{
    int x, y, height;    
};
struct vision_target_observation
{
    int index, x, y, height, width, color, type;
};

int find_goal_lights(const cv::Mat&, cv::Mat&);

std::vector<robot_bumper_observation> find_robots(const cv::Mat&);
std::vector<robot_bumper_observation> find_robots(const cv::Mat&, const bool&);

std::vector<ball_image_observation> find_balls(const cv::Mat&, cv::Mat&);
std::vector<ball_image_observation> find_balls(const cv::Mat&, cv::Mat&, const bool&);

std::vector<vision_target_observation> find_vision_targets(const cv::Mat&, cv::Mat&);
bool get_dynamic_target(const std::vector<vision_target_observation>&, vision_target_observation*);


#endif	/* IDENTIFIERS_HPP */

