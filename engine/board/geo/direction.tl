global enum Direction
   "N"
   "E"
   "S"
   "W"
   "CENTER"
end

global N : Direction = "N"
global E : Direction = "E"
global S : Direction = "S"
global W : Direction = "W"
global CENTER : Direction = "CENTER"

global Dir = {

   hash: {Direction: integer} = {
      [CENTER] = 1,
      [N] = 2,
      [E] = 3,
      [S] = 4,
      [W] = 5,
   },

   unhash: {integer: Direction} = {
      [1] = CENTER,
      [2] = N,
      [3] = E,
      [4] = S,
      [5] = W,
   },

   rotation: {Direction: Direction} = {
      [CENTER] = CENTER,
      [N] = E,
      [E] = S,
      [S] = W,
      [W] = N,
   },

   angle: {Direction: number} = {
      [CENTER] = 0,
      [N] = 0,
      [E] = 90,
      [S] = 180,
      [W] = 270,
   },

   direction_in_tile = function(x: number, y: number) : Direction
	  x = x - math.floor(x)
	  y = y - math.floor(y)

	  local s = 1.0 - y
	  local n = y
	  local e = 1.0 - x
	  local w = x

	  if n <= s and n <= e and n <= w then return N end
	  if s <= n and s <= e and s <= w then return S end
	  if e <= s and n <= n and e <= w then return E end
	  return W
   end
}

-- vim:sw=3:st=3:sts=3:noexpandtab
