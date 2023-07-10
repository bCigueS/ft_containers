/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_tests.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbeylot <sbeylot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 14:00:44 by sbeylot           #+#    #+#             */
/*   Updated: 2023/02/28 12:16:14 by sbeylot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <stack>
#include "stack.hpp"
#include "vector.hpp"

#ifndef STD
# define DEBUG_NAMESPACE "ft"
# define NAMESPACE ft
#else
# define DEBUG_NAMESPACE "std"
# define NAMESPACE std
#endif

#define NEWLINE std::cout << std::endl;
typedef NAMESPACE::stack<int> intStack;
typedef NAMESPACE::stack<std::string> stringStack;
typedef NAMESPACE::stack<intStack> stackStack;

template <class U, class V>
void	isStackEqual(const U& stackA, const V& stackB) {
	std::cout << "[ Stack Comp Equal ]" << std::endl;
	std::cout << "| Equal: " << (stackA == stackB) << std::endl;
}

template <class U>
void	printStack(U stack) {
	std::cout << "| Stack Content |" << std::endl;
	std::cout << "| Size: " << stack.size() << std::endl;
	std::cout << "| Empty: " << stack.empty() << std::endl;
	while (stack.size()) {
		std::cout << "| -" << stack.top() << std::endl;
		stack.pop();
	}
	std::cout << "-----------------" << std::endl;
}

int main(void) {

	std::cout << std::boolalpha;
	std::cout << "DEBUG NAMESPACE: " << DEBUG_NAMESPACE << std::endl;

	{
		intStack st;
		st.push(42);
		st.push(32);
		st.push(22);
		st.push(12);

		intStack stCopy(st);
		std::cout << "[ Copy Construction INT ]" << std::endl;
		NEWLINE;	
		isStackEqual(st, stCopy);
		NEWLINE;	
		std::cout << "st content:" << std::endl;
		printStack(st);

		std::cout << "stCopy content:" << std::endl;
		printStack(st);
	}

	{
		stringStack st;
		st.push("Simon");
		st.push("Rebelle");
		st.push("Pascaline");
		st.push("Tana Tupaia");

		stringStack stCopy(st);
		std::cout << "[ Copy Construction STRING ]" << std::endl;
		NEWLINE;	
		isStackEqual(st, stCopy);
		NEWLINE;	
		std::cout << "st content:" << std::endl;
		printStack(st);

		std::cout << "stCopy content:" << std::endl;
		printStack(st);

		std::cout << "Adding a element in st and equal Compare" << std::endl;
		st.push("Nop");
		NEWLINE;
		isStackEqual(st, stCopy);
	}

	{
		intStack st;
		st.push(42);
		st.push(32);
		st.push(22);
		st.push(12);

		intStack stCopy;
		stCopy.push(2);
		stCopy.push(3);
		stCopy.push(4);
		stCopy.push(5);

		std::cout << "[ Inception of Stack ] " << std::endl;
		stackStack inception;
		inception.push(st);
		inception.push(stCopy);
		std::cout << "First Content -> stCopy [ 5 - 3 - 3 - 2 ]" << std::endl;
		printStack(inception.top());
		inception.pop();

		std::cout << "Second Content -> st [ 12 - 22 - 32 - 42 ]" << std::endl;
		printStack(inception.top());
		inception.pop();

	}


	return (0);
}
