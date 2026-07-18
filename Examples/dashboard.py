#!/usr/bin/env python3
"""System Dashboard - Real-time system monitor with PTUI"""

import ptui
import ptui.widgets as widgets
import time
import random
import math

class Dashboard(ptui.Component):
    def __init__(self):
        super().__init__()
        self.cpu_history = [0] * 60
        self.mem_history = [0] * 60
        self.last_update = time.time()
        
    def render(self):
        term = self.state("terminal")
        if not term:
            return ptui.Element.create("container")
        
        # Update metrics every second
        now = time.time()
        if now - self.last_update > 1.0:
            self.cpu_history.append(random.uniform(10, 90))
            self.cpu_history.pop(0)
            self.mem_history.append(random.uniform(20, 80))
            self.mem_history.pop(0)
            self.last_update = now
        
        # Header
        header = widgets.Text("📊 System Dashboard", ptui.Style()
            .fg(ptui.colors.BrightCyan)
            .bold()
            .set_text_align(ptui.TextAlign.Center)
            .set_width(term.width()))
        
        # CPU Graph
        cpu_graph = self.draw_graph(self.cpu_history, ptui.colors.Green, "CPU Usage")
        
        # Memory Graph
        mem_graph = self.draw_graph(self.mem_history, ptui.colors.Blue, "Memory Usage")
        
        # Stats
        stats = widgets.Container(
            ptui.Style()
                .set_flex_direction(ptui.FlexDirection.Row)
                .set_justify_content(ptui.JustifyContent.SpaceAround)
                .set_padding(1),
            [
                self.stat_box("CPU", f"{self.cpu_history[-1]:.1f}%", ptui.colors.Green),
                self.stat_box("Memory", f"{self.mem_history[-1]:.1f}%", ptui.colors.Blue),
                self.stat_box("Uptime", "12:34:56", ptui.colors.Yellow),
                self.stat_box("Processes", "128", ptui.colors.Magenta),
            ]
        )
        
        # Layout
        layout = widgets.Container(
            ptui.Style()
                .set_flex_direction(ptui.FlexDirection.Column)
                .set_padding(1),
            [
                header,
                cpu_graph,
                mem_graph,
                stats,
                widgets.Text("Press 'q' to quit", ptui.Style().fg(ptui.colors.Gray))
            ]
        )
        
        return layout.render()
    
    def draw_graph(self, data, color, title):
        term = self.state("terminal")
        width = term.width() - 10
        height = 8
        
        # Create canvas
        canvas = [[' ' for _ in range(width)] for _ in range(height)]
        
        # Draw axes
        for y in range(height):
            canvas[y][0] = '│'
        for x in range(width):
            canvas[height-1][x] = '─'
        canvas[height-1][0] = '└'
        
        # Plot data
        for i, value in enumerate(data):
            if i >= width - 2:
                break
            y = height - 2 - int((value / 100.0) * (height - 2))
            y = max(0, min(height - 2, y))
            canvas[y][i + 1] = '█'
        
        # Convert to text
        lines = [title + " (" + str(int(data[-1])) + "%)"]
        for row in canvas:
            lines.append(''.join(row))
        
        return widgets.Container(
            ptui.Style()
                .set_padding(1)
                .set_border(ptui.BorderStyle(color, ptui.BorderStyle.Style.Solid, 1)),
            [widgets.Text(line, ptui.Style().fg(color)) for line in lines]
        )
    
    def stat_box(self, label, value, color):
        return widgets.Container(
            ptui.Style()
                .set_padding(1)
                .set_border(ptui.BorderStyle(color, ptui.BorderStyle.Style.Solid, 1))
                .set_width(20),
            [
                widgets.Text(label, ptui.Style().fg(ptui.colors.Gray)),
                widgets.Text(value, ptui.Style().fg(color).bold().set_font_size(2))
            ]
        )
    
    def handle_event(self, event):
        if event.key == ptui.Key.Q:
            return None
        return self.render()

def main():
    term = ptui.Terminal()
    term.init()
    term.set_title("PTUI System Dashboard")
    term.show_cursor(False)
    
    dashboard = Dashboard()
    dashboard.set_state("terminal", term)
    
    reconciler = ptui.Reconciler(term)
    reconciler.mount(dashboard)
    
    running = True
    while running:
        event = term.get_event(100)
        if event.type != ptui.EventType.NoEvent:
            result = dashboard.handle_event(event)
            if result is None:
                running = False
            else:
                reconciler.render(result)
    
    term.show_cursor(True)
    term.shutdown()

if __name__ == "__main__":
    main()