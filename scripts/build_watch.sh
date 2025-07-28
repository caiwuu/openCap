#!/bin/bash

# 开发模式热更新脚本
# 监控源文件变化，自动重新编译和重启

echo "=== 开发模式热更新 ==="
echo "监控源文件变化，自动重新编译和重启"
echo "按 Ctrl+C 退出"
echo ""

# 检查 fswatch 是否安装
if ! command -v fswatch &> /dev/null; then
    echo "错误: 需要安装 fswatch"
    echo "安装命令: brew install fswatch"
    exit 1
fi

# 检查是否在项目根目录
if [ ! -f "CMakeLists.txt" ]; then
    echo "错误: 请在项目根目录运行此脚本"
    exit 1
fi

# 确保构建目录存在
if [ ! -d "build" ]; then
    echo "创建构建目录..."
    mkdir -p build
    cd build
    cmake ..
    cd ..
fi

# 启动应用
echo "启动应用..."
pkill -f "openCap" 2>/dev/null || true
sleep 1
./build/openCap.app/Contents/MacOS/openCap &
APP_PID=$!

echo "应用已启动 (PID: $APP_PID)"
echo "开始监控文件变化..."

# 监控源文件变化
fswatch -o build/ | while read f; do
    echo ""
    echo "检测到重新编译，重新启动..."
    # 关闭旧进程
        pkill -f "openCap" 2>/dev/null || true
        sleep 1
        
        # 启动新进程
        ./build/openCap.app/Contents/MacOS/openCap &
        APP_PID=$!
        echo "应用已重启 (PID: $APP_PID)"
done 