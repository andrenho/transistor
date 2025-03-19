local Sandbox = require 'sandbox'

sandbox = nil

function init()
    sandbox = Sandbox.new()
    print(sandbox)
end