set(target BabaIsAgent)

file(GLOB_RECURSE sources
	${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

add_executable(${target} ${sources})
target_link_libraries(${target} baba-is-auto)

if(TensorRT_FOUND)
	target_include_directories(${target} PUBLIC ${CUDA_INCLUDE_DIRS})
	target_link_libraries(${target} TensorRT::TensorRT ${CUDA_LIBRARIES})
endif()
