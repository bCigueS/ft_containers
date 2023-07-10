/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reverse_iterator.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbeylot <sbeylot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 15:29:26 by sbeylot           #+#    #+#             */
/*   Updated: 2023/03/06 10:19:37 by sbeylot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REVERSE_ITERATOR_HPP
#define REVERSE_ITERATOR_HPP

#include "iterator_traits.hpp"

namespace ft {

	template <class Iterator>
	class reverse_iterator {

		public:
		typedef typename ft::iterator_traits<Iterator>::iterator_category	iterator_category;
		typedef typename ft::iterator_traits<Iterator>::difference_type		difference_type;
		typedef typename ft::iterator_traits<Iterator>::value_type			value_type;
		typedef typename ft::iterator_traits<Iterator>::pointer				pointer;
		typedef typename ft::iterator_traits<Iterator>::reference			reference;

		protected:
		Iterator	current;

		public:
		//	Default Constructor
		reverse_iterator(void) : current() {}
		//	Constructor with iterator to adapt
		explicit reverse_iterator( Iterator x) : current(x) { }

		//	Copy constructor
		template < class U >
		reverse_iterator( const reverse_iterator<U>& other ) : current(other.base()) { 
		}

		public:
		//	Conversion
		Iterator	base(void) const { return this->current; }

		//	Dereferencement
		reference	operator*(void) const {
			Iterator tmp = this->current;
			return ( *--tmp) ;
		}
		//	Pointer access 
		pointer	operator->(void) const {
			return &(operator*());
		}

		reverse_iterator&	operator++(void) {
			--this->current;
			return ( *this );
		}
		reverse_iterator	operator++(int) {
			reverse_iterator tmp = *this;
			--this->current;
			return ( tmp );
		}
		reverse_iterator&	operator--(void) {
			++this->current;
			return ( *this );
		}
		reverse_iterator	operator--(int) {
			reverse_iterator tmp = *this;
			++this->current;
			return ( tmp );
		}

		reverse_iterator	operator+(difference_type n) const	{ return reverse_iterator(this->current - n); }
		reverse_iterator	operator-(difference_type n) const	{ return reverse_iterator(this->current + n); }
		reverse_iterator&	operator+=(difference_type n) {
			this->current -= n;
			return ( *this );
		}
		reverse_iterator&	operator-=(difference_type n) {
			this->current += n;
			return ( *this );
		}
		
		reference			operator[](difference_type n) const { return ( this->current[-n-1] ); }
	};

	template <class Iterator> bool operator==( const reverse_iterator<Iterator>& x, const reverse_iterator<Iterator>& y) { return (x.base() == y.base()); }
	template <class Iterator> bool operator< ( const reverse_iterator<Iterator>& x, const reverse_iterator<Iterator>& y) { return (x.base() > y.base()); }
	template <class Iterator> bool operator!=( const reverse_iterator<Iterator>& x, const reverse_iterator<Iterator>& y) { return !(x == y); }	
	template <class Iterator> bool operator> ( const reverse_iterator<Iterator>& x, const reverse_iterator<Iterator>& y) {	return (y < x); }	
	template <class Iterator> bool operator>=( const reverse_iterator<Iterator>& x, const reverse_iterator<Iterator>& y) { return !(x < y); }	
	template <class Iterator> bool operator<=( const reverse_iterator<Iterator>& x, const reverse_iterator<Iterator>& y) { return !(y < x); }

	template <class IteratorL, class IteratorR> bool operator==( const reverse_iterator<IteratorL>& x, const reverse_iterator<IteratorR>& y) { return (x.base() == y.base()); }
	template <class IteratorL, class IteratorR> bool operator< ( const reverse_iterator<IteratorL>& x, const reverse_iterator<IteratorR>& y) { return (x.base() > y.base()); }
	template <class IteratorL, class IteratorR> bool operator!=( const reverse_iterator<IteratorL>& x, const reverse_iterator<IteratorR>& y) { return !(x == y); }	
	template <class IteratorL, class IteratorR> bool operator> ( const reverse_iterator<IteratorL>& x, const reverse_iterator<IteratorR>& y) {	return (y < x); }	
	template <class IteratorL, class IteratorR> bool operator>=( const reverse_iterator<IteratorL>& x, const reverse_iterator<IteratorR>& y) { return !(x < y); }	
	template <class IteratorL, class IteratorR> bool operator<=( const reverse_iterator<IteratorL>& x, const reverse_iterator<IteratorR>& y) { return !(y < x); }

	template <class Iterator>
	typename reverse_iterator<Iterator>::difference_type	operator-(
			const reverse_iterator<Iterator>& x,
			const reverse_iterator<Iterator>& y) {
		return (y.base() - x.base());
	}

	template <class IteratorL, class IteratorR>
	typename reverse_iterator<IteratorL>::difference_type operator-(
			const reverse_iterator<IteratorL>& x,
			const reverse_iterator<IteratorR>& y) {
		return (y.base() - x.base());
	}

	template <class Iterator>
	reverse_iterator<Iterator> operator+(
			typename reverse_iterator<Iterator>::difference_type n, 
			const reverse_iterator<Iterator>& x) {
		return ( reverse_iterator<Iterator>(x.base() - n));
	}
}

#endif
