#include "neural_nets.hpp"
#include "gamestate.hpp"

#include <doublefann.h>
#include <fann_cpp.h>

using namespace FANN;

int calc_decision_top(game_state)
{
    neural_net primary_net;
}

int calc_decision_zone(zone_game_state, int)
{
    
}

int calc_decision_zone_white(zone_white_game_state);
int calc_decision_zone_home(zone_home_game_state);
int calc_decision_zone_off(zone_off_game_state);