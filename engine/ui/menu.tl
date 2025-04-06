require "ui.dialog"
require "event"

global record MenuItem
   text: string
   ask_confirmation: Dialog
   event: Event
   items: {MenuItem}
end

-- vim:sw=3:st=3:sts=3:noexpandtab
