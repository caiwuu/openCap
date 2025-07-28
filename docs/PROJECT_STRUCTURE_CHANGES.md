# 项目结构变更操作指南

## 🎯 概述

当您调整项目的目录结构后（如移动文件、重命名目录、添加新模块等），需要同步更新相关配置文件以确保编辑器能正确识别代码。

## 🚀 快速操作

### 方法1：使用自动化脚本（推荐）

```bash
# 在项目根目录运行
./scripts/sync_project_structure.sh
```

这个脚本会自动：
- 🔍 发现所有头文件和源文件
- 📝 更新 `openCap.pro` 文件
- ⚙️ 更新 VSCode 配置
- 🔧 重新生成 `compile_commands.json`

### 方法2：手动操作

如果脚本无法满足特殊需求，可以手动执行以下步骤：

#### 步骤1：重新生成编译配置
```bash
./scripts/update_compile_commands.sh
```

#### 步骤2：更新VSCode配置
编辑 `.vscode/c_cpp_properties.json`，添加新的包含路径。

#### 步骤3：更新qmake项目文件
编辑 `openCap.pro`，更新 `HEADERS` 和 `SOURCES` 列表。

## 📋 详细操作流程

### 1. 调整目录结构
```bash
# 示例：移动文件到新目录
mkdir -p src/new_module
mv src/old_file.cpp src/new_module/
mv src/old_file.h src/new_module/
```

### 2. 运行同步脚本
```bash
./scripts/sync_project_structure.sh
```

### 3. 验证配置
```bash
# 检查编译是否正常
cd build
make clean
make

# 检查编辑器是否能识别类型
# 在VSCode中打开任意源文件，确认没有红色波浪线
```

### 4. 重启编辑器
- 重启 VSCode 或重新加载窗口
- 按 `Cmd+Shift+P`，运行 `C/C++: Reload IntelliSense Database`

## 🔧 脚本功能详解

### `sync_project_structure.sh`

这个脚本提供以下功能：

#### 🔍 自动文件发现
- **头文件发现**：自动查找所有 `.h` 文件
- **源文件发现**：自动查找所有 `.cpp` 和 `.mm` 文件
- **目录发现**：自动发现所有包含头文件的目录

#### 📝 配置文件更新
- **openCap.pro**：自动生成正确的文件列表
- **VSCode配置**：自动更新包含路径
- **编译数据库**：重新生成 `compile_commands.json`

#### ✅ 验证和反馈
- 显示发现的文件数量
- 提供详细的操作建议
- 彩色输出便于阅读

## 🎯 常见场景

### 场景1：添加新模块
```bash
# 1. 创建新模块目录
mkdir -p src/new_feature

# 2. 添加文件
touch src/new_feature/NewClass.h
touch src/new_feature/NewClass.cpp

# 3. 运行同步脚本
./scripts/sync_project_structure.sh
```

### 场景2：重构现有模块
```bash
# 1. 重新组织文件
mkdir -p src/screenshot/advanced
mv src/screenshot/core/AdvancedFeature.* src/screenshot/advanced/

# 2. 运行同步脚本
./scripts/sync_project_structure.sh
```

### 场景3：删除废弃文件
```bash
# 1. 删除文件
rm src/old_module/OldFile.*

# 2. 运行同步脚本
./scripts/sync_project_structure.sh
```

## ⚠️ 注意事项

### 1. 文件命名规范
- 头文件使用 `.h` 扩展名
- 源文件使用 `.cpp` 或 `.mm` 扩展名
- 避免使用特殊字符和空格

### 2. 目录结构建议
```
src/
├── module1/
│   ├── Class1.h
│   └── Class1.cpp
├── module2/
│   ├── Class2.h
│   └── Class2.cpp
└── main.cpp
```

### 3. 包含路径规范
- 使用相对路径包含头文件
- 避免使用绝对路径
- 保持包含路径的一致性

## 🐛 故障排除

### 问题1：脚本运行失败
```bash
# 检查脚本权限
chmod +x scripts/sync_project_structure.sh

# 检查是否在项目根目录
ls CMakeLists.txt
```

### 问题2：编辑器仍然报错
```bash
# 1. 重新生成编译配置
./scripts/update_compile_commands.sh

# 2. 重启VSCode
# 3. 重新加载IntelliSense数据库
```

### 问题3：编译失败
```bash
# 1. 清理构建目录
cd build
make clean

# 2. 重新配置
cmake ..

# 3. 重新编译
make
```

## 📚 相关文档

- [编辑器设置指南](EDITOR_SETUP.md)
- [项目结构说明](PROJECT_STRUCTURE.md)
- [开发环境配置](CONTRIBUTING.md)

## 🎉 总结

通过使用自动化脚本，您可以轻松地管理项目结构变更，确保所有配置文件保持同步。记住：

1. **每次调整目录结构后运行同步脚本**
2. **验证编译和编辑器功能正常**
3. **保持文件命名和目录结构的一致性**

这样可以避免配置不一致导致的问题，提高开发效率！ 