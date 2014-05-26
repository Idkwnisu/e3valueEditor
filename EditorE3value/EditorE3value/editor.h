
#pragma once

class Editor
{
public:
	int main(const std::vector<std::string> &args);

private:
	bool on_window_close(GUIComponent *gui);
	void on_button_ok_clicked(PushButton *button);
	bool actor_create(const InputEvent &input_event); 
};