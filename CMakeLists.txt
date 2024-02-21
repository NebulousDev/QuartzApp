cmake_minimum_required(VERSION 3.25.0)

option(QUARTZAPP_BUILD_STATIC       "Build as a static library"                     OFF)
option(QUARTZAPP_EXPOSE_APIS        "Expose enabled APIs to parent project"         ON)
option(QUARTZAPP_ENABLE_GLFW        "Enable GLFW window creation abilities"         ON)
option(QUARTZAPP_ENABLE_WINAPI      "Enable WinApi window creation abilities"       ON)
option(QUARTZAPP_ENABLE_GLEW        "Enable GLEW OpenGL context creation"           ON)
option(QUARTZAPP_ENABLE_VULKAN      "Enable Vulkan context creation"                ON)
option(QUARTZAPP_ENABLE_DX12        "Enable DX12 context creation"                  OFF)
option(QUARTZAPP_ENABLE_GLSLANG     "Enable glslang linking"                        ON)
option(QUARTZAPP_ENABLE_SHADERC     "Enable shaderc_combined linking"               ON)
option(QUARTZAPP_ENABLE_DXC         "Enable dxc linking"                            ON)
option(QUARTZAPP_ENABLE_SPIRV_TOOLS "Enable SPIRV-Tools linking"                    ON)
option(QUARTZAPP_GENERATE_CONFIGS   "Enable generation of QuartzAppConfig.cmake"    ON)

set(QUARTZAPP_INCLUDE_PREFIX "Quartz" CACHE STRING "Include prefix for installed headers")

project(QuartzApp VERSION 1.1.2)

include(GNUInstallDirs)

set(QUARTZAPP_FOUND_LIBS)
set(QUARTZAPP_FOUND_INCLUDES)
set(QUARTZAPP_COMIPLE_DEFINITIONS)

set(QUARTZAPP_SOURCE_FILES
	"Source/Application.cpp"
	"Source/Window.cpp"
	"Source/Surface.cpp"
	"Source/LogCallbacks.cpp"
	"Source/Log.cpp"
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
		message("[QuartzApp] WinApi not available. Skipping.")
	endif()

else()
	message("[QuartzApp] WinApi disabled. Skipping.")
endif()

if(QUARTZAPP_ENABLE_GLEW)

	find_package(GLEW QUIET)

	if(${GLEW_FOUND})

		set(QUARTZAPP_FOUND_LIBS ${QUARTZAPP_FOUND_LIBS} GLEW::GLEW)
		set(QUARTZAPP_COMIPLE_DEFINITIONS ${QUARTZAPP_COMIPLE_DEFINITIONS} QUARTZAPP_GLEW)
		set(QUARTZAPP_GLEW PARENT_SCOPE)

		set(QUARTZAPP_SOURCE_FILES ${QUARTZAPP_SOURCE_FILES}
			"Source/OpenGL/GLApiSurface.cpp"
		)

		message("[QuartzApp] GLEW found: ${GLEW_DIR}")

	else()
		message("[QuartzApp] GLEW not found. Skipping.")
	endif()

else()
	message("[QuartzApp] GLEW disabled. Skipping.")
endif()

if(QUARTZAPP_ENABLE_VULKAN)

	# Note: This should use CMake's built-in FindVulkan module 
	find_package(Vulkan QUIET COMPONENTS glslang shaderc_combined dxc SPIRV-Tools)

	if(${Vulkan_FOUND})

		message("[QuartzApp] Vulkan found: ${Vulkan_LIBRARY}")

		set(QUARTZAPP_FOUND_LIBS ${QUARTZAPP_FOUND_LIBS} Vulkan::Vulkan Vulkan::Headers)

		if(${Vulkan_glslang_FOUND})
			if(QUARTZAPP_ENABLE_GLSLANG)
				set(QUARTZAPP_FOUND_LIBS ${QUARTZAPP_FOUND_LIBS} Vulkan::glslang)
				message("[QuartzApp] Vulkan glslang found: ${Vulkan_glslang_LIBRARY}")
			else()
				message("[QuartzApp] Vulkan glslang found. Disabled.")
			endif()
		else()
			message("[QuartzApp] Vulkan glslang not found. Skipping.")
		endif()

		if(${Vulkan_shaderc_combined_FOUND})
			if(QUARTZAPP_ENABLE_SHADERC)
				set(QUARTZAPP_FOUND_LIBS ${QUARTZAPP_FOUND_LIBS} Vulkan::shaderc_combined)
				message("[QuartzApp] Vulkan shaderc found: ${Vulkan_shaderc_combined_LIBRARY}")
			else()
				message("[QuartzApp] Vulkan shaderc found. Disabled.")
			endif()
		else()
			message("[QuartzApp] Vulkan shaderc not found. Skipping.")
		endif()

		if(${Vulkan_dxc_lib_FOUND})
			if(QUARTZAPP_ENABLE_DXC)
				set(QUARTZAPP_FOUND_LIBS ${QUARTZAPP_FOUND_LIBS} Vulkan::dxc_lib)
				message("[QuartzApp] Vulkan dxc found: ${Vulkan_dxc_LIBRARY}")
			else()
				message("[QuartzApp] Vulkan dxc found. Disabled.")
			endif()
		else()
			message("[QuartzApp] Vulkan dxc not found. Skipping.")
		endif()

		if(${Vulkan_SPIRV-Tools_FOUND})
			if(QUARTZAPP_ENABLE_SPIRV_TOOLS)
				set(QUARTZAPP_FOUND_LIBS ${QUARTZAPP_FOUND_LIBS} Vulkan::SPIRV-Tools)
				message("[QuartzApp] Vulkan SPIRV-Tools found: ${Vulkan_SPIRV-Tools_LIBRARY}")
			else()
				message("[QuartzApp] Vulkan SPIRV-Tools found. Disabled.")
			endif()
		else()
			message("[QuartzApp] Vulkan SPIRV-Tools not found. Skipping.")
		endif()

		set(QUARTZAPP_COMIPLE_DEFINITIONS ${QUARTZAPP_COMIPLE_DEFINITIONS} QUARTZAPP_VULKAN)
		set(QUARTZAPP_VULKAN PARENT_SCOPE)

		set(QUARTZAPP_SOURCE_FILES ${QUARTZAPP_SOURCE_FILES}
			"Source/Vulkan/VulkanApiSurface.cpp"
		)

	else()
		message("[QuartzApp] Vulkan not found. Skipping.")
	endif()

else()
	message("[QuartzApp] Vulkan disabled. Skipping.")
endif()

if(QUARTZAPP_ENABLE_DX12)

	#find_package(directx-headers QUIET)

	if(${directx-headers_FOUND})

		set(QUARTZAPP_FOUND_LIBS ${QUARTZAPP_FOUND_LIBS} Microsoft::DirectX-Guids Microsoft::DirectX-Headers)
		set(QUARTZAPP_COMIPLE_DEFINITIONS ${QUARTZAPP_COMIPLE_DEFINITIONS} QUARTZAPP_DX12)
		set(QUARTZAPP_DX12 PARENT_SCOPE)

		message("[QuartzApp] DX12 found: ${directx-headers_DIR}")

	else()
		message("[QuartzApp] DX12 not found. Skipping.")
	endif()

else()
	message("[QuartzApp] DX12 disabled. Skipping.")
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

	set(QUARTZAPP_FOUND_INCLUDES  ${QUARTZAPP_FOUND_INCLUDES} 
		${QUARTZLIB_INCLUDE_PATH} 
		${QUARTZMATH_INCLUDE_PATH}
	)

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