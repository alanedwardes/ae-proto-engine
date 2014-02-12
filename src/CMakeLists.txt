CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

CMAKE_POLICY(SET CMP0015 NEW)

PROJECT(ae-proto-engine)

INCLUDE_DIRECTORIES(
	public/headers
)

LINK_DIRECTORIES(
	public/staticlibs
	public/importlibs
)

INCLUDE(game/shared/shared.cmake)
INCLUDE(game/client/client.cmake)
INCLUDE(game/server/server.cmake)

INCLUDE(server_executable/server_executable.cmake)
INCLUDE(client_executable/client_executable.cmake)