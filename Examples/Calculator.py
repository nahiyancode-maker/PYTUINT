print("Hello woels")




#!/usr/bin/env python3
"""Calculator - TUI calculator with history"""

import PYTUINT
import PYTUINT.widgets as widgets
import math

class Calculator(PYTUINT.Component):
    def __init__(self):
        super().__init__()
        self.expression = ""
        self.result = "0"
        self.history = []
        self.memory = 0
        
    def render(self):
        term = self.state("terminal")
        if not term:
            return PYTUINT.Element.create("container")
        
        # Display
        display = widgets.Container(
            PYTUINT.Style()
                .bg(PYTUINT.colors.Black)
                .fg(PYTUINT.colors.Green)
                .set_padding(1)
                .set_width(term.width() - 4),
            [
                widgets.Text(self.expression or "0", PYTUINT.Style().fg(PYTUINT.colors.Gray)),
                widgets.Text(self.result, PYTUINT.Style().fg(PYTUINT.colors.Green).bold().set_font_size(2))
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
                style = PYTUINT.Style().bg(PYTUINT.colors.DarkGray).fg(PYTUINT.colors.White).set_width(5).set_height(2)
                
                if btn in ['÷', '×', '-', '+', '=']:
                    style.bg(PYTUINT.colors.Orange).fg(PYTUINT.colors.Black)
                elif btn in ['C', '⌫', '%', '±']:
                    style.bg(PYTUINT.colors.Gray).fg(PYTUINT.colors.Black)
                
                row_elements.append(
                    widgets.Button(
                        btn,
                        lambda b=btn: self.on_button(b),
                        style
                    )
                )
            
            button_rows.append(
                widgets.Container(
                    PYTUINT.Style().set_flex_direction(PYTUINT.FlexDirection.Row),
                    [b.render() for b in row_elements]
                )
            )
        
        # History
        history_elements = []
        if self.history:
            history_elements.append(widgets.Text("History:", PYTUINT.Style().fg(PYTUINT.colors.Gray).bold()))
            for expr, res in self.history[-5:]:
                history_elements.append(widgets.Text(f"{expr} = {res}", PYTUINT.Style().fg(PYTUINT.colors.DarkGray)))
        
        # Layout
        layout = widgets.Container(
            PYTUINT.Style()
                .set_flex_direction(PYTUINT.FlexDirection.Column)
                .set_align_items(PYTUINT.AlignItems.Center)
                .set_padding(2),
            [
                widgets.Text("🧮 PYTUINT Calculator", PYTUINT.Style().fg(PYTUINT.colors.BrightCyan).bold()),
                display,
                *button_rows,
                widgets.Container(
                    PYTUINT.Style()
                        .set_flex_direction(PYTUINT.FlexDirection.Column)
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
            PYTUINT.Key.ZERO: '0', PYTUINT.Key.ONE: '1', PYTUINT.Key.TWO: '2',
            PYTUINT.Key.THREE: '3', PYTUINT.Key.FOUR: '4', PYTUINT.Key.FIVE: '5',
            PYTUINT.Key.SIX: '6', PYTUINT.Key.SEVEN: '7', PYTUINT.Key.EIGHT: '8',
            PYTUINT.Key.NINE: '9', PYTUINT.Key.PERIOD: '.',
            PYTUINT.Key.PLUS: '+', PYTUINT.Key.MINUS: '-',
            PYTUINT.Key.ASTERISK: '×', PYTUINT.Key.SLASH: '÷',
            PYTUINT.Key.ENTER: '=', PYTUINT.Key.BACKSPACE: '⌫',
            PYTUINT.Key.ESCAPE: 'C', PYTUINT.Key.PERCENT: '%'
        }
        
        if event.key in key_map:
            self.on_button(key_map[event.key])
        elif event.key == PYTUINT.Key.Q:
            return None
        
        return self.render()

def main():
    term = PYTUINT.Terminal()
    term.init()
    term.set_title("PYTUINT Calculator")
    term.show_cursor(False)
    
    calc = Calculator()
    calc.set_state("terminal", term)
    
    reconciler = PYTUINT.Reconciler(term)
    reconciler.mount(calc)
    
    running = True
    while running:
        event = term.get_event(100)
        if event.type != PYTUINT.EventType.NoEvent:
            result = calc.handle_event(event)
            if result is None:
                running = False
            else:
                reconciler.render(result)
    
    term.show_cursor(True)
    term.shutdown()

if __name__ == "__main__":
    main()