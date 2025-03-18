## Modules
- UI
  - User interface (C++) - no Lua access
- Engine (generate .a file)
  - Engine (Teal)
  - API (Teal + C++)
  - C simulation

Dev version:
- UI uses following .so:
  - API (loads and compile tl files on the fly)
    - Loads C simulation as .so

Release version
- API and simulation generated as .a files
  - API load compiled tl files as bytecode
- .a engine file embedded in C++
