<h1 align="center">
  <img src="https://raw.githubusercontent.com/nahiyancode-maker/ptui/main/assets/logo.png" alt="PTUI Logo" width="400"/>
  <br>
  <b>PTUI</b> — Python Terminal User Interface
</h1>

<p align="center">
  <b>A React-inspired, GPU-grade TUI framework built with <code>C++20</code> and <code>pybind11</code>.</b>
</p>

<p align="center">
  <a href="https://pypi.org/project/ptui-tui/"><img src="https://img.shields.io/pypi/v/ptui-tui?style=for-the-badge&logo=pypi&logoColor=white" alt="PyPI"/></a>
  <a href="https://github.com/nahiyancode-maker/ptui/actions"><img src="https://img.shields.io/github/actions/workflow/status/nahiyancode-maker/ptui/build.yml?style=for-the-badge&logo=github" alt="Build Status"/></a>
  <a href="https://github.com/nahiyancode-maker/ptui/blob/main/LICENSE"><img src="https://img.shields.io/github/license/nahiyancode-maker/ptui?style=for-the-badge" alt="License"/></a>
  <a href="https://github.com/nahiyancode-maker/ptui/stargazers"><img src="https://img.shields.io/github/stars/nahiyancode-maker/ptui?style=for-the-badge&logo=github" alt="Stars"/></a>
</p>

<p align="center">
  <a href="#features">Features</a> •
  <a href="#installation">Installation</a> •
  <a href="#quick-start">Quick Start</a> •
  <a href="#examples">Examples</a> •
  <a href="#api-reference">API Reference</a> •
  <a href="#contributing">Contributing</a>
</p>

<br>

> **PTUI** brings component-based, declarative UIs to the terminal. If you love React, you'll love building terminal apps. Every pixel is orchestrated with C++ speed, exposed through a Pythonic API that feels native.

---

## ✨ Features

| 🎨 Component-Driven | ⚡ Blazing Fast | 🔋 Batteries Included |
|---------------------|----------------|------------------------|
| **Rich element set** — `View`, `Text`, `Button`, `Input`, `TextArea`, `Slider`, `Checkbox`, `Select`, `Table`, `Tabs`, `Modal`, `ProgressBar`, `Spinner`, `Image` (ASCII/Unicode). | **C++20 core** — Rendering, diffing, event loop, and ANSI escape handling written in modern C++. No Python overhead on hot paths. | **Zero external dependencies** — Only standard C++17/20 and `<pybind11>`. No `ncurses`, no `prompt_toolkit`. |
| **React-like hooks** — `useState`, `useEffect`, `useMemo`, `useRef`, `useReducer`. | **Sub-millisecond redraws** — Optimised dirty-rect diffing only updates changed cells. | **Built-in key/mouse bindings** — Composable event system with focus, blur, click, drag, scroll. |
| **CSS-inspired styling** — Flexbox-style layout engine, theme system, responsive terminal sizing. | **Cross-platform** — macOS, Linux, Windows 10+ (with VT100 support). | **Async ready** — First-class `asyncio` integration. |
| **Composable** — Build complex UIs from tiny, testable components. | **Memory-safe** — Smart pointers, RAII, no leaks. | **Themeable** — Dark, light, solarised, or custom. |

---

## 🚀 Installation

### From PyPI (recommended)

```bash
pip install ptui-tui