#include <iostream>
#include "crfpp.h"

// c++ -O3 example.cpp -lcrfpp

int main(int argc, char **argv) {

  // -v 3: access deep information like alpha,beta,prob
  // -nN: enable nbest output. N should be >= 2
  CRFPP::Tagger *tagger =
  //  CRFPP::createTagger("-m model -v 3 -n2");
  	CRFPP::createTagger("-m model -v 3 -n2");

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
  




  std::cout << "column size: " << tagger->xsize() << std::endl;
  std::cout << "token size: " << tagger->size() << std::endl;
  std::cout << "tag size: " << tagger->ysize() << std::endl;

  std::cout << "tagset information:" << std::endl;
  for (size_t i = 0; i < tagger->ysize(); ++i) {
    std::cout << "tag " << i << " " << tagger->yname(i) << std::endl;
  }

  // parse and change internal stated as 'parsed'
  if (! tagger->parse()) return -1;

  std::cout << "conditional prob=" << tagger->prob()
            << " log(Z)=" << tagger->Z() << std::endl;

  for (size_t i = 0; i < tagger->size(); ++i) {
    for (size_t j = 0; j < tagger->xsize(); ++j) {
      std::cout << tagger->x(i, j) << '\t';
    }
    std::cout << tagger->y2(i) << '\t';
    std::cout << std::endl;

    std::cout << "Details";
    for (size_t j = 0; j < tagger->ysize(); ++j) {
      std::cout << '\t' << tagger->yname(j) << "/prob=" << tagger->prob(i,j)
                << "/alpha=" << tagger->alpha(i, j)
                << "/beta=" << tagger->beta(i, j);
    }
    std::cout << std::endl;
  }

  // when -n20 is specified, you can access nbest outputs
  std::cout << "nbest outputs:" << std::endl;
  for (size_t n = 0; n < 10; ++n) {
    if (! tagger->next()) break;
    std::cout << "nbest n=" << n << "\tconditional prob=" << tagger->prob() << std::endl;
    // you can access any information using tagger->y()...
  }
  std::cout << "Done" << std::endl;

  delete tagger;

  return 0;
}
