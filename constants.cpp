#include "constants.hpp"

#include <string>

const double CAMERA_HEIGHT = 50;
const double STATIC_TARGET_CENTER_HEIGHT = 40; 
const double STATIC_TARGET_TOP_HEIGHT = 56;
const double STATIC_TARGET_BOTTOM_HEIGHT = 24;
const double DYNAMIC_TARGET_CENTER_HEIGHT = 40; 
const double DYNAMIC_TARGET_TOP_HEIGHT = 56;
const double DYNAMIC_TARGET_BOTTOM_HEIGHT = 24;

const double FIELD_WIDTH = 324;
const double FIELD_LENGTH = 648;
const double VISION_TARGET_Y = 29;

const double RADIANS_PER_PIXEL = 3.3780566e-4;
const double DEGREES_PER_PIXEL = 0.0193548387;

const double RESOLUTION_X = 1240;
const double RESOLUTION_Y = 720;

const int RED_HUE = 0;
const int BLUE_HUE = 111;
const int HUE_WIDTH_COLOR = 69;

const int BALL_AREA_TOLERANCE = 0.1;

const int VISION_HUE_1 = 72;
const int HUE_WIDTH_VISION_TARGET_1 = 15;
const int VISION_HUE_2 = 55;
const int HUE_WIDTH_VISION_TARGET_2 = 30;

const double STATIC_TARGET_ASPECT = 0.125;
const double DYNAMIC_TARGET_ASPECT = 0.5;
const double ASPECT_TOLERANCE = 0.1;

const double AREA_DIFFERENTIAL = 0.3;
const double AREA_TOLERANCE = 0.15;

const double RADIUS_DIFFERENTIAL = 0.3;

const double EPSILON = 0.1;

//Pathfinding
const int ACTION_COUNT = 8;
const double TIME_STEP = 1;
const double MAX_ROBOT_SPEED = 1;

const std::string CRIO_ADDRESS = "10.15.4.2";
const std::string DS_ADDRESS = "10.15.4.5";


//0 = Boolean, 1 = Integer, 2 = Long, 3 = Double
const int NULL_PACKET_FORMAT[] = {2, 1, 0, 1, 2, 3};
const int NULL_PACKET_ELEMENT_COUNT = 6;
const int NULL_PACKET_SIZE = 33;
const int INFORM_PACKET_FORMAT[] = {2, 1, 0, 3, 3};
const int INFORM_PACKET_ELEMENT_COUNT = 5;
const int INFORM_PACKET_SIZE = 29;
const int CMD_PACKET_FORMAT[] = {2, 1, 3, 3, 3, 3, 3, 3, 1, 1, 0, 0, 0};
const int CMD_PACKET_ELEMENT_COUNT = 13;
const int CMD_PACKET_SIZE = 71;
const int CRIO_PACKET_FORMAT[] = {2, 3, 3, 3};
const int CRIO_PACKET_ELEMENT_COUNT = 4;
const int CRIO_PACKET_SIZE = 32;
const int INDEXED_TYPE_SIZES[] = {1, 4, 8, 8};
