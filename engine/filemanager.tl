require "sandbox"

global record FileManager
   sandbox: Sandbox
   filename: string
   in_progress_filename: string
   __index: FileManager
end
FileManager.__index = FileManager

function FileManager:load(filename: string) : Sandbox
   local ok, data = pcall(dofile, filename)
   if ok then
	  return Sandbox.unserialize(data as SerialSandbox)
   else
	  return nil
   end
end

function FileManager:load_in_progress() : Sandbox
   return self:load(self.in_progress_filename)
end

function FileManager.new() : FileManager
    local self : FileManager = setmetatable({}, FileManager as metatable<FileManager>)
	if package.config:sub(1,1) == "/" then  -- unix
	   os.execute("mkdir -p ~/.config/transistor")
	   self.in_progress_filename = os.getenv("HOME") .. "/.config/transistor/in_progress.trn"
	else  -- windows
	   os.execute("md $HOME/transistor")  -- untested
	   self.in_progress_filename = os.getenv("HOME") .. "/transistor/in_progress.trn"
	end
	self.sandbox = self:load_in_progress()
	if not self.sandbox then
	   self.sandbox = Sandbox.new()
	else
	   print("Loaded from previously in progress")
	end
    return self
end

local function serialize(data: any, f: FILE)
   if type(data) == "number" then
	  f:write(data as number)
   elseif type(data) == "string" then
	  f:write(string.format("%q", data as string))
   elseif type(data) == "table" then
	  f:write("{")
	  if (data as table)[1] then
		 for i,v in ipairs(data as {any}) do
			serialize(v, f)
			if i ~= #(data as {any}) then f:write(",") end
		 end
	  else
		 local found = false
		 for k,v in pairs(data as {any:any}) do
			f:write(k as string,"=")
			serialize(v, f)
			f:write(",")
			found = true
		 end
		 if found then f:seek("set", f:seek() - 1) end -- remove last comma
	  end
	  f:write("}")
   end
end

function FileManager:save(filename: string)
   local f = assert(io.open(filename, "w"))
   f:write("return ")
   serialize(self.sandbox:serialize(), f)
   f:close()
end

function FileManager:save_in_progress()
   self:save(self.in_progress_filename)
end

function FileManager:dump()
   serialize(self.sandbox:serialize(), io.stdout)
   io.stdout:write("\n")
end

-- vim:sw=3:st=3:sts=3:noexpandtab

