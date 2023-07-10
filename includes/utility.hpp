/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbeylot <sbeylot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 11:58:25 by sbeylot           #+#    #+#             */
/*   Updated: 2023/02/27 09:49:35 by sbeylot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILITY_HPP
#define UTILITY_HPP

#include "iterator_traits.hpp"
#include "reverse_iterator.hpp"

namespace ft {
	template < class Iterator>
	class reverse_iterator;

	template < class Iterator >
	struct iterator_traits;

	template <class T>
	struct iterator_traits<T*>;

	template <class T>
	struct iterator_traits<const T*>;

	//	FT::PAIR
	template<class T1, class T2>
	struct pair {
		typedef	T1		first_type;
		typedef T2		second_type;

		first_type	first;
		second_type	second;

		pair(void) : first(), second() { }

		template <class U, class V>
		pair(const pair<U, V>& pr) : first(pr.first), second(pr.second) {
		}

		pair(const pair& pr) : first(pr.first), second(pr.second) { }
		pair(const first_type& a, const second_type& b) : first(a), second(b) { }

		
		pair	&operator=(const pair& pr) {
			first = pr.first;
			second = pr.second;
			return *this;
		}

	};

	template <class T1, class T2>
	bool operator==(const ft::pair<T1, T2>& lhs, const ft::pair<T1, T2>& rhs) {
		return (lhs.first == rhs.first && rhs.second == rhs.second);
	}

	template <class T1, class T2>
	bool operator< (const ft::pair<T1, T2>& lhs, const ft::pair<T1, T2>& rhs) {
		if (lhs.first < rhs.first)
			return true;
		else if (rhs.first < lhs.first)
			return false;
		else if (lhs.second < rhs.second)
			return true;
		else
			return false;
	}

	template <class T1, class T2>
	bool operator!=(const ft::pair<T1, T2>& lhs, const ft::pair<T1, T2>& rhs) {
		return !(lhs == rhs);
	}

	template <class T1, class T2>
	bool operator> (const ft::pair<T1, T2>& lhs, const ft::pair<T1, T2>& rhs) {
		return (rhs < lhs);
	}

	template <class T1, class T2>
	bool operator<=(const ft::pair<T1, T2>& lhs, const ft::pair<T1, T2>& rhs) {
		return !(rhs < lhs);
	}

	template <class T1, class T2>
	bool operator>=(const ft::pair<T1, T2>& lhs, const ft::pair<T1, T2>& rhs) {
		return !(lhs < rhs);
	}



	template< class T1, class T2>
	ft::pair<T1, T2> make_pair(T1 t, T2 u) {
		ft::pair<T1, T2> test(t, u);
		return (test);
	}

	template <class InputIterator1, class InputIterator2>
	bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
		while (first1 != last1) {
			if (!(*first1 == *first2))
				return false;
			++first1;
			++first2;
		}
		return true;
	};

	template< class InputIt1, class InputIt2>
	bool	lexicographical_compare(InputIt1 first1, InputIt2 last1, InputIt2 first2, InputIt2 last2) {
		for(; (first1 != last1) && (first2 != last2); ++first1, (void)++first2)
		{
			if (*first1 < *first2)
				return true;
			if (*first2 < *first1)
				return false;
		}
		return (first1 == last1) && (first2 != last2);
	}

	template <class InputIt1, class InputIt2, class Compare>
	bool lexicographical_compare(InputIt1 first1, InputIt2 last1, InputIt2 first2, InputIt2 last2, Compare comp) {
		for (; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2)
		{
			if (comp(*first1, *first2))
				return true;
			if (comp(*first2, *first1))
				return false;
		}
		return (first1 == last1) && (first2 != last2);
	}

	template < class T, bool v>
	struct integral_constant {
		static const bool value = v;
		typedef T	value_type;
		typedef integral_constant type;
		operator value_type() const { return value; }
	};

	template <class T> struct is_integral							: public integral_constant<T, false> { };
	template <> struct is_integral<bool>							: public integral_constant<bool, true> { };
	template <> struct is_integral<char>							: public integral_constant<bool, true> { };
	template <> struct is_integral<wchar_t>							: public integral_constant<bool, true> { };
	template <> struct is_integral<signed char>						: public integral_constant<bool, true> { };
	template <> struct is_integral<short int>						: public integral_constant<bool, true> { };
	template <> struct is_integral<int>								: public integral_constant<bool, true> { };
	template <> struct is_integral<long int>						: public integral_constant<bool, true> { };
	template <> struct is_integral<long long int>					: public integral_constant<bool, true> { };
	template <> struct is_integral<unsigned char>					: public integral_constant<bool, true> { };
	template <> struct is_integral<unsigned short int>				: public integral_constant<bool, true> { };
	template <> struct is_integral<unsigned int>					: public integral_constant<bool, true> { };
	template <> struct is_integral<unsigned long int>				: public integral_constant<bool, true> { };
	template <> struct is_integral<unsigned long long int>			: public integral_constant<bool, true> { };

	template <bool Cond, class T = void> struct enable_if { };
	template <class T> struct enable_if<true, T> { typedef T type; };

}

#endif
