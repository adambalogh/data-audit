#include <math.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>

#include "openssl/bn.h"

#include "audit/client/block_tagger.h"
#include "audit/client/prf.h"
#include "audit/client/file_tag.h"
#include "audit/util.h"

#define NDEBUG

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

double size(std::ifstream& file) {
  file.seekg(0, file.end);
  auto length = file.tellg();
  file.seekg(0, file.beg);
  return length;
}

int main(int argc, char** argv) {
  std::string seed{"oewihoaihfoiqdhg;ierh;oifeh"};
  RAND_seed(seed.c_str(), seed.size());

  std::vector<std::string> files{
      "/Applications/iMovie.app/Contents/Frameworks/"
      "StudioSharedResources.framework/Versions/A/Resources/AppThemeBitsB.car",
      "/Applications/Firefox.app/Contents/MacOS/XUL",  // 150 MB
      "/Applications/BioShock3.app/Contents/GameData/XGame/Movies/"
      "BioshockInfinite_Credits.bik",  // 370 MB
      "/Applications/BioShock3.app/Contents/GameData/DLC/DLCC/"
      "CookedPCConsole_FR/WorldTextures0_DLCC.tfc",
  };

  double int_size = 4;
  for (auto file_name : files) {
    std::ifstream file;
    // char buf[1000 * 1000];
    // file.rdbuf()->pubsetbuf(buf, sizeof buf);
    file.open(file_name, std::ifstream::binary);

    time(size(file), file);
    remove("test_file");
  }
}
