-- This file is generated by x-studio Pro 10.0.9000.122 © 2022, All rights reserved.
-- module name: xslxml - The x-studio lua xml lib
local class = require("class_spec")
local xslxml = xsxml;

local _M = class("xslxml.element");
function _M:ctor(element_ptr)
    self.element_ptr = element_ptr;
end

function _M:to_string(styled)
    return xslxml.element_to_string(self.element_ptr, styled)
end

function _M:set_value(value)
    xslxml.element_set_value(self.element_ptr, value)
end

function _M:set_attribute_value(attr,value)
    xslxml.element_set_attribute_value(self.element_ptr,attr, value)
end

function _M:get_value(defaultValue)
    return xslxml.element_get_value(self.element_ptr, defaultValue)
end

function _M:get_attribute_value(attr,defaultValue)
    return xslxml.element_get_attribute_value(self.element_ptr, attr, defaultValue);
end

-- need call dispose when not use.
function _M:add_child(name,value)
    if(value == nil) then
        return _M.new(xslxml.element_add_child(self.element_ptr, name));
    else
        return _M.new(xslxml.element_add_child(self.element_ptr, name, value));
    end
end

--[[
-- get_child of current element: 
-- get_child(name)
-- get_child(index)
]]
function _M:get_child(...)
    return _M.new(xslxml.element_get_child(self.element_ptr, ...));
end

function _M:is_good()
    return xslxml.element_is_good(self.element_ptr);
end

-- remove current element from xml tree.
function _M:remove_self()
    xslxml.element_remove_self(self.element_ptr);
end

function _M:dispose()
    if(self.element_ptr ~= nil) then
        xslxml.element_dispose(self.element_ptr);
        self.element_ptr = nil
    end
end

xslxml.element = _M;
EditorLogD('init xslxml succeed.');

return xslxml;
