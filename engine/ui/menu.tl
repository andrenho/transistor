require "ui.dialog"
require "event"
require "debug_"

global record MenuItem
   text: string
   ask_confirmation: Dialog
   event: Event
   items: {MenuItem}
end

global function main_menu() : {MenuItem}
   local menu = {
	  { 
		 text = "Circuit",
		 items = {
			{ 
			   text = "Clear circuit",
			   ask_confirmation = Dialog.question("Clear circuit", "Are you sure you want to clear the circuit?", { type = "clear_board" })
			},
		 }
	  },
   }
   if DEBUG then
	  table.insert(menu, {
		 text = "Debug",
		 items = {
			{ text = "ImGUI demo window", event = { type = "show_imgui_demo" } },
		 }
	  })
   end
   return menu
end

-- vim:sw=3:st=3:sts=3:noexpandtab
