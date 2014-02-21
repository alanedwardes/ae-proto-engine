CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

INCLUDE(${CMAKE_CURRENT_LIST_DIR}/../shared/shared.cmake)

SET(WD ${CMAKE_CURRENT_LIST_DIR}/)
SET(CLIENT_TARGET client)

SET(${CLIENT_TARGET}_SOURCE_FILES
	${CMAKE_CURRENT_LIST_FILE}
	${WD}Client.cpp
	${WD}ClientUpdateManager.cpp
	${WD}WorldRenderer.cpp
	${WD}Client.h
	${WD}ClientPhysicsEntity.h
	${WD}ClientPlayerEntity.h
	${WD}ClientUpdateManager.h
	${WD}ClientFactoryManifest.h
	${WD}InputManager.h
	${WD}WorldRenderer.h
	${WD}View.h
	${WD}Button.h
	${WD}Label.h
	${WD}Input.h
	${WD}SFMLDrawing.h
	${WD}WindowManager.h
	${WD}WindowManager.cpp
	${WD}UIManager.h
)

SOURCE_GROUP("" FILES 
	${CMAKE_CURRENT_LIST_FILE}
	${WD}Client.h
	${WD}Client.cpp
)

SOURCE_GROUP("UI" FILES
	${WD}View.h
	${WD}Button.h
	${WD}Label.h
	${WD}Input.h
)

SOURCE_GROUP("Services" FILES 
	${WD}SFMLDrawing.h
)

SOURCE_GROUP("Systems" FILES
	${WD}ClientUpdateManager.h
	${WD}ClientUpdateManager.cpp
	${WD}InputManager.h
	${WD}WorldRenderer.h
	${WD}WorldRenderer.cpp
	${WD}WindowManager.h
	${WD}WindowManager.cpp
	${WD}UIManager.h
)

SOURCE_GROUP("Game Objects" FILES 
	${WD}ClientFactoryManifest.h
)

SOURCE_GROUP("Game Objects\\Entities" FILES 
	${WD}ClientPhysicsEntity.h
	${WD}ClientPlayerEntity.h
)

SOURCE_GROUP("Game Objects\\Geometry" FILES 

)

ADD_LIBRARY(${CLIENT_TARGET} SHARED ${${CLIENT_TARGET}_SOURCE_FILES} ${SHARED_SOURCE_FILES})

TARGET_LINK_LIBRARIES(${CLIENT_TARGET} kernel32)
TARGET_LINK_LIBRARIES(${CLIENT_TARGET} user32)
TARGET_LINK_LIBRARIES(${CLIENT_TARGET} gdi32)
TARGET_LINK_LIBRARIES(${CLIENT_TARGET} winspool)
TARGET_LINK_LIBRARIES(${CLIENT_TARGET} shell32)
TARGET_LINK_LIBRARIES(${CLIENT_TARGET} ole32)
TARGET_LINK_LIBRARIES(${CLIENT_TARGET} oleaut32)
TARGET_LINK_LIBRARIES(${CLIENT_TARGET} uuid)
TARGET_LINK_LIBRARIES(${CLIENT_TARGET} comdlg32)
TARGET_LINK_LIBRARIES(${CLIENT_TARGET} advapi32)
TARGET_LINK_LIBRARIES(${CLIENT_TARGET} opengl32)
TARGET_LINK_LIBRARIES(${CLIENT_TARGET} glu32)

TARGET_LINK_LIBRARIES(${CLIENT_TARGET} zlib/zlib)

TARGET_LINK_LIBRARIES(${CLIENT_TARGET} optimized sfml/sfml-system-s)
TARGET_LINK_LIBRARIES(${CLIENT_TARGET} debug     sfml/sfml-system-s-d)

TARGET_LINK_LIBRARIES(${CLIENT_TARGET} optimized sfml/sfml-window-s)
TARGET_LINK_LIBRARIES(${CLIENT_TARGET} debug     sfml/sfml-window-s-d)

TARGET_LINK_LIBRARIES(${CLIENT_TARGET} optimized sfml/sfml-network-s)
TARGET_LINK_LIBRARIES(${CLIENT_TARGET} debug     sfml/sfml-network-s-d)

TARGET_LINK_LIBRARIES(${CLIENT_TARGET} optimized sfml/sfml-graphics-s)
TARGET_LINK_LIBRARIES(${CLIENT_TARGET} debug     sfml/sfml-graphics-s-d)

TARGET_LINK_LIBRARIES(${CLIENT_TARGET} optimized sfml/sfml-audio-s)
TARGET_LINK_LIBRARIES(${CLIENT_TARGET} debug     sfml/sfml-audio-s-d)

TARGET_LINK_LIBRARIES(${CLIENT_TARGET} optimized jsoncpp/jsoncpp)
TARGET_LINK_LIBRARIES(${CLIENT_TARGET} debug     jsoncpp/jsoncpp-d)

TARGET_LINK_LIBRARIES(${CLIENT_TARGET} optimized stlplus/portability)
TARGET_LINK_LIBRARIES(${CLIENT_TARGET} debug     stlplus/portability-d)

ADD_DEFINITIONS(-DSFML_STATIC)
ADD_DEFINITIONS(-DCLIENT)

SET_TARGET_PROPERTIES(${CLIENT_TARGET}
	PROPERTIES
	COMPILE_FLAGS "/FIDebug.h"
	COMPILE_DEFINITIONS SFML_STATIC
	
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG   "${WD}../../public/importlibs/client"
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${WD}../../public/importlibs/client"
	
	RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${WD}../../../game/bin"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${WD}../../../game/bin"
)