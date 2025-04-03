require "sandbox"
require "ui.dialog"
require "ui.render"

global record UI
   __index: UI
end
UI.__index = UI

function UI.new() : UI
    local self : UI = setmetatable({}, UI as metatable<UI>)
    return self
end

function UI:menu(): {MenuItem}
   return {
	  { 
		 text = "Circuit",
		 items = {
			{ 
			   text = "Clear circuit",
			   ask_confirmation = Dialog.question("Clear circuit", "Are you sure you want to clear the circuit?", { type = "clear_board" })
			},
		 }
	  },
   }
end

function UI:render(sandbox: Sandbox) : Render
   return {
      scenes = sandbox:render(),
	  menu = self:menu(),
   }
end

-- vim:sw=3:st=3:sts=3:noexpandtab
