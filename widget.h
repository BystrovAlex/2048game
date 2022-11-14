#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    int field[20];
    int score, highscore;
    QMap<int, QColor> colors;
    QPushButton *button1, *button2, *button3;
    bool Game, Win, Reset;

    bool CreateNewTile();

protected:
    void paintEvent(QPaintEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    void closeEvent(QCloseEvent *event);

public slots:
    void KeyPressed() {
        this->repaint();
    }
    void b1();
    void b2();

};
#endif // WIDGET_H
