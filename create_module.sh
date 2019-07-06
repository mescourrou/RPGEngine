#!/bin/bash

# You can modify than
source_dir=src
capitalize_first=true




root_dir=`dirname $0`


function add_module_to_project
{
	echo "-> Do not forget to add "
	echo "list(APPEND MODULES $1)"
	echo " to the ${source_dir}/CMakeLists.txt"
}

function create_cmake
{
	upper_case="$(tr '[:lower:]' '[:upper:]' <<< ${1:0:1})${1:1}"
	lower_case="$(tr '[:upper:]' '[:lower:]' <<< ${1:0:1})${1:1}"
	touch CMakeLists.txt
	echo "add_library($1 $upper_case.cpp $upper_case.hpp)
target_link_libraries($1 BaseObject)
target_include_directories($1 PUBLIC \${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories($1 PUBLIC \"\${CMAKE_BINARY_DIR}/\${RPG_CONFIG_FILE_DIR}\")

if (RPG_BUILD_TEST)
	find_package(gtest REQUIRED)
	target_link_libraries($1 gtest)
	add_subdirectory(unit_tests)
endif()
	" >> CMakeLists.txt
}

function create_class
{
	upper_case="$(tr '[:lower:]' '[:upper:]' <<< ${1:0:1})${1:1}"
	lower_case="$(tr '[:upper:]' '[:lower:]' <<< ${1:0:1})${1:1}"
	touch $upper_case.cpp $upper_case.hpp

	echo "#include \"$upper_case.hpp\"
namespace ${lower_case} {
	
$upper_case::$upper_case()
{

}

}
" >> $upper_case.cpp

	echo "#pragma once

// Project
#include \"general_config.hpp\"
#include <BaseObject.hpp>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

namespace $lower_case
{

#ifdef RPG_BUILD_TEST
class ${upper_case}Test;
#endif

class $upper_case : public BaseObject
{
	DECLARE_BASEOBJECT($upper_case)
#ifdef RPG_BUILD_TEST
	friend class ${lower_case}::${upper_case}Test;
#endif
public:
	$upper_case();
	~${upper_case}() override = default;
};

} // namespace $lower_case
" >> $upper_case.hpp
}

function create_test_class
{
	upper_case="$(tr '[:lower:]' '[:upper:]' <<< ${1:0:1})${1:1}"
	lower_case="$(tr '[:upper:]' '[:lower:]' <<< ${1:0:1})${1:1}"
	touch ${upper_case}Test.cpp ${upper_case}Test.hpp

	echo "#include \"${upper_case}Test.hpp\"
namespace ${lower_case} {

}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
" >> ${upper_case}Test.cpp

	echo "#pragma once

#include <gtest/gtest.h>
#include <${upper_case}.hpp>

namespace ${lower_case} {

class ${upper_case}Test : public testing::Test
{
public:
};

}
" >> ${upper_case}Test.hpp

}

function create_test_cmake
{

	upper_case="$(tr '[:lower:]' '[:upper:]' <<< ${1:0:1})${1:1}"
	lower_case="$(tr '[:upper:]' '[:lower:]' <<< ${1:0:1})${1:1}"
	touch CMakeLists.txt
	echo "find_package(gtest REQUIRED)

add_executable($1_test ${upper_case}Test.cpp ${upper_case}Test.hpp)
target_link_libraries($1_test gtest $1)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/data" AND IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data")
add_custom_command(TARGET $1_test POST_BUILD
  COMMAND \"\${CMAKE_COMMAND}\" -E copy_directory
	 \"\${CMAKE_CURRENT_SOURCE_DIR}/data\"
	 \"\${CMAKE_CURRENT_BINARY_DIR}/data\"
  COMMENT \"Copying test data output directory\")
endif()

" >> CMakeLists.txt
}



function create_module
{
	if [ $# -eq 0 ];then
		echo "Error : no more parameters given"
	fi
	module_name=$1
	# Force the first letter in capital
	if [ "$capitalize_first" = true ]; then
		module_name="$(tr '[:lower:]' '[:upper:]' <<< ${module_name:0:1})${module_name:1}"
	fi


	echo "Creation of the $module_name module"

	cd $root_dir/$source_dir
	# Verify if the modules exists
	if [ -e $module_name ]; then
		echo "Module already created, erase ? Y/[N]"
		read response
		response=${response^^}
		if [ "$response" = "Y" ]; then
			rm -rf $module_name
		else
			exit
		fi
	fi
	mkdir $module_name

	echo "Add module"
	add_module_to_project $module_name

	cd $module_name
	
	echo "Create Class files"
	create_class $module_name

	echo "Create CMakeLists.txt"
	create_cmake $module_name

	echo "Create unit_tests directory"
	mkdir unit_tests
	cd unit_tests

	echo "Create Test Class"
	create_test_class $module_name

	echo "Create CMakeLists.txt for test"
	create_test_cmake $module_name

	echo "Create data directory"
	mkdir data
}

function help
{
	echo "Usage : $0 [module names]"
}



if [ $# -eq 0 ]; then
	help
	exit
fi

while [ $# -gt 0 ] ; do

	create_module $1
	
	shift
done


