#include "precomp.h"
#pragma once

class Actor{
public : Actor(std::string imgPath,GUIComponent* win,GUIComponent* propwin,Label** labelPointer,int x, int y);
		 void Init(std::string imgPath,GUIComponent* win,GUIComponent* propwin,Label** labelPointer,int x, int y);
		 void MoveTo(int x,int y);
		 clan::Image* img;
		 clan::ImageView* imgActor;
		 clan::Label* label;
		 clan::Label** LabelPointer;
		 clan::GUIComponent* propertiesWindow;

		 bool isToConnect;
		 
		 int prevX;
		 int prevY;
		 bool isMoving;
		 
		 void setConnecting(bool flag);
		 void setText(std::string string);

		 bool Update(const InputEvent &input_event);
		 bool actor_clicked(const InputEvent &input_event);
		 bool actor_released(const InputEvent &input_event);
};