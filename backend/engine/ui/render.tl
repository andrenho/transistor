local type Scene = require "ui.scene"

global type Sandbox

global record MenuItem
   text: string
   ask_confirmation: string
   f: function(Sandbox)
   items: {MenuItem}
end

local record Render
    scenes: {Scene}
    menu: {MenuItem}
end

return Render
