local ComponentDef = require("components.componentdef")
local Component = require("components.component")
local Graphics = require("graphics.graphics")
local Context = require("graphics.context")

local or_2i : ComponentDef = {
   key = "__or_2i",
   type = "ic_dip",
   can_rotate = true,
   
   name = "OR (2 inputs)",
   category = "logic_gates",
   subcategory = "OR gates",
   
   pins = {
      { name = "IN0", direction = "input" },
      { name = "IN1", direction = "input" },
      { name = "~Q",  direction = "output" },
      { name = "Q",   direction = "output" },
   },
   
   infobox = [[
      TODO
   ]],
   
   init = function()
      -- TODO - load icon
   end,
   
   render = function(_: Component, _: Graphics, _: integer, _: integer, _: Context)
      -- TODO
   end,
}

return or_2i
