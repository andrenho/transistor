local type Callback = require "ui.callback"

global enum DialogType
   "information"
   "question"
   "warning"
   "error"
end

local record DialogButton
   text: string
   callback: Callback
end

local record Dialog
   title: string   -- needs to be unique
   text: string | {string}
   type: DialogType
   buttons: {DialogButton}
   default_button: integer
end

function Dialog.messagebox(title: string, text: string, type_?: DialogType) : Dialog
   return {
	  title = title,
	  text = text,
	  type = type_ or "information",
	  buttons = { { text = "Ok" } },
	  default_button = 1,
   }
end

function Dialog.question(title: string, text: string, callback: Callback, default_button?: integer) : Dialog
   return {
	  title = title,
	  text = text,
	  type = "question",
	  buttons = { 
		 { text = "Yes", callback = callback },
		 { text = "No" }
	  },
	  default_button = default_button or 2,
   }
end

return Dialog

-- vim:sw=3:st=3:sts=3:noexpandtab
