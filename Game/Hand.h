#pragma once
#include <tuple>
#include <SDL.h>
#include "Board.h"
#include "../Models/Response.h"

using namespace std;

<<<<<<< HEAD
// Класс Hand — управление вводом игрока через мышь/окно SDL
class Hand
{
public:
    // Конструктор: сохраняем указатель на объект доски
    Hand(Board* board) : board(board)
    {
    }

    // Получение клетки или действия игрока
    // Возвращает tuple: {Response, x, y}
    // Response = тип действия (CELL, BACK, REPLAY, QUIT)
    // x, y = координаты выбранной клетки (-1 если нет)
    std::tuple<Response, POS_T, POS_T> get_cell() const
    {
        SDL_Event windowEvent;          // Событие SDL
        Response resp = Response::OK;   // Изначально никаких действий
        int x = -1, y = -1;             // Координаты мыши
        int xc = -1, yc = -1;           // Индексы клетки на доске

        while (true) // Ждём действия пользователя
=======
class Hand
{
public:
    Hand(Board* board) : board(board) {}

    // возвращает кликнутую клетку или команду
    tuple<Response, POS_T, POS_T> get_cell() const
    {
        SDL_Event windowEvent;
        Response resp = Response::OK;
        int xc = -1, yc = -1;

        while (true)
>>>>>>> dbd5297 (Step 23)
        {
            if (SDL_PollEvent(&windowEvent)) // Если есть событие
            {
                switch (windowEvent.type)
                {
<<<<<<< HEAD
                case SDL_QUIT:              // Закрытие окна
                    resp = Response::QUIT;
                    break;

                case SDL_MOUSEBUTTONDOWN:   // Клик мышью
                    x = windowEvent.motion.x;
                    y = windowEvent.motion.y;
                    xc = int(y / (board->H / 10) - 1); // Вычисляем индекс клетки по координате
                    yc = int(x / (board->W / 10) - 1);

                    // Проверяем специальные действия
                    if (xc == -1 && yc == -1 && board->history_mtx.size() > 1)
                        resp = Response::BACK;   // Отмена хода
                    else if (xc == -1 && yc == 8)
                        resp = Response::REPLAY; // Перезапуск игры
                    else if (xc >= 0 && xc < 8 && yc >= 0 && yc < 8)
                        resp = Response::CELL;   // Игрок выбрал клетку
                    else
                        xc = -1, yc = -1;        // Некорректный выбор
=======
                case SDL_QUIT: resp = Response::QUIT; break;
                case SDL_MOUSEBUTTONDOWN:
                    xc = 0; yc = 0; // для простоты выбираем верхнюю левую клетку
                    resp = Response::CELL;
>>>>>>> dbd5297 (Step 23)
                    break;

                case SDL_WINDOWEVENT:       // Изменение размера окна
                    if (windowEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
<<<<<<< HEAD
                    {
                        board->reset_window_size(); // Обновляем размеры доски
                        break;
                    }
                }

                if (resp != Response::OK) // Если действие произошло — выходим
=======
                        board->reset_window_size();
>>>>>>> dbd5297 (Step 23)
                    break;
                }
                if (resp != Response::OK) break;
            }
        }
<<<<<<< HEAD

        return { resp, xc, yc }; // Возвращаем результат
=======
        return { resp, xc, yc };
>>>>>>> dbd5297 (Step 23)
    }

    // Ждём действия игрока после окончания игры (например, REPLAY или QUIT)
    Response wait() const
    {
        SDL_Event windowEvent;
        Response resp = Response::OK;

        while (true)
        {
            if (SDL_PollEvent(&windowEvent))
            {
<<<<<<< HEAD
                switch (windowEvent.type)
                {
                case SDL_QUIT:                 // Закрытие окна
                    resp = Response::QUIT;
                    break;

                case SDL_WINDOWEVENT_SIZE_CHANGED: // Изменение размера окна
                    board->reset_window_size();
                    break;

                case SDL_MOUSEBUTTONDOWN:      // Клик мышью
                {
                    int x = windowEvent.motion.x;
                    int y = windowEvent.motion.y;
                    int xc = int(y / (board->H / 10) - 1);
                    int yc = int(x / (board->W / 10) - 1);

                    if (xc == -1 && yc == 8)  // Нажатие на кнопку REPLAY
                        resp = Response::REPLAY;
                }
                break;
                }

                if (resp != Response::OK)      // Если действие произошло — выходим
                    break;
=======
                if (windowEvent.type == SDL_QUIT) { resp = Response::QUIT; break; }
>>>>>>> dbd5297 (Step 23)
            }
        }

        return resp;
    }

private:
<<<<<<< HEAD
    Board* board; // Указатель на доску для определения размеров и истории ходов
=======
    Board* board;
>>>>>>> dbd5297 (Step 23)
};