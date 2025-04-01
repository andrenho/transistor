local type Context = require "ui.context"

local tnew = require "table.new"

-- draw:        { "draw", image, x, y, w, h, context }
-- push_context { "push_context", context }
-- pop_context  { "pop_context" }
local type Instruction = { any }

local record Scene
   __index: Scene
   instructions: {Instruction}
end
Scene.__index = Scene

function Scene.new() : Scene
    local self : Scene = setmetatable({}, Scene as metatable<Scene>)
	self.instructions = tnew(800, 0) as {Instruction}
    return self
end

function Scene:add_image(image: string, x: integer, y: integer, context?: Context)
   self.instructions[#self.instructions+1] = { "draw", image, x, y, 0, 0, context }
end

function Scene:push_context(context: Context)
   self.instructions[#self.instructions+1] = { "push_context", context }
end

function Scene:pop_context()
   self.instructions[#self.instructions+1] = { "pop_context" }
end

return Scene

-- vim:sw=3:st=3:sts=3:noexpandtab

