/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_tests.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbeylot <sbeylot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 10:48:05 by sbeylot           #+#    #+#             */
/*   Updated: 2023/03/05 12:27:14 by sbeylot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <map>
#include <vector>
#include "map.hpp"
#include "vector.hpp"

#ifndef STD
#	define DEBUG_NAMESPACE "ft"
#	define NAMESPACE ft
#else
#	define DEBUG_NAMESPACE "std"
#	define NAMESPACE std
#endif

#define	NEWLINE std::cout << std::endl;
typedef NAMESPACE::map<int, int> iiMap;
typedef NAMESPACE::map<int, std::string> isMap;
typedef NAMESPACE::map<char, int> ciMap;


template <class U>
void	printMapContent(const U& map, bool content) {
	std::cout << "| Map Content |" << std::endl;
	std::cout 
		<< "| Size: " << map.size()
		<< " - Empty: " << map.empty() 
		<< " - Max Size: " << map.max_size()
		<< std::endl;
	if (content) {
		std::cout << "| Content " << std::endl;
		for(typename U::const_iterator it = map.begin(); it != map.end(); it++) {
			std::cout << "| -Key: " << it->first << " - " << it->second << std::endl;
		}
		std::cout << "| Content " << std::endl;
	}
	std::cout << "------------------------" << std::endl;
}

void	InsertEraseTest(void) {
	std::cout << "[ Insert Erase Test ] " << std::endl;
	std::cout << "--------------------- " << std::endl;

	iiMap mp;
	printMapContent(mp, false);
	std::cout << "| Insert 10 000 element || Value = key * 3 |" << std::endl;
	for (int i = 0; i < 10000; i++) {
		mp[i] = i * 3;
	}
	printMapContent(mp, false);
	std::cout << "| Insert half of mp into mp2 |" << std::endl;
	iiMap::const_iterator cit = mp.find(mp.size() / 2); 
	iiMap::const_iterator cite = mp.end();

	std::cout << "| cit : key: " << cit->first << " - " << cit->second << std::endl;
	iiMap mp2;
	mp2.insert(cit, cite);
	printMapContent(mp2, false);

	std::cout << "| Equal Range key 5 |" << std::endl;
	NAMESPACE::pair<iiMap::iterator, iiMap::iterator> equalRange = mp.equal_range(0);
	std::cout << "| Lower Bound: " << equalRange.first->first << " - " << equalRange.first->second << " |" << std::endl;
	std::cout << "| Upper Bound: " << equalRange.second->first << " - " << equalRange.second->second << " |" << std::endl;
	std::cout << "| Insert in mp2 |" << std::endl;
	mp2.insert(equalRange.first, equalRange.second);
	printMapContent(mp2, false);


	std::cout << "| Clear mp and mp2 |" << std::endl;
	mp.clear();
	mp2.clear();
	printMapContent(mp, true);
	printMapContent(mp, true);

	std::cout << "| Insert 10 elements |" << std::endl;
	for (int i = 1; i <= 10; i++) {
		mp.insert(NAMESPACE::pair<int, int>(i, i * 10));
	}
	printMapContent(mp, true);
	std::cout << "| Erase begin and end |" << std::endl;
	mp.erase(mp.begin());
	mp.erase(--(mp.end()));
	printMapContent(mp, true);

	iiMap::iterator it = ++(++(mp.begin()));
	iiMap::iterator ite = --(--(--(mp.end())));
	std::cout << "| Erase key : " << it->first << " to " << ite->first << std::endl;

	mp.erase(it, ite);
	printMapContent(mp, true);
}

void	Construction_test(void) {
	std::cout << "[ Construction, Diff, Accessor Test ] " << std::endl;
	std::cout << "--------------------------- " << std::endl;

	isMap mp;
	mp[0] = "Simon";
	mp[1] = "Pascaline";
	mp[40] = "Rebelle";
	mp[21] = "Tana Tupaia";
	mp[1000] = "Muse Tanguent";

	printMapContent(mp, true);
	std::cout << "| Copy Constructor | " << std::endl;
	isMap mpCopy(mp);
	printMapContent(mpCopy, true);

	NEWLINE;

	std::cout << "mp == mpCopy: " << (mp == mpCopy) << std::endl;
	std::cout << "mp != mpCopy: " << (mp != mpCopy) << std::endl;
	std::cout << "mp < mpCopy: " << (mp <  mpCopy) << std::endl;
	std::cout << "mp > mpCopy: " << (mp >  mpCopy) << std::endl;
	std::cout << "mp <= mpCopy: " << (mp <= mpCopy) << std::endl;
	std::cout << "mp >= mpCopy: " << (mp >= mpCopy) << std::endl;

	std::cout << "| Adding One element to mpCopy |" << std::endl;
	mpCopy.insert(NAMESPACE::pair<int, std::string>(49, "New Element"));

	std::cout << "mp == mpCopy: " << (mp == mpCopy) << std::endl;
	std::cout << "mp != mpCopy: " << (mp != mpCopy) << std::endl;
	std::cout << "mp < mpCopy: " << (mp <  mpCopy) << std::endl;
	std::cout << "mp > mpCopy: " << (mp >  mpCopy) << std::endl;
	std::cout << "mp <= mpCopy: " << (mp <= mpCopy) << std::endl;
	std::cout << "mp >= mpCopy: " << (mp >= mpCopy) << std::endl;

	NEWLINE;

	std::cout << "| Iterator compare |" << std::endl;
	isMap::iterator compareIt = mp.begin();
	isMap::const_iterator compareIt2 = mp.begin();

	std::cout << "| mp.begin == mp.begin() const |" << std::endl;
	std::cout << (compareIt == compareIt2) << std::endl;

	std::cout << "| mp.begin == mp.end() const |" << std::endl;
	compareIt2 = mp.end();
	std::cout << (compareIt == compareIt2) << std::endl;



	std::cout << "| Reverse It Construction |" << std::endl;
	isMap::reverse_iterator rit = mpCopy.rbegin();
	isMap::reverse_iterator rite = mpCopy.rend();
	isMap rmpCopy(rit, rite);
	printMapContent(rmpCopy, true);

	std::cout << "| Comp begin with 40 |" << std::endl;
	std::cout << (mp.key_comp()(mp.begin()->first, mp.find(40)->first)) << std::endl;

	std::cout << "| [] operator and at() |" << std::endl;
	std::cout << "mp[0]: " << mp[0] << std::endl;
	std::cout << "at(40): " << mp.at(40) << std::endl;
	try {
		std::cout << "at(42)" << mp.at(42) << std::endl;
	}
	catch (std::out_of_range& e) {
		std::cout << "Error, out of range" << e.what() << std::endl;
	}

	std::cout << "| Swap mp and rmpCopy |" << std::endl;
	mp.swap(rmpCopy);
	printMapContent(mp, true);
	printMapContent(rmpCopy, true);
}

int main(void) {

	std::cout << std::boolalpha;
	InsertEraseTest();
	Construction_test();

	return (0);
}
