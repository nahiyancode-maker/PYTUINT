"""
PYTUINT Core - Pure Python Implementation
Fallback when C++ extensions aren't available
"""

import sys
import os
from typing import List, Dict, Any, Callable, Optional

class BaseComponent:
    """Base class for all UI components"""
    
    def __init__(self, **kwargs):
        self.id: str = kwargs.get('id', '')
        self.children: List[Any] = []
        self.parent: Optional[Any] = None
        self.visible: bool = True
        self.styles: Dict[str, Any] = {}
        self.event_handlers: Dict[str, List[Callable]] = {}
        
    def add_child(self, child):
        child.parent = self
        self.children.append(child)
        return child
    
    def remove_child(self, child):
        if child in self.children:
            self.children.remove(child)
            child.parent = None
        return self
    
    def set_style(self, **styles):
        self.styles.update(styles)
        return self
    
    def on(self, event: str, handler: Callable):
        if event not in self.event_handlers:
            self.event_handlers[event] = []
        self.event_handlers[event].append(handler)
        return self
    
    def trigger(self, event: str, **kwargs):
        for handler in self.event_handlers.get(event, []):
            handler(**kwargs)
    
    def render(self) -> str:
        raise NotImplementedError


class Window(BaseComponent):
    """Main application window"""
    
    def __init__(self, title: str = "PYTUINT App", width: int = 80, height: int = 24):
        super().__init__()
        self.title = title
        self.width = width
        self.height = height
        self.running = False
    
    def render(self) -> str:
        if not self.visible:
            return ""
        
        output = []
        if self.title:
            output.append(f"┌{'─' * (self.width-2)}┐")
            output.append(f"│{self.title:^{self.width-2}}│")
            output.append(f"├{'─' * (self.width-2)}┤")
        
        for child in self.children:
            if child.visible:
                output.append(child.render())
        
        if self.title:
            output.append(f"└{'─' * (self.width-2)}┘")
        
        return "\n".join(output)
    
    def run(self):
        self.running = True
        print(self.render())
        while self.running:
            try:
                cmd = input("> ")
                if cmd.lower() in ['quit', 'exit', 'q']:
                    self.running = False
                self.trigger('command', text=cmd)
            except (EOFError, KeyboardInterrupt):
                self.running = False


class Text(BaseComponent):
    """Simple text display"""
    
    def __init__(self, content: str = "", **kwargs):
        super().__init__(**kwargs)
        self.content = content
    
    def render(self) -> str:
        if not self.visible:
            return ""
        style = self.styles
        align = style.get('align', 'left')
        width = style.get('width', len(self.content))
        
        if align == 'center':
            return self.content.center(width)
        elif align == 'right':
            return self.content.rjust(width)
        return self.content.ljust(width)


class Button(BaseComponent):
    """Clickable button"""
    
    def __init__(self, label: str = "Button", **kwargs):
        super().__init__(**kwargs)
        self.label = label
    
    def render(self) -> str:
        if not self.visible:
            return ""
        return f"[ {self.label} ]"
    
    def click(self):
        self.trigger('click')


class Input(BaseComponent):
    """Text input field"""
    
    def __init__(self, placeholder: str = "", value: str = "", **kwargs):
        super().__init__(**kwargs)
        self.placeholder = placeholder
        self.value = value
    
    def render(self) -> str:
        if not self.visible:
            return ""
        if self.value:
            return f"[{self.value}]"
        return f"[{self.placeholder}]"


class Checkbox(BaseComponent):
    """Checkbox component"""
    
    def __init__(self, label: str = "", checked: bool = False, **kwargs):
        super().__init__(**kwargs)
        self.label = label
        self.checked = checked
    
    def render(self) -> str:
        if not self.visible:
            return ""
        box = "[x]" if self.checked else "[ ]"
        return f"{box} {self.label}"
    
    def toggle(self):
        self.checked = not self.checked
        self.trigger('change', checked=self.checked)


class Row(BaseComponent):
    """Horizontal layout container"""
    
    def render(self) -> str:
        if not self.visible:
            return ""
        rendered = [child.render() for child in self.children if child.visible]
        return " ".join(rendered)


class Column(BaseComponent):
    """Vertical layout container"""
    
    def render(self) -> str:
        if not self.visible:
            return ""
        rendered = [child.render() for child in self.children if child.visible]
        return "\n".join(rendered)


class List(BaseComponent):
    """List container with items"""
    
    def __init__(self, items: List[str] = None, **kwargs):
        super().__init__(**kwargs)
        self.items = items or []
    
    def render(self) -> str:
        if not self.visible:
            return ""
        lines = []
        for i, item in enumerate(self.items):
            lines.append(f"  {i+1}. {item}")
        return "\n".join(lines)


class Label(BaseComponent):
    """Label component"""
    
    def __init__(self, text: str = "", **kwargs):
        super().__init__(**kwargs)
        self.text = text
    
    def render(self) -> str:
        if not self.visible:
            return ""
        return self.text


class Spacer(BaseComponent):
    """Spacer for layout"""
    
    def __init__(self, height: int = 1, **kwargs):
        super().__init__(**kwargs)
        self.height = height
    
    def render(self) -> str:
        if not self.visible:
            return ""
        return "\n" * self.height


class Divider(BaseComponent):
    """Horizontal divider line"""
    
    def __init__(self, char: str = "─", length: int = 40, **kwargs):
        super().__init__(**kwargs)
        self.char = char
        self.length = length
    
    def render(self) -> str:
        if not self.visible:
            return ""
        return self.char * self.length


class Progress(BaseComponent):
    """Progress bar"""
    
    def __init__(self, value: float = 0, max: float = 100, **kwargs):
        super().__init__(**kwargs)
        self.value = value
        self.max = max
    
    def render(self) -> str:
        if not self.visible:
            return ""
        percent = min(100, max(0, (self.value / self.max) * 100))
        filled = int(percent / 5)
        bar = "█" * filled + "░" * (20 - filled)
        return f"[{bar}] {percent:.0f}%"


class Card(BaseComponent):
    """Card container with border"""
    
    def __init__(self, title: str = "", **kwargs):
        super().__init__(**kwargs)
        self.title = title
    
    def render(self) -> str:
        if not self.visible:
            return ""
        
        lines = []
        width = max(30, max(len(child.render()) for child in self.children) if self.children else 30)
        
        lines.append(f"┌{'─' * (width+2)}┐")
        if self.title:
            lines.append(f"│ {self.title:<{width}} │")
            lines.append(f"├{'─' * (width+2)}┤")
        
        for child in self.children:
            if child.visible:
                rendered = child.render()
                for line in rendered.split('\n'):
                    lines.append(f"│ {line:<{width}} │")
        
        lines.append(f"└{'─' * (width+2)}┘")
        return "\n".join(lines)


class Dropdown(BaseComponent):
    """Dropdown menu"""
    
    def __init__(self, options: List[str] = None, selected: int = 0, **kwargs):
        super().__init__(**kwargs)
        self.options = options or []
        self.selected = selected
    
    def render(self) -> str:
        if not self.visible:
            return ""
        current = self.options[self.selected] if self.options else "Select..."
        return f"▼ {current}"


# Utility functions
def get_text(component: Text) -> str:
    return component.content

def set_text(component: Text, text: str) -> None:
    component.content = text

def on_click(component: Button, handler: Callable) -> None:
    component.on('click', handler)

def on_input(component: Input, handler: Callable) -> None:
    component.on('input', handler)

def clear(component: BaseComponent) -> None:
    component.children.clear()

def add_child(parent: BaseComponent, child: BaseComponent) -> None:
    parent.add_child(child)

def remove_child(parent: BaseComponent, child: BaseComponent) -> None:
    parent.remove_child(child)

def set_style(component: BaseComponent, **styles) -> None:
    component.set_style(**styles)

def on_key(component: BaseComponent, handler: Callable) -> None:
    component.on('key', handler)

def on_scroll(component: BaseComponent, handler: Callable) -> None:
    component.on('scroll', handler)

def set_visible(component: BaseComponent, visible: bool) -> None:
    component.visible = visible

def run(window: Window) -> None:
    window.run()

# Make all components available at module level
__all__ = [
    "BaseComponent",
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
    "run",
]