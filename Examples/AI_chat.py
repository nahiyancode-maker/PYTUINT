#!/usr/bin/env python3
"""AI Chat CLI - ChatGPT-style terminal chat with PTUI"""

import ptui
import ptui.widgets as widgets
import random
import time

class AIChat(ptui.Component):
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
            return ptui.Element.create("container")
        
        # Header
        header = widgets.Text("PTUI AI Chat", ptui.Style()
            .fg(ptui.colors.BrightCyan)
            .bold()
            .set_text_align(ptui.TextAlign.Center)
            .set_width(term.width()))
        
        # Messages
        msg_elements = []
        for i, msg in enumerate(self.messages):
            style = ptui.Style().fg(ptui.colors.Green) if msg["role"] == "user" else ptui.Style().fg(ptui.colors.White)
            prefix = "You: " if msg["role"] == "user" else "AI: "
            msg_elements.append(widgets.Text(prefix + msg["content"], style))
        
        # Input
        input_box = widgets.TextInput(
            placeholder="Type your message... (Enter to send, Esc to quit)",
            on_change=self.handle_input_change,
            style=ptui.Style()
                .fg(ptui.colors.Gray)
                .set_width(term.width() - 4)
        )
        
        # Layout
        layout = widgets.Container(
            ptui.Style()
                .set_flex_direction(ptui.FlexDirection.Column)
                .set_padding(1),
            [
                header,
                widgets.ScrollView(
                    widgets.Container(
                        ptui.Style().set_flex_direction(ptui.FlexDirection.Column),
                        msg_elements
                    ),
                    ptui.Style().set_height(term.height() - 5)
                ),
                input_box
            ]
        )
        
        return layout.render()
    
    def handle_input_change(self, value):
        self.input_value = value
    
    def handle_event(self, event):
        if event.key == ptui.Key.Enter and self.input_value.strip():
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
        
        elif event.key == ptui.Key.Escape:
            return None  # Quit
        
        return self.render()

def main():
    term = ptui.Terminal()
    term.init()
    term.set_title("PTUI AI Chat")
    term.show_cursor(False)
    
    chat = AIChat()
    chat.set_state("terminal", term)
    
    # Initial render
    reconciler = ptui.Reconciler(term)
    reconciler.mount(chat)
    
    # Event loop
    running = True
    while running:
        event = term.get_event(100)
        if event.type != ptui.EventType.NoEvent:
            result = chat.handle_event(event)
            if result is None:
                running = False
            else:
                reconciler.render(result)
    
    term.show_cursor(True)
    term.shutdown()

if __name__ == "__main__":
    main()