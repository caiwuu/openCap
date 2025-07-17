# Contributing to Jietu | 贡献指南

[English](#english) | [中文](#中文)

## English

Thank you for your interest in contributing to Jietu! 🎉

We welcome contributions of all kinds, including bug reports, feature requests, documentation improvements, and code contributions.

## 📋 Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [Development Setup](#development-setup)
- [Making Changes](#making-changes)
- [Submitting Changes](#submitting-changes)
- [Code Style](#code-style)
- [Testing](#testing)
- [Documentation](#documentation)

## 🤝 Code of Conduct

This project adheres to a [Code of Conduct](CODE_OF_CONDUCT.md). By participating, you are expected to uphold this code.

## 🚀 Getting Started

### Prerequisites

- **Qt6.2+**: Download from [Qt's official website](https://www.qt.io/download)
- **CMake 3.16+**: [Download CMake](https://cmake.org/download/)
- **C++17 Compiler**: GCC 8+, Clang 7+, or MSVC 2017+
- **Git**: For version control

### Development Environment

We recommend using one of the following IDEs:
- **Qt Creator** (recommended for Qt development)
- **CLion** (excellent CMake support)
- **Visual Studio Code** with C++ and CMake extensions

## 🛠️ Development Setup

1. **Fork and Clone**
   ```bash
   git clone https://github.com/yourusername/jietu.git
   cd jietu
   ```

2. **Configure Development Build**
   ```bash
   mkdir build-dev
   cd build-dev
   cmake -DCMAKE_BUILD_TYPE=Debug ..
   ```

3. **Build and Test**
   ```bash
   make -j$(nproc)  # Linux/macOS
   # or
   cmake --build . --config Debug  # Cross-platform
   
   # Run the application
   ./jietu-qt.app/Contents/MacOS/jietu-qt  # macOS
   ./jietu-qt  # Linux
   ```

## 🔄 Making Changes

### Branch Naming Convention

- `feature/description` - New features
- `bugfix/description` - Bug fixes
- `docs/description` - Documentation changes
- `refactor/description` - Code refactoring
- `test/description` - Test improvements

### Development Workflow

1. **Create a Branch**
   ```bash
   git checkout -b feature/your-feature-name
   ```

2. **Make Your Changes**
   - Follow the existing code style
   - Add tests for new functionality
   - Update documentation as needed

3. **Commit Your Changes**
   ```bash
   git add .
   git commit -m "feat: add new screenshot tool"
   ```

### Commit Message Convention

We follow the [Conventional Commits](https://www.conventionalcommits.org/) specification:

- `feat:` - New features
- `fix:` - Bug fixes
- `docs:` - Documentation changes
- `style:` - Code style changes (formatting, etc.)
- `refactor:` - Code refactoring
- `test:` - Test additions or modifications
- `chore:` - Maintenance tasks

Examples:
```
feat: add rectangular selection tool
fix: resolve screenshot overlay positioning issue
docs: update installation instructions for Windows
refactor: extract screenshot rendering logic
test: add unit tests for SystemTray class
```

## 📤 Submitting Changes

1. **Push Your Branch**
   ```bash
   git push origin feature/your-feature-name
   ```

2. **Create a Pull Request**
   - Go to the GitHub repository
   - Click "New Pull Request"
   - Provide a clear title and description
   - Reference any related issues

3. **Pull Request Guidelines**
   - Describe what changes you made and why
   - Include screenshots for UI changes
   - Ensure all tests pass
   - Keep pull requests focused and atomic

## 🎨 Code Style

### C++ Style Guidelines

- **Indentation**: 2 spaces (no tabs)
- **Naming Conventions**:
  - Classes: `PascalCase` (e.g., `ScreenshotOverlay`)
  - Methods: `camelCase` (e.g., `takeScreenshot()`)
  - Variables: `camelCase` with descriptive names
  - Member variables: `m_` prefix (e.g., `m_isSelecting`)
  - Constants: `UPPER_SNAKE_CASE`

- **Header Guards**: Use `#pragma once` or traditional guards
- **Includes**: Group and sort includes (system, Qt, project)

### Example Code Style

```cpp
#pragma once

#include <QWidget>
#include <QPixmap>
#include "jietu/SomeOtherClass.h"

class ScreenshotOverlay : public QWidget {
    Q_OBJECT

public:
    explicit ScreenshotOverlay(QWidget *parent = nullptr);
    ~ScreenshotOverlay();

    void startScreenshot();

private slots:
    void onScreenshotCompleted();

private:
    void updateSelection();
    
    QPixmap m_screenshot;
    bool m_isSelecting;
    static constexpr int DEFAULT_MARGIN = 10;
};
```

### CMake Style

- Use modern CMake (3.16+) practices
- Prefer `target_*` commands over global commands
- Use consistent indentation (2 spaces)

## 🧪 Testing

### Running Tests

```bash
# Build with tests
cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON ..
make -j$(nproc)

# Run tests
ctest --output-on-failure
```

### Adding Tests

- Place test files in `tests/` directory
- Use descriptive test names
- Test both positive and negative cases
- Mock external dependencies when possible

## 📖 Documentation

### Documentation Guidelines

- Keep documentation up to date with code changes
- Use clear and concise language
- Include code examples where helpful
- Update README.md for user-facing changes

### Building Documentation

```bash
# If using Doxygen (optional)
doxygen Doxyfile
```

## 🐛 Reporting Issues

### Bug Reports

When reporting bugs, please include:
- Operating system and version
- Qt version
- Steps to reproduce the issue
- Expected vs. actual behavior
- Screenshots (if applicable)
- Relevant log output

### Feature Requests

For feature requests, please describe:
- The problem you're trying to solve
- Your proposed solution
- Any alternative solutions considered
- Additional context or screenshots

## 💬 Getting Help

- **GitHub Issues**: For bug reports and feature requests
- **GitHub Discussions**: For general questions and community chat
- **Documentation**: Check the `docs/` directory for detailed guides

## 🙏 Recognition

Contributors will be recognized in:
- The main README.md file
- Release notes for significant contributions
- The project's Hall of Fame (if established)

## 📄 License

By contributing to Jietu, you agree that your contributions will be licensed under the same [MIT License](LICENSE) that covers the project.

---

Thank you for contributing to Jietu! 🚀

---

## 中文

感谢您对贡献Jietu项目的兴趣！🎉

我们欢迎各种形式的贡献，包括错误报告、功能请求、文档改进和代码贡献。

## 📋 目录

- [行为准则](#行为准则)
- [快速开始](#快速开始)
- [开发环境设置](#开发环境设置)
- [进行更改](#进行更改)
- [提交更改](#提交更改)
- [代码风格](#代码风格)
- [测试](#测试)
- [文档](#文档)

## 🤝 行为准则

本项目遵循[行为准则](CODE_OF_CONDUCT.md)。参与项目即表示您同意遵守此准则。

## 🚀 快速开始

### 前置要求

- **Qt6.2+**：从[Qt官网](https://www.qt.io/download)下载
- **CMake 3.16+**：[下载CMake](https://cmake.org/download/)
- **C++17编译器**：GCC 8+、Clang 7+ 或 MSVC 2017+
- **Git**：用于版本控制

### 开发环境

我们推荐使用以下IDE之一：
- **Qt Creator**（推荐用于Qt开发）
- **CLion**（优秀的CMake支持）
- **Visual Studio Code**（配合C++和CMake扩展）

## 🛠️ 开发环境设置

1. **分叉和克隆**
   ```bash
   git clone https://github.com/yourusername/jietu.git
   cd jietu
   ```

2. **配置开发构建**
   ```bash
   mkdir build-dev
   cd build-dev
   cmake -DCMAKE_BUILD_TYPE=Debug ..
   ```

3. **构建和测试**
   ```bash
   make -j$(nproc)  # Linux/macOS
   # 或者
   cmake --build . --config Debug  # 跨平台
   
   # 运行应用程序
   ./jietu-qt.app/Contents/MacOS/jietu-qt  # macOS
   ./jietu-qt  # Linux
   ```

## 🔄 进行更改

### 分支命名约定

- `feature/描述` - 新功能
- `bugfix/描述` - 错误修复
- `docs/描述` - 文档更改
- `refactor/描述` - 代码重构
- `test/描述` - 测试改进

### 开发工作流

1. **创建分支**
   ```bash
   git checkout -b feature/your-feature-name
   ```

2. **进行更改**
   - 遵循现有代码风格
   - 为新功能添加测试
   - 根据需要更新文档

3. **提交更改**
   ```bash
   git add .
   git commit -m "feat: 添加新的截图工具"
   ```

### 提交信息约定

我们遵循[约定式提交](https://www.conventionalcommits.org/)规范：

- `feat:` - 新功能
- `fix:` - 错误修复
- `docs:` - 文档更改
- `style:` - 代码风格更改（格式化等）
- `refactor:` - 代码重构
- `test:` - 测试添加或修改
- `chore:` - 维护任务

示例：
```
feat: 添加矩形选择工具
fix: 解决截图覆盖层定位问题
docs: 更新Windows安装说明
refactor: 提取截图渲染逻辑
test: 为SystemTray类添加单元测试
```

## 📤 提交更改

1. **推送分支**
   ```bash
   git push origin feature/your-feature-name
   ```

2. **创建Pull Request**
   - 前往GitHub仓库
   - 点击"New Pull Request"
   - 提供清晰的标题和描述
   - 引用相关问题

3. **Pull Request指南**
   - 描述您做了什么更改以及为什么
   - 为UI更改包含截图
   - 确保所有测试通过
   - 保持pull request专注和原子化

## 🎨 代码风格

### C++风格指南

- **缩进**：2个空格（不使用制表符）
- **命名约定**：
  - 类：`PascalCase`（如 `ScreenshotOverlay`）
  - 方法：`camelCase`（如 `takeScreenshot()`）
  - 变量：`camelCase`，使用描述性名称
  - 成员变量：`m_`前缀（如 `m_isSelecting`）
  - 常量：`UPPER_SNAKE_CASE`

- **头文件保护**：使用 `#pragma once` 或传统保护
- **包含顺序**：对包含进行分组和排序（系统、Qt、项目）

### 示例代码风格

```cpp
#pragma once

#include <QWidget>
#include <QPixmap>
#include "jietu/SomeOtherClass.h"

class ScreenshotOverlay : public QWidget {
    Q_OBJECT

public:
    explicit ScreenshotOverlay(QWidget *parent = nullptr);
    ~ScreenshotOverlay();

    void startScreenshot();

private slots:
    void onScreenshotCompleted();

private:
    void updateSelection();
    
    QPixmap m_screenshot;
    bool m_isSelecting;
    static constexpr int DEFAULT_MARGIN = 10;
};
```

### CMake风格

- 使用现代CMake（3.16+）实践
- 优先使用 `target_*` 命令而不是全局命令
- 使用一致的缩进（2个空格）

## 🧪 测试

### 运行测试

```bash
# 构建测试
cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON ..
make -j$(nproc)

# 运行测试
ctest --output-on-failure
```

### 添加测试

- 将测试文件放在 `tests/` 目录中
- 使用描述性的测试名称
- 测试正面和负面情况
- 尽可能模拟外部依赖

## 📖 文档

### 文档指南

- 保持文档与代码更改同步
- 使用清晰简洁的语言
- 在有帮助的地方包含代码示例
- 为面向用户的更改更新README.md

### 构建文档

```bash
# 如果使用Doxygen（可选）
doxygen Doxyfile
```

## 🐛 报告问题

### 错误报告

报告错误时，请包含：
- 操作系统和版本
- Qt版本
- 重现问题的步骤
- 预期与实际行为
- 截图（如适用）
- 相关日志输出

### 功能请求

对于功能请求，请描述：
- 您试图解决的问题
- 您提议的解决方案
- 考虑的任何替代解决方案
- 附加上下文或截图

## 💬 获取帮助

- **GitHub Issues**：用于错误报告和功能请求
- **GitHub Discussions**：用于一般问题和社区聊天
- **文档**：查看 `docs/` 目录获取详细指南

## 🙏 认可

贡献者将在以下地方得到认可：
- 主README.md文件
- 重大贡献的发布说明
- 项目名人堂（如果建立）

## 📄 许可证

通过为Jietu做贡献，您同意您的贡献将在与项目相同的[MIT许可证](LICENSE)下授权。

---

感谢您为Jietu做贡献！🚀 