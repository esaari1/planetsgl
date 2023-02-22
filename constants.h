// constants.h
//
// Copyright (C) 2009, Eric Saari
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#include <math.h>

// Multiply to convert from degrees to radians
const double DEG_TO_RAD = M_PI / 180.0;

// Multiply to convert from radians to degrees
const double RAD_TO_DEG = 180.0 / M_PI;

// Multiply to convert from astronomical units to kilometers
const double AU_TO_KM = 149598000;

// Number of days in a tropical year
const double TROPICAL_YEAR = 365.242191;

#endif
