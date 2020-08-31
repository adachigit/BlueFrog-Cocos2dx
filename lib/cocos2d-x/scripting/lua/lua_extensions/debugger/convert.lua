local inputFile = nil
local outputFile = nil
local hexCount = 0

function writeHex(hex)
	outputFile:write(hex)
	
	hexCount = hexCount + 1
	if hexCount >= 16 then
		outputFile:write("\n")
		hexCount = 0
	end
end

function convert()
	inputFile = io.open(".\\luac.out", "rb")
	outputFile = io.open(".\\debugger_convert.txt", "w")
	
	local buff = inputFile:read("*a")
	print(string.format("File size is %d", string.len(buff)))
	
	for i = 1, string.len(buff) do
		local s = string.format("0x%02x,", string.byte(buff, i))
		writeHex(s)
	end
--[[
	for l in inputFile:lines() do
		for i = 1, string.len(l) do
			local s = string.format("0x%x,", string.byte(l, i))
			writeHex(s)
			if i == string.len(l) then
				for _, ll in ipairs({"0x0d,", "0x0a,", "0x00," }) do
					writeHex(ll)
				end
			end
		end
	end
--]]
	outputFile:close()
end

convert()