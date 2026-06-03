//(ps.:kommentelve ai-val, szval nem en irtam fenttartasokkal kezelendo)
#pragma once
#include <algorithm>
#include <list>
#include <utility>

// ============================================================================
// ABSZTRAKT BÁZISOSZTÁLY
// ============================================================================
template <typename T>
class self_organizing_list
{
protected:
    // Referenciaként tároljuk az eredeti listát, hogy a módosítások
    // (cserék, törlések, beszúrások) a main-beli listákban is végbemenjenek.
    std::list<T>& data;

public:
    self_organizing_list(std::list<T>& d) : data(d) {}
    virtual ~self_organizing_list() {}

    // Tisztán virtuális függvény a pontos érték alapú kereséshez.
    virtual bool contains(const T& value) = 0;
};

// ============================================================================
// TRANSPOSE STRATÉGIA (Szomszédos felcserélés)
// ============================================================================
template <typename T>
class transpose : public self_organizing_list<T>
{
public:
    transpose(std::list<T>& d) : self_organizing_list<T>(d) {}

    // Sima keresés: Ha megvan az elem, eggyel előrébb cseréljük a szomszédjával.
    bool contains(const T& value)
    {
        typename std::list<T>::iterator it = this->data.begin();

        while (it != this->data.end())
        {
            if (*it == value)
            {
                // Csak akkor cserélünk, ha nem a legelső elemet találtuk meg
                if (it != this->data.begin())
                {
                    typename std::list<T>::iterator prev = it;
                    --prev;
                    T temp = *prev;
                    *prev = *it;
                    *it = temp;
                }
                return true; // Érték alapú keresésnél az első találat után azonnal leállunk
            }
            ++it;
        }
        return false;
    }

    // Predikátum alapú keresés: VÉGIGMEGY A TELJES LISTÁN!
    // Mivel a csere lokális (csak szomszédos), a lista végén lévő X tulajdonságú
    // elemeket nem érinti az, ha a lista elején is cseréltünk valamit.
    // Ezért minden elemet meg kell vizsgálni, és ha igaz rá a feltétel, "meglökni" előre.
    template <typename CMP>
    bool contains_if(CMP cmp)
    {
        bool found = false;
        typename std::list<T>::iterator it = this->data.begin();

        while (it != this->data.end())
        {
            if (cmp(*it))
            {
                found = true; // Megjegyezzük, hogy volt legalább egy találat

                if (it != this->data.begin())
                {
                    typename std::list<T>::iterator prev = it;
                    --prev;

                    // Helyi értékcsere a szomszéddal
                    T temp = *prev;
                    *prev = *it;
                    *it = temp;
                }
            }
            ++it; // NEM lépünk ki (nincs return true), megyünk tovább a lista végéig!
        }
        return found;
    }
};

// ============================================================================
// MOVE_TO_BACK STRATÉGIA (Hátra mozgatás)
// ============================================================================
template<typename T>
class move_to_back : public self_organizing_list<T>
{
public:
    move_to_back(std::list<T>& d) : self_organizing_list<T>(d) {}

    // Sima keresés: Ha megvan az elem, a std::list::splice segítségével 
    // O(1) idő alatt átkötjük a lista legvégére.
    bool contains(const T& value)
    {
        typename std::list<T>::iterator it = this->data.begin();

        while (it != this->data.end())
        {
            if (*it == value)
            {
                this->data.splice(
                    this->data.end(),
                    this->data,
                    it
                );
                return true;
            }
            ++it;
        }
        return false;
    }

    // Predikátum alapú keresés: AZONNAL LEÁLL AZ ELSŐ TALÁLATNÁL (return true)!
    // Mivel ez a művelet globális (kitépi az elemet és a lista legvégére dobja),
    // ha nem állnánk meg, a mögötte lévő többi X tulajdonságú elemet is mögé pakolnánk.
    // Ez visszanyalna, mert a hátraküldött elemek egymást előznék meg, teljesen 
    // felborítva a teszt által elvárt végső sorrendet.
    template<typename CMP>
    bool contains_if(CMP cmp)
    {
        typename std::list<T>::iterator it = this->data.begin();

        while (it != this->data.end())
        {
            if (cmp(*it))
            {
                T val = *it;
                this->data.erase(it);   // Kiejtjük a jelenlegi helyéről
                this->data.push_back(val); // Átdobjuk a sor legvégére
                return true; // AZONNAL KILÉPÜNK, hogy megőrizzük a többi elem struktúráját!
            }
            ++it;
        }
        return false;
    }
};
