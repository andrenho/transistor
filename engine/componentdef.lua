ComponentDef = {}
ComponentDef.__index = ComponentDef

function ComponentDef.validate(def)
   -- TODO
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
end

function ComponentDef:pin_positions_ic_quad(pos, dir)
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