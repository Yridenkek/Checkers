#pragma once
#include <stdlib.h>

// “ип дл€ координат клетки на доске
typedef int8_t POS_T;

// —труктура дл€ хранени€ информации о ходе
struct move_pos
{
    POS_T x, y;             //  оординаты клетки, с которой делаетс€ ход ("from")
    POS_T x2, y2;           //  оординаты клетки, на которую делаетс€ ход ("to")
    POS_T xb = -1, yb = -1; //  оординаты фигуры, котора€ была побита (-1 если нет)

    //  онструктор дл€ обычного хода без вз€ти€
    move_pos(const POS_T x, const POS_T y, const POS_T x2, const POS_T y2)
        : x(x), y(y), x2(x2), y2(y2)
    {
    }

    //  онструктор дл€ хода с вз€тием фигуры
    move_pos(const POS_T x, const POS_T y, const POS_T x2, const POS_T y2, const POS_T xb, const POS_T yb)
        : x(x), y(y), x2(x2), y2(y2), xb(xb), yb(yb)
    {
    }

    // ѕерегрузка оператора == дл€ сравнени€ ходов
    // —равниваем только начальные и конечные клетки (не вз€тую фигуру)
    bool operator==(const move_pos& other) const
    {
        return (x == other.x && y == other.y && x2 == other.x2 && y2 == other.y2);
    }

    // ѕерегрузка оператора != дл€ удобного сравнени€
    bool operator!=(const move_pos& other) const
    {
        return !(*this == other);
    }
};