#pragma once


class BirthDate
{
public:
	BirthDate();
	explicit BirthDate(uint16_t year, uint8_t month, uint8_t day);
	explicit BirthDate(uint32_t birthDateInt);

	static const BirthDate& GetEmptyBirthDate();
	static const BirthDate GetCurrentDate();

	uint32_t GetBirthDateAsInt() const;

	uint16_t GetYear() const;
	uint8_t GetMonth() const;
	uint8_t GetDay() const;

private:
	uint16_t m_year;
	uint8_t m_month;
	uint8_t m_day;

	// date in integer format (YYYYMMDD)
	uint32_t m_birthDateInt;
};


bool operator==(const BirthDate& lhs, const BirthDate& rhs);
bool operator!=(const BirthDate& lhs, const BirthDate& rhs);
