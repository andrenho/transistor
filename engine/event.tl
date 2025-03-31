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

local record Event
   type: EventType
   x: integer
   y: integer
   button: EventButton
   key: string
   index: integer
end

return Event

-- vim:sw=3:st=3:sts=3:noexpandtab
