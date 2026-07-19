# examples/TodoApp.py
# A fully functional Todo application using PYTUINT flat API
# Requires: pip install git+https://github.com/nahiyancode-maker/PYTUINT

from PYTUINT import (
    Window, Text, Button, Input, Checkbox, Row, Column,
    List, Label, Spacer, Divider, Progress, Card, Dropdown,
    run, get_text, set_text, on_click, on_input, clear,
    set_value, set_visible,
    add_child, remove_child, set_style, on_key, on_scroll
)

# --- State ---
todos = []          # list of {"text": str, "done": bool, "priority": str}
current_filter = "all"
current_input = ""
current_edit = None

# --- Main Window ---
win = Window(
    title="PYTUINT Todo App - Pro",
    width=900,
    height=700,
    bg_color="#1e1e2f",
    resizable=True,
    min_width=700,
    min_height=500
)

# --- Header ---
header = Card(
    width="100%", height=80, bg_color="#2a2a3d",
    radius=0, elevation=2, padding=20
)
title = Text("✅ PYTUINT Todo Pro", font_size=24, color="#ffffff", bold=True)
Spacer(width=20)
stats_text = Text("Total: 0 | Done: 0 | Left: 0", font_size=14, color="#aaaaaa")
add_child(header, title)
add_child(header, stats_text)

# --- Input Area ---
input_row = Row(width="100%", height=60, gap=10, padding=(20,10,20,10))

todo_input = Input(
    placeholder="What needs to be done?",
    width=400, height=42, font_size=14,
    bg_color="#2a2a3d", color="#ffffff", border_color="#444466",
    on_enter=lambda: add_todo()
)

priority_dropdown = Dropdown(
    options=["low","medium","high"], default="medium",
    width=120, height=42, font_size=13,
    bg_color="#2a2a3d", color="#ffffff"
)

add_btn = Button(
    "+ Add", width=100, height=42, font_size=14,
    bg_color="#4CAF50", color="#ffffff", radius=6,
    on_click=lambda: add_todo()
)

edit_save_btn = Button(
    "💾 Save", width=100, height=42, font_size=14,
    bg_color="#2196F3", color="#ffffff", radius=6,
    visible=False,
    on_click=lambda: save_edit()
)

cancel_edit_btn = Button(
    "✖ Cancel", width=100, height=42, font_size=14,
    bg_color="#f44336", color="#ffffff", radius=6,
    visible=False,
    on_click=lambda: cancel_edit()
)

add_child(input_row, todo_input)
add_child(input_row, priority_dropdown)
add_child(input_row, add_btn)
add_child(input_row, edit_save_btn)
add_child(input_row, cancel_edit_btn)

# --- Filter Bar ---
filter_row = Row(width="100%", height=50, gap=8, padding=(20,5,20,5))
all_btn = Button("All", width=80, height=34, font_size=12, radius=17,
                 bg_color="#5c6bc0", color="#ffffff",
                 on_click=lambda: set_filter("all"))
active_btn = Button("Active", width=80, height=34, font_size=12, radius=17,
                   bg_color="#3949ab", color="#ffffff",
                   on_click=lambda: set_filter("active"))
done_btn = Button("Done", width=80, height=34, font_size=12, radius=17,
                 bg_color="#3949ab", color="#ffffff",
                 on_click=lambda: set_filter("done"))
clear_done_btn = Button("Clear Done", width=110, height=34, font_size=12, radius=17,
                       bg_color="#d32f2f", color="#ffffff",
                       on_click=lambda: clear_done())
Spacer(width=10)
progress = Progress(width=200, height=8, color="#4CAF50", bg_color="#444466", value=0)

add_child(filter_row, all_btn)
add_child(filter_row, active_btn)
add_child(filter_row, done_btn)
add_child(filter_row, clear_done_btn)
add_child(filter_row, progress)
add_child(filter_row, Text("", font_size=12, color="#aaaaaa", name="progress_label"))

# --- Todo List Area ---
todo_list = Column(
    width="100%", height="100%", gap=6,
    padding=(20,10,20,10), scroll_y=True
)

# --- Footer ---
footer = Card(
    width="100%", height=40, bg_color="#2a2a3d",
    radius=0, elevation=2, padding=10
)
footer_text = Text(
    "PYTUINT Pro Demo • Flat API • No Submodules • 2025",
    font_size=12, color="#777779"
)
add_child(footer, footer_text)

# Assemble window
add_child(win, header)
add_child(win, input_row)
add_child(win, filter_row)
add_child(win, todo_list)
add_child(win, footer)

# --- Logic Functions ---
def refresh():
    clear(todo_list)
    filtered = [t for t in todos if
                current_filter == "all" or
                (current_filter == "active" and not t["done"]) or
                (current_filter == "done" and t["done"])]
    for i, t in enumerate(filtered):
        card = Card(
            width="100%", height=56,
            bg_color="#2a2a3d" if i % 2 == 0 else "#252538",
            radius=8, elevation=1, padding=(12,6,12,6)
        )
        row = Row(gap=10)
        cb = Checkbox(checked=t["done"], on_change=lambda val, idx=i: toggleTodo(idx))
        priority_colors = {"low":"#4CAF50","medium":"#FF9800","high":"#f44336"}
        p_label = Text("● "+t["priority"], font_size=11, color=priority_colors[t["priority"]])
        txt = Text(t["text"], font_size=14,
                   color="#888888" if t["done"] else "#ffffff",
                   strikethrough=t["done"])
        Spacer(width=10)
        edit_btn = Button("✎", width=36, height=36, font_size=14, radius=18,
                          bg_color="#3949ab", color="#ffffff",
                          on_click=lambda _,idx=i: start_edit(idx))
        del_btn = Button("✖", width=36, height=36, font_size=14, radius=18,
                         bg_color="#d32f2f", color="#ffffff",
                         on_click=lambda _,idx=i: deleteTodo(idx))
        add_child(row, cb)
        add_child(row, p_label)
        add_child(row, txt)
        add_child(row, edit_btn)
        add_child(row, del_btn)
        add_child(card, row)
        add_child(todo_list, card)
    update_stats()
    update_progress()

def add_todo():
    global current_input
    current_input = get_text(todo_input).strip()
    if not current_input:
        return
    todos.append({"text": current_input, "done": False,
                  "priority": priority_dropdown.value})
    set_text(todo_input, "")
    refresh()

def toggleTodo(idx):
    filtered = [t for t in todos if
                current_filter == "all" or
                (current_filter == "active" and not t["done"]) or
                (current_filter == "done" and t["done"])]
    real_idx = todos.index(filtered[idx])
    todos[real_idx]["done"] = not todos[real_idx]["done"]
    refresh()

def deleteTodo(idx):
    filtered = [t for t in todos if
                current_filter == "all" or
                (current_filter == "active" and not t["done"]) or
                (current_filter == "done" and t["done"])]
    real_idx = todos.index(filtered[idx])
    todos.pop(real_idx)
    refresh()

def start_edit(idx):
    global current_edit
    filtered = [t for t in todos if
                current_filter == "all" or
                (current_filter == "active" and not t["done"]) or
                (current_filter == "done" and t["done"])]
    current_edit = todos.index(filtered[idx])
    set_text(todo_input, todos[current_edit]["text"])
    set_visible(edit_save_btn, True)
    set_visible(cancel_edit_btn, True)
    set_visible(add_btn, False)

def save_edit():
    global current_edit
    if current_edit is None:
        return
    new_text = get_text(todo_input).strip()
    if new_text:
        todos[current_edit]["text"] = new_text
    cancel_edit()
    refresh()

def cancel_edit():
    global current_edit
    current_edit = None
    set_text(todo_input, "")
    set_visible(edit_save_btn, False)
    set_visible(cancel_edit_btn, False)
    set_visible(add_btn, True)

def clear_done():
    global todos
    todos = [t for t in todos if not t["done"]]
    refresh()

def set_filter(f):
    global current_filter
    current_filter = f
    refresh()

def update_stats():
    total = len(todos)
    done = sum(1 for t in todos if t["done"])
    left = total - done
    set_text(stats_text, f"Total: {total} | Done: {done} | Left: {left}")

def update_progress():
    total = len(todos)
    done = sum(1 for t in todos if t["done"])
    pct = (done * 100 // total) if total > 0 else 0
    set_value(progress, pct)
    for child in filter_row.children:
        if getattr(child, "name", "") == "progress_label":
            set_text(child, f"{pct}%")
            break

# --- Keyboard shortcuts ---
on_key("Escape", lambda: cancel_edit())

# --- Run ---
run(win)