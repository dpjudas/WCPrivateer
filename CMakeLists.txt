cmake_minimum_required(VERSION 3.15)

project(WCPrivateer)

if(MSVC)
	find_package(ZMusic)
else()
	find_package(ZMusic REQUIRED)
endif()

if(MSVC AND NOT ZMUSIC_FOUND)
	# Use prebuilt library
	set(ZMUSIC_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Binaries/windows/zmusic")
	set(ZMUSIC_INCLUDE_DIR ${ZMUSIC_ROOT_PATH}/include )
	set(ZMUSIC_LIBRARIES zmusic )
	if(ARM64)
		link_directories(${ZMUSIC_ROOT_PATH}/arm64)
	else()
		link_directories(${ZMUSIC_ROOT_PATH}/64bit)
	endif()
	set(ZMUSIC_FOUND TRUE)
endif()

add_subdirectory(Libraries/ZVulkan)
add_subdirectory(Libraries/ZWidget)

set(WCPRIVATEERCOMMON_SOURCES
	Sources/Precomp.cpp
	Sources/Precomp.h
	Sources/Game/GameApp.cpp
	Sources/Game/GameApp.h
	Sources/Game/Screens/GameScreen.cpp
	Sources/Game/Screens/GameScreen.h
	Sources/Game/Screens/AutopilotScreen.cpp
	Sources/Game/Screens/AutopilotScreen.h
	Sources/Game/Screens/CockpitScreen.cpp
	Sources/Game/Screens/CockpitScreen.h
	Sources/Game/Screens/ConversationScreen.cpp
	Sources/Game/Screens/ConversationScreen.h
	Sources/Game/Screens/LoadSaveScreen.cpp
	Sources/Game/Screens/LoadSaveScreen.h
	Sources/Game/Screens/MainMenuScreen.cpp
	Sources/Game/Screens/MainMenuScreen.h
	Sources/Game/Screens/NavScreen.cpp
	Sources/Game/Screens/NavScreen.h
	Sources/Game/Screens/OptionsScreen.cpp
	Sources/Game/Screens/OptionsScreen.h
	Sources/Game/Screens/JoystickScreen.cpp
	Sources/Game/Screens/JoystickScreen.h
	Sources/Game/Screens/Scene/SceneScreen.cpp
	Sources/Game/Screens/Scene/SceneScreen.h
	Sources/Game/Screens/Scene/BarScreen.cpp
	Sources/Game/Screens/Scene/BarScreen.h
	Sources/Game/Screens/Scene/BaseScreen.cpp
	Sources/Game/Screens/Scene/BaseScreen.h
	Sources/Game/Screens/Scene/HangarScreen.cpp
	Sources/Game/Screens/Scene/HangarScreen.h
	Sources/Game/Screens/Scene/MercenaryScreen.cpp
	Sources/Game/Screens/Scene/MercenaryScreen.h
	Sources/Game/Screens/Scene/MerchantScreen.cpp
	Sources/Game/Screens/Scene/MerchantScreen.h
	Sources/Game/Screens/Scene/MissionComputerScreen.cpp
	Sources/Game/Screens/Scene/MissionComputerScreen.h
	Sources/Game/Screens/Scene/ShipDealerScreen.cpp
	Sources/Game/Screens/Scene/ShipDealerScreen.h
	Sources/Game/Screens/Scene/ShipGarageScreen.cpp
	Sources/Game/Screens/Scene/ShipGarageScreen.h
	Sources/Game/Screens/Scene/ShipSoftwareScreen.cpp
	Sources/Game/Screens/Scene/ShipSoftwareScreen.h
	Sources/Game/Screens/Scene/TradeScreen.cpp
	Sources/Game/Screens/Scene/TradeScreen.h
	Sources/Game/Screens/Movie/MovieScreen.cpp
	Sources/Game/Screens/Movie/MovieScreen.h
	Sources/Game/Screens/Movie/IntroScreen.cpp
	Sources/Game/Screens/Movie/IntroScreen.h
	Sources/Game/Screens/Movie/VictoryScreen.cpp
	Sources/Game/Screens/Movie/VictoryScreen.h
	Sources/Game/Screens/Movie/LandingScreen.cpp
	Sources/Game/Screens/Movie/LandingScreen.h
	Sources/Game/Screens/Movie/TakeoffScreen.cpp
	Sources/Game/Screens/Movie/TakeoffScreen.h
	Sources/Game/Screens/Movie/JumpScreen.cpp
	Sources/Game/Screens/Movie/JumpScreen.h
	Sources/Game/RenderDevice.cpp
	Sources/Game/RenderDevice.h
	Sources/Game/Vulkan/VulkanRenderDevice.cpp
	Sources/Game/Vulkan/VulkanRenderDevice.h
	Sources/Audio/AudioPlayer.cpp
	Sources/Audio/AudioPlayer.h
	Sources/Audio/AudioSource.cpp
	Sources/Audio/AudioSource.h
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
	Sources/FileFormat/WCScene.cpp
	Sources/FileFormat/WCScene.h
	Sources/FileFormat/WCMovie.cpp
	Sources/FileFormat/WCMovie.h
	Sources/FileFormat/WCMusic.cpp
	Sources/FileFormat/WCMusic.h
	Sources/FileFormat/WCConversation.cpp
	Sources/FileFormat/WCConversation.h
	Sources/FileFormat/WCGameData.cpp
	Sources/FileFormat/WCGameData.h
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
	zwidget
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
	Libraries/resample/r8bbase.cpp
	Libraries/resample/r8bbase.h
	Libraries/resample/pffft.cpp
	Libraries/resample/pffft.h
	Libraries/resample/fft4g.h
	Libraries/resample/CDSPBlockConvolver.h
	Libraries/resample/CDSPFIRFilter.h
	Libraries/resample/CDSPFracInterpolator.h
	Libraries/resample/CDSPHBDownsampler.h
	Libraries/resample/CDSPHBUpsampler.h
	Libraries/resample/CDSPProcessor.h
	Libraries/resample/CDSPRealFFT.h
	Libraries/resample/CDSPResampler.h
	Libraries/resample/CDSPSincFilterGen.h
)

set(LIBRARY_WIN32_SOURCES
)

set(LIBRARY_UNIX_SOURCES
)

source_group("Sources" REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/Sources/.+")
source_group("Sources\\Game" REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Game/.+")
source_group("Sources\\Game\\Screens" REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Game/Screens/.+")
source_group("Sources\\Game\\Screens\\Scene" REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Game/Screens/Scene/.+")
source_group("Sources\\Game\\Screens\\Movie" REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Game/Screens/Movie/.+")
source_group("Sources\\Game\\Vulkan" REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Game/Vulkan/.+")
source_group("Sources\\Tool" REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Tool/.+")
source_group("Sources\\FileFormat" REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/Sources/FileFormat/.+")
source_group("Sources\\Util" REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Util/.+")
source_group("Sources\\Commandlet" REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Commandlet/.+")
source_group("Sources\\Audio" REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Audio/.+")
source_group("Libraries" REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/Libraries/.+")
source_group("Libraries\\miniz" REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/Libraries/miniz/.+")
source_group("Libraries\\lzw" REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/Libraries/lzw/.+")
source_group("Libraries\\resample" REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/Libraries/resample/.+")

include_directories(Sources Libraries Libraries/ZVulkan/include Libraries/ZWidget/include Libraries/miniz ${ZMUSIC_INCLUDE_DIR})

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
target_link_libraries(WCPrivateer WCPrivateerCommon ${WCPRIVATEERCOMMON_LIBS} ${ZMUSIC_LIBRARIES})
set_target_properties(WCPrivateer PROPERTIES CXX_STANDARD 20)

add_executable(WCPrivateerTool ${WCPRIVATEERTOOL_SOURCES})
target_link_libraries(WCPrivateerTool WCPrivateerCommon ${WCPRIVATEERCOMMON_LIBS} ${ZMUSIC_LIBRARIES})
set_target_properties(WCPrivateerTool PROPERTIES CXX_STANDARD 20)

if(WIN32)
	add_custom_command(TARGET WCPrivateer POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/Binaries/windows/zmusic/64bit/zmusic.dll $<TARGET_FILE_DIR:WCPrivateer>/zmusic.dll
		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/Binaries/WCPrivateer.sf2 $<TARGET_FILE_DIR:WCPrivateer>/soundfonts/WCPrivateer.sf2
		#COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/Binaries/GENMIDI.GS.wopl $<TARGET_FILE_DIR:WCPrivateer>/fm_banks/GENMIDI.GS.wopl
		#COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/Binaries/gs-by-papiezak-and-sneakernets.wopn $<TARGET_FILE_DIR:WCPrivateer>/fm_banks/gs-by-papiezak-and-sneakernets.wopn
	)
endif()

install(TARGETS WCPrivateer RUNTIME DESTINATION bin BUNDLE DESTINATION app)
install(TARGETS WCPrivateerTool RUNTIME DESTINATION bin)
