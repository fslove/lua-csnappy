#!/usr/bin/env lua

require 'Test.More'

plan(1025)

local snappy = require 'snappy'

for i = 0, 1024 do
    local input = string.rep('0', i)
    local compressed = snappy.compress(input)
    local decompressed = snappy.decompress(compressed)
    is(decompressed, input, "length: " .. i)
end
