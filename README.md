# Sequence Alignment

这是一个用于DNA序列比对的项目。

## 项目结构

```
Sequent_Alignment/
├── lab1/                   # Lab1 - DNA重复序列分析
│   ├── bin/                # 编译后的可执行文件目录
│   ├── sequence/           # 存放DNA序列输入文件
│   │   ├── reference.in    # 参考DNA序列文件
│   │   └── query.in        # 查询DNA序列文件
│   ├── src/                # 源代码目录
│   │   ├── lab1.cpp        # 主程序入口
│   │   ├── repeat.cpp      # 重复序列分析实现
│   │   └── repeat.h        # 重复序列分析头文件
│   └── CMakeLists.txt      # CMake构建配置文件
├── lab2/                   # Lab2 - 待添加
├── lab3/                   # Lab3 - 待添加
└── ...                     # 更多实验
```

## 前提条件

在编译运行项目前，请确保已安装以下软件：

### Windows
- [MinGW-w64](https://www.mingw-w64.org/downloads/) - C++编译器
- [CMake](https://cmake.org/download/) (3.10或更高版本)

### macOS
- Xcode命令行工具 (通过运行 `xcode-select --install` 安装)
- [CMake](https://cmake.org/download/) (3.10或更高版本)

### Linux
- GCC/G++ (通过包管理器安装，如 `sudo apt install build-essential`)
- [CMake](https://cmake.org/download/) (3.10或更高版本)

## 编译与运行

本项目通过CMake进行构建。以下是在不同操作系统上编译和运行各个实验的通用方法。

### macOS/Linux

对于任意一个lab（例如labx）：

```bash
# 从项目根目录
cd labx
mkdir -p build
cd build
cmake ..
make
cd ../bin
./labx
```

### Windows

对于任意一个lab（例如labx）：

```bash
# 从项目根目录
cd labx
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
mingw32-make
cd ..\bin
labx.exe
```