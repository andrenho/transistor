local type Context = require "ui.context"

local enum Command
    "draw"
    "push_context"
end

local interface Instruction
    command: Command
end

global record DrawInstruction
        is Instruction
        where self.command == "draw"
    image: string
    x: integer
    y: integer
    w: integer
    h: integer
    context: Context
end

global record PushContextInstruction
      is Instruction
      where self.command == "push_context"
   context: Context
end

return Instruction
