cmake_minimum_required (VERSION 3.20)

project(OGLGAME)

include(FetchContent)

if(WIN32)
set(OGLGAME_PLATFORM "Win")
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON) #for some reason it doesn't build all .lib files without it
elseif(LINUX)
set(OGLGAME_PLATFORM "Linux")
else()
message(FATAL_ERROR "Unsupported platform! (l bozo )")
endif()

set(BIN_DIR "${CMAKE_SOURCE_DIR}/OGLGAME/bin/${OGLGAME_PLATFORM}-${CMAKE_BUILD_TYPE}")
set(EXTBIN_DIR "${CMAKE_SOURCE_DIR}/extbin/${OGLGAME_PLATFORM}-${CMAKE_BUILD_TYPE}")

if(WIN32)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})
elseif(LINUX)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR})
endif()

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp" "src/*.h" "extsrc/*.c")
add_executable (${PROJECT_NAME} ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 26)

if(WIN32)
target_compile_definitions(${PROJECT_NAME} PRIVATE OGLGAME_PLATFORM_WINDOWS=1)
elseif(LINUX)
target_compile_definitions(${PROJECT_NAME} PRIVATE OGLGAME_PLATFORM_LINUX=1)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
target_compile_definitions(${PROJECT_NAME} PRIVATE OGLGAME_BUILD_TYPE_DEBUG=1)
target_compile_definitions(${PROJECT_NAME} PRIVATE OGLGAME_SHOWDBGINFO=1)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
target_compile_definitions(${PROJECT_NAME} PRIVATE OGLGAME_BUILD_TYPE_RELEASE=1)
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
target_compile_definitions(${PROJECT_NAME} PRIVATE OGLGAME_BUILD_TYPE_RELWITHDEBINFO=1)
target_compile_definitions(${PROJECT_NAME} PRIVATE OGLGAME_SHOWDBGINFO=1)
endif()
target_compile_definitions(${PROJECT_NAME} PRIVATE OGLGAME_BUILD_TYPE="${CMAKE_BUILD_TYPE}")

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/src")

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/extinclude")

#SDL3
FetchContent_Declare(
    sdl3
    GIT_REPOSITORY  https://github.com/libsdl-org/SDL.git
    GIT_TAG         main
)
FetchContent_MakeAvailable(sdl3)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3-shared)

#SDL3 Image
FetchContent_Declare(
    sdl3image
    GIT_REPOSITORY  https://github.com/libsdl-org/SDL_image.git
    GIT_TAG         main
)
FetchContent_MakeAvailable(sdl3image)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3_image::SDL3_image-shared)

#glm
FetchContent_Declare(
    glm
    GIT_REPOSITORY  https://github.com/g-truc/glm.git
    GIT_TAG         master
)
FetchContent_MakeAvailable(glm)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)

#fmt
FetchContent_Declare(
    fmt
    GIT_REPOSITORY  https://github.com/fmtlib/fmt.git
    GIT_TAG         master
)
FetchContent_MakeAvailable(fmt)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)

#json
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG master
)
FetchContent_MakeAvailable(json)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

#assimp
set(ASSIMP_NO_EXPORT ON CACHE INTERNAL "Disable Assimp's export functionality")
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE INTERNAL "Don't build Assimp with all importers enabled")
set(ASSIMP_BUILD_TESTS OFF CACHE INTERNAL "Dont build Assimp's tests")
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE INTERNAL "Enable Assimp's obj importer")
FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG master
)
FetchContent_MakeAvailable(assimp)
target_link_libraries(${PROJECT_NAME} PRIVATE assimp::assimp)

#openal
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/ext/openal/include")
target_link_libraries(${PROJECT_NAME} PRIVATE "${EXTBIN_DIR}/OpenAL32.lib")