from PYTUINT import View, Text, Button, TextInput, useState, run, useRouter
from datetime import datetime

# ═══════════════════════════════════════════════════════════════════════════════
# TaskFlow - Advanced Terminal Task Manager
# Demonstrates PYTUINT's complex UI capabilities
# ═══════════════════════════════════════════════════════════════════════════════


# ───────────────────────────────────────────────────────────────────────────────
# Mock Data & State Management
# ───────────────────────────────────────────────────────────────────────────────

INITIAL_TASKS = [
    {"id": 1, "title": "Design API endpoints", "status": "completed", "priority": "high", "due": "2024-01-15"},
    {"id": 2, "title": "Implement authentication", "status": "in-progress", "priority": "high", "due": "2024-01-20"},
    {"id": 3, "title": "Write unit tests", "status": "pending", "priority": "medium", "due": "2024-01-25"},
    {"id": 4, "title": "Dockerize application", "status": "pending", "priority": "low", "due": "2024-02-01"},
    {"id": 5, "title": "Performance optimization", "status": "pending", "priority": "medium", "due": "2024-02-10"},
]

# ───────────────────────────────────────────────────────────────────────────────
# Component: Statistics Card
# ───────────────────────────────────────────────────────────────────────────────

def StatCard(title, value, color="white"):
    return View(
        style={
            "flex_direction": "column",
            "align_items": "center",
            "padding": 1,
            "border": "single",
            "border_color": color,
            "width": 20,
        },
        children=[
            Text(content=title, style={"font_weight": "bold", "color": "gray"}),
            Text(content=str(value), style={"font_size": "large", "color": color, "font_weight": "bold"}),
        ],
    )

# ───────────────────────────────────────────────────────────────────────────────
# Component: Task Item
# ───────────────────────────────────────────────────────────────────────────────

def TaskItem(task, on_delete, on_toggle):
    priority_colors = {
        "high": "red",
        "medium": "yellow",
        "low": "green",
    }
    status_icons = {
        "completed": "✓",
        "in-progress": "◐",
        "pending": "○",
    }
    
    return View(
        style={
            "flex_direction": "row",
            "justify_content": "space-between",
            "padding": 1,
            "border": "rounded",
            "border_color": priority_colors[task["priority"]],
            "margin_bottom": 1,
        },
        children=[
            View(
                style={"flex_direction": "column", "flex": 1},
                children=[
                    Text(
                        content=f"{status_icons[task['status']]} {task['title']}",
                        style={
                            "font_weight": "bold" if task["status"] == "completed" else "normal",
                            "text_decoration": "strikethrough" if task["status"] == "completed" else "none",
                        },
                    ),
                    Text(
                        content=f"Due: {task['due']} | Priority: {task['priority'].upper()}",
                        style={"color": "gray", "font_size": "small"},
                    ),
                ],
            ),
            View(
                style={"flex_direction": "row", "gap": 1},
                children=[
                    Button(
                        label="✓",
                        on_click=lambda: on_toggle(task["id"]),
                        style={"background": "green", "color": "white", "width": 3},
                    ),
                    Button(
                        label="✗",
                        on_click=lambda: on_delete(task["id"]),
                        style={"background": "red", "color": "white", "width": 3},
                    ),
                ],
            ),
        ],
    )

# ───────────────────────────────────────────────────────────────────────────────
# View: Dashboard
# ───────────────────────────────────────────────────────────────────────────────

def DashboardView(tasks):
    completed = len([t for t in tasks if t["status"] == "completed"])
    in_progress = len([t for t in tasks if t["status"] == "in-progress"])
    pending = len([t for t in tasks if t["status"] == "pending"])
    
    return View(
        style={"flex_direction": "column", "padding": 2},
        children=[
            Text(content="📊 TaskFlow Dashboard", style={"font_size": "xlarge", "font_weight": "bold", "margin_bottom": 2}),
            
            # Statistics Row
            View(
                style={"flex_direction": "row", "justify_content": "space-around", "margin_bottom": 3},
                children=[
                    StatCard("TOTAL TASKS", len(tasks), "blue"),
                    StatCard("COMPLETED", completed, "green"),
                    StatCard("IN PROGRESS", in_progress, "yellow"),
                    StatCard("PENDING", pending, "red"),
                ],
            ),
            
            # Recent Activity
            Text(content="📋 Recent Tasks", style={"font_size": "large", "font_weight": "bold", "margin_bottom": 1}),
            *[
                TaskItem(task, lambda _: None, lambda _: None)
                for task in sorted(tasks, key=lambda x: x["due"])[:3]
            ],
        ],
    )

# ───────────────────────────────────────────────────────────────────────────────
# View: Task List
# ───────────────────────────────────────────────────────────────────────────────

def TaskListView(tasks, current_filter, set_filter, on_delete, on_toggle):
    filtered_tasks = [
        t for t in tasks if current_filter == "all" or t["status"] == current_filter
    ] if current_filter != "all" else tasks
    
    return View(
        style={"flex_direction": "column", "padding": 2},
        children=[
            Text(content="📝 All Tasks", style={"font_size": "xlarge", "font_weight": "bold", "margin_bottom": 2}),
            
            # Filter Tabs
            View(
                style={"flex_direction": "row", "margin_bottom": 2, "gap": 1},
                children=[
                    Button(
                        label=f"{'✓ ' if current_filter == 'all' else ''}All",
                        on_click=lambda: set_filter("all"),
                        style={"background": "blue" if current_filter == "all" else "gray", "color": "white"},
                    ),
                    Button(
                        label=f"{'◐ ' if current_filter == 'in-progress' else ''}In Progress",
                        on_click=lambda: set_filter("in-progress"),
                        style={"background": "yellow" if current_filter == "in-progress" else "gray", "color": "black"},
                    ),
                    Button(
                        label=f"{'○ ' if current_filter == 'pending' else ''}Pending",
                        on_click=lambda: set_filter("pending"),
                        style={"background": "red" if current_filter == "pending" else "gray", "color": "white"},
                    ),
                    Button(
                        label=f"{'✓ ' if current_filter == 'completed' else ''}Completed",
                        on_click=lambda: set_filter("completed"),
                        style={"background": "green" if current_filter == "completed" else "gray", "color": "white"},
                    ),
                ],
            ),
            
            # Task List
            *(
                [
                    TaskItem(task, on_delete, on_toggle)
                    for task in sorted(filtered_tasks, key=lambda x: x["due"])
                ] if filtered_tasks else [
                    Text(
                        content="🎉 No tasks found! Add one to get started.",
                        style={"color": "gray", "text_align": "center", "margin_top": 3},
                    )
                ]
            ),
        ],
    )

# ───────────────────────────────────────────────────────────────────────────────
# View: Add Task Form
# ───────────────────────────────────────────────────────────────────────────────

def AddTaskView(on_submit):
    title, set_title = useState("")
    priority, set_priority = useState("medium")
    due_date, set_due_date = useState(datetime.now().strftime("%Y-%m-%d"))
    
    def handle_submit():
        if title.strip():
            on_submit({
                "title": title,
                "priority": priority,
                "due": due_date,
                "status": "pending",
            })
            set_title("")
            set_priority("medium")
            set_due_date(datetime.now().strftime("%Y-%m-%d"))
    
    return View(
        style={"flex_direction": "column", "padding": 2, "align_items": "center"},
        children=[
            Text(content="➕ Add New Task", style={"font_size": "xlarge", "font_weight": "bold", "margin_bottom": 3}),
            
            # Title Input
            View(
                style={"width": 50, "margin_bottom": 2},
                children=[
                    Text(content="Task Title:", style={"font_weight": "bold", "margin_bottom": 1}),
                    TextInput(
                        value=title,
                        on_change=set_title,
                        placeholder="Enter task description...",
                        style={"border": "single", "padding": 1},
                    ),
                ],
            ),
            
            # Priority Selection
            View(
                style={"width": 50, "margin_bottom": 2},
                children=[
                    Text(content="Priority:", style={"font_weight": "bold", "margin_bottom": 1}),
                    View(
                        style={"flex_direction": "row", "gap": 1},
                        children=[
                            Button(
                                label="🔴 High",
                                on_click=lambda: set_priority("high"),
                                style={"background": "red" if priority == "high" else "gray", "color": "white", "flex": 1},
                            ),
                            Button(
                                label="🟡 Medium",
                                on_click=lambda: set_priority("medium"),
                                style={"background": "yellow" if priority == "medium" else "gray", "color": "black", "flex": 1},
                            ),
                            Button(
                                label="🟢 Low",
                                on_click=lambda: set_priority("low"),
                                style={"background": "green" if priority == "low" else "gray", "color": "white", "flex": 1},
                            ),
                        ],
                    ),
                ],
            ),
            
            # Due Date Input
            View(
                style={"width": 50, "margin_bottom": 3},
                children=[
                    Text(content="Due Date (YYYY-MM-DD):", style={"font_weight": "bold", "margin_bottom": 1}),
                    TextInput(
                        value=due_date,
                        on_change=set_due_date,
                        placeholder="2024-12-31",
                        style={"border": "single", "padding": 1},
                    ),
                ],
            ),
            
            # Submit Button
            Button(
                label="CREATE TASK",
                on_click=handle_submit,
                style={
                    "background": "blue",
                    "color": "white",
                    "font_weight": "bold",
                    "width": 20,
                    "padding": 1,
                },
            ),
        ],
    )

# ───────────────────────────────────────────────────────────────────────────────
# Main Application Component
# ───────────────────────────────────────────────────────────────────────────────

def App():
    # State management
    tasks, set_tasks = useState(INITIAL_TASKS[:])
    current_view, set_current_view = useState("dashboard")
    filter_status, set_filter_status = useState("all")
    next_id, set_next_id = useState(len(INITIAL_TASKS) + 1)
    
    # Event handlers
    def add_task(new_task):
        set_tasks(tasks + [{**new_task, "id": next_id}])
        set_next_id(next_id + 1)
    
    def delete_task(task_id):
        set_tasks([t for t in tasks if t["id"] != task_id])
    
    def toggle_task_status(task_id):
        set_tasks([
            {**t, "status": "completed" if t["status"] != "completed" else "pending"}
            if t["id"] == task_id else t
            for t in tasks
        ])
    
    # Navigation component
    def NavigationBar():
        return View(
            style={
                "flex_direction": "row",
                "background": "black",
                "padding": 1,
                "justify_content": "space-between",
                "align_items": "center",
            },
            children=[
                Text(content="🚀 TaskFlow", style={"color": "white", "font_weight": "bold", "font_size": "large"}),
                View(
                    style={"flex_direction": "row", "gap": 2},
                    children=[
                        Button(
                            label="📊 Dashboard",
                            on_click=lambda: set_current_view("dashboard"),
                            style={"background": "transparent", "color": "white","border": "none" if current_view == "dashboard" else "hidden"},
                        ),
                        Button(
                            label="📝 Tasks",
                            on_click=lambda: set_current_view("tasks"),
                            style={"background": "transparent", "color": "white", "border": "none" if current_view == "tasks" else "hidden"},
                        ),
                        Button(
                            label="➕ Add Task",
                            on_click=lambda: set_current_view("add"),
                            style={"background": "transparent", "color": "white", "border": "none" if current_view == "add" else "hidden"},
                        ),
                    ],
                ),
            ],
        )
    
    # Render current view
    views = {
        "dashboard": DashboardView(tasks),
        "tasks": TaskListView(tasks, filter_status, set_filter_status, delete_task, toggle_task_status),
        "add": AddTaskView(add_task),
    }
    
    return View(
        style={"flex": 1},
        children=[
            NavigationBar(),
            views[current_view],
        ],
    )

# ═══════════════════════════════════════════════════════════════════════════════
# Run Application
# ═══════════════════════════════════════════════════════════════════════════════

if __name__ == "__main__":
    run(App)