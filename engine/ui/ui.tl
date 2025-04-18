require "sandbox"
require "ui.dialog"
require "ui.infobox"
require "ui.menu"
require "ui.toolbox"

global record UI
   record Render
	   scenes: {Scene}
	   menu: {MenuItem}
	   toolbox: ToolBox
	   infobox: string
   end

   __index: UI
end
UI.__index = UI

function UI.new() : UI
    local self : UI = setmetatable({}, UI as metatable<UI>)
    return self
end

function UI:render(sandbox: Sandbox, wires_values: {integer, integer}) : UI.Render
   return {
      scenes = sandbox:render(wires_values),
	  menu = main_menu(),
	  toolbox = toolbox(sandbox),
	  infobox = infobox(sandbox),
   }
end

-- vim:sw=3:st=3:sts=3:noexpandtab
