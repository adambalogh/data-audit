cmake_minimum_required(VERSION 3.4)
set (CMAKE_CXX_STANDARD 11)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(/users/adambalogh/Developer)
include_directories(/usr/local/include)

link_directories(/usr/local/lib)

set(LIBRARIES server client local_disk cpor cryptopp crypto)
set(TEST_LIBRARIES gtest gtest_main test_util ${LIBRARIES})

add_library(dropbox
  providers/dropbox/dropbox_client.cc
  providers/dropbox/fetcher.cc
  providers/dropbox/file_list_source.cc
  providers/dropbox/file_tag_source.cc
  providers/dropbox/storage.cc
  providers/dropbox/token_source.cc
  providers/dropbox/token_source_instance.cc
)

add_library(client
  util.cc
  client/prf.cc
  client/upload/block_tag_serializer.cc
  client/upload/block_tagger.cc
  client/upload/client.cc
  client/upload/file.cc
  client/upload/storage.cc
  client/verify/client.cc
  client/verify/proof_source.cc
  client/verify/verify_file.cc
)

add_library(server 
  util.cc
  server/prover.cc
)

add_library(local_disk
  providers/local_disk/fetcher.cc
  providers/local_disk/storage.cc
  providers/local_disk/file_tag_source.cc
  providers/local_disk/file_list_source.cc
)

add_library(cpor proto/cpor.pb.cc)
target_link_libraries(cpor protobuf)

add_library(files files/file_browser.cc)

add_library(test_util test_util.cc)

add_executable(benchmark_main benchmark/main.cc)
target_link_libraries(benchmark_main benchmark ${LIBRARIES})

add_executable(block_tag_map_test block_tag_map_test.cc)
target_link_libraries(block_tag_map_test ${TEST_LIBRARIES})

add_executable(prover_test server/prover_test.cc)
target_link_libraries(prover_test ${LIBRARIES})


