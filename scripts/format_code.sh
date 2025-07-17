#!/bin/bash

# Jietu代码格式化脚本
# 使用clang-format格式化所有C++源文件

set -e  # 遇到错误时退出

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 项目根目录
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

echo -e "${BLUE}🎨 Jietu代码格式化工具${NC}"
echo -e "${BLUE}=============================${NC}"

# 检查clang-format是否安装
if ! command -v clang-format &> /dev/null; then
    echo -e "${RED}❌ 错误: clang-format未安装${NC}"
    echo -e "${YELLOW}请安装clang-format:${NC}"
    echo -e "  macOS: brew install clang-format"
    echo -e "  Ubuntu: sudo apt-get install clang-format"
    echo -e "  Windows: 通过LLVM安装包安装"
    exit 1
fi

# 检查.clang-format配置文件
if [ ! -f "$PROJECT_ROOT/.clang-format" ]; then
    echo -e "${RED}❌ 错误: 未找到.clang-format配置文件${NC}"
    exit 1
fi

echo -e "${GREEN}✅ clang-format已安装${NC}"
echo -e "${GREEN}✅ 配置文件存在${NC}"
echo ""

# 查找所有C++源文件
echo -e "${BLUE}🔍 查找C++源文件...${NC}"

# 查找源文件
SOURCES=$(find "$PROJECT_ROOT/src" -name "*.cpp" -o -name "*.mm" 2>/dev/null || true)
HEADERS=$(find "$PROJECT_ROOT/include" -name "*.h" 2>/dev/null || true)

# 合并文件列表
FILES="$SOURCES $HEADERS"

if [ -z "$FILES" ]; then
    echo -e "${YELLOW}⚠️  未找到C++源文件${NC}"
    exit 0
fi

# 计算文件数量
FILE_COUNT=$(echo $FILES | wc -w | tr -d ' ')
echo -e "${GREEN}📁 找到 $FILE_COUNT 个文件${NC}"

# 显示文件列表
echo -e "${BLUE}文件列表:${NC}"
for file in $FILES; do
    # 显示相对路径
    rel_path=$(realpath --relative-to="$PROJECT_ROOT" "$file" 2>/dev/null || echo "$file")
    echo "  - $rel_path"
done
echo ""

# 检查格式化选项
DRY_RUN=false
VERBOSE=false

for arg in "$@"; do
    case $arg in
        --dry-run)
            DRY_RUN=true
            shift
            ;;
        --verbose)
            VERBOSE=true
            shift
            ;;
        --help)
            echo "用法: $0 [选项]"
            echo ""
            echo "选项:"
            echo "  --dry-run    检查格式但不修改文件"
            echo "  --verbose    显示详细输出"
            echo "  --help       显示此帮助信息"
            echo ""
            echo "示例:"
            echo "  $0                # 格式化所有文件"
            echo "  $0 --dry-run      # 检查格式但不修改"
            echo "  $0 --verbose      # 显示详细过程"
            exit 0
            ;;
    esac
done

# 格式化文件
if [ "$DRY_RUN" = true ]; then
    echo -e "${YELLOW}🔍 检查代码格式 (不修改文件)...${NC}"
    
    HAS_ISSUES=false
    for file in $FILES; do
        if [ "$VERBOSE" = true ]; then
            echo -e "${BLUE}检查: $file${NC}"
        fi
        
        if ! clang-format --dry-run --Werror "$file" &>/dev/null; then
            echo -e "${RED}❌ 格式问题: $(realpath --relative-to="$PROJECT_ROOT" "$file")${NC}"
            HAS_ISSUES=true
        elif [ "$VERBOSE" = true ]; then
            echo -e "${GREEN}✅ 格式正确: $(realpath --relative-to="$PROJECT_ROOT" "$file")${NC}"
        fi
    done
    
    if [ "$HAS_ISSUES" = true ]; then
        echo ""
        echo -e "${RED}❌ 发现格式问题${NC}"
        echo -e "${YELLOW}运行 '$0' 自动修复格式问题${NC}"
        exit 1
    else
        echo -e "${GREEN}✅ 所有文件格式正确${NC}"
        exit 0
    fi
else
    echo -e "${YELLOW}🔧 格式化代码文件...${NC}"
    
    FORMATTED_COUNT=0
    for file in $FILES; do
        if [ "$VERBOSE" = true ]; then
            echo -e "${BLUE}格式化: $file${NC}"
        fi
        
        # 备份原文件内容
        ORIGINAL_CONTENT=$(cat "$file")
        
        # 格式化文件
        clang-format -i "$file"
        
        # 检查是否有变化
        NEW_CONTENT=$(cat "$file")
        if [ "$ORIGINAL_CONTENT" != "$NEW_CONTENT" ]; then
            echo -e "${GREEN}✅ 已格式化: $(realpath --relative-to="$PROJECT_ROOT" "$file")${NC}"
            FORMATTED_COUNT=$((FORMATTED_COUNT + 1))
        elif [ "$VERBOSE" = true ]; then
            echo -e "${BLUE}无需修改: $(realpath --relative-to="$PROJECT_ROOT" "$file")${NC}"
        fi
    done
    
    echo ""
    if [ $FORMATTED_COUNT -eq 0 ]; then
        echo -e "${GREEN}✅ 所有文件已经符合格式规范${NC}"
    else
        echo -e "${GREEN}✅ 已格式化 $FORMATTED_COUNT 个文件${NC}"
        echo -e "${YELLOW}💡 建议使用 git diff 检查更改${NC}"
    fi
fi

echo ""
echo -e "${GREEN}🎉 代码格式化完成!${NC}" 