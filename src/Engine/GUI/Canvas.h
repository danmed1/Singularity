#ifndef CANVAS_H
#define CANVAS_H

class Canvas {
public:
	virtual void drawLine(xdl::xdl_int x1, xdl::xdl_int y1, xdl::xdl_int x2, xdl::xdl_int y2) = 0;
	virtual void drawRect(xdl::xdl_int x1, xdl::xdl_int y1, xdl::xdl_int x2, xdl::xdl_int y2) = 0;
}

#endif