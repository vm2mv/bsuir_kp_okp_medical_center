#pragma once

class Console;
class Language;

class ConsoleMenu
{
	struct MenuItem
	{
		std::string name;
		std::function<void()> action;
	};

public:
	explicit ConsoleMenu(Console& console, const Language& language);
	~ConsoleMenu();

	void AddItem(const std::string& itemName, std::function<void()> action);
	void AddMenu(const std::string& itemName, const ConsoleMenu menu);

	void StartMenuAndWaitExit() const;

private:
	const Language& GetLanguage() const;

	void StartMenuDialog(bool childMenu, const std::string& itemName) const;
	bool DoMenuDialog(bool childMenu, const std::string& itemName) const;
	void PrintMenu(bool childMenu, const std::string& itemName) const;
	void CallAction(const std::function<void()>& actionCallback) const;

private:
	const Language& m_language;
	Console& m_console;
	std::map<size_t, MenuItem> m_items;
};

