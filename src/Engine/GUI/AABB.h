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

	xdl::xdl_bool isPointInside(xdl::xdl_int x, xdl::xdl_int y) {
		return ((x1 <= x) && (x <= x2) && (y1 <= y) && (y <= y2));
	}
	xdl::xdl_int getWidth() const {
		return (x2-x1);
	}
	xdl::xdl_int getHeight() const {
		return (y2-y1);
	}
	xdl::xdl_int x1, y1, x2, y2;
};


#endif
