print("Compilation")

local function circuit_single_tile()
   local sandbox = Sandbox.new()
   local board = sandbox:add_board(20, 10)
   board:add_component("__npn", P(1, 1), N)
   board:add_component("__led", P(3, 1), N)
   board:add_wires(P(1, 1), P(3, 1), HORIZONTAL, WR(LAYER_TOP, WIDTH_1))
   return sandbox
end

local function circuit_ic(dir)
   local sandbox = Sandbox.new()
   local board = sandbox:add_board(20, 10)
   board:add_component("__npn", P(1, 1), N)
   board:add_component("__or_2i", P(3, 1), dir)
   board:add_wires(P(1, 1), P(3, 1), HORIZONTAL, WR(LAYER_TOP, WIDTH_1))
   return sandbox
end

local function has_pin(connection, pin_no, key)
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
   local sandbox = circuit_single_tile()
   local pins = compiler.find_all_pins(sandbox)
   
   local function get(pos, pin_no)
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
   local sandbox = circuit_single_tile()
   
   local connections = compiler.compile(sandbox)
   assert(#connections == 1)
   assert(#connections[1].pins == 2)
   assert(#connections[1].wires == 4)
   
   assert(contains(connections[1].wires, P(1, 1, E)))
   assert(contains(connections[1].wires, P(2, 1, W)))
   assert(contains(connections[1].wires, P(2, 1, E)))
   assert(contains(connections[1].wires, P(3, 1, W)))
   
   assert(has_pin(connections[1], 4, "__npn"))
   assert(has_pin(connections[1], 2, "__led"))
end

do
   print("   - Multiple connections to the same component")
   local sandbox = circuit_single_tile()
   sandbox.boards[1]:add_wires(P(1, 0), P(1, 1), VERTICAL, WR(LAYER_TOP, WIDTH_1))
   
   local connections = compiler.compile(sandbox)
   assert(#connections == 2)
end

do
   print("   - Single-tile component rotation")
   local sandbox = circuit_single_tile()
   sandbox.boards[1]:rotate_tile(P(1, 1))
   
   local connections = compiler.compile(sandbox)
   assert(has_pin(connections[1], 1, "__npn"))
   assert(has_pin(connections[1], 2, "__led"))
end

print(" - IC")

do
   print("   - Compile")
   local sandbox = circuit_ic(N)
   
   local connections = compiler.compile(sandbox)
   print(serpent.line(compiler.snapshot(sandbox, connections), { comment=false, fatal=true }))
   assert(#connections == 1)
   assert(#connections[1].pins == 2)
   assert(#connections[1].wires == 2)
   
   assert(contains(connections[1].wires, P(1, 1, E)))
   assert(contains(connections[1].wires, P(2, 1, W)))
   
   assert(has_pin(connections[1], 4, "__npn"))
   assert(has_pin(connections[1], 1, "__or_2i"))
end

do
   print(" - Rotated IC")
   local sandbox = circuit_ic(S)
   
   local connections = compiler.compile(sandbox)
   assert(#connections == 1)
   assert(#connections[1].pins == 2)
   assert(#connections[1].wires == 2)
   
   assert(contains(connections[1].wires, P(1, 1, E)))
   assert(contains(connections[1].wires, P(2, 1, W)))
   
   assert(has_pin(connections[1], 4, "__npn"))
   assert(has_pin(connections[1], 3, "__or_2i"))
end
