local enum EventType
   "move_pointer"
   "mouse_press"
   "mouse_release"
   "key_down"
   "key_up"

   "clear_board"
end

local record Event
   type: EventType
   x: integer
   y: integer
   button: string
   key: string
end

return Event

-- vim:sw=3:st=3:sts=3:noexpandtab
