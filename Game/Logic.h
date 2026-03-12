#pragma once
#include <random>
#include <vector>

#include "../Models/Move.h"
#include "Board.h"
#include "Config.h"

const int INF = 1e9;

class Logic
{
public:
    Logic(Board* board, Config* config) : board(board), config(config)
    {
        rand_eng = std::default_random_engine(
            !((*config)("Bot", "NoRandom")) ? unsigned(time(0)) : 0);
        scoring_mode = (*config)("Bot", "BotScoringType");
        optimization = (*config)("Bot", "Optimization");
    }

    // Метод для поиска наилучших ходов бота
    // Реализация удалена для самостоятельной работы на шаге 22
    vector<move_pos> find_best_turns(const bool color);

private:
    // Вспомогательная функция для рекурсивного поиска лучшего первого хода
    // Реализация удалена для самостоятельной работы на шаге 22
    double find_first_best_turn(vector<vector<POS_T>> mtx, const bool color, const POS_T x, const POS_T y, size_t state,
        double alpha = -1);

    // Рекурсивная функция минимакс с альфа-бета отсечением
    // Реализация удалена для самостоятельной работы на шаге 22
    double find_best_turns_rec(vector<vector<POS_T>> mtx, const bool color, const size_t depth, double alpha = -1,
        double beta = INF + 1, const POS_T x = -1, const POS_T y = -1);

public:
    // Поиск всех возможных ходов для цвета color
    void find_turns(const bool color)
    {
        find_turns(color, board->get_board());
    }

    // Поиск ходов с конкретной клетки
    void find_turns(const POS_T x, const POS_T y)
    {
        find_turns(x, y, board->get_board());
    }

private:
    // Основная логика поиска ходов по цвету
    void find_turns(const bool color, const std::vector<std::vector<POS_T>>& mtx);
    void find_turns(const POS_T x, const POS_T y, const std::vector<std::vector<POS_T>>& mtx);

public:
    std::vector<move_pos> turns; // Список доступных ходов
    bool have_beats;             // Есть ли обязательные "бьющие" ходы
    int Max_depth;               // Максимальная глубина поиска

private:
    std::default_random_engine rand_eng;
    std::string scoring_mode;
    std::string optimization;
    std::vector<move_pos> next_move;
    std::vector<int> next_best_state;
    Board* board;
    Config* config;
};