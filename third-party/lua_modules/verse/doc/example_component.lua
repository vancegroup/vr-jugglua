local jid, password = "echo.localhost", "hellohello";

--os.execute("squish --minify-level=none verse");
require "verse"
require "verse.component"
require "socket"
c = verse.new(verse.logger())
c:add_plugin("version");
c:add_plugin("ping");

-- Add some hooks for debugging
c:hook("opened", function () print("Stream opened!") end);
c:hook("closed", function () print("Stream closed!") end);
c:hook("stanza", function (stanza) print("Stanza:", stanza) end);

-- This one prints all received data
c:hook("incoming-raw", print, 1000);

-- Print a message after authentication
c:hook("authentication-success", function () print("Logged in!"); end);
c:hook("authentication-failure", function (err) print("Failed to log in! Error: "..tostring(err.condition)); end);

-- Print a message and exit when disconnected
c:hook("disconnected", function () print("Disconnected!"); os.exit(); end);

-- Now, actually start the connection:
c.connect_host = "127.0.0.1"
c:connect_component(jid, password);

-- Catch binding-success which is (currently) how you know when a stream is ready
c:hook("ready", function ()
	print("Stream ready!");
	c.version:set{ name = "verse example component" };
end);

print("Starting loop...")
verse.loop()
