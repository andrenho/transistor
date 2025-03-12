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
         return {}
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
   
   compile = function(sandbox)
      local connections = {}
      
      local pins = compiler.find_all_pins(sandbox)
      
      -- find single-tile component pins
      local single_tile_component_pins = {}
      for _,pin in ipairs(pins) do
         if pin.component.def.type == "single_tile" then
            single_tile_component_pins[#single_tile_component_pins+1] = P(pin.pos.x, pin.pos.y, CENTER)
         end
      end
      
      -- create set of wires
      local wire_set = PositionSet.new()
      for _,board in ipairs(sandbox.boards) do
         for pos_hash,_ in pairs(board.wires) do
            wire_set:add(Position.unhash(pos_hash))
         end
      end
      
      -- find groups, and create connections
      for _,group in ipairs(compiler.find_connected_wires(wire_set, single_tile_component_pins)) do
         
         local connection = {
            wires = {},
            pins = {},
         }
         
         -- add wires to connection, and add to the list of central pins
         local central_pins = PositionSet.new()
         for _,pos in ipairs(group) do
            
            -- add to central pins
            central_pins:add(pos.x, pos.y, CENTER)
            
            -- add wire to connection
            connection.wires[#connection.wires+1] = pos
            
            -- add pin (single-tile components) to connection
            for _,pin in ipairs(pins) do
               if pos == pin.pos then
                  connection.pins[#connection.pins+1] = pin
               end
            end
            
         end
         
         -- add pins (IC)
         for _,central_pin in pairs(central_pins.items) do
            for _,pin in ipairs(pins) do
               if central_pin == pin.pos then
                  connection.pins[#connection.pins+1] = pin
               end
            end
         end
         
         connections[#connections+1] = connection
      end
      
      return connections
   end,
   
   snapshot = function(sandbox, connections)
      local snap = {
         components = {},
         connections = {}
      }
      
      -- components to be simulated in C
      for _,board in ipairs(sandbox.boards) do
         for _,c in ipairs(board.components) do
            snap.components[#snap.components+1] = { c.def.key, c.data:ptr(), c.pin:ptr(), c.def.simulate == nil, board:pos_hash_c(c.position) }
         end
      end
      
      -- add connections
      for _,connection in ipairs(connections) do
         local conn = { pins = {}, wires = {} }
         for _,pin in ipairs(connection.pins) do
            conn.pins[#conn.pins+1] = { pin.component.pin:ptr(), pin.pin_no, pin.component.def.pins[pin.pin_no].direction }
         end
         for _,wire_pos in pairs(connection.wires) do
            conn.wires[#conn.wires+1] = sandbox.boards[1]:pos_hash_c(wire_pos) -- TODO - add board
         end
         snap.connections[#snap.connections+1] = conn
      end
      
      return snap
   end,
}