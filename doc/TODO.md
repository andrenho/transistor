# v0.0.1 - base engine

- [x] base engine
- [x] unit tests

# v0.0.2 - simple circuit editing

- [x] UI setup
- [x] draw board
- [x] GUI events
- [x] move board
- [x] add wires
- [x] components
  - [x] images
  - [x] add
  - [x] manipulate
- [x] delete wires/component
- [x] refactoring
 - [x] move circuit png to UI
 - [x] ImageContext (?)
 - [x] richer RenderContext, incorporate Scene into it
 - [x] create board id
 - [x] create unique position (including board)
 - [x] migrate all simulation parameters from x,y to Position
 - [x] use unique position to identify wire value
- [x] simulation
  - [x] query wire value from editor
  - [x] wires to north seem not to be working
  - [x] circuit is not turning off when button is depressed
- [x] VCC
- [x] bugs
  - [x] energy not travelling south
- [x] PNP, NPN
  - [x] rotation

# v0.0.3 - basic UI

- [x] theme for ImGUI
- [x] main menu
- [x] serialization / deserialization
  - [x] serialization
  - [x] deserialization
  - [x] check version
  - [x] save/load
- [x] infobox (sidebar?)
- [x] exceptions

# v0.0.4 - Rearchitect

- Architecture:
  - read from const object
  - make changes through queues
- [x] Engine
  - [x] Fix test errors
  - [x] Remove editor
  - [x] Make Game and UI globals
  - [x] Improve exception report with message queues
- [x] Remove events.hh
- [x] UI state
- [x] Improve error (don't quit sometimes)

# v0.0.5 - improvements + refactoring

- [x] advanced infobox
- [x] global res()
- [x] toolbar
  - [x] detach resource manager from UI?
  - [x] basic components
  - [x] selected component
    - [x] shadow draw
    - [x] component shortcuts
    - [x] place component
    - [x] rotate cursor
    - [x] don't place over existing component, overwrite wires
  - [x] component infobox
    - [x] create a selected tool dataset
    - [x] clear confusion between NPN and PNP
- [x] simplify single-tile component
  - [x] Join cursor render and render
  - [x] Return list of Pins
  - [x] Validate component

# v0.0.6 - IC support

- [ ] ic support
  - [ ] logic (write unit tests)
    - [ ] create OR ic
    - [ ] pin placement
    - [ ] don't allow placing on top of other components, remove wires underneath
    - [ ] rotate
  - [ ] UI
    - [ ] select from the toolbox
    - [ ] rotation/placement
    - [ ] show pin names
- [ ] ic Lua support
  - [ ] serialize/deserialize

# v0.0.7 - UI improvements

- [ ] improve error handling (ImGui)
- [ ] resize board
- [ ] selection
- [ ] cut/copy/paste
- [ ] add text to board

# v0.0.8 - advanced wiring

- [ ] lower layer wiring
- [ ] bus
- [ ] bus adapter

# v0.0.9 - subcircuits

- [ ] ?

# v0.0.10 - game

- [ ] title screen
- [ ] game level support

# v0.0.11 - custom boards

- [ ] video
- [ ] audio
- [ ] inputs

# v0.9.0 - game levels

- [ ] Nand2Tetris levels