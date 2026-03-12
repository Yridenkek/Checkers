#pragma once
#include <chrono>
#include <thread>
#include <fstream>
#include <vector>
#include <tuple>
#include <string>

#include "../Models/Project_path.h"
#include "Board.h"
#include "Config.h"
#include "Hand.h"
#include "Logic.h"

// Класс Game — основной контроллер игры в шашки
class Game
{
public:
    // Конструктор класса Game
    // - Создаёт Board с размерами окна из настроек
    // - Создаёт Hand и Logic для управления игрой
    // - Очищает log.txt для нового сеанса игры
    Game() : board(config("WindowSize", "Width"), config("WindowSize", "Hight")), hand(&board), logic(&board, &config)
    {
        std::ofstream fout(project_path + "log.txt", std::ios_base::trunc);
        fout.close();
    }

    // Основная функция игры
    // Возвращает результат:
    // 0 — ничья или досрочное завершение
    // 1 — победа игрока
    // 2 — победа бота
    int play()
    {
        auto start = std::chrono::steady_clock::now(); // Засекаем время начала игры

        // Если это повторная игра после REPLAY
        if (is_replay)
        {
            logic = Logic(&board, &config); // Пересоздаём объект Logic
            config.reload();                // Перезагружаем настройки
            board.redraw();                 // Перерисовываем доску
        }
        else
        {
            board.start_draw();             // Первоначальная отрисовка доски
        }
        is_replay = false;

        int turn_num = -1;                  // Номер текущего хода
        bool is_quit = false;               // Флаг выхода из игры
        const int Max_turns = config("Game", "MaxNumTurns"); // Максимальное количество ходов

        // Главный игровой цикл
        while (++turn_num < Max_turns)
        {
            beat_series = 0;                 // Сброс серии взятий
            logic.find_turns(turn_num % 2);  // Находим все возможные ходы для текущего игрока
            if (logic.turns.empty())         // Если ходов нет — игра заканчивается
                break;

            // Устанавливаем уровень бота для текущего цвета
            logic.Max_depth = config("Bot", std::string((turn_num % 2) ? "Black" : "White") + std::string("BotLevel"));

            // Если текущий игрок — человек
            if (!config("Bot", std::string("Is") + std::string((turn_num % 2) ? "Black" : "White") + std::string("Bot")))
            {
                auto resp = player_turn(turn_num % 2); // Выполняем ход игрока
                if (resp == Response::QUIT) { is_quit = true; break; }
                else if (resp == Response::REPLAY) { is_replay = true; break; }
                else if (resp == Response::BACK)   // Отмена последнего хода
                {
                    if (config("Bot", std::string("Is") + std::string((1 - turn_num % 2) ? "Black" : "White") + std::string("Bot")) &&
                        !beat_series && board.history_mtx.size() > 2)
                    {
                        board.rollback();
                        --turn_num;
                    }
                    if (!beat_series)
                        --turn_num;

                    board.rollback();
                    --turn_num;
                    beat_series = 0;
                }
            }
            else
            {
                bot_turn(turn_num % 2); // Ход бота
            }
        }

        auto end = std::chrono::steady_clock::now(); // Засекаем время окончания игры

        // Логируем длительность игры
        std::ofstream fout(project_path + "log.txt", std::ios_base::app);
        fout << "Game time: " << (int)std::chrono::duration<double, std::milli>(end - start).count() << " millisec\n";
        fout.close();

        if (is_replay)
            return play();
        if (is_quit)
            return 0;

        int res = 2; // По умолчанию — победа бота
        if (turn_num == Max_turns)
            res = 0;  // Ничья
        else if (turn_num % 2)
            res = 1;  // Победа игрока

        board.show_final(res);           // Отображаем финальный результат
        auto resp = hand.wait();         // Ждём действия пользователя после окончания
        if (resp == Response::REPLAY)
        {
            is_replay = true;
            return play();
        }
        return res;
    }

private:
    // Функция хода бота
    void bot_turn(const bool color)
    {
        auto start = std::chrono::steady_clock::now(); // Засекаем время начала хода бота

        auto delay_ms = config("Bot", "BotDelayMS"); // Задержка между ходами
        std::thread th(SDL_Delay, delay_ms);        // Создаём поток для симметричной задержки
        auto turns = logic.find_best_turns(color);  // Получаем список оптимальных ходов
        th.join();                                  // Ждём завершения задержки
        bool is_first = true;

        // Выполняем все ходы
        for (auto turn : turns)
        {
            if (!is_first)
                SDL_Delay(delay_ms);                // Задержка между ходами
            is_first = false;

            beat_series += (turn.xb != -1);        // Счётчик серии взятий
            board.move_piece(turn, beat_series);   // Двигаем фигуру на доске
        }

        auto end = std::chrono::steady_clock::now(); // Засекаем время окончания хода
        std::ofstream fout(project_path + "log.txt", std::ios_base::app);
        fout << "Bot turn time: " << (int)std::chrono::duration<double, std::milli>(end - start).count() << " millisec\n";
        fout.close();
    }

    // Функция хода игрока
    Response player_turn(const bool color)
    {
        // Подсветка всех возможных клеток начала хода
        std::vector<std::pair<POS_T, POS_T>> cells;
        for (auto turn : logic.turns)
            cells.emplace_back(turn.x, turn.y);
        board.highlight_cells(cells);

        move_pos pos = { -1, -1, -1, -1 }; // Выбранный ход
        POS_T x = -1, y = -1;

        // Первый выбор клетки игроком
        while (true)
        {
            auto resp = hand.get_cell();
            if (std::get<0>(resp) != Response::CELL)
                return std::get<0>(resp); // Если игрок выбрал quit/back/replay

            std::pair<POS_T, POS_T> cell{ std::get<1>(resp), std::get<2>(resp) };
            bool is_correct = false;

            for (auto turn : logic.turns)
            {
                if (turn.x == cell.first && turn.y == cell.second)
                {
                    is_correct = true;
                    break;
                }
                if (turn == move_pos{ x, y, cell.first, cell.second })
                {
                    pos = turn;
                    break;
                }
            }

            if (pos.x != -1)
                break;

            if (!is_correct)
            {
                if (x != -1)
                {
                    board.clear_active();
                    board.clear_highlight();
                    board.highlight_cells(cells);
                }
                x = -1; y = -1;
                continue;
            }

            x = cell.first; y = cell.second;
            board.clear_highlight();
            board.set_active(x, y);

            // Подсветка целей для выбранной фигуры
            std::vector<std::pair<POS_T, POS_T>> cells2;
            for (auto turn : logic.turns)
            {
                if (turn.x == x && turn.y == y)
                    cells2.emplace_back(turn.x2, turn.y2);
            }
            board.highlight_cells(cells2);
        }

        board.clear_highlight();
        board.clear_active();
        board.move_piece(pos, pos.xb != -1); // Первый ход

        if (pos.xb == -1)
            return Response::OK;              // Нет серии взятий

        // Серия взятий
        beat_series = 1;
        while (true)
        {
            logic.find_turns(pos.x2, pos.y2);
            if (!logic.have_beats)
                break;

            std::vector<std::pair<POS_T, POS_T>> cells;
            for (auto turn : logic.turns)
                cells.emplace_back(turn.x2, turn.y2);
            board.highlight_cells(cells);
            board.set_active(pos.x2, pos.y2);

            // Выбор клетки для продолжения взятий
            while (true)
            {
                auto resp = hand.get_cell();
                if (std::get<0>(resp) != Response::CELL)
                    return std::get<0>(resp);

                std::pair<POS_T, POS_T> cell{ std::get<1>(resp), std::get<2>(resp) };
                bool is_correct = false;

                for (auto turn : logic.turns)
                {
                    if (turn.x2 == cell.first && turn.y2 == cell.second)
                    {
                        is_correct = true;
                        pos = turn;
                        break;
                    }
                }
                if (!is_correct) continue;

                board.clear_highlight();
                board.clear_active();
                beat_series += 1;
                board.move_piece(pos, beat_series);
                break;
            }
        }

        return Response::OK;
    }

private:
    Config config;   // Настройки игры
    Board board;     // Игровая доска
    Hand hand;       // Управление вводом игрока
    Logic logic;     // Логика игры
    int beat_series; // Счётчик серии взятий
    bool is_replay = false; // Флаг перезапуска игры
};