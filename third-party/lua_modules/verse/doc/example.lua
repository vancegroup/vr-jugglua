-- Change these:
local jid, password = "spartaprimary@indigo.student.iastate.edu", "sparta";

-- This line squishes verse each time you run,
-- handy if you're hacking on Verse itself
--os.execute("squish --minify-level=none --use-http");

require "verse" -- Verse main library
require "verse.client" -- XMPP client library
local st = require "util.stanza"

c = verse.new();
c:add_plugin("version");



function process_message(event)
	if not event.body then return; end;
	if event.delay then return; end; -- don't process old mesages
	--local body = event.body
	local body = event:get_child("body")
	print("process_message given: " , event)
	--c:send(stanza:reply():tag("body"):text("Got your message of " .. body:get_text()))
	verse.quit()
end
c:hook("message", process_message)

-- Add some hooks for debugging
c:hook("opened", function () print("Stream opened!") end);
c:hook("closed", function () print("Stream closed!") end);

-- This one prints all received datagrep 
--c:hook("incoming-raw", print, 1000);

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
	c.version:set{ name = "verse example client" };
	c:send(verse.presence()
		:tag("status"):text("Running the example app!"))
	c:query_version(c.jid, function (v) print("I am using "..(v.name or "<unknown>")); end);
	verse.changesettings{timeout=0;
		sendtimeout = 0;
		readtimeout = 0;
		sleeptime = 0}
	--verse.quit()
end);

print("Starting loop...")
verse.loop()
