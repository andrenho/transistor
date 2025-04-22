require "ui.context"

-- draw:        { "draw", image, x, y, w, h, context }
-- text:        { "text", font, font_sz, text, x, y, w, h, r, g, b, context }
-- push_context { "push_context", context }
-- pop_context  { "pop_context" }
local type Instruction = { any }

local tnew = require "table.new"

global record Scene
   __index: Scene
   instructions: {Instruction}
end
Scene.__index = Scene

global record SceneText
   record Color
	  r: integer
	  g: integer
	  b: integer
   end
   font: string | ResourceId
   font_sz: integer
   text: string
   x: integer
   y: integer
   w: integer
   h: integer
   color: Color
   context: Context
end

local IC_TEXT_DEFAULT_COLOR : SceneText.Color = { r = 0xb0, g = 0xb0, b = 0xb0 }
local IC_TEXT_SHADOW_COLOR : SceneText.Color = { r = 0x20, g = 0x20, b = 0x20 }

function Scene.new() : Scene
    local self : Scene = setmetatable({}, Scene as metatable<Scene>)
	self.instructions = tnew(800, 0) as {Instruction}
    return self
end

function Scene:add_image(image: string | ResourceId, x: integer, y: integer, context?: Context)
   self.instructions[#self.instructions+1] = { "draw", image, x, y, 0, 0, context }
end

function Scene:add_text(text: SceneText)
   self.instructions[#self.instructions+1] = {
	  "text", assert(text.font), assert(text.font_sz), assert(text.text),
	  assert(text.x), assert(text.y), text.w or 0, text.h or 0,
	  text.color.r, text.color.g, text.color.b,
	  text.context
   }
end

function Scene:ic_text(text: string, context?: Context)
   if context.rotation >= 180 then
	  context.rotation = context.rotation - 180
   end
   self:add_text({
	  font = "ic_font",
	  font_sz = 16,
	  text = text,
	  x = 1,
	  y = 1,
	  color = IC_TEXT_SHADOW_COLOR,
	  context = context,
   })
   self:add_text({
	  font = "ic_font",
	  font_sz = 16,
	  text = text,
	  x = 0,
	  y = 0,
	  color = IC_TEXT_DEFAULT_COLOR,
	  context = context,
   })
end

function Scene:push_context(context: Context)
   self.instructions[#self.instructions+1] = { "push_context", context }
end

function Scene:pop_context()
   self.instructions[#self.instructions+1] = { "pop_context" }
end

-- vim:sw=3:st=3:sts=3:noexpandtab

