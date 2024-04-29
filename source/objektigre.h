#ifndef OBJEKTIGRE_H
#define OBJEKTIGRE_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QTimer>
#include <QVector>

class Game;

class GameObject : public QGraphicsPixmapItem
{
public:
    /* enumeracija tipova objekata koji se nalaze na mapi */
    enum TipObjekta {LOPTICA, ZID, VRATA, PACMAN, DUH, PRAZNO };
    /* mogući smjerovi kretanja */
    enum Smjer {GORE = 0, DOLJE = 1, LIJEVO = 2, DESNO = 3, STOP = 4};
    static const int sirina_objekta = 20;      // širina objekta u pikselima

    GameObject(TipObjekta, QPixmap);           // za dani tip objekta prikazuje sliku
    ~GameObject();
    TipObjekta getTip();
    /* vraća koordinate na mapi */
    int getX();
    int getY();
    int getRezultat();
    void setRezultat(int);
    void setSmjer(Smjer);
    void setNextSmjer(Smjer);
    Smjer getSmjer();
    Smjer getNextSmjer();

    friend class Game;
    friend class Ghost;

protected:
    int mapa_x, mapa_y;               // koordinate na mapi
    Smjer smjer, next_smjer;          // trenutni i sljedeći smjer kretanja kontroliran w, a, s, d tipkama
    TipObjekta tip;
    int rezultat;
};

class Pacman : public GameObject
{
public:
    Pacman();
    void pomak();
    Game *PacMan;   // objekt igre Pacman

    friend class Game;

private:
    /* metode za postavljanje smjera kretanja */
    void pomakGore();
    void pomakDolje();
    void pomakLijevo();
    void pomakDesno();
    void pojediLopticu(int, int);
    bool naMapi(int, int);           // provjera može li Pacman na Mapu mapa[i][j]

    QVector<QPixmap> animacija[4];   // animacije
    int index_animacije;
};

class Ghost : public GameObject
{
public:
    enum Boja {CRVEN = 0, ZUT = 1, ROZ = 2, ZELEN = 3};
    const static int broj_duhova = 4;
    Game *PacMan;                     // objekt igre Pacman

    Ghost(int);
    void pomak();
    Boja getBoja();

    friend class Game;
    friend class Pacman;

private:
    /* metode za postavljanje smjera kretanja */
    void pomakGore();
    void pomakDolje();
    void pomakLijevo();
    void pomakDesno();

    void postaviRandomSmjer();
    void izadjiIzKaveza();
    void ganjajPacmana();

    QPair<int, int> (*chase_strategy)(Ghost*);  // funkcijski pokazivači na strategiju ganjanja
    bool naMapi(int, int);                      // provjera može li duh na Mapu mapa[i][j]

    Boja boja;
    QVector<QPixmap> animacija[4];              // animacije

    int index_animacije;
    int vrijeme_pustanja;                       // vrijeme potrebno da duhovi izađu iz kaveza
    bool pusten;
};

/* strategije po kojima duhovi ganjaju pacmana su prikazane
 * kao QPair vektora od duha ka pacmanu */
QPair<int, int> strategija1(Ghost*);
QPair<int, int> strategija2(Ghost*);
QPair<int, int> strategija3(Ghost*);
QPair<int, int> strategija4(Ghost*);

#endif // OBJEKTIGRE_H
