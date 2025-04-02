local Sandbox = require 'sandbox'
local UI = require 'ui.ui'
local FileManager = require 'filemanager'
local Render = require 'ui.render'
local type Event = require 'event'

local sandbox : Sandbox = nil
local file_manager : FileManager = nil
local ui : UI = nil

global function init()
   sandbox = Sandbox.new()
   file_manager = FileManager.new(sandbox)
   ui = UI.new()
end

global function render() : Render
   local r = ui:render(sandbox)
   return r
end

global function do_events(events: {Event})
   for _,event in ipairs(events) do
	  sandbox:do_event(event)
   end
end

global function save_in_progress()
   file_manager:save_in_progress()
end

global function run_tests()
   require 'tests.positions'
   require 'tests.pinpositions'
   require 'tests.placement'
   require 'tests.array'
   require 'tests.serialization'
end

-- vim:sw=3:st=3:sts=3:noexpandtab
