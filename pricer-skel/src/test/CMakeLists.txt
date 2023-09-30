include(FetchContent)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(TEST_FILES
        BasketOptionTest.cpp
        ../MonteCarlo.cpp
        ../BlackScholesModel.cpp
        AsianOptionTest.cpp
        BlackScholesModelTest.cpp
        MonteCarloTest.cpp
        PerformanceOptionTest.cpp)

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(Tests_run ${TEST_FILES})
target_link_libraries(Tests_run gtest_main)
target_link_libraries(Tests_run Library)
target_link_libraries(Tests_run ${LIBS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")