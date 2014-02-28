project(octarine)

add_executable(${PROJECT_NAME} ../src/ovm.c)

target_link_libraries(${PROJECT_NAME} pthread)

# include(projects/cmake/incLLVM.cmake)
# set(SOURCE_DIR src)
# include(projects/cmake/globfiles.cmake)

# # The _EXPORTING definition is for windows dll export support
# add_definitions(-D_EXPORTING)
# add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_h} ${${PROJECT_NAME}_c})

# if(APPLE)
# find_library (CF_LIBRARY CoreFoundation)
# mark_as_advanced (CF_LIBRARY)
# set (EXTRA_LIBS ${CF_LIBRARY})
# target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS})
# endif()

# llvm_map_components_to_libraries(REQ_LLVM_LIBRARIES jit native)

# target_link_libraries(${PROJECT_NAME} platform ${REQ_LLVM_LIBRARIES})
