# 梯形图编辑器 (Ladder Diagram Editor)

基于 Qt 6.8.3 开发的梯形图组态软件。

## 功能特性

- 支持基本梯形图元件：常开触点、常闭触点、输出线圈、置位/复位线圈
- 支持定时器、计数器等高级指令
- 可视化拖拽编辑
- 元件属性编辑
- 连接线自动对齐
- 网格对齐功能
- JSON 格式文件保存/加载
- 撤销/重做功能
- 缩放和平移视图

## 系统要求

- Qt 6.8.3 或更高版本
- CMake 3.16 或更高版本
- C++17 编译器

## 编译步骤

### Windows (Visual Studio)

```bash
# 1. 创建构建目录
mkdir build
cd build

# 2. 配置项目（根据实际Qt安装路径调整）
cmake .. -DCMAKE_PREFIX_PATH="C:/Qt/6.8.3/msvc2019_64"

# 3. 编译项目
cmake --build . --config Release

# 4. 运行程序
.\bin\Release\LadderDiagramEditor.exe
```

### Linux

```bash
# 1. 创建构建目录
mkdir build
cd build

# 2. 配置项目
cmake ..

# 3. 编译项目
cmake --build .

# 4. 运行程序
./bin/LadderDiagramEditor
```

### macOS

```bash
# 1. 创建构建目录
mkdir build
cd build

# 2. 配置项目
cmake .. -DCMAKE_PREFIX_PATH=$(brew --prefix qt6)

# 3. 编译项目
cmake --build .

# 4. 运行程序
./bin/LadderDiagramEditor.app/Contents/MacOS/LadderDiagramEditor
```

## 使用说明

### 基本操作

1. **添加元件**：从左侧面板的元件库中双击元件添加到画布
2. **移动元件**：拖拽元件到新位置，会自动对齐到网格
3. **删除元件**：选中元件后按 Delete 键
4. **编辑属性**：选中元件后在右侧面板编辑属性

### 连线操作

1. 点击工具栏的"连线模式"按钮进入连线模式
2. 点击第一个元件的连接点作为起点
3. 点击第二个元件的连接点作为终点
4. 按 ESC 键取消当前连线

### 视图操作

- **缩放**：鼠标滚轮或工具栏的放大/缩小按钮
- **平移**：按住鼠标中键拖动
- **重置视图**：工具栏的重置缩放按钮

### 文件操作

- **新建**：Ctrl+N
- **打开**：Ctrl+O
- **保存**：Ctrl+S
- **另存为**：Ctrl+Shift+S

## 项目结构

```
├── src/
│   ├── core/           # 核心类
│   │   ├── LadderElement.h/cpp    # 元件基类
│   ├── elements/       # 元件实现
│   │   ├── ContactElements.h/cpp  # 触点、线圈等
│   │   ├── ConnectionLine.h/cpp   # 连接线
│   ├── ui/             # 用户界面
│   │   ├── LadderScene.h/cpp      # 场景和视图
│   │   ├── MainWindow.h/cpp       # 主窗口
│   │   ├── PropertyEditor.h/cpp   # 属性编辑器
│   └── main.cpp        # 程序入口
├── CMakeLists.txt      # CMake配置
└── README.md           # 本文件
```

## 许可证

MIT License

## 联系方式

如有问题或建议，欢迎提交 Issue 或 Pull Request。
