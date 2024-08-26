# Install script for directory: /home/chordata/Documents/GitHub/iqtree2

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
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

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/iqtree2" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/iqtree2")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/iqtree2"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/iqtree2")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/iqtree2" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/iqtree2")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/iqtree2"
         OLD_RPATH "/home/chordata/Documents/GitHub/iqtree2/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/iqtree2")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("/home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/CMakeFiles/iqtree2.dir/install-cxx-module-bmi-Debug.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE FILE FILES "/home/chordata/Documents/GitHub/iqtree2/example/models.nex")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE FILE FILES "/home/chordata/Documents/GitHub/iqtree2/example/example.phy")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE FILE FILES "/home/chordata/Documents/GitHub/iqtree2/example/example.nex")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE FILE FILES "/home/chordata/Documents/GitHub/iqtree2/example/example.cf")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/booster/cmake_install.cmake")
  include("/home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/pll/cmake_install.cmake")
  include("/home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/ncl/cmake_install.cmake")
  include("/home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/nclextra/cmake_install.cmake")
  include("/home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/utils/cmake_install.cmake")
  include("/home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/pda/cmake_install.cmake")
  include("/home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/lbfgsb/cmake_install.cmake")
  include("/home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/whtest/cmake_install.cmake")
  include("/home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/sprng/cmake_install.cmake")
  include("/home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/vectorclass/cmake_install.cmake")
  include("/home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/model/cmake_install.cmake")
  include("/home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/gsl/cmake_install.cmake")
  include("/home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/alignment/cmake_install.cmake")
  include("/home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/tree/cmake_install.cmake")
  include("/home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/terrace/cmake_install.cmake")
  include("/home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/simulator/cmake_install.cmake")
  include("/home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/main/cmake_install.cmake")
  include("/home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/yaml-cpp/cmake_install.cmake")
  include("/home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/phylo-yaml/cmake_install.cmake")
  include("/home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/terraphast/cmake_install.cmake")
  include("/home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/terracetphast/cmake_install.cmake")
  include("/home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/lsd2/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/chordata/Documents/GitHub/iqtree2/cmake-build-debug/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
