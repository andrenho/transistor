# v0.0.5 - improvements + refactoring

- [x] Teal reimplementation

# v0.0.6 - IC support

- [x] optional ImGUI window in debug mode
- [x] ic support
  - [x] logic (write unit tests)
    - [x] create OR ic
    - [x] pin placement
      - [x] IC DIP
      - [x] IC DIP (configurable width)
      - [x] IC Quad
    - [x] don't allow placing on top of other components
    - [x] remove wires underneath
    - [x] don't allow rotation
    - [x] clear tile
    - [x] simulate
      - [x] rotate (before placement)
  - [x] Refactor toolbox
    - [x] Move tools to ComponentDatabase
    - [x] Make regex replacements in initialization (infobox.cc)
    - [x] Move components.cc to a data directory (organize as `data`)
  - [x] UI
    - [x] select from the toolbox
      - [x] create submenus (categories: logic gates, series 7400, memory, cpu)
    - [x] unify cursor rendering and component rendering
    - [x] draw temporary IC
    - [x] draw IC
      - [x] draw from text
      - [x] automatic shadows
      - [ ] draw from image
    - [x] rotation/placement
    - [x] limit IC placement in board borders
    - [ ] show pin names
    - [ ] moving board not working
- [x] ic Lua support
  - [x] serialize/deserialize

# v0.0.7 - UI improvements

- [x] improve error handling (ImGui)
- [ ] resize board
- [ ] selection
- [ ] cut/copy/paste
- [ ] add text to board
- [ ] show pin names when hovering

# v0.0.8 - advanced wiring

- [ ] lower layer wiring
- [ ] bus
- [ ] bus adapter

# v0.0.9 - custom boards

- [ ] video
- [ ] audio
- [ ] inputs

# v0.0.10 - subcircuits

- [ ] ?

# v0.0.11 - game

- [ ] title screen
- [ ] game level support

# v0.9.0 - game levels

- [ ] Nand2Tetris levels
