function find_connected_wires(wires, single_tile_component_pos)
   
   local function find_connected_group(start, wires, single_tile_component_pos)
      -- TODO
   end
   
   assert(getmetatable(wires) == PositionSet)
   
   if wires:len() == 0 then
      return nil
   end
   
   local groups = {}
   while wires:len() > 0 do
      groups[#groups+1] = find_connected_group(wires:first(), wires, single_tile_component_pos)
   end
   return groups
end