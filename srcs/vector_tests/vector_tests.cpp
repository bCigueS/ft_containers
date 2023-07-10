/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_tests.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbeylot <sbeylot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 10:08:32 by sbeylot           #+#    #+#             */
/*   Updated: 2023/03/05 12:27:15 by sbeylot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <algorithm>
#include <vector>
#include "vector.hpp"

#ifndef STD
#	define DEBUG_NAMESPACE "ft"
#	define NAMESPACE ft
#else
#	define DEBUG_NAMESPACE "std"
#	define NAMESPACE std
#endif

#define	NEWLINE std::cout << std::endl;
typedef NAMESPACE::vector<int> iVec;
typedef NAMESPACE::vector<std::string> sVec;
typedef NAMESPACE::vector<iVec> vVector;

template <class U>
void	printVecContent(const U& vector) {
	std::cout << "| Vector Content |" << std::endl;
	std::cout 
		<< "| Size: " << vector.size()
		<< " - Capacity: " << vector.capacity()
		<< " - Empty: " << vector.empty() 
		<< " - Max Size: " << vector.max_size()
		<< std::endl;
	std::cout << "| Content " << std::endl;
	if (vector.size() > 15) {
		int count = 0;
		std::cout << "| ";
		for(typename U::const_iterator it = vector.begin(); it != vector.end(); it++) {
			std::cout << "- " << *it << " ";
			count++;
			if (count % 10 == 0)
				std::cout << std::endl << "| ";
		}
		std::cout << " |" << std::endl;
	}
	else {
		for(typename U::const_iterator it = vector.begin(); it != vector.end(); it++) {
			std::cout << "| -" << *it;
		}
	}
	std::cout << "| Content " << std::endl;
	std::cout << "------------------------" << std::endl;
}

void	PushPop_test(void) {
		std::cout << "[ Push and Pop Test ] " << std::endl;
		std::cout << "--------------------- " << std::endl;

		iVec vc;
		vc.push_back(1);
		vc.push_back(2);
		vc.push_back(3);
		vc.push_back(4);
		std::cout << "| Push 1, 2, 3, 4 |" << std::endl;
		printVecContent(vc);
		std::cout << "| Push 5, 6, 7, 8, 9, 10, 42 |" << std::endl;
		vc.push_back(5);
		vc.push_back(6);
		vc.push_back(7);
		vc.push_back(8);
		vc.push_back(9);
		vc.push_back(10);
		vc.push_back(42);
		printVecContent(vc);
		std::cout << "| Pop 3 times |" << std::endl;
		vc.pop_back();
		vc.pop_back();
		vc.pop_back();
		printVecContent(vc);
}

void	ResizeReserve_test(void) {
	std::cout << "[ Resize and Reserve Test ] " << std::endl;
	std::cout << "--------------------------- " << std::endl;

	iVec vc;
	printVecContent(vc);
	std::cout << "| Reserve 10 spaces |" << std::endl;
	vc.reserve(10);
	printVecContent(vc);
	std::cout << "| Adding 2 elements |" << std::endl;
	vc.push_back(42);
	vc.push_back(52);
	printVecContent(vc);
	std::cout << "| Resize to 3 | " <<std::endl;
	vc.resize(3);
	printVecContent(vc);

	std::cout << "| Resize to 0 | " << std::endl;
	vc.resize(0);
	printVecContent(vc);

	std::cout << "| Resize to 5 and push one element | " << std::endl;
	vc.resize(5);
	vc.push_back(42);
	printVecContent(vc);

	std::cout << "| Length Error Catch | " << std::endl;
	try {
		vc.reserve(vc.max_size() + 1);
	}
	catch (std::length_error& le) {
		std::cout << le.what() << std::endl;
	}
	catch(std::exception &e) {
		std::cout << e.what() << std::endl;
	}
}

void	ClearErase_test(void) {
	std::cout << "[ Resize and Reserve Test ] " << std::endl;
	std::cout << "--------------------------- " << std::endl;

	iVec vc;
	for (size_t i = 0; i < 10; i++) {
		vc.push_back(i);
	}
	vc.insert(vc.begin(), 100);
	printVecContent(vc);
	std::cout << "| Erase begin() | " << std::endl;
	vc.erase(vc.begin());
	printVecContent(vc);
	std::cout << "| Erase 2 to 7 | " << std::endl; 
	iVec::iterator it = vc.begin();
	iVec::iterator ite = vc.end();
	++(++it);
	--(--ite);
	vc.erase(it, ite);
	printVecContent(vc);
	std::cout << "| Clear | " << std::endl;
	vc.clear();
	printVecContent(vc);
}

void	Insert_test(void) {
	std::cout << "[ Insert Test ] " << std::endl;
	std::cout << "--------------------------- " << std::endl;

	iVec vc(12, 12);
	iVec vc2(5, 5);
	
	vc.insert(vc.begin(), 200, 12);
	printVecContent(vc);
	vc.insert(vc.begin() + 12, 200, 30);
	printVecContent(vc);
	vc.insert(vc.end(), 12, 50);
	printVecContent(vc);
	vc.insert(vc.end() - 1, 0, 60);
	printVecContent(vc);
	vc.insert(vc.end() - 1, 1, 70);
	printVecContent(vc);
	vc.insert(vc.begin() + 412, vc2.begin(), vc2.end());
	printVecContent(vc);
	vc.insert(vc.begin() + 6, vc2.begin(), vc2.end());
	printVecContent(vc);
	vc.insert(vc.end(), vc2.begin(), vc2.end());
	printVecContent(vc);
}

void	Construction_test(void) {
	std::cout << "[ Construction, Diff, Accessor Test ] " << std::endl;
	std::cout << "--------------------------- " << std::endl;

	sVec vc;
	vc.push_back("Simon");
	vc.push_back("Pascaline");
	vc.push_back("Rebelle");
	vc.push_back("Tana Tupaia");
	vc.push_back("Muse Tanguent");

	printVecContent(vc);
	std::cout << "| Copy Constructor | " << std::endl;
	sVec vcCopy(vc);
	printVecContent(vcCopy);

	NEWLINE;

	std::cout << "Vc == VcCopy: " << (vc == vcCopy) << std::endl;
	std::cout << "Vc != VcCopy: " << (vc != vcCopy) << std::endl;
	std::cout << "Vc < VcCopy: " << (vc <  vcCopy) << std::endl;
	std::cout << "Vc > VcCopy: " << (vc >  vcCopy) << std::endl;
	std::cout << "Vc <= VcCopy: " << (vc <= vcCopy) << std::endl;
	std::cout << "Vc >= VcCopy: " << (vc >= vcCopy) << std::endl;

	std::cout << "| Adding One element to vcCopy |" << std::endl;
	vcCopy.push_back("OKOK");

	std::cout << "Vc == VcCopy: " << (vc == vcCopy) << std::endl;
	std::cout << "Vc != VcCopy: " << (vc != vcCopy) << std::endl;
	std::cout << "Vc < VcCopy: " << (vc <  vcCopy) << std::endl;
	std::cout << "Vc > VcCopy: " << (vc >  vcCopy) << std::endl;
	std::cout << "Vc <= VcCopy: " << (vc <= vcCopy) << std::endl;
	std::cout << "Vc >= VcCopy: " << (vc >= vcCopy) << std::endl;

	NEWLINE;

	std::cout << "| Reverse It Construction |" << std::endl;
	sVec::reverse_iterator rit = vcCopy.rbegin();
	sVec::reverse_iterator rite = vcCopy.rend();
	sVec rVcCopy(rit, rite);
	printVecContent(rVcCopy);

	std::cout << "| [] operator and at() |" << std::endl;
	std::cout << "vc[0]: " << vc[0] << std::endl;
	std::cout << "at(4): " << vc.at(4) << std::endl;

	std::cout << "| Swap vc and rVcCopy |" << std::endl;
	vc.swap(rVcCopy);
	printVecContent(vc);
	printVecContent(rVcCopy);
}

void	Algo_test(void) {
	std::cout << "[ Algo Test ] " << std::endl;
	std::cout << "--------------------------- " << std::endl;
	int myInts[] = {10, 20, 30, 40, 50, 60, 70};
	iVec vc(7);

	std::copy (myInts, myInts + 7, vc.begin());
	printVecContent(vc);

}

int main(void) {
	std::cout << std::boolalpha;
	std::cout << "DEBUG_NAMESPACE: " << DEBUG_NAMESPACE << std::endl;

	PushPop_test();
	ResizeReserve_test();
	ClearErase_test();
	Insert_test();
	Construction_test();
	Algo_test();




	return (0);
}
