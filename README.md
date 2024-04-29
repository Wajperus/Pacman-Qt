# Pacman-Qt 🕹️

Sveučilišni odjel za stručne studije Split | Sveučiliše u Splitu

**Projekat**: Pacman igrica u C++ koristeći QT framework  
**Izradili**: Ćorić Boris, Lovrić Josip

## Realizacije Pacman API

Glavni `main.cpp` fajl koristi `<QApplication>` klasu za stvaranje aplikacije.

Korištenjem `<QMainWindow>` klase kreira se glavni prozor na kojemu su postavljene
labele (*rezultat, pobjeda/poraz*) i realizirano upravljanje ulazom sa tipkovnice.

```CPP
void MainWindow::keyPressEvent(QKeyEvent *e) {
    switch (e->key()) {
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
```

Dodavanje objekata na mapu vrši se čitanjem tekstualne datoteke mape predstavljene
kao matrica brojeva i slova kojima je respektivno označen pojedini objekt na mapi.
Komunikacija među objektima koristi signale i slotove: potpis signala mora odgovarati
potpisu slota, što je realizirano pomoću `QTimer` klase i `connect` funkcije. Detekcija
kolizije duha i pacmana koristi `collidesWithItem` funkciju koja vraća *true* ukoliko
je došlo do sudara.

```CPP
    pacman_timer = new QTimer(this);
    /*        objekt1         signal        objekt2        slot         */
    connect(pacman_timer, SIGNAL(timeout()), this, SLOT(pacman_handler()));
    pacman_timer->start(INTERVAL_PACMANA);
```

Animacije se realiziraju pomoću QVector-a QPixmap objekata PNG slika preuzetih
sa interneta. Za svakog duha duha (Inky, Pinky, Blinky, Clyde) realizirana je po
jedna strategija za ganjanje pacmana korištenjem `QPair<int, int>` koji vraćaju 
udaljenost na x i y osi. Ispod je prikazana najjednostavnija strategija koja 
računa udaljenost pacmana i duha oduzimanjem njihovih x i y koordinata.

```CPP
QPair<int, int> strategija1(Ghost* ghost) {
    Pacman *pacman = ghost->PacMan->pacman;

    int udaljenost_x = pacman->getX() - ghost->getX();
    int udaljenost_y = pacman->getY() - ghost->getY();

    return QPair<int, int>(udaljenost_x, udaljenost_y);
}
```
