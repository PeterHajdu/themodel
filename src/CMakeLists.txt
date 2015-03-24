
set(SOURCE_FILES
  lua.cpp
  helpers.cpp
  node.cpp
  jsoncpp.cpp
  json_exporter.cpp
  zmq_remote.cpp
    )

add_library(themodel STATIC ${SOURCE_FILES})
target_link_libraries(themodel ${LIBS})
install(TARGETS themodel DESTINATION lib)

set(REMOTE_MODEL_SOURCE_FILES
  remote_model.cpp
  )

add_executable(the-model-remote ${REMOTE_MODEL_SOURCE_FILES})
target_link_libraries(the-model-remote themodel zmq pthread ${LIBS})
install(TARGETS the-model-remote DESTINATION bin)

