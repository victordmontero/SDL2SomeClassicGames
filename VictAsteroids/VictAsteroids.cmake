function(project_VictAsteroids_Debug_Linux)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
  set(CMAKE_BUILD_TYPE RelWithDebInfo)

  add_definitions(
    -DDEBUG
    -DLINUX
  )

  set(SRC 
    src/Main.cpp
  )
  add_executable( VictAsteroids_Debug_Linux ${SRC})
  set_target_properties( VictAsteroids_Debug_Linux 
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "/home/phenom/Proyectos/SDL2SomeClassicGames/bin/Linux/VictAsteroids/Debug"
    LIBRARY_OUTPUT_DIRECTORY "/home/phenom/Proyectos/SDL2SomeClassicGames/bin/Linux/VictAsteroids/Debug"
    RUNTIME_OUTPUT_DIRECTORY "/home/phenom/Proyectos/SDL2SomeClassicGames/bin/Linux/VictAsteroids/Debug"
    OUTPUT_NAME  "VictAsteroids"
  )

  set(LIBS 
    SDL2main
    SDL2
    SDL2_image
    SDL2_ttf
  )
  target_link_libraries(VictAsteroids_Debug_Linux ${LIBS})
endfunction(project_VictAsteroids_Debug_Linux)
project_VictAsteroids_Debug_Linux()

function(project_VictAsteroids_Debug_Windows)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
  set(CMAKE_BUILD_TYPE RelWithDebInfo)

  add_definitions(
    -DDEBUG
  )

  set(INCLUD_DIRS 
    ../libs/SDL2_image-2.0.1/i686-w64-mingw32/include/SDL2
    ../libs/SDL2-2.0.5/i686-w64-mingw32/include/SDL2
    ../libs/SDL2_ttf-2.0.14/i686-w64-mingw32/include/SDL2
  )
  include_directories(${INCLUD_DIRS})

  set(LIB_DIRS
    ../libs/SDL2_image-2.0.1/i686-w64-mingw32/lib
    ../libs/SDL2-2.0.5/i686-w64-mingw32/lib
    ../libs/SDL2_ttf-2.0.14/i686-w64-mingw32/lib
  )
  link_directories(${LIB_DIRS})

  set(SRC 
    src/Main.cpp
  )
  add_executable( VictAsteroids_Debug_Windows ${SRC})
  set_target_properties( VictAsteroids_Debug_Windows 
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "/home/phenom/Proyectos/SDL2SomeClassicGames/bin/Windows/VictAsteroids/Debug"
    LIBRARY_OUTPUT_DIRECTORY "/home/phenom/Proyectos/SDL2SomeClassicGames/bin/Windows/VictAsteroids/Debug"
    RUNTIME_OUTPUT_DIRECTORY "/home/phenom/Proyectos/SDL2SomeClassicGames/bin/Windows/VictAsteroids/Debug"
    OUTPUT_NAME  "VictAsteroids"
  )

  set(LIBS 
    Mingw32
    SDL2main
    SDL2
    SDL2_image
    SDL2_ttf
  )
  target_link_libraries(VictAsteroids_Debug_Windows ${LIBS})
endfunction(project_VictAsteroids_Debug_Windows)
project_VictAsteroids_Debug_Windows()

function(project_VictAsteroids_Release_Linux)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
  set(CMAKE_BUILD_TYPE RelWithDebInfo)

  add_definitions(
    -DNDEBUG
    -DLINUX
  )

  set(SRC 
    src/Main.cpp
  )
  add_executable( VictAsteroids_Release_Linux ${SRC})
  set_target_properties( VictAsteroids_Release_Linux 
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "/home/phenom/Proyectos/SDL2SomeClassicGames/bin/Linux/VictAsteroids/Release"
    LIBRARY_OUTPUT_DIRECTORY "/home/phenom/Proyectos/SDL2SomeClassicGames/bin/Linux/VictAsteroids/Release"
    RUNTIME_OUTPUT_DIRECTORY "/home/phenom/Proyectos/SDL2SomeClassicGames/bin/Linux/VictAsteroids/Release"
    OUTPUT_NAME  "VictAsteroids"
  )

  set(LIBS 
    SDL2main
    SDL2
    SDL2_image
    SDL2_ttf
  )
  target_link_libraries(VictAsteroids_Release_Linux ${LIBS})
endfunction(project_VictAsteroids_Release_Linux)
project_VictAsteroids_Release_Linux()

function(project_VictAsteroids_Release_Windows)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
  set(CMAKE_BUILD_TYPE RelWithDebInfo)

  add_definitions(
    -DNDEBUG
  )

  set(INCLUD_DIRS 
    ../libs/SDL2_image-2.0.1/i686-w64-mingw32/include/SDL2
    ../libs/SDL2-2.0.5/i686-w64-mingw32/include/SDL2
    ../libs/SDL2_ttf-2.0.14/i686-w64-mingw32/include/SDL2
  )
  include_directories(${INCLUD_DIRS})

  set(LIB_DIRS
    ../libs/SDL2_image-2.0.1/i686-w64-mingw32/lib
    ../libs/SDL2-2.0.5/i686-w64-mingw32/lib
    ../libs/SDL2_ttf-2.0.14/i686-w64-mingw32/lib
  )
  link_directories(${LIB_DIRS})

  set(SRC 
    src/Main.cpp
  )
  add_executable( VictAsteroids_Release_Windows ${SRC})
  set_target_properties( VictAsteroids_Release_Windows 
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "/home/phenom/Proyectos/SDL2SomeClassicGames/bin/Windows/VictAsteroids/Release"
    LIBRARY_OUTPUT_DIRECTORY "/home/phenom/Proyectos/SDL2SomeClassicGames/bin/Windows/VictAsteroids/Release"
    RUNTIME_OUTPUT_DIRECTORY "/home/phenom/Proyectos/SDL2SomeClassicGames/bin/Windows/VictAsteroids/Release"
    OUTPUT_NAME  "VictAsteroids"
  )

  set(LIBS 
    Mingw32
    SDL2main
    SDL2
    SDL2_image
    SDL2_ttf
  )
  target_link_libraries(VictAsteroids_Release_Windows ${LIBS})
endfunction(project_VictAsteroids_Release_Windows)
project_VictAsteroids_Release_Windows()
