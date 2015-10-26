#include <iostream>
#include <sstream>
#include <fstream>

#include "file_tagger.h"

#include "third_party/cryptopp/integer.h"

int main() {
  std::stringstream s{"hellokocsogerokoerkow"};
  audit::FileTagger f(s);
  auto tag = f.NextTag();
  std::cout << tag.sigma << std::endl;
}
