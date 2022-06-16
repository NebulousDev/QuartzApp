cmake_minimum_required(VERSION 3.20.0)

project(QuartzApp)

include(GNUInstallDirs)

find_package(glfw3 CONFIG REQUIRED)

add_library(${PROJECT_NAME} SHARED
	"Source/Application.cpp"
	"Source/Window.cpp"
	"Source/GLFW/GLFWApplication.cpp"
	"Source/GLFW/GLFWWindow.cpp"
	"Source/GLFW/GLFWHelper.cpp"
)

target_compile_definitions(${PROJECT_NAME} PRIVATE QUARTZ_DLL)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_include_directories(${PROJECT_NAME} 
	PUBLIC 
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include>"
		"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
	PRIVATE
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Source>"
)

target_link_libraries(${PROJECT_NAME}
	PRIVATE 
		glfw
)

# When not running a super-build, generate QuartzAppConfig.cmake
if(QUARTZ_SUBERBUILD)

target_include_directories(${PROJECT_NAME} 
	PRIVATE
		${QUARTZLIB_INCLUDE_PATH}
		${QUARTZMATH_INCLUDE_PATH}
)

else()

	find_package(quartzlib CONFIG)
	find_package(quartzmath CONFIG)

	target_link_libraries(${PROJECT_NAME}
		PRIVATE 
			quartzlib
			quartzmath
	)

	install(
		TARGETS ${PROJECT_NAME} 
		DESTINATION ${CMAKE_INSTALL_LIBDIR} 
		EXPORT ${PROJECT_NAME}-Export
	)

	install(
		DIRECTORY "Include/" 
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	)

	install(
		EXPORT ${PROJECT_NAME}-Export 
		FILE ${PROJECT_NAME}Config.cmake 
		DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}"
	)

endif()