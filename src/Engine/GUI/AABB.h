/*
	Copyright (c) 2015 Cengiz Terzibas

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.
	
	cengiz@terzibas.de
*/

#ifndef AABB_H
#define AABB_H

/**
 * @class AABB
 * @brief Axis Aligned Bounding Box
 */
class AABB {
public:
	AABB() : x1(0), y1(0), x2(0), y2(0) {}

	AABB(xdl::xdl_int _x1, xdl::xdl_int _y1, xdl::xdl_int _x2, xdl::xdl_int _y2) : x1(_x1), y1(_y1), x2(_x2), y2(_y2) {}
	
	/// Check if the specified x,y coordinates lies inside the boundaries.
	xdl::xdl_bool isPointInside(xdl::xdl_int x, xdl::xdl_int y) {
		return ((x1 <= x) && (x <= x2) && (y1 <= y) && (y <= y2));
	}
	
	/// Returns the width (x axis) of this AABB.
	xdl::xdl_int getWidth() const {
		return (x2-x1);
	}
	
	/// Returns the height (y axis) of this AABB.
	xdl::xdl_int getHeight() const {
		return (y2-y1);
	}
	xdl::xdl_int x1, y1, x2, y2;
};


#endif
