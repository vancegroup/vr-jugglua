local function not_impl()
	error("Encoding function not implemented");
end

module "encodings"

stringprep = {};
base64 = { encode = not_impl, decode = not_impl };

return _M;
