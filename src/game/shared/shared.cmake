CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(WD ${CMAKE_CURRENT_LIST_DIR}/)
SET(SHARED_TARGET shared)

INCLUDE_DIRECTORIES(
	${WD}../../public/headers/shared
)

SET(${SHARED_TARGET}_SOURCE_FILES
	${WD}BaseGameObject.cpp
	${WD}Compressor.cpp
	${WD}GameObjectFactoryHolder.cpp
	${WD}Manifest.cpp
	${WD}StreamSerialiser.cpp
	${WD}TcpCommunicator.cpp
	${WD}UdpCommunicator.cpp
	${WD}WorldManager.cpp
	${WD}../../public/headers/shared/BaseGameObject.h
	${WD}../../public/headers/shared/BasePlayerEntity.h
	${WD}../../public/headers/shared/BaseRenderedGeometry.h
	${WD}../../public/headers/shared/BaseSimulatedGeometry.h
	${WD}../../public/headers/shared/Communicator.h
	${WD}../../public/headers/shared/Compressor.h
	${WD}../../public/headers/shared/Debug.h
	${WD}../../public/headers/shared/GameObjectFactory.h
	${WD}../../public/headers/shared/GameObjectFactoryHolder.h
	${WD}../../public/headers/shared/INetworked.h
	${WD}../../public/headers/shared/IRendered.h
	${WD}../../public/headers/shared/ISimulated.h
	${WD}../../public/headers/shared/Key.h
	${WD}../../public/headers/shared/Manifest.h
	${WD}../../public/headers/shared/BaseSimulatedEntity.h
	${WD}../../public/headers/shared/Point.h
	${WD}../../public/headers/shared/Polygon.h
	${WD}../../public/headers/shared/GameState.h
	${WD}../../public/headers/shared/StreamSerialiser.h
	${WD}../../public/headers/shared/UpdateManager.h
	${WD}../../public/headers/shared/TcpCommunicator.h
	${WD}../../public/headers/shared/UdpCommunicator.h
	${WD}../../public/headers/shared/Vector.h
	${WD}../../public/headers/shared/WorldManager.h
)

SOURCE_GROUP("Data" FILES 
	${WD}../../public/headers/shared/Point.h
	${WD}../../public/headers/shared/Vector.h
	${WD}../../public/headers/shared/Key.h
	${WD}../../public/headers/shared/Polygon.h
)

SOURCE_GROUP("Game Objects\\BaseGeometry" FILES 
	${WD}../../public/headers/shared/BaseSimulatedGeometry.h
	${WD}../../public/headers/shared/BaseRenderedGeometry.h
)

SOURCE_GROUP("Game Objects\\Behaviours" FILES 
	${WD}../../public/headers/shared/ISimulated.h
	${WD}../../public/headers/shared/IRendered.h
	${WD}../../public/headers/shared/INetworked.h
	${WD}../../public/headers/shared/IProcessed.h
)

SOURCE_GROUP("Game Objects\\BaseEntities" FILES 
	${WD}../../public/headers/shared/BasePlayerEntity.h
	${WD}../../public/headers/shared/BaseSimulatedEntity.h
)

SOURCE_GROUP("Systems" FILES 
	${WD}WorldManager.cpp
	${WD}GameObjectFactoryHolder.cpp
	${WD}../../public/headers/shared/UpdateManager.h
	${WD}../../public/headers/shared/WorldManager.h
	${WD}../../public/headers/shared/GameObjectFactory.h
	${WD}../../public/headers/shared/GameObjectFactoryHolder.h
)

SOURCE_GROUP("Utilities" FILES 
	${WD}Manifest.cpp
	${WD}StreamSerialiser.cpp
	${WD}Compressor.cpp
	${WD}../../public/headers/shared/StreamSerialiser.h
	${WD}../../public/headers/shared/Manifest.h
	${WD}../../public/headers/shared/Debug.h
	${WD}../../public/headers/shared/Compressor.h
	${WD}../../public/headers/shared/GameState.h
)

SOURCE_GROUP("Game Objects" FILES 
	${WD}BaseGameObject.cpp
	${WD}../../public/headers/shared/BaseGameObject.h
)

SOURCE_GROUP("Network" FILES 
	${WD}UdpCommunicator.cpp
	${WD}TcpCommunicator.cpp
	${WD}../../public/headers/shared/TcpCommunicator.h
	${WD}../../public/headers/shared/UdpCommunicator.h
	${WD}../../public/headers/shared/Communicator.h
)

ADD_LIBRARY(${SHARED_TARGET} STATIC ${${SHARED_TARGET}_SOURCE_FILES})

SET_TARGET_PROPERTIES(${SHARED_TARGET}
	PROPERTIES
	COMPILE_FLAGS "/FIDebug.h"
	COMPILE_DEFINITIONS SFML_STATIC
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG   "${WD}../../public/staticlibs/shared"
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${WD}../../public/staticlibs/shared"
)