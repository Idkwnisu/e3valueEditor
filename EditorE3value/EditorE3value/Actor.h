#include "precomp.h"
#pragma once

class Actor{
public : Actor(std::string imgPath,GUIComponent* win,GUIComponent* propwin,int x, int y);
		 void Init(std::string imgPath,GUIComponent* win,GUIComponent* propwin,int x, int y);
		 void MoveTo(int x,int y);
		 clan::Image* img;
		 clan::ImageView* imgActor;
		 clan::Label* label;
		 clan::GUIComponent* propertiesWindow;


		 int prevX;
		 int prevY;
		 bool isMoving;
		 
		 void setText(std::string string);

		 bool Update(const InputEvent &input_event);
		 bool actor_clicked(const InputEvent &input_event);
		 bool actor_released(const InputEvent &input_event);
};