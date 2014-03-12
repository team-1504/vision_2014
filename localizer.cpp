/* 
 * File:   localizer.cpp
 * Author: gavaga
 * 
 * Created on January 19, 2014, 7:41 PM
 */

#include "localizer.hpp"
#include "constants.hpp"

#include <boost/variant/get.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <string>
#include <cmath>

using namespace std;

std::vector<observation> observations;
robot_state robot_state_int;
robot_state robot_state_obs;

void init_localizer()
{
    observations = vector<observation>();
    robot_state_int = robot_state();
    robot_state_obs = robot_state();
}

robot_state get_robot_location()
{
    return robot_state_obs.time >= robot_state_int.time ? robot_state_obs: robot_state_int;
}

void update_interpolation(const robot_state& delta)
{
    robot_state_int.camera_relative_angle = delta.camera_relative_angle;
    robot_state_int.robot_angle = delta.robot_angle;
    robot_state_int.x += delta.x;
    robot_state_int.y += delta.y;
    robot_state_int.time = delta.time;
}

void reset_cycle()
{
    observations.clear();
}

void convert_to_newest_coordinates(int index)
{
    double delta_x = 0;
    double delta_y = 0;
    double delta_theta = ((observation)observations.at(observations.size())).delta.robot_angle - ((observation)observations.at(index)).delta.robot_angle;
    
    for (int i = index + 1; i < observations.size(); ++i)
    {
        delta_x += ((observation)observations[i]).delta.x;
        delta_y += ((observation)observations[i]).delta.y;
    }
    vision_target adj;
    double distance_1 = ((observation)observations[index]).target.distance;
    double theta = ((observation)observations[index]).target.angle;

    double d_r_2 = pow(delta_x,2) + pow(delta_y,2);
    adj.distance = sqrt(d_r_2 + pow(distance_1,2) - 2*distance_1*sqrt(d_r_2)*cos(theta + atan(delta_x/delta_y)));
    adj.angle = 90 - (delta_theta + (asin(distance_1 * sin(theta - atan(delta_y/delta_x)) / adj.distance) - atan((double)delta_y/(double)delta_x)));
    
    observation obs;
    obs.delta = ((observation)observations[index]).delta;
    obs.target = adj;
    
    observations.at(index) = obs;
}

bool calc_robot_state()
{
    int obs_count = observations.size();
    
    vector<vision_target> simultaneous_obs(obs_count);
    
    for (int i = 0; i < obs_count - 1; ++i)
    {
        convert_to_newest_coordinates(i);
    }
    
    vector<vision_target> red_targets (2);
    vector<vision_target> blue_targets (2);
    for (int i = 0; i < simultaneous_obs.size(); ++i)
    {
        if(((vision_target)simultaneous_obs[i]).color == 1)
        {
            red_targets.push_back(simultaneous_obs[i]);
        }
        else if (((vision_target)simultaneous_obs[i]).color == 0)
        {
            blue_targets.push_back(simultaneous_obs[i]);
        }
    }
    
    vector<vision_target> primary_targets(2);
    if (red_targets.size() >= blue_targets.size())
    {
        primary_targets = red_targets;
    }
    else
    {
        primary_targets = blue_targets;
    }
    
    double delta_th = ((vision_target)primary_targets[1]).angle - ((vision_target)primary_targets[0]).angle;
    int primary_index = delta_th > 0? 0: 1;
    
    double y_simple = ((pow(FIELD_WIDTH - 2 * VISION_TARGET_Y, 2) + pow(((vision_target)primary_targets[(primary_index + 1)%2]).distance, 2) - pow(((vision_target)primary_targets[primary_index]).distance, 2)) / 2 * (FIELD_WIDTH - 2 * VISION_TARGET_Y)) + VISION_TARGET_Y;
    double x_simple = sqrt(pow(y_simple - VISION_TARGET_Y, 2) + pow(((vision_target)primary_targets[(primary_index + 1)%2]).distance, 2));
    double theta_simple = ((vision_target)primary_targets[primary_index]).angle + acos(x_simple / ((vision_target)primary_targets[primary_index]).distance);
    
    if (((vision_target)primary_targets[0]).color)
    {
        robot_state_obs.x = FIELD_LENGTH - x_simple;
        robot_state_obs.y = FIELD_WIDTH - y_simple;
        robot_state_obs.robot_angle = ((PI + theta_simple) / (2*PI));
        robot_state_obs.robot_angle = (robot_state_obs.robot_angle - ((double)((int)robot_state_obs.robot_angle))) * robot_state_obs.robot_angle;
    }
    else
    {
        robot_state_obs.x = x_simple;
        robot_state_obs.y = y_simple;
        robot_state_obs.robot_angle = theta_simple;
    }
    
    robot_state_int = robot_state_obs;
    return true;
}

int add_obs(const vision_target_observation& target_obs, const robot_state& delta)
{
    vision_target target = convert_observation_to_target(target_obs, delta, NULL);
    
    observation obs;
    obs.target = target;
    obs.delta = delta;
    
    observations.push_back(obs);
}

vision_target convert_observation_to_target(const vision_target_observation& target_obs, const robot_state& delta, cv::Mat* out)
{
    vision_target target;
    
    target.distance = calc_distance(target_obs);
    
//    target.angle = delta.camera_relative_angle + 0.5*RESOLUTION_X - target_obs.x;
    target.angle = delta.robot_angle + (0.5*RESOLUTION_X - target_obs.x)*RADIANS_PER_PIXEL*(180.0/PI);
    target.type = target_obs.type;
    target.color = target_obs.color;
    if (out != NULL)
    {
        cv::putText(*out, to_string(target.angle), cv::Point(10,15), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 0), 1, 8, false);
        cv::putText(*out, to_string(target.distance), cv::Point(150,15), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 0), 1, 8, false);
    }
    return target;
}

double calc_distance(const vision_target_observation& target_obs)
{
    double phi = (0.5 * RESOLUTION_Y - target_obs.y) * DEGREES_PER_PIXEL;
    return abs(CAMERA_HEIGHT - (target_obs.type == 0?STATIC_TARGET_CENTER_HEIGHT:DYNAMIC_TARGET_CENTER_HEIGHT)) / tan(phi);
}

robot_state generate_robot_delta(const generic_packet& in)
{
    robot_state delta;
    
//    delta.camera_relative_angle = get<double>(in.at(4));
    delta.robot_angle = get<double>(in.at(3));
    delta.x = get<double>(in.at(2));
    delta.y = get<double>(in.at(1));
    
    return delta;
}
