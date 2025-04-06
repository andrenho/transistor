require "sandbox"

global record FileManager
   sandbox: Sandbox
   filename: string
   directory: string
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
	   self.directory = os.getenv("HOME") .. "/.config/transistor"
	   os.execute("mkdir -p " .. self.directory)
	else  -- windows
	   self.directory = os.getenv("HOME") .. "/transistor"
	   os.execute("md " .. self.directory)
	end
	self.in_progress_filename = self.directory .. "/in_progress.trn"
	self.sandbox = self:load_in_progress()
	if not self.sandbox then
	   self.sandbox = Sandbox.new()
	else
	   print("Loaded from previously in progress")
	end
    return self
end

function FileManager:save(filename: string)
   local f = assert(io.open(filename, "w"))
   f:write("return " .. serpent.line(self.sandbox:serialize(), { comment=false, compact=true }))
   f:close()
end

function FileManager:save_in_progress()
   self:save(self.in_progress_filename)
end

-- vim:sw=3:st=3:sts=3:noexpandtab
