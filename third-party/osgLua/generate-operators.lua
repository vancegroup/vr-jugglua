#!/usr/bin/env lua


mtindex = {}

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


scalarOrder = {
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


mt = {
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


MathTypes = {
	Vec(2, "b");
	Vec(2, "d");
	Vec(2, "f");
	Vec(2, "s");
	Vec(3, "b");
	Vec(3, "d");
	Vec(3, "f");
	Vec(3, "s");
	Vec(4, "b");
	Vec(4, "d");
	Vec(4, "f");
	Vec(4, "s");
	Vec(4, "ub");
	Matrix("d");
	Matrix("f");
	MathType{
		category = "Quat";
		dimension = 4;
		scalar = "d";
	};
}

MathAndArithmeticTypes = {
  Scalar("double", "d");
  Scalar("float", "f");
}

for _, v in ipairs(MathTypes) do
  table.insert(MathAndArithmeticTypes, v)
end

genericBinaryOp = function(symbol)
  return function(a1,  a2)
    return table.concat({a1, symbol, a2}, " ")
  end
end
reverseArgs = function(op)
  return function(a1,  a2)
    return op(a2, a1)
  end
end

local accessAllElements = function(t, a)
	local r = {}
	for i=1,t.dimension do
  		table.insert(r, table.concat({a, "[", i - 1, "]"}))
	end
	return table.concat(r, ", ")
end

promoting = function(op, t1, t2)
  return function(a1,  a2)
  	local a1t = {t1:getName(), "("}
  	if t1.category == "Vec" then
  		table.insert(a1t, accessAllElements(t1, a1))
  	else
  		table.insert(a1t, a1)
  	end
  	table.insert(a1t,  ")")
 	
  	
  	local a2t = {t2:getName(), "("}
  	if t2.category == "Vec" then
  		table.insert(a2t, accessAllElements(t2, a2))
  	else
  		table.insert(a2t, a2)
  	end
  	table.insert(a2t,  ")")
    return op(table.concat(a1t), table.concat(a2t))
  end
end



Operators = {
  {
    name = "add";
    test = function(a, b)
      if a.category == "Vec" and b.category == "Vec" and a.dimension == b.dimension then
      	if a.scalar == b.scalar then
	        return genericBinaryOp("+")
	    else
	    	return promoting(genericBinaryOp("+"), a:promoteScalar(b), b:promoteScalar(a))
	    end
      end
    end
  };
  {
    name = "sub";
    test = function(a, b)
      if a.category == "Vec" and b.category == "Vec" and a.dimension == b.dimension then
      	if a.scalar == b.scalar then
	        return genericBinaryOp("-")
	    else
	    	return promoting(genericBinaryOp("-"), a:promoteScalar(b), b:promoteScalar(a))
	    end
      end
    end
  };
  {
    name = "div";
    test = function(a, b)
      if (a.category == "Quat" and b.category == "Quat") or (a.category == "Vec" and b.category == "Scalar") then
        return genericBinaryOp("/")
      end
    end
  };
  {
    name = "mul";
    test = function(a, b)
      local optionallyPromoting = function()
        if a.scalar == b.scalar then
        end
      end
      -- composing rotations or doing a dot product
      if 
        (a.category == b.category) and
        (a.category == "Matrix" or a.category == "Vec" or a.category == "Quat") and
        (a.dimension == b.dimension)
        then
      	if a.scalar == b.scalar then
	        return genericBinaryOp("*")
	      else
	      	return promoting(genericBinaryOp("*"), a:promoteScalar(b), b:promoteScalar(a))
	      end
      -- Applying transformations
      elseif
        (a.category == "Matrix" --[[or a.category == "Quat"]]) and
        (b.category == "Vec" and (b.dimension == 3 or b.dimension == 4) and b:isFloating())
        then
        return genericBinaryOp("*")
      elseif
        (a.category == "Vec" and (a.dimension == 3 or a.dimension == 4) and a:isFloating()) and
        (b.category == "Matrix" --[[or b.category == "Quat"]])
        then
        return genericBinaryOp("*")

      -- Scaling a vector
      elseif
        a.category == "Vec" and a:isFloating() and b.category == "Scalar"
        then
        return genericBinaryOp("*")
      elseif
        a.category == "Scalar" and b.category == "Vec" and b:isFloating() 
        then
        return reverseArgs(genericBinaryOp("*"))
      end
    end
  };
  {
    name = "pow";
    test = function(a, b)
      if 
        a.category == "Vec" and b.category == a.category and
        a.dimension == 3 and b.dimension == 3 and
        a:isFloating() and b:isFloating()
        then
        return genericBinaryOp("^")
      end
    end
  };
  {
    name = "eq";
    test = function(a, b)
      if 
        a.category == "Matrix" and a:isSameAs(b)
        then
        return genericBinaryOp("==")
      elseif
        a.category == "Vec" and a.category == b.category and a.dimension == b.dimension
        then
        if a.scalar == b.scalar then
	        return genericBinaryOp("==")
	      else
	      	return promoting(genericBinaryOp("=="), a:promoteScalar(b), b:promoteScalar(a))
	      end
      end
    end
  };
}

anotherOperandExists = function(a, op)
  for _, b in ipairs(MathAndArithmeticTypes) do
    if op.test(a, b) or op.test(b, a) then
      return true
    end
  end
end

local CodeGen = require "CodeGen"
local tmpl = CodeGen{
wholeFile = [[
/** @file
	@brief Implementation GENERATED BY generate-operators.lua - Do not edit by hand!

	@date 2013

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
	
*/

//           Copyright Iowa State University 2013.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


// Internal Includes
#include "RegisterMathMetamethods.h"
#include "UsableAs.h"
#include "MissingOperators.h"

#include "LuaIncludeFull.h"

// Library/third-party includes

#include <osgLua/Value>

#include <osgLua/introspection/ExtendedTypeInfo>
#include <osgLua/introspection/Value>
#include <osgLua/introspection/Type>
#include <osgLua/introspection/variant_cast>

${types/includes(); separator='\n'}

// Standard includes
// - none


namespace osgLua {
  // Anonymous namespace for tag types
  namespace {
    ${operators/operatorTag()}
  } // end of namespace

  template<typename Operator, typename T>
  struct AttemptOperator;

  ${attempts/singleTypeAttempt()}

  bool registerMathMetamethods(lua_State * L, introspection::Type const& t) {
    ${types/singleTypePush()}
    return false;
  }
} // end of namespace osgLua

]];
  
includes = [[#include <osg/${baretypename}>]];

operatorTag = [[struct ${name};
]];
 
singleTypeAttempt = [[
template<>
struct AttemptOperator<${operator}, ${typename}> {
  static int attempt(lua_State * L) {
    if (lua_isnil(L, -2) || lua_isnil(L, -1)) {
      return luaL_error(L, "[%s:%d] Could not ${operator}: %s operand is nil", __FILE__, __LINE__, (lua_isnil(L, -2) ? "first" : "second"));
    }
    ${asFirst?attemptFirst()}
    ${asSecond?attemptSecond()}
    
    return luaL_error(L, "[%s:%d] Could not ${operator} instances of %s, %s", __FILE__, __LINE__,
      getValue(L, -2).getType().getQualifiedName().c_str(), getValue(L, -1).getType().getQualifiedName().c_str());
  }
};

]];

singleTypePush = [[
if (introspection::Reflection::getType(extended_typeid<${typename}>()) == t) {
  ${operators/singleOperatorPush()}
  return true;
}
]];
singleOperatorPush = [[
lua_pushcfunction(L, &(AttemptOperator<${operator}, ${typename}>::attempt));
lua_setfield(L, -2, "__${operator}");
]];

attemptFirst = [[
if (osgLuaValueUsableAs<${typename}>(L, -2)) {
  ${asFirst/bothArgAttemptFirst()}
  return true;
}
]];

bothArgAttemptFirst = [[
if (osgLuaValueUsableAs<${other}>(L, -1)) {
  ${bothArgPerform()}
}
]];

attemptSecond = [[
if (osgLuaValueUsableAs<${typename}>(L, -1)) {
  ${asSecond/bothArgAttemptSecond()}
}
]];

bothArgAttemptSecond = [[
if (osgLuaValueUsableAs<${other}>(L, -2)) {
  ${bothArgPerform()}
}
]];

bothArgPerform = [[
${aType} a = introspection::variant_cast<${aType}>(getValue(L, -2));
${bType} b = introspection::variant_cast<${bType}>(getValue(L, -1));
introspection::Value r = ${perform};
Value::push(L, r);
return 1;

]];

}

tmpl.types = {}
tmpl.attempts = {}
tmpl.operators = Operators

for _, a in ipairs(MathTypes) do
  local mytype = {}
  local inserted = false
  mytype.typename = a:getName()
  mytype.baretypename = a:getUnqualifiedName()
  mytype.operators = {}
  for _, op in ipairs(Operators) do
    if anotherOperandExists(a, op) then
      
      if not inserted then
        inserted = true
        table.insert(tmpl.types, mytype)
      end

      local myoperator = {operator = op.name, typename = a:getName()}
      table.insert(mytype.operators, myoperator)
      table.insert(tmpl.attempts, myoperator)

      for _, b in ipairs(MathAndArithmeticTypes) do
        local result = op.test(a, b)
        if result then
          myoperator.asFirst = myoperator.asFirst or {}
          local thisOperation =
          {
            aType = a:getName();
            bType = b:getName();
            other = b:getName();
            perform = result("a", "b")
          }
          table.insert(myoperator.asFirst, thisOperation)
        end

        local result = op.test(b, a)
        if result then
          myoperator.asSecond = myoperator.asSecond or {}
          local thisOperation =
          {
            aType = b:getName();
            bType = a:getName();
            other = b:getName();
            perform = result("a", "b")
          }
          table.insert(myoperator.asSecond, thisOperation)
        end
      end
    end
  end

  
end
output = tmpl "wholeFile"
print(output)

