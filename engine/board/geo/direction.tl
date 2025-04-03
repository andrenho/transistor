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
    }
}

-- vim:sw=3:st=3:sts=3:noexpandtab
