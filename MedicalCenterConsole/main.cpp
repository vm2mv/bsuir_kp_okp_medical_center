#include "stdafx.h"
#include "Console.h"
#include "Application.h"

int main()
{
	// set utf-8 locale for console and boost filesystem
	std::locale::global(boost::locale::generator().generate(""));
	boost::filesystem::path::imbue(std::locale());

	::SetConsoleCP(1251);
	::SetConsoleOutputCP(CP_UTF8);

	try
	{
		Application app;
		app.StartAndWaitExit();
	}
	catch (const std::exception& ex)
	{
		printf("\r\n\t Internal exception: %s", ex.what());
	}

	return 0;
}

