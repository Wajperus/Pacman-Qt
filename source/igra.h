#ifndef IGRA_H
#define IGRA_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QTimer>
#include <QPair>

#include "objektigre.h"

#define VRIJEDNOST_LOPTICE   10          // vrijednost loptice
#define INTERVAL_PACMANA     10          // interval kretanja pacmana
#define INTERVAL_DUHA        10          // interval kretanja duhova

class Game : public QGraphicsScene
{
    Q_OBJECT

public:
    /* moguća stanja igre */
    enum StatusIgre {PLAYING, WIN, LOSE, PAUSE};
    /* konstruktor prima x i y koordinate, visinu i sirinu mape te .txt fajl
     * sa kojeg će se za dane vrijednosti (0, 1, z, p, d) postavljati odgovarajući objekti */
    Game(int, int, int, int, QString);
    ~Game();
    void start();
    void stop();
    void pacman_next_direction(GameObject::Smjer);
    int getRezultat();

    GameObject ***mapa;              // mapa
    GameObject *vrata;               // pokazivač na vrata kaveza
    Pacman *pacman;                  // pokazivač na pacmana
    Ghost *duh[Ghost::broj_duhova];  // pokazivač na duha
    StatusIgre status;

    friend class Pacman;
    friend class Ghost;

private slots:
    void pacman_handler();
    void ghost_handler(int);

private:
    int visina_mape, sirina_mape, povrsina_mape;            // visina = 20, sirina = 29, povrsina = visina * sirina
    int broj_loptica, broj_pojedenih_loptica, rezultat;
    int geo_x, geo_y;                                       // koordinate geometrije

    QTimer *duh_timer[Ghost::broj_duhova], *pacman_timer;   // tajmer za kontrolu događaja nad duhovima i pacmanom
};

#endif // IGRA_H
