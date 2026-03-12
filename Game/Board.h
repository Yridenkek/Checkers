#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include "../Models/Move.h"
#include "../Models/Project_path.h"

<<<<<<< HEAD
#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#else
#include <SDL.h>
#include <SDL_image.h>
#endif

=======
>>>>>>> dbd5297 (Step 23)
using namespace std;

// Класс Board — отвечает за игровую доску, отрисовку и историю ходов
class Board
{
public:
    Board() = default;
<<<<<<< HEAD

    // Конструктор с указанием ширины и высоты окна
    Board(const unsigned int W, const unsigned int H) : W(W), H(H) {}

    // Инициализация и отрисовка начальной доски
=======
    Board(unsigned int W, unsigned int H) : W(W), H(H) {}

>>>>>>> dbd5297 (Step 23)
    int start_draw()
    {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        {
            print_exception("SDL_Init failed");
            return 1;
        }

<<<<<<< HEAD
        // Если размеры окна не заданы — берём размеры рабочего стола
=======
>>>>>>> dbd5297 (Step 23)
        if (W == 0 || H == 0)
        {
            SDL_DisplayMode dm;
            if (SDL_GetDesktopDisplayMode(0, &dm))
            {
                print_exception("SDL_GetDesktopDisplayMode failed");
                return 1;
            }
<<<<<<< HEAD
            W = min(dm.w, dm.h);
            W -= W / 15; // небольшой отступ
            H = W;
        }

        // Создаём окно SDL
        win = SDL_CreateWindow("Checkers", 0, H / 30, W, H, SDL_WINDOW_RESIZABLE);
        if (win == nullptr)
        {
            print_exception("SDL_CreateWindow can't create window");
            return 1;
        }

        // Создаём рендерер для отрисовки
=======
            W = min(dm.w, dm.h) - dm.w / 15;
            H = W;
        }

        win = SDL_CreateWindow("Checkers", 0, H / 30, W, H, SDL_WINDOW_RESIZABLE);
        if (!win) { print_exception("SDL_CreateWindow failed"); return 1; }

>>>>>>> dbd5297 (Step 23)
        ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!ren) { print_exception("SDL_CreateRenderer failed"); return 1; }

        board_tex = IMG_LoadTexture(ren, (project_path + "Textures/board.png").c_str());
        w_piece = IMG_LoadTexture(ren, (project_path + "Textures/piece_white.png").c_str());
        b_piece = IMG_LoadTexture(ren, (project_path + "Textures/piece_black.png").c_str());
        w_queen = IMG_LoadTexture(ren, (project_path + "Textures/queen_white.png").c_str());
        b_queen = IMG_LoadTexture(ren, (project_path + "Textures/queen_black.png").c_str());
        back = IMG_LoadTexture(ren, (project_path + "Textures/back.png").c_str());
        replay = IMG_LoadTexture(ren, (project_path + "Textures/replay.png").c_str());

        if (!board_tex || !w_piece || !b_piece || !w_queen || !b_queen || !back || !replay)
        {
            print_exception("IMG_LoadTexture failed");
            return 1;
        }

<<<<<<< HEAD
        // Загружаем текстуры для доски, фигур, кнопок
        board = IMG_LoadTexture(ren, board_path.c_str());
        w_piece = IMG_LoadTexture(ren, piece_white_path.c_str());
        b_piece = IMG_LoadTexture(ren, piece_black_path.c_str());
        w_queen = IMG_LoadTexture(ren, queen_white_path.c_str());
        b_queen = IMG_LoadTexture(ren, queen_black_path.c_str());
        back = IMG_LoadTexture(ren, back_path.c_str());
        replay = IMG_LoadTexture(ren, replay_path.c_str());

        if (!board || !w_piece || !b_piece || !w_queen || !b_queen || !back || !replay)
        {
            print_exception("IMG_LoadTexture can't load main textures from " + textures_path);
            return 1;
        }

        SDL_GetRendererOutputSize(ren, &W, &H);
        make_start_mtx(); // Создаём стартовую матрицу доски
        rerender();       // Отрисовываем доску
=======
        SDL_GetRendererOutputSize(ren, (int*)&W, (int*)&H);
        make_start_mtx();
        rerender();
>>>>>>> dbd5297 (Step 23)
        return 0;
    }

    // Перерисовка доски и сброс истории
    void redraw()
    {
        game_results = -1;
        history_mtx.clear();
        history_beat_series.clear();
        make_start_mtx();
        clear_active();
        clear_highlight();
    }

<<<<<<< HEAD
    // Перемещение фигуры по структуре move_pos
    void move_piece(move_pos turn, const int beat_series = 0)
    {
        if (turn.xb != -1)
        {
            mtx[turn.xb][turn.yb] = 0; // убираем побитую фигуру
        }
        move_piece(turn.x, turn.y, turn.x2, turn.y2, beat_series);
    }

    // Перемещение фигуры по координатам
    void move_piece(const POS_T i, const POS_T j, const POS_T i2, const POS_T j2, const int beat_series = 0)
    {
        if (mtx[i2][j2]) throw runtime_error("final position is not empty, can't move");
        if (!mtx[i][j]) throw runtime_error("begin position is empty, can't move");

        // Превращение в дамку при достижении противоположного края
        if ((mtx[i][j] == 1 && i2 == 0) || (mtx[i][j] == 2 && i2 == 7))
            mtx[i][j] += 2;

=======
    void move_piece(move_pos turn, int beat_series = 0)
    {
        if (turn.xb != -1) mtx[turn.xb][turn.yb] = 0;
        move_piece(turn.x, turn.y, turn.x2, turn.y2, beat_series);
    }

    void move_piece(POS_T i, POS_T j, POS_T i2, POS_T j2, int beat_series = 0)
    {
        if (mtx[i2][j2]) throw runtime_error("Destination not empty");
        if (!mtx[i][j]) throw runtime_error("Source empty");
        if ((mtx[i][j] == 1 && i2 == 0) || (mtx[i][j] == 2 && i2 == 7)) mtx[i][j] += 2;
>>>>>>> dbd5297 (Step 23)
        mtx[i2][j2] = mtx[i][j];
        drop_piece(i, j);
        add_history(beat_series);
    }

<<<<<<< HEAD
    // Убирает фигуру с клетки
    void drop_piece(const POS_T i, const POS_T j)
    {
        mtx[i][j] = 0;
        rerender();
    }

    // Превращает фигуру в дамку
    void turn_into_queen(const POS_T i, const POS_T j)
    {
        if (mtx[i][j] == 0 || mtx[i][j] > 2) throw runtime_error("can't turn into queen in this position");
        mtx[i][j] += 2;
        rerender();
    }
=======
    void drop_piece(POS_T i, POS_T j) { mtx[i][j] = 0; rerender(); }

    void turn_into_queen(POS_T i, POS_T j) { mtx[i][j] += 2; rerender(); }

    vector<vector<POS_T>> get_board() const { return mtx; }
>>>>>>> dbd5297 (Step 23)

    // Получение текущей матрицы доски
    vector<vector<POS_T>> get_board() const { return mtx; }

    // Подсветка клеток для возможных ходов
    void highlight_cells(vector<pair<POS_T, POS_T>> cells)
    {
<<<<<<< HEAD
        for (auto pos : cells)
            is_highlighted_[pos.first][pos.second] = 1;
        rerender();
    }

    // Сброс подсветки
    void clear_highlight()
    {
        for (POS_T i = 0; i < 8; ++i)
            is_highlighted_[i].assign(8, 0);
        rerender();
    }

    // Устанавливаем активную клетку (выбранную игроком)
    void set_active(const POS_T x, const POS_T y)
    {
        active_x = x;
        active_y = y;
        rerender();
    }

    // Сброс активной клетки
    void clear_active()
    {
        active_x = -1;
        active_y = -1;
        rerender();
    }

    bool is_highlighted(const POS_T x, const POS_T y) { return is_highlighted_[x][y]; }
=======
        for (auto p : cells) is_highlighted_[p.first][p.second] = 1;
        rerender();
    }

    void clear_highlight() { for (auto& row : is_highlighted_) row.assign(8, 0); rerender(); }

    void set_active(POS_T x, POS_T y) { active_x = x; active_y = y; rerender(); }
    void clear_active() { active_x = -1; active_y = -1; rerender(); }
>>>>>>> dbd5297 (Step 23)

    // Откат последнего хода (для BACK)
    void rollback()
    {
        int series = max(1, history_beat_series.back());
        while (series-- && history_mtx.size() > 1)
        {
            history_mtx.pop_back();
            history_beat_series.pop_back();
        }
        mtx = history_mtx.back();
        clear_active();
        clear_highlight();
    }

<<<<<<< HEAD
    // Отображение финального результата игры
    void show_final(const int res)
    {
        game_results = res;
        rerender();
    }

    // Если окно изменило размер — обновляем размеры и перерисовываем
=======
    void show_final(int res) { game_results = res; rerender(); }

>>>>>>> dbd5297 (Step 23)
    void reset_window_size()
    {
        SDL_GetRendererOutputSize(ren, (int*)&W, (int*)&H);
        rerender();
    }

<<<<<<< HEAD
    // Очистка ресурсов SDL
=======
    ~Board() { if (win) quit(); }

private:
    void add_history(int beat_series = 0) { history_mtx.push_back(mtx); history_beat_series.push_back(beat_series); }

    void make_start_mtx()
    {
        mtx = vector<vector<POS_T>>(8, vector<POS_T>(8, 0));
        for (POS_T i = 0; i < 8; ++i)
        {
            for (POS_T j = 0; j < 8; ++j)
            {
                if (i < 3 && (i + j) % 2 == 1) mtx[i][j] = 2;
                if (i > 4 && (i + j) % 2 == 1) mtx[i][j] = 1;
            }
        }
        add_history();
    }

    void rerender()
    {
        SDL_RenderClear(ren);
        SDL_RenderCopy(ren, board_tex, NULL, NULL);
        // draw pieces
        for (POS_T i = 0; i < 8; ++i)
            for (POS_T j = 0; j < 8; ++j)
            {
                if (!mtx[i][j]) continue;
                SDL_Rect rect{ W * (j + 1) / 10 + W / 120, H * (i + 1) / 10 + H / 120, W / 12, H / 12 };
                SDL_Texture* tex = (mtx[i][j] == 1) ? w_piece : (mtx[i][j] == 2) ? b_piece : (mtx[i][j] == 3) ? w_queen : b_queen;
                SDL_RenderCopy(ren, tex, NULL, &rect);
            }
        SDL_RenderPresent(ren);
    }

    void print_exception(const string& text)
    {
        ofstream fout(project_path + "log.txt", ios_base::app);
        fout << "Error: " << text << endl;
        fout.close();
    }

>>>>>>> dbd5297 (Step 23)
    void quit()
    {
        SDL_DestroyTexture(board_tex);
        SDL_DestroyTexture(w_piece);
        SDL_DestroyTexture(b_piece);
        SDL_DestroyTexture(w_queen);
        SDL_DestroyTexture(b_queen);
        SDL_DestroyTexture(back);
        SDL_DestroyTexture(replay);
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        SDL_Quit();
    }

<<<<<<< HEAD
    ~Board()
    {
        if (win)
            quit();
    }

private:
    // Добавление текущей матрицы и серии взятий в историю
    void add_history(const int beat_series = 0)
    {
        history_mtx.push_back(mtx);
        history_beat_series.push_back(beat_series);
    }

    // Инициализация стартовой позиции доски
    void make_start_mtx()
    {
        for (POS_T i = 0; i < 8; ++i)
        {
            for (POS_T j = 0; j < 8; ++j)
            {
                mtx[i][j] = 0;
                if (i < 3 && (i + j) % 2 == 1) mtx[i][j] = 2; // черные фигуры
                if (i > 4 && (i + j) % 2 == 1) mtx[i][j] = 1; // белые фигуры
            }
        }
        add_history();
    }

    // Перерисовка всей доски, фигур, подсветок и кнопок
    void rerender()
    {
        SDL_RenderClear(ren);
        SDL_RenderCopy(ren, board, NULL, NULL);

        // Отрисовка всех фигур
        for (POS_T i = 0; i < 8; ++i)
        {
            for (POS_T j = 0; j < 8; ++j)
            {
                if (!mtx[i][j]) continue;

                int wpos = W * (j + 1) / 10 + W / 120;
                int hpos = H * (i + 1) / 10 + H / 120;
                SDL_Rect rect{ wpos, hpos, W / 12, H / 12 };

                SDL_Texture* piece_texture;
                if (mtx[i][j] == 1) piece_texture = w_piece;
                else if (mtx[i][j] == 2) piece_texture = b_piece;
                else if (mtx[i][j] == 3) piece_texture = w_queen;
                else piece_texture = b_queen;

                SDL_RenderCopy(ren, piece_texture, NULL, &rect);
            }
        }

        // Отрисовка подсветки
        SDL_SetRenderDrawColor(ren, 0, 255, 0, 0);
        const double scale = 2.5;
        SDL_RenderSetScale(ren, scale, scale);
        for (POS_T i = 0; i < 8; ++i)
        {
            for (POS_T j = 0; j < 8; ++j)
            {
                if (!is_highlighted_[i][j]) continue;
                SDL_Rect cell{ int(W * (j + 1) / 10 / scale), int(H * (i + 1) / 10 / scale),
                              int(W / 10 / scale), int(H / 10 / scale) };
                SDL_RenderDrawRect(ren, &cell);
            }
        }

        // Отрисовка активной клетки
        if (active_x != -1)
        {
            SDL_SetRenderDrawColor(ren, 255, 0, 0, 0);
            SDL_Rect active_cell{ int(W * (active_y + 1) / 10 / scale), int(H * (active_x + 1) / 10 / scale),
                                 int(W / 10 / scale), int(H / 10 / scale) };
            SDL_RenderDrawRect(ren, &active_cell);
        }
        SDL_RenderSetScale(ren, 1, 1);

        // Отрисовка кнопок "back" и "replay"
        SDL_Rect rect_left{ W / 40, H / 40, W / 15, H / 15 };
        SDL_RenderCopy(ren, back, NULL, &rect_left);
        SDL_Rect replay_rect{ W * 109 / 120, H / 40, W / 15, H / 15 };
        SDL_RenderCopy(ren, replay, NULL, &replay_rect);

        // Отрисовка результата игры
        if (game_results != -1)
        {
            string result_path = draw_path;
            if (game_results == 1) result_path = white_path;
            else if (game_results == 2) result_path = black_path;

            SDL_Texture* result_texture = IMG_LoadTexture(ren, result_path.c_str());
            if (result_texture == nullptr)
            {
                print_exception("IMG_LoadTexture can't load game result picture from " + result_path);
                return;
            }
            SDL_Rect res_rect{ W / 5, H * 3 / 10, W * 3 / 5, H * 2 / 5 };
            SDL_RenderCopy(ren, result_texture, NULL, &res_rect);
            SDL_DestroyTexture(result_texture);
        }

        SDL_RenderPresent(ren);
        SDL_Delay(10); // Для корректной работы на macOS
        SDL_Event windowEvent;
        SDL_PollEvent(&windowEvent);
    }

    // Логирование ошибок
    void print_exception(const string& text)
    {
        ofstream fout(project_path + "log.txt", ios_base::app);
        fout << "Error: " << text << ". " << SDL_GetError() << endl;
        fout.close();
    }

public:
    int W = 0; // ширина окна
    int H = 0; // высота окна

    // История состояний доски
=======
public:
    unsigned int W = 0, H = 0;
>>>>>>> dbd5297 (Step 23)
    vector<vector<vector<POS_T>>> history_mtx;

private:
    SDL_Window* win = nullptr;
    SDL_Renderer* ren = nullptr;
<<<<<<< HEAD

    // Текстуры доски и фигур
    SDL_Texture* board = nullptr;
    SDL_Texture* w_piece = nullptr;
    SDL_Texture* b_piece = nullptr;
    SDL_Texture* w_queen = nullptr;
    SDL_Texture* b_queen = nullptr;
    SDL_Texture* back = nullptr;
    SDL_Texture* replay = nullptr;

    // Пути к текстурам
    const string textures_path = project_path + "Textures/";
    const string board_path = textures_path + "board.png";
    const string piece_white_path = textures_path + "piece_white.png";
    const string piece_black_path = textures_path + "piece_black.png";
    const string queen_white_path = textures_path + "queen_white.png";
    const string queen_black_path = textures_path + "queen_black.png";
    const string white_path = textures_path + "white_wins.png";
    const string black_path = textures_path + "black_wins.png";
    const string draw_path = textures_path + "draw.png";
    const string back_path = textures_path + "back.png";
    const string replay_path = textures_path + "replay.png";

    // Координаты выбранной клетки
    int active_x = -1, active_y = -1;

    // Результат игры (-1 = игра не закончена)
    int game_results = -1;

    // Матрица подсветки возможных ходов
    vector<vector<bool>> is_highlighted_ = vector<vector<bool>>(8, vector<bool>(8, 0));

    // Основная матрица доски
    // 0 — пустая клетка, 1 — белая, 2 — черная, 3 — белая дамка, 4 — черная дамка
    vector<vector<POS_T>> mtx = vector<vector<POS_T>>(8, vector<POS_T>(8, 0));

    // Серии взятий для истории
=======
    SDL_Texture* board_tex = nullptr, * w_piece = nullptr, * b_piece = nullptr, * w_queen = nullptr, * b_queen = nullptr, * back = nullptr, * replay = nullptr;
    int active_x = -1, active_y = -1;
    int game_results = -1;
    vector<vector<bool>> is_highlighted_ = vector<vector<bool>>(8, vector<bool>(8, 0));
    vector<vector<POS_T>> mtx = vector<vector<POS_T>>(8, vector<POS_T>(8, 0));
>>>>>>> dbd5297 (Step 23)
    vector<int> history_beat_series;
};