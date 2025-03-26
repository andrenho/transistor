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

function UI:menu(): {MenuItem}
   return {
	  { text = "Circuit", items = {
		 { 
			text = "Clear circuit",
			ask_confirmation = "Are you sure you want to clear the circuit?",
			callback = function(sandbox: Sandbox) sandbox.board:clear() end,
		 },
	  } },
   }
end

function UI:render(sandbox: Sandbox) : Render
   return {
      scenes = sandbox:render(),
	  menu = self:menu(),
   }
end

return UI

-- vim:sw=3:st=3:sts=3:noexpandtab

