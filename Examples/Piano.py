#!/usr/bin/env python3
"""Piano - Musical terminal piano with PYTUINT"""

import PYTUINT
import PYTUINT.widgets as widgets
import math

class Piano(PYTUINT.Component):
    def __init__(self):
        super().__init__()
        self.active_notes = set()
        self.octave = 4
        self.volume = 0.5
        
    def render(self):
        term = self.state("terminal")
        if not term:
            return PYTUINT.Element.create("container")
        
        # Title
        header = widgets.Text("🎹 PYTUINT Piano", PYTUINT.Style()
            .fg(PYTUINT.colors.BrightMagenta)
            .bold()
            .set_text_align(PYTUINT.TextAlign.Center)
            .set_width(term.width()))
        
        # Piano keys
        keys = []
        white_keys = ['C', 'D', 'E', 'F', 'G', 'A', 'B']
        black_keys = ['C#', 'D#', '', 'F#', 'G#', 'A#', '']
        
        # White keys
        for i, note in enumerate(white_keys):
            is_active = note in self.active_notes
            style = PYTUINT.Style().bg(PYTUINT.colors.White if is_active else PYTUINT.colors.LightGray).fg(PYTUINT.colors.Black).set_width(5).set_height(3)
            
            key = widgets.Container(style, [
                widgets.Text(note, PYTUINT.Style().set_text_align(PYTUINT.TextAlign.Center))
            ])
            keys.append(key.render())
        
        # Black keys (overlay)
        black_key_elements = []
        for i, note in enumerate(black_keys):
            if not note:
                continue
            is_active = note in self.active_notes
            style = PYTUINT.Style().bg(PYTUINT.colors.Black if is_active else PYTUINT.colors.DarkGray).fg(PYTUINT.colors.White).set_width(3).set_height(2)
            
            key = widgets.Container(style, [
                widgets.Text(note, PYTUINT.Style().set_text_align(PYTUINT.TextAlign.Center))
            ])
            black_key_elements.append(key.render())
        
        # Controls
        controls = widgets.Container(
            PYTUINT.Style()
                .set_flex_direction(PYTUINT.FlexDirection.Row)
                .set_justify_content(PYTUINT.JustifyContent.Center)
                .set_padding(1),
            [
                widgets.Button("◀ Octave", lambda: self.change_octave(-1), 
                    PYTUINT.Style().bg(PYTUINT.colors.Blue).fg(PYTUINT.colors.White)),
                widgets.Text(f"  Octave: {self.octave}  ", PYTUINT.Style().fg(PYTUINT.colors.White)),
                widgets.Button("Octave ▶", lambda: self.change_octave(1),
                    PYTUINT.Style().bg(PYTUINT.colors.Blue).fg(PYTUINT.colors.White)),
                widgets.Text("  Volume: ", PYTUINT.Style().fg(PYTUINT.colors.White)),
                widgets.Button("-", lambda: self.change_volume(-0.1),
                    PYTUINT.Style().bg(PYTUINT.colors.Red).fg(PYTUINT.colors.White)),
                widgets.Text(f" {int(self.volume * 100)}% ", PYTUINT.Style().fg(PYTUINT.colors.White)),
                widgets.Button("+", lambda: self.change_volume(0.1),
                    PYTUINT.Style().bg(PYTUINT.colors.Green).fg(PYTUINT.colors.White)),
            ]
        )
        
        # Help
        help_text = widgets.Text(
            "Keys: A S D F G H J (white) | W E T Y U (black) | Q: quit",
            PYTUINT.Style().fg(PYTUINT.colors.Gray).set_text_align(PYTUINT.TextAlign.Center)
        )
        
        # Layout
        layout = widgets.Container(
            PYTUINT.Style()
                .set_flex_direction(PYTUINT.FlexDirection.Column)
                .set_align_items(PYTUINT.AlignItems.Center)
                .set_padding(2),
            [
                header,
                widgets.Container(
                    PYTUINT.Style().set_flex_direction(PYTUINT.FlexDirection.Row),
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
            PYTUINT.Key.A: 'C', PYTUINT.Key.S: 'D', PYTUINT.Key.D: 'E', PYTUINT.Key.F: 'F',
            PYTUINT.Key.G: 'G', PYTUINT.Key.H: 'A', PYTUINT.Key.J: 'B',
            PYTUINT.Key.W: 'C#', PYTUINT.Key.E: 'D#', PYTUINT.Key.T: 'F#',
            PYTUINT.Key.Y: 'G#', PYTUINT.Key.U: 'A#'
        }
        
        if event.key in key_map:
            self.active_notes.add(key_map[event.key])
            self.force_update()
        elif event.key == PYTUINT.Key.Q:
            return None
        
        return self.render()

def main():
    term = PYTUINT.Terminal()
    term.init()
    term.set_title("PYTUINT Piano")
    term.show_cursor(False)
    
    piano = Piano()
    piano.set_state("terminal", term)
    
    reconciler = PYTUINT.Reconciler(term)
    reconciler.mount(piano)
    
    running = True
    while running:
        event = term.get_event(50)
        if event.type != PYTUINT.EventType.NoEvent:
            result = piano.handle_event(event)
            if result is None:
                running = False
            else:
                reconciler.render(result)
    
    term.show_cursor(True)
    term.shutdown()

if __name__ == "__main__":
    main()