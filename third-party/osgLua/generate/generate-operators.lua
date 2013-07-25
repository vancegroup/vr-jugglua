#!/usr/bin/env lua

require "mathtypehelpers"


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

UnaryOperators = {
	{
    name = "unm";
    test = function(a)
      if a.category == "Vec" and a:isFloating() then
        return function(a)
          return "-" .. a
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

tmpl = dofile("operators-template.lua")

tmpl.types = {}
tmpl.attempts = {}
tmpl.unaryattempts = {}
tmpl.operators = Operators
tmpl.unaryoperators = UnaryOperators

for _, a in ipairs(MathTypes) do
  local mytype = {}
  local inserted = false
  mytype.typename = a:getName()
  mytype.baretypename = a:getUnqualifiedName()
  mytype.operators = {}
  for _, op in ipairs(UnaryOperators) do
    local result = op.test(a)
    if result then
      if not inserted then
        inserted = true
        table.insert(tmpl.types, mytype)
      end
      local myoperator = {operator = op.name, typename = a:getName(), perform = result("a")}
      table.insert(mytype.operators, myoperator)
      table.insert(tmpl.unaryattempts, myoperator)
    end
  end
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

