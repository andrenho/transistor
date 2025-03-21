local Sandbox = require 'sandbox'

sandbox = nil

function init()
   sandbox = Sandbox.new()
end

function run_tests()
   require 'tests.positions'
   require 'tests.pinpositions'
end
