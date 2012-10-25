cmake_minimum_required(VERSION 2.8)

#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Wall")

FIND_PACKAGE(OpenGL)
FIND_PACKAGE(GLUT)

IF ( NOT GLUT_FOUND )
	IF ( MSVC )
		SET ( GLUT_FOUND 1 )
		SET ( GLUT_INCLUDE_DIR ${Assimp_SOURCE_DIR}/samples/glut/ )
		SET ( GLUT_LIBRARIES ${Assimp_SOURCE_DIR}/samples/glut/glut32.lib )
	ELSE ( MSVC )
		MESSAGE( WARNING "Please install glut." )
	ENDIF ( MSVC )
ENDIF ( NOT GLUT_FOUND )

SET ( DEVIL_INCLUDE_DIR /usr/include/IL/ )

INCLUDE_DIRECTORIES(
	${Assimp_SOURCE_DIR}/include
	${Assimp_SOURCE_DIR}/code
	${OPENGL_INCLUDE_DIR}
    ${DEVIL_INCLUDE_DIR}
    "/usr/include/IL/"
)

LINK_DIRECTORIES( 
	${Assimp_BINARY_DIR} 
	${Assimp_BINARY_DIR}/lib 
)

ADD_EXECUTABLE( loader3D
    glfwMain.cpp
)

SET_PROPERTY(TARGET loader3D PROPERTY DEBUG_POSTFIX ${ASSIMP_DEBUG_POSTFIX})

TARGET_LINK_LIBRARIES( loader3D assimp ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} -lIL -lglfw)
SET_TARGET_PROPERTIES( loader3D PROPERTIES
	OUTPUT_NAME loader3D
)

