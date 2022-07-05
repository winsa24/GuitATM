
cmake_minimum_required(VERSION 3.0)
project(myapp)
set(CMAKE_BUILD_TYPE Debug)

message("-- Project: " ${PROJECT_NAME} " (" ${CMAKE_SYSTEM_NAME} " " ${CMAKE_BUILD_TYPE} "mode)")

add_executable(${PROJECT_NAME} 
  myapp.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
overview
tutorial
examples/headers
../guit/native 
../guit/core
../guit/gui 
../guit/util 
../guit/gadgets 
../guit/props
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

  target_compile_options(${PROJECT_NAME} PUBLIC -std=c++17 -Wall)
  target_link_options(${PROJECT_NAME} PUBLIC -std=c++17)
  target_link_directories(${PROJECT_NAME} PUBLIC ${SDL2_LIB_DIR})
  target_link_libraries(${PROJECT_NAME}
    guit
    SDL2
    SDL2_image
    SDL2_ttf
    "-framework OpenGL"
    "-framework Carbon"
    "-framework Cocoa"
    "-iframework /System/Library/PrivateFrameworks -framework MultitouchSupport"
  )

#    /opt/local/lib/libSDL2.dylib
#    /opt/local/lib/libSDL2_image.dylib
#    /opt/local/lib/libSDL2_ttf.dylib
	
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

  target_compile_options(${PROJECT_NAME} PUBLIC -std=c++17 -Wall)
  target_link_options(${PROJECT_NAME} PUBLIC -std=c++17)
  target_link_libraries(${PROJECT_NAME}
    guit
    SDL2
    SDL2_image
    SDL2_ttf
    GL
    GLU
    pthread
    stdc++fs
  )

elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")

  target_compile_options(${PROJECT_NAME} PUBLIC /std:c++17 /W1)
  target_link_options(${PROJECT_NAME} PUBLIC)
  target_link_directories(${PROJECT_NAME} PUBLIC ${SDL2_LIB_DIR})
  target_link_libraries(${PROJECT_NAME}
    guit
    SDL2
    SDL2_image
    SDL2_ttf
    ${OPENGL_LIBRARIES}
  )

endif()

set(GUIT_APP_DIR ${GUIT_DIR}/${PROJECT_NAME})
message("-- Installing Resource Files in: " ${GUIT_APP_DIR})

file(GLOB FILES *.css resources/*.png)

foreach(f ${FILES})
  file(COPY ${f}  DESTINATION ${GUIT_APP_DIR})
endforeach(f)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
foreach(f ${FILES})
  file(COPY ${f}  DESTINATION ${GUIT_DIR}/examples)
endforeach()
endif()


