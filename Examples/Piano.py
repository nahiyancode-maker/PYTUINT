#!/usr/bin/env python3
"""Piano - Musical terminal piano with PTUI"""

import ptui
import ptui.widgets as widgets
import math

class Piano(ptui.Component):
    def __init__(self):
        super().__init__()
        self.active_notes = set()
        self.octave = 4
        self.volume = 0.5
        
    def render(self):
        term = self.state("terminal")
        if not term:
            return ptui.Element.create("container")
        
        # Title
        header = widgets.Text("🎹 PTUI Piano", ptui.Style()
            .fg(ptui.colors.BrightMagenta)
            .bold()
            .set_text_align(ptui.TextAlign.Center)
            .set_width(term.width()))
        
        # Piano keys
        keys = []
        white_keys = ['C', 'D', 'E', 'F', 'G', 'A', 'B']
        black_keys = ['C#', 'D#', '', 'F#', 'G#', 'A#', '']
        
        # White keys
        for i, note in enumerate(white_keys):
            is_active = note in self.active_notes
            style = ptui.Style().bg(ptui.colors.White if is_active else ptui.colors.LightGray).fg(ptui.colors.Black).set_width(5).set_height(3)
            
            key = widgets.Container(style, [
                widgets.Text(note, ptui.Style().set_text_align(ptui.TextAlign.Center))
            ])
            keys.append(key.render())
        
        # Black keys (overlay)
        black_key_elements = []
        for i, note in enumerate(black_keys):
            if not note:
                continue
            is_active = note in self.active_notes
            style = ptui.Style().bg(ptui.colors.Black if is_active else ptui.colors.DarkGray).fg(ptui.colors.White).set_width(3).set_height(2)
            
            key = widgets.Container(style, [
                widgets.Text(note, ptui.Style().set_text_align(ptui.TextAlign.Center))
            ])
            black_key_elements.append(key.render())
        
        # Controls
        controls = widgets.Container(
            ptui.Style()
                .set_flex_direction(ptui.FlexDirection.Row)
                .set_justify_content(ptui.JustifyContent.Center)
                .set_padding(1),
            [
                widgets.Button("◀ Octave", lambda: self.change_octave(-1), 
                    ptui.Style().bg(ptui.colors.Blue).fg(ptui.colors.White)),
                widgets.Text(f"  Octave: {self.octave}  ", ptui.Style().fg(ptui.colors.White)),
                widgets.Button("Octave ▶", lambda: self.change_octave(1),
                    ptui.Style().bg(ptui.colors.Blue).fg(ptui.colors.White)),
                widgets.Text("  Volume: ", ptui.Style().fg(ptui.colors.White)),
                widgets.Button("-", lambda: self.change_volume(-0.1),
                    ptui.Style().bg(ptui.colors.Red).fg(ptui.colors.White)),
                widgets.Text(f" {int(self.volume * 100)}% ", ptui.Style().fg(ptui.colors.White)),
                widgets.Button("+", lambda: self.change_volume(0.1),
                    ptui.Style().bg(ptui.colors.Green).fg(ptui.colors.White)),
            ]
        )
        
        # Help
        help_text = widgets.Text(
            "Keys: A S D F G H J (white) | W E T Y U (black) | Q: quit",
            ptui.Style().fg(ptui.colors.Gray).set_text_align(ptui.TextAlign.Center)
        )
        
        # Layout
        layout = widgets.Container(
            ptui.Style()
                .set_flex_direction(ptui.FlexDirection.Column)
                .set_align_items(ptui.AlignItems.Center)
                .set_padding(2),
            [
                header,
                widgets.Container(
                    ptui.Style().set_flex_direction(ptui.FlexDirection.Row),
                    keys
                ),
                controls,
                help_text
            ]
        )
        
        return layout.render()
    
    def change_octave(self, delta):
        self.octave = max(1, min(7, self.octave + delta))
        self.force_update()
    
    def change_volume(self, delta):
        self.volume = max(0.0, min(1.0, self.volume + delta))
        self.force_update()
    
    def handle_event(self, event):
        key_map = {
            ptui.Key.A: 'C', ptui.Key.S: 'D', ptui.Key.D: 'E', ptui.Key.F: 'F',
            ptui.Key.G: 'G', ptui.Key.H: 'A', ptui.Key.J: 'B',
            ptui.Key.W: 'C#', ptui.Key.E: 'D#', ptui.Key.T: 'F#',
            ptui.Key.Y: 'G#', ptui.Key.U: 'A#'
        }
        
        if event.key in key_map:
            self.active_notes.add(key_map[event.key])
            self.force_update()
        elif event.key == ptui.Key.Q:
            return None
        
        return self.render()

def main():
    term = ptui.Terminal()
    term.init()
    term.set_title("PTUI Piano")
    term.show_cursor(False)
    
    piano = Piano()
    piano.set_state("terminal", term)
    
    reconciler = ptui.Reconciler(term)
    reconciler.mount(piano)
    
    running = True
    while running:
        event = term.get_event(50)
        if event.type != ptui.EventType.NoEvent:
            result = piano.handle_event(event)
            if result is None:
                running = False
            else:
                reconciler.render(result)
    
    term.show_cursor(True)
    term.shutdown()

if __name__ == "__main__":
    main()