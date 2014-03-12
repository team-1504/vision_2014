/* 
 * File:   utils.hpp
 * Author: gavaga
 *
 * Created on January 19, 2014, 5:25 PM
 */

#include <cstdlib>

#ifndef UTILS_HPP
#define	UTILS_HPP

struct robot_state
{
    int time;
    double x, y, robot_angle, camera_relative_angle;
};

struct vision_target
{
    int type, color;
    double distance, angle;
};

template <typename T>
T swap_endian(T u)
{
    union
    {
        T u;
        unsigned char u8[sizeof(T)];
    } source, dest;

    source.u = u;

    for (size_t k = 0; k < sizeof(T); k++)
        dest.u8[k] = source.u8[sizeof(T) - k - 1];

    return dest.u;
}
#endif	/* UTILS_HPP */

