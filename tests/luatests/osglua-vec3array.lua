require("help")

help(osg.Vec3Array())

assert(#(osg.Vec3Array().Item) == 0)
print "Creating empty array"
a = osg.Vec3Array()

print "Inserting an element"
a.Item:insert(osg.Vec3(0,0,0))
print(#(a.Item))
assert(#(a.Item) == 1)
print(a.Item[1])
assert(a.Item[1] == osg.Vec3(0,0,0))


print "Modifying an element"
a.Item[1] = osg.Vec3(1,1,1)
print(#(a.Item))
assert(#(a.Item) == 1)
assert(a.Item[1] == osg.Vec3(1,1,1))
--[[
print "Trying ipairs"
for i, v in ipairs(a.Item) do
	print(i, v)
end]]

print "Trying an insert by setting the next contiguous index"
a.Item[2] = osg.Vec3(2,2,2)

assert(#(a.Item) == 2)
assert(a.Item[2] == osg.Vec3(2,2,2))

a.Item:remove(1)

assert(#(a.Item) == 1)
assert(a.Item[1] == osg.Vec3(2,2,2))

