require "sandbox"
require "ui.dialog"
require "ui.menu"
require "ui.toolbox"

global record UI
   record Render
	   scenes: {Scene}
	   menu: {MenuItem}
	   toolbox: ToolBox
   end
selected_key: string

   __index: UI
end
UI.__index = UI

function UI.new() : UI
    local self : UI = setmetatable({}, UI as metatable<UI>)
	self.selected_key = ""
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

function UI:toolbox(component_db: ComponentDB) : ToolBox
   local toolbox : {ToolItem} = {
	  { image = "tb_arrow",  key = "",         tooltip = "Unselect (ESC)" },
	  { image = "tb_vcc",    key = "__vcc",    tooltip = "VCC [always `1`] (v)" },
	  { image = "tb_button", key = "__button", tooltip = "Input button (b)" },
	  { image = "tb_led",    key = "__led",    tooltip = "LED (l)" },
	  { image = "tb_npn",    key = "__npn",    tooltip = "NPN transistor [activate to open] (n)" },
	  { image = "tb_pnp",    key = "__pnp",    tooltip = "PNP transistor [activate to close] (p)" },
	  { separator = true },
	  { image = "tb_logic_gates", category = "logic_gates", tooltip = "Logic gates" },
	  { image = "tb_digital",     category = "digital",     tooltip = "Digital components" },
	  { image = "tb_memory",      category = "memory",      tooltip = "Memories" },
	  { image = "tb_cpu",         category = "cpu",         tooltip = "CPUs and microcontrollers" },
   }
   for _,t in ipairs(toolbox) do 
	  if t.category then
		 t.menus = component_db.categories.categories[t.category]
		 t.submenus = {}
		 for _,item in ipairs(t.menus) do
			t.submenus[item] = component_db.categories.subcategories[item]
		 end
	  end
   end
   return toolbox
end

function UI:render(sandbox: Sandbox, wires_values: {integer, integer}) : UI.Render
   return {
      scenes = sandbox:render(wires_values),
	  menu = self:menu(),
	  toolbox = self:toolbox(sandbox.component_db),
   }
end

-- vim:sw=3:st=3:sts=3:noexpandtab
