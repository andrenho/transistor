require "components.componentdef"
require "components.category"

global record ComponentDB
   items: {string: ComponentDef}
   categories: Categories
    __index: ComponentDB
end
ComponentDB.__index = ComponentDB

global record SerialComponentDB
end

function ComponentDB:update_categories()
   self.categories = { 
	  categories = { logic_gates={}, digital={}, memory={}, cpu={} },
	  subcategories = {}
   }
   for key,def in pairs(self.items) do
	  if def.category then
		 table.insert(self.categories.categories[def.category], def.subcategory)
		 if not self.categories.subcategories[def.subcategory] then self.categories.subcategories[def.subcategory] = {} end
		 table.insert(self.categories.subcategories[def.subcategory], { name = def.name, key = key })
	  end
   end
   for _,subcategories in pairs(self.categories.categories) do table.sort(subcategories) end
   for _,def in pairs(self.categories.subcategories) do
	  table.sort(def, function(a: Categories.Def, b: Categories.Def) : boolean return a.name < b.name end)
   end
end

function ComponentDB:add(def: ComponentDef, native: boolean, lua_code?: string)
   ComponentDef.validate(def)
   setmetatable(def, ComponentDef as metatable<ComponentDef>)
   def.code = lua_code
   def.native = native
   self.items[def.key] = def
   self:update_categories()
end

function ComponentDB:add_from_str(lua_code: string, native: boolean)
   local def = load(lua_code, "component_loader")() as ComponentDef
   self:add(def, native, lua_code)
end

function ComponentDB:load_all_native_components()  -- this function is just for testing and is overwritten by C engine
   local native_components = {
	  'basic.vcc',
	  'basic.led',
	  'basic.npn',
	  'basic.pnp',
	  'basic.button',
	  'gates.or_2i',
   }
   for _,c in ipairs(native_components) do
	  self:add(require("components." .. c) as ComponentDef, true)
   end
end

function ComponentDB.new() : ComponentDB
   local self : ComponentDB = setmetatable({}, ComponentDB as metatable<ComponentDB>)
   self.items = {}
   self:load_all_native_components()
   return self
end

function ComponentDB:def(key: string) : ComponentDef
   local item = self.items[key]
   if not item then error("Component " .. key .. " not found", 2) end
   return item
end

function ComponentDB:serialize() : SerialComponentDB
   return {}
end

function ComponentDB.unserialize(_: SerialComponentDB) : ComponentDB
   return ComponentDB.new()
end

-- vim:sw=3:st=3:sts=3:noexpandtab
