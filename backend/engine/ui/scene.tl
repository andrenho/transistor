local type Context = require "ui.context"

local enum Command
    "draw"
    "push_context"
    "pop_context"
end

local interface Instruction
    command: Command
end

local record DrawInstruction
        is Instruction
        where self.command == "draw"
    image: string
    x: integer
    y: integer
    w: integer
    h: integer
    context: Context
end

local record PushContextInstruction
      is Instruction
      where self.command == "push_context"
   context: Context
end

local record Scene
	  is {Instruction}
   __index: Scene
end
Scene.__index = Scene

function Scene.new() : Scene
    local self : Scene = setmetatable({}, Scene as metatable<Scene>)
    return self
end

function Scene:add_image(image: string, x: integer, y: integer, context?: Context)
   self[#self+1] = { command = "draw", image=image, x=x, y=y, context=context } as DrawInstruction
end

function Scene:push_context(context: Context)
   self[#self+1] = { command = "push_context", context=context } as PushContextInstruction
end

function Scene:pop_context()
   self[#self+1] = { command = "pop_context" } as Instruction
end

return Scene

-- vim:sw=3:st=3:sts=3:noexpandtab

