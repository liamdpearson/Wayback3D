function start()
end

function update(deltaTime)
    if input.keyPressed(key.Y) then
        self:play()
    end
    if input.keyPressed(key.U) then
        self:stop()
    end
end