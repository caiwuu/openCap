#!/bin/bash

# 截图工具启动脚本

echo "正在启动截图工具..."

# 检查是否已经在运行
if pgrep -f "openCap" > /dev/null; then
    echo "截图工具已经在运行中！"
    exit 0
fi

# 切换到构建目录
cd "$(dirname "$0")/build"

# 启动应用程序
./openCap.app/Contents/MacOS/openCap &

echo "截图工具已启动！"
echo ""
echo "使用方法："
echo "1. 在状态栏找到截图工具图标"
echo "2. 双击图标或右键选择'截图'开始截图"
echo "3. 拖拽选择要截图的区域"
echo "4. 松开鼠标完成截图，图片保存到桌面"
echo "5. 按ESC键可以取消截图"
echo "6. 右键图标选择'退出'可以关闭程序"
echo "" 