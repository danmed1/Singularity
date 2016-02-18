/*
	Copyright (c) 2016 Cengiz Terzibas

	Permission is hereby granted, free of charge, to any person obtaining a copy of
	this software and associated documentation files (the "Software"), to deal in the
	Software without restriction, including without limitation the rights to use, copy,
	modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
	and to permit persons to whom the Software is furnished to do so, subject to the
	following conditions:

	The above copyright notice and this permission notice shall be included in all copies
	or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
	INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
	PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
	FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
	OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
	DEALINGS IN THE SOFTWARE.

	cengiz@terzibas.de
*/

#ifndef PROCEDURAL_SYSTEM_H
#define PROCEDURAL_SYSTEM_H

#include <XdevLRAI/XdevLRAI.h>
#include <Engine/Entity/Mesh.h>

namespace soan {

	class ProceduralSystem {
		public:
			ProceduralSystem(xdl::IPXdevLRAI rai);

			struct ProceduralSystemGridVertex {
				xdl::xdl_float x,y,z;
				xdl::xdl_float r,g,b,a;
				xdl::xdl_float nx, ny, nz;
				xdl::xdl_float u, v;
			};


			std::shared_ptr<soan::Mesh> createGrid(xdl::xdl_float size, xdl::xdl_float resolution);

		private:

			xdl::IPXdevLRAI m_rai;
	};
}

#endif
