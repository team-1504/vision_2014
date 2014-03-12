/* 
 * File:   pathfinding.hpp
 * Author: gavaga
 *
 * Created on January 23, 2014, 11:26 AM
 */

#ifndef PATHFINDING_HPP
#define	PATHFINDING_HPP

#include <vector>
#include <iostream>
#include <list>
#include <unordered_map>
#include <unordered_set>

class self_state
{
private:
    int x, y, theta;
public:
    inline self_state()
    {
        x = 0;
        y = 0;
        theta = 0;
    }
    inline self_state(int x_p, int y_p, int theta_p)
    {
        x = x_p; 
        y = y_p; 
        theta = theta_p;
    }
    inline self_state(const self_state& other)
    {
        x = other.get_x();
        y = other.get_y();
        theta = other.get_theta();
    }
    inline bool operator== (self_state& other) const
    {
        return other.get_x() == x && other.get_y() == y && other.get_theta() == theta;
    }
    inline bool operator== (const self_state& other) const
    {
        return other.get_x() == x && other.get_y() == y && other.get_theta() == theta;
    }
    inline bool operator!= (self_state& other) const
    {
        return !(*this==other);
    }
    inline bool operator!= (const self_state& other) const
    {
        return !(*this==other);
    }
    inline int get_x() const {return x;}
    inline int get_y() const {return y;}
    inline int get_theta() const {return theta;}
    inline int hash() const
    {
        int hash = 0;
        hash = get_x();
        hash = (hash << 1 | hash >> 31);
        hash ^ get_y();
        hash = (hash << 1 | hash >> 31);
        hash ^ get_theta();
        return hash;        
    }
};

namespace std
{
    template <>
    struct hash<self_state>
    {
        std::size_t operator() (const self_state& s) const
        {
            return (std::size_t)s.hash();
        }
    };
}

struct robot_state
{
    int x, y, theta;
    double vx, vy, omega;
    int time;
};

struct action
{
    double vx, vy, omega;
};

//typedef std::unordered_set<self_state> co_set;
//typedef std::unordered_map<self_state, self_state> path_map;
//typedef std::unordered_map<self_state, double> score_map;

std::list<self_state> a_star_search(double*, self_state, const self_state&);
std::unordered_set<self_state> generate_neighbors(const self_state&);
bool check_valid_state(const self_state&);
double dist_between(const self_state&, const self_state&);

std::list<self_state> reconstruct_path(const std::unordered_map<self_state, self_state>&,const self_state&);
action generate_transition_action(const self_state&, const self_state&);

double null_heuristic(const self_state&);
double a_star_heuristic(const self_state&);


#endif	/* PATHFINDING_HPP */

