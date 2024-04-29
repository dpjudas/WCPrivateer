cmake_minimum_required(VERSION 3.15)

project(WCPrivateer)

add_subdirectory(Libraries/ZVulkan)

set(WCPRIVATEERCOMMON_SOURCES
	Sources/Precomp.cpp
	Sources/Precomp.h
	Sources/Game/GameApp.cpp
	Sources/Game/GameApp.h
	Sources/Tool/ToolApp.cpp
	Sources/Tool/ToolApp.h
	Sources/FileFormat/WCArchive.cpp
	Sources/FileFormat/WCArchive.h
	Sources/FileFormat/WCImage.cpp
	Sources/FileFormat/WCImage.h
	Sources/FileFormat/WCPak.cpp
	Sources/FileFormat/WCPak.h
	Sources/FileFormat/WCPalette.cpp
	Sources/FileFormat/WCPalette.h
	Sources/FileFormat/FileEntryReader.h
	Sources/Util/UTF8Reader.cpp
	Sources/Util/UTF8Reader.h
	Sources/Util/UTF16.cpp
	Sources/Util/UTF16.h
	Sources/Commandlet/Commandlet.cpp
	Sources/Commandlet/Commandlet.h
	Sources/Commandlet/ExportCommandlet.cpp
	Sources/Commandlet/ExportCommandlet.h
	Sources/Commandlet/QuitCommandlet.cpp
	Sources/Commandlet/QuitCommandlet.h
)

set(WCPRIVATEERCOMMON_LIBS
	zvulkan
)

set(WCPRIVATEERGAME_SOURCES
	Sources/MainGame.cpp
)

set(WCPRIVATEERTOOL_SOURCES
	Sources/MainTool.cpp
)

set(WCPRIVATEERCOMMON_WIN32_SOURCES
)

set(WCPRIVATEERCOMMON_UNIX_SOURCES
)

set(LIBRARY_SOURCES
	Libraries/miniz/miniz.c
	Libraries/miniz/miniz.h
	Libraries/lzw/lzw.c
	Libraries/lzw/lzw.h
)

set(LIBRARY_WIN32_SOURCES
)

set(LIBRARY_UNIX_SOURCES
)

source_group("Sources" REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/Sources/.+")
source_group("Sources\\Game" REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Game/.+")
source_group("Sources\\Tool" REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Tool/.+")
source_group("Sources\\FileFormat" REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/Sources/FileFormat/.+")
source_group("Sources\\Util" REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Util/.+")
source_group("Sources\\Commandlet" REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Commandlet/.+")
source_group("Libraries" REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/Libraries/.+")
source_group("Libraries\\miniz" REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/Libraries/miniz/.+")
source_group("Libraries\\lzw" REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/Libraries/lzw/.+")

include_directories(Sources Libraries Libraries/ZVulkan/include Libraries/miniz)

if(WIN32)
	set(WCPRIVATEERCOMMON_SOURCES ${WCPRIVATEERCOMMON_SOURCES} ${WCPRIVATEERCOMMON_WIN32_SOURCES})
	set(WCPRIVATEERCOMMON_LIBS ${WCPRIVATEERCOMMON_LIBS} ${WCPRIVATEERCOMMON_WIN32_LIBS})
	set(LIBRARY_SOURCES ${LIBRARY_SOURCES} ${LIBRARY_WIN32_SOURCES})
	add_definitions(-DUNICODE -D_UNICODE)
elseif(APPLE)
	set(WCPRIVATEERCOMMON_SOURCES ${WCPRIVATEERCOMMON_SOURCES} ${WCPRIVATEERCOMMON_UNIX_SOURCES})
	set(LIBRARY_SOURCES ${LIBRARY_SOURCES} ${LIBRARY_UNIX_SOURCES})
	set(WCPRIVATEERCOMMON_LIBS ${WCPRIVATEERCOMMON_LIBS} ${WCPRIVATEERCOMMON_UNIX_LIBS} ${CMAKE_DL_LIBS})
	add_definitions(-DUNIX -D_UNIX)
	add_link_options(-pthread)
else()
	set(WCPRIVATEERCOMMON_SOURCES ${WCPRIVATEERCOMMON_SOURCES} ${WCPRIVATEERCOMMON_UNIX_SOURCES})
	set(WCPRIVATEERCOMMON_LIBS ${WCPRIVATEERCOMMON_LIBS} ${WCPRIVATEERCOMMON_UNIX_LIBS} ${CMAKE_DL_LIBS} -ldl -lasound)
	set(LIBRARY_SOURCES ${LIBRARY_SOURCES} ${LIBRARY_UNIX_SOURCES})
	add_definitions(-DUNIX -D_UNIX)
	add_link_options(-pthread)
endif()

if(MSVC)
	# Use all cores for compilation
	set(CMAKE_CXX_FLAGS "/MP ${CMAKE_CXX_FLAGS} /D_ITERATOR_DEBUG_LEVEL=0")

	# Ignore warnings in third party code
	set_source_files_properties(${LIBRARY_SOURCES} PROPERTIES COMPILE_FLAGS "/wd4244 /wd4267 /wd4005 /wd4018 -D_CRT_SECURE_NO_WARNINGS")
endif()

add_library(WCPrivateerCommon STATIC ${WCPRIVATEERCOMMON_SOURCES} ${LIBRARY_SOURCES})
set_target_properties(WCPrivateerCommon PROPERTIES CXX_STANDARD 20)
target_precompile_headers(WCPrivateerCommon PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/Sources/Precomp.h>)

add_executable(WCPrivateer WIN32 MACOSX_BUNDLE ${WCPRIVATEERGAME_SOURCES})
target_link_libraries(WCPrivateer WCPrivateerCommon ${WCPRIVATEERCOMMON_LIBS})
set_target_properties(WCPrivateer PROPERTIES CXX_STANDARD 20)

add_executable(WCPrivateerTool ${WCPRIVATEERTOOL_SOURCES})
target_link_libraries(WCPrivateerTool WCPrivateerCommon ${WCPRIVATEERCOMMON_LIBS})
set_target_properties(WCPrivateerTool PROPERTIES CXX_STANDARD 20)

# add_custom_command(TARGET WCPrivateer POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/WCPrivateer.pk3" $<TARGET_FILE_DIR:WCPrivateer>)

install(TARGETS WCPrivateer RUNTIME DESTINATION bin BUNDLE DESTINATION app)
install(TARGETS WCPrivateerTool RUNTIME DESTINATION bin)
