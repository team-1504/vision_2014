/* 
 * File:   decision_tree.hpp
 * Author: gavaga
 *
 * Created on January 29, 2014, 2:53 PM
 */

#ifndef DECISION_TREE_HPP
#define	DECISION_TREE_HPP

#include "gamestate.hpp"

#include <doublefann.h>
#include <fann_cpp.h>

int calc_decision_top(game_state);

int calc_decision_zone(zone_game_state, int);

int calc_decision_zone_white(zone_white_game_state);
int calc_decision_zone_home(zone_home_game_state);
int calc_decision_zone_off(zone_off_game_state);

#endif	/* DECISION_TREE_HPP */

