#include "igra.h"

GameObject::GameObject(TipObjekta t, QPixmap pixmap)
    : QGraphicsPixmapItem(pixmap)
{
    tip = t;
}

GameObject::TipObjekta GameObject::getTip() { return tip; }

int GameObject::getX() { return mapa_x; }

int GameObject::getY() { return mapa_y; }

int GameObject::getRezultat() { return rezultat; }

void GameObject::setRezultat(int rez) { rezultat = rez; }

void GameObject::setSmjer(Smjer s) { smjer = s; }

GameObject::Smjer GameObject::getSmjer() { return smjer; }

GameObject::Smjer GameObject::getNextSmjer() { return next_smjer; }

void GameObject::setNextSmjer(Smjer s) { next_smjer = s; }

GameObject::~GameObject() {}
