cmake_minimum_required(VERSION 3.2)

project(Data-Audit)

set (CMAKE_CXX_STANDARD 14)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(/users/adambalogh/Developer)
include_directories(/usr/local/include)

link_directories(/usr/local/lib)

set(LIBRARIES server client local_disk cpor cryptopp crypto)
set(TEST_LIBRARIES gtest gtest_main test_util ${LIBRARIES})

enable_testing()

macro (create_test target target_file)
  add_executable(${target} ${target_file})
  target_link_libraries(${target} ${TEST_LIBRARIES})
  add_test (${target} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${target})
endmacro (create_test)

#add_library(dropbox
#  providers/dropbox/dropbox_client.cc
#  providers/dropbox/fetcher.cc
#  providers/dropbox/file_list_source.cc
#  providers/dropbox/file_tag_source.cc
#  providers/dropbox/storage.cc
#  providers/dropbox/token_source.cc
#  providers/dropbox/token_source_instance.cc
#)

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

# Tests

create_test(util_test util_test.cc)
create_test(block_tag_map_test block_tag_map_test.cc)
create_test(prf_test client/prf_test.cc)
create_test(progress_bar_test client/progress_bar_test.cc)
create_test(block_tag_serializer_test client/upload/block_tag_serializer_test.cc)
create_test(block_tagger_test client/upload/block_tagger_test.cc)
create_test(file_test client/upload/file_test.cc)
create_test(verify_file_test client/verify/verify_file_test.cc)
create_test(prover_test server/prover_test.cc)
