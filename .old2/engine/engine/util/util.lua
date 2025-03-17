function contains(array, item)
   assert(type(array) == 'table')
   for _,v in pairs(array) do if v == item then return true end end
   return false
end