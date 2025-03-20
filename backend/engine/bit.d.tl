local record bit
    tobit: function(x: integer): integer
    tohex: function(x: integer, n?: integer): string
    bnot: function(x: integer): integer
    bor: function(...: integer): integer
    band: function(...: integer): integer
    bxor: function(...: integer): integer
    lshift: function(x: integer, n: integer): integer
    rshift: function(x: integer, n: integer): integer
    arshift: function(x: integer, n: integer): integer
    rol: function(x: integer, n: integer): integer
    ror: function(x: integer, n: integer): integer
    bswap: function(x: integer): integer
end

return bit
