add_executable(fametest
    ${TEST_FOLDER}/Test.cpp
    ${TEST_FOLDER}/PerformanceTest.cpp
    ${TEST_FOLDER}/UtilTest.cpp
    ${TEST_FOLDER}/main.cpp
)

target_link_libraries(fametest
    fame
)
