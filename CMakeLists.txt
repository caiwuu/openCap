cmake_minimum_required(VERSION 3.16)
project(openCap VERSION 1.0 LANGUAGES CXX OBJCXX)

# 启用compile_commands.json生成
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 将部署目标设置为macOS 12.0或更高，与您的Qt版本兼容
set(CMAKE_OSX_DEPLOYMENT_TARGET "12.0" CACHE STRING "Minimum OS X version" FORCE)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-DQT_NO_DEBUG_OUTPUT)
    add_definitions(-DNDEBUG)
    message(STATUS "Release模式：已禁用调试输出")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Svg)

# 添加 Qt 包含目录
include_directories(${Qt6Core_INCLUDE_DIRS})
include_directories(${Qt6Widgets_INCLUDE_DIRS})
include_directories(${Qt6Svg_INCLUDE_DIRS})

# Qt设置
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 自动收集源文件和头文件（现代化结构：头文件和源文件在同一目录）
file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "src/*.mm"
    "src/*.h"
)

# 从源文件列表中移除平台特定文件，稍后根据平台添加
if(NOT APPLE)
    list(FILTER SOURCES EXCLUDE REGEX ".*\\.mm$")
    list(FILTER SOURCES EXCLUDE REGEX ".*/platform/mac/.*")
endif()

# macOS特定文件处理
if(APPLE)
    # 设置Objective-C++标准
    set(CMAKE_OBJCXX_STANDARD 17)
    set(CMAKE_OBJCXX_STANDARD_REQUIRED ON)
    
    # 查找并链接 macOS 框架
    find_library(CARBON_FRAMEWORK Carbon REQUIRED)
    find_library(COCOA_FRAMEWORK Cocoa REQUIRED)

    # 设置编译器标志以确保兼容性
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mmacosx-version-min=12.0")
    set(CMAKE_OBJCXX_FLAGS "${CMAKE_OBJCXX_FLAGS} -mmacosx-version-min=12.0")
endif()

# 添加资源文件
set(RESOURCES resources.qrc)

# 创建可执行文件
qt6_add_executable(openCap ${SOURCES} ${RESOURCES})

# 自动收集所有模块目录作为包含目录
file(GLOB_RECURSE MODULE_DIRS LIST_DIRECTORIES true "${CMAKE_SOURCE_DIR}/src/*")
set(INCLUDE_DIRS "src")  # 始终包含src根目录

# 过滤出目录并添加到包含路径
foreach(item ${MODULE_DIRS})
    if(IS_DIRECTORY ${item})
        # 获取相对于项目根目录的路径
        file(RELATIVE_PATH rel_dir ${CMAKE_SOURCE_DIR} ${item})
        list(APPEND INCLUDE_DIRS ${rel_dir})
    endif()
endforeach()

# 设置包含目录（现代化：自动发现所有模块目录）
target_include_directories(openCap PRIVATE ${INCLUDE_DIRS})

# 链接Qt库 (添加了Qt6::Svg支持SVG图标)
target_link_libraries(openCap PRIVATE Qt6::Core Qt6::Widgets Qt6::Svg)

# macOS特定设置
if(APPLE)
    # 链接 macOS 框架
    target_link_libraries(openCap PRIVATE 
        "-framework Cocoa"
        "-framework Carbon")
    
    set_target_properties(openCap PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/Info.plist"
    )
endif()

# 安装设置
install(TARGETS openCap
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION .
)

# 安装图标文件
install(DIRECTORY ${CMAKE_SOURCE_DIR}/icons DESTINATION bin) 