# Jietu - QQ Style Screenshot Tool

[![CI/CD](https://github.com/yourusername/jietu/workflows/CI%2FCD%20Pipeline/badge.svg)](https://github.com/yourusername/jietu/actions)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Qt Version](https://img.shields.io/badge/Qt-6.2+-blue.svg)](https://www.qt.io/)
[![Platform](https://img.shields.io/badge/Platform-macOS%20%7C%20Linux%20%7C%20Windows-lightgrey.svg)](https://github.com/yourusername/jietu)
[![GitHub release](https://img.shields.io/github/release/yourusername/jietu.svg)](https://github.com/yourusername/jietu/releases)
[![GitHub stars](https://img.shields.io/github/stars/yourusername/jietu.svg?style=social&label=Star)](https://github.com/yourusername/jietu)

[English](#english) | [中文](#中文)

## English

A QQ-style screenshot tool built with Qt6, providing a seamless and intuitive screen capture experience.

### ✨ Features

- 🎯 **QQ-style Interface**: Perfectly mimics QQ's screenshot functionality
- 📱 **System Tray Integration**: Runs in background with system tray icon
- 🖼️ **Instant Screen Freeze**: Captures and freezes current screen state immediately
- 🎨 **Drag Selection**: Interactive area selection with real-time coordinate display
- ⌨️ **Keyboard Shortcuts**: ESC to cancel, mouse interactions for selection
- 💾 **Auto Save**: Automatically saves to desktop with timestamp
- 🔍 **High-DPI Support**: Perfect support for Retina and high-resolution displays
- 🍎 **Native macOS Integration**: Uses Cocoa APIs for true fullscreen capture

### 📋 Requirements

- **Operating System**: macOS 10.15+ (primary), Windows 10+, Linux (experimental)
- **Qt Version**: Qt6.2 or later
- **Build Tools**: CMake 3.16+, C++17 compiler
- **macOS Permissions**: Screen Recording permission required

### 🚀 Quick Start

#### Installation

**macOS (Homebrew)**:
```bash
# Install dependencies
brew install qt6 cmake

# Clone and build
git clone https://github.com/yourusername/jietu.git
cd jietu
mkdir build && cd build
cmake ..
make -j$(sysctl -n hw.ncpu)

# Run
./jietu-qt.app/Contents/MacOS/jietu-qt
```

**Build from Source**:
```bash
# Clone repository
git clone https://github.com/yourusername/jietu.git
cd jietu

# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
cmake --build . --config Release

# Run the application
./jietu-qt.app/Contents/MacOS/jietu-qt  # macOS
# or
./jietu-qt  # Linux/Windows
```

#### First Run Setup

1. **Grant Permissions**: On macOS, go to "System Preferences → Security & Privacy → Privacy → Screen Recording" and add Jietu
2. **System Tray**: Look for the Jietu icon in your system tray/menu bar
3. **Start Screenshot**: Double-click the tray icon or right-click → "Take Screenshot"

### 📖 Usage

1. **Launch**: Application runs in system tray (no main window)
2. **Screenshot**: 
   - Double-click tray icon, or
   - Right-click tray icon → "Take Screenshot"
3. **Select Area**: Click and drag to select screenshot region
4. **Complete**: Release mouse to auto-save to desktop
5. **Cancel**: Press `ESC` key
6. **Exit**: Right-click tray icon → "Exit"

### 🏗️ Project Structure

```
jietu/
├── include/jietu/          # Header files
│   ├── ScreenshotApp.h     # Main application
│   ├── ScreenshotOverlay.h # Screenshot overlay window
│   ├── ScreenshotRenderer.h # Rendering engine
│   ├── ScreenshotToolbar.h # Toolbar component
│   ├── SystemTray.h        # System tray management
│   └── MacWindowLevel.h    # macOS window level control
├── src/                    # Source files
├── docs/                   # Documentation
├── icons/                  # Application icons
├── scripts/                # Build and test scripts
├── CMakeLists.txt          # CMake configuration
└── Info.plist             # macOS bundle info
```

### 🛠️ Development

```bash
# Development build with debug info
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j$(nproc)

# Run tests
./scripts/test_jietu.sh

# Code formatting (if available)
clang-format -i src/*.cpp include/jietu/*.h
```

#### Adding New Source Files

The project uses CMake's `GLOB_RECURSE` to automatically detect source files. When you add new `.cpp` or `.h` files:

1. **Add your files** to the appropriate directories:
   - Source files: `src/`
   - Header files: `include/jietu/`

2. **Reconfigure CMake** to detect new files:
   ```bash
   # Reconfigure CMake
   cmake -B build .
   
   # Or clean and reconfigure (if having issues)
   rm -rf build && cmake -B build .
   ```

3. **Build as usual**:
   ```bash
   cmake --build build -j$(nproc)
   ```

**Note**: Unlike manual file listing, with `GLOB_RECURSE` you don't need to edit `CMakeLists.txt` when adding new files, just reconfigure CMake.

### 🤝 Contributing

We welcome contributions! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for details.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

### 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

### 🙏 Acknowledgments

- Inspired by QQ's screenshot functionality
- Built with [Qt6](https://www.qt.io/) framework
- Uses native macOS Cocoa APIs for optimal integration

---

## 中文

一个使用 Qt6 开发的仿QQ截图工具，提供流畅直观的屏幕截图体验。

### ✨ 功能特点

- 🎯 **QQ风格界面**: 完美复刻QQ截图功能
- 📱 **系统托盘**: 后台运行，托盘图标控制
- 🖼️ **瞬间冻结**: 立即捕获并冻结当前屏幕状态
- 🎨 **拖拽选择**: 交互式区域选择，实时显示坐标
- ⌨️ **快捷键**: ESC取消，鼠标交互选择
- 💾 **自动保存**: 自动保存到桌面，带时间戳
- 🔍 **高分辨率支持**: 完美支持Retina和高分辨率显示器
- 🍎 **原生macOS集成**: 使用Cocoa API实现真正的全屏截图

### 📋 系统要求

- **操作系统**: macOS 10.15+ (主要支持), Windows 10+, Linux (实验性)
- **Qt版本**: Qt6.2 或更高版本
- **构建工具**: CMake 3.16+, C++17 编译器
- **macOS权限**: 需要屏幕录制权限

### 🚀 快速开始

#### 安装方法

**macOS (Homebrew)**:
```bash
# 安装依赖
brew install qt6 cmake

# 克隆并构建
git clone https://github.com/yourusername/jietu.git
cd jietu
mkdir build && cd build
cmake ..
make -j$(sysctl -n hw.ncpu)

# 运行
./jietu-qt.app/Contents/MacOS/jietu-qt
```

#### 首次运行设置

1. **授予权限**: 在macOS上，前往"系统偏好设置 → 安全性与隐私 → 隐私 → 屏幕录制"添加Jietu
2. **系统托盘**: 在系统托盘/菜单栏查找Jietu图标
3. **开始截图**: 双击托盘图标或右键 → "截图"

### 📖 使用方法

1. **启动**: 应用在系统托盘运行（无主窗口）
2. **截图**: 双击托盘图标，或右键托盘图标 → "截图"
3. **选择区域**: 点击拖拽选择截图区域
4. **完成**: 释放鼠标自动保存到桌面
5. **取消**: 按 `ESC` 键
6. **退出**: 右键托盘图标 → "退出"

#### 添加新源文件

项目使用CMake的 `GLOB_RECURSE` 来自动检测源文件。当您添加新的 `.cpp` 或 `.h` 文件时：

1. **添加文件** 到相应目录：
   - 源文件：`src/`
   - 头文件：`include/jietu/`

2. **重新配置CMake** 以检测新文件：
   ```bash
   # 重新配置CMake
   cmake -B build .
   
   # 或者清理后重新配置（如有问题）
   rm -rf build && cmake -B build .
   ```

3. **正常构建**：
   ```bash
   cmake --build build -j$(nproc)
   ```

**注意**：与手动列出文件不同，使用 `GLOB_RECURSE` 时添加新文件无需编辑 `CMakeLists.txt`，只需重新配置CMake。

### 🤝 贡献

欢迎贡献代码！请查看 [CONTRIBUTING.md](CONTRIBUTING.md) 了解详情。

### 📝 许可证

本项目采用 MIT 许可证 - 查看 [LICENSE](LICENSE) 文件了解详情。

### 🙏 致谢

- 灵感来源于QQ的截图功能
- 使用 [Qt6](https://www.qt.io/) 框架构建
- 使用原生 macOS Cocoa API 实现最佳集成

---

**Star ⭐ this repository if you find it helpful!** 