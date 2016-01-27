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

#include "Material.h"

namespace soan {

	Material::Material() :
		m_roughness(1.0f),
		m_skyBoxTexture(nullptr),
		m_useDiffuseConst(xdl::xdl_true),
		m_useNormalConst(xdl::xdl_true),
		m_useSpecularConst(xdl::xdl_true),
		m_useRoughnessConst(xdl::xdl_true),
		m_useDiffuseMap(xdl::xdl_false),
		m_useNormalMap(xdl::xdl_false),
		m_useDisplacementMap(xdl::xdl_false),
		m_useSpecularMap(xdl::xdl_false),
		m_useRoughnessMap(xdl::xdl_false) {

		memset(m_textures, 0, sizeof(xdl::IPXdevLTexture)*7);

	}
	Material*  Material::refCopy() {
		Material* nm = new Material();
		*nm = *this;
		return nm;
	}

	unsigned int Material::getNumTextures() {
		return 2;
	}

	xdl::xdl_float* Material::getAmbient() {
		return ambient;
	}

	xdl::xdl_float* Material::getDiffuse() {
		return diffuse;
	}

	xdl::xdl_float* Material::getSpecular() {
		return specular;
	}

	xdl::xdl_float Material::getRoughness() {
		return m_roughness;
	}

	xdl::XdevLTexture* Material::getTexture(xdl::xdl_uint idx) {
		return m_textures[idx].get();
	}

	xdl::IPXdevLTexture Material::getTextureRef(xdl::xdl_uint idx) {
		return m_textures[idx];
	}

	xdl::XdevLTextureCube* Material::getSkyBoxTexture() {
		return m_skyBoxTexture.get();
	}

	xdl::IPXdevLTextureCube Material::getSkyBoxTextureRef() {
		return m_skyBoxTexture;
	}

	xdl::xdl_bool Material::getUseDiffuseMap() {
		return m_useDiffuseMap;
	}

	xdl::xdl_bool Material::getUseNormalMap() {
		return m_useNormalMap;
	}

	xdl::xdl_bool  Material::getUseDisplacementMap() {
		return m_useDisplacementMap;
	}

	xdl::xdl_bool Material::getUseSpecularMap() {
		return m_useSpecularMap;
	}

	xdl::xdl_bool Material::getUseRoughnessMap() {
		return m_useRoughnessMap;
	}

	xdl::xdl_uint Material::getStateMask() {
		xdl::xdl_uint state = 0;
		if(m_useDiffuseConst) {
			state |= 1;
		}
		if(m_useNormalConst) {
			state |= 2;
		}
		if(m_useSpecularConst) {
			state |= 4;
		}
		if(m_useRoughnessConst) {
			state |= 8;
		}
		if(m_useDiffuseMap) {
			state |= 16;
		}
		if(m_useNormalMap) {
			state |= 32;
		}
		if(m_useDisplacementMap) {
			state |= 64;
		}
		if(m_useSpecularMap) {
			state |= 128;
		}
		if(m_useRoughnessMap) {
			state |= 256;
		}
		return state;
	}

	void Material::setUseDiffuseConst(xdl::xdl_bool state) {
		m_useDiffuseConst = state;
	}

	void Material::setUseNormalConst(xdl::xdl_bool state) {
		m_useNormalConst = state;
	}

	void Material::setUseSpecularConst(xdl::xdl_bool state) {
		m_useSpecularConst = state;
	}

	void Material::setRoughnessConst(xdl::xdl_bool state) {
		m_useRoughnessConst = state;
	}

	void Material::setUseDiffuseMap(xdl::xdl_bool state) {
		m_useDiffuseMap = state;
	}

	void Material::setUseNormalMap(xdl::xdl_bool state) {
		m_useNormalMap = state;
	}

	void  Material::setUseDisplacementMap(xdl::xdl_bool state) {
		m_useDisplacementMap = state;
	}

	void Material::setUseSpecularMap(xdl::xdl_bool state) {
		m_useSpecularMap = state;
	}

	void Material::setRoughnessMap(xdl::xdl_bool state) {
		m_useRoughnessMap = state;
	}

	void Material::setAmbient(xdl::xdl_float r, xdl::xdl_float g, xdl::xdl_float b, xdl::xdl_float a) {
		ambient[0] = r;
		ambient[1] = g;
		ambient[2] = b;
		ambient[3] = a;
	}

	void Material::setDiffuse(xdl::xdl_float r, xdl::xdl_float g, xdl::xdl_float b, xdl::xdl_float a) {
		diffuse[0] = r;
		diffuse[1] = g;
		diffuse[2] = b;
		diffuse[3] = a;
	}

	void Material::setSpecular(xdl::xdl_float r, xdl::xdl_float g, xdl::xdl_float b, xdl::xdl_float a) {
		specular[0] = r;
		specular[1] = g;
		specular[2] = b;
		specular[3] = a;
	}


	void Material::setRoughness(xdl::xdl_float roughness) {
		m_roughness = roughness;
	}

	void Material::setTexture(MaterialTextures texture_index, xdl::IPXdevLTexture texture) {
		m_textures[texture_index] = texture;
	}

	void Material::setTexture(xdl::IPXdevLTextureCube skyBoxTexture) {
		m_skyBoxTexture = skyBoxTexture;
	}
}
