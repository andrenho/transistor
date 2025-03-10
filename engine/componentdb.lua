ComponentDB = {}
ComponentDB.__index = ComponentDB

function ComponentDB.new()
   local self = setmetatable({}, ComponentDB)
   self.items = {}
   self:load_all_native_components()
   return self
end

function ComponentDB:add_from_str(lua_code)
   local c = load(lua_code, "component_loader")()
   ComponentDef.validate(c)
   setmetatable(c, ComponentDef)
   self.items[c.key] = c
end

function ComponentDB:load_all_native_components()  -- this function is just for testing and is overwritten by C engine
   function load_component(filename)
      local f = io.open(filename, "r")
      local content = f:read("*a")
      f:close()
      self:add_from_str(content)
   end
   load_component("components/basic/button.lua")
   load_component("components/basic/led.lua")
   load_component("components/basic/npn.lua")
   load_component("components/basic/pnp.lua")
   load_component("components/basic/vcc.lua")
   load_component("components/gates/or_2i.lua")
end
