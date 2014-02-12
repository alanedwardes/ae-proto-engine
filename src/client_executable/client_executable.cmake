CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(WD ${CMAKE_CURRENT_LIST_DIR}/)
SET(CLIENT_EXECUTABLE_TARGET client_executable)

ADD_EXECUTABLE(${CLIENT_EXECUTABLE_TARGET} ${WD}main.cpp)

TARGET_LINK_LIBRARIES(${CLIENT_EXECUTABLE_TARGET} client/client)

SET_TARGET_PROPERTIES(${CLIENT_EXECUTABLE_TARGET}
	PROPERTIES	
	RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${WD}../../game/bin"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${WD}../../game/bin"
)

ADD_DEPENDENCIES(${CLIENT_EXECUTABLE_TARGET} ${SHARED_TARGET} ${CLIENT_TARGET})