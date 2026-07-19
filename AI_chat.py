#!/usr/bin/env python3
"""AI Chat CLI - ChatGPT-style terminal chat with PTUI"""

import PYTUINT

import random


class AIChat(PYTUINT.Component):
    def __init__(self):
        super().__init__()
        self.messages = [
            {"role": "ai", "content": "Hello! I'm PTUI AI. How can I help you today?"}
        ]
        self.input_value = ""
        self.scroll_offset = 0
        self.typing = False
        
    def render(self):
        term = self.state("terminal")
        if not term:
            return PYTUINT.Element.create("container")
        
        # Header
        header = PYTUINT.Text("PYTUINT AI Chat", PYTUINT.Style()
            .fg(PYTUINT.colors.BrightCyan)
            .bold()
            .set_text_align(PYTUINT.TextAlign.Center)
            .set_width(term.width()))
        
        # Messages
        msg_elements = []
        for i, msg in enumerate(self.messages):
            style = PYTUINT.Style().fg(PYTUINT.colors.Green) if msg["role"] == "user" else PYTUINT.Style().fg(PYTUINT.colors.White)
            prefix = "You: " if msg["role"] == "user" else "AI: "
            msg_elements.append(PYTUINT.Text(prefix + msg["content"], style))
        
        # Input
        input_box = PYTUINT.TextInput(
            placeholder="Type your message... (Enter to send, Esc to quit)",
            on_change=self.handle_input_change,
            style=PYTUINT.Style()
                .fg(PYTUINT.colors.Gray)
                .set_width(term.width() - 4)
        )
        
        # Layout
        layout = PYTUINT.Container(
            PYTUINT.Style()
                .set_flex_direction(PYTUINT.FlexDirection.Column)
                .set_padding(1),
            [
                header,
                PYTUINT.ScrollView(
                    PYTUINT.Container(
                        PYTUINT.Style().set_flex_direction(PYTUINT.FlexDirection.Column),
                        msg_elements
                    ),
                    PYTUINT.Style().set_height(term.height() - 5)
                ),
                input_box
            ]
        )
        
        return layout.render()
    
    def handle_input_change(self, value):
        self.input_value = value
    
    def handle_event(self, event):
        if event.key == PYTUINT.Key.Enter and self.input_value.strip():
            self.messages.append({"role": "user", "content": self.input_value})
            self.typing = True
            self.input_value = ""
            
            # Simulate AI response
            responses = [
                "That's an interesting question!",
                "Let me think about that...",
                "I understand what you mean.",
                "Could you elaborate more?",
                "Here's what I think about that...",
                "Great point!",
                "I'm not sure about that, but I can try to help."
            ]
            self.messages.append({"role": "ai", "content": random.choice(responses)})
            self.typing = False
            self.force_update()
        
        elif event.key == PYTUINT.Key.Escape:
            return None  # Quit
        
        return self.render()

def main():
    term = PYTUINT.Terminal()
    term.init()
    term.set_title("PYTUINT AI Chat")
    term.show_cursor(False)
    
    chat = AIChat()
    chat.set_state("terminal", term)
    
    # Initial render
    reconciler = PYTUINT.Reconciler(term)
    reconciler.mount(chat)
    
    # Event loop
    running = True
    while running:
        event = term.get_event(100)
        if event.type != PYTUINT.EventType.NoEvent:
            result = chat.handle_event(event)
            if result is None:
                running = False
            else:
                reconciler.render(result)
    
    term.show_cursor(True)
    term.shutdown()

if __name__ == "__main__":
    main()