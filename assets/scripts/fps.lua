local fps
local frames = 0
local time = 0

function start()
    fps = find.ui("fps")
end

function update(deltaTime)
    frames = frames + 1
    time = time + deltaTime

    if frames % 250 == 0 then
        fps.text = "FPS: " .. math.floor(frames/time)
        frames = 0
        time = 0
    end
end