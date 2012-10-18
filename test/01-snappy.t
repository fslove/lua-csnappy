#!/usr/bin/env lua

require 'Test.More'

plan(4 + 1025)

local snappy = require 'snappy'

error_like(function () snappy.compress(snappy) end,
           "bad argument #1 to 'compress' %(string expected, got table%)",
           "bad argument")

error_like(function () snappy.decompress(snappy) end,
           "bad argument #1 to 'decompress' %(string expected, got table%)",
           "bad argument")

error_like(function () snappy.decompress '' end,
           "snappy: bad header",
           "bad header")

error_like(function () snappy.decompress 'malformed' end,
           "snappy: malformed data",
           "malformed data")

for i = 0, 1024 do
    local input = string.rep('0', i)
    local compressed = snappy.compress(input)
    local decompressed = snappy.decompress(compressed)
    is(decompressed, input, "length: " .. i)
end
