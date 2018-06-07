//
//  behavior.cpp
//  path_planning
//
//  Created by ANKIT GROVER on 6/3/18.
//

#include "behavior.hpp"
#include <stdio.h>      /* printf */
#include <math.h>
#include <iostream>

using namespace std;
int BehaviorPlanner::planLane(double car_s, double car_d, vector<vector<double> > sensor_fusion, int prev_path_size) {
    int car_lane = computeLane(car_d);
    int min_cost_lane = car_lane;
    vector<double> lane_ahead_vehicle = closestVehicle(car_s, car_lane, sensor_fusion, true, prev_path_size);
    // Ahead vehicle is above 100m then keep driving on same lane
    if (lane_ahead_vehicle[0] >= 70) {
        // Do nothing as minimum cost lane is car lane by default.
    } else {
        // Iterate over every lane and choose less cost lane.
        double min_cost = std::numeric_limits<double>::max();
        for (unsigned int i=0; i < TOTAL_LANES; ++i) {
            double cost = costLane(car_s, car_lane, i, sensor_fusion, prev_path_size);
            cout << "Cost is " << cost << "of lane " << i << endl;
            if (cost < min_cost) {
                min_cost = cost;
                min_cost_lane = i;
                cout << "Minimum cost lane is" << i << endl;
            }
        }
    }
    return min_cost_lane;
}

int BehaviorPlanner::computeLane(double d) {
    if (d < 4) {
        return 0;
    } else if (d < 8) {
        return 1;
    }
    return 2;
}

// Value of logistic funtion is between 0 to 30.00
double logistic(int value) {
    return 30 - (30 *( 2/ (1 + exp(-value)) - 1.0));
}

double BehaviorPlanner::costLane(double car_s, int car_lane, int lane, vector<vector<double> > sensor_fusion, int prev_path_size) {
    // Penalized if lane is different to current lane
    double cost_lane = 0;
    if (lane != car_lane) {
        cost_lane += 5.0;
    }
    vector<double> front_vehicle = closestVehicle(car_s, lane, sensor_fusion, true,  prev_path_size);
    vector<double> rear_vehicle = closestVehicle(car_s, lane, sensor_fusion, false,  prev_path_size);
    if (front_vehicle.size() > 0) {
        if (front_vehicle[0] < 40) {
            cost_lane += 60;
            cout << "Front vehicle is close and Lane  " << lane << ",dist is " << front_vehicle[0] << endl;
        } else {
            // Penalize as per distance
            cost_lane += logistic(front_vehicle[0]);
            // Penalize as per speed
            cost_lane += (logistic(front_vehicle[1]) / 3);
            cout << "Front vehicle on Lane " << lane << "and dist is " << front_vehicle[0] << endl;
        }
    }
    if (rear_vehicle.size() > 0) {
        if (rear_vehicle[0] < 20) {
            cost_lane += 40;
        } else {
            // Penalize as per distance
            cost_lane += (logistic(rear_vehicle[0]/4) /4);
            // Penalize as per speed
            //cost_lane += logistic(rear_vehicle[1]/5);
            cout << "Rear vehicle on Lane " << lane << "and dist is " << rear_vehicle[0] << endl;
        }
    }
    // Too close, high penalty
    return cost_lane;
}

vector<double> BehaviorPlanner::closestVehicle(double car_s, int desired_lane, vector<vector<double> > sensor_fusion, bool direction, int prev_path_size) {
    vector<double> vehicle_info;
    double max_dist = 10000.00;
    double last_min_s = std::numeric_limits<double>::max();
    double speed_vehicle = 0;
    for (unsigned int i =0; i < sensor_fusion.size(); ++i) {
        float d = sensor_fusion[i][6];
        int lane = computeLane(d);
        if (desired_lane == lane) {
            float vx = sensor_fusion[i][3];
            float vy = sensor_fusion[i][4];
            double check_car_s = sensor_fusion[i][5];
            double check_speed = sqrt(vx*vx + vy*vy);
            check_car_s += (double) prev_path_size * .02 * check_speed;
            double diff = max_dist;
            if (direction) {
                // Ahead vehicle
                diff = fabs(check_car_s - car_s);
                if (diff < last_min_s) {
                    last_min_s = diff;
                    speed_vehicle = check_speed;
                }
            } else {
                // Behind vehicle
                diff = fabs(car_s - check_car_s);
                if (diff < last_min_s) {
                    last_min_s = diff;
                    speed_vehicle = check_speed;
                }
                
            }
            // Return s and speed of the vehicle
        }
        
    }
    if (last_min_s <= max_dist) {
        cout << "Closest vehicle on Lane " << desired_lane << "and dist is " << last_min_s << endl;
        vehicle_info.push_back(last_min_s);
        vehicle_info.push_back(speed_vehicle);
    }
    return vehicle_info;
}
