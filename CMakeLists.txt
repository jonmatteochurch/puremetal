#
# PureMetal - A simple program for pure metal phase field simulations.
# Copyright (C) 2017  Jon Matteo Church scjmc@leeds.ac.uk
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

cmake_minimum_required(VERSION 3.1)
project(pure_metal)

set ( CMAKE_CXX_STANDARD 11 )
set ( CMAKE_CXX_STANDARD_REQUIRED ON )

if ( CMAKE_COMPILER_IS_GNUCXX )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-literal-suffix -Wcast-align -Wnoexcept -Wredundant-decls -Wlogical-op -Wstrict-null-sentinel -Wstrict-overflow=2 -Wswitch-default -Wmissing-format-attribute -Wnormalized=nfc -Wstack-protector -Winline -Wunsafe-loop-optimizations -Wuseless-cast" )
endif ()
if ( ${CMAKE_CXX_COMPILER} MATCHES "ic.*" )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -no-ansi-alias -Wundef -Wcast-qual -Wzero-as-null-pointer-constant -Wshadow -Wconversion -Wsign-conversion -Wabi" )
endif ()

set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -Wdisabled-optimization -Wformat=2 -Winit-self -Wmissing-declarations -Wmissing-include-dirs -Woverloaded-virtual -Wsign-promo -Werror -Wno-unused -Winvalid-pch -Wno-long-long -Wstrict-aliasing=3" )
set ( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG -O3" )

# find_package ( MPI ) # VTK dependancy
# if ( MPI_FOUND )
#   include_directories ( ${MPI_INCLUDE_PATH} )
# endif ()

#find_package( Qt5Widgets ) # VTK dependancy

find_package ( GSL REQUIRED )
include_directories(${GSL_INCLUDE_DIRS})

find_package ( VTK 6.1 COMPONENTS vtkIOXML NO_MODULE )
include(${VTK_USE_FILE})

add_executable(pure_metal src/main.cpp src/approximation.cpp src/csplineinterpolant.cpp src/field.cpp src/messages.cpp src/options.cpp src/polynomialinterpolant.cpp src/postprocessor.cpp src/simulation.cpp src/specifications.cpp src/vtkfile.cpp )

target_link_libraries( pure_metal ${GSL_LIBRARIES} )
target_link_libraries(pure_metal ${VTK_LIBRARIES})

# target_link_libraries( pure_metal ${MPI_LIBRARIES} )

# if ( MPI_COMPILE_FLAGS )
#    set_target_properties( pure_metal PROPERTIES COMPILE_FLAGS "${MPI_COMPILE_FLAGS}" )
# endif ()
# 
# if ( MPI_LINK_FLAGS )
#     set_target_properties ( pure_metal PROPERTIES LINK_FLAGS "${MPI_LINK_FLAGS}" )
# endif ()

install(TARGETS pure_metal RUNTIME DESTINATION bin)
