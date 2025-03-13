ComponentDB = {}
ComponentDB.__index = ComponentDB

function ComponentDB.new()
   local self = setmetatable({}, ComponentDB)
   self.items = {}
   self:load_all_native_components()
   return self
end

function ComponentDB:add(def, native, lua_code)
   ComponentDef.validate(def)
   setmetatable(def, ComponentDef)
   def.code = lua_code
   def.native = native
   self.items[def.key] = def
end

function ComponentDB:add_from_str(lua_code, native)
   local def = load(lua_code, "component_loader")()
   self:add(def, native, lua_code)
end

function ComponentDB:load_all_native_components()  -- this function is just for testing and is overwritten by C engine
   assert(not RUNNING_FROM_C)
   local function load_component(filename)
      local f = io.open(filename, "r")
      local content = f:read("*a")
      f:close()
      self:add_from_str(content, true)
   end
   load_component("engine/components/basic/button.lua")
   load_component("engine/components/basic/led.lua")
   load_component("engine/components/basic/npn.lua")
   load_component("engine/components/basic/pnp.lua")
   load_component("engine/components/basic/vcc.lua")
   load_component("engine/components/gates/or_2i.lua")
end

function ComponentDB:def(key)
   return self.items[key]
end

function ComponentDB:take_snapshot()
   local defs = {}
   for _,def in pairs(self.items) do
      if not def.native then
         defs[#defs+1] = def.code
      end
   end
   return defs
end

function ComponentDB:import_snapshot(snap)
   for _,code in ipairs(snap) do
      self:add_from_str(code, false)
   end
end