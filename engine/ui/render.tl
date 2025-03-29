local type Scene = require "ui.scene"
local type Dialog = require "ui.dialog"
local type Callback = require "ui.callback"

global type Sandbox

global record MenuItem
   text: string
   ask_confirmation: Dialog
   callback: Callback
   items: {MenuItem}
end

local record Render
    scenes: {Scene}
    menu: {MenuItem}
end

return Render

-- vim:sw=3:st=3:sts=3:noexpandtab
