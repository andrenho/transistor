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
      { name = "TESTX", direction = "input" },
      { name = "IN1", direction = "input" },
      { name = "!Q",  direction = "output" },
      { name = "Q",   direction = "output" },
   },
   
   infobox = [[
      TODO
   ]],
   
   init = function()
      -- TODO - load icon
   end,
   
   render = function(_: Component | nil, scene: Scene, context: Context)
	  scene:ic_text("OR", context)
   end,
}

return or_2i

-- vim:sw=3:st=3:sts=3:noexpandtab
