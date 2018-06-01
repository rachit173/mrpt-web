# jsonrpcstub modified for mrpt-web
macro(jsonrpcstub_create EXECUTABLE SPEC SERVERNAME SERVERDIR SERVERFILENAME)
	add_custom_target(${SPEC})
	add_custom_command(
		TARGET ${SPEC}
		POST_BUILD
		DEPENDS "{CMAKE_CURRENT_SOURCE_DIR}/${SPEC}"
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		COMMAND ${CMAKE_COMMAND} -DSPEC_PATH="${CMAKE_CURRENT_SOURCE_DIR}/${SPEC}" -DSOURCE_DIR="${CMAKE_SOURCE_DIR}"
		-DSERVER_DIR="${SERVERDIR}"
		-DSERVER_NAME=${SERVERNAME}
		-DSERVER_FILENAME=${SERVERFILENAME}
		-DJSON_RPC_STUB="${JSON_RPC_STUB}"
		-P "${CMAKE_CURRENT_SOURCE_DIR}/jsonrpcstub.cmake"
	)
	add_dependencies(${EXECUTABLE} ${SPEC})
endmacro()

file(GLOB SRCS "*.cpp" "*.h")

set(RAWLOG_SPEC_CLASSNAME CRPCRawLog)

add_executable(mrpt-ws-rpc
	${SRCS}
)

target_link_libraries(mrpt-ws-rpc
PUBLIC
${CMAKE_THREAD_LIBS_INIT}
mrpt-web
libjson-rpc-cpp::server
Boost::system
OpenSSL::SSL
OpenSSL::Crypto
${MRPT_LIBS})

jsonrpcstub_create(mrpt-ws-rpc CRPCRawLog.spec.json ${RAWLOG_SPEC_CLASSNAME}Abstract ${CMAKE_CURRENT_SOURCE_DIR} ${RAWLOG_SPEC_CLASSNAME}Face)