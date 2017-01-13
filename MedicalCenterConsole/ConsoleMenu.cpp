#include "stdafx.h"
#include "ConsoleMenu.h"
#include "Console.h"
#include "Language.h"


ConsoleMenu::ConsoleMenu(Console& console, const Language& language) :
	m_language(language),
	m_console(console)
{
}


ConsoleMenu::~ConsoleMenu()
{
}


void ConsoleMenu::AddItem(const std::string& itemName, std::function<void()> action)
{
	size_t menuItemNumer = m_items.size() + 1;
	m_items[menuItemNumer] = MenuItem{ itemName, action };
}


void ConsoleMenu::AddMenu(const std::string& itemName, const ConsoleMenu menu)
{
	std::function<void()> actionFunc = [itemName, menu]()
	{
		menu.StartMenuDialog(true, itemName);
	};

	m_items[m_items.size() + 1] = MenuItem{ itemName, actionFunc };
}


void ConsoleMenu::StartMenuAndWaitExit() const
{
	StartMenuDialog(false, "");
}


void ConsoleMenu::StartMenuDialog(bool childMenu, const std::string& itemName) const
{ 
	while (true)
	{
		if (!DoMenuDialog(childMenu, itemName))
		{
			break;
		}
	}
}


bool ConsoleMenu::DoMenuDialog(bool childMenu, const std::string& itemName) const
{
	PrintMenu(childMenu, itemName);

	m_console.PrintWhite(GetLanguage().GetString(LanguageString::MenuSelectItem));

	uint64_t selectedMenuItemNum = m_console.RequestInputInteger();

	if (selectedMenuItemNum == 0)
	{
		return false;
	}

	auto it = m_items.find(selectedMenuItemNum);

	if (it == m_items.cend())
	{
		m_console.PrintErrorWithNewLine(GetLanguage().GetString(LanguageString::IncorrectValue));

		return true;
	}

	CallAction(it->second.action);

	return true;
}


void ConsoleMenu::PrintMenu(bool childMenu, const std::string& itemName) const
{
	if (childMenu)
	{
		m_console.PrintWhite(u8"%s\r\n", itemName.c_str());
	}

	for (const auto& i : m_items)
	{
		m_console.PrintInfo(u8"%zu. %s\r\n", i.first, i.second.name.c_str());
	}

	if (childMenu)
	{
		m_console.PrintInfoWithNewLine(GetLanguage().GetString(LanguageString::MenuReturnToParent));
	}
	else
	{
		m_console.PrintInfoWithNewLine(GetLanguage().GetString(LanguageString::MenuExit));
	}
}


void ConsoleMenu::CallAction(const std::function<void()>& actionCallback) const
{
	if (actionCallback == nullptr)
	{
		return;
	}

	try
	{
		actionCallback();
	}
	catch (const std::exception& ex)
	{
		m_console.PrintErrorWithNewLine(GetLanguage().GetString(LanguageString::UnknowError));
		m_console.PrintErrorWithNewLine("\t %s", ex.what());
	}
}


const Language& ConsoleMenu::GetLanguage() const
{
	return m_language;
}
