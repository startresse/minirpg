cmake_minimum_required(VERSION 4.2.3)

# set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")

project ("minirpg" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

add_executable(${CMAKE_PROJECT_NAME})

# set(CMAKE_CXX_MODULE_STD ON)
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
	"src/main.cpp"
)
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
	FILE_SET modules
	TYPE CXX_MODULES
	FILES
	"src/gameloop.ixx"
	"src/input.ixx"
	"src/asciirenderer.ixx"
	"src/gamestate.ixx"
)

target_compile_features(${CMAKE_PROJECT_NAME} PRIVATE cxx_std_23)
