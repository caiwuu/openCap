# Changelog | 更新日志

[English](#english) | [中文](#中文)

## English

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- Toolbar with drawing tools (rectangle, arrow, text, etc.)
- Advanced screenshot renderer with performance optimizations
- High-DPI display support improvements

### Changed
- Improved CMake configuration with automatic file detection
- Enhanced project structure for better maintainability

### Fixed
- Memory optimization for screenshot rendering
- Cursor positioning accuracy on high-DPI displays

## [1.0.0] - 2024-01-XX

### Added
- ✨ QQ-style screenshot functionality
- 📱 System tray integration with context menu
- 🖼️ Instant screen freeze and capture
- 🎨 Interactive area selection with real-time feedback
- 💾 Automatic saving to desktop with timestamps
- 🔍 Perfect Retina and high-DPI display support
- 🍎 Native macOS integration using Cocoa APIs
- ⌨️ Keyboard shortcuts (ESC to cancel)
- 🎯 Precise selection with coordinate display
- 📏 Real-time size and position information
- 🖱️ Smooth drag selection interface
- 🏗️ Modern Qt6-based architecture
- 🔧 CMake build system with cross-platform support
- 📖 Comprehensive documentation

### Technical Features
- **Cross-platform compatibility**: Primary support for macOS 10.15+
- **Native window management**: Uses Cocoa APIs for proper window layering
- **Memory efficient**: Optimized screenshot rendering and caching
- **Modern C++17**: Clean, maintainable codebase
- **Qt6 integration**: Leverages latest Qt framework features
- **High performance**: Native screen capture APIs for responsiveness

### Initial Release Notes
This is the first stable release of OpenCap, providing a complete QQ-style screenshot experience on macOS. The application runs silently in the system tray and provides instant, high-quality screenshot capabilities with an intuitive user interface.

### Known Issues
- Windows and Linux support is experimental in this release
- Some edge cases with multi-monitor setups may need refinement

---

## Release Notes Format

Each release follows this structure:

### Added
- New features and capabilities

### Changed
- Changes to existing functionality

### Deprecated
- Features that will be removed in future versions

### Removed
- Features removed in this version

### Fixed
- Bug fixes and issue resolutions

### Security
- Security-related changes and fixes

---

**Full Changelog**: [Compare releases](https://github.com/yourusername/openCap/releases)

---

## 中文

此文件记录项目的所有重要变更。

格式基于 [Keep a Changelog](https://keepachangelog.com/en/1.0.0/)，
项目遵循 [语义化版本](https://semver.org/spec/v2.0.0.html)。

## [未发布]

### 新增
- 工具栏和绘图工具（矩形、箭头、文字等）
- 高级截图渲染器，性能优化
- 高分辨率显示器支持改进

### 变更
- 改进CMake配置，支持自动文件检测
- 增强项目结构，提高可维护性

### 修复
- 截图渲染内存优化
- 高分辨率显示器上的光标定位精度

## [1.0.0] - 2024-01-XX

### 新增功能
- ✨ QQ风格截图功能
- 📱 系统托盘集成和右键菜单
- 🖼️ 瞬间屏幕冻结和捕获
- 🎨 交互式区域选择，实时反馈
- 💾 自动保存到桌面，带时间戳
- 🔍 完美支持Retina和高分辨率显示器
- 🍎 原生macOS集成，使用Cocoa API
- ⌨️ 键盘快捷键（ESC取消）
- 🎯 精确选择，坐标显示
- 📏 实时尺寸和位置信息
- 🖱️ 流畅的拖拽选择界面
- 🏗️ 现代Qt6架构
- 🔧 CMake构建系统，跨平台支持
- 📖 完整文档

### 技术特性
- **跨平台兼容性**：主要支持macOS 10.15+
- **原生窗口管理**：使用Cocoa API进行窗口层级控制
- **内存高效**：优化的截图渲染和缓存
- **现代C++17**：清晰、可维护的代码库
- **Qt6集成**：利用最新Qt框架特性
- **高性能**：原生屏幕捕获API，响应迅速

### 初始版本说明
这是OpenCap的首个稳定版本，在macOS上提供完整的QQ风格截图体验。应用程序在系统托盘中静默运行，提供即时、高质量的截图功能和直观的用户界面。

### 已知问题
- Windows和Linux支持在此版本中为实验性
- 多显示器设置中的一些边缘情况可能需要改进

---

## 发布说明格式

每个版本遵循以下结构：

### 新增
- 新功能和能力

### 变更
- 现有功能的更改

### 弃用
- 将在未来版本中移除的功能

### 移除
- 本版本中移除的功能

### 修复
- Bug修复和问题解决

### 安全
- 安全相关的更改和修复

---

**完整更新日志**: [比较版本](https://github.com/yourusername/openCap/releases) 