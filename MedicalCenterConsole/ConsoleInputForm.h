#pragma once

class Console;
class Language;


class ConsoleInputForm
{
public:
	using FieldName = std::string;
	using FieldDisplayText = std::string;
	using FieldCheckValuePredicat = std::function<bool(const FieldName& fieldName,
		const std::string& fieldValue, std::string& fieldValueFormatDescription)>;

	struct InputFieldItem
	{
		FieldName name;
		bool isPassword;
		FieldDisplayText display;
		FieldCheckValuePredicat predicat;
	};

	using InputFields = std::vector<InputFieldItem>;
	using InputFieldsValues = std::map<FieldName, std::string>;

public:
	explicit ConsoleInputForm(Console& console, const Language& language, const InputFields& inputFields);

	void DoInputFormFields();
	const InputFieldsValues& GetInputFieldsValues() const;

private:
	void DoInputFormField(const InputFieldItem& fieldItem);
	bool CheckFieldValue(const FieldCheckValuePredicat& predicat, const std::string& fieldName, 
		const std::string& fieldValue);

private:
	const Language& m_language;
	Console& m_console;
	const InputFields m_inputFields;
	InputFieldsValues m_fieldsValues;
};

