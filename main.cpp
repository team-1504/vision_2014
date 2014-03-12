/* 
 * File:   main.cpp
 * Author: gavaga
 *
 * Created on January 18, 2014, 7:47 PM
 */

#include "identifiers.hpp"
#include "utils.hpp"
#include "localizer.hpp"
#include "constants.hpp"
#include "networktables.hpp"

#include <cstdlib>
#include <string>
#include <sstream>
#include <iostream>

#include <ctime>

#include <boost/date_time/posix_time/posix_time.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;


/*
 * 
 */
int main(int argc, char** argv) 
{
    clock_t program_start_time = clock();
    cv::VideoCapture vid;
    vid.open(argv[1]);
    
    namedWindow("Debug");
    
   
    long time_stamp = 0;

    Mat frame;
    Mat frame_out;
   
    int size = INFORM_PACKET_SIZE;
    
    generic_packet packet_out;
        
    com_module coms = com_module::get_instance("10.15.4.2");

    cout << "Coms initied" << endl;
    
    generic_packet packet_in(4);
    vision_target_observation * hot = new vision_target_observation;
    vector<vision_target_observation> t_obs;
    vector<vision_target> targets;
    
    robot_state current_robot_state;
   

    sleep(1.0);

    while(true)
    {
        time_stamp = (clock() - program_start_time) / ((double)(CLOCKS_PER_SEC / 1000));
  //      cout << "Running main loop" << endl;
        vid.read(frame);
    //    cout << "Frame read" << endl;      
        coms.get_packet(packet_in);
        if (packet_in.size() > 0)
        {
            current_robot_state = generate_robot_delta(packet_in);
        }
        else
        {
            current_robot_state = robot_state();
            current_robot_state.robot_angle = 0;
            current_robot_state.time = 0;
            current_robot_state.x = 0;
            current_robot_state.y = 0;
        }
//	cout << "Packet got" << endl;
        
        t_obs = find_vision_targets(frame, frame_out);
	cout << "Count: " << t_obs.size() << endl;
        if (t_obs.size() > 0)
        {
		packet_out = generic_packet(t_obs.size()*3 + 1);
		packet_out[0] = (int)t_obs.size();

		int j = 0;
		for (int i = 1; i < packet_out.size(); i+=3)
		{
			vision_target t = convert_observation_to_target(t_obs[j++], current_robot_state, &frame_out);
			packet_out[i] = (double)t.angle;
			packet_out[i+1] = (double)t.distance;
			packet_out[i+2] = (int)t.type;
		}
	}

//	cout << "Packet Generated" << endl;

	coms.set_packet(packet_out);

        cv::imshow("Debug", frame_out);
        
        waitKey(30);
    }
}




