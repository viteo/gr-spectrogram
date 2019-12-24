# Copyright 2011,2012,2016 Free Software Foundation, Inc.
#
# This file was generated by gr_modtool, a tool from the GNU Radio framework
# This file is a part of gr-spectrogram
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

########################################################################
# QT GUI file generation stuff (Q_OBJECT)
########################################################################
#set(QTGUI_MOC_HEADERS
#    ${GNURADIO_RUNTIME_INCLUDE_DIRS}/displayform.h
#    ${GNURADIO_RUNTIME_INCLUDE_DIRS}/waterfalldisplayform.h
#    ${GNURADIO_RUNTIME_INCLUDE_DIRS}/DisplayPlot.h
#    ${GNURADIO_RUNTIME_INCLUDE_DIRS}/WaterfallDisplayPlot.h    
#)
#QT4_WRAP_CPP(QTGUI_MOC_OUTFILES ${QTGUI_MOC_HEADERS})

########################################################################
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX
include(GrMiscUtils)

include_directories(    
    ${VOLK_INCLUDE_DIRS}    
    ${PYTHON_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIR}
    ${QT_INCLUDE_DIRS}
    ${QWT_INCLUDE_DIRS}
    )
link_directories(
    ${QWT_LIBRARY_DIRS}
    ${Boost_LIBRARY_DIRS}
    ${GNURADIO_ALL_LIBRARIES}
    )

list(APPEND spectrogram_sources
#    ${QTGUI_MOC_OUTFILES}
    waterfall_vector_sink_f_impl.cc
)

include(GrPython)
if(ENABLE_PYTHON)
	add_definitions(-DENABLE_PYTHON)
	list(APPEND qtgui_libs
			${PYTHON_LIBRARIES}
			)
endif(ENABLE_PYTHON)


set(spectrogram_sources "${spectrogram_sources}" PARENT_SCOPE)
if(NOT spectrogram_sources)
	MESSAGE(STATUS "No C++ sources... skipping lib/")
	return()
endif(NOT spectrogram_sources)

add_definitions(-DQWT_DLL) #setup QWT library linkage
add_library(gnuradio-spectrogram 
            SHARED ${spectrogram_sources})
target_link_libraries(gnuradio-spectrogram 
                    ${Boost_LIBRARIES} 
                    ${QWT_LIBRARIES} 
                    ${PYTHON_LIBRARIES} 
                    ${QT_LIBRARIES} 
                    ${GNURADIO_ALL_LIBRARIES})
set_target_properties(gnuradio-spectrogram PROPERTIES DEFINE_SYMBOL "gnuradio_spectrogram_EXPORTS")

if(APPLE)
    set_target_properties(gnuradio-spectrogram PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif(APPLE)

########################################################################
# Install built library files
########################################################################
GR_LIBRARY_FOO(gnuradio-spectrogram)

########################################################################
# Print summary
########################################################################
message(STATUS "Using install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Building for version: ${VERSION} / ${LIBVER}")
