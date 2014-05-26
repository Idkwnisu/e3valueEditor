#include "precomp.h"
#include "editor.h"
#include "Actor.h"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

	std::vector<Actor> actorList;
	clan::GUIComponent *window;
	clan::GUIComponent* popupwindow;
	clan::LineEdit* lineedit;

	static clan::Label* toMod; //puntatore alla label dell'attore da modificare

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
	win_desc.set_allow_resize(false);
	win_desc.set_title("Editor");
	win_desc.set_position(clan::Rect(100, 100, Size(SCREEN_WIDTH,SCREEN_HEIGHT)), false);

	window= new clan::GUIComponent (&gui, win_desc, "Window");
	window->func_close().set(this, &Editor::on_window_close, window);

	GUIManager gui2(theme);
	clan::DisplayWindowDescription popup_desc;
	popup_desc.set_allow_resize(false);
	popup_desc.set_title("Properties");
	popup_desc.set_position(clan::Rect(100, 100, Size(200,100)), false);

	popupwindow = new clan::GUIComponent (&gui2, popup_desc, "Window");
	popupwindow->set_visible(false);

	clan::Label* label = new clan::Label(popupwindow);
	label->set_geometry(clan::Rect(0,0,clan::Size(40,20)));
	label->set_text("Nome");

	lineedit = new clan::LineEdit(popupwindow);
	lineedit->set_geometry(clan::Rect(40, 0, clan::Size(100, 30)));
	lineedit->set_text("LineEdit");

	clan::PushButton* OKButton = new clan::PushButton(popupwindow);
	OKButton->set_geometry(clan::Rect(10,40,clan::Size(70,30)));
	OKButton->set_text("OK");
	OKButton->func_clicked().set(this, &Editor::on_button_ok_clicked, OKButton);

	toMod = new Label(window);

	clan::MenuBar mb(window);
	mb.set_geometry(clan::Rect(0, 0, Size(SCREEN_WIDTH,30)));
	clan::PopupMenu ppMenu;
	ppMenu.insert_item("New");
	ppMenu.insert_item("Open");
	ppMenu.insert_item("Close");
	ppMenu.set_minimum_width(100);
	mb.add_menu("File",ppMenu);


	clan::Image actor(window->get_canvas(), "Resources/actor.png");

	clan::ImageView actorCreator(window);
	actorCreator.set_image(actor);
	actorCreator.set_geometry(clan::Rect(0,SCREEN_HEIGHT-actor.get_height(),Size(actor.get_width()*0.75,actor.get_height()*0.75)));
	actorCreator.func_input_pressed().set(this,&Editor::actor_create);

	clan::Label newLabel(&actorCreator);
	newLabel.set_geometry(clan::Rect(actor.get_width()*0.25,actor.get_height()*0.25,Size(100,20)));//0.25 -> circa metà dell'immagine ridimensionata
	newLabel.set_text("New Actor");


	gui.exec();

	return 0;
}

bool Editor::on_window_close(clan::GUIComponent *gui)
{
	gui->exit_with_code(0);
	return true;
}

void Editor::on_button_ok_clicked(clan::PushButton *button)
{
	toMod->set_text(lineedit->get_text());
	popupwindow->set_visible(false);
}

bool Editor::actor_create(const InputEvent &input_event)
{
	actorList.push_back(*new Actor("Resources/actor.png",window,popupwindow,&toMod,0,500));
	return true;
}