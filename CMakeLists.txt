cmake_minimum_required(VERSION 3.20.0)

option(QUARTZAPP_BUILD_STATIC       "Build as a static library"                     OFF)
option(QUARTZAPP_EXPOSE_APIS        "Expose enabled APIs to parent project"         ON)
option(QUARTZAPP_ENABLE_GLFW        "Enable GLFW window creation abilities"         ON)
option(QUARTZAPP_ENABLE_WINAPI      "Enable WinApi window creation abilities"       ON)
option(QUARTZAPP_ENABLE_GLEW        "Enable GLEW OpenGL context creation"           ON)
option(QUARTZAPP_ENABLE_VULKAN      "Enable Vulkan context creation"                ON)
option(QUARTZAPP_ENABLE_DX12        "Enable DX12 context creation"                  ON)
option(QUARTZAPP_GENERATE_CONFIGS   "Enable generation of QuartzAppConfig.cmake"    ON)

set(QUARTZAPP_INCLUDE_PREFIX "Quartz" CACHE STRING "Include prefix for installed headers")

project(QuartzApp VERSION 1.0.0)

include(GNUInstallDirs)

set(QUARTZAPP_FOUND_LIBS)
set(QUARTZAPP_FOUND_INCLUDES)
set(QUARTZAPP_COMIPLE_DEFINITIONS)

set(QUARTZAPP_SOURCE_FILES
	"Source/Application.cpp"
	"Source/Window.cpp"
	"Source/Surface.cpp"
)

if(QUARTZAPP_ENABLE_GLFW)

	find_package(glfw3 QUIET)

	if(${glfw3_FOUND})

		set(QUARTZAPP_FOUND_LIBS ${QUARTZAPP_FOUND_LIBS} glfw)
		set(QUARTZAPP_COMIPLE_DEFINITIONS ${QUARTZAPP_COMIPLE_DEFINITIONS} QUARTZAPP_GLFW)
		set(QUARTZAPP_GLFW PARENT_SCOPE)

		set(QUARTZAPP_SOURCE_FILES ${QUARTZAPP_SOURCE_FILES}
			"Source/GLFW/GLFWApplication.cpp"
			"Source/GLFW/GLFWWindow.cpp"
			"Source/GLFW/GLFWHelper.cpp"
			"Source/GLFW/GLFWRegistry.cpp"
			"Source/GLFW/GLFWCallbacks.cpp"
		)

		message("[QuartzApp] GLFW found: ${glfw3_DIR}")

	else()
		message("[QuartzApp] GLFW not found. Skipping configuration.")
	endif()

else()
	message("[QuartzApp] GLFW disabled. Skipping configuration.")
endif()

if(QUARTZAPP_ENABLE_WINAPI)

	if(WIN32)

		set(QUARTZAPP_COMIPLE_DEFINITIONS ${QUARTZAPP_COMIPLE_DEFINITIONS} QUARTZAPP_WINAPI)
		set(QUARTZAPP_WINAPI PARENT_SCOPE)

		set(QUARTZAPP_SOURCE_FILES ${QUARTZAPP_SOURCE_FILES}
			"Source/Windows/WinApiApplication.cpp"
			"Source/Windows/WinApiWindow.cpp"
			"Source/Windows/WinApiHelper.cpp"
			"Source/Windows/WinApiRegistry.cpp"
		)

		message("[QuartzApp] WinApi is available.")

	else()
		message("[QuartzApp] WinApi not available. Skipping configuration.")
	endif()

else()
	message("[QuartzApp] WinApi disabled. Skipping configuration.")
endif()

if(QUARTZAPP_ENABLE_GLEW)

	find_package(GLEW QUIET)

	if(${GLEW_FOUND})

		set(QUARTZAPP_FOUND_LIBS ${QUARTZAPP_FOUND_LIBS} GLEW::GLEW)
		set(QUARTZAPP_COMIPLE_DEFINITIONS ${QUARTZAPP_COMIPLE_DEFINITIONS} QUARTZAPP_GLEW)
		set(QUARTZAPP_GLEW PARENT_SCOPE)

		set(QUARTZAPP_SOURCE_FILES ${QUARTZAPP_SOURCE_FILES}
			"Source/OpenGL/GLSurface.cpp"
		)

		message("[QuartzApp] GLEW found: ${GLEW_DIR}")

	else()
		message("[QuartzApp] GLEW not found. Skipping configuration.")
	endif()

else()
	message("[QuartzApp] GLEW disabled. Skipping configuration.")
endif()

if(QUARTZAPP_ENABLE_VULKAN)

	# Note: This *should* be using CMake's built-in FindVulkan module 
	find_package(Vulkan QUIET)

	if(${Vulkan_FOUND})

		set(QUARTZAPP_FOUND_LIBS ${QUARTZAPP_FOUND_LIBS} Vulkan::Vulkan Vulkan::Headers)
		set(QUARTZAPP_COMIPLE_DEFINITIONS ${QUARTZAPP_COMIPLE_DEFINITIONS} QUARTZAPP_VULKAN)
		set(QUARTZAPP_VULKAN PARENT_SCOPE)

		set(QUARTZAPP_SOURCE_FILES ${QUARTZAPP_SOURCE_FILES}
			"Source/Vulkan/VulkanSurface.cpp"
		)

		message("[QuartzApp] Vulkan found: ${Vulkan_LIBRARY}")

	else()
		message("[QuartzApp] Vulkan not found. Skipping configuration.")
	endif()

else()
	message("[QuartzApp] Vulkan disabled. Skipping configuration.")
endif()

if(QUARTZAPP_ENABLE_DX12)

	find_package(directx-headers QUIET)

	if(${directx-headers_FOUND})

		set(QUARTZAPP_FOUND_LIBS ${QUARTZAPP_FOUND_LIBS} Microsoft::DirectX-Guids Microsoft::DirectX-Headers)
		set(QUARTZAPP_COMIPLE_DEFINITIONS ${QUARTZAPP_COMIPLE_DEFINITIONS} QUARTZAPP_DX12)
		set(QUARTZAPP_DX12 PARENT_SCOPE)

		message("[QuartzApp] DX12 found: ${directx-headers_DIR}")

	else()
		message("[QuartzApp] DX12 not found. Skipping configuration.")
	endif()

else()
	message("[QuartzApp] DX12 disabled. Skipping configuration.")
endif()

if(NOT ${QUARTZ_SUPERBUILD})

	find_package(quartzlib QUIET)
	find_package(quartzmath QUIET)

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

if(QUARTZAPP_BUILD_STATIC)

	add_library(${PROJECT_NAME} STATIC ${QUARTZAPP_SOURCE_FILES})

	set(QUARTZAPP_COMIPLE_DEFINITIONS ${QUARTZAPP_COMIPLE_DEFINITIONS} QUARTZAPP_STATIC)

else()
	
	add_library(${PROJECT_NAME} SHARED ${QUARTZAPP_SOURCE_FILES})

	set(QUARTZAPP_COMIPLE_DEFINITIONS ${QUARTZAPP_COMIPLE_DEFINITIONS} QUARTZAPP_DLL)

endif()

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

if(QUARTZAPP_EXPOSE_APIS)

	set(QUARTZAPP_PUBLIC_INCLUDES	${QUARTZAPP_FOUND_INCLUDES})
	set(QUARTZAPP_PUBLIC_LIBS		${QUARTZAPP_FOUND_LIBS})

	target_compile_definitions(${PROJECT_NAME} PUBLIC ${QUARTZAPP_COMIPLE_DEFINITIONS})

else()

	set(QUARTZAPP_PRIVATE_INCLUDES	${QUARTZAPP_FOUND_INCLUDES})
	set(QUARTZAPP_PRIVATE_LIBS		${QUARTZAPP_FOUND_LIBS})

	target_compile_definitions(${PROJECT_NAME} PRIVATE ${QUARTZAPP_COMIPLE_DEFINITIONS})

endif()

target_include_directories(${PROJECT_NAME} 
	PUBLIC 
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include>"
		"$<BUILD_INTERFACE:${QUARTZAPP_PUBLIC_INCLUDES}>"
		"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
	PRIVATE
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Source>"
		"$<BUILD_INTERFACE:${QUARTZAPP_PRIVATE_INCLUDES}>"
)

target_link_libraries(${PROJECT_NAME}
	PUBLIC
		${QUARTZAPP_PUBLIC_LIBS}
	PRIVATE 
		${QUARTZAPP_PRIVATE_LIBS}
)

# Generate QuartzAppConfig.cmake
if(QUARTZAPP_GENERATE_CONFIGS)

	install(
		TARGETS ${PROJECT_NAME} 
		DESTINATION ${CMAKE_INSTALL_LIBDIR} 
		EXPORT ${PROJECT_NAME}-Export
	)

	install(
		EXPORT ${PROJECT_NAME}-Export 
		FILE ${PROJECT_NAME}Config.cmake 
		DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}"
	)

	install(
		DIRECTORY "${PROJECT_SOURCE_DIR}/Include/" 
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${QUARTZAPP_INCLUDE_PREFIX}
	)

	message("[${PROJECT_NAME}] Generating ${PROJECT_NAME}Config.cmake...")

endif()