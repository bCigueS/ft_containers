/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbeylot <sbeylot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 12:02:41 by sbeylot           #+#    #+#             */
/*   Updated: 2023/03/01 20:33:12 by sbeylot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STACK_HPP
#define STACK_HPP

#include <iostream>
#include "vector.hpp"

namespace ft {

	template <class T, class Container = ft::vector<T> >
	class stack {

		public:
			friend	bool operator==(const stack<T, Container>& x, const stack<T, Container>& y) { return x.c == y.c; }
			friend	bool operator< (const stack<T, Container>& x, const stack<T, Container>& y) { return x.c < y.c; }

		public:
			typedef typename	Container::value_type	value_type;
			typedef typename	Container::size_type	size_type;
			typedef				Container				container_type;

		protected:
			Container c;

		public:
			stack(const stack& src) : c(src.c) { *this = src; } 
			stack&	operator=(const stack& other) {
				c = other.c;
				return *this;
			}

			explicit stack(const Container& src = Container()) : c(src) {}
			virtual	~stack(void) {
			}

			bool		empty()			const		{ return c.empty(); }
			size_type	size()			const		{ return c.size(); }
			value_type&	top()						{ return c.back(); }
			value_type	top()		 	const		{ return c.back(); }
			void		push(const value_type& x)	{ c.push_back(x); }
			void		pop()						{ c.pop_back(); }

	};


	template <class T, class Container>
	bool operator!=(const stack<T, Container>& x, const stack<T, Container>& y) {
		return !(x == y);
	}

	template <class T, class Container>
	bool operator>(const stack<T, Container>& x, const stack<T, Container>& y) {
		return y < x;
	}	

	template <class T, class Container>
	bool operator>=(const stack<T, Container>& x, const stack<T, Container>& y) {
		return !(x < y);
	}	

	template <class T, class Container>
	bool operator<=(const stack<T, Container>& x, const stack<T, Container>& y) {
		return !(y < x);
	}	
}


#endif
