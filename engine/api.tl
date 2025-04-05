require "sandbox"
require "ui.ui"
require "filemanager"
require "ui.render"
require "event"
require "compiler.compiler"

global sandbox : Sandbox
global file_manager : FileManager
global ui : UI

local first_step = true

global function init()
   print("-----------------------")
   file_manager = FileManager.new()
   sandbox = file_manager.sandbox
   ui = UI.new()
end

global function render(wires_values: {integer, integer}) : Render
   -- print(serpent.line(wires_values))
   return ui:render(sandbox, wires_values)
end

global function do_events(events: {Event}) : CompiledCircuit
   local recompile = false
   
   for _,event in ipairs(events) do
	  if sandbox:do_event(event) then
		 recompile = true
	  end
	  if event.type == "key_down" and event.key == "d" then file_manager:dump() end
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
