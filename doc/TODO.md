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

- [ ] theme for ImGUI
- [ ] main menu
- [ ] infobox (sidebar?)
- [ ] serialization / deserialization
  - [ ] save/load

# v0.0.4 - ICs + customization

- [ ] ic support
  - [ ] rotation
  - [ ] native behaviour
  - [ ] Lua support

# v0.0.5 - circuits QOL

- [ ] resize board
- [ ] selection
- [ ] cut/copy/paste
- [ ] add text to board

# v0.0.6 - advanced wiring

- [ ] lower layer wiring
- [ ] bus
- [ ] bus adapter

# v0.0.7 - subcircuits

- [ ] ?

# v0.0.8 - game

- [ ] title screen
- [ ] game level support

# v0.0.9 - custom boards

- [ ] video
- [ ] audio
- [ ] inputs

# v0.9.0 - game levels

- [ ] Nand2Tetris levels