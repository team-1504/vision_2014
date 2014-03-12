/* 
 * File:   gamestate.hpp
 * Author: gavaga
 *
 * Created on January 28, 2014, 8:02 PM
 */

#ifndef GAMESTATE_HPP
#define	GAMESTATE_HPP

#include "utils.hpp"

struct game_state
{
    int robot_row;
    int robot_col;
    int ally_zone_1;
    int ally_zone_2;
    int opponent_zone_1;
    int opponent_zone_2;
    int opponent_zone_3;
    int alliance_ball_zone;
    bool alliance_ball_possession;
    int opponent_ball_zone;
    bool opponent_ball_possession;
    int cycle_time;               //in milliseconds
    bool ball_scored;
    bool trussed;
    bool has_possessed;
};

struct zone_home_game_state
{
    
};

struct zone_white_game_state
{
    
};

struct zone_off_game_state
{
    
};

struct zone_game_state
{
    union
    {
        zone_home_game_state home_zone;
        zone_white_game_state white_zone;
        zone_off_game_state off_zone;
    };
};
#endif	/* GAMESTATE_HPP */

