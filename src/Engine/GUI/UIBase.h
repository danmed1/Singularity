#ifndef UI_BASE_H
#define UI_BASE_H

#include <XdevLTypes.h>
#include <vector>

namespace gui {

	struct Rect {
		Rect(xdl::xdl_float _x, xdl::xdl_float _y, xdl::xdl_float _width, xdl::xdl_float _height) :
			x(_x), y(_y), width(_width), height(_height) {}

		xdl::xdl_float x;
		xdl::xdl_float y;
		xdl::xdl_float width;
		xdl::xdl_float height;
	};

	class UIBase {
		public:
			UIBase();
			virtual ~UIBase();
			void drawRect(xdl::xdl_float x, xdl::xdl_float y, xdl::xdl_float width, xdl::xdl_float height);
		private:
			std::vector<Rect> m_rectArray;
	};

}

#endif // UI_BASE_H
