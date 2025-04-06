global enum ComponentCategory
	"basic"
	"logic_gates"
	"digital"
	"memory"
	"cpu"
end

global record Categories
	record Def
		name: string
		key: string
	end

	categories: {ComponentCategory: {string}}
	subcategories: {string: {Def}}
end

-- vim:sw=3:st=3:sts=3:noexpandtab
