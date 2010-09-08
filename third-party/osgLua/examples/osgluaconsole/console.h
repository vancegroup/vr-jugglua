#ifndef OSGLUA_CONSOLE 
#define OSGLUA_CONSOLE

#include <osgDB/WriteFile>
#include <osg/Projection>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/MatrixTransform>
#include <osgText/Text>
#include <osg/StateSet>
#include <osg/Depth>

#include <osgViewer/Viewer>

#include <osgLua/Script>

namespace osgLua {

class Console: public osg::Projection
{
    public:
		enum TextType {
			CONSOLE_NORMAL,
			CONSOLE_COMMAND,
			CONSOLE_ERROR
		};

        Console(unsigned int numLines);

		unsigned int getNumLines() const { return _numLines; }
		void addText(const std::string &s, TextType type = CONSOLE_NORMAL);

		void setScript(osgLua::Script *s);
		osgLua::Script *getScript() { return _script.get(); }

		void addInputChar(char c);
		void attachToViewer( osgViewer::Viewer *viewer);

		// returns false if the execution fails, true otherwise
		bool exec(const std::string &s);

    private:
		// s doesn't have any '\n'
		void _addText(const std::string &s,  TextType type = CONSOLE_NORMAL);

		unsigned int _numLines;
		unsigned int _current;
        osg::ref_ptr<osgText::Text> *_text;
        osg::ref_ptr<osgText::Text> _input;
		osg::ref_ptr<osgLua::Script> _script;
		char _command[512];
};

} // end of osgLua namespace
#endif
