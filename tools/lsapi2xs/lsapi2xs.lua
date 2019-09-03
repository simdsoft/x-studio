--- LuaStudio到x-studio365 API转换程序V0.1 by x-studio365软件作者 -----------

require 'lsapi_conf'

for _,v in ipairs(KeyMap) do
    v.cmd = scicmds[v.cmd] -- string to number
    v.key = string.byte(v.key)
end

local file = io.open('xsapiLuaEXT.xml', "wb")

string.split = require('split').split

local str = '<autoc>\n'
for k,v in pairs(Lua_BaseLibFuncTip) do
    local buffer = '<keyword name="lualib:'
    buffer = buffer .. k 
    buffer = buffer .. '" func="yes">\n'
    buffer = buffer .. '<overload '
    
    -- parse return type
    local rettype = 'void'
    
    local nameEnd = v:find(' ')
    local guess = string.sub(v, 1, nameEnd - 1)
    local descPos = 1
    local nparams = {}
    if(#guess < 24) then
        rettype = guess
        -- parse args
        local left = v:find('%(')
        if left then
            local right = v:find('%)')
            if right and right > left then
                local strParams = v:sub(left + 1, right - 1)
                local arrParams = strParams:split(',')
                if(arrParams) then
                    for _,strParam in ipairs(arrParams) do
                        local arrParam = strParam:split(' ')
                        local filtered = {}
                        for _,mixer in ipairs(arrParam) do
                            if #mixer > 0 then
                                filtered[#filtered + 1] = mixer
                            end
                        end
                        if #filtered == 2 then
                            nparams[#nparams + 1] = filtered[2]
                        end
                    end
                end
                descPos = right + 1
            end
        end
    end
    
    local desc = v:sub(descPos):gsub('\n', '&#x0A;') -- replace
    
    -- now parse done, write buffer
    buffer = buffer .. ' retval="' 
    buffer = buffer .. rettype
    buffer = buffer .. '" descr="'
    buffer = buffer .. desc
    buffer = buffer .. '">\n'
    
    for _,paramName in ipairs(nparams) do
        buffer = buffer .. '<param name="'
        buffer = buffer .. paramName
        buffer = buffer .. '" />\n'
    end
    
    -- buffer = buffer ..  
    
    buffer = buffer .. '</overload>\n'
    buffer = buffer .. '</keyword>\n'
    
    str = str .. buffer
end

str = str .. '</autoc>\n'

file:write(str)

file:close()
