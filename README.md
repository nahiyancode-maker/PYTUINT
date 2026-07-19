<div align="center">

# PYTUINT

### Python Terminal User Interface Library

**A modern, high-performance TUI framework combining a Pythonic API with a C++20 rendering engine.**

![PyPI](https://img.shields.io/badge/pypi-PYTUINT--tui-blue)
![License](https://img.shields.io/badge/license-MIT-green)
![Python](https://img.shields.io/badge/python-3.8%2B-blue)
![C++](https://img.shields.io/badge/C%2B%2B-20-00599C)
![Version](https://img.shields.io/badge/version-0.3.0-orange)

```bash
pip install git+https://github.com/nahiyancode-maker/PYTUINT
```

</div>

---

## Table of Contents

- [Overview](#overview)
- [Why PYTUINT](#why-PYTUINT)
- [Architecture](#architecture)
- [Component Library](#component-library)
- [Hooks](#hooks)
- [Styling](#styling)
- [Quick Start](#quick-start)
- [Installation](#installation)
- [Examples](#examples)
- [Project Structure](#project-structure)
- [Performance Strategies](#performance-strategies)
- [Comparison](#comparison)
- [Roadmap](#roadmap)
- [Contributing](#contributing)

---

## Overview

PYTUINT (Python Terminal User Interface) reimagines terminal UI development by pairing a React/SwiftUI-style declarative component model with a compiled C++20 backend. It targets CLI tool authors, DevOps engineers, dashboard builders, and educators who want rich, interactive terminal interfaces without sacrificing performance.

## Why PYTUINT

- **Hybrid architecture** — Python handles the developer-facing API (components, hooks, styling); a C++20 engine (bound via `pybind11`) handles screen buffering, layout, and ANSI rendering.
- **Performance isolation** — expensive operations (grid manipulation, line wrapping, escape-code batching, screen diffing) run in compiled code, not the Python interpreter.
- **Modern C++ safety** — concepts, ranges, `constexpr`, and RAII enforce correctness at compile time and eliminate the memory leaks common in long-running terminal sessions.
- **Familiar ergonomics** — context managers, decorators, and hooks make PYTUINT feel native to Python developers.

## Architecture

### Component Tree Model
Interfaces are described as a tree of components, each with its own state and lifecycle — declarative, not imperative. A reconciliation engine diffs renders and applies only the minimal set of terminal updates, reducing flicker.

### Rendering Engine (C++20)
- Double-buffered screen state (front/back buffers)
- Cell-level diffing to minimize PTY writes
- `constexpr`-evaluated color tables and escape sequences
- GIL-releasing operations so background work (e.g., async LLM streaming) doesn't block the UI

## Component Library

| Component | Purpose |
|---|---|
| `View` | Flexbox container (`flex`, `justify_content`, `align_items`, etc.) |
| `Text` | Styled text with optional word-wrap |
| `Button` | Clickable / keyboard-activatable control |
| `Input` | Single-line text field with cursor & clipboard handling |
| `Slider` | Bounded numeric range control |
| `Checkbox` | Boolean toggle |
| `Select` | Dropdown selection |
| `Table` | Row/column data grid with keyboard navigation |
| `Tabs` | Switchable labeled panels |
| `Modal` | Overlay dialog with backdrop |
| `ProgressBar` | Proportional progress indicator |
| `Spinner` | Animated loading indicator |

## Hooks

```python
count, set_count = PYTUINT.useState(0)
PYTUINT.useEffect(lambda: fetch_data(), deps=[query])
cached = PYTUINT.useMemo(lambda: expensive_calc(x), deps=[x])
ref = PYTUINT.useRef(None)
state, dispatch = PYTUINT.useReducer(reducer, initial_state)
```

| Hook | Behavior |
|---|---|
| `useState` | Persists state across renders; setter triggers re-render |
| `useEffect` | Runs side effects when dependencies change |
| `useMemo` | Memoizes expensive computations |
| `useRef` | Mutable value that persists without re-rendering |
| `useReducer` | Redux-style state machine for complex logic |

## Styling

Components accept a `style` dict:

```python
style = {
    "width": 40,
    "flex": 1,
    "bg_color": "#1e1e2e",
    "fg_color": "#cdd6f4",
    "border": True,
    "border_color": "#89b4fa",
    "font_weight": "bold",
    "wrap": True,
}
```

## Quick Start

```python
import PYTUINT

def App():
    count, set_count = PYTUINT.useState(0)

    return PYTUINT.View(
        style={"flex": 1, "justify_content": "center", "align_items": "center"},
        children=[
            PYTUINT.Text(content=f"Count: {count}", style={"font_weight": "bold"}),
            PYTUINT.Button(label="Increment", on_click=lambda: set_count(count + 1)),
        ],
    )

PYTUINT.run(App)
```

## Installation

**Requirements**
- Python 3.8+
- C++20 compiler: GCC 10+, Clang 13+, or MSVC 19.30+ (VS 2022 17.0+)
- `pybind11` (fetched automatically during build)

### From PyPI (recommended)

```bash
pip install git+https://github.com/nahiyancode-maker/PYTUINT
```

### From Source

```bash
# Development (editable) install
pip install -e ".[dev]"

# Recompile after native changes
python setup.py build_ext --inplace

# Production install
pip install .
```

## Examples

| File | Demonstrates |
|---|---|
| `examples/ai_chat.py` | Streaming markdown chat UI, scroll management |
| `examples/piano.py` | Real-time input handling, mouse mapping |
| `examples/dashboard.py` | Live-updating charts, grid layout, `ProgressBar` |
| `examples/calculator.py` | Button grids, `useReducer` state logic |

Run any example directly: `python examples/ai_chat.py`

## Project Structure

```
├── src_cpp/        # C++20 rendering engine
├── python/         # pybind11 bindings (bindings.cpp)
├── src/PYTUINT/       # Pure-Python API layer
│   ├── __init__.py # Public API surface
│   └── core.py     # Reconciliation engine, hook lifecycle
└── examples/       # Sample applications
```

## Performance Strategies

- **Differential rendering** — only changed cells are redrawn (95%+ fewer PTY writes on typical dashboards)
- **Batching** — escape sequences coalesced into single `write()` syscalls
- **Layout caching** — flex computations skipped when tree/geometry are unchanged
- **GIL release** — long C++ operations free the interpreter for concurrent Python work

## Comparison

| Library | Distinguishing Trait |
|---|---|
| `curses` | Low-level POSIX standard, manual window/attribute management |
| `prompt_toolkit` | Advanced text editing, imperative model |
| `Textual` | CSS-like styling, message-based events, production-proven |
| `PyTermGUI` | Minimal API surface |
| **PYTUINT** | C++20 performance + flexbox layout + React-style hooks |

## Roadmap

- **v0.2** — Inline animations, custom theme engine, plugin system
- **v0.3** — True-color and Sixel image support
- **v0.4** — WebAssembly build for browser-based terminals (e.g., xterm.js)

Longer-term: accessibility (braille/screen reader support), multi-cursor input, CRDT-based collaborative sessions.

## Contributing

- Report bugs via GitHub Issues
- Propose features via RFC-style discussions
- Submit PRs via fork → branch → PR
- See `CONTRIBUTING.md` for the code of conduct

---

<div align="center">

**PYTUINT** is MIT licensed · © 2026 PYTUINT Contributors
Built with C++20 and a belief that your terminal deserves better.

Author: **nahiyancode-maker** · Version `0.1.0` — a foundation, not a finished product.

</div>
