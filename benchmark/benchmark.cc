#include <math.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <stdio.h>

#include "openssl/bn.h"

#include "audit/client/block_tagger.h"
#include "audit/client/prf.h"
#include "audit/client/file_tag.h"
#include "audit/util.h"

int time(double size, std::istream& file) {
  clock_t t;
  audit::CryptoNumberGenerator g;
  audit::BN_ptr p{BN_new(), ::BN_free};
  if (!BN_generate_prime_ex(p.get(), 1024, 0, NULL, NULL, NULL)) {
    return -1;
  }

  std::unique_ptr<audit::PRF> prf{new audit::HMACPRF{
      "afewf32432GrggwGWQGOKgejo23hr43goregh ihroihofiewhfo "
      "ihwoiehoegirhge;h"}};

  t = clock();
  audit::FileTag f{file, 5, 128, std::move(p), &g};
  audit::BlockTagger block_tagger{f, std::move(prf)};
  while (block_tagger.HasNext()) {
    auto tag = block_tagger.GetNext();
  }
  t = clock() - t;
  auto sec = ((float)t) / CLOCKS_PER_SEC;
  std::cout << size / 1000 / 1000 << " MBs: " << sec << " sec" << std::endl;
  return 0;
}

int main(int argc, char** argv) {
  std::string seed{"oewihoaihfoiqdhg;ierh;oifeh"};
  RAND_seed(seed.c_str(), seed.size());

  double int_size = 4;
  for (int i = 4; i < 7; ++i) {
    double size = pow(10, 3 + i);
    {
      std::ofstream out;
      out.open("test_file");
      auto written = 0;
      auto i = 0;
      while (written < size) {
        out << i;
        i = (i + 1) % 10;
        written += int_size;
      }
    }
    std::ifstream file{"test_file", std::ifstream::binary};
    time(size, file);
    remove("test_file");
  }
}
