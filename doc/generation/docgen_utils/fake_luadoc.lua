
return function(moduleName)

	local f = assert(io.open(moduleName ..'.luadoc', 'w'))

	local handled = {}

	local outputToFile = function(s)
		if s ~= nil then
			f:write(s, '\n')
		else
			f:write('\n')
		end
	end

	local output = outputToFile --print

	local outputCommentLine = function(s)
		output("-- " .. s:gsub('\n', '\n-- '))
	end

	local outputMethod = function(methodName, currDoc)
		if handled[methodName] then return end
		handled[methodName] = true
		output "---"
		outputCommentLine(currDoc.description)
		output(("function %s(%s) end"):format(methodName, table.concat(currDoc.params, ", ")))
		output()
	end

	local createSectionOutputter = function(tagname)
		return function(sectionName, desc)
			output("--- " .. desc)
			outputCommentLine("@" .. tagname .. " " .. sectionName)
			output()
		end
	end

	local outputModule = function(desc)
		output "---"
		outputCommentLine(desc)
		output(("module '%s'"):format(moduleName))
		output()
	end

	return {
		['moduleDesc'] = outputModule,
		['method'] = outputMethod,
		['class'] = createSectionOutputter("class"),
		['section'] = createSectionOutputter("section"),
	}
end
