CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(WD ${CMAKE_CURRENT_LIST_DIR}/)
SET(SERVER_TARGET server)

SET(${SERVER_TARGET}_SOURCE_FILES
	${CMAKE_CURRENT_LIST_FILE}
	${WD}Server.cpp
	${WD}ServerUpdateManager.cpp
	${WD}WorldSimulator.cpp
	${WD}ServerFactoryManifest.h
	${WD}Server.h
	${WD}ServerPhysicsEntity.h
	${WD}ServerPlayerEntity.h
	${WD}ServerStaticEntity.h
	${WD}ServerUpdateManager.h
	${WD}WorldSimulator.h
)

SOURCE_GROUP("" FILES 
	${CMAKE_CURRENT_LIST_FILE}
	${WD}Server.h
	${WD}Server.cpp
)

SOURCE_GROUP("Systems" FILES 
	${WD}ServerUpdateManager.h
	${WD}ServerUpdateManager.cpp
	${WD}WorldSimulator.h
	${WD}WorldSimulator.cpp
)

SOURCE_GROUP("Game Objects" FILES 
	${WD}ServerFactoryManifest.h
)

SOURCE_GROUP("Game Objects\\Entities" FILES 
	${WD}ServerPhysicsEntity.h
	${WD}ServerPlayerEntity.h
	${WD}ServerStaticEntity.h
)

SOURCE_GROUP("Game Objects\\Geometry" FILES 
	${WD}ServerStaticGeometry.h
)

ADD_LIBRARY(${SERVER_TARGET} SHARED ${${SERVER_TARGET}_SOURCE_FILES})

TARGET_LINK_LIBRARIES(${SERVER_TARGET} kernel32)
TARGET_LINK_LIBRARIES(${SERVER_TARGET} user32)
TARGET_LINK_LIBRARIES(${SERVER_TARGET} gdi32)
TARGET_LINK_LIBRARIES(${SERVER_TARGET} winspool)
TARGET_LINK_LIBRARIES(${SERVER_TARGET} shell32)
TARGET_LINK_LIBRARIES(${SERVER_TARGET} ole32)
TARGET_LINK_LIBRARIES(${SERVER_TARGET} oleaut32)
TARGET_LINK_LIBRARIES(${SERVER_TARGET} uuid)
TARGET_LINK_LIBRARIES(${SERVER_TARGET} comdlg32)
TARGET_LINK_LIBRARIES(${SERVER_TARGET} advapi32)

TARGET_LINK_LIBRARIES(${SERVER_TARGET} zlib/zlib)

TARGET_LINK_LIBRARIES(${SERVER_TARGET} shared/shared)

TARGET_LINK_LIBRARIES(${SERVER_TARGET} optimized box2d/box2d)
TARGET_LINK_LIBRARIES(${SERVER_TARGET} debug     box2d/box2d-d)

TARGET_LINK_LIBRARIES(${SERVER_TARGET} optimized sfml/sfml-system-s)
TARGET_LINK_LIBRARIES(${SERVER_TARGET} debug     sfml/sfml-system-s-d)

TARGET_LINK_LIBRARIES(${SERVER_TARGET} optimized sfml/sfml-network-s)
TARGET_LINK_LIBRARIES(${SERVER_TARGET} debug     sfml/sfml-network-s-d)

TARGET_LINK_LIBRARIES(${SERVER_TARGET} optimized jsoncpp/jsoncpp)
TARGET_LINK_LIBRARIES(${SERVER_TARGET} debug     jsoncpp/jsoncpp-d)

TARGET_LINK_LIBRARIES(${SERVER_TARGET} optimized stlplus/portability)
TARGET_LINK_LIBRARIES(${SERVER_TARGET} debug     stlplus/portability-d)

SET_TARGET_PROPERTIES(${SERVER_TARGET}
	PROPERTIES
	COMPILE_FLAGS "/FIshared/Debug.h"
	COMPILE_DEFINITIONS SFML_STATIC
	
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG   "${WD}../../public/importlibs/server"
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${WD}../../public/importlibs/server"
	
	RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${WD}../../../game/bin"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${WD}../../../game/bin"
)

ADD_DEPENDENCIES(${SERVER_TARGET} ${SHARED_TARGET})