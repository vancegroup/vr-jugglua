

local mtindex = {}

do
  local cats = {
    Vec = function(t)
      return ("Vec%d%s"):format(t.dimension, t.scalar)
    end;
    Matrix = function(t)
      assert(t.dimension == 4)
      return "Matrix" .. t.scalar
    end;
    Quat = function()
      return "Quat"
    end;
    Scalar = function(t)
      assert(t.name)
      return t.name
    end
  }

  mtindex.getUnqualifiedName = function(self)
    return cats[self.category](self)
  end

  mtindex.getName = function(self)
  	if self.category == "Scalar" then
	    return self:getUnqualifiedName()
	else
		return "osg::" .. self:getUnqualifiedName()
	end
  end
end

do
  local floatTypes = {
    d = true;
    f = true;
  }

  mtindex.isFloating = function(self)
    return floatTypes[self.scalar]
  end
end

mtindex.isSameAs = function(self, other)
  return self:getName() == other:getName()
end


local scalarOrder = {
  "ub",
  "b",
  "s",
  "f",
  "d"
}
for i, t in ipairs(scalarOrder) do
  scalarOrder[t] = i
end

mtindex.promoteScalar = function(self, other)
  local t = {}
  for k, v in pairs(self) do
    t[k] = v
  end
  local scalarID = math.max(scalarOrder[self.scalar], scalarOrder[other.scalar])
  t.scalar = scalarOrder[scalarID]
  return MathType(t)
end


local mt = {
  __index = mtindex
}

MathType = function(t)
  return setmetatable(t, mt)
end

Vec = function(dim, scalarType)
  assert(dim)
  assert(scalarType)
  return MathType{
    category = "Vec";
    dimension = dim;
    scalar = scalarType;
  }
end

Matrix = function(scalarType)
  assert(scalarType)
  return MathType{
    category = "Matrix";
    dimension = 4;
    scalar = scalarType;
  }
end

Scalar = function(typeName, scalarType)
  assert(scalarType)
  return MathType{
    category = "Scalar";
    dimension = 1;
    scalar = scalarType;
    name = typeName;
  }
end
