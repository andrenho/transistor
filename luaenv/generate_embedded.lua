function scandir(directory)
   local i, t, popen = 0, {}, io.popen
   local pfile = popen('find "'..directory..'" -name *.tl | sort')
   for filename in pfile:lines() do
      i = i + 1
      t[i] = filename
   end
   pfile:close()
   return t
end

local files = scandir("engine")

print([[#ifndef EMBEDDED_HH
#define EMBEDDED_HH

#include <unordered_map>

struct Bytecode {
    uint8_t const* data;
    size_t sz;
};
]])

-- includes
for _,file in ipairs(files) do
   print('#include "' .. file:gsub("%.tl", ".lua.h") .. '"')
end
print()

print([[
static std::unordered_map<std::string, Bytecode> embedded_bytecode = {
#define LOAD(name) { #name, { engine_##name##_lua, engine_##name##_lua_sz } }]])

-- loads
for _,file in ipairs(files) do
   print("    LOAD(" .. file:gsub("engine/", ""):gsub("%.tl", ""):gsub("/", "_"):gsub("%.", "_") .. "),")
end

print([[#undef LOAD
};

#endif //EMBEDDED_HH
]])
