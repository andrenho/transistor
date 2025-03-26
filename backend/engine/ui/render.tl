local type Scene = require "ui.scene"
local type Dialog = require "ui.dialog"

global type Sandbox

global record MenuItem
   text: string
   ask_confirmation: Dialog
   callback: function(Sandbox)
   items: {MenuItem}
end

local record Render
    scenes: {Scene}
    menu: {MenuItem}
end

return Render
