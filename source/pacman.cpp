#include "igra.h"

#define W (GameObject::sirina_objekta)

/* konstruiramo vektor animacija unutar kojeg smještavamo
 * slike respektivno za stranu kretanja pacmana */
Pacman::Pacman() : GameObject(
    GameObject::PACMAN, QPixmap(":/a3.png"))
{
    smjer = STOP;
    next_smjer = STOP;
    index_animacije = 2;
    animacija[DESNO].push_back(QPixmap(":/a1.png"));
    animacija[DESNO].push_back(QPixmap(":/a2.png"));
    animacija[DESNO].push_back(QPixmap(":/a3.png"));
    animacija[DESNO].push_back(QPixmap(":/a4.png"));
    animacija[DESNO].push_back(QPixmap(":/a5.png"));
    animacija[DESNO].push_back(QPixmap(":/a6.png"));
    animacija[DESNO].push_back(QPixmap(":/a5.png"));
    animacija[DESNO].push_back(QPixmap(":/a4.png"));
    animacija[DESNO].push_back(QPixmap(":/a3.png"));
    animacija[DESNO].push_back(QPixmap(":/a2.png"));

    animacija[GORE].push_back(QPixmap(":/a1.png"));
    animacija[GORE].push_back(QPixmap(":/b2.png"));
    animacija[GORE].push_back(QPixmap(":/b3.png"));
    animacija[GORE].push_back(QPixmap(":/b4.png"));
    animacija[GORE].push_back(QPixmap(":/b5.png"));
    animacija[GORE].push_back(QPixmap(":/b6.png"));
    animacija[GORE].push_back(QPixmap(":/b5.png"));
    animacija[GORE].push_back(QPixmap(":/b4.png"));
    animacija[GORE].push_back(QPixmap(":/b3.png"));
    animacija[GORE].push_back(QPixmap(":/b2.png"));

    animacija[LIJEVO].push_back(QPixmap(":/a1.png"));
    animacija[LIJEVO].push_back(QPixmap(":/c2.png"));
    animacija[LIJEVO].push_back(QPixmap(":/c3.png"));
    animacija[LIJEVO].push_back(QPixmap(":/c4.png"));
    animacija[LIJEVO].push_back(QPixmap(":/c5.png"));
    animacija[LIJEVO].push_back(QPixmap(":/c6.png"));
    animacija[LIJEVO].push_back(QPixmap(":/c5.png"));
    animacija[LIJEVO].push_back(QPixmap(":/c4.png"));
    animacija[LIJEVO].push_back(QPixmap(":/c3.png"));
    animacija[LIJEVO].push_back(QPixmap(":/c2.png"));

    animacija[DOLJE].push_back(QPixmap(":/a1.png"));
    animacija[DOLJE].push_back(QPixmap(":/d2.png"));
    animacija[DOLJE].push_back(QPixmap(":/d3.png"));
    animacija[DOLJE].push_back(QPixmap(":/d4.png"));
    animacija[DOLJE].push_back(QPixmap(":/d5.png"));
    animacija[DOLJE].push_back(QPixmap(":/d6.png"));
    animacija[DOLJE].push_back(QPixmap(":/d5.png"));
    animacija[DOLJE].push_back(QPixmap(":/d4.png"));
    animacija[DOLJE].push_back(QPixmap(":/d3.png"));
    animacija[DOLJE].push_back(QPixmap(":/ad2.png"));
}

void Pacman::pomakGore()
{
    index_animacije++;
    if (index_animacije == animacija[GORE].size())
        index_animacije = 0;

    setPixmap(animacija[GORE][index_animacije]);
    setY(static_cast<int>(y()) - 1);
}

void Pacman::pomakLijevo()
{
    index_animacije++;
    if (index_animacije == animacija[LIJEVO].size())
        index_animacije = 0;

    setPixmap(animacija[LIJEVO][index_animacije]);
    setX(static_cast<int>(x()) - 1);
}

void Pacman::pomakDolje()
{
    index_animacije++;
    if (index_animacije == animacija[DOLJE].size())
        index_animacije = 0;

    setPixmap(animacija[DOLJE][index_animacije]);
    setY(static_cast<int>(y()) + 1);
}

void Pacman::pomakDesno()
{
    index_animacije++;
    if (index_animacije == animacija[DESNO].size())
        index_animacije = 0;

    setPixmap(animacija[DESNO][index_animacije]);
    setX(static_cast<int>(x()) + 1);
}

void Pacman::pojediLopticu(int trenutni_y, int trenutni_x)
{
    GameObject *obj = PacMan->mapa[trenutni_y][trenutni_x];

    switch (obj->getTip())
    {
    case LOPTICA:
        PacMan->rezultat += obj->getRezultat();
        PacMan->broj_loptica--;
        break;
    default:
        return;
    }

    /* kada pacman pojede lopticu blok se nadopuni praznim prostorom */
    QPixmap blankpix;
    PacMan->mapa[mapa_y][mapa_x] = new GameObject(GameObject::PRAZNO, blankpix);
    PacMan->mapa[trenutni_y][trenutni_x] = this;
    delete obj;
}

bool Pacman::naMapi(int i, int j)
{
    if (i < 0 || j < 0)
        return false;

    if (i >= PacMan->visina_mape || j >= PacMan->sirina_mape)
        return false;

    switch (PacMan->mapa[i][j]->getTip())
    {
    case ZID:
    case VRATA:
        return false;
    default:
        return true;
    }
}

/* kontrola kretnji pacmana */
void Pacman::pomak()
{
    /* pretvaramo x i y koordinate u int */
    int pacman_x = static_cast<int>(x());
    int pacman_y = static_cast<int>(y());
    /* x i y koordinate na mapi */
    int trenutni_x = (pacman_x - PacMan->geo_x) / W;
    int trenutni_y = (pacman_y - PacMan->geo_y) / W;
    /* ostatak pixela za nadopuniti blok */
    int ostatak_x = (pacman_x - PacMan->geo_x) % W;
    int ostatak_y = (pacman_y - PacMan->geo_y) % W;
    Smjer next_dir = getNextSmjer();

    if (ostatak_x == 0) {
        if (ostatak_y == 0) {
            /* kada pacman u potpunosti odgovara bloku jede lopticu */
            pojediLopticu(trenutni_y, trenutni_x);

            /* osvježi koordinate na mapi */
            mapa_x = trenutni_x;
            mapa_y = trenutni_y;

            /* pobjeda ako su sve loptice pojedene */
            if (PacMan->broj_loptica == 0)
            {
                PacMan->status = Game::WIN;
                return;
            }
        }

        /* kada pacman u potpunosti odgovara bloku provjeri promjenu smjera */
        switch (next_dir)
        {
        case STOP:
            smjer = next_dir;
            break;
        case GORE:
            if (naMapi(mapa_y - 1, mapa_x))
                smjer = next_dir;
            break;
        case DOLJE:
            if (naMapi(mapa_y + 1, mapa_x))
                smjer = next_dir;
            break;
        case LIJEVO:
            if (ostatak_y == 0 && naMapi(mapa_y, mapa_x - 1))
                smjer = next_dir;
            break;
        case DESNO:
            if (ostatak_y == 0 && naMapi(mapa_y, mapa_x + 1))
                smjer = next_dir;
            break;
        }
    } else if (ostatak_y == 0) {
        switch (next_dir)
        {
        case STOP:
            smjer = next_smjer;
            break;
        case LIJEVO:
            if (naMapi(mapa_y, mapa_x - 1))
                smjer = next_smjer;
            break;
        case DESNO:
            if (naMapi(mapa_y, mapa_x + 1))
                smjer = next_smjer;
            break;
        default:
            break;
        }
    }

    /* ukoliko dođe do zida */
    switch (smjer)
    {
    case STOP:
        break;
    case GORE:
        if (ostatak_y == 0 && !naMapi(mapa_y - 1, mapa_x)) {
            smjer = STOP;
            next_smjer = STOP;
        } else { pomakGore(); }
        break;
    case DOLJE:
        if (ostatak_y == 0 && !naMapi(mapa_y + 1, mapa_x)) {
            smjer = STOP;
            next_smjer = STOP;
        } else { pomakDolje(); }
        break;
    case LIJEVO:
        if (ostatak_x == 0 && !naMapi(mapa_y, mapa_x - 1)) {
            smjer = STOP;
            next_smjer = STOP;
        } else { pomakLijevo(); }
        break;
    case DESNO:
        if (ostatak_x == 0 && !naMapi(mapa_y, mapa_x + 1)) {
            smjer = STOP;
            next_smjer = STOP;
        } else { pomakDesno(); }
        break;
    }
}
