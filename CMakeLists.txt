cmake_minimum_required(VERSION 2.8)

project(Singularity)

#
# Set the output directories
#

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/binary)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/binary)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/binary)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/binary)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/binary)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/binary)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/binary)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/binary)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/binary)

#
# Check compiler
#

if(CMAKE_COMPILER_IS_GNUCC)
  set(COMPILER_GCC TRUE)
elseif(CMAKE_C_COMPILER_ID MATCHES "Clang")
  set(COMPILER_CLANG TRUE)
elseif(MSVC_VERSION GREATER 1400) # VisualStudio 8.0+
  set(COMPILER_VISUAL_STUDIO TRUE)
endif()

#
# Set the XdevLSDK environment variables.
#

set(XDEVL_HOME $ENV{XDEVL_HOME})
set(XDEVL_PLUGINS $ENV{XDEVL_PLUGINS})

#
# Set all used sources.
#

set(SOURCES_FILES
	${CMAKE_SOURCE_DIR}/src/Singularity.cpp
	${CMAKE_SOURCE_DIR}/src/Engine/Camera.cpp
	${CMAKE_SOURCE_DIR}/src/Engine/Frustum.cpp
	${CMAKE_SOURCE_DIR}/src/Engine/GBuffer.cpp
	${CMAKE_SOURCE_DIR}/src/Engine/Light.cpp
	${CMAKE_SOURCE_DIR}/src/Engine/ProceduralSystem.cpp
	${CMAKE_SOURCE_DIR}/src/Engine/SkyBox.cpp
	${CMAKE_SOURCE_DIR}/src/Engine/SoanMotionState.cpp
	${CMAKE_SOURCE_DIR}/src/Engine/TextureServer.cpp
	${CMAKE_SOURCE_DIR}/src/Engine/Transformable.cpp
	${CMAKE_SOURCE_DIR}/src/Engine/Effects/GausBlur.cpp
	${CMAKE_SOURCE_DIR}/src/Engine/Effects/DepthOfField.cpp
	${CMAKE_SOURCE_DIR}/src/Engine/Effects/PostProcessEffect.cpp
	${CMAKE_SOURCE_DIR}/src/Engine/Effects/ShadowMap.cpp
	${CMAKE_SOURCE_DIR}/src/Engine/Effects/ToneMapping.cpp
	${CMAKE_SOURCE_DIR}/src/Engine/Entity/Material.cpp
	${CMAKE_SOURCE_DIR}/src/Engine/Entity/Mesh.cpp
	${CMAKE_SOURCE_DIR}/src/Engine/Entity/Model.cpp
	${CMAKE_SOURCE_DIR}/src/Engine/GUI/CanvasXdevLOpenGL.cpp
	${CMAKE_SOURCE_DIR}/src/Engine/Physics/Physics.cpp
	
	${CMAKE_SOURCE_DIR}/src/Game/Actor.cpp
	${CMAKE_SOURCE_DIR}/src/Game/Bullet.cpp
	${CMAKE_SOURCE_DIR}/src/Game/Renderable.cpp
	${CMAKE_SOURCE_DIR}/src/Game/SpaceShip.cpp
	${CMAKE_SOURCE_DIR}/src/Game/SpaceShipInputHandler.cpp
	${CMAKE_SOURCE_DIR}/src/Game/Universe/Astroid.cpp
	${CMAKE_SOURCE_DIR}/src/Game/Universe/CelestialObject.cpp
	${CMAKE_SOURCE_DIR}/src/Game/Universe/Comet.cpp
	${CMAKE_SOURCE_DIR}/src/Game/Universe/Planet.cpp
	${CMAKE_SOURCE_DIR}/src/Game/Universe/PlanetarySystem.cpp
	${CMAKE_SOURCE_DIR}/src/Game/Universe/Satellite.cpp
	${CMAKE_SOURCE_DIR}/src/Game/Universe/Star.cpp

	
	
	${CMAKE_SOURCE_DIR}/src/Utils/AssimpToModel.cpp
	${CMAKE_SOURCE_DIR}/src/Utils/FPSCounter.cpp
	${CMAKE_SOURCE_DIR}/src/Utils/glew.c
	${CMAKE_SOURCE_DIR}/src/Utils/SignedDistanceFieldGenerator.cpp
	${CMAKE_SOURCE_DIR}/src/Utils/SingularityDebugDrawer.cpp
	${CMAKE_SOURCE_DIR}/src/Utils/SplashScreen.cpp
)

if(COMPILER_GCC OR COMPILER_CLANG)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pedantic -Wall")
	set(CMAKE_CXX_STANDARD 11 )
elseif(COMPILER_VISUAL_STUDIO)
endif()

#
# Add used include directories
#

include_directories(
	${CMAKE_SOURCE_DIR}/src 
	${XDEVL_HOME}/include 
	${XDEVL_PLUGINS} 
	${XDEVL_PLUGINS}/XdevLRAI
	${CMAKE_SOURCE_DIR}/dependencies/bullet/include
	${CMAKE_SOURCE_DIR}/dependencies/assimp/include
	${CMAKE_SOURCE_DIR}/dependencies/freeimage/include
)

#
# Add used library directories
#

set(LINK_DIRECTORIES 
	${XDEVL_HOME}/lib
)

if(APPLE)
	set(LINK_DIRECTORIES ${LINK_DIRECTORIES}
		${CMAKE_SOURCE_DIR}/dependencies/bullet/lib/macosx 
		${CMAKE_SOURCE_DIR}/dependencies/assimp/lib/macosx
		${CMAKE_SOURCE_DIR}/dependencies/freeimage/lib/macosx
	)
elseif(UNIX)
	set(LINK_DIRECTORIES ${LINK_DIRECTORIES}
		${CMAKE_SOURCE_DIR}/dependencies/bullet/lib/linux 
		${CMAKE_SOURCE_DIR}/dependencies/assimp/lib/linux
		${CMAKE_SOURCE_DIR}/dependencies/freeimage/lib/linux
	)
endif()

link_directories( ${LINK_DIRECTORIES})

#
# Create the Singularity executable
#

add_executable(Singularity ${SOURCES_FILES})

#
# Set the used external libraries
#

set(LINK_LIBRARIES 
	XdevLd
	assimp
	freeimage
	BulletDynamics
	BulletCollision
	LinearMath
)

if(UNIX AND NOT APPLE)
	set(LINK_LIBRARIES ${LINK_LIBRARIES} GL pthread dl)
elseif(APPLE) 
	set(LINK_LIBRARIES ${LINK_LIBRARIES} "-framework OpenGL")
endif()

target_link_libraries(Singularity ${LINK_LIBRARIES})
