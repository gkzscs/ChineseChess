#pragma execution_character_set("utf-8")
#include "game.h"
#include "ui_game.h"
#include <QSplashScreen>
#include <QThread>
#include <QMessageBox>
#include "board.h"


game::game(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::game)
{
    ui->setupUi(this);

    init();
    start();
}

game::~game()
{
    delete ui;
}

bool game::start()
{
    resetGame();

    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/pic"));
    splash->show();
    splash->finish(this);

    return true;
}

bool game::end(chess::CampType type)
{
    if (_board) _board->deleteLater();

    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/pic"));
    splash->show();
    splash->finish(this);

    QString info = "恭喜%1获得胜利！";
    if (type == chess::Chu) info = info.arg("项羽");
    else if (type == chess::Han) info = info.arg("刘邦");
    QMessageBox::information(this, "游戏结束", info);

    return true;
}

void game::init()
{
    initMember();
    initUI();
    initSignalSlot();
}

void game::initMember()
{
    _board = nullptr;
}

void game::initUI()
{
    createBoard();
    createChesses();
}

void game::initSignalSlot()
{
    connect(_board, &board::endGame, this, &game::end);
}

void game::createBoard()
{
    const int width = 800;
    const int height = 600;

    resize(width+100, height+100);

    _board = new board(this);
    _board->setSize(width, height);
}

void game::createChesses()
{
    QStringList labList, nameList;
    labList << "车" << "马" << "象" << "仕" << "将" << "仕" << "象" << "马" << "车"
            << "炮" << "炮" << "兵" << "兵" << "兵" << "兵" << "兵";

    for (int i = 0; i < labList.length(); ++i)
    {
        _chess[i] = new chess(chess::Chu, labList.at(i), _board);
        _chess[i+16] = new chess(chess::Han, labList.at(i), _board);
    }
}

void game::resetGame()
{
    for (int i = 0; i < 9; ++i)
    {
        _board->moveChess(_chess[i], 0, i);
        _board->moveChess(_chess[i+16], 9, i);
    }
    _board->moveChess(_chess[9], 2, 1);
    _board->moveChess(_chess[10], 2, 7);
    _board->moveChess(_chess[25], 7, 1);
    _board->moveChess(_chess[26], 7, 1);
    for (int i = 0; i < 5; ++i)
    {
        _board->moveChess(_chess[i+11], 3, i*2);
        _board->moveChess(_chess[i+27], 6, i*2);
    }
}

