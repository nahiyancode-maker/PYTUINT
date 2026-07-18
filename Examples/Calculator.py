print("Hello woels")




#!/usr/bin/env python3
"""Calculator - TUI calculator with history"""

import ptui
import ptui.widgets as widgets
import math

class Calculator(ptui.Component):
    def __init__(self):
        super().__init__()
        self.expression = ""
        self.result = "0"
        self.history = []
        self.memory = 0
        
    def render(self):
        term = self.state("terminal")
        if not term:
            return ptui.Element.create("container")
        
        # Display
        display = widgets.Container(
            ptui.Style()
                .bg(ptui.colors.Black)
                .fg(ptui.colors.Green)
                .set_padding(1)
                .set_width(term.width() - 4),
            [
                widgets.Text(self.expression or "0", ptui.Style().fg(ptui.colors.Gray)),
                widgets.Text(self.result, ptui.Style().fg(ptui.colors.Green).bold().set_font_size(2))
            ]
        )
        
        # Buttons
        buttons = [
            ['C', '⌫', '%', '÷'],
            ['7', '8', '9', '×'],
            ['4', '5', '6', '-'],
            ['1', '2', '3', '+'],
            ['0', '.', '±', '=']
        ]
        
        button_rows = []
        for row in buttons:
            row_elements = []
            for btn in row:
                style = ptui.Style().bg(ptui.colors.DarkGray).fg(ptui.colors.White).set_width(5).set_height(2)
                
                if btn in ['÷', '×', '-', '+', '=']:
                    style.bg(ptui.colors.Orange).fg(ptui.colors.Black)
                elif btn in ['C', '⌫', '%', '±']:
                    style.bg(ptui.colors.Gray).fg(ptui.colors.Black)
                
                row_elements.append(
                    widgets.Button(
                        btn,
                        lambda b=btn: self.on_button(b),
                        style
                    )
                )
            
            button_rows.append(
                widgets.Container(
                    ptui.Style().set_flex_direction(ptui.FlexDirection.Row),
                    [b.render() for b in row_elements]
                )
            )
        
        # History
        history_elements = []
        if self.history:
            history_elements.append(widgets.Text("History:", ptui.Style().fg(ptui.colors.Gray).bold()))
            for expr, res in self.history[-5:]:
                history_elements.append(widgets.Text(f"{expr} = {res}", ptui.Style().fg(ptui.colors.DarkGray)))
        
        # Layout
        layout = widgets.Container(
            ptui.Style()
                .set_flex_direction(ptui.FlexDirection.Column)
                .set_align_items(ptui.AlignItems.Center)
                .set_padding(2),
            [
                widgets.Text("🧮 PTUI Calculator", ptui.Style().fg(ptui.colors.BrightCyan).bold()),
                display,
                *button_rows,
                widgets.Container(
                    ptui.Style()
                        .set_flex_direction(ptui.FlexDirection.Column)
                        .set_padding(1)
                        .set_width(term.width() - 4),
                    history_elements
                ) if history_elements else widgets.Text("")
            ]
        )
        
        return layout.render()
    
    def on_button(self, btn):
        if btn == 'C':
            self.expression = ""
            self.result = "0"
        elif btn == '⌫':
            self.expression = self.expression[:-1]
        elif btn == '=':
            try:
                # Safe evaluation
                expr = self.expression.replace('×', '*').replace('÷', '/')
                self.result = str(eval(expr))
                self.history.append((self.expression, self.result))
                self.expression = self.result
            except:
                self.result = "Error"
        elif btn == '±':
            if self.expression and self.expression[0] == '-':
                self.expression = self.expression[1:]
            else:
                self.expression = '-' + self.expression
        elif btn == '%':
            try:
                self.result = str(float(self.expression) / 100)
                self.expression = self.result
            except:
                pass
        else:
            self.expression += btn
        
        self.force_update()
    
    def handle_event(self, event):
        key_map = {
            ptui.Key.ZERO: '0', ptui.Key.ONE: '1', ptui.Key.TWO: '2',
            ptui.Key.THREE: '3', ptui.Key.FOUR: '4', ptui.Key.FIVE: '5',
            ptui.Key.SIX: '6', ptui.Key.SEVEN: '7', ptui.Key.EIGHT: '8',
            ptui.Key.NINE: '9', ptui.Key.PERIOD: '.',
            ptui.Key.PLUS: '+', ptui.Key.MINUS: '-',
            ptui.Key.ASTERISK: '×', ptui.Key.SLASH: '÷',
            ptui.Key.ENTER: '=', ptui.Key.BACKSPACE: '⌫',
            ptui.Key.ESCAPE: 'C', ptui.Key.PERCENT: '%'
        }
        
        if event.key in key_map:
            self.on_button(key_map[event.key])
        elif event.key == ptui.Key.Q:
            return None
        
        return self.render()

def main():
    term = ptui.Terminal()
    term.init()
    term.set_title("PTUI Calculator")
    term.show_cursor(False)
    
    calc = Calculator()
    calc.set_state("terminal", term)
    
    reconciler = ptui.Reconciler(term)
    reconciler.mount(calc)
    
    running = True
    while running:
        event = term.get_event(100)
        if event.type != ptui.EventType.NoEvent:
            result = calc.handle_event(event)
            if result is None:
                running = False
            else:
                reconciler.render(result)
    
    term.show_cursor(True)
    term.shutdown()

if __name__ == "__main__":
    main()