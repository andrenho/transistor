Cursor = {}
Cursor.__index = Cursor

BUTTON_LEFT   = "left"
BUTTON_MIDDLE = "middle"
BUTTON_RIGHT  = "right"

function Cursor.new(board)
   local self = setmetatable({}, Cursor)
   self.board = board
   self.pos = nil
   self.selected_direction = N
   self.selected_wire = WR(LAYER_TOP, WIDTH_1)
   self.selected_key = nil
   self.erasing = false
   self.wire = {
      drawing = false,
      starting_pos = nil,
      orientation = HORIZONTAL,
      orientation_defined = false,
   }
   return self
end

function Cursor:set_pointer(pos)   -- nil = out of bounds
   if pos.x < 0 or pos.y < 0 or pos.x >= self.board.w or pos.y >= self.board.h then
      self.pos = nil
      return
   end
   
   self.pos = pos
   
   if pos ~= nil then
      if self.wire.drawing then
         
         local dx = self.wire.starting_pos.x - pos.x
         local dy = self.wire.starting_pos.y - pos.y
         if not self.wire.orientation_defined and (dx ~= 0 or dy ~= 0) then
            if math.abs(dx) > math.abs(dy) then
               self.wire.orientation = HORIZONTAL
            else
               self.wire.orientation = VERTICAL
            end
            self.wire.orientation_defined = true
         end
         
         if self.wire.orientation_defined and dx == 0 and dy == 0 then
            self.wire.orientation_defined = false
         end
         
      end
      
      if self.erasing then
         self.board:clear_tile(P(self.pos.x, self.pos.y))
      end
   end
end

function Cursor:click(button)
   if self.pos then
      if button == BUTTON_LEFT then
         local component = self.board:component(self.pos)
         if component then
            component:on_click()
         elseif self.selected_key then
            self.board:add_component(self.selected_key, self,pos, self.selected_direction)
         end
      end
   end
end

function Cursor:button_release(button)
end

function Cursor:key_press(key, mod)
   if self.pos then
      if key == 'r' then
         local component = self.board:component(self.pos)
         if component then
            self.board:rotate_tile(self.pos)
         else
            self.selected_direction = dir_rotate_component(self.selected_direction)
         end
      elseif key == 'v' then self.board:add_component("__vcc", self.pos, self.selected_direction)
      elseif key == 'b' then self.board:add_component("__button", self.pos, self.selected_direction)
      elseif key == 'l' then self.board:add_component("__led", self.pos, self.selected_direction)
      elseif key == 'p' then self.board:add_component("__pnp", self.pos, self.selected_direction)
      elseif key == 'n' then self.board:add_component("__npn", self.pos, self.selected_direction)
      elseif key == 'x' then
         self.erasing = true
         self.board:clear_tile(P(self.pos.x, self.pos.y))
      elseif key == 'w' then
         self.wire.drawing = true
         self.wire.orientation_defined = false
         self.wire.orientation = HORIZONTAL
         self.wire.starting_pos = self.pos
      end
   end
end

function Cursor:key_release(key)
   if key == 'w' and self.wire.drawing then
      if self.pos == self.wire.starting_pos and self.pos.dir ~= CENTER then
         self.board:add_wire(self.pos, self.selected_wire)
      else
         self.board:add_wires(self.wire.starting_pos, self.pos, self.wire.orientation, self.selected_wire)
      end
      self.wire.drawing = false
   elseif key == "x" then
      self.erasing = false
   end
end

function Cursor:select_component_def(key)
   assert(type(key) == 'string')
   self.selected_key = key
end