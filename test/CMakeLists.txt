#dd_definitions(-D)
#message()
#${CMAKE_CURRENT_LIST_DIR}
#string(REGEX REPLACE "\\.h" "" HEADER ${HEADER})

# TODO: auto search tests
add_executable(pomelotest
    ${TEST_FOLDER}/Test.cpp
    ${TEST_FOLDER}/PerformanceTest.cpp
    ${TEST_FOLDER}/UtilTest.cpp
    ${TEST_FOLDER}/TimerTest.cpp
    ${TEST_FOLDER}/LoggerTest.cpp
    ${TEST_FOLDER}/ObjectPoolTest.cpp
    ${TEST_FOLDER}/ECSTest.cpp
    ${TEST_FOLDER}/SandboxTest.cpp
    ${TEST_FOLDER}/main.cpp
)

target_link_libraries(pomelotest
    pomelo
)
