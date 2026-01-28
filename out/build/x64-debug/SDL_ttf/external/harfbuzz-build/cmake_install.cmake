# Install script for directory: D:/Coding/C++/Chess/SDL_ttf/external/harfbuzz

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "D:/Coding/C++/Chess/out/install/x64-debug")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/harfbuzz" TYPE FILE FILES
    "D:/Coding/C++/Chess/SDL_ttf/external/harfbuzz/src/hb-aat-layout.h"
    "D:/Coding/C++/Chess/SDL_ttf/external/harfbuzz/src/hb-aat.h"
    "D:/Coding/C++/Chess/SDL_ttf/external/harfbuzz/src/hb-blob.h"
    "D:/Coding/C++/Chess/SDL_ttf/external/harfbuzz/src/hb-buffer.h"
    "D:/Coding/C++/Chess/SDL_ttf/external/harfbuzz/src/hb-common.h"
    "D:/Coding/C++/Chess/SDL_ttf/external/harfbuzz/src/hb-cplusplus.hh"
    "D:/Coding/C++/Chess/SDL_ttf/external/harfbuzz/src/hb-deprecated.h"
    "D:/Coding/C++/Chess/SDL_ttf/external/harfbuzz/src/hb-draw.h"
    "D:/Coding/C++/Chess/SDL_ttf/external/harfbuzz/src/hb-face.h"
    "D:/Coding/C++/Chess/SDL_ttf/external/harfbuzz/src/hb-font.h"
    "D:/Coding/C++/Chess/SDL_ttf/external/harfbuzz/src/hb-map.h"
    "D:/Coding/C++/Chess/SDL_ttf/external/harfbuzz/src/hb-ot-color.h"
    "D:/Coding/C++/Chess/SDL_ttf/external/harfbuzz/src/hb-ot-deprecated.h"
    "D:/Coding/C++/Chess/SDL_ttf/external/harfbuzz/src/hb-ot-font.h"
    "D:/Coding/C++/Chess/SDL_ttf/external/harfbuzz/src/hb-ot-layout.h"
    "D:/Coding/C++/Chess/SDL_ttf/external/harfbuzz/src/hb-ot-math.h"
    "D:/Coding/C++/Chess/SDL_ttf/external/harfbuzz/src/hb-ot-meta.h"
    "D:/Coding/C++/Chess/SDL_ttf/external/harfbuzz/src/hb-ot-metrics.h"
    "D:/Coding/C++/Chess/SDL_ttf/external/harfbuzz/src/hb-ot-name.h"
    "D:/Coding/C++/Chess/SDL_ttf/external/harfbuzz/src/hb-ot-shape.h"
    "D:/Coding/C++/Chess/SDL_ttf/external/harfbuzz/src/hb-ot-var.h"
    "D:/Coding/C++/Chess/SDL_ttf/external/harfbuzz/src/hb-ot.h"
    "D:/Coding/C++/Chess/SDL_ttf/external/harfbuzz/src/hb-paint.h"
    "D:/Coding/C++/Chess/SDL_ttf/external/harfbuzz/src/hb-set.h"
    "D:/Coding/C++/Chess/SDL_ttf/external/harfbuzz/src/hb-shape-plan.h"
    "D:/Coding/C++/Chess/SDL_ttf/external/harfbuzz/src/hb-shape.h"
    "D:/Coding/C++/Chess/SDL_ttf/external/harfbuzz/src/hb-style.h"
    "D:/Coding/C++/Chess/SDL_ttf/external/harfbuzz/src/hb-unicode.h"
    "D:/Coding/C++/Chess/SDL_ttf/external/harfbuzz/src/hb-version.h"
    "D:/Coding/C++/Chess/SDL_ttf/external/harfbuzz/src/hb.h"
    "D:/Coding/C++/Chess/SDL_ttf/external/harfbuzz/src/hb-ft.h"
    "D:/Coding/C++/Chess/SDL_ttf/external/harfbuzz/src/hb-gdi.h"
    "D:/Coding/C++/Chess/SDL_ttf/external/harfbuzz/src/hb-uniscribe.h"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "D:/Coding/C++/Chess/out/build/x64-debug/SDL_ttf/external/harfbuzz-build/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
