/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbeylot <sbeylot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 10:39:34 by sbeylot           #+#    #+#             */
/*   Updated: 2023/03/06 10:09:08 by sbeylot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_HPP
#define MAP_HPP

#include <exception>
#include <iostream>
#include <sstream>
#include <memory>
#include <iterator>
#include <algorithm>

#include "utility.hpp"
#include "RBtree.hpp"

namespace ft {

	template <class Key, class T, class Compare = std::less<Key>, class Allocator = std::allocator<ft::pair<const Key, T> > >
	class map {
		private:
		typedef RBtree<Key, T, Compare, Allocator> rb_tree;
		rb_tree	_data;
	
		public:
		
		typedef Key											key_type;
		typedef T											mapped_type;
		typedef ft::pair<const Key, T>						value_type;
		typedef Compare										key_compare;
		typedef	Allocator									allocator_type;
		typedef typename rb_tree::reference					reference;
		typedef typename rb_tree::const_reference			const_reference;
		typedef	typename rb_tree::pointer					pointer;
		typedef	typename rb_tree::const_pointer				const_pointer;

		typedef	typename rb_tree::iterator					iterator;
		typedef typename rb_tree::const_iterator			const_iterator;
		typedef typename rb_tree::size_type					size_type;
		typedef typename rb_tree::difference_type			difference_type;
		typedef ft::reverse_iterator<iterator>				reverse_iterator;
		typedef	ft::reverse_iterator<const_iterator>		const_reverse_iterator;

		class value_compare : public std::binary_function<value_type, value_type, bool> {

			friend class map;

			protected:
			Compare comp;
			value_compare(Compare c) : comp(c) { }

			public:
			typedef bool result_type;
			typedef value_type first_argument_type;
			typedef value_type second_argument_type;
			bool operator() (const value_type& x, const value_type& y) const {
				return comp(x.first, y.first);
			}
		};

		public:

		/*	---|	Constructor/Destructor		|---	*/
		explicit map(const Compare& comp = Compare(), const Allocator& = Allocator()) : _data(comp) { }

		template <class InputIterator>
		map(InputIterator first, InputIterator last, const Compare& comp = Compare(), const Allocator& = Allocator()) : _data(comp) { 
			_data.insert(first, last);
		}
		map(const map<Key, T, Compare, Allocator>& x) {
			*this = x;
		}
		~map() {

		}

		map<Key, T, Compare, Allocator>& operator=(const map<Key, T, Compare, Allocator>& x) { 
				_data = x._data;
				return *this;
		}


		/*	---|	Iterator	|---	*/
		iterator				begin()			{ return _data.begin(); }
		const_iterator			begin() const	{ return _data.begin(); }
		iterator				end()			{ return _data.end(); }
		const_iterator			end() const		{ return _data.end(); }
		reverse_iterator		rbegin()		{ return _data.rbegin(); }
		const_reverse_iterator	rbegin() const	{ return _data.rbegin(); }
		reverse_iterator		rend()			{ return _data.rend(); }
		const_reverse_iterator	rend() const	{ return _data.rend(); }

		iterator				lower_bound(const key_type& x)			{ return _data.lower_bound(x); }
		const_iterator			lower_bound(const key_type& x) const	{ return _data.lower_bound(x); }
		iterator				upper_bound(const key_type& x)			{ return _data.upper_bound(x); }
		const_iterator			upper_bound(const key_type& x) const	{ return _data.upper_bound(x); }
		
		ft::pair<const_iterator, const_iterator>
		equal_range(const key_type& x) const	{ return _data.equal_range(x); }

		ft::pair<iterator, iterator>
		equal_range(const key_type& x)			{ return _data.equal_range(x); }

		/*	---|	Capacity	|---	*/
		bool		empty() const				{ return _data.empty(); }
		size_type	size() const				{ return _data.size();	}
		size_type	max_size() const			{ return _data.max_size(); }

		/*	---|	Element acces	|--- */

	   mapped_type& operator[](const key_type& k) {
			return (*((insert(value_type(k, T()))).first)).second;
		}

	   mapped_type& at(const key_type& k) {
		   return _data.at(k);
	   }

	   mapped_type& at(const key_type& k) const {
		   return _data.at(k);
	   }

		/*	---|	Modifier	|---	*/
		ft::pair<iterator, bool> insert(const value_type& x) {
			return _data.insert(x);
		}

		iterator			insert(iterator position, const value_type& x) {
			return _data.insert(position, x);
		}

		template <class InputIterator>
		void	insert(InputIterator first, InputIterator last) {
			_data.insert(first, last);
		}

		void		erase(iterator position) {
			_data.erase(position);
		}

		size_type	erase(const key_type& x) {
			return (_data.erase(x));
		}
		void		erase(iterator first, iterator last) {
			_data.erase(first, last);
		}

		void		swap(map<Key, T, Compare>& t) {
			_data.swap(t._data);
		}

		void		clear() {
			_data.clear();
		}

		void	printTree(void) {
			_data.printTree();
		}	


		/*	---|	Observers	|---	*/
		key_compare		key_comp() const { return _data.key_comp(); }
		value_compare	value_comp() const { return value_compare(key_comp()); }

		/*	---|	Map Operation	|---	*/
		iterator		find(const key_type& x) {
			return _data.findHelper(x);
		}

		const_iterator	find(const	key_type& x) const {
			return _data.findHelper(x);
		}

		size_type		count(const key_type& x) const {
			if (find(x) != end())
				return 1;
			else
				return 0;
		}

	};

	template <class Key, class T, class Compare, class Allocator>
	bool operator==(
			const map<Key, T, Compare, Allocator>&x,
			const map<Key, T, Compare, Allocator>&y)
	{ return (x.size() == y.size() && ft::equal(x.begin(), x.end(), y.begin())); }

	template <class Key, class T, class Compare, class Allocator>
	bool operator< (
			const map<Key, T, Compare, Allocator>&x,
			const map<Key, T, Compare, Allocator>&y)
	{ return ft::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end()); }

	template <class Key, class T, class Compare, class Allocator>
	bool operator!=(
			const map<Key, T, Compare, Allocator>&x,
			const map<Key, T, Compare, Allocator>&y)
	{ return !(x == y); }

	template <class Key, class T, class Compare, class Allocator>
	bool operator> (
			const map<Key, T, Compare, Allocator>&x,
			const map<Key, T, Compare, Allocator>&y)
	{ return (y < x); }

	template <class Key, class T, class Compare, class Allocator>
	bool operator>=(
			const map<Key, T, Compare, Allocator>&x,
			const map<Key, T, Compare, Allocator>&y)
	{ return !(x < y); }

	template <class Key, class T, class Compare, class Allocator>
	bool operator<=(
			const map<Key, T, Compare, Allocator>&x,
			const map<Key, T, Compare, Allocator>&y)
	{ return !(y < x); }
	
	template <class Key, class T, class Compare, class Allocator>
	void swap(
			map<Key, T, Compare, Allocator>&x,
			map<Key, T, Compare, Allocator>&y)
	{ x.swap(y); }

}


#endif
