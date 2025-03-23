local Sandbox = require 'sandbox'
local UI = require 'ui.ui'
local Render = require 'ui.render'

global sandbox : Sandbox = nil
global ui : UI = nil

global function init()
   sandbox = Sandbox.new()
   sandbox:add_board(20, 10)
   ui = UI.new()
end

global function render() : Render
   return ui:render(sandbox)
end

global function run_tests()
   require 'tests.positions'
   require 'tests.pinpositions'
   require 'tests.placement'
end

-- vim:sw=3:st=3:sts=3:noexpandtab
