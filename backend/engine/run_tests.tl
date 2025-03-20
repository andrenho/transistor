#!/usr/bin/env -S tl run

local Sandbox = require('sandbox')
local Board = require('board.board')

local sandbox = Sandbox.new()
local board_id = sandbox:add_device("board", 20, 10)

local board = sandbox:get_device(board_id) as Board
print(board)
