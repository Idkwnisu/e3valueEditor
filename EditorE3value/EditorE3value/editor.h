
#pragma once

class Editor
{
public:
	int main(const std::vector<std::string> &args);

private:
	bool on_window_close(GUIComponent *gui);
	void on_button_ok_clicked(PushButton *button);
	void on_button_delete_clicked(PushButton *button);
	void on_window_draw(Canvas &canvas, const clan::Rect &clip_rect);
	void on_right_mouse_click(const InputEvent &key);
	bool actor_create(const InputEvent &input_event); 


	void delete_twin_links(int link);
	void delete_all_actor_links(int actor);

	std::string intToString(int value);
};