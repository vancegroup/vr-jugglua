print("CTEST_FULL_OUTPUT")
require "Scene"

function expectSuccess(script)
    print("Expect success:", script)
    loadstring(script)()
    print("OK!")
end

function expectAnythingButCrash(script)
    print("Expect success or failure but no crash:", script)
    local f = loadstring(script)
    local success, retval = pcall(f)
    if success then
        print("OK! Call succeeded")
    else
        print("OK! Call reported an error:", retval)
    end
end

function expectError(script)
    print("Expect error:", script)
    local f = loadstring(script)
    local success, retval = pcall(f)
    if success then
        error("Should have failed!")
    end
    print("OK! Reported error:", retval)
end

expectSuccess [[MatrixTransform{}.Matrix = osg.Matrixd()]]
expectAnythingButCrash [[MatrixTransform{}.Matrix = osg.Matrixf()]]
expectSuccess [[MatrixTransform{}.Matrix = MatrixTransform{}.Matrix]]


print("Done!")
