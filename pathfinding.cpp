#include "pathfinding.hpp"
#include "constants.hpp"
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <vector>
#include <cmath>

using namespace std;

list<self_state> a_star_search(double (*heuristic)(self_state, self_state), self_state& initial_state, const self_state& goal)
{
    unordered_set<self_state> closed_set;
    unordered_set<self_state> open_set;
    unordered_map<self_state, self_state> previous_states;
    unordered_map<self_state, double> g_scores;
    unordered_map<self_state, double> f_scores;
    
    g_scores.insert(make_pair<self_state, double>((self_state)initial_state, 0.0));
    f_scores.insert(make_pair<self_state, double>((self_state)initial_state, (*heuristic)(initial_state, goal)));    
    
    while (open_set.size() != 0)
    {
        unordered_set<self_state>::iterator current = open_set.begin();
        for (unordered_set<self_state>::iterator iterator = open_set.begin(); iterator != open_set.end(); ++iterator)
        {
            if (f_scores[*iterator] < f_scores[*current])
            {
                current = iterator;
            }
        }
        if (*current==goal)
        {
            return reconstruct_path(previous_states, goal);
        }
        
        open_set.erase(*current);
        closed_set.insert(*current);
        unordered_set<self_state> neighbors = generate_neighbors(*current);
        for (auto const& neighbor: neighbors)
        {            
            if (closed_set.count(neighbor) > 0)
            {
                continue;
            }
            
            double tentative_g_score = g_scores[*current] + dist_between(*current, neighbor);
            bool in_open_set = open_set.count((self_state)neighbor) > 0;
            if (!in_open_set || tentative_g_score < g_scores[neighbor])
            {
                previous_states.insert(make_pair<self_state, self_state>((self_state)(neighbor), (self_state)(*current)));
                g_scores.insert(make_pair((self_state)neighbor, tentative_g_score));
                f_scores.insert(make_pair((self_state)neighbor, g_scores[neighbor] + (*heuristic)(neighbor, goal)));
                if (!in_open_set)
                {
                    open_set.insert(neighbor);
                }
            }
        }       
    }
}

list<self_state> reconstruct_path(const unordered_map<self_state, self_state>& previous, const self_state& current)
{
    list<self_state> path = {};
    if (previous.count(current) > 0)
    {
        path = reconstruct_path(previous, const_cast<unordered_map<self_state, self_state> &>(previous)[current]);
    }
    path.push_back(current);
    return path;
}

unordered_set<self_state> generate_neighbors(const self_state& current)
{
    unordered_set<self_state> neighbors;
    
    int dx, dy;
    double theta;
    
    
    for (int i = 0; i < ACTION_COUNT; ++i)
    {
        theta = ((double)i)*2.0*PI*(1 / (double)ACTION_COUNT);
        dx = (int) (MAX_ROBOT_SPEED * TIME_STEP * cos(theta));
        dy = (int) (MAX_ROBOT_SPEED * TIME_STEP * sin(theta));
        
        self_state neighbor = {current.get_x() + dx, current.get_y() + dy, current.get_theta()};
        
        if (check_valid_state(neighbor))
        {
            neighbors.insert(neighbor);
        }
    }
    return neighbors;
}


bool check_valid_state(const self_state& state)
{
    return true;
}

double dist_between(const self_state& state_1, const self_state& state_2)
{
    return sqrt(pow(state_2.get_x() - state_1.get_x(), 2) + pow(state_2.get_y() - state_1.get_y(), 2));
}