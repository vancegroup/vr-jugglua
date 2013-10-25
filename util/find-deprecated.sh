#!/bin/sh -e
warnString() {
  xargs -0 grep --with-filename --line-number --fixed-strings \
    "$1" | sed "s/$/    [$2]/"
}

warnOnAllLuaFiles() {
  find examples vrjugglua -name "*.lua" -print0 | \
    warnString "$1" "$2"
}

warnOnExamples() {
  find examples -name "*.lua" -print0 | \
    warnString "$1" "$2"
}

(
cd $(dirname $0) && cd ..
warnOnAllLuaFiles "vrjLua.appendToModelSearchPath(getScriptFilename())" "Replace with AddAppDirectory()"
warnOnAllLuaFiles "dofile(vrjLua.findInModelSearchPath" "Replace with runfile()"

warnOnExamples "osg.MatrixTransform()" "Consider using MatrixTransform{}"
warnOnExamples "osg.PositionAttitudeTransform()" "Consider using Transform{}"
warnOnExamples "osg.Switch(" "Consider using Switch{}"
warnOnExamples "osg.Group(" "Consider using Group{}"

for size in 4 3 2; do
    warnOnExamples "osg.Vec${size}d" "Use Vec() instead"
done

for type in f b s ub; do
for size in 4 3 2; do
    warnOnExamples "osg.Vec${size}${type}" "Use Vec${type}() instead"
done
done




)
