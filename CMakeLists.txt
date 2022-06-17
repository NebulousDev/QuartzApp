cmake_minimum_required(VERSION 3.22.0)

option(QUARTZAPP_EXPOSE_APIS	"Expose enabled APIs to parent project" OFF)

option(QUARTZAPP_ENABLE_GLFW	"Enable GLFW window creation abilities"	ON)
option(QUARTZAPP_ENABLE_GLEW	"Enable GLEW OpenGL context creation"	ON)
option(QUARTZAPP_ENABLE_VULKAN	"Enable Vulkan context creation"		ON)
option(QUARTZAPP_ENABLE_DX12	"Enable DX12 context creation"			ON)

option(QUARTZAPP_GENERATE_CONFIGS 
	"Enable generation of QuartzAppConfig.cmake" ON)

project(QuartzApp)

include(GNUInstallDirs)

set(QUARTZAPP_FOUND_LIBS)
set(QUARTZAPP_FOUND_INCLUDES)
set(QUARTZAPP_COMIPLE_DEFINITIONS)

if(QUARTZAPP_ENABLE_GLFW)

	find_package(glfw3)

	if(${glfw3_FOUND})

		set(QUARTZAPP_FOUND_LIBS ${QUARTZAPP_FOUND_LIBS} glfw)
		set(QUARTZAPP_COMIPLE_DEFINITIONS ${QUARTZAPP_COMIPLE_DEFINITIONS} QUARTZAPP_GLFW)

		message("[QuartzApp] GLFW found: ${glfw3_DIR}")

	else()
		message("[QuartzApp] GLFW not found. Skipping configuration.")
	endif()

else()
	message("[QuartzApp] GLFW disabled. Skipping configuration.")
endif()

if(QUARTZAPP_ENABLE_GLEW)

	find_package(GLEW)

	if(${GLEW_FOUND})

		set(QUARTZAPP_FOUND_LIBS ${QUARTZAPP_FOUND_LIBS} GLEW::GLEW)
		set(QUARTZAPP_COMIPLE_DEFINITIONS ${QUARTZAPP_COMIPLE_DEFINITIONS} QUARTZAPP_GLEW)

		message("[QuartzApp] GLEW found: ${GLEW_DIR}")

	else()
		message("[QuartzApp] GLEW not found. Skipping configuration.")
	endif()

else()
	message("[QuartzApp] GLEW disabled. Skipping configuration.")
endif()

if(QUARTZAPP_ENABLE_VULKAN)

	find_package(Vulkan)

	if(${Vulkan_FOUND})

		set(QUARTZAPP_FOUND_LIBS ${QUARTZAPP_FOUND_LIBS} Vulkan::Vulkan Vulkan::Headers)
		set(QUARTZAPP_COMIPLE_DEFINITIONS ${QUARTZAPP_COMIPLE_DEFINITIONS} QUARTZAPP_VULKAN)

		message("[QuartzApp] Vulkan found: ${Vulkan_LIBRARY}")

	else()
		message("[QuartzApp] Vulkan not found. Skipping configuration.")
	endif()

else()
	message("[QuartzApp] Vulkan disabled. Skipping configuration.")
endif()

if(QUARTZAPP_ENABLE_DX12)

	find_package(directx-headers)

	if(${directx-headers_FOUND})

		set(QUARTZAPP_FOUND_LIBS ${QUARTZAPP_FOUND_LIBS} Microsoft::DirectX-Guids Microsoft::DirectX-Headers)
		set(QUARTZAPP_COMIPLE_DEFINITIONS ${QUARTZAPP_COMIPLE_DEFINITIONS} QUARTZAPP_DX12)

		message("[QuartzApp] DX12 found: ${directx-headers_DIR}")

	else()
		message("[QuartzApp] DX12 not found. Skipping configuration.")
	endif()

else()
	message("[QuartzApp] DX12 disabled. Skipping configuration.")
endif()

if(NOT ${QUARTZ_SUPERBUILD})

	find_package(quartzlib)
	find_package(quartzmath)

	if(${quartzlib_FOUND})

		set(QUARTZAPP_FOUND_LIBS ${QUARTZAPP_FOUND_LIBS} quartzlib)
		message("[QuartzApp] QuartzLib found: ${quartzlib_DIR}")

	else()
		message(FATAL_ERROR "[QuartzApp] QuartzLib not found but is required. Aborting.")
	endif()

	if(${quartzmath_FOUND})

		set(QUARTZAPP_FOUND_LIBS ${QUARTZAPP_FOUND_LIBS} quartzmath)
		message("[QuartzApp] QuartzMath found: ${quartzmath_DIR}")

	else()
		message(FATAL_ERROR "[QuartzApp] QuartzMath not found but is required. Aborting.")
	endif()

else()

	set(QUARTZAPP_FOUND_INCLUDES ${QUARTZAPP_FOUND_INCLUDES} 
		${QUARTZLIB_INCLUDE_PATH} ${QUARTZMATH_INCLUDE_PATH})

endif()

add_library(${PROJECT_NAME} SHARED
	"Source/Application.cpp"
	"Source/Window.cpp"
	"Source/GLFW/GLFWApplication.cpp"
	"Source/GLFW/GLFWWindow.cpp"
	"Source/GLFW/GLFWHelper.cpp"
	"Source/Surface.cpp" 
	"Source/Vulkan/VulkanSurface.cpp"
 "Source/Vulkan/VulkanHelper.h" "Include/OpenGL/GLSurface.h" "Source/OpenGL/GLSurface.cpp")

target_compile_definitions(${PROJECT_NAME} PRIVATE QUARTZAPP_DLL ${QUARTZAPP_COMIPLE_DEFINITIONS})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

if(QUARTZAPP_EXPOSE_APIS)

	set(QUARTZAPP_PUBLIC_INCLUDES	${QUARTZAPP_FOUND_INCLUDES})
	set(QUARTZAPP_PUBLIC_LIBS		${QUARTZAPP_FOUND_LIBS})

else()

	set(QUARTZAPP_PRIVATE_INCLUDES	${QUARTZAPP_FOUND_INCLUDES})
	set(QUARTZAPP_PRIVATE_LIBS		${QUARTZAPP_FOUND_LIBS})

endif()

target_include_directories(${PROJECT_NAME} 
	PUBLIC 
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include>"
		"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
		${QUARTZAPP_PUBLIC_INCLUDES}
	PRIVATE
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Source>"
		${QUARTZAPP_PRIVATE_INCLUDES}
)

target_link_libraries(${PROJECT_NAME}
	PUBLIC
		${QUARTZAPP_PUBLIC_LIBS}
	PRIVATE 
		${QUARTZAPP_PRIVATE_LIBS}
)

# Generate QuartzAppConfig.cmake
if(NOT ${QUARTZ_SUPERBUILD} AND QUARTZAPP_GENERATE_CONFIGS)

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

	message("[QuartzApp] Generating ${PROJECT_NAME}Config.cmake...")

endif()