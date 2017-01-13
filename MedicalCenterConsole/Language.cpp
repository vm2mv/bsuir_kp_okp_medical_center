#include "stdafx.h"
#include "Language.h"


const Language::LanguageStringsMap_t Language::DEFAULT_LANGUAGE_STRINGS = {
	{ 
		LanguageString::Welcom,
		u8"* Программа учета сведений о пациентах медицинского центра.\r\n"
		u8"\r\n"
		u8"* ИИТ БГУИР. Факультет Компьютерных Технологий.\r\n"
		u8"* Специальность \"Инженерно-психологическое обеспечение информационных технологий\" (ИПОИТ).\r\n"
		u8"* Курсовой проект по дисциплине \"Основы конструирования программ\".\r\n"
		u8"* Выполнил: Студент 1го курса группы 680971, Микулич Василий Александрович.\r\n"
		u8"* Руководитель: Меженная Марина Михайловна\r\n"
		u8"\r\n"
	},
	{
		LanguageString::UnknowError,
		u8"В программе произошла неизвестная ошибка. Обратитесь к разработчику."
	},
	{
		LanguageString::CantOpenUsersDatabase,
		u8"Не удалось открыть базу данных пользователей. Обратитесь к системному администратору."
	},
	{
		LanguageString::LoginFailed,
		u8"Имя пользователя или пароль не верны." 
	},
	{
		LanguageString::LoginSuccess,
		u8"Вход в систему выполнен успешно."
	},
	{ 
		LanguageString::LoginedUserIsAdmin, 
		u8"Вы зашли в систему с правами администратора." 
	},
	{ 
		LanguageString::QuestionRepeatEnterYesNo,
		u8"Повторить ввод?" 
	},
	{ 
		LanguageString::QuestionContinueYesNo,
		u8"Продолжить?" 
	},
	{
		LanguageString::PatientsDatabaseDefaultHasOpenend,
		u8"Открыт файл базы данных пациентов по умолчанию: %s." 
	},
	{ 
		LanguageString::PatientsDatabaseDefaultHasCreatedAndOpenend,
		u8"Создан и открыт файл базы данных пациентов по умолчанию: %s." 
	},
	{
		LanguageString::CantOpenPatientsDatabase,
		u8"Не удалось открыть базу данных. Убедитесь, что Вы указали корректный путь." 
	},
	{ 
		LanguageString::AdminNotFoundInUsersDatabase,
		u8"База данных не содержит учётной записи администратора."
	},
	{
		LanguageString::QuestionCreateFirstAdmin,
		u8"Для продолжения работы необходимо создать учётную запись администратора. Продолжить?"
	},
	{
		LanguageString::CreateNewUsersDatabaseInfo,
		u8"Первый запуск. Будет создана новая база данных пользователей."
	},
	{ 
		LanguageString::CreateAdminInNewUsersDatabaseInfo,
		u8"При создании базы данных будет создана учётная запись администратора." 
	},
	{
		LanguageString::UsersDatabaseCreated,
		u8"База данных пользователей успешно создана."
	},
	{
		LanguageString::AdminCreatedInUsersDatabase,
		u8"Учётная запись администратора успешно добавлена в базу данных пользователей."
	},
	{
		LanguageString::IncorrectValue,
		u8"Вы ввели некорректное значение. Повторите ввод."
	},
	{
		LanguageString::EmptyValueNotAllowed,
		u8"Поле не может быть пустым."
	},
	{
		LanguageString::NewUserFormCaption,
		u8"Введите данные для новой учётной записи."
	},
	{
		LanguageString::UserFormName,
		u8"Имя пользователя (login)"
	},
	{
		LanguageString::UserFormPassword,
		u8"Пароль"
	},
	{
		LanguageString::UserFormConfirmPassword,
		u8"Повторите ввод пароля"
	},
	{
		LanguageString::PasswordsNotEq,
		u8"Пароли не совпадают."
	},
	{
		LanguageString::LoginName,
		u8"Имя пользователя"
	},
	{
		LanguageString::LoginPasword,
		u8"Пароль"
	},
	{
		LanguageString::PatientFormName,
		u8"ФИО пациента"
	},
	{
		LanguageString::PatientFormGender,
		u8"Пол (м/ж/т)"
	},
	{
		LanguageString::PatientFormBirthDate,
		u8"Дата рождения (ДД.ММ.ГГГГ)"
	},
	{
		LanguageString::PatientFormCity,
		u8"Город"
	},
	{
		LanguageString::PatientFormPhone,
		u8"Телефон"
	},
	{
		LanguageString::PatientFormDiagnoses,
		u8"Диагнозы (через запятую)"
	},
	{
		LanguageString::MenuSelectItem,
		u8"Выберите пункт меню: "
	},
	{
		LanguageString::MenuReturnToParent,
		u8"0. Выйти в родительское меню"
	},
	{
		LanguageString::MenuExit,
		u8"0. Выйти из программы"
	},
	{
		LanguageString::EnterPathToPatientsDatabase,
		u8"Введите путь к файлу базы данных пациентов"
	},
	{
		LanguageString::PatientsDatabaseFileNotFound,
		u8"Не удалось найти файл по указаному пути."
	},
	{
		LanguageString::CantCheckExistPatientsDatabase,
		u8"Не удалось проверить существование файла."
	},
	{
		LanguageString::MenuControlUsers,
		u8"Управление учетными записями пользователей"
	},
	{
		LanguageString::MenuControlPatientsDatabasesFiles,
		u8"Управление файлами данных"
	},
	{
		LanguageString::MenuControlData,
		u8"Обработка данных"
	},			
	{
		LanguageString::MenuEditPatientsData,
		u8"Редактирование данных о пациентах"
	},
	{
		LanguageString::MenuProcessPatientsData,
		u8"Обработка данных о пациентах"
	},
	{
		LanguageString::MenuViewNonresidentPatients,
		u8"Показать иногородних пациентов"
	},
	{
		LanguageString::MenuViewPatientsByAgeAndDiagnose,
		u8"Показать пациентов старше X лет с диагнозом Y"
	},
	{
		LanguageString::MenuSearchPatients,
		u8"Поиск пациентов"
	},
	{
		LanguageString::MenuOrderPatients,
		u8"Сортировка пациентов"
	},
	{
		LanguageString::MenuSearchPatientsByName,
		u8"Поиск пациентов по имени"
	},
	{
		LanguageString::MenuSearchPatientsByCity,
		u8"Поиск пациентов по городу"
	},
	{
		LanguageString::MenuSearchPatientsByDiagnose,
		u8"Поиск пациентов по диагнозу"
	},
	{
		LanguageString::MenuSearchPatientsByPhone,
		u8"Поиск пациентов по номеру телефона"
	},
	{
		LanguageString::MenuOrderPatientsByName,
		u8"Сортировка пациентов по имени"
	},
	{
		LanguageString::MenuOrderPatientsByCity,
		u8"Сортировка пациентов по городу"
	},
	{
		LanguageString::MenuOrderPatientsByBirthDate,
		u8"Сортировка пациентов по дате рождения"
	},
	{
		LanguageString::MenuViewAllPatients,
		u8"Просмотр всех пациентов"
	},
	{
		LanguageString::MenuAddPatient,
		u8"Добавить пациента"
	},
	{
		LanguageString::MenuEditPatient,
		u8"Изменить пациента"
	},
	{
		LanguageString::MenuRemovePatient,
		u8"Удалить пациента"
	},
	{
		LanguageString::MenuPatientsFileCreateFile,
		u8"Создать файл данных пациентов"
	},
	{
		LanguageString::MenuPatientsFileOpen,
		u8"Открыть файл данных пациентов"
	},
	{
		LanguageString::MenuPatientsCloseFile,
		u8"Закрыть файл данных пациентов"
	},
	{
		LanguageString::MenuPatientsRemoveFile,
		u8"Удалить файл данных пациентов"
	},
	{
		LanguageString::RemoveFileConfirm,
		u8"Вы действительно хотите удалить файл?"
	},			
	{
		LanguageString::MenuViewAllUsers,
		u8"Просмотр всех учетных записей"
	},
	{
		LanguageString::MenuAddNewUser,
		u8"Добавление новой учётной записи"
	},
	{
		LanguageString::MenuEditUser,
		u8"Редактирование учётной записи"
	},
	{
		LanguageString::MenuRemoveUser,
		u8"Удаление учётной записи"
	},
	{
		LanguageString::FoundRecords,
		u8"Найдено записей: "
	},
	{
		LanguageString::NotFoundAnyRecord,
		u8"Записи не найдены"
	},			
	{
		LanguageString::UserID,
		u8"ID"
	},
	{
		LanguageString::UserName,
		u8"Имя пользователя"
	},
	{
		LanguageString::UserRole,
		u8"Роль"
	},
	{
		LanguageString::UserRoleIsAdmin,
		u8"Администратор"
	},
	{
		LanguageString::UserRoleIsUser,
		u8"Пользователь"
	},
	{
		LanguageString::CreateNewUserAsAdmin,
		u8"Предоставить пользователю права администратора?"
	},
	{
		LanguageString::UserLoginExistAlready,
		u8"Пользователь с таким именем уже существует."
	},
	{
		LanguageString::UsersAdded,
		u8"Новый пользователь успешно добавлен."
	},
	{
		LanguageString::CantAddUser,
		u8"При добавлении пользователя произошла ошибка."
	},
	{
		LanguageString::RemoveUserConfirm,
		u8"Вы действительно хотите удалить этого пользователя?"
	},
	{
		LanguageString::UserRemoved,
		u8"Пользователь успешно удалён."
	},
	{
		LanguageString::CantRemoveUser,
		u8"При удалении пользователя произошла ошибка."
	},
	{
		LanguageString::EnterUserId,
		u8"Введите ID пользователя: "
	},
	{
		LanguageString::UserIdNotFound,
		u8"Пользователя с таким ID не существует."
	},
	{
		LanguageString::UserFound,
		u8"Пользователь найден:"
	},
	{
		LanguageString::EditUserConfirm,
		u8"Вы действительно хотите изменить этого пользователя?"
	},
	{
		LanguageString::EditUserChangeRole,
		u8"Изменить роль пользователя?"
	},
	{
		LanguageString::EditUserAsAdmin,
		u8"Предоставить пользователю права администратора?"
	},
	{
		LanguageString::EditUserChangeName,
		u8"Изменить имя пользователя?"
	},
	{
		LanguageString::EditUserNewName,
		u8"Введите новое имя пользователя (login): "
	},
	{
		LanguageString::EditUserChangePassword,
		u8"Изменить пароль пользователя?"
	},
	{
		LanguageString::EditUserNewPassword,
		u8"Введите новый пароль: "
	},
	{
		LanguageString::EditUserNewPasswordConfirm,
		u8"Повторите ввод нового пароля: "
	},
	{
		LanguageString::UserChanged,
		u8"Пользователь успешно изменён."
	},
	{
		LanguageString::CantChangeUser,
		u8"При изменении пользователя произошла ошибка."
	},
	{
		LanguageString::PatientId,
		u8"ID"
	},
	{
		LanguageString::PatientName,
		u8"Имя"
	},
	{
		LanguageString::PatientGender,
		u8"Пол"
	},
	{
		LanguageString::PatientBirthDate,
		u8"Дата рождения"
	},
	{
		LanguageString::PatientCity,
		u8"Город"
	},
	{
		LanguageString::PatientPhone,
		u8"Телефон"
	},
	{
		LanguageString::PatientDiagnoses,
		u8"Диагнозы"
	},
	{
		LanguageString::GenderMale,
		u8"Мужчина"
	},
	{
		LanguageString::GenderFeemale,
		u8"Женщина"
	},
	{
		LanguageString::GenderShemale,
		u8"Транссексуал"
	},
	{
		LanguageString::PatientAdded,
		u8"Пациент успешно добавлен."
	},
	{
		LanguageString::PatientEditConfirm,
		u8"Вы действительно хотите изменить этого пациента?"
	},
	{
		LanguageString::PatientChanged,
		u8"Пациент успешно изменён."
	},
	{
		LanguageString::CantChangePatient,
		u8"При изменении пациента произошла ошибка."
	},
	{
		LanguageString::PatientRemoveConfirm,
		u8"Вы действительно хотите удалить этого пациента?"
	},
	{
		LanguageString::PatientRemoved,
		u8"Пациент успешно удалён."
	},
	{
		LanguageString::CantRemovePatient,
		u8"При удалении пациента произошла ошибка."
	},
	{
		LanguageString::SearchPatientName,
		u8"Введите имя пациента: "
	},
	{
		LanguageString::SearchPatientCity,
		u8"Введите название города: "
	},
	{
		LanguageString::SearchPatientDiagnose,
		u8"Введите диагноз: "
	},
	{
		LanguageString::SearchPatientPhone,
		u8"Введите номер телефона: "
	},
	{
		LanguageString::SearchPatientAge,
		u8"Введите возраст (полных лет): "
	},
	{
		LanguageString::EnterPatientId,
		u8"Введите ID пациента: "
	},
	{
		LanguageString::PatientIdNotFound,
		u8"Пациента с таким ID не существует."
	},
	{
		LanguageString::PatientFound,
		u8"Пациент найден:"
	},
	{
		LanguageString::PatientsFileExistAlready,
		u8"По указанному пути файл уже существует."
	},
	{
		LanguageString::PatientsFileCreated,
		u8"Файл успешно создан и открыт."
	},
	{
		LanguageString::PatientsFileOpened,
		u8"Файл успешно открыт."
	},
	{
		LanguageString::PatientsFileClosed,
		u8"Файл закрыт."
	},
	{
		LanguageString::CantRemoveOpenedPatientsFile,
		u8"Файл по указанному пути открыт. Закройте его и потворите попытку удаления."
	},
	{
		LanguageString::PatientsFileRemoved,
		u8"Файл успешно удалён."
	},
	{
		LanguageString::CantRemovePatientsFile,
		u8"Не удалось удалить файл."
	},
	{
		LanguageString::NotFoundOpenedPatientsFile,
		u8"Нет открытых файлов с данными о пациентах. Откройте файл и потворите попытку."
	},
};


Language::Language() :
	m_strings(DEFAULT_LANGUAGE_STRINGS)
{
}


const char* Language::GetString(const LanguageString languageStringId) const
{
	const auto it = m_strings.find(languageStringId);

	if (it == m_strings.cend())
	{
		std::string exceptionText("unknown language string id: ");
		exceptionText += std::to_string(static_cast<uint32_t>(languageStringId));

		throw std::invalid_argument(exceptionText);
	}

	return it->second.c_str();
}
