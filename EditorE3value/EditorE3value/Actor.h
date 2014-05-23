#include "precomp.h"
#pragma once

class Actor{
public : Actor();
		 void Init(std::string imgPath,GUIComponent* win,int x, int y);
		 void MoveTo(int x,int y);
		 clan::Image* img;
		 clan::ImageView* imgActor;

		 int prevX;
		 int prevY;
		 bool isMoving;
		 
		 bool Update(const InputEvent &input_event);
		 bool actor_clicked(const InputEvent &input_event);
		 bool actor_released(const InputEvent &input_event);
};