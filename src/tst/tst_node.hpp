/*
* Copyright (c) 2018 Takaaki Nishimoto
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published bytes
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
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
using namespace std;
namespace tst{
    using istring = vector<uint8_t>;
	using ichar = uint8_t;
	using ochar = uint64_t;
	using NodeIndex = std::pair<uint64_t, bool>;
	// The first value is an index in internal node vector or leaves vector.
	// If the second bool is false, this index is in internal nodes vector.
	// otherwise, this index is in leaves vector.
	using InsertResult = std::pair<bool, uint64_t>;

	struct TSTNode
	{

	public:

		uint64_t parent;
		uint64_t textPosition;
		uint16_t edgeLength;
		//std::vector<std::pair<uint64_t, bool>> children;

		static const uint64_t BOTTOM = UINT64_MAX - 1;
		static const uint64_t EMPTY = UINT64_MAX;

		TSTNode() : parent(EMPTY), textPosition(UINT64_MAX), edgeLength(UINT16_MAX)
		{
			//children.clear();
		}
		TSTNode(uint64_t _parent, uint64_t _textPos, uint64_t _textLength) : parent(_parent), textPosition(_textPos), edgeLength(_textLength)
		{
			//children.clear();			
		}
		ichar getLabel(istring& text) {
			return text[this->textPosition];
		}
		bool isRoot(){
			return this->parent == UINT64_MAX;
		}
		
		
	};

}