#include <iostream>
#include "crfpp.h"

// c++ -O3 example.cpp -lcrfpp

int main(int argc, char **argv)
{

	// -v 3: access deep information like alpha,beta,prob
    	// -nN: enable nbest output. N should be >= 2
	CRFPP::Tagger *tagger = CRFPP::createTagger("-m model -v 3 -n2");
	//  CRFPP::createTagger("-m model -v 3 -n2");
	

	if (!tagger) {
		std::cerr << CRFPP::getTaggerError() << std::endl;
		return -1;
	}

	// clear internal context
	tagger->clear();

	// add context
	tagger->add("B N");
	tagger->add("F V");
	tagger->add("A N");
	tagger->add(". D");
	tagger->add("B N");
	tagger->add("F V");
	tagger->add("C N");
	tagger->add(". D");
	tagger->add("H H");
	tagger->add("C N");
	tagger->add(". D");




	// parse and change internal stated as 'parsed'
	if (! tagger->parse()) return -1;


	for (size_t i = 0; i < tagger->size(); ++i) {
	//	for (size_t j = 0; j < tagger->xsize(); ++j) {
	//		std::cout << tagger->x(i, j) << '\t';
	//	}
		std::cout << tagger->y2(i) << '\t';
		std::cout << std::endl;

	}


	delete tagger;

	return 0;
}
