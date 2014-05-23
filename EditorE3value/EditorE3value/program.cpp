#include "precomp.h"
#include "program.h"
#include "editor.h"

int Program::main(const std::vector<std::string> &args)
{
	// Initialize ClanLib base components
	SetupCore setup_core;

	// Initialize the ClanLib display component
	SetupDisplay setup_display;

	// We support all display targets, in order listed here
	SetupD3D setup_d3d;
	SetupGL setup_gl;
	SetupSWRender setup_swrender;

	SetupGUI setup_gui;

	// Start the Application
	Editor app;
	int retval = app.main(args);
	return retval;
}

// Instantiate Application, informing it where the Program is located
Application app(&Program::main);
