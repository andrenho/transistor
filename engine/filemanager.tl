local Sandbox = require "sandbox"

local record FileManager
   sandbox: Sandbox
   filename: string
   __index: FileManager
end
FileManager.__index = FileManager

function FileManager.new(sandbox: Sandbox) : FileManager
    local self : FileManager = setmetatable({}, FileManager as metatable<FileManager>)
	self.sandbox = sandbox
	self.filename = nil
    return self
end

function FileManager:save(filename?: string)
   -- TODO
end

function FileManager:load(filename: string) : Sandbox
   -- TODO
end

return FileManager

-- vim:sw=3:st=3:sts=3:noexpandtab

