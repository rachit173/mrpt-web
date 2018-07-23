set(LIB_SRC
ex.cpp
)

add_library(rpc_sample ${LIB_SRC})

add_executable(rpc-app-sample
	main.cpp
)
target_link_libraries(rpc_sample
PUBLIC
jsoncpp_lib_static
mrpt-web
)
target_link_libraries(rpc-app-sample
PUBLIC
rpc_sample
${CMAKE_THREAD_LIBS_INIT}
mrpt-web
jsoncpp_lib_static
Boost::system
)
