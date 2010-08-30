local verse = require "verse";
local stream = verse.stream_mt;

local jid_split = require "util.jid".split;
local lxp = require "lxp";
local st = require "util.stanza";

-- Shortcuts to save having to load util.stanza
verse.message, verse.presence, verse.iq, verse.stanza, verse.reply, verse.error_reply =
	st.message, st.presence, st.iq, st.stanza, st.reply, st.error_reply;

local init_xmlhandlers = require "core.xmlhandlers";

local xmlns_stream = "http://etherx.jabber.org/streams";

local stream_callbacks = {
	stream_ns = xmlns_stream,
	stream_tag = "stream",
	 default_ns = "jabber:client" };
	
function stream_callbacks.streamopened(stream, attr)
	stream.stream_id = attr.id;
	if not stream:event("opened", attr) then
		stream.notopen = nil;
	end
	return true;
end

function stream_callbacks.streamclosed(stream)
	return stream:event("closed");
end

function stream_callbacks.handlestanza(stream, stanza)
	if stanza.attr.xmlns == xmlns_stream then
		return stream:event("stream-"..stanza.name, stanza);
	elseif stanza.attr.xmlns then
		return stream:event("stream/"..stanza.attr.xmlns, stanza);
	end

	return stream:event("stanza", stanza);
end

function stream:reset()
	-- Reset stream
	local parser = lxp.new(init_xmlhandlers(self, stream_callbacks), "\1");
	self.parser = parser;
	
	self.notopen = true;
	
	return true;
end

function stream:connect_client(jid, pass)
	self.jid, self.password = jid, pass;
	self.username, self.host, self.resource = jid_split(jid);
	
	-- Required XMPP features
	self:add_plugin("tls");
	self:add_plugin("sasl");
	self:add_plugin("bind");
	self:add_plugin("session");
	
	function self.data(conn, data)
		local ok, err = self.parser:parse(data);
		if ok then return; end
		stream:debug("debug", "Received invalid XML (%s) %d bytes: %s", tostring(err), #data, data:sub(1, 300):gsub("[\r\n]+", " "));
		stream:close("xml-not-well-formed");
	end
	
	self:hook("incoming-raw", function (data) return self.data(self.conn, data); end);
	
	self.curr_id = 0;
	
	self.tracked_iqs = {};
	self:hook("stanza", function (stanza)
		local id, type = stanza.attr.id, stanza.attr.type;
		if id and stanza.name == "iq" and (type == "result" or type == "error") and self.tracked_iqs[id] then
			self.tracked_iqs[id](stanza);
			self.tracked_iqs[id] = nil;
			return true;
		end
	end);
	
	self:hook("stanza", function (stanza)
		if stanza.attr.xmlns == nil or stanza.attr.xmlns == "jabber:client" then
			if stanza.name == "iq" and (stanza.attr.type == "get" or stanza.attr.type == "set") then
				local xmlns = stanza.tags[1] and stanza.tags[1].attr.xmlns;
				if xmlns then
					ret = self:event("iq/"..xmlns, stanza);
					if not ret then
						ret = self:event("iq", stanza);
					end
				end
				if ret == nil then
					self:send(verse.error_reply(stanza, "cancel", "service-unavailable"));
					return true;
				end
			else
				ret = self:event(stanza.name, stanza);
			end
		end
		return ret;
	end, -1);

	local function stream_ready()
		self:event("ready");
	end
	self:hook("session-success", stream_ready, -1)
	self:hook("bind-success", stream_ready, -1);

	local _base_close = self.close;
	function self:close(reason)
		if not self.notopen then
			self:send("</stream:stream>");
		end
		return _base_close(self);
	end
	
	-- Initialise connection
	self:connect(self.connect_host or self.host, self.connect_port or 5222);
	self:reopen();
end

function stream:reopen()
	self:reset();
	self:send(st.stanza("stream:stream", { to = self.host, ["xmlns:stream"]='http://etherx.jabber.org/streams',
		xmlns = "jabber:client", version = "1.0" }):top_tag());
end

function stream:send_iq(iq, callback)
	local id = self:new_id();
	self.tracked_iqs[id] = callback;
	iq.attr.id = id;
	self:send(iq);
end

function stream:new_id()
	self.curr_id = self.curr_id + 1;
	return tostring(self.curr_id);
end
