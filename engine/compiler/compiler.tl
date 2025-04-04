require "sandbox"
require "board.board"
require "components.component"
require "board.geo.position"
require "board.geo.positionset"
require "compiler.connections"

global record Pin
   component: Component
   pos: Position
   pin_no: integer
end

global record Connection
   wires: {Position}
   pins: {Pin}
end

global function find_all_pins(msandbox: Sandbox) : {Pin}
   local pins = {}
   
   for _,component in ipairs(msandbox.board.components) do
	  for _,pp in ipairs(component:pin_positions()) do
		 pins[#pins+1] = {
			component = component,
			pos = pp.pos,
			pin_no = pp.pin_no,
		 }
	  end
   end
   
   return pins
end

global function compile(msandbox: Sandbox) : {Connection}
   local connections : {Connection} = {}
   
   local pins = find_all_pins(msandbox)
   
   -- find single-tile component pins
   local single_tile_component_pins = {}
   for _,pin in ipairs(pins) do
	  if pin.component.def.type == "single_tile" then
		 single_tile_component_pins[#single_tile_component_pins+1] = P(pin.pos.x, pin.pos.y, CENTER)
	  end
   end
   
   -- create set of wires
   local wire_set = PositionSet.new()
   for pos_hash,_ in pairs(msandbox.board.wires) do
	  wire_set:add(Position.unhash(pos_hash))
   end
   
   -- find groups, and create connections
   for _,group in ipairs(find_connected_wires(wire_set, single_tile_component_pins)) do
	  
	  local connection : Connection = {
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
end

-- vim:sw=3:st=3:sts=3:noexpandtab
