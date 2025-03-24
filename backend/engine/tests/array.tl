print('Test array')

do
   print(' - Set and read')
   local a = Array.new(4)
   assert(#a == 4)
   a[2] = 24
   assert(a[1] == 0)
   assert(a[2] == 24)
end

-- vim:sw=3:st=3:sts=3:noexpandtab
