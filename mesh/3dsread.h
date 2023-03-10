// 3dsread.h
// 
// Copyright (C) 2000, Chris Laurel <claurel@shatters.net>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

#ifndef _3DSREAD_H_
#define _3DSREAD_H_

#include <iostream>
#include <fstream>
#include <string>
#include "3dsmodel.h"

using namespace std;

M3DScene* Read3DSFile(ifstream& in);
M3DScene* Read3DSFile(const string& filename);

#endif // _3DSREAD_H_
