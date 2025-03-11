ComponentDef = {}
ComponentDef.__index = ComponentDef

function ComponentDef.validate(def)
   -- TODO
end

function ComponentDef:rect(pos, dir)
   if self.type == "single_tile" then
      return Rect.new(pos, pos)
   elseif self.type == "ic_dip" then
      local h = math.floor(#self.pins / 2)
      if dir == N or dir == S then
         return Rect.new(pos.x - 1, pos.y - 1, pos.x + self.ic_width, pos.y + h)
      else
         return Rect.new(pos.x - 1, pos.y - 1, pos.x + h, pos.y + self.ic_width)
      end
   elseif self.type == "ic_quad" then
      local h = math.floor(#self.pins / 4)
      return Rect.new(pos.x - 1, pos.y - 1, pos.x + h, pos.y + h)
   else
      assert(false)
   end
end

function ComponentDef:pin_positions_single_tile(pos, dir)
   local function next_dir(d)
      if d == N then return W end
      if d == W then return S end
      if d == S then return E end
      if d == E then return N end
      assert(false)
   end
   
   local pin_pos = {}
   if #self.pins == 4 then
      for i=1,4 do
         pin_pos[i] = { pin_no = i, pos = P(pos.x, pos.y, dir) }
         dir = next_dir(dir)
      end
   elseif #self.pins == 2 then
      pin_pos[1] = { pin_no = 1, pos = P(pos.x, pos.y, dir) }
      pin_pos[2] = { pin_no = 2, pos = P(pos.x, pos.y, next_dir(next_dir(dir))) }
   elseif #self.pins == 1 then
      pin_pos[1] = { pin_no = 1, pos = P(pos.x, pos.y, dir) }
   else
      assert(false)
   end
   return pin_pos
end

function ComponentDef:pin_positions_ic_dip(pos, dir)
   local h = math.floor(#self.pins / 2)
   local pin_pos = {}
   if dir == N then
      for i=0,(h-1) do pin_pos[#pin_pos+1] = { pos = P(pos.x - 1, pos.y + i) } end
      for i=(h-1),0,-1 do pin_pos[#pin_pos+1] = { pos = P(pos.x + self.ic_width, pos.y + i) } end
   elseif dir == E then
      for i=0,(h-1) do pin_pos[#pin_pos+1] = { pos = P(pos.x + i, pos.y + self.ic_width) } end
      for i=(h-1),0,-1 do pin_pos[#pin_pos+1] = { pos = P(pos.x + i, pos.y - 1) } end
   elseif dir == S then
      for i=(h-1),0,-1 do pin_pos[#pin_pos+1] = { pos = P(pos.x + self.ic_width, pos.y + i) } end
      for i=0,(h-1) do pin_pos[#pin_pos+1] = { pos = P(pos.x - 1, pos.y + i) } end
   elseif dir == W then
      for i=(h-1),0,-1 do pin_pos[#pin_pos+1] = { pos = P(pos.x + i, pos.y - 1) } end
      for i=0,(h-1) do pin_pos[#pin_pos+1] = { pos = P(pos.x + i, pos.y + self.ic_width) } end
   else
      assert(false)
   end
   for i,p in ipairs(pin_pos) do p.pin_no = i end
   return pin_pos
end

function ComponentDef:pin_positions_ic_quad(pos, dir)
   local d
   if dir == N then d = { W, S, E, N } end
   if dir == E then d = { N, W, S, E } end
   if dir == S then d = { E, N, W, S } end
   if dir == W then d = { S, E, N, W } end
   
   local h = math.floor(#self.pins / 4)
   local pin_pos = {}
   for k=1,4 do
      if d[k] == W then
         for i=0,(h-1) do pin_pos[#pin_pos+1] = { pos = P(pos.x - 1, pos.y + i) } end
      elseif d[k] == S then
         for i=0,(h-1) do pin_pos[#pin_pos+1] = { pos = P(pos.x + i, pos.y + h) } end
      elseif d[k] == E then
         for i=(h-1),0,-1 do pin_pos[#pin_pos+1] = { pos = P(pos.x + h, pos.y + i) } end
      elseif d[k] == N then
         for i=(h-1),0,-1 do pin_pos[#pin_pos+1] = { pos = P(pos.x + i, pos.y - 1) } end
      end
   end
   
   for i,p in ipairs(pin_pos) do p.pin_no = i end
   return pin_pos
end

function ComponentDef:pin_positions(pos, dir)
   if self.type == "single_tile" then
      return self:pin_positions_single_tile(pos, dir)
   elseif self.type == "ic_dip" then
      return self:pin_positions_ic_dip(pos, dir)
   elseif self.type == "ic_quad" then
      return self:pin_positions_ic_quad(pos, dir)
   else
      assert(false)
   end
end