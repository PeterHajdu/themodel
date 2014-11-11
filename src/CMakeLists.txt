
set(SOURCE_FILES
  lua.cpp
  helpers.cpp
    )

add_library(themodel STATIC ${SOURCE_FILES})
target_link_libraries(themodel ${LIBS})
install(TARGETS themodel DESTINATION lib)

