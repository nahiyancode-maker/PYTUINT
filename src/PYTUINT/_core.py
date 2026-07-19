import sys
import os
import builtins
import importlib.util

# ═══════════════════════════════════════════════════════════════════════════════
# PYTUINT _core.py - Python wrapper for the C++ pybind11 module
# ═══════════════════════════════════════════════════════════════════════════════

# Load the compiled C++ extension
_spec = importlib.util.find_spec("PYTUINT._core")
if _spec is None:
    raise ImportError("PYTUINT._core not found. Build the C++ extension first.")

_mod = importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(_mod)

# Re-export everything from the C++ module
globals().update({k: v for k, v in _mod.__dict__.items() if not k.startswith("_")})

# ═══════════════════════════════════════════════════════════════════════════════
# Terminal utilities
# ═══════════════════════════════════════════════════════════════════════════════

def terminal_size():
    """Get current terminal dimensions."""
    try:
        import shutil
        c, r = shutil.get_terminal_size()
        return {"rows": r, "columns": c}
    except Exception:
        return {"rows": 24, "columns": 80}


def clear_screen():
    """Clear terminal screen."""
    sys.stdout.write("\033[2J\033[H")
    sys.stdout.flush()


def move_cursor(row, col):
    """Move cursor to position (1-indexed)."""
    sys.stdout.write(f"\033[{row};{col}H")
    sys.stdout.flush()


def hide_cursor():
    sys.stdout.write("\033[?25l")
    sys.stdout.flush()


def show_cursor():
    sys.stdout.write("\033[?25h")
    sys.stdout.flush()


def enable_alt_buffer():
    sys.stdout.write("\033[?1049h")
    sys.stdout.flush()


def disable_alt_buffer():
    sys.stdout.write("\0033[?1049l")
    sys.stdout.flush()


def enable_raw_mode():
    """Enable raw terminal input (Unix only)."""
    if sys.platform != "win32":
        import tty
        import termios
        fd = sys.stdin.fileno()
        termios.tcsetattr(fd, termios.TCSADRAIN, termios.tcgetattr(fd))
        tty.setraw(fd)


def disable_raw_mode():
    """Disable raw terminal input (Unix only)."""
    if sys.platform != "win32":
        import tty
        import termios
        fd = sys.stdin.fileno()
        termios.tcsetattr(fd, termios.TCSADRAIN, termios.tcgetattr(fd))
        tty.setcbreak(fd)


def read_key():
    """Read a single keypress from stdin."""
    if sys.platform == "win32":
        import msvcrt
        ch = msvcrt.getch().decode("utf-8", errors="ignore")
        if ch == "\xe0" or ch == "\x00":
            ch += msvcrt.getch().decode("utf-8", errors="ignore")
        return ch
    else:
        import tty
        import termios
        import select
        fd = sys.stdin.fileno()
        old = termios.tcgetattr(fd)
        try:
            tty.setraw(fd)
            ch = sys.stdin.read(1)
            if ch == "\x1b":
                r, _, _ = select.select([sys.stdin], [], [], 0.05)
                if r:
                    ch += sys.stdin.read(2)
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old)
        return ch


def key_name(k):
    """Map raw key input to human-readable names."""
    return {
        "\x1b[A": "UP", "\x1b[B": "DOWN", "\x1b[C": "RIGHT", "\x1b[D": "LEFT",
        "\x1b[H": "HOME", "\x1b[F": "END", "\x1b[5~": "PAGEUP", "\x1b[6~": "PAGEDOWN",
        "\x1b[2~": "INSERT", "\x1b[3~": "DELETE", "\r": "ENTER", "\n": "ENTER",
        "\t": "TAB", "\x7f": "BACKSPACE", "\x1b": "ESCAPE", " ": "SPACE",
        "\xe0H": "UP", "\xe0P": "DOWN", "\xe0M": "RIGHT", "\xe0K": "LEFT",
    }.get(k, k)


# ═══════════════════════════════════════════════════════════════════════════════
# Python-side helpers
# ═══════════════════════════════════════════════════════════════════════════════

RESET = "\033[0m"
BOLD = "\033[1m"
DIM = "\033[2m"
ITALIC = "\033[3m"
UNDERLINE = "\033[4m"
BLINK = "\033[5m"
REVERSE = "\033[7m"
STRIKETHROUGH = "\033[9m"


def rgb(r, g, b):
    return f"\033[38;2;{r};{g};{b}m"


def bg_rgb(r, g, b):
    return f"\033[48;2;{r};{g};{b}m"


def hex_color(h):
    h = h.lstrip("#")
    r, g, b = int(h[:2], 16), int(h[2:4], 16), int(h[4:], 16)
    return rgb(r, g, b)


def lerp_color(c1, c2, t):
    return tuple(int(a * (1 - t) + b * t) for a, b in zip(c1, c2))


def gradient(text, c1, c2):
    lines = text.split("\n")
    out = []
    n = len(lines)
    for i, line in enumerate(lines):
        t = i / max(n - 1, 1)
        r, g, b = lerp_color(c1, c2, t)
        out.append(rgb(r, g, b) + line + RESET)
    return "\n".join(out)


def strip_ansi(s):
    import re
    return re.sub(r"\033\[[0-9;]*m", "", s)


def text_width(s):
    return len(strip_ansi(s))


def visible_length(s):
    return text_width(s)


def pad_center(s, width):
    w = text_width(s)
    if w >= width:
        return s
    left = (width - w) // 2
    return " " * left + s + " " * (width - w - left)


def pad_left(s, width):
    w = text_width(s)
    return s + " " * max(0, width - w)


def pad_right(s, width):
    w = text_width(s)
    return " " * max(0, width - w) + s


def truncate(s, max_width):
    w = text_width(s)
    if w <= max_width:
        return s
    return s[: max_width - 1] + "…"


# ═══════════════════════════════════════════════════════════════════════════════
# Application runner
# ═══════════════════════════════════════════════════════════════════════════════

_app = None
_running = False


def start(app):
    global _app, _running
    _app = app
    _running = True
    enable_alt_buffer()
    hide_cursor()
    clear_screen()
    try:
        _app.on_init()
        while _running:
            clear_screen()
            sz = terminal_size()
            _app.on_draw(sz["columns"], sz["rows"])
            sys.stdout.flush()
            k = read_key()
            if k == "\x03" or key_name(k) == "ESCAPE":
                break
            _app.on_key(key_name(k), k)
    except KeyboardInterrupt:
        pass
    finally:
        show_cursor()
        disable_alt_buffer()
        _running = False


def stop():
    global _running
    _running = False