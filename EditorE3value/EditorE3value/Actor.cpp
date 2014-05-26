#include "Actor.h"

Actor::Actor(std::string imgPath,GUIComponent* win,GUIComponent* propwin,Label** labelPointer,int x, int y)
{
	img = new clan::Image(win->get_canvas(), "Resources/actor.png");
	//isMoving = true;
	isMoving = false;

	imgActor = new clan::ImageView(win);
	imgActor->set_image(*img);
	imgActor->set_geometry(clan::Rect(x,y,Size(img->get_width(),img->get_height())));
	imgActor->func_input_pressed().set(this,&Actor::actor_clicked);
	imgActor->func_input_released().set(this,&Actor::actor_released);

	imgActor->func_input_pointer_moved().set(this,&Actor::Update);

	propertiesWindow = propwin;
	
	label = new clan::Label(imgActor);
	label->set_geometry(clan::Rect(img->get_width()/3,img->get_height()/3,Size(100,30)));
	label->set_text("Actor");

	LabelPointer = labelPointer;
}

void Actor::Init(std::string imgPath,GUIComponent* win,GUIComponent* propwin,Label** labelPointer,int x, int y)
{
	img = new clan::Image(win->get_canvas(), "Resources/actor.png");
	//isMoving = true;
	isMoving = false;

	imgActor = new clan::ImageView(win);
	imgActor->set_image(*img);
	imgActor->set_geometry(clan::Rect(x,y,Size(img->get_width(),img->get_height())));
	imgActor->func_input_pressed().set(this,&Actor::actor_clicked);
	imgActor->func_input_released().set(this,&Actor::actor_released);

	imgActor->func_input_pointer_moved().set(this,&Actor::Update);

	propertiesWindow = propwin;
	
	label = new clan::Label(imgActor);
	label->set_geometry(clan::Rect(img->get_width()/3,img->get_height()/3,Size(100,30)));
	label->set_text("Actor");

	LabelPointer = labelPointer;
}

void Actor::setText(std::string string)
{
	label->set_text(string);
}

void Actor::MoveTo(int x, int y)
{
	imgActor->set_geometry(clan::Rect(Point(x,y),Size(img->get_width(),img->get_height())));
}

bool Actor::actor_clicked(const InputEvent &input_event)
{
	if(input_event.id == mouse_left)
	{prevX = input_event.mouse_pos.x;
	prevY = input_event.mouse_pos.y;
	isMoving = true;}
	if(input_event.id == mouse_right)
	{
		propertiesWindow->set_visible(true);
		*LabelPointer = label;
	}
	return true;
}

bool Actor::actor_released(const InputEvent &input_event)
{
	isMoving = false;
	return true;
}

bool Actor::Update(const InputEvent &input_event)
{
	if(isMoving){
	Point position = input_event.mouse_pos;
	
	int x = (position.x - prevX)+imgActor->get_geometry().left;
	int y = (position.y - prevY)+imgActor->get_geometry().top;
	
	MoveTo(x,y);
	}
	return true;
}