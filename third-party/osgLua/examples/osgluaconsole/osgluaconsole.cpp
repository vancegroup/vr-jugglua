#include <osgLua/Script>
#include <iostream>


#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgIntrospection/Value>
#include <osgIntrospection/Reflection>
#include <osgIntrospection/variant_cast>
#include <osg/Vec3>
#include <osg/io_utils>

#include "console.h"


osgLua::Script *createScript() {
	osgLua::Script *s = new osgLua::Script();
	/* you can do this or let the script enable and load
	 * libraries using osgLua.load(...)
	 */
	s->enable("osg");
	return s;
}


int main(int argc, char **argv) {
	// use an ArgumentParser object to manage the program arguments.
	osg::ArgumentParser arguments(&argc, argv);

	// set up the usage document, in case we need to print out how to use this program.
	arguments.getApplicationUsage()->setDescription(arguments.getApplicationName() + " is the example which demonstrates how to do Head Up Displays.");
	arguments.getApplicationUsage()->setCommandLineUsage(arguments.getApplicationName() + " [options] [filename] ...");
	arguments.getApplicationUsage()->addCommandLineOption("-h or --help", "Display this information");


	// construct the viewer.
	osgViewer::Viewer viewer(arguments);

	// get details on keyboard and mouse bindings used by the viewer.
	viewer.getUsage(*arguments.getApplicationUsage());

	// if user request help write it out to cout.
	if (arguments.read("-h") || arguments.read("--help")) {
		arguments.getApplicationUsage()->write(std::cout);
		return 1;
	}

	// any option left unread are converted into errors to write out later.
	arguments.reportRemainingOptionsAsUnrecognized();

	// report any errors if they have occured when parsing the program aguments.
	if (arguments.errors()) {
		arguments.writeErrorMessages(std::cout);
		return 1;
	}

	// read the scene from the list of file specified commandline args.
	osg::ref_ptr<osg::Node> scene = osgDB::readNodeFiles(arguments);

	osg::ref_ptr<osg::Group> group  = new osg::Group;
	osg::ref_ptr<osg::Group> root  = new osg::Group;
	group->addChild(root.get());
	if (scene.valid()) {
		root->addChild(scene.get());
	}

	osg::ref_ptr<osgLua::Console> console = new osgLua::Console(50);
	osg::ref_ptr<osgLua::Script> script = createScript();
	console->setScript(script.get());
	group->addChild(console.get());
	console->addText("Wellcome to osgLua.\n\n");
	console->addText("Press (esc) to exit");
	console->addText("");
	script->set(root.get(), "root");

	// set the scene to render
	viewer.setSceneData(group.get());

	// create the windows and run the threads.
	viewer.realize();

	// attach to viewer and set ratio settings
	console->attachToViewer(&viewer);

	viewer.run();

	return 0;
}
