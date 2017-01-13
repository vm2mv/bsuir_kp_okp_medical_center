#include "stdafx.h"
#include "BirthDate.h"


bool operator==(const BirthDate& lhs, const BirthDate& rhs)
{
	return (lhs.GetBirthDateAsInt() == rhs.GetBirthDateAsInt());
};


bool operator!=(const BirthDate& lhs, const BirthDate& rhs)
{
	return (lhs.GetBirthDateAsInt() != rhs.GetBirthDateAsInt());
};


BirthDate::BirthDate() :
	BirthDate(0)
{
}


BirthDate::BirthDate(uint32_t birthDateInt) :
	BirthDate(birthDateInt / 10000, (birthDateInt % 10000) / 100, birthDateInt % 100)
{
}


BirthDate::BirthDate(uint16_t year, uint8_t month, uint8_t day) :
	m_year(year),
	m_month(month),
	m_day(day),
	m_birthDateInt(year * 10000 + month * 100 + day)
{
}


const BirthDate& BirthDate::GetEmptyBirthDate()
{
	static BirthDate emptyBirthDate(0);

	return emptyBirthDate;
}


const BirthDate BirthDate::GetCurrentDate()
{
	auto now = std::chrono::system_clock::now();
	auto inTimeT = std::chrono::system_clock::to_time_t(now);

	tm tmNow;
	
	if (localtime_s(&tmNow, &inTimeT) != 0)
	{
		throw std::runtime_error("localtime_s error");
	}

	return BirthDate(tmNow.tm_year + 1900, tmNow.tm_mon + 1, tmNow.tm_mday);
}


uint32_t BirthDate::GetBirthDateAsInt() const
{
	return m_birthDateInt;
}


uint16_t BirthDate::GetYear() const
{
	return m_year;
}


uint8_t BirthDate::GetMonth() const
{
	return m_month;
}


uint8_t BirthDate::GetDay() const
{
	return m_day;
}
