lua-docstrings
==============
Original Author: Ryan Pavlik

<rpavlik@acm.org> <abiryan@ryand.net>
<http://academic.cleardefinition.com>

Iowa State University HCI Graduate Program/VRAC

Introduction
------------

This is just a small Lua module, named help, that does three things:

- Provides a way to record documentation for code, in the code: based on
the ideas from <http://lua-users.org/wiki/DecoratorsAndDocstrings>

- Provides a "help()" function to look up documentation for arbitrary
Lua objects, intended for "end user" use.

- Allows connection to other lua systems, like Luabind and osgLua,
that provide their own methods of introspective help, for seamless
use with "help()"

See the `examples.lua` and `test.lua` files for some instructive examples,
or do the following in your Lua interpreter:

    require("help")
    help(help) -- naturally :)



License
-------

This module is subject to this license:

> Copyright Iowa State University 2011.
>
> Distributed under the Boost Software License, Version 1.0.
>
> (See accompanying file `LICENSE_1_0.txt` or copy at
> <http://www.boost.org/LICENSE_1_0.txt>)


Important License Note!
-----------------------

If you find this file inside of another project, rather at the top-level
directory, you're in a separate project that is making use of lua-docstrings.
That separate project can (and probably does) have its own license specifics.