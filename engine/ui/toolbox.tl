require "components.category"
require "sandbox"

global record ToolItem
	image: string
	key: string
	tooltip: string
	category: ComponentCategory
	menus: {string}
	submenus: {string: {Categories.Def}}
	selected: boolean
	separator: boolean

	-- TODO - category
end

global type ToolBox = {ToolItem}

global function toolbox(sandbox: Sandbox) : ToolBox
   local tbox : {ToolItem} = {
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
   for _,t in ipairs(tbox) do 
	  if t.key == sandbox.board.cursor.selected_component then t.selected = true end
	  if t.category then
		 t.menus = sandbox.component_db.categories.categories[t.category]
		 t.submenus = {}
		 for _,item in ipairs(t.menus) do
			t.submenus[item] = sandbox.component_db.categories.subcategories[item]
		 end
	  end
   end
   return tbox
end

-- vim:sw=3:st=3:sts=3:noexpandtab
