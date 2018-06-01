set(SERVER_TMPFILE "${SERVER_DIR}/${SERVER_NAME}.h.tmp")
set(SERVER_OUTFILE "${SERVER_DIR}/${SERVER_FILENAME}.h")
execute_process(
    COMMAND ${JSON_RPC_STUB} ${SPEC_PATH}
    --cpp-server=${SERVER_NAME} --cpp-server-file=${SERVER_TMPFILE}
    OUTPUT_VARIABLE ERR ERROR_QUIET
)

file(READ ${SERVER_TMPFILE} SERVER_CONTENT)

string(REGEX REPLACE "include\ <jsonrpccpp/server.h>" "include\ <mrpt/web/CModularServer.h>" SERVER_CONTENT "${SERVER_CONTENT}")
string(REGEX REPLACE "public\ jsonrpc::AbstractServer<${SERVER_NAME}>" "public ServerInterface<${SERVER_NAME}>" SERVER_CONTENT "${SERVER_CONTENT}")
string(REGEX REPLACE "${SERVER_NAME}\\(jsonrpc::AbstractServerConnector\ &conn,\ jsonrpc::serverVersion_t\ type\ =\ jsonrpc::JSONRPC_SERVER_V2\\)\ :\ jsonrpc::AbstractServer<${SERVER_NAME}>\\(conn, type\\)" "${SERVER_NAME}()" SERVER_CONTENT "${SERVER_CONTENT}")

file(WRITE ${SERVER_OUTFILE} "${SERVER_CONTENT}")

if (ERR)
	message(WARNING "Your version of jsonrcpstub tool is not supported. Please upgrade it.")
	message(WARNING "${ERR}")
else()
	if (SERVER_DIR)
		file(REMOVE "${SERVER_TMPFILE}")
    endif()
endif()
