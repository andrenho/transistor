- [x] Major refactoring
  - [x] Rewrite lua engine
  - [x] Serialization
  - [x] Cursor
  - [x] Run tests from C
  - [x] Component def assertions
  - [x] Test simulation
    - [x] Create thread
    - [x] Send components to simulator
    - [x] Send pins to simulator
    - [x] Test simulation
  - [x] Reorganize directories
  - [x] Include and load components from C

- [ ] UI
  - [x] Basic setup
  - [x] Load resources
  - [x] Draw board + wires
    - [x] Events
  - [x] Wire cursor
    - [x] Place single wire
  - [x] Render components
    - [x] Graphics object
  - [x] Create simulation functions
  - [x] Error on delete
  - [x] Timing capture (FPS, steps, process timings)

- [ ] Thread refactoring
  - [ ] Lua state owned by simulation thread
  - [ ] Rendering done on Lua side, no more snapshot
  - Design:
    - Simulation thread
      - Owns Lua state
      - Accepts Lua requests, pauses simulation, execute it and returns it
    - Engine: higher level requests, calls simulation
  
- [ ] GUI
  - [ ] Demo window
  - [ ] Main menu
    - [ ] Thread policy
    - [ ] About
  - [ ] Save/load (serialize/unserialize)
    - [ ] Treat errors
    - [ ] Autosave/load on initialization
  - [ ] Toolbox
    - [x] Create toolbox (basic components)
    - [ ] Organize database per category/subcategory
    - [ ] Add additional components to toolbox
    - [ ] Component cursor (add opacity to Board:render_components in board.lua)
  - [ ] Cursors
    - [ ] Custom?
    - [ ] Deleting
  - [ ] Infobox (sidebar)
  - [ ] Exception management
  - [ ] Deal with extern "C"
  - [ ] Nomenclature: deserialize vs unserialize
  - [ ] Close version
    - [ ] Test for leaks
    - [ ] Test release
  
- [ ] ICs
  - [ ] Toolbox for ICs
  - [ ] OR IC 
  - [ ] Show pin names when hovering

- [ ] Prepare for distribution
 - [ ] Rebrand
 - [ ] Github README
 - [ ] Emscripten version
 

- [ ] Future implementation 1
  - [ ] Resize board
  - [ ] Selection, cut/copy/paste
  - [ ] Add text to board
  - [ ] Lua ICs
    - [ ] UI library
  - [ ] Miniatures
    - [ ] Saved circuits
    - [ ] ICs

- [ ] Future implementation 2
  - [ ] Lower layer wiring
  - [ ] Bus, bus adapter
  - [ ] Subcircuits

- [ ] Future implementation 3
  - [ ] Multiple boards
    - [ ] Bugfix: on the compilation, consider the board along with the position when creating the list of wires
  - [ ] Video
  - [ ] Audio
  - [ ] Inputs (keyboard, etc)

- [ ] Game
  - [ ] Title screen
  - [ ] Settings menu
  - [ ] Nand2Tetris levels