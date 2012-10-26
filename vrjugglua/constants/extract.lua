
tablename = arg[2] or "osgLua"

local f = io.open(arg[1] or "gl.h", "rb")
-- Grab everything between the " * Constants" and "/* GL_ARB_multitexture" comment lines
contents = f:read("*all"):gsub("^.- %* Constants(.-) /%* GL_ARB_multitexture.*", "%1")
f:close()

-- Create table in case it doesn't exists
print( ("%s = %s or {}"):format(tablename, tablename) )

-- Populate table
local defs = 0
for defname, val in contents:gmatch("#define (GL_[%w_]+)%s*(0x%x%x%x%x)") do
	print( ("%s.%s = %s"):format(tablename, defname, val) )
	defs = defs + 1
end

-- Friendly little comment.
print("-- Total constants defined:", defs)

