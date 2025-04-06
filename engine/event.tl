global enum EventType
   "move_pointer"
   "mouse_press"
   "mouse_wheel"
   "mouse_release"
   "key_down"
   "key_up"

   "clear_board"
   "window_resize"
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
