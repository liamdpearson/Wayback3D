local test

function start()
    test = find.obj("test sound")
end

function update(deltaTime)
    if input.keyPressed(key.Y) then
        test:play()
    end
end