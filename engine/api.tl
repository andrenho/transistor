local Sandbox = require 'sandbox'
local UI = require 'ui.ui'
local Render = require 'ui.render'
local type Event = require 'event'

global sandbox : Sandbox = nil
global ui : UI = nil

global function init()
   sandbox = Sandbox.new()
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

global function run_tests()
   require 'tests.positions'
   require 'tests.pinpositions'
   require 'tests.placement'
   require 'tests.array'
end

-- vim:sw=3:st=3:sts=3:noexpandtab
