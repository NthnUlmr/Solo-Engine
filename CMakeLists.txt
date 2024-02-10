# CMakeList.txt : CMake project for SoloEngine2, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.14.1)

# CMAKE Based on : https://github.com/drsnuggles8/OloEngineBase/tree/master

# IPO support
cmake_policy(SET CMP0069 NEW) 
set(CMAKE_POLICY_DEFAULT_CMP0069 NEW)

# Link Projects in other directories with target_link_libraries()
cmake_policy(SET CMP0079 NEW) 
set(CMAKE_POLICY_DEFAULT_CMP0079 NEW)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

find_program(AWK NAMES gawk awk)

if(AWK)
message("AWK FOUND")
message(${AWK})
endif()

project (SoloEngine2 VERSION 1.0.0)


# Includes
include(cmake/SetupConfigurations.cmake)
include(cmake/CPM.cmake)

# Project wide setup
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_DOTNET_TARGET_FRAMEWORK_VERSION "v4.8.1")
set(CMAKE_CSharp_FLAGS "/langversion:8")
set_property(GLOBAL PROPERTY USE_FOLDERS YES)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "Utilities/cmake")


add_subdirectory(SoloEditor)
add_subdirectory(SoloEngine)
add_dependencies(SoloEditor SoloEngine )



set_target_properties(SoloEditor PROPERTIES FOLDER "Applications")



if(${CMAKE_GENERATOR} MATCHES "Visual Studio")
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "SoloEditor")
endif()

