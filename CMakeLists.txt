cmake_minimum_required (VERSION 3.19)

project ("SkipList")

# Add source to this project's executable.
add_executable (${PROJECT_NAME}  "SkipList.h" "UnitTest.cpp" "SkipList.hpp")
add_executable (${PROJECT_NAME}_Bench  "SkipList.h" "Benchmark.cpp" "SkipList.hpp")

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_compile_features(${PROJECT_NAME}_Bench PUBLIC cxx_std_20)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/9741c42200b66abc708ee6da269a29c8bd912cee.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
  benchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG        e451e50e9b8af453f076dec10bd6890847f1624e
)
FetchContent_MakeAvailable(benchmark)

target_link_libraries(${PROJECT_NAME} 
PRIVATE GTest::gmock_main)

target_link_libraries(${PROJECT_NAME}_Bench
PRIVATE benchmark::benchmark_main)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})