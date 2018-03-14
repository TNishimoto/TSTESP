/* 
 * Copyright (C) 2018, Takaaki Nishimoto, all rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *   1. Redistributions of source code must retain the above Copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above Copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of the authors nor the names of its contributors
 *      may be used to endorse or promote products derived from this
 *      software without specific prior written permission.
 */

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <fstream>
#include <strstream>
#include <cstdio>
#include <cstdlib>
#include <stdint.h>
#include <time.h>
#include <algorithm>
#include <stdint.h>
//#include "my_function.h"
//#include "zstring.h"
#include <cassert>
#include <set>
#include "tst_tree.hpp"

namespace tst{
	
	
	void computeAllQgrams(string &text, int q, vector<string> result);
	bool test(string& text, int q);
}