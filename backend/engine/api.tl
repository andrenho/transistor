local Sandbox = require 'sandbox'

global sandbox : Sandbox = nil

global function init()
   sandbox = Sandbox.new()
end

global function run_tests()
   require 'tests.positions'
   require 'tests.pinpositions'
   require 'tests.placement'
end

-- vim:sw=3:st=3:sts=3:noexpandtab
