require 'board.geo.position'

print("Positions")

local function list_contains(haystack: {Position}, needle: Position) : boolean
   for _,i in ipairs(haystack) do
      if i == needle then return true end
   end
   return false
end

do
   print(" - Position equality")
   print(P(1, 1) == P(1, 1))
   assert(P(1, 1) == P(1, 1))
   assert(P(1, 1) ~= P(1, 1, N))
end

do
   print(" - Hash and unhash")
   local p = P(194, 237, W)
   assert(Position.unhash(p:hash()) == p)
end

do
   print(" - A to B: horizontal")
   local pos = Position.a_to_b(P(1, 1), P(2, 3), HORIZONTAL)
   assert(#pos == 6)
   assert(list_contains(pos, P(1, 1, E)))
   assert(list_contains(pos, P(2, 1, W)))
   assert(list_contains(pos, P(2, 1, S)))
   assert(list_contains(pos, P(2, 2, N)))
   assert(list_contains(pos, P(2, 2, S)))
   assert(list_contains(pos, P(2, 3, N)))
end

do
   print(" - A to B: vertical")
   local pos = Position.a_to_b(P(1, 1), P(2, 3), VERTICAL)
   assert(#pos == 6)
   assert(list_contains(pos, P(1, 1, S)))
   assert(list_contains(pos, P(1, 2, N)))
   assert(list_contains(pos, P(1, 2, S)))
   assert(list_contains(pos, P(1, 3, N)))
   assert(list_contains(pos, P(1, 3, E)))
   assert(list_contains(pos, P(2, 3, W)))
end

-- vim:sw=3:st=3:sts=3:noexpandtab
