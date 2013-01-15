print("nav-testbed-launcher.lua starting up...")

require("nav-app-skeleton")

print("Parsing arguments")

-- Override the stock initScene with one to load the provided script.
script = arg[1]
do
	local oldInitScene = osgnav.initScene
	function osgnav:initScene()
		oldInitScene(self)
		dofile(script)
	end
end

for i = 2, #arg do
	vrjKernel.loadConfigFile(arg[i])
end

print("Entering kernel")
vrjKernel.enter()

