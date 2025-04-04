require "board.geo.position"
require "board.geo.positionset"

local function contains<T>(array: {any:T}, item: T) : boolean
   assert(type(array) == 'table')
   for _,v in pairs(array) do if v == item then return true end end
   return false
end

local function tile_neighbours(p: Position, has_single_tile_component_pin: boolean): {Position}

    local positions : {Position} = {}
    
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

global function find_connected_wires(wires: PositionSet, single_tile_component_pos: {Position}) :{{Position}}
   
   local function find_connected_group(start: Position) : {Position}
     local result : {Position} = {}
     local to_visit = PositionSet.new()
     to_visit:add(start)
     
     while to_visit:len() > 0 do
       local visiting = to_visit:first() as Position
       
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
      return {}
   end

   local groups = {}
   while wires:len() > 0 do
      groups[#groups+1] = find_connected_group(wires:first())
   end
   return groups
end

-- vim:sw=3:st=3:sts=3:noexpandtab
