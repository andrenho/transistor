global enum PopupToolCategory
	"logic_gates"
	"digital"
	"memory"
	"cpu"
end

global record ToolItem
	image: string
	key: string
	tooltip: string
	category: PopupToolCategory
	selected: boolean
	separator: boolean

	-- TODO - category
end

global type ToolBox = {ToolItem}

-- vim:sw=3:st=3:sts=3:noexpandtab
