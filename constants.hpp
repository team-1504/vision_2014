/* 
 * File:   extern constants.hpp
 * Author: gavaga
 *
 * Created on January 19, 2014, 11:08 PM
 */

#ifndef CONSTANTS_HPP
#define	CONSTANTS_HPP

#define PI 3.141592

#include <string>

extern const double CAMERA_HEIGHT;
extern const double STATIC_TARGET_CENTER_HEIGHT; 
extern const double STATIC_TARGET_TOP_HEIGHT;
extern const double STATIC_TARGET_BOTTOM_HEIGHT;
extern const double DYNAMIC_TARGET_CENTER_HEIGHT; 
extern const double DYNAMIC_TARGET_TOP_HEIGHT;
extern const double DYNAMIC_TARGET_BOTTOM_HEIGHT;

extern const double FIELD_WIDTH;
extern const double FIELD_LENGTH;
extern const double VISION_TARGET_Y;

extern const double RADIANS_PER_PIXEL;
extern const double DEGREES_PER_PIXEL;

extern const double RESOLUTION_X;
extern const double RESOLUTION_Y;

extern const int RED_HUE;
extern const int BLUE_HUE;
extern const int HUE_WIDTH_COLOR;

extern const int BALL_AREA_TOLERANCE;

extern const int VISION_HUE_1;
extern const int HUE_WIDTH_VISION_TARGET_1;
extern const int VISION_HUE_2;
extern const int HUE_WIDTH_VISION_TARGET_2;

extern const double STATIC_TARGET_ASPECT;
extern const double DYNAMIC_TARGET_ASPECT;
extern const double ASPECT_TOLERANCE;

extern const double AREA_DIFFERENTIAL;
extern const double AREA_TOLERANCE;

extern const double RADIUS_DIFFERENTIAL;

extern const double EPSILON;

//Pathfinding
extern const int ACTION_COUNT;
extern const double TIME_STEP;
extern const double MAX_ROBOT_SPEED;

extern const std::string CRIO_ADDRESS;
extern const std::string DS_ADDRESS;


//0 = Boolean, 1 = Integer, 2 = Long, 3 = Double
extern const int NULL_PACKET_FORMAT[];
extern const int NULL_PACKET_ELEMENT_COUNT;
extern const int NULL_PACKET_SIZE;
extern const int INFORM_PACKET_FORMAT[];
extern const int INFORM_PACKET_ELEMENT_COUNT;
extern const int INFORM_PACKET_SIZE;
extern const int CMD_PACKET_FORMAT[];
extern const int CMD_PACKET_ELEMENT_COUNT;
extern const int CMD_PACKET_SIZE;
extern const int CRIO_PACKET_FORMAT[];
extern const int CRIO_PACKET_ELEMENT_COUNT;
extern const int CRIO_PACKET_SIZE;
extern const int INDEXED_TYPE_SIZES[];

#endif	/* CONSTANTS_HPP */

