#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include "chess.h"

class board : public QWidget
{
    Q_OBJECT

public:
    explicit board(QWidget *parent = nullptr);
    explicit board(int w, int h, QWidget *parent = nullptr);

    void setSize(int w, int h);

public:
    bool moveChess(chess *che, int row, int col);

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void clearBoard();
    void drawBoard();
    void drawBattleField();
    void drawRow(int row);
    void drawColumn(int col);
    void drawBaseCamp();

private:
    QPoint getPos(int row, int col);
    int getRow(const QPoint &pos);
    int getCol(const QPoint &pos);
    chess * getChess(int row, int col);
    void resetNowChess();
    bool canMove(const chess *che, int row, int col);
    bool canChariotMove(chess::CampType type, int orgRow, int orgCol, int tgtRow, int tgtCol);
    bool canHorseMove(chess::CampType type, int orgRow, int orgCol, int tgtRow, int tgtCol);
    bool canChancellorMove(chess::CampType type, int orgRow, int orgCol, int tgtRow, int tgtCol);
    bool canGuardMove(chess::CampType type, int orgRow, int orgCol, int tgtRow, int tgtCol);
    bool canGeneralMove(chess::CampType type, int orgRow, int orgCol, int tgtRow, int tgtCol);
    bool canCannonMove(chess::CampType type, int orgRow, int orgCol, int tgtRow, int tgtCol);
    bool canSoldierMove(chess::CampType type, int orgRow, int orgCol, int tgtRow, int tgtCol);

signals:
    void endGame(chess::CampType type);

private:
    int _width;
    int _height;
    int _row;
    int _column;
    chess *_nowChess;

};

#endif // BOARD_H
