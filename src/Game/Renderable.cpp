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

#include "Game/Renderable.h"

namespace soan {

	namespace game {

		Renderable::Renderable() :
			m_model(nullptr),
			m_renderingEnabled(xdl::xdl_false),
			m_castShadow(xdl::xdl_true) {
		}

		Renderable::Renderable(Model* model) : m_model(model), m_renderingEnabled(xdl::xdl_true) {

		}

		Renderable::~Renderable() {
	
		}

		xdl::xdl_int Renderable::init() {

			return xdl::ERR_OK;
		}



		const std::shared_ptr<Model>& Renderable::getModel() {
			assert(isRenderingEnabled() && "Renderable::getModel(): Rendering not enabled");
			return m_model;
		}

		void Renderable::setModel(const std::shared_ptr<Model>& model, xdl::xdl_uint collisionShape) {
			m_model = model;
			m_renderingEnabled = xdl::xdl_true;

//
//			switch(collisionShape) {
//			case 1: {
//
//					btSphereShape* sphereShape = new btSphereShape(m_model->getBoundingBoxX()/2.0);
//					btVector3 localInertia(0,0,0);
//
//					sphereShape->calculateLocalInertia(m_mass, localInertia);
//
//					m_body->setCollisionShape(sphereShape);
//				}
//				break;
//				case 0:
//				default: {
//					m_colShape = new btBoxShape(btVector3(m_model->getBoundingBoxX()/2.0, m_model->getBoundingBoxY()/2.0f, m_model->getBoundingBoxZ()/2.0f));
//					btVector3 localInertia(0,0,0);
//					m_colShape->calculateLocalInertia(m_mass, localInertia);
//					m_body->setCollisionShape(m_colShape);
//				}break;
//			}


		}

		void Renderable::setRenderingEnabled(xdl::xdl_bool state) {
			m_renderingEnabled = state;
		}

		xdl::xdl_bool Renderable::getCastShadow() {
			return m_castShadow;
		}

		void Renderable::setCastShadow(xdl::xdl_bool state) {
			m_castShadow = state;
		}


		void Renderable::reset() {

		}

		bool Renderable::update(xdl::xdl_uint64 timeStep) {
			
			// We have to return false because it did not die yet.
			return false;

		}

		xdl::xdl_bool Renderable::isRenderingEnabled() {
			return m_renderingEnabled;
		}

	}

}
