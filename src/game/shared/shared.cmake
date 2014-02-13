CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(SHAREDWD ${CMAKE_CURRENT_LIST_DIR}/)

INCLUDE_DIRECTORIES(
	${SHAREDWD}
)

SET(SHARED_SOURCE_FILES
	${CMAKE_CURRENT_LIST_FILE}
	${SHAREDWD}BaseGameObject.cpp
	${SHAREDWD}Compressor.cpp
	${SHAREDWD}Manifest.cpp
	${SHAREDWD}StreamSerialiser.cpp
	${SHAREDWD}TcpCommunicator.cpp
	${SHAREDWD}UdpCommunicator.cpp
	${SHAREDWD}WorldManager.cpp
	${SHAREDWD}BaseGameObject.h
	${SHAREDWD}BasePlayerEntity.h
	${SHAREDWD}BaseRenderedGeometry.h
	${SHAREDWD}BaseSimulatedGeometry.h
	${SHAREDWD}Communicator.h
	${SHAREDWD}Compressor.h
	${SHAREDWD}Debug.h
	${SHAREDWD}GameObjectFactory.h
	${SHAREDWD}INetworked.h
	${SHAREDWD}IRendered.h
	${SHAREDWD}ISimulated.h
	${SHAREDWD}Key.h
	${SHAREDWD}Manifest.h
	${SHAREDWD}BaseSimulatedEntity.h
	${SHAREDWD}Point.h
	${SHAREDWD}Polygon.h
	${SHAREDWD}StreamSerialiser.h
	${SHAREDWD}UpdateManager.h
	${SHAREDWD}GameState.h
	${SHAREDWD}TcpCommunicator.h
	${SHAREDWD}UdpCommunicator.h
	${SHAREDWD}Vector.h
	${SHAREDWD}WorldManager.h
	${SHAREDWD}Locator.h
	${SHAREDWD}Locator.cpp
	${SHAREDWD}IWorldManager.h
	${SHAREDWD}IWorldSimulator.h
	${SHAREDWD}IInputManager.h
	${SHAREDWD}IGameState.h
	${SHAREDWD}IFactoryManifest.h
)

SOURCE_GROUP("Shared" FILES
	${CMAKE_CURRENT_LIST_FILE}
)

SOURCE_GROUP("Shared\\Data" FILES 
	${SHAREDWD}Point.h
	${SHAREDWD}Vector.h
	${SHAREDWD}Key.h
	${SHAREDWD}Polygon.h
)

SOURCE_GROUP("Shared\\Game Objects\\BaseGeometry" FILES 
	${SHAREDWD}BaseSimulatedGeometry.h
	${SHAREDWD}BaseRenderedGeometry.h
)

SOURCE_GROUP("Shared\\Game Objects\\Behaviours" FILES 
	${SHAREDWD}ISimulated.h
	${SHAREDWD}IRendered.h
	${SHAREDWD}INetworked.h
	${SHAREDWD}IProcessed.h
)

SOURCE_GROUP("Shared\\Game Objects\\BaseEntities" FILES 
	${SHAREDWD}BasePlayerEntity.h
	${SHAREDWD}BaseSimulatedEntity.h
)

SOURCE_GROUP("Shared\\Services" FILES
	${SHAREDWD}Locator.h
	${SHAREDWD}Locator.cpp
	${SHAREDWD}IWorldManager.h
	${SHAREDWD}IWorldSimulator.h
	${SHAREDWD}IGameState.h
	${SHAREDWD}IInputManager.h
	${SHAREDWD}IFactoryManifest.h
)

SOURCE_GROUP("Shared\\Systems" FILES 
	${SHAREDWD}WorldManager.cpp
	${SHAREDWD}UpdateManager.h
	${SHAREDWD}WorldManager.h
	${SHAREDWD}GameState.h
	${SHAREDWD}GameObjectFactory.h
)

SOURCE_GROUP("Shared\\Utilities" FILES 
	${SHAREDWD}Manifest.cpp
	${SHAREDWD}StreamSerialiser.cpp
	${SHAREDWD}Compressor.cpp
	${SHAREDWD}StreamSerialiser.h
	${SHAREDWD}Manifest.h
	${SHAREDWD}Debug.h
	${SHAREDWD}Compressor.h
	${SHAREDWD}GameState.h
)

SOURCE_GROUP("Shared\\Game Objects" FILES 
	${SHAREDWD}BaseGameObject.cpp
	${SHAREDWD}BaseGameObject.h
)

SOURCE_GROUP("Shared\\Network" FILES 
	${SHAREDWD}UdpCommunicator.cpp
	${SHAREDWD}TcpCommunicator.cpp
	${SHAREDWD}TcpCommunicator.h
	${SHAREDWD}UdpCommunicator.h
	${SHAREDWD}Communicator.h
)