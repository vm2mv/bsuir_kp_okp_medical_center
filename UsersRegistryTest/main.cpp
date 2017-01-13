#include "stdafx.h"


int main(int argc, wchar_t* argv[])
{
	::testing::InitGoogleTest(&argc, argv);

	int result = RUN_ALL_TESTS();

#ifdef _DEBUG
	getchar();
#endif

	return result;
}

