#ifndef UI_RENDERER_H
#define UI_RENDERER_H

#include <Engine/GUI/UIBase.h>

namespace gui {

	class UIRenderer {
		public:
			virtual ~UIRenderer() {};
			virtual void draw(UIBase* base) = 0;
	};

}

#endif // UI_RENDERER_H
