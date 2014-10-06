#ifndef UI_RENDERER_IMPL_H
#define UI_RENDERER_IMPL_H

#include <Engine/GUI/UIRenderer.h>
namespace gui {

	class UIRendererImpl : public UIRenderer {
		public:
			UIRendererImpl();
			virtual ~UIRendererImpl();
			virtual void draw(UIBase* base);
	};

}

#endif // UI_RENDERER_H
