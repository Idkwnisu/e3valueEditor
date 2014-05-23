#include "precomp.h"
#include "editor.h"
#include "Actor.h"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600


int Editor::main(const std::vector<std::string> &args)
{

	std::string theme;
	if (clan::FileHelp::file_exists("Resources/GUIThemeAero/theme.css"))
		theme = "Resources/GUIThemeAero";
	else if (clan::FileHelp::file_exists("Resources/GUIThemeBasic/theme.css"))
		theme = "Resources/GUIThemeBasic";
	else
		throw clan::Exception("Not themes found");


	GUIManager gui(theme);
	clan::DisplayWindowDescription win_desc;
	win_desc.set_allow_resize(true);
	win_desc.set_title("Editor");
	win_desc.set_position(clan::Rect(100, 100, Size(SCREEN_WIDTH,SCREEN_HEIGHT)), false);

	clan::GUIComponent window(&gui, win_desc, "Window");
	window.func_close().set(this, &Editor::on_window_close, &window);

	clan::MenuBar mb(&window);
	mb.set_geometry(clan::Rect(0, 0, Size(SCREEN_WIDTH,30)));
	clan::PopupMenu ppMenu;
	ppMenu.insert_item("New");
	ppMenu.insert_item("Open");
	ppMenu.insert_item("Close");
	ppMenu.set_minimum_width(100);
	mb.add_menu("File",ppMenu);
	clan::Image actor(window.get_canvas(), "Resources/actor.png");
	clan::ImageView actorCreator(&window);
	actorCreator.set_image(actor);
	actorCreator.set_geometry(clan::Rect(0,SCREEN_HEIGHT-actor.get_height()-30,Size(actor.get_width(),actor.get_height())));
	gui.exec();

	return 0;
}

bool Editor::on_window_close(clan::GUIComponent *gui)
{
	gui->exit_with_code(0);
	return true;
}
