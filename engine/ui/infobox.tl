require "sandbox"

global function infobox(sandbox: Sandbox) : string
   local def : ComponentDef = sandbox.component_db.items[sandbox.board.cursor.selected_component]
   return def and def.infobox or nil
end
