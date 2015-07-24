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

#ifndef NODE_H
#define NODE_H

template<typename T, typename OBJ>
class XdevLQuadTree;

template<typename T, typename OBJ>
struct XdevLQuadTreeNode {
		friend class XdevLQuadTree<T, OBJ>;
		typedef std::list<OBJ> NodeItemVectorType;

		XdevLQuadTreeNode() :
			top_left(nullptr),
			top_right(nullptr),
			bottom_left(nullptr),
			bottom_right(nullptr),
			hasItems(xdl::xdl_false) {}

		XdevLQuadTreeNode(const AABB& _aabb) :
			aabb(_aabb),
			top_left(nullptr),
			top_right(nullptr),
			bottom_left(nullptr),
			bottom_right(nullptr),
			hasItems(xdl::xdl_false)
		{}

		/// Returns the stored user item of this node.
		const OBJ getFirstItem() const {
			return *items.begin();
		}

		NodeItemVectorType& getItems() {
			return items;
		}

		xdl::xdl_uint getNumberOfItems() {
			return items.size();
		}

		/// Sets the user item for this node.
		void addItem(const OBJ& itm) {
			items.push_back(itm);
		}

		const AABB& getAABB() const {
			return aabb;
		}

		void setAABB(const AABB& _aabb) {
			aabb = _aabb;
		}

		const bool& anyItems() const {
			return hasItems;
		}

		void anyItems(bool state) {
			hasItems = state;
		}

		void split() {
			top_left		= new XdevLQuadTreeNode<T, OBJ>(AABB(aabb.x1, aabb.y1, aabb.x1 + (aabb.x2-aabb.x1)/2, aabb.y1 +(aabb.y2-aabb.y1)/2));
			top_right		= new XdevLQuadTreeNode<T, OBJ>(AABB(aabb.x1 + (aabb.x2-aabb.x1)/2 , aabb.y1, aabb.x2, aabb.y1 + (aabb.y2-aabb.y1)/2));
			bottom_left		= new XdevLQuadTreeNode<T, OBJ>(AABB(aabb.x1, aabb.y1 + (aabb.y2-aabb.y1)/2, aabb.x1 + (aabb.x2-aabb.x1)/2, aabb.y2));
			bottom_right	= new XdevLQuadTreeNode<T, OBJ>(AABB(aabb.x1 + (aabb.x2-aabb.x1)/2, aabb.y1 + (aabb.y2-aabb.y1)/2, aabb.x2, aabb.y2));
		}

		xdl::xdl_bool hasChildren() {
			return (nullptr != top_left) && (nullptr != top_right) && (nullptr != bottom_left) && (nullptr != bottom_right);
		}
	private:
		AABB aabb;

		XdevLQuadTreeNode* top_left;
		XdevLQuadTreeNode* top_right;
		XdevLQuadTreeNode* bottom_left;
		XdevLQuadTreeNode* bottom_right;

		xdl::xdl_bool hasItems;

		// Holds an user defined object.
		NodeItemVectorType items;
};


#endif
