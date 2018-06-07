//
//  behavior.hpp
//  path_planning
//
//  Created by ANKIT GROVER on 6/3/18.
//

#ifndef behavior_hpp
#define behavior_hpp

#include <stdio.h>
#include <vector>
#include <string>

using namespace std;

class BehaviorPlanner {
public:
    
    int TOTAL_LANES = 3;
    int computeLane(double car_d);
    // Will decide which lane to choose, left, right and keep in current lane as per cost functions.
    int planLane(double car_s, double car_d, vector<vector<double>> sensor_fusion, int prev_path_size);
    
    // Calculate cost of a lane that will average out all the other cost functions
    // Cost of a lane will be between 1 to 100, higher value means not safe.
    double costLane(double car_s, int car_lane, int lane, vector<vector<double> > sensor_fusion, int prev_path_size);
    
    // Compute closest vehicle around our car in a given lane as per direction (i.e ahead or behind of our vehicle).
    vector<double> closestVehicle(double car_s, int desired_lane, vector<vector<double>> sensor_fusion, bool direction, int prev_path_size);
    
};

#endif /* behavior_hpp */
