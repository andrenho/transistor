require "components.category"

global record ToolItem
	image: string
	key: string
	tooltip: string
	category: ComponentCategory
	menu: {string}
	submenu: {string: {Categories.Def}}
	selected: boolean
	separator: boolean

	-- TODO - category
end

global type ToolBox = {ToolItem}

-- vim:sw=3:st=3:sts=3:noexpandtab
