"""
Python-side utilities for PYTUINT
"""

def create_style(**kwargs):
    """Helper to create style dictionaries"""
    return {
        "width": kwargs.get("width", "auto"),
        "height": kwargs.get("height", "auto"),
        "bg_color": kwargs.get("bg_color", "#1e1e2e"),
        "fg_color": kwargs.get("fg_color", "#cdd6f4"),
        "border": kwargs.get("border", False),
        "border_color": kwargs.get("border_color", "#89b4fa"),
        "font_weight": kwargs.get("font_weight", "normal"),
    }