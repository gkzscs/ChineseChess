#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <chess.h>

class board;

namespace Ui {
class game;
}

class game : public QWidget
{
    Q_OBJECT

public:
    explicit game(QWidget *parent = nullptr);
    ~game();

    bool start();
    bool end(chess::CampType type);

private:
    void init();
    void initMember();
    void initUI();
    void initSignalSlot();

private:
    void createBoard();
    void createChesses();

private:
    void resetGame();

private:
    Ui::game *ui;
    board *_board;
    chess *_chess[32];

};

#endif // GAME_H
