#include "ttkapplication.h"

#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
#include <QMessageBox>
#include <QDateTime>

#define WIDTH 30
#define HEIGHT 20
#define SIZE 25

TTKApplication::TTKApplication(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("TTKApplication");
    setFixedSize(SIZE * WIDTH, SIZE * HEIGHT);

    initialize();
}

void TTKApplication::moveThread()
{
    moveTo(m_direction);
    check();
    update();
}

void TTKApplication::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(QPen(Qt::black,1));

    for(int i=0; i != WIDTH; ++i)
    {
        for(int j=0; j!=HEIGHT; ++j)
        {
            switch(m_data[j][i])
            {
                case Ground:
                    painter.drawPixmap(i * SIZE, j * SIZE, QPixmap(":/data/res/ground.png"));
                    break;
                case Stone:
                    painter.setBrush(Qt::red);
                    painter.drawPixmap(i * SIZE, j * SIZE, QPixmap(":/data/res/stone.png"));
                    break;
            }
        }
    }

    painter.drawPixmap(m_apple.x() * SIZE, m_apple.y() * SIZE, QPixmap(":/data/res/apple.png"));

    switch(m_direction)
    {
        case Up:
            painter.drawPixmap(m_snake[0].x() * SIZE, m_snake[0].y() * SIZE, QPixmap(":/data/res/head_up.png"));
            break;
        case Down:
            painter.drawPixmap(m_snake[0].x() * SIZE, m_snake[0].y() * SIZE, QPixmap(":/data/res/head_down.png"));
            break;
        case Left:
            painter.drawPixmap(m_snake[0].x() * SIZE, m_snake[0].y() * SIZE, QPixmap(":/data/res/head_left.png"));
            break;
        case Right:
            painter.drawPixmap(m_snake[0].x() * SIZE, m_snake[0].y() * SIZE, QPixmap(":/data/res/head_right.png"));
            break;
        default:
            break;
    }

    for(int i=1; i!=m_snake.size(); ++i)
    {
        switch(directionOf(i))
        {
            case Up:
                painter.drawPixmap(m_snake[i].x() * SIZE, m_snake[i].y() * SIZE, QPixmap(":/data/res/body_up.png"));
                break;
            case Down:
                painter.drawPixmap(m_snake[i].x() * SIZE, m_snake[i].y() * SIZE, QPixmap(":/data/res/body_down.png"));
                break;
            case Left:
                painter.drawPixmap(m_snake[i].x() * SIZE, m_snake[i].y() * SIZE, QPixmap(":/data/res/body_left.png"));
                break;
            case Right:
                painter.drawPixmap(m_snake[i].x() * SIZE, m_snake[i].y() * SIZE, QPixmap(":/data/res/body_right.png"));
                break;
            default:
                break;
        }
    }
    event->accept();
}

void TTKApplication::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key_Up:
            if(m_direction == Right || m_direction == Left)
            {
                moveTo(m_direction = Up);
                check();
                update();
            }
            break;
        case Qt::Key_Down:
            if(m_direction == Right || m_direction == Left)
            {
                moveTo(m_direction = Down);
                check();
                update();
            }
            break;
        case Qt::Key_Right:
            if(m_direction == Up || m_direction == Down)
            {
                moveTo(m_direction = Right);
                check();
                update();
            }
            break;
        case Qt::Key_Left:
            if(m_direction == Up || m_direction == Down)
            {
                moveTo(m_direction = Left);
                check();
                update();
            }
            break;
        default:
            break;
    }
    event->accept();
}

void TTKApplication::initialize()
{
    m_snake.push_back(QPoint(5, 4));
    m_snake.push_back(QPoint(5, 5));
    m_snake.push_back(QPoint(5, 6));
    m_snake.push_back(QPoint(5, 7));

    m_data << QVector<Type>(WIDTH, Stone);

    QVector<Type> stoneVector;
    stoneVector << Stone;
    for(int i = 1; i <= WIDTH - 2; ++i)
    {
        stoneVector << Ground;
    }
    stoneVector << Stone;

    for(int j = 1; j <= HEIGHT - 2; ++j)
    {
        m_data << stoneVector;
    }

    m_data << QVector<Type>(WIDTH, Stone);
    randApple();

    m_direction = Right;

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), SLOT(moveThread()));
    m_timer->start(280);
}

TTKApplication::Direction TTKApplication::directionOf(int index)
{
    if(m_snake[index].y() == m_snake[index - 1].y() - 1)
    {
        return Down;
    }
    else if(m_snake[index].y() - 1 == m_snake[index - 1].y())
    {
        return Up;
    }
    else if(m_snake[index].x() == m_snake[index - 1].x() - 1)
    {
        return Right;
    }
    else if(m_snake[index].x() - 1 == m_snake[index - 1].x())
    {
        return Left;
    }
    else
    {
        return None;
    }
}

void TTKApplication::moveTo(Direction direction)
{
    QVector<QPoint> vector(m_snake.size());
    switch(direction)
    {
        case Up:
            vector[0] = QPoint(m_snake[0].x(), m_snake[0].y() - 1);
            qCopy(m_snake.begin(), m_snake.end() - 1, vector.begin() + 1);
            m_snake = vector;
            break;
        case Left:
            vector[0] = QPoint(m_snake[0].x() - 1, m_snake[0].y());
            qCopy(m_snake.begin(), m_snake.end() - 1, vector.begin() + 1);
            m_snake = vector;
            break;
        case Down:
            vector[0] = QPoint(m_snake[0].x(), m_snake[0].y() + 1);
            qCopy(m_snake.begin(), m_snake.end() - 1, vector.begin() + 1);
            m_snake = vector;
            break;
        case Right:
            vector[0] = QPoint(m_snake[0].x() + 1, m_snake[0].y());
            qCopy(m_snake.begin(), m_snake.end() - 1, vector.begin() + 1);
            m_snake = vector;
            break;
        default: break;
    }
}

void TTKApplication::eatApple()
{
    if((m_snake.end() - 1)->x() == (m_snake.end() - 2)->x() && (m_snake.end() - 1)->y() < (m_snake.end() - 2)->y())
    {
        m_snake.push_back(QPoint((m_snake.end() - 1)->x(),(m_snake.end() - 1)->y() - 1));
    }
    if((m_snake.end() - 1)->x() == (m_snake.end() - 2)->x() && (m_snake.end() - 1)->y() > (m_snake.end() - 2)->y())
    {
        m_snake.push_back(QPoint((m_snake.end() - 1)->x(),(m_snake.end() - 1)->y() + 1));
    }
    if((m_snake.end() - 1)->y() == (m_snake.end() - 2)->y() && (m_snake.end() - 1)->x() < (m_snake.end() - 2)->x())
    {
        m_snake.push_back(QPoint((m_snake.end() - 1)->x() - 1,(m_snake.end() - 1)->y()));
    }
    if((m_snake.end() - 1)->y() == (m_snake.end() - 2)->y() && (m_snake.end() - 1)->x() > (m_snake.end() - 2)->x())
    {
        m_snake.push_back(QPoint((m_snake.end() - 1)->x() + 1,(m_snake.end() - 1)->y()));
    }

    if(m_data[(m_snake.end() - 1)->y()][(m_snake.end() - 1)->x()] == Stone)
    {
        m_snake.pop_back();
    }
}

void TTKApplication::randApple()
{
    qsrand(QDateTime::currentMSecsSinceEpoch());

    m_apple.setX(qrand() % WIDTH);
    m_apple.setY(qrand() % HEIGHT);

    if(m_data[m_apple.y()][m_apple.x()] !=0 || m_snake.contains(m_apple))
    {
        randApple();
    }
}

void TTKApplication::check()
{
    if(m_snake[0] == m_apple)
    {
        eatApple();
        randApple();
    }

    if(m_data[m_snake[0].y()][m_snake[0].x()] == Stone)
    {
        QMessageBox::information(this, tr("Game over"), tr("You eat the stone!"));
        exit(EXIT_SUCCESS);
    }

    if(qFind(m_snake.begin() + 1,m_snake.end(),m_snake[0]) != m_snake.end())
    {
        QMessageBox::information(this, tr("Game over"), tr("You eat yourself!"));
        exit(EXIT_SUCCESS);
    }

}
