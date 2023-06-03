# `Mini-Lisp` 脚手架

## 准备步骤

- 使用 [VS Code Config Helper v4](https://v4.vscch.tk) 配置一遍。这只是确保你正确安装了编译器。
  - 如果你确定你安装了相关编译器，或者之前配置过，则可以跳过这一步。
  - 请在“选择编译器”中，切换编译器类型为你想要用的类型（MinGW 或 MSVC 或 Apple Clang）。
  - 配置完成后，可**直接删除工作文件夹**，这里不需要它。
- 安装 [Cmake](https://cmake.org/download/#latest)。
  - （Windows 11）你可以使用 `winget` 安装：执行 `winget install cmake`。
  - （macOS）如果你安装了 `brew`，则可以执行 `brew install cmake`。
- 在 CLion 中打开本文件夹，配置好编译器和 CMake。
  - 如果你正确安装了相关编译器和 CMake，CLion 通常能自动检测到它们。
  - Windows 下如果你同时安装了 MinGW 和 MSVC，你可能需要手动选择其中一个。

## 编译、运行与调试

- 按“锤子”（Ctrl/Cmd + F9）按钮编译项目。按“播放”（Ctrl + R）按钮运行项目。
- 按“虫子”（Ctrl + D）按钮调试项目。

> 你每次新建文件后，你可能需要重新配置 CMake。菜单栏 Tools -> CMake -> Reload CMake Project。
