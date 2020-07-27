#ifndef TTKAPPLICATION_H
#define TTKAPPLICATION_H

/* =================================================
 * This file is part of the TTKSnakeGame project
 * Copyright (C) 2017 - 2020 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

#include <QWidget>

class TTKApplication : public QWidget
{
    Q_OBJECT
public:
    enum Direction
    {
        None,
        Up,
        Down,
        Left,
        Right
    };

    enum Type
    {
        Ground,
        Stone
    };

    explicit TTKApplication(QWidget *parent = nullptr);

private Q_SLOTS:
    void moveThread();

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;

private:
    void initialize();
    Direction directionOf(int index);
    void moveTo(Direction direction);

    void eatApple();
    void randApple();
    void check();

    QVector<QPoint> m_snake;
    QPoint m_apple;
    QVector< QVector<Type> > m_data;
    QTimer *m_timer;
    Direction m_direction;

};

#endif
