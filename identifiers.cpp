#include "identifiers.hpp"
#include "constants.hpp"
#include "utils.hpp"

#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <string>

#include <ctime>

#include <boost/format.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

vector<robot_bumper_observation> find_robots(const Mat& frame)
{
    int robot_count = 0;
    vector<robot_bumper_observation> robots(robot_count);
    return robots;
}
vector<robot_bumper_observation> find_robots(const Mat& frame, const bool& color)
{
    int robot_count = 0;
    vector<robot_bumper_observation> robots(robot_count);
    return robots;
}

vector<ball_image_observation> find_balls(const Mat& frame, Mat& return_frame)
{
    vector<ball_image_observation> red = find_balls(frame, return_frame, true);
    vector<ball_image_observation> blue = find_balls(frame, return_frame, false);
    
    vector<ball_image_observation> balls(red.size() + blue.size());
    
    for (int i = 0; i < balls.capacity(); ++i)
    {
        if (i < red.size())
        {
            balls[i] = red[i];
        }
        else
        {
            balls[i] = blue[i - red.size()];
        }
    }
    return balls;
}

vector<ball_image_observation> find_balls(const Mat& frame, Mat& return_frame, const bool& color)
{
    //TODO: stuff
}

/**
 * 
 * Author: gavaga 
 * Date: 1/18/2014
 * 
 **********************************
 * Algorithm:
 * 
 *  1. Convert BGR Mat to HSV Mat.
 *  2. Split HSV image into 3 discrete channels.
 *  3. Create mask from an adaptive threshold on H, value based on an
 *     observed image of illuminated vision target.
 *  4. Create second mask on Saturation channel, low adaptive/binary threshold
 *  5. Create mask on Value channel, high inverse adaptive/binary threshold
 *  6. Add masks to get final imagejava call already running method from thread.
 *  7. Extract features.
 *  8. Compare total intensity of each feature to a threshold calculated from the
 *     area of the feature's bounding box.
 *  9. Remove non-rectangular features.
 * 10. Classify all remaining features as dynamic or static vision targets based
 *     on aspect ratios. (Wide = dynamic, tall = static)
 * 11. Determine color=side of targets observed based on red/blue thresholds on H,
 *     then a sum of intensities.
 * 
 * @param frame = cv::Mat that represents the current frame
 * @param frame_out = cv::Mat to return a debug frame;
 * @return vector<vision_target_observation> that lists all vision targets visible in frame, classified by static vs. dynamic
 */
vector<vision_target_observation> find_vision_targets(const Mat& frame, Mat& frame_out)
{
    clock_t t;
    t = clock();
    
    Mat hsv_frame = frame.clone();
    cvtColor(hsv_frame, hsv_frame, COLOR_BGR2HSV);
    
    frame_out = frame.clone();
    
    GaussianBlur(hsv_frame, hsv_frame, Size(5, 5), 0, 0);
    
    //TODO: Maybe switch to using cv::mixChannels();
    Mat hue, sat, val;
    vector<Mat> hsv(3);
    split(hsv_frame, hsv);
    hue = hsv[0];
    sat = hsv[1];
    val = hsv[2];
    
    Mat hue_2 = hue.clone();
    Mat sat_2 = sat.clone();
    Mat val_2 = val.clone();
    
    inRange(hue, Scalar(VISION_HUE_1 - HUE_WIDTH_VISION_TARGET_1), Scalar(VISION_HUE_1 + HUE_WIDTH_VISION_TARGET_1), hue);

    inRange(hue_2, Scalar(VISION_HUE_2 - HUE_WIDTH_VISION_TARGET_2), Scalar(VISION_HUE_2 + HUE_WIDTH_VISION_TARGET_2), hue_2);
    Mat inverter(hue_2.rows, hue_2.cols, hue_2.type(), Scalar(255,255,255));
    subtract(inverter, hue_2, hue_2);
    
    threshold(sat, sat, 30, 255, THRESH_BINARY);
    threshold(val, val, 220, 255, THRESH_BINARY);
    
    threshold(sat_2, sat_2, 80, 255, THRESH_BINARY_INV);
    threshold(val_2, val_2, 220, 255, THRESH_BINARY);
    
    //create final binary image from the three masks; should leave only relevant features
    bitwise_or(hue, hue_2, hue);
    bitwise_or(sat, sat_2, sat);
    bitwise_or(val, val_2, val);
    
    bitwise_and(hue, sat, hue);
    bitwise_and(hue, val, hue);
    
//    frame_out = hue.clone();
    
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    
    findContours(hue, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0,0));
//    drawContours(frame_out, contours, -1, Scalar(rand()&255, rand()&255, rand()&255), 2, 8, hierarchy);
    
    vector<RotatedRect> target_rects(contours.size());
    
    uchar* p;
    
    double max_area = 0;
    for (int i = 0; i < contours.size(); ++i)
    {
        max_area = max(max_area, contourArea(contours[i]));
    }
    
    for (int index = 0; index < contours.size(); ++index)
    {
        RotatedRect r = minAreaRect(Mat(contours[index]));
        double area = contourArea(contours[index]);
         
        if ( abs(1 - (area / (r.size.height * r.size.width))) < AREA_TOLERANCE && area / max_area > AREA_DIFFERENTIAL)
        {
            target_rects[index] = r;
        }
    }
    target_rects.shrink_to_fit();
    
    vector<vision_target_observation> targets(target_rects.size());
    for (int i = 0; i < target_rects.size(); ++i)
    {
        double aspect = ((double)((RotatedRect)target_rects[i]).size.width) / ((double)((RotatedRect)target_rects[i]).size.height);
        bool is_static = abs(aspect - STATIC_TARGET_ASPECT) < ASPECT_TOLERANCE;
        bool is_dynamic = abs(aspect - DYNAMIC_TARGET_ASPECT) < ASPECT_TOLERANCE;
        if (is_static || is_dynamic && ((RotatedRect)target_rects[i]).center.x > 5 && ((RotatedRect)target_rects[i]).center.y > 5 )
        {
            vision_target_observation back = {};
            back.x = ((RotatedRect)target_rects[i]).center.x;
            back.y = ((RotatedRect)target_rects[i]).center.y;
            back.height = ((RotatedRect)target_rects[i]).size.height;
            back.width = ((RotatedRect)target_rects[i]).size.width;
            back.index = i;
            back.type = is_static? 0: 1;
            targets[i] = back;
        }
        else
        {
            continue;
        }               
    }
    
    for (int i = 0; i < targets.size(); ++i)
    {
        Scalar color(rand()&255, rand()&255, rand()&255);
//        circle(frame_out, Point(((RotatedRect)target_rects[targets[i].index]).center.x, ((RotatedRect)target_rects[targets[i].index]).center.y), 5, color, -1, 8, 0);
        Point2f rect_points[4];
        target_rects[targets[i].index].points(rect_points);
        for (int j = 0; j < 4; ++j)
        {
            line(frame_out, rect_points[j], rect_points[(j+1)%4], color, 2, 8);
        }
	string text = targets[i].type == 0? str(boost::format("Static %1%") % i): str(boost::format("Dynamic %1%") % i);
        cv::putText(frame_out, text, Point(((RotatedRect)target_rects[targets[i].index]).center.x - (((RotatedRect)target_rects[targets[i].index]).size.width / 4), ((RotatedRect)target_rects[targets[i].index]).center.y), FONT_HERSHEY_SIMPLEX, 0.5, color, 1, 8, false);
    }
    
    
    
    t = clock() - t;
    
//    cout << "IMG Parse time: " << ((float)t)/CLOCKS_PER_SEC << endl;
    
    return targets;
}

bool get_dynamic_target(const vector<vision_target_observation>& targets, vision_target_observation *dynamic)
{
    for (vision_target_observation target: targets)
    {
        if (target.type == 1)
        {
            *dynamic = target;
            return true;
        }
    }
    return false;
}
