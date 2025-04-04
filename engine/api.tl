require 'sandbox'
require 'ui.ui'
require 'filemanager'
require 'ui.render'
require 'event'

global sandbox : Sandbox = nil
global file_manager : FileManager = nil
global ui : UI = nil

global function init()
   print("-----------------------")
   file_manager = FileManager.new()
   sandbox = file_manager.sandbox
   ui = UI.new()
   print(sandbox.board)
end

global function render() : Render
   local r = ui:render(sandbox)
   return r
end

global function do_events(events: {Event})
   for _,event in ipairs(events) do
	  sandbox:do_event(event)
	  if event.type == "key_down" and event.key == "d" then file_manager:dump() end
   end
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
