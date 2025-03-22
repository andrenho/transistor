local type Scene = require "ui.scene"
local Sandbox = require "sandbox"

local record UI
   __index: UI
end
UI.__index = UI

function UI.new() : UI
    local self : UI = setmetatable({}, UI as metatable<UI>)
    return self
end

function UI:render(sandbox: Sandbox) : {Scene}
   return {}
end

return UI

-- vim:sw=3:st=3:sts=3:noexpandtab

