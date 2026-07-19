"""Python fallback implementation for PTUI's core API.

This module provides a lightweight, dependency-free implementation so the
package can be imported and exercised even when the optional C++ extension
is unavailable.
"""

from __future__ import annotations

from typing import Any, Callable, Dict, List, Optional, Tuple


class View:
    """Minimal view container used for import compatibility."""

    def __init__(self, style: Optional[Dict[str, Any]] = None, children: Optional[List[Any]] = None):
        self.style = dict(style or {})
        self.children = list(children or [])

    def render(self) -> Dict[str, Any]:
        return {
            "type": "View",
            "style": self.style,
            "children": [child.render() if hasattr(child, "render") else child for child in self.children],
        }


class Text:
    """Minimal text component used for import compatibility."""

    def __init__(self, content: str = "", style: Optional[Dict[str, Any]] = None):
        self.content = content
        self.style = dict(style or {})

    def render(self) -> str:
        return self.content


class Button:
    """Minimal button component used for import compatibility."""

    def __init__(self, label: str = "", on_click: Optional[Callable[[], None]] = None, style: Optional[Dict[str, Any]] = None):
        self.label = label
        self.on_click = on_click
        self.style = dict(style or {})

    def render(self) -> str:
        return self.label


class Input:
    def __init__(self, value: str = "", placeholder: str = "", style: Optional[Dict[str, Any]] = None):
        self.value = value
        self.placeholder = placeholder
        self.style = dict(style or {})

    def render(self) -> Dict[str, Any]:
        return {"type": "Input", "value": self.value, "placeholder": self.placeholder}


class Slider:
    def __init__(self, value: float = 0.0, style: Optional[Dict[str, Any]] = None):
        self.value = value
        self.style = dict(style or {})

    def render(self) -> Dict[str, Any]:
        return {"type": "Slider", "value": self.value}


class Checkbox:
    def __init__(self, checked: bool = False, style: Optional[Dict[str, Any]] = None):
        self.checked = checked
        self.style = dict(style or {})

    def render(self) -> Dict[str, Any]:
        return {"type": "Checkbox", "checked": self.checked}


class Select:
    def __init__(self, options: Optional[List[str]] = None, style: Optional[Dict[str, Any]] = None):
        self.options = list(options or [])
        self.style = dict(style or {})

    def render(self) -> Dict[str, Any]:
        return {"type": "Select", "options": self.options}


class Table:
    def __init__(self, rows: Optional[List[List[str]]] = None, style: Optional[Dict[str, Any]] = None):
        self.rows = list(rows or [])
        self.style = dict(style or {})

    def render(self) -> Dict[str, Any]:
        return {"type": "Table", "rows": self.rows}


class Tabs:
    def __init__(self, tabs: Optional[List[str]] = None, style: Optional[Dict[str, Any]] = None):
        self.tabs = list(tabs or [])
        self.style = dict(style or {})

    def render(self) -> Dict[str, Any]:
        return {"type": "Tabs", "tabs": self.tabs}


class Modal:
    def __init__(self, title: str = "", content: str = "", style: Optional[Dict[str, Any]] = None):
        self.title = title
        self.content = content
        self.style = dict(style or {})

    def render(self) -> Dict[str, Any]:
        return {"type": "Modal", "title": self.title, "content": self.content}


class ProgressBar:
    def __init__(self, value: float = 0.0, style: Optional[Dict[str, Any]] = None):
        self.value = value
        self.style = dict(style or {})

    def render(self) -> Dict[str, Any]:
        return {"type": "ProgressBar", "value": self.value}


class Spinner:
    def __init__(self, label: str = "", style: Optional[Dict[str, Any]] = None):
        self.label = label
        self.style = dict(style or {})

    def render(self) -> Dict[str, Any]:
        return {"type": "Spinner", "label": self.label}


def useState(initial: Any) -> Tuple[Any, Callable[[Any], None]]:
    return [initial, lambda _value: None]


def useEffect(callback: Callable[[], None], deps: Optional[List[Any]] = None) -> None:
    if callable(callback):
        callback()


def useMemo(factory: Callable[[], Any], deps: Optional[List[Any]] = None) -> Any:
    if callable(factory):
        return factory()
    return None


def useRef(initial: Any = None) -> Dict[str, Any]:
    return {"current": initial}


def useReducer(reducer: Callable[[Any, Any], Any], initial: Any) -> Tuple[Any, Callable[[Any], None]]:
    return [initial, lambda _action: None]


def run(app: Callable[[], Any]) -> Any:
    result = app()
    return result
