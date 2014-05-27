#include <sstream>


#include "precomp.h"
#include "editor.h"
#include "Actor.h"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define FIRST_ACTOR_X 70
#define FIRST_ACTOR_Y 115
#define SECOND_ACTOR_X 110
#define SECOND_ACTOR_Y 115
#define FONT_SIZE 18

	std::vector<Actor*> actorList;

	std::vector<clan::LineSegment2f> linkList;
	std::vector<std::string> values; //stringhe di valori dei collegamenti
	std::vector<clan::Pointf> actorsLinked; //attori collegati (in modo da spostare i collegamenti)

	clan::GUIComponent *window;
	clan::GUIComponent* popupwindow;
	clan::LineEdit* lineedit;

	clan::InputDevice device;

	clan::Font *font;

	clan::Spin* spin;

	int valueToChange = -1; //valore che indica la label dei collegamenti da cambiare, se è -1 vuol dire che non è stato chiamato da nessun testo per cui
								//deve cambiare il valore di un attore

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
	window->func_render().set(this, &Editor::on_window_draw);

	window->set_constant_repaint(true);

	InputContext ic = window->get_ic();
	device = ic.get_mouse();
	Slot slot = device.sig_key_up().connect(this, &Editor::on_right_mouse_click);


	GUIManager gui2(theme);
	clan::DisplayWindowDescription popup_desc;
	popup_desc.set_allow_resize(false);
	popup_desc.set_title("Properties");
	popup_desc.set_position(clan::Rect(100, 100, Size(300,100)), false);

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

	clan::PushButton* DeleteButton = new clan::PushButton(popupwindow);
	DeleteButton->set_geometry(clan::Rect(200,40,clan::Size(70,30)));
	DeleteButton->set_text("Delete");
	DeleteButton->func_clicked().set(this, &Editor::on_button_delete_clicked, DeleteButton);


	toMod = new Label(window);

	spin = new clan::Spin(popupwindow);
	spin->set_geometry(clan::Rect(100,40,Size(80,20)));



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



	font = new clan::Font(window->get_canvas(),"tahoma",FONT_SIZE);




	gui.exec();

	return 0;
}

bool Editor::on_window_close(clan::GUIComponent *gui)
{
	gui->exit_with_code(0);
	return true;
}

void Editor::on_button_delete_clicked(clan::PushButton *button)
{
	if(valueToChange < 0)
	{
		int Actors = actorList.size();
		for(int z = 0; z<Actors; z++)
		{
			if(toMod == actorList.at(z)->label) //controlla l'attore interessato tramite l'indirizzo della sua label
			{
				actorList.at(z)->imgActor->~ImageView();
				actorList.erase(actorList.begin()+z);
				delete_all_actor_links(z);
				z = 0;				//z azzerato e numero di attori aggiornati, altrimenti togliendo attori dalla lista si va oltre la dimensione
				Actors = actorList.size();
			}
		}
	}
	else
	{
		delete_twin_links(valueToChange);
		valueToChange = -1;
	}
	popupwindow->set_visible(false);
}

void Editor::on_button_ok_clicked(clan::PushButton *button)
{
	if(valueToChange < 0)
	{
		toMod->set_text(lineedit->get_text());
		for(int z = 0; z<actorList.size(); z++)
		{
			if(toMod == actorList.at(z)->label) //controlla l'attore interessato tramite l'indirizzo della sua label
			{
				if(spin->get_value() > 0)
				actorList.at(z)->setMult(spin->get_value());
			}
		}
	}
	else
		{
			values.at(valueToChange) = lineedit->get_text();
			valueToChange = -1;
			spin->set_visible(true);
		}
	popupwindow->set_visible(false);
}

void Editor::on_window_draw(Canvas &canvas, const clan::Rect &clip_rect)
{
	for(int z = 0; z < actorList.size(); z++)
	{
		font->draw_text(canvas,actorList.at(z)->imgActor->get_geometry().get_top_left().x,actorList.at(z)->imgActor->get_geometry().get_top_left().y,intToString(actorList.at(z)->getMult()),Colorf::black);//disegna la molteplicità degli attori
	}
	for(int z = 0; z < linkList.size(); z++)
	{
		canvas.draw_line(linkList.at(z),Colorf::black);
		clan::LineSegment2f* line = new LineSegment2f(linkList.at(z));
		if(z % 2 == 0) //pari, vuol dire che è il segmento di andata
			line->p.x -= values.at(z).length();  //dimensione della stringa / dimensione carattere 

		line->p.y += 20;
		font->draw_text(canvas,line->p,values.at(z),clan::Colorf(0,0,0));
	}
	for(int z = 0; z < actorsLinked.size(); z = z + 2) //assumiamo che siano pari perché esistono per forza due collegamenti per ogni attore
	{
		linkList.at(z).p.x = actorList.at(actorsLinked.at(z).x)->imgActor->get_geometry().get_top_left().x + FIRST_ACTOR_X; //assegna ai collegamenti la posizione degli attori
		linkList.at(z).p.y = actorList.at(actorsLinked.at(z).x)->imgActor->get_geometry().get_top_left().y + FIRST_ACTOR_Y;

		linkList.at(z).q.x = actorList.at(actorsLinked.at(z).y)->imgActor->get_geometry().get_top_left().x + FIRST_ACTOR_X;
		linkList.at(z).q.y = actorList.at(actorsLinked.at(z).y)->imgActor->get_geometry().get_top_left().y + FIRST_ACTOR_Y;


		linkList.at(z+1).p.x = actorList.at(actorsLinked.at(z+1).x)->imgActor->get_geometry().get_top_left().x + SECOND_ACTOR_X; //assegna ai collegamenti la posizione degli attori
		linkList.at(z+1).p.y = actorList.at(actorsLinked.at(z+1).x)->imgActor->get_geometry().get_top_left().y + SECOND_ACTOR_Y;

		linkList.at(z+1).q.x = actorList.at(actorsLinked.at(z+1).y)->imgActor->get_geometry().get_top_left().x + SECOND_ACTOR_X;
		linkList.at(z+1).q.y = actorList.at(actorsLinked.at(z+1).y)->imgActor->get_geometry().get_top_left().y + SECOND_ACTOR_Y;
	}
	for(int z = 0; z < actorList.size(); z++)
	{
		if(actorList.at(z)->isToConnect)//un attore da collegare
		{
			for(int y = z+1; y < actorList.size(); y++)
			{
				if(actorList.at(y)->isToConnect)//due attori da collegare
				{
					linkList.push_back(*(new LineSegment2f(clan::Vec2f(actorList.at(z)->imgActor->get_geometry().get_top_left().x,actorList.at(z)->imgActor->get_geometry().get_top_left().y),Vec2f(actorList.at(y)->imgActor->get_geometry().get_top_left().x,actorList.at(y)->imgActor->get_geometry().get_top_left().y))));
					values.push_back("value");
					actorsLinked.push_back(clan::Pointf(z,y));
					

					linkList.push_back(*(new LineSegment2f(clan::Vec2f(actorList.at(y)->imgActor->get_geometry().get_top_left().x,actorList.at(y)->imgActor->get_geometry().get_top_left().y),Vec2f(actorList.at(z)->imgActor->get_geometry().get_top_left().x,actorList.at(z)->imgActor->get_geometry().get_top_left().y))));
					values.push_back("value");
					actorsLinked.push_back(clan::Pointf(y,z));


					actorList.at(z)->setConnecting(false);
					actorList.at(y)->setConnecting(false);
				}
			}
		}
	}
}

void Editor::on_right_mouse_click(const InputEvent &key)
{
	if(key.id == mouse_right)
	{
		Pointf p = key.mouse_pos;
		for(int z = 0; z < values.size(); z++)
		{
			clan::Rect* valueRect;
			if(z%2 == 0)
				valueRect = new clan::Rect(linkList.at(z).p.x - 10,linkList.at(z).p.y - 10,linkList.at(z).p.x + values.at(z).length()*10,linkList.at(z).p.y + 30);
			else
				valueRect = new clan::Rect(linkList.at(z).p.x - 10,linkList.at(z).p.y - 10,linkList.at(z).p.x + values.at(z).length()*10,linkList.at(z).p.y + 30);
			if(valueRect->contains(p))
			{
				popupwindow->set_visible(true);//controlla che il click sia avvenuto sul testo
				spin->set_visible(false);
				valueToChange = z;
			}
		}
	}
}

bool Editor::actor_create(const InputEvent &input_event)
{
	actorList.push_back(new Actor("Resources/actor.png",window,popupwindow,&toMod,0,500));
	return true;
}


void Editor::delete_all_actor_links(int actor)
{
	
	for(int z = 0; z < actorsLinked.size(); z++)
	{
		if(actorsLinked.at(z).x == actor || actorsLinked.at(z).y == actor)
		{
			delete_twin_links(z);
			z = 0;
		}

	}
}

void Editor::delete_twin_links(int links)
{
	int other;
		if(links%2 == 0) //numero pari, dunque il collegamento gemello è il successivo
		{
			other = links+1;

			values.erase(values.begin()+links,values.begin()+other+1);
			linkList.erase(linkList.begin()+links,linkList.begin()+other+1);
			actorsLinked.erase(actorsLinked.begin()+links,actorsLinked.begin()+other+1);
		}
		else //numero dispari, il collegamento gemello è il precedente
		{ 
			other = links-1;
			
			values.erase(values.begin()+other,values.begin()+links+1);
			linkList.erase(linkList.begin()+other,linkList.begin()+links+1);
			actorsLinked.erase(actorsLinked.begin()+other,actorsLinked.begin()+links+1);
		}
}

std::string Editor::intToString(int value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}