compiler = {

   find_connected_wires = function(wires, single_tile_component_pos)
      
      assert(getmetatable(wires) == PositionSet)
      
      local function tile_neighbours(p, has_single_tile_component_pin)
         local positions = {}
         
         if has_single_tile_component_pin then
            if p.dir == W then positions[#positions + 1] = P(p.x - 1, p.y, E)
            elseif p.dir == E then positions[#positions + 1] = P(p.x + 1, p.y, W)
            elseif p.dir == N then positions[#positions + 1] = P(p.x, p.y - 1, S)
            elseif p.dir == S then positions[#positions + 1] = P(p.x, p.y + 1, N)
            else assert(false) end
         
         else
            if p.dir == W then
               positions[#positions + 1] = P(p.x - 1, p.y, E)
               positions[#positions + 1] = P(p.x, p.y, E)
               positions[#positions + 1] = P(p.x, p.y, N)
               positions[#positions + 1] = P(p.x, p.y, S)
            elseif p.dir == E then
               positions[#positions + 1] = P(p.x + 1, p.y, W)
               positions[#positions + 1] = P(p.x, p.y, W)
               positions[#positions + 1] = P(p.x, p.y, N)
               positions[#positions + 1] = P(p.x, p.y, S)
            elseif p.dir == N then
               positions[#positions + 1] = P(p.x, p.y - 1, S)
               positions[#positions + 1] = P(p.x, p.y, S)
               positions[#positions + 1] = P(p.x, p.y, W)
               positions[#positions + 1] = P(p.x, p.y, E)
            elseif p.dir == S then
               positions[#positions + 1] = P(p.x, p.y + 1, N)
               positions[#positions + 1] = P(p.x, p.y, N)
               positions[#positions + 1] = P(p.x, p.y, W)
               positions[#positions + 1] = P(p.x, p.y, E)
            else
               assert(false)
            end
         end
         
         return positions
      end
      
      local function find_connected_group(start)
         local result = {}
         local to_visit = PositionSet.new()
         to_visit:add(start)
         
         while to_visit:len() > 0 do
            local visiting = to_visit:first()
            
            if wires:contains(visiting) then
               
               -- add to result list, and remove from visited
               result[#result+1] = visiting
               wires:remove(visiting)
               
               -- find if we have a single tile component pin here
               local has_single_tile_component_pin = contains(single_tile_component_pos, P(visiting.x, visiting.y, CENTER))
               
               -- add neighbours
               local neighbours = tile_neighbours(visiting, has_single_tile_component_pin)
               for _,neighbour in ipairs(neighbours) do
                  to_visit:add(neighbour)
               end
            end
            
            to_visit:remove(visiting)
         end
         
         return result
      end
      
      if wires:len() == 0 then
         return nil
      end
      
      local groups = {}
      while wires:len() > 0 do
         groups[#groups+1] = find_connected_group(wires:first())
      end
      return groups
   end,

   find_all_pins = function(sandbox)
      assert(getmetatable(sandbox) == Sandbox)
      
      local pins = {}
      
      for _,board in ipairs(sandbox.boards) do
         for _,component in ipairs(board.components) do
            for _,pp in ipairs(component:pin_positions()) do
               pins[#pins+1] = {
                  board = board,
                  component = component,
                  pos = pp.pos,
                  pin_no = pp.pin_no,
               }
            end
         end
      end
      
      return pins
   end,
   
}