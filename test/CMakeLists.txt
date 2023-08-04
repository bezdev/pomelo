file(GLOB TEST_FILES "${TEST_FOLDER}/*")
file(GLOB EXCLUDED_FILES "${TEST_FOLDER}/results")
list(REMOVE_ITEM TEST_FILES ${EXCLUDED_FILES})

###
# file(GLOB EXCLUDED_FILES "${TEST_FOLDER}/PerformanceTest.cpp")
# list(REMOVE_ITEM TEST_FILES ${EXCLUDED_FILES})
###

add_executable(pomelotest ${TEST_FILES})

target_link_libraries(
    pomelotest
    pomelo
)
