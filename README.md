# CarND-Path-Planning-Project
Udacity Self-Driving Car Engineer Nanodegree Program, Term 3, Project 1

### Summary
In this project, I utilized a behavior planner and spline function in order to drive a car around a simulated track, including performing lane changes as necessary when around traffic.

The car is required to be able to drive at least 4.32 miles around the track without going over the speed limit, leaving the road, colliding with another car, spending too long between lanes (i.e. not staying in one lane other than to pass), and not exceeding certain acceleration and jerk thresholds.


### Detailed Implementation

#### Trajectory Generation
It heavily relies on the usage of a spline function, which fits a line to reference anchor points. 

Reference anchor points are generated with help of previous path last 2 points and next points in intervals of 30m, 60m and 90m respectively. 

Next interval points are generted using map_s, lane and converting s,d values into x, y coordinates.

It helps in generating a tangent line with previous path and smooth transition from previous trajectory.

If I didn't find any previous path points, then I used vehicle sensor x, y and yaw values to generate previous point. Then, will use sensor values and current as a reference anchor points.

Number of points on trajectory are generated using the formula i.e (target_dist / ((vel_vehicle * .02s)/2.24m/s))

#### Avoid Collision
For avoiding collision, detecting if there is any vehicle ahead of distance less than 30m.
If yes, then I will decrease the velocity, else increase the velocity on every cycle and on each point of trajectory generation.

Velocity of the vehicle maximum is 49.5 and it decreases every cycle by .224m/s and on each point of trajectory (i.e line no. 369) by .112m/s. 
Changing velocity on each point of trajectory helps in changing number of points on trajectory and it helps in smooth transition during velocity change. This also remove any acceleration jerks.

#### Behavior planner
It is very important that vehicle should change lanes to avoid traffic or slow vehicles, but also it should be done only when it is safe to do. Cost of lane will be in the range of 0 to 100m. 
All the code of behavior planning is in class BehaviorPlanner in behavior.cpp file.
I have implemented Cost function (costLane function line no. 51 on behavior.cpp), its objective is to calculate cost of every lane as per front and rear vehicle distance and speed.
Closest vehicle is computed on every lane in function (closestVehicle line no. 86) that provides distance in meters of each vehicle in a lane.
Logistic function (logistic in line no. 42) is used to adjust cost value using exponential value as in the range of 0 to 30.
Also, if front vehicle distance is more than 70m then I have preferred to keep the lane and avoid unnecessary lane changes (line no. 19)

#### Safety

If front vehicle is too close i.e less than 40m in a lane then cost is increased by 60 value to make sure that lane penalty is high.
Also, computing rear vehicle distance to check for blind spots and if distance is less than 20m then increase cost again by 40.
Also, I have make sure that there should not be too much delta on lane changes to avoid lane change jerks, so the lane change should be next one only (line 263 in main.cpp).