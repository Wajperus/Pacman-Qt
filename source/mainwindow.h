#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QLabel>
#include <QTimer>

#include "igra.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/* klasa za glavni prozor */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void postaviLabele();                       // postavljanje labela (rezultata, pobjede, poraza)
    void keyPressEvent(QKeyEvent*) override;    // upravljanje ulazom sa tipkovnice (w, a, s, d)

private slots:
    void osvjeziRezultat();                     // osvježavanje rezultata

private:
    Ui::MainWindow *ui;
    QLabel *naslov_rezultat, *rezultat, *labela_pobjeda, *labela_poraz;
    QTimer *rezultat_timer;                     // za spajanje timeout() signala sa određenim slotom
    Game *igra;                                 // objekat igre
};

#endif // MAINWINDOW_H
