#pragma once

class Editor
{
public:
	int main(const std::vector<std::string> &args);

private:
	bool on_window_close(GUIComponent *gui);
};