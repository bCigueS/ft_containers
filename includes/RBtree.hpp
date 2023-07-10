/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RBtree.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbeylot <sbeylot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 13:55:19 by sbeylot           #+#    #+#             */
/*   Updated: 2023/03/06 10:20:39 by sbeylot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RBTREE_HPP
#define RBTREE_HPP


#include <iostream>
#include "utility.hpp"
#include <memory>
#include "stack.hpp"

template <class Key, class T, class Compare = std::less<Key>, class Allocator = std::allocator<ft::pair<const Key, T> > >
class RBtree {

	public:
	typedef Key												key_type;
	typedef T												mapped_type;
	typedef Compare											key_compare;
	typedef typename ft::pair<const key_type, mapped_type>	value_type;

	protected:
	struct Node {
		value_type	value;
		bool		black;
		Node*		parent;
		Node*		left;
		Node*		right;
		bool		isNil;
		bool		isHeader;

		Node(const value_type& data = value_type()) : value(data), black(false), parent(0), left(0), right(0), isNil(false), isHeader(false) { }

		Node(const Node& src) : value(src.value), black(src.black), parent(src.parent), left(src.left), right(src.right), isNil(src.isNil), isHeader(src.isHeader) {
		}
		~Node() { }

	};

	public:
	typedef typename std::allocator<Node>				allocator_type;
	typedef typename allocator_type::pointer			nodePtr;
	typedef typename allocator_type::reference			nodeReference;
	typedef typename allocator_type::size_type			size_type;
	typedef typename allocator_type::difference_type	difference_type;

	typedef Allocator									allocator_value;
	typedef typename allocator_value::pointer			pointer;
	typedef typename allocator_value::const_pointer		const_pointer;
	typedef typename allocator_value::reference			reference;
	typedef typename allocator_value::const_reference	const_reference;

	public:
	nodeReference nodeValue(nodePtr node) { return (*node); }

	protected:
	allocator_type	_alloc;
	allocator_value	_valloc;
	key_compare		_comp;
	nodePtr			_root;
	nodePtr			_nil;
	nodePtr			_header;
	size_type		_node_count;
	ft::stack<nodePtr> buffer;


	private:

	void	__initNilHeader(void) {
		_nil = _alloc.allocate(1);
		_alloc.construct(_nil, value_type());
		_nil->isNil = true;
		_nil->isHeader = false;
		_nil->left = _header;
		_nil->right = _nil;
		_nil->black = true;

		_header = _alloc.allocate(1);
		_alloc.construct(_header, value_type());
		_header->isNil = false;
		_header->isHeader = true;
		_header->black = true;
		_header->parent = _root;
		_header->left = _header;
		_header->right = _nil;
		_nil->parent = _header;

		_root = _nil;
	}

	public:
	RBtree(const key_compare& comp = Compare()) : _comp(comp), _root(0), _nil(0), _header(0), _node_count(0){ 
		__initNilHeader();
	}

	RBtree(const RBtree<Key, T>& src, const key_compare& comp = Compare()) :  _comp(comp), _root(0), _nil(0), _header(0), _node_count(0) { 
		*this = src;
	}

	RBtree	&operator=(const RBtree<Key, T>& src) {
		const_iterator cit = src.begin();
		const_iterator cite = src.end();
		_alloc = src._alloc;
		_valloc = src._valloc;
		_comp = src._comp;
		if (_root == 0)
			__initNilHeader();
		else { 
			clear();
		}
		if (src.size() != 0) {
			insert(cit, cite);
		}
		return *this;

	}

	~RBtree() {
		if (_node_count)
			__destroyDeallocate(_root);
		_alloc.destroy(_nil);
		_alloc.deallocate(_nil, 1);
		_alloc.destroy(_header);
		_alloc.deallocate(_header, 1);
		while (buffer.size()) {
			_alloc.deallocate(buffer.top(), 1);
			buffer.pop();
		}
	}

	public:
	class iterator;
	class const_iterator;
	class iterator {
		public:

		typedef typename RBtree<Key, T,Compare>::difference_type	difference_type;
		typedef typename RBtree<Key, T,Compare>::value_type			value_type;
		typedef typename RBtree<Key, T,Compare>::pointer			pointer;
		typedef typename RBtree<Key, T,Compare>::reference			reference;
		typedef std::random_access_iterator_tag						iterator_category;

		protected:
		nodePtr node;

		public:
		iterator(void) { }
		explicit iterator(nodePtr x) : node(x) { }

		bool		operator==(const iterator& y) const { return node == y.node; }
		bool		operator==(const const_iterator& y) const { return node == y.getNode(); }
		bool		operator!=(const iterator& y) const { return !(*this == y); }
		
		reference	operator*(void) const	{ return value(node); }
		pointer		operator->(void) const { 
			if (isHeader(node)) {
				nodePtr tmp = getRight(node);
				iterator it = iterator(tmp);
				return &(*it);
			}
			return &(operator*()); 
		}
		iterator&	operator++(void) {
			nodePtr save = node;
			if (getRight(node)->isNil == false) {
				node = getRight(node);
				while (getLeft(node)->isNil != true) {
					node = getLeft(node);
				}
			}
			else {
				nodePtr tmp = getParent(node);
				while ( node == getRight(tmp)) {
					node = tmp;
					tmp = getParent(tmp);
				}
				if (getRight(getParent(tmp)) == save)
					node = getParent(tmp);
				else if (getRight(node) != tmp)
					node = tmp;
			}
			return *this;
		}

		iterator	operator++(int) {
			iterator tmp = *this;
			++(*this);
			return tmp;
		}

		iterator&	operator--(void) {
			if (isHeader(node)) {
				node = node->right;
				return *this;
			}
			if (getLeft(node)->isNil != true) {
				node = getLeft(node);
				while (getRight(node)->isNil != true) {
					node = getRight(node);
				}
			}
			else {
				nodePtr tmp = getParent(node);
				while (node == getLeft(tmp)) {
					node = tmp;
					tmp = getParent(tmp);
				}
				node = tmp;
			}
			return *this;
		}

		iterator	operator--(int) {
			iterator tmp = *this;
			--(*this);
			return tmp;
		}

		nodePtr	getNode(void) const { return node; }
	};

	class const_iterator {
		protected:
		nodePtr node;


		public:
		typedef typename RBtree<Key, T,Compare>::difference_type	difference_type;
		typedef typename RBtree<Key, T,Compare>::value_type			value_type;
		typedef typename RBtree<Key, T,Compare>::const_pointer			pointer;
		typedef typename RBtree<Key, T,Compare>::const_reference			reference;
		typedef std::random_access_iterator_tag						iterator_category;

		public:
		const_iterator(void) { }
		explicit const_iterator(nodePtr x) : node(x) { }
		const_iterator(const iterator& src) : node(const_cast<nodePtr>(src.getNode())) { }
		bool		operator==(const const_iterator& y) const { return node == y.node; }
		bool		operator!=(const const_iterator& y) const { return node != y.node; }
		
		reference	operator*(void) const	{ return value(node); }
		pointer		operator->(void) const	{ 
			if (isHeader(node)) {
				nodePtr tmp = getRight(node);
				iterator it = iterator(tmp);
				return &(*it);
			}
			return &(operator*()); 
		}
		const_iterator&	operator++(void) {
			nodePtr save = node;

			if (getRight(node)->isNil != true) {
				node = getRight(node);
				while (getLeft(node)->isNil != true) {
					node = getLeft(node);
				}
			}
			else {
				nodePtr tmp = getParent(node);
				while ( node == getRight(tmp)) {
					node = tmp;
					tmp = getParent(tmp);
				}
				if (getRight(getParent(tmp)) == save)
					node = getParent(tmp);
				else if(getRight(node) != tmp)
					node = tmp;
			}
			return *this;
		}

		const_iterator	operator++(int) {
			const_iterator tmp = *this;
			++(*this);
			return tmp;
		}

		const_iterator&	operator--(void) {
			if (isHeader(node)) {
				node = node->right;
				return *this;
			}
			if (getLeft(node)->isNil != true) {
				node = getLeft(node);
				while (getRight(node)->isNil != true) {
					node = getRight(node);
				}
			}
			else {
				nodePtr tmp = getParent(node);
				while (node == getLeft(tmp)) {
					node = tmp;
					tmp = getParent(tmp);
				}
				node = tmp;
			}
			return *this;
		}

		const_iterator	operator--(int) {
			const_iterator tmp = *this;
			--(*this);
			return tmp;
		}
		nodePtr	getNode(void) const { return node; }
	};


	typedef typename ft::reverse_iterator<iterator>			reverse_iterator;
	typedef typename ft::reverse_iterator<const_iterator>	const_reverse_iterator;

	static reference	value(nodePtr node) { return (*node).value; }
	static nodePtr&		getParent(nodePtr node) { return (*node).parent; }
	static nodePtr&		getLeft(nodePtr node) { return (*node).left; }
	static nodePtr&		getRight(nodePtr node) { return (*node).right; }
	static nodePtr		getNil(nodePtr node) {
		nodePtr tmp = node;
		while (tmp->isNil != true) {
			tmp = tmp->parent; 
		}
		return tmp;
	}
	static bool			isHeader(nodePtr node) {
		if (node->isHeader)
			return true;
		return false;
	}

	private:

	void	__transplant(nodePtr nodeU, nodePtr nodeV) {
		if (nodeU->parent == _nil)
			_root = nodeV;
		else if (nodeU == nodeU->parent->left)
			nodeU->parent->left = nodeV;
		else
			nodeU->parent->right = nodeV;
		nodeV->parent = nodeU->parent;
	}

	void	__rBTDelete(nodePtr node) {
		nodePtr	x;
		nodePtr	y = node;
		bool		y_old_color = y->black;

		if (node->left == _nil) {
			x = node->right;	
			__transplant(node, node->right);
		}
		else if (node->right == _nil) {
			x = node->left;
			__transplant(node, node->left);
		}
		else {
			y = __minNode(node->right);
			y_old_color = y->black;
			x = y->right;
			if (y->parent == node) {
				x->parent = y;
			}
			else {
				__transplant(y, y->right);
				y->right = node->right;
				y->right->parent = y;
			}
			__transplant(node, y);
			y->left = node->left;
			y->left->parent = y;
			y->black = node->black;
			if (y->black == true)
				y_old_color = true;
			else
				y_old_color = false;
		}
		if (y_old_color == true)
			__rBTDelete_fix(x);
	}

	void	__rBTDelete_fix(nodePtr node) {
		nodePtr tmp;
		while (node != _root && __black(node)) {
			if (node == node->parent->left) {
				tmp = node->parent->right;
				if (__red(tmp)) {
					tmp->black = true;
					node->parent->black = false;
					__rotateLeft(node->parent);
					tmp = node->parent->right;
				}
				else if (__black(tmp->left) && __black(tmp->right)) {
					tmp->black = false;
					node = node->parent;
				}
				else if (__black(tmp->right)) {
					tmp->left->black = true;
					tmp->black = false;
					__rotateRight(tmp);
					tmp = node->parent->right;
				}
				else {
					tmp->black = node->parent->black;
					node->parent->black = true;
					tmp->right->black = true;
					__rotateLeft(node->parent);
					node = _root;
				}
			}
			else {
				tmp = node->parent->left;
				if (__red(tmp)) {
					tmp->black = true;
					node->parent->black = false;
					__rotateRight(node->parent);
					tmp = node->parent->left;
				}
				else if (__black(tmp->right) && __black(tmp->left)) {
					tmp->black = false;
					node = node->parent;
				}
				else if (__black(tmp->left)) {
					tmp->right->black = true;
					tmp->black = false;
					__rotateLeft(tmp);
					tmp = node->parent->left;
				}
				else {
					tmp->black = node->parent->black;
					node->parent->black = true;
					tmp->left->black = true;
					__rotateRight(node->parent);
					node = _root;
				}
			}
		}
		node->black = true;
	}

	void			__destroyDeallocate(nodePtr node) {
		if (node == _nil) {
			return ;
		}
		__destroyDeallocate(node->left);
		__destroyDeallocate(node->right);
		_alloc.destroy(node);
		_alloc.deallocate(node, 1);
	}

	nodePtr		__newNode(const value_type& value) {
		nodePtr	node = _alloc.allocate(1);
		_alloc.construct(node, value);
		node->parent = _nil;
		node->left = _nil;
		node->right = _nil;
		return (node);
	}

	void		__insert_fix(nodePtr node) {
		nodePtr y;

		while (__red(node->parent)) {
			if (node->parent == node->parent->parent->left) {
				y = node->parent->parent->right;
				if (__red(y)) {
					node->parent->black = true;
					y->black = true;
					node->parent->parent->black = false;
					node = node->parent->parent;
				}
				else if (node == node->parent->right) {
					node = node->parent;
					__rotateLeft(node);
				}
				else {
					node->parent->black = true;
					node->parent->parent->black = false;
					__rotateRight(node->parent->parent);
				}
			}
			else {
				y = node->parent->parent->left;
				if (__red(y)) {
					node->parent->black = true;
					y->black = true;
					node->parent->parent->black = false;
					node = node->parent->parent;
				}
				else if (node == node->parent->left) {
					node = node->parent;
					__rotateRight(node);
				}
				else {
					node->parent->black = true;
					node->parent->parent->black = false;
					__rotateLeft(node->parent->parent);
				}
			}
		}
	}

	void	__insert(nodePtr node, nodePtr base_node) {
		nodePtr y = _nil;
		nodePtr x = base_node;
		while (x != _nil) {
			y = x;
			if (_comp(node->value.first, x->value.first))
				x = x->left;
			else
				x = x->right;
		}
		node->parent = y;
		if (y == _nil)
			_root = node;
		else if (_comp(node->value.first, y->value.first))
			y->left = node;
		else
			y->right = node;
		node->left = _nil;
		node->right = _nil;
		node->black = false;

		__insert_fix(node);
	}

	void			__colorFlip(nodePtr node) {
		nodePtr	right = node->right;
		nodePtr	left = node->left;

		node->black = node->black ? false : true;
		if (right)
			right->black = right->black ? false : true;
		if (left)
			left->black = left->black ? false : true;
	}

	void		__rotateLeft(nodePtr node) {
		nodePtr tmp = node->right;

		node->right = tmp->left;
		if (tmp->left != _nil)
			tmp->left->parent = node;
		tmp->parent = node->parent;
		if (node->parent == _nil)
			_root = tmp;
		else if (node == node->parent->left)
			node->parent->left = tmp;
		else
			node->parent->right = tmp;
		tmp->left = node;
		node->parent = tmp;
	}

	void		__rotateRight(nodePtr node) {
		nodePtr tmp = node->left;

		node->left = tmp->right;
		if (tmp->right != _nil)
			tmp->right->parent = node;
		tmp->parent = node->parent;
		if (node->parent == _nil)
			_root = tmp;
		else if (node == node->parent->right)
			node->parent->right = tmp;
		else
			node->parent->left = tmp;
		tmp->right = node;
		node->parent = tmp;
	}

	bool			__black(const nodePtr node) {
		if (node->black)
			return true;
		else
			return false;
	}

	bool			__red(const nodePtr node) {
		return !(__black(node));
	}

	nodePtr		__minNode(nodePtr node) {
		nodePtr x = node;
		while (x->left != _nil) {
			x = x->left;
		}
		return (x);
	}

	nodePtr		__maxNode(nodePtr node) {
		nodePtr x = node;
		while (x->right != _nil) {
			x = x->right;
		}
		return (x);
	}

	void	__updateHeader(void) {
		_header->parent = _root;
		_nil->parent = _header;
		_nil->left = _nil;
		_nil->left = _nil;
		_header->left = __minNode(_root);
		_header->right = __maxNode(_root);
		if (_node_count == 0) {
			_root = _nil;
			_header->left = _header;
			_header->right = _header;
			_header->parent = _header;
		}
	}

	void	__updateHeader(nodePtr node) {
		if (_node_count == 0) {
			_root = _nil;
			_header->left = _header;
			_header->right = _header;
			_header->parent = _header;
		}
		else {
			_header->parent = _root;
			_nil->parent = _header;
			_nil->left = _nil;
			_nil->left = _nil;
			if (size() < 3) {
				_header->left = __minNode(_root);
				_header->right = __maxNode(_root);
			}
			else {
				if (node->value.first < _header->left->value.first)
					_header->left = node;
				if (node->value.first > _header->right->value.first)
					_header->right = node;
			}
		}
	}


	bool	__isKeyExist(const key_type& k) const {
		nodePtr x = _root;
		while (x != _nil) {
			if (k == x->value.first)
				return true;
			if (_comp(k, x->value.first))
				x = x->left;
			else
				x = x->right;
		}
		return false;
	}
	
	public:

	key_compare	key_comp(void) const { return _comp; }
	iterator	findHelper(const key_type& key) {
		if(!__isKeyExist(key))
			return end();
		return find(key);
	}

	const_iterator	findHelper(const key_type& key) const {
		if(!__isKeyExist(key))
			return end();
		return find(key);
	}

	mapped_type& at(const key_type& k) {
		if (!__isKeyExist(k))
			throw std::out_of_range("Exception: out_of_range");
		else
			return find(k)->second;
	}

	mapped_type& at(const key_type& k) const {
		if (!__isKeyExist(k))
			throw std::out_of_range("Exception: out_of_range");
		else
			return find(k)->second;
	}


	iterator	find(const key_type& key) {

		nodePtr it = _root;
		while (it != _nil && it->value.first != key) {
			if (_comp(key, it->value.first)) 
				it = it->left;
			else
				it = it->right;
		}
		return iterator(it);
	}

	const_iterator	find(const key_type& key) const {
		nodePtr it = _root;
		while (it != _nil && it->value.first != key) {
			if (_comp(key, it->value.first)) 
				it = it->left;
			else
				it = it->right;
		}
		return const_iterator(it);
	}

	ft::pair<iterator, bool>	insert(const value_type& value) {
		ft::pair<iterator, bool> res;

		if (find(value.first) == iterator(_nil)) {
			nodePtr node = __newNode(value);
			res.second = true;
			__insert(node, _root);
			_root->black = true;
			_node_count++;
			res.first = iterator(node);
		}
		else {
			res.second = false;
			res.first = find(value.first);
			_root->black = true;
		}
			__updateHeader();
		return res;
	}

	iterator	insert(iterator position, const value_type& value) {
		if (!__isKeyExist(value.first)) {
			nodePtr node = __newNode(value);
			if (size() <= 1)
				__insert(node, _root);
			else if (position == begin() && _comp(value.first, position->first)) {
				__insert(node, _header->left);
			}
			else if (position == begin() && _comp(position->first, value.first)) {
				__insert(node, _header->right);
			}
			else
				__insert(node, (--position).getNode());
			_root->black = true;
			_node_count++;
			__updateHeader(node);
			return iterator(node);
		}
		return find(value.first);
	}

	template <class InputIterator>
	void	insert(InputIterator first, InputIterator last) {
		while (first != last) {
			insert(*first++);
		}
	}



	size_type	erase(const key_type& key) {
		iterator 	it;
		size_type	count = 0;
		if (_node_count == 0)
			return (0);
		while ((it = find(key)) != iterator(_nil)) {
			__rBTDelete(it.getNode());
			_alloc.destroy(it.getNode());
			_alloc.deallocate(it.getNode(), 1);
			count++;
			_node_count--;
		}
		__updateHeader();
		return (count);
	}

	void	erase(iterator position) {
		if (_node_count == 0)
			return ;
		iterator it = position;
		if (position == end()) {
			it = iterator(_header->right);
			position = it;
		}
		__rBTDelete(position.getNode());
		_alloc.destroy(it.getNode());
		_alloc.deallocate(it.getNode(), 1);
		_node_count--;
		__updateHeader();
	}

	void	__erase(nodePtr node) {
		while (node != _nil) {
			__erase(node->right);
			nodePtr y = node->left;
			_alloc.destroy(node);
			buffer.push(node);
			node = y;
		}
	}

	void	erase(iterator first, iterator last) {
		if (_node_count == 0)
			return ;
		if (first == begin() && last == end()) {
			__erase(_root);
			_root = _nil;
			_header->left = _header;
			_header->right = _header;
			_header->parent = _root;
			_node_count = 0;
		}
		else {
			iterator it;
			while (first != last) {
				it = first;
				first++;
				erase(it);
			}
		}
	}

	public:
	allocator_type	get_allocator(void) const {
		return _alloc;
	}

	private:
	iterator	__leftMost() {
		return (iterator(_header->left));
	}

	const_iterator	__leftMost() const {
		return (const_iterator(_header->left));
	}

	iterator	__rightMost() {
		return (iterator(_header->right));
	}


	const_iterator	__rightMost() const {
		return (const_iterator(_header->right));
	}


	public:
	// Left Most
	iterator				begin(void)				{ return __leftMost(); }
	const_iterator			begin(void) const		{ return __leftMost(); }
	reverse_iterator		rbegin(void)			{ return reverse_iterator(end()); }
	const_reverse_iterator	rbegin(void) const		{ return const_reverse_iterator(end()); }
	iterator				end(void)				{ return iterator(_header); }
	const_iterator			end(void) const			{ return const_iterator(_header); }
	reverse_iterator		rend(void)				{ return reverse_iterator(begin()); }
	const_reverse_iterator	rend(void) const		{ return const_reverse_iterator(begin()); }

	iterator					lower_bound(const key_type& k) {
		nodePtr y = begin().getNode();
		nodePtr x = _root;
		if (!__isKeyExist(k)) {
			if (k > _header->left->value.first && k < _header->right->value.first)
				return end();
			else if (k > _header->right->value.first)
				return end();
			else
				return begin();
		}
		while (x != _nil) {
			if (!_comp(x->value.first, k)) {
				y = x;
				x = x->left;
			}
			else
				x = x->right;
		}
		return iterator(y);
	}

	const_iterator				lower_bound(const key_type& k) const {
		nodePtr y = begin().getNode();
		nodePtr x = _root;
		if (!__isKeyExist(k)) {
			if (k > _header->left->value.first && k < _header->right->value.first)
				return end();
			if (k > _header->right->value.first)
				return end();
			else
				return begin();
		}

		while (x != _nil) {
			if (!_comp(x->value.first, k)) {
				y = x;
				x = x->left;
			}
			else
				x = x->right;
		}
		return const_iterator(y);
	}

	iterator					upper_bound(const key_type& k) {
		nodePtr y = begin().getNode();
		nodePtr x = _root;
		if (!__isKeyExist(k)) {
			if (k > _header->left->value.first && k < _header->right->value.first)
				return end();
			if (k > _header->right->value.first)
				return end();
			else
				return begin();
		}
		if (k == _header->right->value.first)
			return end();

		while (x != _nil) {
			if (_comp(k, x->value.first)) {
				y = x;
				x = x->left;
			}
			else
				x = x->right;
		}
		
		return iterator(y);
	}

	const_iterator				upper_bound(const key_type& k) const {
		nodePtr y = begin().getNode();
		nodePtr x = _root;
		if (!__isKeyExist(k)) {
			if (k > _header->left->value.first && k < _header->right->value.first)
				return end();
			if (k > _header->right->value.first)
				return end();
			else
				return begin();
		}

		if (k == _header->right->value.first)
			return end();

		while (x != _nil) {
			if (_comp(k, x->value.first)) {
				y = x;
				x = x->left;
			}
			else
				x = x->right;
		}
		return const_iterator(y);
	}

	ft::pair<const_iterator, const_iterator>
	equal_range(const key_type& k) const {
		return ft::pair<const_iterator, const_iterator>(lower_bound(k), upper_bound(k));

	}

	ft::pair<iterator, iterator>
	equal_range(const key_type& k) {
		return ft::pair<iterator, iterator>(lower_bound(k), upper_bound(k));
	}


	public:
	bool					empty(void)	const		{ return (_node_count == 0); }
	size_type				size() const			{ return (_node_count); }
	size_type				max_size(void) const	{ return (_alloc.max_size()); }
	void					swap(RBtree<Key, T>& t) {
			std::swap(_root, t._root);
			std::swap(_node_count, t._node_count);
			std::swap(_nil, t._nil);
			std::swap(_header, t._header);
			std::swap(_comp, t._comp);
	}

	void					clear(void) {
		erase(begin(), end());
		_root = _nil;
		_nil->parent = _header;
		_header->left = _header;
		_header->right = _header;
		_header->parent = _header;
		_node_count = 0;
	}
	

	// DEBUG
	private:
	void	__printTab(size_type level) {
		for (size_type i = 0; i < level; i++)
			std::cout << "\t\t";
	}

	void	__printNode(nodePtr node) {
		if (node->black)
			std::cout << "\033[1m[ " << node->value.first << " ] " << "\033[0m" << std::endl;
		else
			std::cout << "\033[1;31m[ " << node->value.first << " ] " << "\033[0m" << std::endl;
	}

	void	__printTree(nodePtr node, size_type level) {
		if (node == _nil)
		{
			__printTab(level);
			std::cout << "" << std::endl;
			return ;
		}
		__printTree(node->right, level + 1);
		__printTab(level);
		__printNode(node);
		__printTree(node->left, level + 1);
	}
	void	printTree(void) { __printTree(_root, 0); }

};

template <class Key, class T>
bool	operator==(const RBtree<Key, T>& x, const RBtree<Key, T>& y) {
	return (x.size() == y.size() && ft::equal(x.begin(), x.end(), y.begin()));
}

template <class Key, class T>
bool	operator< (const RBtree<Key, T>& x, const RBtree<Key, T>& y) {
	return ft::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}

template <class Key, class T>
bool	operator!=(const RBtree<Key, T>& x, const RBtree<Key, T>& y) {
	return !(x == y);
}

template <class Key, class T>
bool	operator> (const RBtree<Key, T>& x, const RBtree<Key, T>& y) {
	return (y < x);
}

template <class Key, class T>
bool	operator>=(const RBtree<Key, T>& x, const RBtree<Key, T>& y) {
	return !(x < y);
}

template <class Key, class T>
bool	operator<=(const RBtree<Key, T>& x, const RBtree<Key, T>& y) {
	return !(y < x);
}


#endif
