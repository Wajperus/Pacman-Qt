#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("PacMan"));                                       // postavlja naslov

    /* incijalizacija grafike i scene */
    ui->graphicsView->setStyleSheet("QGraphicsView {border: none;}");   // bez vertikalnih i horizontalnih scrollbar-ova
    ui->graphicsView->setBackgroundBrush(Qt::black);                    // postavljanje pozadinske boje

    int visina_mape = 20, sirina_mape = 29;                             // mapa igrice 20x29
    int x = 50, y = 50;                                                 // x i y glavnog prozora
    int w = (sirina_mape * GameObject::sirina_objekta);
    int h = (visina_mape * GameObject::sirina_objekta);

    ui->graphicsView->setGeometry(x, y, w, h);
    igra = new Game(x, y, sirina_mape, visina_mape, ":/game_objects/map_objects/mapa.txt");
    ui->graphicsView->setScene(igra);
    postaviLabele();
    igra->start();
}

void MainWindow::postaviLabele()
{
    naslov_rezultat = new QLabel(this);
    naslov_rezultat->setText("REZULTAT:");
    naslov_rezultat->setStyleSheet("QLabel {font-family: Fixedsys;color: yellow;font-size: 16px;}");
    naslov_rezultat->setGeometry(50, 12, 80, 26);

    rezultat = new QLabel(this);
    rezultat->setIndent(-80);
    rezultat->setText("0");
    rezultat->setStyleSheet("QLabel {font-family: Fixedsys;color: yellow;font-size: 16px;}");
    rezultat->setGeometry(150, 12, 150, 26);

    labela_pobjeda = new QLabel(this);
    labela_pobjeda->hide();
    labela_pobjeda->setText("Pobjeda!");
    labela_pobjeda->setStyleSheet("QLabel {font-family: Fixedsys;color: yellow;font-size: 16px;}");
    labela_pobjeda->setGeometry(310, 12, 150, 26);

    labela_poraz = new QLabel(this);
    labela_poraz->hide();
    labela_poraz->setText("Poraz!");
    labela_poraz->setStyleSheet("QLabel {font-family: Fixedsys;color: red;font-size: 16px;}");
    labela_poraz->setGeometry(310, 12, 150, 26);

    /* kreiranje i pokretanje tajmera za osvježavanje rezultata */
    rezultat_timer = new QTimer(this);
    rezultat_timer->start(25);
    connect(rezultat_timer, SIGNAL(timeout()), this , SLOT(osvjeziRezultat()));
    /*         objekt           signal        objekt        slot    */
}

void MainWindow::osvjeziRezultat()
{
    rezultat->setText(QString::number(igra->getRezultat()));
    /* ukoliko igrač pobjedi prikazuje se labela za pobjedu i zaustavlja se tajmer za rezultat */
    if (igra->status == Game::WIN)
    {
        labela_pobjeda->show();
        rezultat_timer->stop();
    }
    else if (igra->status == Game::LOSE)
    {
        labela_poraz->show();
        rezultat_timer->stop();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
    case Qt::Key_W:
        igra->pacman_next_direction(GameObject::GORE);
        break;
    case Qt::Key_A:
        igra->pacman_next_direction(GameObject::LIJEVO);
        break;
    case Qt::Key_S:
        igra->pacman_next_direction(GameObject::DOLJE);
        break;
    case Qt::Key_D:
        igra->pacman_next_direction(GameObject::DESNO);
        break;
    }
}

MainWindow::~MainWindow() { delete ui; }
