require "sandbox"
require "compiler.compiler"
require "board.geo.position"

print("Compilation")

local function circuit_single_tile() : Sandbox
   local msandbox = Sandbox.new()
   local board = msandbox.board
   board:add_component("__npn", P(1, 1), N)
   board:add_component("__led", P(3, 1), N)
   board:add_wires(P(1, 1), P(3, 1), HORIZONTAL, WR("top", "1"))
   return msandbox
end

local function circuit_ic(dir: Direction) : Sandbox
   local msandbox = Sandbox.new()
   local board = msandbox.board
   board:add_component("__npn", P(1, 1), N)
   board:add_component("__or_2i", P(3, 1), dir)
   board:add_wires(P(1, 1), P(3, 1), HORIZONTAL, WR("top", "1"))
   return msandbox
end

local function has_pin(connection: Connection, pin_no: integer, key: string) : boolean
   for _,pin in ipairs(connection.pins) do
      if pin.pin_no == pin_no and pin.component.def.key == key then
         return true
      end
   end
   return false
end

print(" - Basic circuit")

do
   print("   - Find all pins")
   local msandbox = circuit_single_tile()
   local pins = find_all_pins(msandbox)
   
   local function get(pos: Position, pin_no: integer) : Component
      for _,pin in ipairs(pins) do
         if pin.pos == pos and pin.pin_no == pin_no then return pin.component end
      end
      return nil
   end
   
   assert(#pins == 8)
   assert(get(P(1, 1, N), 1).def.key == "__npn")
   assert(get(P(1, 1, W), 2).def.key == "__npn")
   assert(get(P(1, 1, S), 3).def.key == "__npn")
   assert(get(P(1, 1, E), 4).def.key == "__npn")
   assert(get(P(3, 1, N), 1).def.key == "__led")
   assert(get(P(3, 1, W), 2).def.key == "__led")
   assert(get(P(3, 1, S), 3).def.key == "__led")
   assert(get(P(3, 1, E), 4).def.key == "__led")
end

do
   print("   - Compile")
   local msandbox = circuit_single_tile()
   
   local compiled = compile(msandbox)
   local connections = compiled.connections
   assert(#connections == 1)
   assert(#connections[1].pins == 2)
   assert(connections[1].wires:len() == 4)
   assert(#compiled.components == 2)
   
   assert(connections[1].wires:contains(P(1, 1, E)))
   assert(connections[1].wires:contains(P(2, 1, W)))
   assert(connections[1].wires:contains(P(2, 1, E)))
   assert(connections[1].wires:contains(P(3, 1, W)))
   
   assert(has_pin(connections[1], 4, "__npn"))
   assert(has_pin(connections[1], 2, "__led"))
end

do
   print("   - Multiple connections to the same component")
   local msandbox = circuit_single_tile()
   msandbox.board:add_wires(P(1, 0), P(1, 1), VERTICAL, WR("top", "1"))
   
   local connections = compile(msandbox).connections
   assert(#connections == 2)
end

do
   print("   - Single-tile component rotation")
   local msandbox = circuit_single_tile()
   msandbox.board:rotate_tile(P(1, 1))
   
   local connections = compile(msandbox).connections
   assert(has_pin(connections[1], 1, "__npn"))
   assert(has_pin(connections[1], 2, "__led"))
end

print(" - IC")

do
   print("   - Compile")
   local msandbox = circuit_ic(N)
   -- print(serpent.line(msandbox:take_snapshot("scene"), { comment=false, fatal=true }))
   
   local connections = compile(msandbox).connections
   -- print(serpent.line(compiler.snapshot(msandbox, connections), { comment=false, fatal=true }))
   assert(#connections == 1)
   assert(#connections[1].pins == 2)
   assert(connections[1].wires:len() == 2)
   
   assert(connections[1].wires:contains(P(1, 1, E)))
   assert(connections[1].wires:contains(P(2, 1, W)))
   
   assert(has_pin(connections[1], 4, "__npn"))
   assert(has_pin(connections[1], 1, "__or_2i"))
end

do
   print(" - Rotated IC")
   local msandbox = circuit_ic(S)
   
   local connections = compile(msandbox).connections
   assert(#connections == 1)
   assert(#connections[1].pins == 2)
   assert(connections[1].wires:len() == 2)
   
   assert(connections[1].wires:contains(P(1, 1, E)))
   assert(connections[1].wires:contains(P(2, 1, W)))
   
   assert(has_pin(connections[1], 4, "__npn"))
   assert(has_pin(connections[1], 3, "__or_2i"))
end
