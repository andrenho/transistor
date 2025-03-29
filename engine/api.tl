local Sandbox = require 'sandbox'
local UI = require 'ui.ui'
local Render = require 'ui.render'

global sandbox : Sandbox = nil
global ui : UI = nil

global function init()
   sandbox = Sandbox.new()
   sandbox.board:add_wires(P(1, 1), P(5, 3), HORIZONTAL, WR("top", "1"))
   sandbox.board:add_component(sandbox.component_db:def("__button"), P(1, 1), N)
   ui = UI.new()
end

global function board_clear()
   sandbox.board:clear()
end

global function render() : Render
   return ui:render(sandbox)
end

global function run_tests()
   require 'tests.positions'
   require 'tests.pinpositions'
   require 'tests.placement'
   require 'tests.array'
end

-- vim:sw=3:st=3:sts=3:noexpandtab
