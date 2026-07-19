"""
PYTUINT - Python Terminal User Interface
Flat API - No submodules, everything exported at top level
"""

# Import everything from core modules
from .core import (
    Window,
    Text,
    Button,
    Input,
    Checkbox,
    Row,
    Column,
    List,
    Label,
    Spacer,
    Divider,
    Progress,
    Card,
    Dropdown,
    run,
    get_text,
    set_text,
    on_click,
    on_input,
    clear,
    add_child,
    remove_child,
    set_style,
    on_key,
    on_scroll,
    set_visible,
)

# Try to import C++ bindings if available
try:
    from ._core import *
except ImportError:
    pass

# Version info
__version__ = "0.2.0"

# Define what gets imported with "from PYTUINT import *"
__all__ = [
    "Window",
    "Text",
    "Button",
    "Input",
    "Checkbox",
    "Row",
    "Column",
    "List",
    "Label",
    "Spacer",
    "Divider",
    "Progress",
    "Card",
    "Dropdown",
    "run",
    "get_text",
    "set_text",
    "on_click",
    "on_input",
    "clear",
    "add_child",
    "remove_child",
    "set_style",
    "on_key",
    "on_scroll",
    "set_visible",
]