RUNNING_FROM_C = false

function native_array(sz)
   local n = {}
   n.ptr = function() return 77777777 end
   return n
end