-- Change these:
local jid, password = "user@example.com", "secret";
local url = "http://example.com:80/http-bind";

-- This line squishes verse each time you run,
-- handy if you're hacking on Verse itself
--os.execute("squish --minify-level=none verse");

require "verse" -- Verse main library
require "verse.bosh" -- Verse BOSH support
require "verse.client" -- XMPP client library

c = verse.new_bosh(nil, url);
c:add_plugin("version");

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
c:connect_client(jid, password);

-- Catch the "ready" event to know when the stream is ready to use
c:hook("ready", function ()
	print("Stream ready!");
	c.version:set{ name = "Verse example BOSH client" };
	c:query_version(c.jid, function (v) print("I am using "..(v.name or "<unknown>")); end);
end);

print("Starting loop...")
verse.loop()
