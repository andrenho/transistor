local type Context = require "ui.context"

local enum Command
    "draw"
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

return Instruction
