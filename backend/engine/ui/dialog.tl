global enum DialogType
   "information"
   "question"
   "warning"
   "error"
end

local record Dialog
   title: string   -- needs to be unique
   text: string | {string}
   type: DialogType
   buttons: {string}
   default_button: integer
end

function Dialog.messagebox(title: string, text: string, type_?: DialogType) : Dialog
   return {
	  title = title,
	  text = text,
	  type = type_ or "information",
	  buttons = { "Ok" },
	  default_button = 1,
   }
end

function Dialog.question(title: string, text: string, default_button?: integer) : Dialog
   return {
	  title = title,
	  text = text,
	  type = "question",
	  buttons = { "Yes", "No" },
	  default_button = default_button or 2,
   }
end

return Dialog

-- vim:sw=3:st=3:sts=3:noexpandtab
