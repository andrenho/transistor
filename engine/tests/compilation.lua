print("Compilation")

local function circuit()
   local sandbox = Sandbox.new()
   local board = sandbox:add_board(20, 10)
   board:add_component("__button", P(1, 1), N)
   board:add_component("__led", P(3, 1), N)
   board:add_wires(P(1, 1), P(3, 1), HORIZONTAL, { WIDTH_1, LAYER_TOP })
   return sandbox
end

print(" - Basic circuit")

do
   print("   - Find all pins")
   local sandbox = circuit()
   local pins = compiler.find_all_pins(sandbox)
   
   local function get(pos, pin_no)
      for _,pin in ipairs(pins) do
         if pin.pos == pos and pin.pin_no == pin_no then return pin.component end
      end
      return nil
   end
   
   assert(#pins == 8)
   assert(get(P(1, 1, N), 1).def.key == "__button")
   assert(get(P(1, 1, W), 2).def.key == "__button")
   assert(get(P(1, 1, S), 3).def.key == "__button")
   assert(get(P(1, 1, E), 4).def.key == "__button")
   assert(get(P(3, 1, N), 1).def.key == "__led")
   assert(get(P(3, 1, W), 2).def.key == "__led")
   assert(get(P(3, 1, S), 3).def.key == "__led")
   assert(get(P(3, 1, E), 4).def.key == "__led")
end

do
   print("   - Compile")
end

do
   print("   - Multiple connections to the same component")
end

do
   print("   - Single-tile component rotation")
end

print(" - IC")

do
   print("   - Compile")
end

do
   print(" - Rotated IC")
end
