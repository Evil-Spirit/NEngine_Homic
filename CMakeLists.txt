cmake_minimum_required(VERSION 3.16)
project(NEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

add_executable(NEngine WIN32
  Execute.cpp
  res.rc
)

target_include_directories(NEngine PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(NEngine PRIVATE
  WIN32
  _WINDOWS
  NDEBUG
  _CRT_SECURE_NO_WARNINGS
)

target_link_libraries(NEngine PRIVATE
  opengl32
  glu32
  comctl32
  winmm
  comdlg32
  advapi32
  shell32
  ole32
  user32
  gdi32
)

set_target_properties(NEngine PROPERTIES
  OUTPUT_NAME "NEngine"
)

foreach(dir Textures MDS MDA Weapons Mats)
  add_custom_command(TARGET NEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/${dir}
            $<TARGET_FILE_DIR:NEngine>/${dir}
    COMMENT "Copying ${dir} next to NEngine.exe"
  )
endforeach()
