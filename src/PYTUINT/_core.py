import curses
import sys

def run(app_func):
    """
    Main entry point to run a PYTUINT application
    """
    # Initialize curses
    stdscr = curses.initscr()
    curses.start_color()
    curses.use_default_colors()
    curses.noecho()      # Don't echo keyboard input
    curses.cbreak()      # React to keys instantly (no Enter needed)
    stdscr.keypad(True)  # Enable special keys (arrows, etc.)
    stdscr.nodelay(True)  # Non-blocking input
    
    # Initialize color pairs
    curses.init_pair(1, curses.COLOR_WHITE, curses.COLOR_BLUE)
    curses.init_pair(2, curses.COLOR_BLACK, curses.COLOR_WHITE)
    curses.init_pair(3, curses.COLOR_GREEN, curses.COLOR_BLACK)
    curses.init_pair(4, curses.COLOR_YELLOW, curses.COLOR_BLACK)
    curses.init_pair(5, curses.COLOR_RED, curses.COLOR_BLACK)
    
    # Hide cursor
    curses.curs_set(0)
    
    try:
        # Get the view from app function
        view = app_func()
        
        # Event loop
        while True:
            # Clear screen
            stdscr.clear()
            
            # Get terminal dimensions
            height, width = stdscr.getmaxyx()
            
            # Render the view (you'll need to implement this)
            _render_view(stdscr, view, 0, 0, height, width)
            
            # Refresh screen
            stdscr.refresh()
            
            # Handle input
            key = stdscr.getch()
            if key == ord('q') or key == 27:  # 'q' or ESC to quit
                break
            elif key == curses.KEY_RESIZE:
                # Handle terminal resize
                pass
            
            # Small delay to prevent CPU spinning
            curses.napms(50)
            
    finally:
        # Cleanup and restore terminal
        curses.nocbreak()
        stdscr.keypad(False)
        curses.echo()
        curses.endwin()

def _render_view(stdscr, view, y, x, max_height, max_width):
    """
    Recursively render a View and its children to the terminal
    """
    if isinstance(view, View):
        # Draw border if specified
        if view.border:
            border_char = {
                "single": "│─└┘┌┐",
                "double": "║═╚╝╔╗",
                "rounded": "│─╭╮╰╯"
            }.get(view.border, "      ")
            
            # Draw box
            for row in range(view.height):
                for col in range(view.width):
                    if row == 0 or row == view.height - 1:
                        stdscr.addch(y + row, x + col, border_char[1] if col > 0 and col < view.width - 1 else border_char[0])
                    elif col == 0 or col == view.width - 1:
                        stdscr.addch(y + row, x + col, border_char[0])
        
        # Render children
        child_y = y + 1
        child_x = x + 1
        for child in view.children:
            _render_view(stdscr, child, child_y, child_x, max_height, max_width)
            
    elif isinstance(view, Text):
        # Render text content
        content = view.content[:max_width - x]  # Truncate to fit
        stdscr.addstr(y, x, content)
        
    elif isinstance(view, Button):
        # Render button with brackets
        btn_text = f"[ {view.label}]"
        stdscr.addstr(y, x, btn_text, curses.color_pair(2) | curses.A_BOLD)
