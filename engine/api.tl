require "sandbox"
require "debug_"
require "ui.ui"
require "filemanager"
require "event"
require "compiler.compiler"

local sandbox : Sandbox
local file_manager : FileManager
local ui : UI

local last_render: UI.Render

local first_step = true

global function init(debug_?: boolean)
   print("-----------------------")
   DEBUG = debug_ or false
   file_manager = FileManager.new()
   sandbox = file_manager.sandbox
   ui = UI.new()
end

global function render(wires_values: {integer, integer}) : UI.Render
   last_render = ui:render(sandbox, wires_values)
   return last_render
end

local function dump_last_state()
   local filename = file_manager.directory .. "/state.lua"
   local f = assert(io.open(filename, "w"))
   f:write("return " .. serpent.block({
	  serialization = sandbox:serialize(),
	  compilation = compile(sandbox),
	  last_render = last_render,
   }, { comment=false, nocode=true, keyignore={ def=true }, valtypeignore={ userdata=true, ["function"]=true } }))
   f:close()
   print("State filed saved in " .. filename)
end

global function do_events(events: {Event}) : CompiledCircuit
   local recompile = false
   
   for _,event in ipairs(events) do
	  if sandbox:do_event(event) then
		 recompile = true
	  end
	  if event.type == "key_down" and event.key == "d" and event.mod.ctrl then
		 dump_last_state()
	  elseif event.type == "select_key" then
		 sandbox.board.cursor.selected_component = event.key
	  end
   end

   if first_step then
	  recompile = true
	  first_step = false
   end

   if recompile then
	  return compile(sandbox)
   end
   return nil
end

global function simulate()
   sandbox:simulate()
end

global function save_in_progress()
   file_manager:save_in_progress()
end

global function run_tests()
   require 'tests.positions'
   require 'tests.pinpositions'
   require 'tests.placement'
   require 'tests.connected_wires'
   require 'tests.compilation'
   require 'tests.array'
   require 'tests.serialization'
end

-- vim:sw=3:st=3:sts=3:noexpandtab
