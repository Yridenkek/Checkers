#pragma once
#include <tuple>

#include "../Models/Move.h"
#include "../Models/Response.h"
#include "Board.h"

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
        {
            if (SDL_PollEvent(&windowEvent)) // Если есть событие
            {
                switch (windowEvent.type)
                {
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
                    break;

                case SDL_WINDOWEVENT:       // Изменение размера окна
                    if (windowEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                    {
                        board->reset_window_size(); // Обновляем размеры доски
                        break;
                    }
                }

                if (resp != Response::OK) // Если действие произошло — выходим
                    break;
            }
        }

        return { resp, xc, yc }; // Возвращаем результат
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
            }
        }

        return resp;
    }

private:
    Board* board; // Указатель на доску для определения размеров и истории ходов
};