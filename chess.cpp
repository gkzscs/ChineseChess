#include "chess.h"
#include <QPainter>


chess::chess(const CampType type, const QString name, QWidget *parent)
    : QWidget(parent), _type(type), _name(name)
{
    init();
}

chess::CampType chess::getCamp() const
{
    return _type;
}

QString chess::getName() const
{
    return _name;
}

chess::ChessState chess::getState() const
{
    return _state;
}

void chess::setState(ChessState state)
{
    _state = state;

    switch (state)
    {
    case Alive:
        setColor(Qt::white);
        show();
        break;
    case Dead:
        hide();
        setParent(nullptr);
        break;
    case Selected:
        setColor(Qt::yellow);
        break;
    }
}

bool chess::isAlive() const
{
    return !(_state == Dead);
}

bool chess::isSelected() const
{
    return (_state == Selected);
}

void chess::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    drawBgColor();
    drawBoarder();
    drawName();
}

void chess::init()
{
    initMember();
    initUI();
}

void chess::initMember()
{
    _state = Alive;
    _bgColor = Qt::white;
    _row = -1;
    _col = -1;
}

void chess::initUI()
{
    resize(50, 50);
}

void chess::drawBoarder()
{
    QPainter p(this);
    QColor color = getColor();
    p.setPen(color);

    p.drawEllipse(rect());
}

void chess::drawName()
{
    QPainter p(this);
    QColor color = getColor();
    p.setPen(color);

    p.drawText(width()/2, height()/2, _name);
}

void chess::drawBgColor()
{
    QPainter p(this);
    p.setBrush(_bgColor);

    p.drawEllipse(rect());
}

QColor chess::getColor()
{
    QColor color;

    switch (_type)
    {
    case Chu:
        color = Qt::red;
        break;
    case Han:
        color = Qt::black;
        break;
    }

    return color;
}

void chess::setColor(const QColor &color)
{
    _bgColor = color;
    update();
}

