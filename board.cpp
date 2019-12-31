#pragma execution_character_set("utf-8")
#include "board.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>


board::board(QWidget *parent)
    : QWidget(parent), _row(10), _column(9)
{
    _width = 0;
    _height = 0;
    _nowChess = nullptr;
}

board::board(int w, int h, QWidget *parent)
    : board(parent)
{
    setSize(w, h);
}

void board::setSize(int w, int h)
{
    _width = w;
    _height = h;
    resize(w+100, h+100);

    update();
}

bool board::moveChess(chess *che, int row, int col)
{
    if (!che) return false;
    if (row < 0 || row > _row || col < 0 || col >= _column) return false;

    QPoint pos = getPos(row, col);
    int x = pos.x() - che->width()/2;
    int y = pos.y() - che->height()/2;
    che->move(x, y);

    che->_row = row;
    che->_col = col;
    che->setState(chess::Alive);
    _nowChess = nullptr;

    return true;
}

void board::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    drawBoard();
}

void board::mousePressEvent(QMouseEvent *event)
{
    const QPoint &pos = event->pos();
    int row = getRow(pos);
    int col = getCol(pos);

    chess *che = qobject_cast<chess *>(childAt(pos));
    if (che)
    {
        if (!_nowChess)
        {
            _nowChess = che;
            _nowChess->setState(chess::Selected);
            return;
        }

        bool res = canMove(_nowChess, row, col);
        if (!res)
        {
            _nowChess->setState(chess::Alive);
            _nowChess = che;
            _nowChess->setState(chess::Selected);
            return;
        }

        moveChess(_nowChess, row, col);
        che->setState(chess::Dead);

        if (che->_name.compare("将") == 0)
        {
            endGame(_nowChess->_type);
            return;
        }
    }

    if (!_nowChess) return;

    bool res = canMove(_nowChess, row, col);
    if (!res)
    {
        _nowChess->setState(chess::Alive);
        _nowChess = nullptr;
        return;
    }
    moveChess(_nowChess, row, col);
}

void board::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void board::clearBoard()
{
    QPainter p(this);
    p.fillRect(rect(), Qt::white);
}

void board::drawBoard()
{
    clearBoard();

    drawBattleField();
    drawBaseCamp();
}

void board::drawBattleField()
{
    QPainter p(this);
    QPen pen(Qt::black);

    for (int row = 0; row < _row; ++row)
    {
        drawRow(row);
    }
    for (int col = 0; col < _column; ++col)
    {
        drawColumn(col);
    }
}

void board::drawRow(int row)
{
    int y = _height * row / (_row - 1) + 50;

    QPainter p(this);
    p.setPen(Qt::black);

    p.drawLine(50, y, _width+50, y);
}

void board::drawColumn(int col)
{
    int x = _width * col / (_column - 1) + 50;

    QPainter p(this);
    p.setPen(Qt::black);

    if (col == 0 || col == _column-1)
    {
        p.drawLine(x, 50, x, _height+50);
        return;
    }

    int y1 = _height * (_row/2-1) / (_row - 1) + 50;
    int y2 = _height * (_row/2) / (_row - 1) + 50;
    p.drawLine(x, 50, x, y1);
    p.drawLine(x, y2, x, _height+50);
}

void board::drawBaseCamp()
{
    QPainter p(this);
    p.setPen(Qt::black);

    int x1 = _width * 3 / (_column - 1) + 50;
    int x2 = _width * 5 / (_column - 1) + 50;
    int y = _height * 2 / (_row - 1) + 50;

    p.drawLine(x1, 50, x2, y);
    p.drawLine(x2, 50, x1, y);

    int y1 = _height * 7 / (_row - 1) + 50;
    int y2 = _height + 50;
    p.drawLine(x1, y1, x2, y2);
    p.drawLine(x2, y1, x1, y2);
}

QPoint board::getPos(int row, int col)
{
    QPoint pos;
    if (row < 0 || row >= _row || col < 0 || col >= _column) return pos;

    pos.setX(_width * col / (_column-1) + 50);
    pos.setY(_height * row / (_row-1) + 50);
    return pos;
}

int board::getRow(const QPoint &pos)
{
    int y = pos.y();

    for (int row = 0; row < _row; ++row)
    {
        QPoint tmpPos = getPos(row, 0);
        if (tmpPos.y()-25 <= y && tmpPos.y()+25 >= y) return row;
    }

    return -1;
}

int board::getCol(const QPoint &pos)
{
    int x = pos.x();
    for (int col = 0; col < _column; ++col)
    {
        QPoint tmpPos = getPos(0, col);
        if (tmpPos.x()-25 <= x && tmpPos.x()+25 >= x) return col;
    }

    return -1;
}

chess * board::getChess(int row, int col)
{
    QPoint pos = getPos(row, col);
    chess *che = qobject_cast<chess *>(childAt(pos));

    return che;
}

void board::resetNowChess()
{
    if (!_nowChess) return;
    if (_nowChess->isAlive()) _nowChess->setState(chess::Alive);

    _nowChess = nullptr;
}

bool board::canMove(const chess *che, int row, int col)
{
    bool res = false;
    if (!che) return res;
    if (row < 0 || row >= _row || col < 0 || col >= _column) return res;

    int offRow = che->_row - row;
    int offCol = che->_col - col;
    if (offRow == 0 && offCol == 0) return res;

    const QString &name = che->_name;
    if (name.compare("车") == 0)
    {
        res = canChariotMove(che->_type, che->_row, che->_col, row, col);
    }
    else if (name.compare("马") == 0)
    {
        res = canHorseMove(che->_type, che->_row, che->_col, row, col);
    }
    else if (name.compare("象") == 0)
    {
        qDebug() << "name is 象" << endl;
        res = canChancellorMove(che->_type, che->_row, che->_col, row, col);
    }
    else if (name.compare("仕") == 0)
    {
        res = canGuardMove(che->_type, che->_row, che->_col, row, col);
    }
    else if (name.compare("将") == 0)
    {
        res = canGeneralMove(che->_type, che->_row, che->_col, row, col);
    }
    else if (name.compare("炮") == 0)
    {
        res = canCannonMove(che->_type, che->_row, che->_col, row, col);
    }
    else if (name.compare("兵") == 0)
    {
        res = canSoldierMove(che->_type, che->_row, che->_col, row, col);
    }

    return res;
}

bool board::canChariotMove(chess::CampType type, int orgRow, int orgCol, int tgtRow, int tgtCol)
{
    int offRow = orgRow - tgtRow;
    int offCol = orgCol - tgtCol;

    if (offRow != 0 && offCol != 0) return false;
    if (offRow == 0)
    {
        int min = (orgCol < tgtCol ? orgCol : tgtCol);
        int max = (orgCol > tgtCol ? orgCol : tgtCol);
        for (int col = min+1; col < max; ++col)
        {
            chess *obstacle = getChess(orgRow, col);
            if (obstacle) return false;
        }
    }
    else if (offCol == 0)
    {
        int min = (orgRow < tgtRow ? orgRow : tgtRow);
        int max = (orgRow > tgtRow ? orgRow : tgtRow);
        for (int row = min+1; row < max; ++row)
        {
            chess *obstacle = getChess(row, orgCol);
            if (obstacle) return false;
        }
    }

    chess *tgt = getChess(tgtRow, tgtCol);
    if (tgt && tgt->_type == type) return false;

    return true;
}

bool board::canHorseMove(chess::CampType type, int orgRow, int orgCol, int tgtRow, int tgtCol)
{
    int offRow = orgRow - tgtRow;
    int offCol = orgCol - tgtCol;
    int absRow = abs(offRow);
    int absCol = abs(offCol);

    if (absRow + absCol != 3) return false;

    int obstacleRow = orgRow;
    int obstacleCol = orgCol;
    if (absRow == 2) obstacleRow = (orgRow + tgtRow) / 2;
    if (absCol == 2) obstacleCol = (orgCol + tgtCol) / 2;

    chess *obstacle = getChess(obstacleRow, obstacleCol);
    if (obstacle) return false;

    chess *tgt = getChess(tgtRow, tgtCol);
    if (tgt && tgt->_type == type) return false;

    return true;
}

bool board::canChancellorMove(chess::CampType type, int orgRow, int orgCol, int tgtRow, int tgtCol)
{
    int offRow = orgRow - tgtRow;
    int offCol = orgCol - tgtCol;
    int absRow = abs(offRow);
    int absCol = abs(offCol);

    if (tgtRow >= _row/2) return false;
    if (absRow != 2 || absCol != 2) return false;
    int obstacleRow = (orgRow + tgtRow) / 2;
    int obstacleCol = (orgCol + tgtCol) / 2;

    chess *obstacle = getChess(obstacleRow, obstacleCol);
    if (obstacle) return false;

    chess *tgt = getChess(tgtRow, tgtCol);
    if (tgt && tgt->_type == type) return false;

    return true;
}

bool board::canGuardMove(chess::CampType type, int orgRow, int orgCol, int tgtRow, int tgtCol)
{
    int offRow = orgRow - tgtRow;
    int offCol = orgCol - tgtCol;
    int absRow = abs(offRow);
    int absCol = abs(offCol);

    if (tgtRow > 2 || tgtCol < 3 || tgtCol > 5) return false;
    if (absRow != 1 || absCol != 1) return false;

    chess *tgt = getChess(tgtRow, tgtCol);
    if (tgt && tgt->_type == type) return false;

    return true;
}

bool board::canGeneralMove(chess::CampType type, int orgRow, int orgCol, int tgtRow, int tgtCol)
{
    int offRow = orgRow - tgtRow;
    int offCol = orgCol - tgtCol;
    int absRow = abs(offRow);
    int absCol = abs(offCol);

    chess *tgt = getChess(tgtRow, tgtCol);
    if (tgt)
    {
        if (tgt->_type == type) return false;
        if (tgt->_name.compare("将") == 0 && absCol == 0) return true;
    }

    if (tgtRow > 2 || tgtCol < 3 || tgtCol > 5) return false;
    if (absRow + absCol != 1) return false;

    return true;
}

bool board::canCannonMove(chess::CampType type, int orgRow, int orgCol, int tgtRow, int tgtCol)
{
    int offRow = orgRow - tgtRow;
    int offCol = orgCol - tgtCol;
    if (offRow != 0 && offCol != 0) return false;

    int num = 0;
    if (offRow == 0)
    {
        int min = (orgCol < tgtCol ? orgCol : tgtCol);
        int max = (orgCol > tgtCol ? orgCol : tgtCol);
        for (int col = min+1; col < max; ++col)
        {
            chess *obstacle = getChess(orgRow, col);
            if (obstacle) ++num;
        }
    }
    else if (offCol == 0)
    {
        int min = (orgRow < tgtRow ? orgRow : tgtRow);
        int max = (orgRow > tgtRow ? orgRow : tgtRow);
        for (int row = min+1; row < max; ++row)
        {
            chess *obstacle = getChess(row, orgCol);
            if (obstacle) ++num;
        }
    }

    chess *tgt = getChess(tgtRow, tgtCol);
    if (num == 0 && tgt) return false;
    if (num == 1 && (!tgt || tgt->_type == type)) return false;
    else if (num > 1) return false;

    return true;
}

bool board::canSoldierMove(chess::CampType type, int orgRow, int orgCol, int tgtRow, int tgtCol)
{
    int offRow = orgRow - tgtRow;
    int offCol = orgCol - tgtCol;
    int absRow = abs(offRow);
    int absCol = abs(offCol);

    if (absRow + absCol != 1) return false;
    if (absCol != 0)
    {
        if (type == chess::Chu && orgRow < _row/2) return false;
        else if (type == chess::Han && orgRow >= _row/2) return false;
    }
    chess *tgt = getChess(tgtRow, tgtCol);
    if (tgt && tgt->_type == type) return false;

    return true;
}


