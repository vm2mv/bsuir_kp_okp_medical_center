#pragma once

class Session
{
public:
	virtual ~Session()
	{
		;
	}

	virtual void StartSessionAndWaitExit() = 0;
};