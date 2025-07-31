#!/bin/bash

echo "=== Intel Mac构建脚本 ==="
echo "目标架构: x86_64"
echo "机器类型: $(uname -m)"

# 检查是否为Intel Mac
if [ "$(uname -m)" != "x86_64" ]; then
    echo "错误: 此脚本应在Intel Mac上运行"
    exit 1
fi

rm -rf build
mkdir build
cd build

# 构建Intel版本
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES=x86_64
make -j4

# 创建临时目录用于DMG
rm -rf ../temp_dmg
mkdir ../temp_dmg
cp -R openCap.app ../temp_dmg/

# 对应用程序进行代码签名
codesign --force --deep --sign - ../temp_dmg/openCap.app

# 创建Intel版本的DMG
cd ..
rm -rf openCap-Intel.dmg
create-dmg \
  --volname "openCap Intel" \
  --window-pos 200 120 \
  --window-size 600 300 \
  --icon-size 100 \
  --icon "openCap.app" 175 120 \
  --hide-extension "openCap.app" \
  --app-drop-link 425 120 \
  --no-internet-enable \
  "openCap-Intel.dmg" \
  "temp_dmg"

# 清理临时目录
rm -rf temp_dmg

echo "Intel版本构建完成: openCap-Intel.dmg" 