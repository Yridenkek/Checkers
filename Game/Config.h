#pragma once
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "../Models/Project_path.h"

// Класс Config отвечает за загрузку и хранение настроек игры из файла settings.json
class Config
{
public:
    Config()
    {
        reload(); // при создании объекта автоматически загружаем настройки
    }

    // Функция reload() загружает настройки из файла settings.json.
    // Она открывает файл конфигурации, считывает JSON данные
    // и сохраняет их во внутреннюю переменную config.
    // Используется для обновления настроек без перезапуска программы.
    void reload()
    {
        std::ifstream fin(project_path + "settings.json");
        fin >> config;
        fin.close();
    }

    // Перегрузка оператора () позволяет получать значение настройки
    // из JSON конфигурации по имени раздела и имени параметра.
    // Например: config("Bot", "BlackBotLevel")
    // Такой подход упрощает обращение к параметрам настроек программы.
    auto operator()(const string& setting_dir, const string& setting_name) const
    {
        return config[setting_dir][setting_name];
    }

private:
    json config; // объект JSON, в котором хранятся все настройки из settings.json
};