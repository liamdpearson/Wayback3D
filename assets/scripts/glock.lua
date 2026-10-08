local ammo

function start()
    self.rig:setAnim(0)
    ammo = 17
end

function update(deltaTime)
    if input.mousePressed(mouse.LEFT) and self.rig.currentAnim ~= 5 then
        if ammo > 1 then
            self.rig:setAnim(2, 0.01, 0)
            ammo = ammo - 1
            playSound("assets/sounds/gunshot.mp3")
        elseif ammo == 1 then
            self.rig:setAnim(3, 0.01, 1)
            ammo = ammo - 1
            playSound("assets/sounds/gunshot.mp3")
        else
            self.rig:setAnim(4, 0.05, 1)
            playSound("assets/sounds/noammo.wav")
        end
    end

    if input.keyPressed(key.R) and self.rig.currentAnim ~= 5 then
        self.rig:setAnim(5, 0.05, 0)
        ammo = 17
    end
end
