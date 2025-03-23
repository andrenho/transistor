local type Render = require "ui.render"
local Sandbox = require "sandbox"

local record UI
   __index: UI
end
UI.__index = UI

function UI.new() : UI
    local self : UI = setmetatable({}, UI as metatable<UI>)
    return self
end

function UI:render(sandbox: Sandbox) : Render
   return {
      scenes = sandbox:render()
   }
end

return UI

-- vim:sw=3:st=3:sts=3:noexpandtab

