#!/bin/bash

# 截图工具测试脚本

echo "=== 截图工具测试脚本 ==="
echo "当前目录: $(pwd)"
echo "应用路径: $(pwd)/build/openCap.app/Contents/MacOS/openCap"

# 检查应用是否存在
if [ ! -f "build/openCap.app/Contents/MacOS/openCap" ]; then
    echo "错误: 找不到应用程序，请先编译"
    echo "运行: cd build && make -j4"
    exit 1
fi

echo ""
echo "准备启动截图应用..."
echo "应用将在状态栏显示图标"
echo "右键或双击图标可开始截图"
echo "按 Ctrl+C 终止应用"
echo ""

# 设置环境变量以显示更多调试信息
export QT_LOGGING_RULES="*.debug=true"

# 启动应用
echo "启动中..."
cd build && ./openCap.app/Contents/MacOS/openCap 