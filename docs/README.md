# 截图工具 (Qt版本)

一个模仿QQ截图功能的简洁截图工具，使用Qt6开发。

## 功能特点

- 🎯 完全复刻QQ截图的交互体验
- 📱 系统托盘运行，不占用任务栏空间
- 🖼️ 瞬间冻结屏幕画面，真正的静态截图
- 🎨 拖拽选择区域，实时显示坐标和尺寸
- ⌨️ ESC键取消截图，双击托盘图标快速截图
- 💾 自动保存到桌面，文件名包含时间戳

## 系统要求

- macOS 10.15+ 或 Windows 10+ 或 Linux
- Qt6.2+
- CMake 3.16+
- C++17 编译器

## 构建说明

### macOS

1. 安装Qt6和CMake：
```bash
brew install qt6 cmake
```

2. 构建项目：
```bash
mkdir build
cd build
cmake ..
make
```

3. 运行：
```bash
./openCap.app/Contents/MacOS/openCap
```

### Windows

1. 安装Qt6和CMake
2. 使用Qt Creator打开CMakeLists.txt
3. 或使用命令行：
```cmd
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019"
cmake --build . --config Release
```

## 使用方法

1. 启动程序后，会在系统托盘显示图标
2. 双击托盘图标或右键选择"截图"开始截图
3. 按住左键拖拽选择截图区域
4. 松开鼠标完成截图，图片自动保存到桌面
5. 按ESC键取消截图
6. 右键托盘图标选择"退出"关闭程序

## 实现原理

1. **瞬间冻结画面**：使用`QScreen::grabWindow(0)`立即捕获全屏图像
2. **静态背景渲染**：将捕获的图像作为背景绘制，确保画面完全静止
3. **半透明遮罩**：在未选择区域绘制半透明黑色遮罩
4. **实时区域选择**：通过鼠标事件实现拖拽选择，实时更新界面
5. **无边框全屏窗口**：创建覆盖整个屏幕的无边框窗口

## 文件结构

```
openCap/
├── CMakeLists.txt          # 构建配置
├── main.cpp                # 程序入口
├── ScreenshotApp.h/cpp     # 主应用程序类
├── SystemTray.h/cpp        # 系统托盘管理
├── ScreenshotOverlay.h/cpp # 截图覆盖窗口
├── Info.plist              # macOS应用信息
└── README.md               # 项目说明
```

## 技术特点

- 使用Qt6的现代C++特性
- 完全跨平台兼容
- 内存安全的智能指针管理
- 高效的绘图和事件处理
- 系统原生的屏幕捕获API

## 许可证

MIT License 