#include "console.h"

namespace osgLua {

	/****************** Callback **********************/
	class ConsoleCallback : public osgGA::GUIEventHandler {
		public:
			ConsoleCallback(Console *c) : _console(c) {}

			virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter&) {
				switch (ea.getEventType()) {
					case(osgGA::GUIEventAdapter::KEYDOWN): {
							int key = ea.getKey();
							if (key > 0 && key < 256) {
								_console->addInputChar(key);
							} else if (key == osgGA::GUIEventAdapter::KEY_Return) {
								_console->addInputChar('\n');
							} else if (key == osgGA::GUIEventAdapter::KEY_BackSpace ||
							           key == osgGA::GUIEventAdapter::KEY_Delete) {
								_console->addInputChar(8);
							}
							return true;
						}
					default:
						return false;
				}
			}


		protected:
			osg::ref_ptr<Console> _console;
	};
	/**************************************************/


	/************* Lua print function *****************/
	int console_print(lua_State *L) {
		Console *console = reinterpret_cast<Console*>(
		                       lua_touserdata(L, lua_upvalueindex(1)));
		int total = lua_gettop(L);
		std::string s = "";

		if (total > 0) {
			s = lua_tostring(L, 1);
		}
		for (int i = 2; i <= total; ++i) {
			s = s + "    " + lua_tostring(L, i);
		}
		console->addText(s);
		return 0;
	}
	/**************************************************/


	Console::Console(unsigned int nl) : _numLines(nl), _current(nl - 1) {
		setMatrix(osg::Matrix::ortho2D(0.0, 1.0, 0.0, 1.0));
		osg::MatrixTransform* mva = new osg::MatrixTransform;
		mva->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
		mva->setMatrix(osg::Matrix::identity());
		addChild(mva);

		osg::Geode *textGeode = new osg::Geode;
		mva->addChild(textGeode);

		std::string font("/home/rpavlik/.fonts/CharisSILB.ttf");
		_text = new osg::ref_ptr<osgText::Text>[_numLines];

		float lineHeight = 1.0f / (_numLines + 2);

		for (unsigned int i = 0; i < _numLines; ++i) {
			_text[i] = new osgText::Text;
			textGeode->addDrawable(_text[i].get());

			_text[i]->setFont(font);
			_text[i]->setAlignment(osgText::Text::LEFT_BOTTOM);
			_text[i]->setFontResolution(100, 100);
			_text[i]->setCharacterSize(lineHeight);
			_text[i]->setColor(osg::Vec4(1, 1, 0, 1));
			_text[i]->setPosition(osg::Vec3(0, (i + 1) * 1.0f / (_numLines + 1), 0));
		}

		_input = new osgText::Text();
		_input->setFont(font);
		_input->setAlignment(osgText::Text::LEFT_BOTTOM);
		_input->setFontResolution(100, 100);
		_input->setCharacterSize(lineHeight);
		_input->setColor(osg::Vec4(1, 1, 1, 1));
		_input->setPosition(osg::Vec3(0, 0, 0));
		_input->setText("> ");
		textGeode->addDrawable(_input.get());

		osg::StateSet *sset = new osg::StateSet;
		sset->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);

		sset->setRenderBinDetails(11, "RenderBin");

		setStateSet(sset);

		_command[0] = 0;
	}

	void Console::addText(const std::string &s, Console::TextType tt) {
		std::string::size_type init = 0;
		std::string::size_type last = s.find_first_of("\n", init);
		while (last != std::string::npos) {
			_addText(s.substr(init, last - init), tt);
			init = last + 1;
			last = s.find_first_of("\n", init);
		}
		_addText(s.substr(init), tt);
	}

	void Console::_addText(const std::string &s, Console::TextType tt) {
		osg::Vec3 pos = _text[(_current + 1) % _numLines ]->getPosition();
		for (unsigned int i = 0; i < _numLines - 1; ++i) {
			unsigned int p = (i + _current + 1) % _numLines;
			unsigned int next = (p + 1) % _numLines;
			_text[p]->setPosition(_text[next]->getPosition());
		}

		_text[_current]->setPosition(pos);
		_text[_current]->setText(s);
		switch (tt) {
			case CONSOLE_NORMAL:
				_text[_current]->setColor(osg::Vec4(1, 1, 0, 1));
				break;
			case CONSOLE_ERROR:
				_text[_current]->setColor(osg::Vec4(1, 0, 0, 1));
				break;
			case CONSOLE_COMMAND:
				_text[_current]->setColor(osg::Vec4(1, 1, 1, 1));
				break;
		}

		_current = (_current + _numLines - 1) % _numLines;
	}

	void Console::addInputChar(char c) {
		size_t len = strlen(_command);

		if (c == '\n') {
			addText(std::string("> ") + _command, CONSOLE_COMMAND);
			exec(_command);
			_command[0] = 0;
		} else if (c == 8) {
			if (len > 0) {
				_command[len - 1] = 0;
			}
		} else {
			if (len < 512) {
				_command[len] = c;
				_command[len + 1] = 0;
			}
		}
		_input->setText(std::string(">") + std::string(_command));
	}



	void Console::attachToViewer(osgViewer::Viewer *viewer) {
//	viewer->getEventHandlerList().push_front( new ConsoleCallback(this) );
		const osg::DisplaySettings *ds = viewer->getDisplaySettings();
		if (ds) {
			float aspect = ds->getScreenWidth() / (float)ds->getScreenHeight();
			setMatrix(osg::Matrix::ortho2D(0.0, aspect, 0.0, 1.0));
		}
	}

	void Console::setScript(osgLua::Script *s) {
		if (_script.get() != s) {
			if (_script.valid()) {
				// restore the print function
				lua_State *L = _script->getLuaState();
				lua_getfield(L, LUA_REGISTRYINDEX, "osgLua_print");
				lua_setglobal(L, "print");
				lua_pushnil(L);
				lua_setfield(L, LUA_REGISTRYINDEX, "osgLua_print");
			}
			if (s) {
				lua_State *L = s->getLuaState();
				// keep a copy of the print function
				lua_getglobal(L, "print");
				lua_setfield(L, LUA_REGISTRYINDEX, "osgLua_print");
				// replace the print function
				lua_pushlightuserdata(L, (void*)this);
				lua_pushcclosure(L, console_print, 1);
				lua_setglobal(L, "print");
				_script = s;
			}
		}
	}

	bool Console::exec(const std::string &code) {
		try {
			_script->execute(code, 0, "user-input:");
		} catch (osgLua::Exception &e) {
			addText(e.what(), CONSOLE_ERROR);
			return false;
		}
		return true;
	}

} // end of osgLua namespace
