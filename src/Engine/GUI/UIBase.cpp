#include "UIBase.h"

namespace gui {

	UIBase::UIBase() {
	}

	UIBase::~UIBase() {
	}

	void UIBase::drawRect(xdl::xdl_float x, xdl::xdl_float y, xdl::xdl_float width, xdl::xdl_float height) {
		m_rectArray.push_back(Rect(x,y,width,height));
	}
}
