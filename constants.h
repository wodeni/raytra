/*
 * constants.h
 *
 *  Created on: Nov 7, 2016
 *      Author: niw
 */
#include <limits>

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#define SHADOW_RAY 0
#define REGULAR_RAY 1
#define REFLECT_RAY 2

#define NORMAL_MODE 11
#define SLOW_MODE 22
#define BBOX_ONLY_MODE 33

#define VERBOSE 1
#define REGION_X 284
#define REGION_Y 322
#define STEP_NUM 0.0001

extern int mode;
extern int CAMSAMPLES;
extern int SHADOWSAMPLES;
static const double DOUBLE_MAX = std::numeric_limits<double>::max();
static const double DOUBLE_MIN = std::numeric_limits<double>::min();

#endif /* CONSTANTS_H_ */
