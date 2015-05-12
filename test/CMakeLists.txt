# required for igloo:
add_definitions(-Wno-deprecated-declarations)

set(TEST_SOURCE_FILES
    test_runner.cpp
    test_node.cpp
    test_lua.cpp
    test_helpers.cpp
    test_variable.cpp
    test_function.cpp
    test_node_list.cpp
    test_model_tree.cpp
    test_json_exporter.cpp
    test_zmq_remote.cpp
    test_hash.cpp
    test_observer.cpp
    )


add_executable(test_runner EXCLUDE_FROM_ALL ${TEST_SOURCE_FILES})
target_link_libraries(test_runner themodel lua zmq pthread ${LIBS})

get_target_property(TEST_RUNNER_BIN test_runner LOCATION) # for running

find_program( VALGRIND_PATH valgrind )

if(VALGRIND_PATH)
  set(RUNNER
    valgrind --leak-check=full --show-reachable=yes --log-file="valgrind.log"
    )
endif()

add_custom_target(test DEPENDS test_runner)
add_custom_command(TARGET test COMMAND ${RUNNER} ${TEST_RUNNER_BIN} --verbose DEPENDS test_runner)

