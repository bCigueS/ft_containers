/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbeylot <sbeylot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 15:53:42 by sbeylot           #+#    #+#             */
/*   Updated: 2023/03/06 10:17:33 by sbeylot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <exception>
#include <iostream>
#include <sstream>
#include <memory>
#include <iterator>
#include <algorithm>

#include "utility.hpp"

namespace ft {
	
	template <class T, class Allocator = std::allocator<T> >
	class vector {

		public:
		typedef T												value_type;
		typedef Allocator										allocator_type;
		typedef	typename allocator_type::pointer				pointer;
		typedef typename allocator_type::pointer				iterator;
		typedef typename allocator_type::const_pointer			const_iterator;
		typedef ft::reverse_iterator<iterator>					reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>			const_reverse_iterator;
		typedef	typename allocator_type::reference				reference;
		typedef	typename allocator_type::const_reference		const_reference;
		typedef	typename allocator_type::size_type				size_type;
		typedef typename allocator_type::difference_type		difference_type;

		protected:

		allocator_type _alloc;
		iterator start;
		iterator finish;
		iterator end_of_storage;		

		private:
		void	_destroy_deallocate(iterator first, iterator end, size_type capacity)
		{
			iterator it = first;
			iterator ite = end;

			for (; it != ite; it++)
				_alloc.destroy(it);
			_alloc.deallocate(first, capacity);
		}


		/*	------------------------------------------| CONSTRUCTOR DESTRUCTOR COPY |---------------------------------------------------  */
		public:
		/*	-| Default Constructor |- */
		explicit vector(const allocator_type& alloc = allocator_type()) :
			_alloc(alloc), start(0), finish(0), end_of_storage(0) {
		}

		/*	-| Fill Constructor |- */
		explicit vector(size_type n, const value_type& value = value_type(), const allocator_type& alloc = allocator_type()) : 
			_alloc(alloc), start(0), finish(0), end_of_storage(0) {
			start = _alloc.allocate(n);
			//std::uninitialized_fill_n(start, n, value);
			for(size_type i = 0; i < n; i++)
				_alloc.construct(start + i, value);
			finish = start + n;
			end_of_storage = finish;
		}

		/*	-| Range Constructor |- */
		template <class InputIterator>
		vector(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type(), typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type* = 0) : _alloc(alloc), start(0), finish(0), end_of_storage(0)
		{
			difference_type distance = std::distance(first, last);

			start = _alloc.allocate(distance);
			finish = std::uninitialized_copy(first, last, start);
			end_of_storage = finish;
		}

		/*	-| Copy Constructor |- */
		vector(const vector<T, Allocator>& x) : start(0), finish(0), end_of_storage(0) {
			*this = x;
		}

		/*	-| Destructor |- */
		~vector() {
			if (capacity()) {
				_destroy_deallocate(begin(), end(), capacity());
			}
		} 

		/*	-| = Operator Overload |- */
		vector<T, Allocator>& operator=(const vector<T, Allocator>&x) {
			if (this == &x)
				return *this;

			this->_alloc = x.get_allocator();
			size_type new_capacity = capacity();
			if (capacity() != 0) {
				_destroy_deallocate(begin(), end(), capacity());
			}
			if (x.capacity() != 0) {
				if (new_capacity < x.capacity())
					new_capacity = x.size();
				start = _alloc.allocate(new_capacity);
				finish = std::uninitialized_copy(x.begin(), x.end(), start);
				end_of_storage = start + new_capacity;
			}
			else {
				start = 0;
				finish = 0;
				end_of_storage = 0;
			}
			return *this;
		}

		template <class InputIterator>
		void assign(InputIterator first, InputIterator last, typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type* = 0) {
			size_type old_capacity = capacity();
			difference_type distance = std::distance(first, last);
			iterator tmp;

			if (static_cast<size_type>(distance) > old_capacity)
				tmp = _alloc.allocate(distance);
			else
				tmp = _alloc.allocate(old_capacity);
			std::uninitialized_copy(first, last, tmp);
			_destroy_deallocate(begin(), end(), old_capacity);
			start = tmp;
			finish = tmp + distance;
			if (static_cast<size_type>(distance) > old_capacity)
				end_of_storage = tmp + distance;
			else
				end_of_storage = tmp + old_capacity;
		} 

		void assign(size_type n, const T& u) {
			size_type old_capacity = capacity();
			iterator tmp;
			if (n > old_capacity)
				tmp = _alloc.allocate(n);
			else
				tmp = _alloc.allocate(old_capacity);
			std::uninitialized_fill_n(tmp, n, u);
			_destroy_deallocate(begin(), end(), old_capacity);
			start = tmp;
			finish = tmp + n;
			if (n > old_capacity)
				end_of_storage = tmp + n;
			else
				end_of_storage = tmp + old_capacity;
		}

		allocator_type get_allocator() const	{ return (this->_alloc); }

		/*	---| ITERATORS |---  */
		public:
		iterator				begin() 		{ return (this->start); }
		const_iterator			begin() const	{ return (this->start); }
		reverse_iterator 		rbegin()		{ return reverse_iterator(end()); }
		const_reverse_iterator	rbegin() const	{ return const_reverse_iterator(end()); }
		iterator				end()			{ return (this->finish); }
		const_iterator			end() const		{ return (this->finish); }
		reverse_iterator		rend()			{ return reverse_iterator(begin()); }
		const_reverse_iterator	rend() const	{ return const_reverse_iterator(begin()); }
		
		
		/*	---| CAPACITY |---  */
		public:
		size_type		size() const		{ return (this->finish - this->start); }
		size_type		max_size() const	{ return _alloc.max_size(); }
		size_type		capacity() const	{ return (this->end_of_storage - this->start); }
		bool			empty() const		{ return  begin() == end(); }
		
		void		resize(size_type count, value_type value = value_type()) {
			if (count < size()) {
				for (size_type i = count; i < size(); i++) {
					_alloc.destroy(begin() + i);
				}
				finish = begin() + count;
			}
			else {
				size_type new_capacity = capacity();

				if (capacity() < count) {
					if (count > capacity() && capacity() > size())
						new_capacity = count;
					else if (capacity() * 2 > count) 
						new_capacity = capacity() * 2;
					else
						new_capacity = count;
				}

				iterator tmp = _alloc.allocate(new_capacity);
				std::uninitialized_copy(begin(), end(), tmp);
				std::uninitialized_fill_n(tmp + size(), count - size(), value);
				_destroy_deallocate(begin(), end(), capacity());
				start = tmp;
				finish = tmp + count;
				end_of_storage = tmp + new_capacity;
				}
		}

		void		reserve(size_type n) { 
			if (n > max_size())
				throw std::length_error("vector::reserve");
			if (n > capacity()) {
				size_type old_size = size();
				iterator tmp;

				tmp = _alloc.allocate(n);
				std::uninitialized_copy(begin(), end(), tmp);
				_destroy_deallocate(begin(), end(), capacity());
				start = tmp;
				finish = tmp + old_size;
				end_of_storage = start + n;
			}
		}

		/*	----------------------------------------------------| ELEMENT ACCESS |------------------------------------------------------  */
		public:
		reference		operator[](size_type n)			{ return *(begin() + n); }
		const_reference	operator[](size_type n) const 	{ return *(begin() + n); }
		
		reference		at(size_type n) 				{ 
			if (n >= size()) {
				std::ostringstream oss;
				oss << "out of range: n (which is " << n << ") >= this->size() (which is " << size() << ")";
				throw std::out_of_range(oss.str());
			}
			return *(begin() + n);
		}

		const_reference at(size_type n) const			{ 
			if (n >= size()) {
				throw std::out_of_range("out of range");
			}
			return *(begin() + n); 
		}

		reference		front()							{ return *begin(); }
		const_reference	front() const					{ return *begin(); }
		reference		back()							{ return *(end() - 1); }
		const_reference	back() const					{ return *(end() - 1); }

		/*	--------------------------------------------------| MODIFIERS |-------------------------------------------------------------  */
		public:
		void		push_back(const T& x) {
			if (size() == capacity()) {
				if (capacity() == 0)
					reserve(1);
				else
					reserve(capacity() * 2);
			}
			_alloc.construct(finish, x);
			finish++;
		}

		void		pop_back() {
			_alloc.destroy(end() - 1);
			if (begin() != end())
				--finish;
		}

		iterator	insert(iterator position, const T& x) {
			if (finish != end_of_storage) {
				std::uninitialized_copy(position, end(), position + 1);
				finish++;
				_alloc.destroy(position);
				_alloc.construct(position, x);
				return position;
			}
			else if (size() == 0) {
				start = _alloc.allocate(1);
				*start = x;	
				finish = start + 1;
				end_of_storage = finish;
				return start;
			}
			else {
				difference_type pos = position - begin();
				size_type len = size();
				iterator tmp = _alloc.allocate(size() * 2);
				iterator new_pos = tmp + pos;

				std::uninitialized_copy(begin(), position, tmp);
				_alloc.construct(tmp + (position - begin()), x);
				std::uninitialized_copy(position, end(), new_pos + 1);
				*new_pos = x;
				_destroy_deallocate(begin(), end(), capacity());
				finish = tmp + (len + 1);
				end_of_storage = tmp + (len * 2);
				start = tmp;
				return new_pos;
			}
		}

		void		insert(iterator position, size_type n, const T& x) {
			if (n == 0)
				return ;
			difference_type free_cap = end_of_storage - finish;

			if (static_cast<size_type>(free_cap) >= n) {
					std::uninitialized_copy(position, end(), position + n);	
					std::uninitialized_fill_n(position, n, x);
					finish += n;
			}
			else {
				difference_type	distance = position - begin() + n;
				difference_type insert_pos = position - begin();
				size_type len = size() + std::max(size(), n);
				size_type old_size = size();
				iterator tmp = _alloc.allocate(len);
				std::uninitialized_copy(position, end(), tmp + distance);
				std::uninitialized_copy(begin(), position, tmp); 
				std::uninitialized_fill_n(tmp + insert_pos, n, x);
				_destroy_deallocate(begin(), end(), capacity());
				start = tmp;
				finish = tmp + old_size + n;
				end_of_storage = tmp + len;
			}
		}



		template<class InputIterator>
		void		insert(iterator position, InputIterator first, InputIterator last, typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type* = 0) {
			if (first == last) 
				return ;
			difference_type distance_insert = std::distance(first, last);

			if (end_of_storage - finish >= distance_insert) {
				if (end() - position > distance_insert) {
				std::uninitialized_copy(end() - distance_insert, end(), end());
				std::copy_backward(position, end() - distance_insert, end());
				std::uninitialized_copy(first, last, position);
				finish += distance_insert;
				}
				else {
					std::uninitialized_copy(position, end(), position + distance_insert);
					std::uninitialized_copy(first, last, position);
					finish += distance_insert;
				}
			}
			else {
				difference_type distance = position - begin() + distance_insert;
				size_type len = size() + std::max(size(), static_cast<size_type>(distance_insert));
				size_type old_size = size();
				iterator tmp = _alloc.allocate(len);
				std::uninitialized_copy(position, end(), tmp + distance);
				std::uninitialized_copy(begin(), position, tmp);
				for (iterator it = tmp + (position - begin()); first != last; first++, it++) {
					_alloc.construct(it, *first);
				}
				_destroy_deallocate(begin(), end(), capacity());
				start = tmp;
				finish = tmp + old_size + distance_insert;
				end_of_storage = tmp + len;
			}
		}

		iterator	erase(iterator position) {
			size_type old_size = size();
			size_type old_capacity = capacity();
			difference_type distance = position - begin();
			iterator tmp = _alloc.allocate(old_capacity);
			std::uninitialized_copy(begin(), position, tmp);
			std::uninitialized_copy(position + 1, end(), tmp + distance);
			_destroy_deallocate(begin(), end(), capacity());
			start = tmp;
			finish = tmp + (old_size - 1);
			end_of_storage = tmp + (old_capacity);
			return (tmp + distance); 
		}

		iterator	erase(iterator first, iterator last) {
			difference_type range_len = last - first;
			size_type new_size = size() - range_len;
			size_type old_capacity = capacity();
			difference_type distance = first - begin();
			iterator tmp = _alloc.allocate(old_capacity);
			std::uninitialized_copy(begin(), first, tmp);
			std::uninitialized_copy(last, end(), tmp + distance);
			_destroy_deallocate(begin(), end(), capacity());
			start = tmp;
			finish = tmp + (new_size);
			end_of_storage = tmp + old_capacity;
			return (tmp + distance);
		}

		void		clear() { while (size()) pop_back(); }

		void		swap(vector<T>& x) {
			std::swap(start, x.start);
			std::swap(finish, x.finish);
			std::swap(end_of_storage, x.end_of_storage);
		}
	};

	template <class T, class Allocator>
	bool operator==(const vector<T, Allocator>&x, const vector<T, Allocator>&y)
	{ return (x.size() == y.size() && ft::equal(x.begin(), x.end(), y.begin())); }

	template <class T, class Allocator>
	bool operator<(const vector<T, Allocator>&x, const vector<T, Allocator>&y)
	{ return ft::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end()); }

	template <class T, class Allocator>
	bool operator!=(const vector<T, Allocator>&x, const vector<T, Allocator>&y) 
	{ return !(x == y); }

	template <class T, class Allocator>
	bool operator> (const vector<T, Allocator>&x, const vector<T, Allocator>&y)
	{ return y < x; }
	
	template <class T, class Allocator>
	bool operator>=(const vector<T, Allocator>&x, const vector<T, Allocator>&y)
	{ return !(x < y); }
	
	template <class T, class Allocator>
	bool operator<=(const vector<T, Allocator>&x, const vector<T, Allocator>&y) 
	{ return !(y < x); }
	
	template <class T, class Allocator>
	void swap(vector<T, Allocator>&x, vector<T, Allocator>&y)
	{ x.swap(y); }

}

#endif
