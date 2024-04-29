#include "igra.h"

QPair<int, int> strategija1(Ghost* ghost)
{
    Pacman *pacman = ghost->PacMan->pacman;

    int udaljenost_x = pacman->getX() - ghost->getX();
    int udaljenost_y = pacman->getY() - ghost->getY();

    return QPair<int, int>(udaljenost_x, udaljenost_y);
}

QPair<int, int> strategija2(Ghost* ghost)
{
    Pacman *pacman = ghost->PacMan->pacman;

    int udaljenost_x = pacman->getX() - ghost->getX();
    int udaljenost_y = pacman->getY() - ghost->getY();

    switch(pacman->getSmjer()) {
    case GameObject::GORE:
        udaljenost_y -= 4;
        break;
    case GameObject::DOLJE:
        udaljenost_y += 4;
        break;
    case GameObject::LIJEVO:
        udaljenost_x -= 4;
        break;
    case GameObject::DESNO:
        udaljenost_x += 4;
        break;
    default:
        break;
    }

    return QPair<int, int>(udaljenost_x, udaljenost_y);
}

QPair<int, int> strategija3(Ghost* ghost)
{
    Pacman *pacman = ghost->PacMan->pacman;
    Ghost *red_ghost = ghost->PacMan->duh[Ghost::CRVEN];

    int udaljenost_x = (pacman->getX() - red_ghost->getX()) * 2 - ghost->getX();
    int udaljenost_y = (pacman->getY() - red_ghost->getY()) * 2 - ghost->getY();

    return QPair<int, int>(udaljenost_x, udaljenost_y);
}

QPair<int, int> strategija4(Ghost* ghost)
{
    Pacman *pacman = ghost->PacMan->pacman;

    int udaljenost_x = pacman->getX() - ghost->getX();
    int udaljenost_y = pacman->getY() - ghost->getY();
    if (udaljenost_x * udaljenost_x + udaljenost_y * udaljenost_y < 64) {
        udaljenost_x = 2 - ghost->getX();
        udaljenost_y = 2 - ghost->getY();
    }

    return QPair<int, int>(udaljenost_x, udaljenost_y);
}
