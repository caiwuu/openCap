# Contributing to Jietu

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