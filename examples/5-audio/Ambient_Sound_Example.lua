--require getScriptFilename so we can add current path to modelSearch Path
require("getScriptFilename")
vrjLua.appendToModelSearchPath(getScriptFilename())

--find the path of an MP3 file and store it in "mp3Path" variable
mp3Path = vrjLua.findInModelSearchPath("SampleMP3.mp3") 
--find the path of a WAV file and store it in "wavPath" variable
wavPath = vrjLua.findInModelSearchPath("SampleWAV.wav")

--In order to play the sound, we need to change to the correct API (must uncomment one of the following)
--1)Use OpenAL for WAV files
snx.changeAPI("OpenAL")
--2)Use Audiere for MP3 files
--snx.changeAPI("Audiere")

--create a sound info object 
soundInfo = snx.SoundInfo()
-- set the filename attribute of the soundFile (path to your sound file)
soundInfo.filename = wavPath
--create a new sound handle and pass it the filename from the soundInfo object
soundHandle = snx.SoundHandle(soundInfo.filename)
--configure the soundHandle to use the soundInfo
soundHandle:configure(soundInfo)
--play or "trigger" the sound
soundHandle:trigger(1)
