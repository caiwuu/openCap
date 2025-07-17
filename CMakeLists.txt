cmake_minimum_required(VERSION 3.16)
project(openCap VERSION 1.0 LANGUAGES CXX OBJCXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Svg)

# Qt设置
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 包含目录
include_directories(include)

# 自动收集源文件
file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "src/*.mm"
)

# 自动收集头文件
file(GLOB_RECURSE HEADERS
    "include/*.h"
)

# 从源文件列表中移除平台特定文件，稍后根据平台添加
if(NOT APPLE)
    list(FILTER SOURCES EXCLUDE REGEX ".*\\.mm$")
endif()

# macOS特定文件处理
if(APPLE)
    # 设置Objective-C++标准
    set(CMAKE_OBJCXX_STANDARD 17)
    set(CMAKE_OBJCXX_STANDARD_REQUIRED ON)
    
    # 查找并链接 macOS 框架
    find_library(CARBON_FRAMEWORK Carbon REQUIRED)
    find_library(COCOA_FRAMEWORK Cocoa REQUIRED)
endif()

# 创建可执行文件
qt6_add_executable(openCap ${SOURCES} ${HEADERS})

# 设置包含目录
target_include_directories(openCap PRIVATE include)

# 链接Qt库 (添加了Qt6::Svg支持SVG图标)
target_link_libraries(openCap PRIVATE Qt6::Core Qt6::Widgets Qt6::Svg)

# 复制icons文件夹到build目录
file(COPY ${CMAKE_SOURCE_DIR}/icons DESTINATION ${CMAKE_BINARY_DIR})

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
    
    # 对于Bundle，也复制icons到Bundle的Resources目录
    add_custom_command(TARGET openCap POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/icons $<TARGET_BUNDLE_DIR:openCap>/Contents/Resources/icons)
endif()

# 安装设置
install(TARGETS openCap
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION .
)

# 安装图标文件
install(DIRECTORY ${CMAKE_SOURCE_DIR}/icons DESTINATION bin) 