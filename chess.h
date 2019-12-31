#ifndef CHESS_H
#define CHESS_H
#include <QWidget>


class chess : public QWidget
{
    Q_OBJECT
    friend class board;

public:
    enum CampType { Chu, Han };
    enum ChessState { Alive, Dead, Selected };

public:
    explicit chess(const CampType type, const QString name, QWidget *parent = nullptr);
    CampType getCamp() const;
    QString getName() const;
    ChessState getState() const;
    void setState(ChessState state);
    bool isAlive() const;
    bool isSelected() const;

protected:
    virtual void paintEvent(QPaintEvent *event) override;

private:
    void init();
    void initMember();
    void initUI();

private:
    void drawBoarder();
    void drawName();
    void drawBgColor();
    QColor getColor();
    void setColor(const QColor &color);

private:
    CampType _type;
    ChessState _state;
    QString _name;
    QColor _bgColor;
    int _row;
    int _col;

};

#endif // CHESS_H
