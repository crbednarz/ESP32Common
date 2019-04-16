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
endmacro()


macro(setup_headers_module header_path)
	set(COMPONENT_ADD_INCLUDEDIRS ${header_path})
	register_component()
endmacro()

macro(reference_module)
	set(COMPONENT_REQUIRES ${ARGN})
endmacro()
