--[[
lua-periphery -- https://github.com/vsergeev/lua-periphery
]]--
local Serial    = require('periphery').Serial
local vips      = require "vips"
local ffi       = require "ffi" --https://luajit.org/ext_ffi.html
local lfs       = require "lfs"
local io        = require "io"

local cam = require "cameraRecorder"

local teensy = nil
for file in lfs.dir("/dev/") do
    
    if file:match "ttyACM%d" then
        
        --teensy = io.open("/dev/" .. file)
        teensy = Serial("/dev/" .. file, 115200)
        print("Found Teensy at " .. file)
    end
end

local teensyInit = false
function connect2Teensy()
    if teensy == nil then return false end

    while (teensyInit == false) do
        --local b = teensy:read()
        local b = teensy:read(128, 500);
        if #b > 0 then
            print(b)
            for line in b:gmatch("[^\r\n]+") do
        
                if line:sub(1, 1) == "w" then cam.w = tonumber(line:sub(2, -1)) print("Image width: " ..  cam.w)end
                if line:sub(1, 1) == "h" then cam.h = tonumber(line:sub(2, -1)) print("Image height: " ..  cam.h)end 
                if line:sub(1, 1) == "r" then teensyInit = true end
            end
        else
            print("No Teensy-GVX Init!")
            teensy:close()
            os.exit() 
        end
    end
    return teensyInit
end


connect2Teensy()
cam.init()
print("Waiting for file ...")

local recording     = true
local frameStatus   = "STANDBY"
local frameNum      = 0
local totalByte     = 0
local raw           = nil --raw string buffer... keep.

t1 = os.clock()
while (recording == true) or frameNum < 30 do
    local b = ""
    if (frameStatus == "DATA") then
        b = teensy:read(cam.w * cam.byte, 25)    
    else
        b = teensy:read(1)
    end

    if (frameStatus == "DATA") then
        -- receive each line until you complete a frame.
        --b = b:sub(1, -2)
        if #raw < cam.h then 
            --print("= Line " .. #raw .. " =====================")
            table.insert(raw, b) 
            totalByte = totalByte + #b
        end

        if #raw == cam.h then    
            --validate data
            print(#raw .. " lines received. Expected " .. cam.h)
            print(totalByte .. " bytes receviced. Expected " .. cam.bufferSize)
            --if(frameNum == 25) then
                cam.saveFrame(table.concat(raw), "frame" .. frameNum)
            --end
            frameNum = frameNum + 1
            frameStatus = "STANDBY" 
            recording = false 
        end
        
    end
    
    if (string.sub(b, 1, 1) == '!') and (frameStatus == "STANDBY") then
        print("= New Frame ==================")
        raw         = {} 
        totalByte   = 0
        frameStatus = "DATA" 
    end
end

print(string.format("elapsed time: %.2f\n", os.clock() - t1))
print("Exiting program.")
teensy:close()