local ffi   =  require "ffi"
local vips  = require "vips"
local cam   = {}

cam.w = 640
cam.h = 480
cam.byte = 3
cam.bufferSize = 0


cam.init = function ()
    cam.bufferSize = cam.w * cam.h * cam.byte
    cam.buffer = ffi.new("char[?]", cam.w * cam.h * cam.byte)
    print("Image buffer :" .. cam.bufferSize)
    print("Line buffer : " .. cam.w * cam.byte)
end

cam.saveFrame = function(buffer, filename)
    for i=1,#buffer,1 do
        cam.buffer[i] = buffer:byte(i)
    end
    
    
    local image = vips.Image.new_from_memory(cam.buffer, cam.w, cam.h, cam.byte, "char")
    image:write_to_file(filename .. ".png")
end



return cam



--image = vips.Image.text("hello!")
--image:write_to_file("hello.png")
--image = vips.Image.new_from_file("frame.bmp")
--print(image)