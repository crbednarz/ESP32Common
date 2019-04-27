macro(setup_module)
	file(GLOB_RECURSE COMPONENT_SRCS
		"public/*.c"
		"public/*.h"
		"public/*.hpp"
		"public/*.cpp"
		"private/*.c"
		"private/*.h"
		"private/*.hpp"
		"private/*.cpp")

	set(COMPONENT_ADD_INCLUDEDIRS "public" "private")
	register_component()
	component_compile_options(-std=gnu++17)
endmacro()


macro(setup_headers_module header_path)
	set(COMPONENT_ADD_INCLUDEDIRS ${header_path})
	register_component()
endmacro()

macro(reference_module)
	set(COMPONENT_REQUIRES ${ARGN})
endmacro()


macro(register_project project_name)
	if (ESP_PLATFORM)
		include($ENV{IDF_PATH}/tools/cmake/project.cmake)
		set(EXTRA_COMPONENT_DIRS "${EXTRA_COMPONENT_DIRS} common/components")
	endif()

	project(${project_name})
endmacro()