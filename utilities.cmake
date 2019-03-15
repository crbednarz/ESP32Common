macro(setup_module name)
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
