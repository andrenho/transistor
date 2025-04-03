require "ui.scene"
require "ui.context"
require "components.componentdef"
require "components.component"

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
   
   render = function(component: Component, scene: Scene, context: Context)
      -- TODO
   end,
}

return or_2i

-- vim:sw=3:st=3:sts=3:noexpandtab
