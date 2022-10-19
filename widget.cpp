#include "widget.h"
#include "ui_widget.h"

#include <QVector>
#include <QPainter>
#include <QMap>
#include <QKeyEvent>
#include <QPushButton>
#include <QFile>

const int WINSCORE = 2048;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    QFile file("save.dat");
    if (file.open(QIODevice::ReadOnly)) {
        QDataStream in(&file);
        for (int i=0; i<16; i++) in >> field[i];
        in >> score;
        in >> highscore;
        in >> Game;
        in >> Win;
        in >> Reset;
        if (Game) grabKeyboard();
        file.close();
    } else {
        for (int i=0; i<16; i++) field[i]=0;
        score = 0;
        highscore = 0;
        Game = false;
        Win = false;
        Reset = true;
    }
    colors[2] = QColor(255, 255, 255);
    colors[4] = QColor(255, 255, 191);
    colors[8] = QColor(255, 255, 127);
    colors[16] = QColor(255, 255, 63);
    colors[32] = QColor(255, 255, 0);
    colors[64] = QColor(255, 200, 0);
    colors[128] = QColor(255, 160, 0);
    colors[256] = QColor(255, 120, 0);
    colors[512] = QColor(255, 80, 0);
    colors[1024] = QColor(255, 40, 0);
    colors[2048] = QColor(255, 0, 0);
    colors[4096] = QColor(200, 0, 0);
    colors[8192] = QColor(160, 0, 0);
    colors[16384] = QColor(120, 0, 0);
    colors[32768] = QColor(80, 0, 0);
    colors[65536] = QColor(40, 0, 0);
    colors[131072] = QColor(0, 0, 0);

    button1 = new QPushButton(this);
    button1->setGeometry(60, 515, 80, 30);
    button1->setText(QString("New Game"));
    connect(button1, SIGNAL(clicked()), SLOT(b1()));

    button2 = new QPushButton(this);
    button2->setGeometry(160, 515, 80, 30);
    button2->setText(QString("Reset"));
    connect(button2, SIGNAL(clicked()), SLOT(b2()));

    button3 = new QPushButton(this);
    button3->setGeometry(260, 515, 80, 30);
    button3->setText(QString("Exit"));
    connect(button3, SIGNAL(clicked()), SLOT(b3()));

}

void Widget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter paint(this);
    QFont font("Helvetica", 20);
    QFontMetrics fm(font);
    paint.setFont(font);
    {
        QString tmp = QString("Score:");
        int tmpW = fm.horizontalAdvance(tmp);
        paint.drawText(100-tmpW/2, 40, tmp);
        tmp = QString("Best:");
        tmpW = fm.horizontalAdvance(tmp);
        paint.drawText(300-tmpW/2, 40, tmp);
        tmp = QString::number(score);
        tmpW = fm.horizontalAdvance(tmp);
        paint.drawText(100-tmpW/2, 80, tmp);
        tmp = QString::number(highscore);
        tmpW = fm.horizontalAdvance(tmp);
        paint.drawText(300-tmpW/2, 80, tmp);
    }
    paint.drawLine(0, 100, 400, 100);
    paint.drawLine(0, 500, 400, 500);
    for (int i=0; i<16; i++) {
        if (field[i]>0) {
            paint.setPen(colors[field[i]]);
            paint.setBrush(colors[field[i]]);
            paint.drawRoundedRect(2+i%4*100, 102+i/4*100, 96, 96, 10, 10);
            QString text = QString::number(field[i]);
            int textW = fm.horizontalAdvance(text);
            if (field[i]<64) paint.setPen(QColor(0, 0, 0));
            else paint.setPen(QColor(255, 255, 255));
            paint.drawText(50-textW/2+i%4*100, 160+i/4*100, text);
        }
    }
    if (!Game&&!Reset) {
        paint.setPen(QColor(255, 0, 0));
        QString tmp = QString("GAME OVER");
        paint.drawText(380-fm.horizontalAdvance(tmp), 585, tmp);
    }
    if (Win) {
        paint.setPen(QColor(0, 255, 0));
        paint.drawText(20, 585, QString("YOU WON!"));
    }
}

void Widget::keyPressEvent(QKeyEvent *event) {
    int key = event->key();
    int keyL = event->nativeVirtualKey();// key();
    if (key==Qt::Key_Left || keyL==Qt::Key_A) {
        bool f=false;
        for (int i=0; i<16; i++) {
            if (field[i]==0) continue;
            int j=i;
            while ((j%4>0)&&(field[j-1]==0)) {
                field[j-1]=field[j];
                field[j]=0;
                j--;
                f=true;
            }
            if ((j%4>0)&&(field[j-1]==field[j])) {
                field[j-1]*=2;
                if (field[j-1]==WINSCORE) Win = true;
                field[j]=0;
                score+=field[j-1];
                field[j-1]--;
                f=true;
            }
        }
        for (int i=0; i<16; i++) if (field[i]%2==1) field[i]++;
        if (f) Game=CreateNewTile();
    }
    else if (key==Qt::Key_Right || keyL==Qt::Key_D) {
        bool f=false;
        for (int i=15; i>=0; i--) {
            if (field[i]==0) continue;
            int j=i;
            while ((j%4<3)&&(field[j+1]==0)) {
                field[j+1]=field[j];
                field[j]=0;
                j++;
                f=true;
            }
            if ((j%4<3)&&(field[j+1]==field[j])) {
                field[j+1]*=2;
                if (field[j+1]==WINSCORE) Win = true;
                field[j]=0;
                score+=field[j+1];
                field[j+1]--;
                f=true;
            }
        }
        for (int i=0; i<16; i++) if (field[i]%2==1) field[i]++;
        if (f) Game=CreateNewTile();
    }
    else if (key==Qt::Key_Up || keyL==Qt::Key_W) {
        bool f=false;
        for (int i=0; i<16; i++) {
            if (field[i]==0) continue;
            int j=i;
            while ((j>3)&&(field[j-4]==0)) {
                field[j-4]=field[j];
                field[j]=0;
                j-=4;
                f=true;
            }
            if ((j>3)&&(field[j-4]==field[j])) {
                field[j-4]*=2;
                if (field[j-4]==WINSCORE) Win = true;
                field[j]=0;
                score+=field[j-4];
                field[j-4]--;
                f=true;
            }
        }
        for (int i=0; i<16; i++) if (field[i]%2==1) field[i]++;
        if (f) Game=CreateNewTile();
    }
    else if (key==Qt::Key_Down || keyL==Qt::Key_S) {
        bool f=false;
        for (int i=15; i>=0; i--) {
            if (field[i]==0) continue;
            int j=i;
            while ((j<12)&&(field[j+4]==0)) {
                field[j+4]=field[j];
                field[j]=0;
                j+=4;
                f=true;
            }
            if ((j<12)&&(field[j+4]==field[j])) {
                field[j+4]*=2;
                if (field[j+4]==WINSCORE) Win = true;
                field[j]=0;
                score+=field[j+4];
                field[j+4]--;
                f=true;
            }
        }
        for (int i=0; i<16; i++) if (field[i]%2==1) field[i]++;
        if (f) Game=CreateNewTile();
    }
    if (score>highscore) highscore=score;
    if (!Game) releaseKeyboard();
    emit KeyPressed();

}

bool Widget::CreateNewTile() {
    srand(time(NULL));
    int tile = rand()%10;
    QVector<int> empty;
    for (int i=0; i<16; i++) if (field[i]==0) empty.push_back(i);
    int place = rand()%empty.size();
    if (tile == 0) field[empty[place]] = 4;
    else field[empty[place]] = 2;
    if (empty.size()>1) return true;
    else {
        bool f=false;
        for (int i=0; i<15; i++) {
            if (((i+1)%4>0)&&(field[i]==field[i+1])) f=true;
            if (((i+4)<16)&&(field[i]==field[i+4])) f=true;
        }
        return f;
    }
}

void Widget::b1() {  // New Game
    for (int i=0; i<16; i++) field[i]=0;
    score=0;
    Game=true;
    Win=false;
    Reset=false;
    CreateNewTile();
    CreateNewTile();
    grabKeyboard();
    emit KeyPressed();
}

void Widget::b2() {  // Reset
    for (int i=0; i<16; i++) field[i]=0;
    score=0;
    highscore=0;
    Win=false;
    Game=false;
    Reset=true;
    releaseKeyboard();
    emit KeyPressed();
}

void Widget::b3() {  // Exit
    releaseKeyboard();
    QFile file("save.dat");
    if (file.open(QIODevice::WriteOnly)) {
        QDataStream out(&file);
        for (int i=0; i<16; i++) out << field[i];
        out << score;
        out << highscore;
        out << Game;
        out << Win;
        out << Reset;
        file.close();
    }

    exit(0);
}

Widget::~Widget()
{
    delete ui;
}

