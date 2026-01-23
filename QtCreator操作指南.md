# Qt Creator 操作指南 - 添加新主题按钮

## 📋 当前状态

✅ 已完成（代码层面）：
- thememanager.h - 添加了赛博朋克和樱花主题枚举
- thememanager.cpp - 添加了新主题样式
- mainwindow.h - 添加了新主题按钮槽函数

⚠️  需要完成（UI 层面）：
- 在 Qt Creator 中添加两个新主题按钮到界面

---

## 🚀 操作步骤（在 Qt Creator 中）

### 步骤 1：打开项目

1. 启动 Qt Creator
2. 打开 WeBot 项目：`文件` → `打开文件或项目`
3. 选择 `WeBot.pro` 文件

### 步骤 2：添加新主题按钮

1. 在左侧项目树中找到 `Forms` 文件夹
2. 双击打开 `mainwindow.ui` 文件
3. 在设计器视图中找到 `主题设置` 分组
4. 找到现有的三个按钮：浅色主题、深色主题、科技主题

**添加"赛博朋克"按钮：**

1. 从左侧工具箱拖拽一个 `QPushButton` 到"科技主题"按钮右侧
2. 选中该按钮，在右侧属性编辑器中设置：
   - `objectName`: `cyberThemeButton`
   - `text`: `赛博朋克`
   - `minimumSize`: `100, 35`
   - `geometry`: 调整到合适位置

**添加"樱花主题"按钮：**

1. 从左侧工具箱拖拽一个 `QPushButton` 到"赛博朋克"按钮右侧
2. 选中该按钮，在右侧属性编辑器中设置：
   - `objectName`: `sakuraThemeButton`
   - `text`: `樱花主题`
   - `minimumSize`: `100, 35`
   - `geometry`: 调整到合适位置

3. 确保这五个按钮在同一行（水平布局）

### 步骤 3：连接信号和槽

1. 在设计器中右键点击"赛博朋克"按钮
2. 选择 `转到槽...`
3. 选择 `onCyberThemeButton_clicked()`
4. 同样操作"樱花主题"按钮，选择 `onSakuraThemeButton_clicked()`

### 步骤 4：实现槽函数

1. 在左侧项目树中展开 `Sources` 文件夹
2. 查找 `mainwindow.cpp` 文件：
   - 如果存在，打开它
   - 如果不存在，创建它（见下方代码）

**如果 mainwindow.cpp 不存在，需要创建：**

```cpp
#include "mainwindow.h"
#include "thememanager.h"
#include "logger.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 新主题按钮槽函数
void MainWindow::onCyberThemeButton_clicked()
{
    ThemeManager::Theme theme = ThemeManager::CyberTheme;
    ThemeManager::getInstance()->setTheme(theme);
    LOG_INFO("切换到赛博朋克主题");
}

void MainWindow::onSakuraThemeButton_clicked()
{
    ThemeManager::Theme theme = ThemeManager::SakuraTheme;
    ThemeManager::getInstance()->setTheme(theme);
    LOG_INFO("切换到樱花主题");
}

// 注意：需要保留原有的其他槽函数和方法
```

**如果 mainwindow.cpp 已存在，只需添加：**

```cpp
// 在文件末尾或合适位置添加这两个槽函数

void MainWindow::onCyberThemeButton_clicked()
{
    ThemeManager::Theme theme = ThemeManager::CyberTheme;
    ThemeManager::getInstance()->setTheme(theme);
    LOG_INFO("切换到赛博朋克主题");
}

void MainWindow::onSakuraThemeButton_clicked()
{
    ThemeManager::Theme theme = ThemeManager::SakuraTheme;
    ThemeManager::getInstance()->setTheme(theme);
    LOG_INFO("切换到樱花主题");
}
```

### 步骤 5：编译运行

1. 保存所有文件：`Ctrl + Shift + S`
2. 清理构建缓存：
   - 菜单 `构建` → `清理全部`
3. 重新编译：
   - 菜单 `构建` → `构建全部`（或按 `Ctrl + B`）
4. 运行程序：
   - 点击左下角绿色运行按钮
   - 或按 `Ctrl + R`

---

## 🔍 验证

编译成功后，你应该看到：

1. **主题设置**分组中有 5 个按钮：
   - 浅色主题
   - 深色主题
   - 科技主题
   - **赛博朋克** ⬅️ 新增
   - **樱花主题** ⬅️ 新增

2. 点击新按钮应该能切换主题

3. 查看日志输出（如果有）：
   - "切换到赛博朋克主题"
   - "切换到樱花主题"

---

## ❗ 常见问题

### 问题 1：找不到 mainwindow.cpp

**原因**：项目可能使用 Qt Designer 生成的代码
**解决**：
- 检查 `Sources` 文件夹
- 如果没有，可能代码在其他地方或使用了不同的架构
- 参考现有代码结构添加槽函数

### 问题 2：编译错误

**原因**：可能是语法错误或头文件未包含
**解决**：
- 检查是否包含了正确的头文件：`#include "thememanager.h"`
- 检查函数声明是否在 mainwindow.h 中
- 查看编译器输出的具体错误信息

### 问题 3：运行时崩溃

**原因**：ThemeManager 未正确初始化
**解决**：
- 确保 ThemeManager 在程序启动时初始化
- 检查 ThemeManager::getInstance() 调用是否有效

### 问题 4：按钮点击无反应

**原因**：信号和槽未正确连接
**解决**：
- 重新连接信号和槽
- 使用 Qt Creator 的信号槽编辑器
- 检查 objectName 是否正确

---

## 💡 提示

1. **使用信号槽编辑器**：
   - 按 `F4` 切换到信号槽编辑模式
   - 拖拽按钮的 `clicked()` 信号到窗口的对应槽函数

2. **实时预览**：
   - 在设计器中按 `Ctrl + P` 预览 UI

3. **自动保存**：
   - 在 Qt Creator 中启用自动保存功能

4. **布局管理器**：
   - 使用水平布局管理器（QHBoxLayout）确保按钮对齐

---

## ✅ 完成检查清单

- [ ] 在 Qt Creator 中打开 mainwindow.ui
- [ ] 添加"赛博朋克"按钮
- [ ] 添加"樱花主题"按钮
- [ ] 设置按钮的 objectName
- [ ] 设置按钮的文本
- [ ] 调整按钮位置
- [ ] 连接信号和槽（或使用 Qt Creator 自动连接）
- [ ] 实现槽函数（如果需要）
- [ ] 保存所有文件
- [ ] 清理构建缓存
- [ ] 重新编译项目
- [ ] 运行并测试新主题

---

**如果在操作过程中遇到任何问题，请告诉我具体的错误信息！** 🚀
