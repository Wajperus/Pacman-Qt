#include <QString>
#include <QFile>
#include <QRandomGenerator>

#include "igra.h"

#define W (GameObject::sirina_objekta)

/* intervali brojeva prije puštanja duhova iz kaveza */
int VRIJEME_PUSTANJA_DUHOVA[] = {0, 200, 400, 600};

Game::Game(int x, int y, int map_w, int map_h, QString mapa_txt)
    : QGraphicsScene(x, y, W * map_w, W * map_h)
{
    geo_x = x;
    geo_y = y;
    status = PLAYING;
    mapa_txt = ":/mapa.txt";

    /* inicijalizacija pokazivača na mapu */
    povrsina_mape = map_w * map_h;
    sirina_mape = map_w;
    visina_mape = map_h;
    mapa = new GameObject**[visina_mape];
    for (int i = 0; i < visina_mape; i++)
    {
        mapa[i] = new GameObject*[sirina_mape];
        for (int j = 0; j < sirina_mape; j++)
            mapa[i][j] = nullptr;
    }

    /* inicijalizacija grafike */
    broj_loptica = broj_pojedenih_loptica = rezultat = 0;
    int ghost_i = 0;
    QPixmap zidpic(":/wall.png");
    QPixmap lopticapic(":/dot.png");
    QPixmap vratapic(":/gate.png");
    QPixmap praznopic;
    QFile mapafajl(mapa_txt);
    mapafajl.open(QIODevice::ReadOnly|QIODevice::Text);
    /* kreiranje objekta pacman */
    pacman = new Pacman();

    /* dodavanje objekata na mapu vrši se čitanjem tekstualne datoteke mape
     * predstavljene brojevima i slovima od kojih svako odgovara objektu na mapi */
    for (int i = 0; i < map_h; i++) {
        QByteArray line = mapafajl.readLine();
        for (int j = 0; j < map_w; j++) {
            int blok_x = x + (j * W);
            int blok_y = y + (i * W);
            switch (line[j]) {
            case '1':
                mapa[i][j] = new GameObject(GameObject::ZID, zidpic);
                mapa[i][j]->setPos(blok_x, blok_y);
                addItem(mapa[i][j]);
                break;
            case '0':
                mapa[i][j] = new GameObject(GameObject::LOPTICA, lopticapic);
                mapa[i][j]->setRezultat(VRIJEDNOST_LOPTICE);
                mapa[i][j]->setPos(blok_x, blok_y);
                addItem(mapa[i][j]);
                broj_loptica++;
                break;
            case '3':
                mapa[i][j] = new GameObject(GameObject::PRAZNO, praznopic);
                break;
            case '2':
                vrata = new GameObject(GameObject::VRATA, vratapic);
                vrata->mapa_x = j;
                vrata->mapa_y = i;
                vrata->setPos(blok_x, blok_y);
                addItem(vrata);
                mapa[i][j] = vrata;
                break;
            case 'p':
                pacman = new Pacman();
                pacman->PacMan = this;
                /* veća Z vrijednost označava da će dijete uvijek biti na vrhu od
                 * onoga sa manjom vrijednošću */
                pacman->setZValue(2);
                pacman->setPos(blok_x, blok_y);
                addItem(pacman);
                mapa[i][j] = pacman;
                break;
            case 'd':
                mapa[i][j] = new GameObject(GameObject::PRAZNO, praznopic);
                duh[ghost_i] = new Ghost(ghost_i);
                duh[ghost_i]->PacMan = this;
                duh[ghost_i]->setZValue(2);
                duh[ghost_i]->vrijeme_pustanja = VRIJEME_PUSTANJA_DUHOVA[ghost_i];
                duh[ghost_i]->mapa_x = j;
                duh[ghost_i]->mapa_y = i;
                duh[ghost_i]->setPos(blok_x, blok_y);
                addItem(duh[ghost_i]);
                ghost_i++;
                break;
            }
            if (mapa[i][j]) {
                mapa[i][j]->mapa_x = j;
                mapa[i][j]->mapa_y = i;
            }
        }
    }

    duh[Ghost::CRVEN]->chase_strategy = &strategija1;
    duh[Ghost::ROZ]->chase_strategy = &strategija2;
    duh[Ghost::ZELEN]->chase_strategy = &strategija3;
    duh[Ghost::ZUT]->chase_strategy = &strategija4;
}

void Game::pacman_handler()
{
    pacman->pomak();
    if (status == WIN)
        stop();
}

void Game::ghost_handler(int ghost_id)
{
    duh[ghost_id]->pomak();
    if (status == LOSE)
        stop();
}

void Game::start()
{
    pacman_timer = new QTimer(this);
    connect(pacman_timer, SIGNAL(timeout()), this, SLOT(pacman_handler()));
    pacman_timer->start(INTERVAL_PACMANA);

    for (int i = 0; i < Ghost::broj_duhova; i++) {
        duh_timer[i] = new QTimer(this);
        /* lambda funkcija omogućava slanje indeksa duha upravljačkoj funkciji */
        connect(duh_timer[i], &QTimer::timeout, [=](){ghost_handler(i);} );
        duh_timer[i]->start(INTERVAL_DUHA);
    }
}

void Game::stop()
{
    pacman_timer->stop();
    for (int i = 0; i < Ghost::broj_duhova; i++)
        duh_timer[i]->stop();
}

void Game::pacman_next_direction(GameObject::Smjer s) { pacman->setNextSmjer(s); }

int Game::getRezultat() { return rezultat; }

Game::~Game()
{
    for (int i = 0; i < visina_mape; i++) {
        for (int j = 0; j < sirina_mape; j++) {
            if (mapa[i][j] != nullptr)
                delete mapa[i][j];
        }
        delete[] mapa[i];
    }
    delete[] mapa;
    delete pacman_timer;
    for (int i = 0; i < Ghost::broj_duhova; i++) {
        delete duh_timer[i];
    }
}
