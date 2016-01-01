#include <math.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>

#include "openssl/bn.h"

#include "audit/common.h"
#include "audit/client/prf.h"
#include "audit/client/upload/block_tagger.h"
#include "audit/client/upload/file.h"
#include "audit/util.h"

#define NDEBUG

int time(double size, std::istream& file) {
  clock_t t;
  audit::CryptoNumberGenerator g;
  audit::BN_ptr p{BN_new(), ::BN_free};
  if (!BN_generate_prime_ex(p.get(), 1024, 0, NULL, NULL, NULL)) {
    return -1;
  }

  std::unique_ptr<audit::PRF> prf{new audit::HMACPRF};

  std::vector<audit::BN_ptr> alphas;
  for (int i = 0; i < 5; ++i) {
    alphas.push_back(g.GenerateNumber(*p));
  }

  t = clock();

  audit::upload::FileContext context{
      audit::upload::File{file, ""}, audit::upload::TaggingParameters{5, 128},
      std::move(alphas), std::move(p), std::move(prf)};

  audit::upload::BlockTagger block_tagger{context};

  while (block_tagger.HasNext()) {
    auto tag = block_tagger.GetNext();
  }
  t = clock() - t;
  auto sec = ((float)t) / CLOCKS_PER_SEC;
  std::cout << size / 1000 / 1000 << " MBs: " << sec << " sec" << std::endl;
  return 0;
}

double size(std::ifstream& file) {
  file.seekg(0, file.end);
  auto length = file.tellg();
  file.seekg(0, file.beg);
  return length;
}

int main(int argc, char** argv) {
  std::string seed{"oewihoaihfoiqdhg;ierh;oifeh"};
  // RAND_seed(seed.c_str(), seed.size());

  std::vector<std::string> files{
      "/Applications/iMovie.app/Contents/Frameworks/"
      "StudioSharedResources.framework/Versions/A/Resources/"
      "AppThemeBitsB.car",
      "/Applications/Firefox.app/Contents/MacOS/XUL",  // 150 MB
      "/Applications/BioShock3.app/Contents/GameData/XGame/Movies/"
      "BioshockInfinite_Credits.bik",  // 370 MB
      "/Applications/BioShock3.app/Contents/GameData/DLC/DLCC/"
      "CookedPCConsole_FR/WorldTextures0_DLCC.tfc",
  };

  double int_size = 4;
  for (auto file_name : files) {
    std::ifstream file;
    file.open(file_name, std::ifstream::binary);
    time(size(file), file);
    remove("test_file");
  }
}
