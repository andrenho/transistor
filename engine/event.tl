global enum EventType
   -- user interface events
   "move_pointer"
   "mouse_press"
   "mouse_wheel"
   "mouse_release"
   "key_down"
   "key_up"

   -- WM events
   "window_resize"

   -- user commands
   "select_key"
   "clear_board"
   "show_imgui_demo"
end

global enum EventButton
   "left"
   "middle"
   "right"
end

global record KeyMod
   ctrl: boolean
   alt: boolean
   shift: boolean
end

global record Event
   type: EventType
   x: integer
   y: integer
   button: EventButton
   key: string
   mod: KeyMod
   index: integer
end

-- vim:sw=3:st=3:sts=3:noexpandtab
