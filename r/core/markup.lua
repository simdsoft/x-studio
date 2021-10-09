-- Copyright 2011-16 Paul Kulchenko, ZeroBrane LLC
-- styles for comment markup
---------------------------------------------------------
local MD_MARK_ITAL = '_' -- italic
local MD_MARK_BOLD = '**' -- bold
local MD_MARK_LINK = '[' -- link description start
local MD_MARK_HEAD = '#' -- header
local MD_MARK_CODE = '`' -- code
local MD_MARK_BOXD = '|' -- highlight
local MD_MARK_MARK = ' ' -- separator

-- old versions of Scintilla had only 5-bit styles, so assign styles manually in those cases
local markup = {
    [MD_MARK_MARK] = {
        ["v"] = false,
        ["st"] = 249,
    },
    [MD_MARK_BOXD] = {
        ["b"] = true,
        ["st"] = 255,
        ["fg"] = {
            [1] = 127,
            [2] = 0,
            [3] = 127,
        },
    },
    [MD_MARK_CODE] = {
        ["st"] = 254,
        ["fg"] = {
            [1] = 127,
            [2] = 127,
            [3] = 127,
        },
        ["fs"] = 10,
    },
    [MD_MARK_LINK] = {
        ["st"] = 252,
        ["hs"] = {
            [1] = 32,
            [2] = 32,
            [3] = 127,
        },
        ["u"] = true,
    },
    [MD_MARK_ITAL] = {
        ["i"] = true,
        ["st"] = 250,
    },
    [MD_MARK_HEAD] = {
        ["b"] = true,
        ["fn"] = "Lucida Console",
        ["st"] = 253,
    },
    [MD_MARK_BOLD] = {
        ["b"] = true,
        ["st"] = 251,
    },
}

-- allow other editor features to recognize this special markup
function MarkupIsSpecial(style) return style == markup[MD_MARK_MARK].st end
function MarkupIsAny(style)
    for _, mark in pairs(markup) do
        if style == mark.st then return true end
    end
    return false
end
