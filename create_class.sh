#!/bin/bash

# You can modify than
source_dir=src
capitalize_first=true




root_dir=`dirname $0`


function add_files_to_project
{
	upper_case="$(tr '[:lower:]' '[:upper:]' <<< ${1:0:1})${1:1}"
	lower_case="$(tr '[:upper:]' '[:lower:]' <<< ${1:0:1})${1:1}"
	echo "-> Do not forget to add "
	echo "${upper_case}.cpp and ${upper_case}.hpp to ${source_dir}/${2}/CMakeLists.txt"
	echo "${upper_case}Test.cpp and ${upper_case}Test.hpp to ${source_dir}/${2}/unit_tests/CMakeLists.txt"
}


function create_class_files
{
	upper_case="$(tr '[:lower:]' '[:upper:]' <<< ${1:0:1})${1:1}"
	lower_case="$(tr '[:upper:]' '[:lower:]' <<< ${2:0:1})${2:1}"
	touch $upper_case.cpp $upper_case.hpp

	echo "#include \"$upper_case.hpp\"

$lower_case::$upper_case::$upper_case()
{

}
" >> $upper_case.cpp

	echo "#pragma once

// Project
#include \"general_config.hpp\"

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

namespace $lower_case
{

#ifdef RPG_BUILD_TEST
class ${upper_case}Test;
#endif

class $upper_case
{
#ifdef RPG_BUILD_TEST
	friend class ${lower_case}::${upper_case}Test;
#endif
public:
	$upper_case();
};

} // namespace $lower_case
" >> $upper_case.hpp
}

function create_test_class
{
	upper_case="$(tr '[:lower:]' '[:upper:]' <<< ${1:0:1})${1:1}"
	lower_case="$(tr '[:upper:]' '[:lower:]' <<< ${2:0:1})${2:1}"
	touch ${upper_case}Test.cpp ${upper_case}Test.hpp

	echo "#include \"${upper_case}Test.hpp\"
namespace ${lower_case} {

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


function create_class
{
	module_name=$2
	class_name=$1
	# Force the first letter in capital
	if [ "$capitalize_first" = true ]; then
		module_name="$(tr '[:lower:]' '[:upper:]' <<< ${module_name:0:1})${module_name:1}"
	fi
	class_name="$(tr '[:lower:]' '[:upper:]' <<< ${class_name:0:1})${class_name:1}"


	cd $root_dir/$source_dir
	# Verify if the modules exists
	if [ ! -e $module_name ]; then
		echo "Create first the module"
		exit
	fi

	echo "Add module"
	add_files_to_project $class_name $module_name

	cd $module_name
	
	echo "Create Class files"
	create_class_files $class_name $module_name

	cd unit_tests

	echo "Create Test Class"
	create_test_class $class_name $module_name

}

function help
{
	echo "Usage : $0 [module names]"
}



if [ $# -ne 2 ]; then
	help
	exit
fi

create_class $1 $2

