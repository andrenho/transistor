local type SceneItem = require "ui.sceneitem"

local type Scene = {SceneItem}

local record Render
    steps_per_second: integer
    scenes: {Scene}
end

return Render
