# 项目目录结构说明

## 📁 重构后的目录结构

```
jietu/
├── include/jietu/          # 头文件目录
│   ├── MacWindowLevel.h
│   ├── ScreenshotApp.h
│   ├── ScreenshotOverlay.h
│   ├── ScreenshotRenderer.h
│   └── SystemTray.h
├── src/                    # 源文件目录
│   ├── MacWindowLevel.mm
│   ├── ScreenshotApp.cpp
│   ├── ScreenshotOverlay.cpp
│   ├── ScreenshotRenderer.cpp
│   ├── SystemTray.cpp
│   └── main.cpp
├── docs/                   # 文档目录
│   ├── PERFORMANCE_OPTIMIZATION.md
│   ├── README.md
│   ├── USAGE.md
│   └── PROJECT_STRUCTURE.md
├── scripts/                # 脚本目录
│   ├── start_jietu.sh
│   └── test_jietu.sh
├── build/                  # 构建输出目录
├── CMakeLists.txt          # CMake构建配置
└── Info.plist             # macOS应用配置
```

## ✨ 重构的优势

### 1. **清晰的接口分离**
- **头文件** (`include/jietu/`): 定义类的接口和公共API
- **源文件** (`src/`): 实现具体功能，隐藏实现细节

### 2. **命名空间式的头文件组织**
- 使用 `jietu/` 子目录避免头文件名冲突
- 在源文件中使用 `#include "jietu/ClassName.h"` 形式

### 3. **符合C++最佳实践**
- **标准库开发风格**: 类似于 `<vector>`, `<iostream>` 的组织方式
- **易于维护**: 头文件和实现分离，便于代码阅读和维护
- **团队协作友好**: 清晰的结构便于团队成员理解项目

### 4. **构建系统优化**
- CMakeLists.txt 使用 `target_include_directories` 设置包含路径
- 支持现代CMake最佳实践

## 🔧 技术实现细节

### Include 路径设置
```cmake
# CMakeLists.txt 中的关键配置
include_directories(include)
target_include_directories(jietu-qt PRIVATE include)
```

### 源文件中的 Include 语句
```cpp
// 正确的include方式
#include "jietu/ScreenshotApp.h"
#include "jietu/SystemTray.h"
```

## 📋 C++ 项目最佳实践总结

1. **头文件与源文件分离**: 将接口定义和实现分开
2. **使用子目录**: 在include下创建项目名子目录避免冲突
3. **文档分类**: 将不同类型的文档放在docs目录
4. **脚本分类**: 将构建和测试脚本放在scripts目录
5. **现代CMake**: 使用target-based的CMake命令而非全局命令

## 🚀 构建项目

```bash
# 进入构建目录
cd build

# 配置项目
cmake ..

# 编译项目
make -j$(nproc)

# 运行应用
./jietu-qt.app/Contents/MacOS/jietu-qt
```

这种目录结构适用于中小型C++项目，既保持了代码的清晰组织，又符合现代C++开发的最佳实践。 