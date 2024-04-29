#include <QRandomGenerator>

#include "igra.h"
#include "chasing_strategy.cpp"

#define W (GameObject::sirina_objekta)

Ghost::Ghost(int b) : GameObject(
    GameObject::DUH, QPixmap())
{
    boja = (Boja)b;
    index_animacije = 0;
    pusten = false;
    smjer = Smjer(QRandomGenerator::global()->generate() % 4);

    switch (boja)
    {
    case CRVEN:     // Blinky
        animacija[DESNO].push_back(QPixmap(":/redright1.png"));
        animacija[DESNO].push_back(QPixmap(":/redright2.png"));
        animacija[GORE].push_back(QPixmap(":/redup1.png"));
        animacija[GORE].push_back(QPixmap(":/redup2.png"));
        animacija[LIJEVO].push_back(QPixmap(":/redleft1.png"));
        animacija[LIJEVO].push_back(QPixmap(":/redleft2.png"));
        animacija[DOLJE].push_back(QPixmap(":/reddown1.png"));
        animacija[DOLJE].push_back(QPixmap(":/reddown2.png"));
        break;
    case ZUT:       // Inky
        animacija[DESNO].push_back(QPixmap(":/yellowright1.png"));
        animacija[DESNO].push_back(QPixmap(":/yellowright2.png"));
        animacija[GORE].push_back(QPixmap(":/yellowup1.png"));
        animacija[GORE].push_back(QPixmap(":/yellowup2.png"));
        animacija[LIJEVO].push_back(QPixmap(":/yellowleft1.png"));
        animacija[LIJEVO].push_back(QPixmap(":/yellowleft2.png"));
        animacija[DOLJE].push_back(QPixmap(":/yellowdown1.png"));
        animacija[DOLJE].push_back(QPixmap(":/yellowdown2.png"));
        break;
    case ZELEN:     // Clyde
        animacija[DESNO].push_back(QPixmap(":/greenright1.png"));
        animacija[DESNO].push_back(QPixmap(":/greenright2.png"));
        animacija[GORE].push_back(QPixmap(":/greenup1.png"));
        animacija[GORE].push_back(QPixmap(":/greenup2.png"));
        animacija[LIJEVO].push_back(QPixmap(":/greenleft1.png"));
        animacija[LIJEVO].push_back(QPixmap(":/greenleft2.png"));
        animacija[DOLJE].push_back(QPixmap(":/greendown1.png"));
        animacija[DOLJE].push_back(QPixmap(":/greendown2.png"));
        break;
    case ROZ:       // Pinky
        animacija[DESNO].push_back(QPixmap(":/pinkright1.png"));
        animacija[DESNO].push_back(QPixmap(":/pinkright2.png"));
        animacija[GORE].push_back(QPixmap(":/pinkup1.png"));
        animacija[GORE].push_back(QPixmap(":/pinkup2.png"));
        animacija[LIJEVO].push_back(QPixmap(":/pinkleft1.png"));
        animacija[LIJEVO].push_back(QPixmap(":/pinkleft2.png"));
        animacija[DOLJE].push_back(QPixmap(":/pinkdown1.png"));
        animacija[DOLJE].push_back(QPixmap(":/pinkdown2.png"));
        break;
    }

    setPixmap(animacija[DESNO][0]);
}

Ghost::Boja Ghost::getBoja() { return boja; }

void Ghost::pomakGore()
{
    QVector<QPixmap> *tmp;

    tmp = &animacija[GORE];

    index_animacije++;
    if (index_animacije >= tmp->size())
        index_animacije = 0;

    setPixmap((*tmp)[index_animacije]);
    setY(static_cast<int>(y()) - 1);
}

void Ghost::pomakLijevo()
{
    QVector<QPixmap> *tmp;

    tmp = &animacija[LIJEVO];

    index_animacije++;
    if (index_animacije >= tmp->size())
        index_animacije = 0;

    setPixmap((*tmp)[index_animacije]);
    setX(static_cast<int>(x()) - 1);
}

void Ghost::pomakDolje()
{
    QVector<QPixmap> *tmp;

    tmp = &animacija[DOLJE];

    index_animacije++;
    if (index_animacije >= tmp->size())
        index_animacije = 0;

    setPixmap((*tmp)[index_animacije]);
    setY(static_cast<int>(y()) + 1);
}

void Ghost::pomakDesno()
{
    QVector<QPixmap> *tmp;

    tmp = &animacija[DESNO];

    index_animacije++;
    if (index_animacije >= tmp->size())
        index_animacije = 0;

    setPixmap((*tmp)[index_animacije]);
    setX(static_cast<int>(x()) + 1);
}

bool Ghost::naMapi(int i, int j)
{
    if (i < 0 || j < 0)
        return false;

    if (i >= PacMan->visina_mape || j >= PacMan->sirina_mape)
        return false;

    switch (PacMan->mapa[i][j]->getTip())
    {
    case ZID:
        return false;
    case VRATA:
        if (pusten == false && vrijeme_pustanja == 0)
            return true;
        else return false;
    default:
        return true;
    }
}

/* spremamo niz smjerova ovisno o poziciji duha te nasumično biramo jedan */
void Ghost::postaviRandomSmjer()
{
    QVector<Smjer> rand_smjer;

    if (naMapi(mapa_y, mapa_x + 1))
        rand_smjer.push_back(DESNO);

    if (naMapi(mapa_y, mapa_x - 1))
        rand_smjer.push_back(LIJEVO);

    if (naMapi(mapa_y + 1, mapa_x))
        rand_smjer.push_back(DOLJE);

    if (naMapi(mapa_y - 1, mapa_x))
        rand_smjer.push_back(GORE);

    smjer = rand_smjer.at(QRandomGenerator::global()->generate() % rand_smjer.size());
}

void Ghost::izadjiIzKaveza()
{
    int x_udaljenost = PacMan->vrata->mapa_x - mapa_x;
    int y_udaljenost = PacMan->vrata->mapa_y - mapa_y;

    if (x_udaljenost > 0)
        setSmjer(GameObject::DESNO);
    else if (x_udaljenost < 0)
        setSmjer(GameObject::LIJEVO);
    else {
        if (y_udaljenost > 0)
            setSmjer(GameObject::DOLJE);
        else
            setSmjer(GameObject::GORE);
    }
}

void Ghost::ganjajPacmana()
{
    /* ukoliko je objekat zid vrijednost je false */
    bool smjerovi[5] = {false, false, false, false, false};
    QVector<Smjer> smjerovi_vec;

    if (naMapi(mapa_y, mapa_x + 1)) {
        smjerovi[DESNO] = true;
        smjerovi_vec.push_back(DESNO);
    }
    if (naMapi(mapa_y, mapa_x - 1)) {
        smjerovi[LIJEVO] = true;
        smjerovi_vec.push_back(LIJEVO);
    }
    if (naMapi(mapa_y + 1, mapa_x)) {
        smjerovi[DOLJE] = true;
        smjerovi_vec.push_back(DOLJE);
    }
    if (naMapi(mapa_y - 1, mapa_x)) {
        smjerovi[GORE] = true;
        smjerovi_vec.push_back(GORE);
    }

    Smjer obrnuti_smjer;

    switch (smjer)
    {
    case GORE:
        obrnuti_smjer = DOLJE;
        break;
    case DOLJE:
        obrnuti_smjer = GORE;
        break;
    case LIJEVO:
        obrnuti_smjer = DESNO;
        break;
    case DESNO:
        obrnuti_smjer = LIJEVO;
        break;
    case STOP:
        obrnuti_smjer = STOP;
        break;
    }

    /* smjer se mijenja samo kada duh nije na presjeku radi izbjegavanja oscilacija */
    if (smjerovi_vec.size() > 2)
    {
        QPair<int, int> vector = chase_strategy(this);

        int udaljenost_x = vector.first;
        int udaljenost_y = vector.second;

        if (udaljenost_y > 0)
        {
            if (smjerovi[DOLJE] && smjer != GORE)
                smjer = DOLJE;
            else {
                if (udaljenost_x >= 0)
                {
                    if (smjerovi[DESNO])
                        smjer = DESNO;
                    else if (smjerovi[LIJEVO])
                        smjer = LIJEVO;
                } else {
                    if (smjerovi[LIJEVO])
                        smjer = LIJEVO;
                    else if (smjerovi[DESNO])
                        smjer = DESNO;
                }
            }
        } else if (udaljenost_y < 0) {
            if (smjerovi[GORE] && smjer != DOLJE)
                smjer = GORE;
            else {
                if (udaljenost_x >= 0) {
                    if (smjerovi[DESNO])
                        smjer = DESNO;
                    else if (smjerovi[LIJEVO])
                        smjer = LIJEVO;
                } else {
                    if (smjerovi[LIJEVO])
                        smjer = LIJEVO;
                    else if (smjerovi[DESNO])
                        smjer = DESNO;
                }
            }
        } else if (udaljenost_x > 0) {
            if (smjerovi[DESNO] && smjer != LIJEVO)
                smjer = DESNO;
            else {
                if (smjerovi[GORE])
                    smjer = GORE;
                else if(smjerovi[DOLJE])
                    smjer = DOLJE;
            }
        } else if (udaljenost_x < 0) {
            if (smjerovi[LIJEVO] && smjer != DESNO)
                smjer = LIJEVO;
            else {
                if (smjerovi[GORE])
                    smjer = GORE;
                else if(smjerovi[DOLJE])
                    smjer = DOLJE;
            }
        }
    } else if (smjerovi_vec.size() == 2) {
        if (smjerovi[smjer] == false)
        {
            /* ukoliko je duh u ćošku */
            if (smjerovi[0] == obrnuti_smjer)
                smjer = smjerovi_vec[1];
            else
                smjer = smjerovi_vec[0];
        }
    }
}

void Ghost::pomak()
{
    if (vrijeme_pustanja > 0)
        vrijeme_pustanja--;

    int duh_x = static_cast<int>(x());
    int duh_y = static_cast<int>(y());
    /* koordinate bloka na mapi */
    int trenutni_x = (duh_x - PacMan->geo_x) / W;
    int trenutni_y = (duh_y - PacMan->geo_y) / W;
    /* ostatak piksela za uklopiti blok */
    int ostatak_x = (duh_x - PacMan->geo_x) % W;
    int ostatak_y = (duh_y - PacMan->geo_y) % W;

    /* kada je duh u potpunosti uklopljen na blok, odluči da li će se mijenjati smjer */
    if (ostatak_x == 0 && ostatak_y == 0) {
        // update ghost's coordinate in map
        mapa_x = trenutni_x;
        mapa_y = trenutni_y;
        /* ganjaj pacmana */
        if (pusten)
            ganjajPacmana();
        else {
            if (vrijeme_pustanja == 0) {
                izadjiIzKaveza();
                /* kada duhovi dođu do vrata pusti ih */
                if (PacMan->mapa[mapa_y][mapa_x]->getTip() == VRATA) {
                    pusten = true;
                }
            } else {
                /* duhovi ostaju u kavezu */
                postaviRandomSmjer();
            }
        }
    }

    switch (smjer)
    {
    case STOP:
        break;
    case GORE:
        pomakGore();
        break;
    case DOLJE:
        pomakDolje();
        break;
    case LIJEVO:
        pomakLijevo();
        break;
    case DESNO:
        pomakDesno();
        break;
    }

    /* igra izgubljena ukoliko pacman udari duha */
    if (collidesWithItem(PacMan->pacman, Qt::IntersectsItemShape))
            PacMan->status = Game::LOSE;
}
