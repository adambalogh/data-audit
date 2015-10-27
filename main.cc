#include <iostream>
#include <sstream>
#include <fstream>

#include "file_tagger.h"

int main() {
  std::stringstream s{"aztwiowerihweriohwitrh"};
  audit::CryptoNumberGenerator g;
  audit::FileTagger t{s, 2, 1, CryptoPP::Integer{10424345432432}, g};

  while (t.HasNext()) {
    auto tag = t.GetNext();
    std::cout << tag.index << ": " << tag.sigma << std::endl;
  }
  auto file_tag = t.GetFileTag();
}
