require "ui.scene"
require "ui.dialog"
require "event"

global type Sandbox

global record MenuItem
   text: string
   ask_confirmation: Dialog
   event: Event
   items: {MenuItem}
end

global record Render
    scenes: {Scene}
    menu: {MenuItem}
end

-- vim:sw=3:st=3:sts=3:noexpandtab
