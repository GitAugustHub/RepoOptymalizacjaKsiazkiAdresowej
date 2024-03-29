#include <iostream>
#include <vector>
#include <unistd.h>
#include <stdlib.h>
#include <cstdio>
#include <fstream>
#include <string> 
#include <sstream>
using namespace std;

struct Adresat 
{
int id, idUzytkownika;
string imie, nazwisko, nrTelefonu, email, adres;
};

struct Uzytkownik
{
int idUzytkownika;
string nazwaUzytkownika, haslo;
bool zalogowany;
};

string wczytajLinie()
{
  string wejscie = "";
  getline(cin, wejscie);
  return wejscie;
}

char wczytajZnak() 
{
  string wejscie = "";
  char znak = {0};
  while (true)
  {
    getline(cin, wejscie);
    if (wejscie.length() == 1)
    {
      znak = wejscie[0];
      break;
    }
    cout << "To nie jest pojedynczy znak. Wpisz ponownie" << endl;
  }
  return znak;
}

void czyscEkran() 
{
    cout << "\033[2J\033[1;1H"; // Sekwencja czyszczaca ANSI
}

void czekajNaWcisniecieKlawisza() 
{
    cout << "Nacisnij klawisz ENTER by kontynuowac...";
    cin.ignore(numeric_limits <streamsize> :: max(), '\n');
}

void wyswietlKontakt(Adresat adresat)
{ 
    cout << adresat.id << endl;
    cout << "Imie: " << adresat.imie << endl;
    cout << "Nazwisko: " << adresat.nazwisko << endl;
    cout << "Numer telefonu: " << adresat.nrTelefonu << endl;
    cout << "Email: " << adresat.email << endl;
    cout << "Adres: " << adresat.adres << endl;
}

void zapisanieKsiazkiDoPliku(const vector <Adresat> &adresaci, int idEdytowanegoAdresata)
{
    string linia;
    bool czyEdytowany = false;
    
    ifstream plikOryginalny("Ksiazka_adresowa.txt");
    ofstream plikTymczasowy("Adresaci_tymczasowy.txt");

    if (!plikOryginalny.is_open())
    {
        cerr << "Nie udalo sie otworzyc pliku oryginalnego" << endl;
        return;
    }

    while (getline(plikOryginalny, linia))
    {
        stringstream ss(linia);
        string idString;
        Adresat edytowanyAdresat;
        getline(ss, idString, '|');
        int id = stoi(idString);

        if (id == idEdytowanegoAdresata)
        {
            if (!czyEdytowany)
            {
                // Edytujemy linię dla adresata o danym ID
                
                for (const Adresat &adresat : adresaci)
                {
                    if (adresat.id == idEdytowanegoAdresata)
                    {
                        edytowanyAdresat = adresat;
                        break;
                    }
                }
                plikTymczasowy << edytowanyAdresat.id << "|"
                               << edytowanyAdresat.idUzytkownika << "|"
                               << edytowanyAdresat.imie << "|"
                               << edytowanyAdresat.nazwisko << "|"
                               << edytowanyAdresat.nrTelefonu << "|"
                               << edytowanyAdresat.email << "|"
                               << edytowanyAdresat.adres << "|" << endl;

                czyEdytowany = true;
            }
            else
            {
                // Pomijamy linię, bo już edytowaliśmy adresata
                plikTymczasowy << linia << endl;
            }
        }
        else
        {
            // Zapisujemy linie z danymi adresatow innych uzytkownikow
            plikTymczasowy << linia << endl;
        }
    }

    plikOryginalny.close();
    plikTymczasowy.close();

    if (remove("Ksiazka_adresowa.txt") != 0)
    {
        cerr << "Nie udalo sie usunac oryginalnego pliku" << endl;
        return;
    }

    if (rename("Adresaci_tymczasowy.txt", "Ksiazka_adresowa.txt") != 0)
    {
        cerr << "Nie udalo sie zmienic nazwy tymczasowego pliku" << endl;
        return;
    }

    cout << "Zaktualizowano plik z danymi" << endl;
    sleep(3);
}

void zapisanieUzytkownikaDoPliku(const vector <Uzytkownik> &uzytkownicy)
{
    fstream plik;
    plik.open("Uzytkownicy.txt", ios::out);
    if (plik.good() == false) 
    {
        cout << "Plik z lista uzytkownikow nie istnieje!" << endl;
        czekajNaWcisniecieKlawisza();
    }  
    else 
    {
        for (int i = 0; i < uzytkownicy.size(); ++i)
        {
        plik << uzytkownicy[i].idUzytkownika << "|";
        plik << uzytkownicy[i].nazwaUzytkownika << "|";
        plik << uzytkownicy[i].haslo << "|" << endl;
        }
    plik.close();
  }
}

int dodanieOsobyDoKsiazkiAdresowej(vector <Adresat> &adresaci, int idZalogowanegoUzytkownika, int idOstatniegoAdresata)
{
  Adresat adresat;
  string imie, nazwisko, nrTelefonu, email, adres;
  czyscEkran();

  if (idOstatniegoAdresata == 0)
  {
    adresat.id = 1;
  }
  else
  {
    adresat.id = idOstatniegoAdresata + 1;
  }
  adresat.idUzytkownika = idZalogowanegoUzytkownika; 
  cout << "Podaj imie: "; 
  adresat.imie = wczytajLinie();
  cout << "Podaj nazwisko: "; 
  adresat.nazwisko = wczytajLinie();
  cout << "Podaj nr telefonu: "; 
  adresat.nrTelefonu = wczytajLinie();
  cout << "Podaj adres email: "; 
  adresat.email = wczytajLinie();
  cout << "Podaj adres: "; 
  adresat.adres = wczytajLinie();

  adresaci.push_back(adresat);

  ofstream plik("Ksiazka_adresowa.txt", ios::app); // Otwarcie pliku w trybie dodawania tekstu

    if (!plik.is_open())
    {
        cerr << "Nie udalo sie otworzyc pliku Ksiazka_adresowa.txt" << endl;
        return idOstatniegoAdresata;
    }

    plik << adresat.id << "|" << adresat.idUzytkownika << "|" << adresat.imie << "|"
         << adresat.nazwisko << "|" << adresat.nrTelefonu << "|" << adresat.email << "|" << adresat.adres << "|" << endl;

    plik.close();
  
  cout << "Dodano do ksiazki adresowej" << endl;
  czekajNaWcisniecieKlawisza();

  return adresat.id;
}

pair < vector <Adresat>, int >  wczytywanieZnajomychDoStruktury(const string nazwaPliku, int idZalogowanegoUzytkownika)
{
    vector <Adresat> adresaci;
    Adresat adresat;
    string id, idUzytkownika;
    string liniaZDanymi = "";
    int idOstatniegoAdresata = 0;

    ifstream plik(nazwaPliku);

    if (!plik.is_open())
    {
        cerr << "Nie udalo sie otworzyc pliku " << nazwaPliku << endl;
        return make_pair(adresaci, idOstatniegoAdresata); // Zwrocenie pustego wektora w przypadku błedu
    }

    while (getline(plik, liniaZDanymi))
    {
        stringstream ss(liniaZDanymi);
        getline(ss, id, '|');
        adresat.id = stoi(id);
        idOstatniegoAdresata = adresat.id;
        getline(ss, idUzytkownika, '|');
        adresat.idUzytkownika = stoi(idUzytkownika);
        getline(ss, adresat.imie, '|');
        getline(ss, adresat.nazwisko, '|');
        getline(ss, adresat.nrTelefonu, '|');
        getline(ss, adresat.email, '|');
        getline(ss, adresat.adres, '|');

        if (adresat.idUzytkownika == idZalogowanegoUzytkownika)
        {
            adresaci.push_back(adresat);
        }
    }
    plik.close();
    return make_pair(adresaci, idOstatniegoAdresata);
}

vector <Uzytkownik> wczytywanieUzytkownikowDoStruktury(const string nazwaPliku)
{
  vector <Uzytkownik> uzytkownicy;
  Uzytkownik uzytkownik;
  string idUzytkownika;
  string liniaZDanymi = "";
  
  ifstream plik(nazwaPliku);

  if (!plik.is_open()) 
    {
        cerr << "Nie udalo sie otworzyc pliku " << nazwaPliku << endl;
        return uzytkownicy; // Zwrocenie pustego wektora w przypadku błedu
    }

    while (getline (plik, liniaZDanymi)) 
    {
        stringstream ss(liniaZDanymi);
        getline(ss, idUzytkownika, '|');
        uzytkownik.idUzytkownika = atoi(idUzytkownika.c_str());
        getline(ss, uzytkownik.nazwaUzytkownika, '|');
        getline(ss, uzytkownik.haslo, '|');
        
    uzytkownicy.push_back(uzytkownik);
    }
    plik.close();
    return uzytkownicy;
}

void wyszukajOsobePoImieniu(vector <Adresat> &adresaci, int idZalogowanegoUzytkownika)
{
    string imie;
    bool osobaOdnaleziona = false;
        
        czyscEkran();
        cout << "Podaj imie: ";
        cin >> imie;

        for (const Adresat adresat : adresaci) 
        {
            if (adresat.idUzytkownika == idZalogowanegoUzytkownika && adresat.imie == imie) 
            {
                cout << "Znalazlem osobe o imieniu " << imie << endl;
                sleep(1);
                cout << "Oto dane z ksiazki adresowej: " << endl;
                wyswietlKontakt(adresat);
                sleep(3);
                osobaOdnaleziona = true;
            }
        }
        if (!osobaOdnaleziona) 
        {
        cout << "Osoby o imieniu: " << imie << " nie ma w ksiazce adresowej." << endl;
        sleep(3);
        }
}

void wyszukajOsobePoNazwisku(vector <Adresat> &adresaci, int idZalogowanegoUzytkownika)
{
    string nazwisko;
    bool osobaOdnaleziona = false;
    
        czyscEkran();
        cout << "Podaj nazwisko: ";
        cin >> nazwisko;
        
        for (const Adresat adresat : adresaci) 
        {
            if (adresat.idUzytkownika == idZalogowanegoUzytkownika && adresat.nazwisko == nazwisko) 
            {
                cout << "Znalazlem osobe o nazwisku " << nazwisko << endl;
                sleep(1);
                cout << "Oto dane z ksiazki adresowej: " << endl;
                wyswietlKontakt(adresat);
                sleep(3);
                osobaOdnaleziona = true;
            }
        }
        if (osobaOdnaleziona == false) 
        {
        cout << "Osoby o nazwisku: " << nazwisko << " nie ma w ksiazce adresowej." << endl;
        sleep(3);
        }
}

void wyswietlKsiazkeAdresowa(vector <Adresat> &adresaci, int idZalogowanegoUzytkownika)
{
  czyscEkran();

  if (adresaci.empty()) 
    {
        cout << "Brak znajomych w ksiazce adresowej." << endl;
        czekajNaWcisniecieKlawisza();
        return;
    }
    cout << "Ksiazka adresowa:" << endl;
    for (int i = 0; i < adresaci.size(); ++i) 
    {
        if (adresaci[i].idUzytkownika == idZalogowanegoUzytkownika)
        {
            cout << adresaci[i].id << endl;
            cout << "Imie: " << adresaci[i].imie << endl;
            cout << "Nazwisko: " << adresaci[i].nazwisko << endl;
            cout << "Numer telefonu: " << adresaci[i].nrTelefonu << endl;
            cout << "Email: " << adresaci[i].email << endl;
            cout << "Adres: " << adresaci[i].adres << endl << endl;  
        }
           
    }
  czekajNaWcisniecieKlawisza();
}

void usunAdresataOPodanymId(vector <Adresat> &adresaci, int idZalogowanegoUzytkownika)
{
    int idUsuwanegoAdresata;
    string linia;
    bool znalezionoAdresata = false;

    cout << "Podaj ID adresata, ktory ma zostac usuniety: ";
    cin >> idUsuwanegoAdresata;
    
    ifstream plikOryginalny("Ksiazka_adresowa.txt");
    ofstream plikTymczasowy("Adresaci_tymczasowy.txt");

    if (!plikOryginalny.is_open())
    {
        cerr << "Nie udalo sie otworzyc pliku oryginalnego" << endl;
        return;
    }

    while (getline(plikOryginalny, linia))
    {
        stringstream ss(linia);
        string idString;
        Adresat edytowanyAdresat;

        getline(ss, idString, '|');
        int id = stoi(idString);
        getline(ss, idString, '|');
        int idUzytkownika = stoi(idString);

        if (id == idUsuwanegoAdresata && idUzytkownika == idZalogowanegoUzytkownika)
        {
            for (vector <Adresat> :: iterator itr = adresaci.begin(); itr != adresaci.end(); itr++)
            {
                if (itr -> id == id && itr -> idUzytkownika == idZalogowanegoUzytkownika)
                {
                    adresaci.erase(itr);
                    znalezionoAdresata = true;
                    break; 
                }
            }
        }
        else
        {
            // Zapisujemy linie z danymi adresatow innych uzytkownikow
            plikTymczasowy << linia << endl;
        }
    }

    plikOryginalny.close();
    plikTymczasowy.close();

    if (remove("Ksiazka_adresowa.txt") != 0)
    {
        cerr << "Nie udalo sie usunac oryginalnego pliku" << endl;
        return;
    }

    if (rename("Adresaci_tymczasowy.txt", "Ksiazka_adresowa.txt") != 0)
    {
        cerr << "Nie udalo sie zmienic nazwy tymczasowego pliku" << endl;
        return;
    }

    if (!znalezionoAdresata)
    {
        cout << "Nie znaleziono adresata o podanym ID w Twojej książce adresowej." << endl;
        sleep(3);
    }
    else
    {
        cout << "Usunieto wybranego adresata" << endl;
        sleep(3); 
    }
    
}

void edytujAdresata(vector <Adresat> &adresaci, int idZalogowanegoUzytkownika)
{
    int idWybranegoAdresata;
    char wybor;
    bool znalezionoAdresata = false;

    cout << "Podaj ID adresata, ktory ma zostac edytowany: ";
    cin >> idWybranegoAdresata;

    for (vector <Adresat> :: iterator itr = adresaci.begin(); itr != adresaci.end(); itr++)
    {
        if (itr -> id == idWybranegoAdresata && itr -> idUzytkownika == idZalogowanegoUzytkownika)
        {
        czyscEkran();
        znalezionoAdresata = true;
        cout << "Edycja wybranego adresata. Wybierz dane do aktualizacji:" << endl << endl;
        cout << "1. Imie." << endl;
        cout << "2. Nazwisko" << endl;
        cout << "3. Numer telefonu" << endl;
        cout << "4. Email" << endl;
        cout << "5. Adres" << endl;
        cout << "9. Powrot do menu" << endl << endl;
        cout << "Wybierz numer i wciśnij ENTER" << endl;
      
        wybor = wczytajZnak();

            switch (wybor)
            {
            case '1':
                cout << "Podaj nowe imie: ";
                itr -> imie =  wczytajLinie();
                zapisanieKsiazkiDoPliku(adresaci, idWybranegoAdresata);
                break;
            case '2':
                cout << "Podaj nowe nazwisko: ";
                itr -> nazwisko =  wczytajLinie();
                zapisanieKsiazkiDoPliku(adresaci, idWybranegoAdresata);
                break;
            case '3':
                cout << "Podaj nowy numer telefonu: ";
                itr -> nrTelefonu =  wczytajLinie();
                zapisanieKsiazkiDoPliku(adresaci, idWybranegoAdresata);
                break;
            case '4':
                cout << "Podaj nowy email: ";
                itr -> email =  wczytajLinie();
                zapisanieKsiazkiDoPliku(adresaci, idWybranegoAdresata);
                break;
            case '5':
                cout << "Podaj nowy adres zamieszkania: ";
                itr -> adres =  wczytajLinie();
                zapisanieKsiazkiDoPliku(adresaci, idWybranegoAdresata);
                break;
            case '6':
                cout << endl << "Powrot do menu uzytkownika" << endl << endl;
                break;
            default:
                cout << endl << "Nie ma takiej opcji w menu! Powrot do menu uzytkownika." << endl << endl;
                break;
            }
        }
    }
    
    if (!znalezionoAdresata)
    {
        cout << "Nie znaleziono adresata o podanym ID w Twojej książce adresowej." << endl;
        sleep(3);
    }
        
}

void zmianaHasla(vector<Uzytkownik> &uzytkownicy, int idZalogowanegoUzytkownika)
{
    string noweHaslo;
    cout << "Podaj nowe haslo: ";
    noweHaslo = wczytajLinie();

    for (Uzytkownik &uzytkownik : uzytkownicy)
    {
        if (uzytkownik.idUzytkownika == idZalogowanegoUzytkownika)
        {
            uzytkownik.haslo = noweHaslo;
            zapisanieUzytkownikaDoPliku(uzytkownicy); // Zapisz zmiany do pliku
            cout << "Haslo zostalo zmienione pomyslnie!" << endl;
            czekajNaWcisniecieKlawisza();
            return;
        }
    }

    cout << "Nie udalo sie zmienic hasla. Nie znaleziono zalogowanego uzytkownika." << endl;
    czekajNaWcisniecieKlawisza();
}

void wylogowanieUzytkownika(vector<Uzytkownik> &uzytkownicy, int idZalogowanegoUzytkownika)
{
    for (Uzytkownik &uzytkownik : uzytkownicy)
    {
        if (uzytkownik.idUzytkownika == idZalogowanegoUzytkownika)
        {
            uzytkownik.zalogowany = false;
            cout << "Wylogowano pomyslnie!" << endl;
            czekajNaWcisniecieKlawisza();
            return;
        }
    }
    cout << "Blad podczas wylogowywania. Nie znaleziono zalogowanego uzytkownika." << endl;
    czekajNaWcisniecieKlawisza();
}

int logowanieUzytkownika(vector <Uzytkownik> &uzytkownicy)
{
    int idZalogowanegoUzytkownika = 0;
    string nazwaUzytkownika, haslo;
    czyscEkran();
    cout << "------ LOGOWANIE ------" << endl;
    cout << "Podaj nazwe uzytkownika: ";
    nazwaUzytkownika = wczytajLinie();
    cout << "Podaj haslo: ";
    haslo = wczytajLinie();

    for (Uzytkownik &uzytkownik : uzytkownicy)
    {
        if (uzytkownik.nazwaUzytkownika == nazwaUzytkownika && uzytkownik.haslo == haslo)
        {
            uzytkownik.zalogowany = true;
            idZalogowanegoUzytkownika = uzytkownik.idUzytkownika;
            cout << "Zalogowano pomyslnie!" << endl;
            czekajNaWcisniecieKlawisza();
            break;
        }
    }
    if (idZalogowanegoUzytkownika == 0)
    {
        cout << "Niepoprawna nazwa uzytkownika lub haslo!" << endl;
        czekajNaWcisniecieKlawisza();
    }
    return idZalogowanegoUzytkownika;
}

void uruchomienieKsiazkiAdresowej(vector <Uzytkownik> &uzytkownicy, vector<Adresat> &adresaci, int idZalogowanegoUzytkownika, int idOstatniegoAdresata) 
{ 
  char wybor;
  string nazwaPliku = "Ksiazka_adresowa.txt";

  while(true)
    {
      czyscEkran();
      cout << "Ksiazka adresowa" << endl << endl;
      cout << "1. Dodaj nowa osobe" << endl;
      cout << "2. Wyszukaj osobe po imieniu" << endl;
      cout << "3. Wyszukaj osobe po nazwisku" << endl;
      cout << "4. Wyswietl wszystkie zapisane kontakty" << endl;
      cout << "5. Usun adresata" << endl;
      cout << "6. Edytuj adresata" << endl;
      cout << "7. Zmien haslo" << endl;
      cout << "8. Wyloguj sie" << endl;
      cout << "9. Zakoncz program" << endl << endl;
      cout << "Wybierz numer i wciśnij ENTER" << endl;
      
      wybor = wczytajZnak();

      switch (wybor)
      {
        case '1':
            idOstatniegoAdresata = dodanieOsobyDoKsiazkiAdresowej(adresaci, idZalogowanegoUzytkownika, idOstatniegoAdresata);
            break;
        case '2':
            wyszukajOsobePoImieniu(adresaci, idZalogowanegoUzytkownika);
            break;
        case '3':
            wyszukajOsobePoNazwisku(adresaci, idZalogowanegoUzytkownika);
            break;
        case '4':
            wyswietlKsiazkeAdresowa(adresaci, idZalogowanegoUzytkownika);
            break;
        case '5':
            usunAdresataOPodanymId(adresaci, idZalogowanegoUzytkownika);
            break;
        case '6':
            edytujAdresata(adresaci, idZalogowanegoUzytkownika);
            break;
        case '7':
            zmianaHasla(uzytkownicy, idZalogowanegoUzytkownika);
            break;
        case '8':
            wylogowanieUzytkownika(uzytkownicy, idZalogowanegoUzytkownika);
            return;
        case '9':
            exit(0);
            break;
      }
    }
}

void rejestracjaNowegoUzytkownika(vector <Uzytkownik> &uzytkownicy)
{
  Uzytkownik uzytkownik;
  string nazwaUzytkownika, haslo;
  czyscEkran();

  if (uzytkownicy.empty() == true)
  {
    uzytkownik.idUzytkownika = 1;
  }
  else
  {
    uzytkownik.idUzytkownika = uzytkownicy.back().idUzytkownika + 1;
  }
  cout << "------ REJESTRACJA NOWEGO UZYTKOWNIKA ------" << endl;
  cout << "Podaj nazwe nowego uzytkownika: "; 
  uzytkownik.nazwaUzytkownika = wczytajLinie();
  cout << "Podaj haslo: "; 
  uzytkownik.haslo = wczytajLinie();

  uzytkownicy.push_back(uzytkownik);

  zapisanieUzytkownikaDoPliku(uzytkownicy); 
  
  cout << "Dodano nowego uzytkownika" << endl;
  czekajNaWcisniecieKlawisza();
}

// ponownie definiuje funkcje bo kompilator jej nie widzial i sugerowal blad
void zapisanieKsiazkiDoPliku(const vector <Adresat> &adresaci, int idEdytowanegoAdresata);

int main()
{
  char wybor;
  int idZalogowanegoUzytkownika;
  int idOstatniegoAdresata = 0;
  string nazwaPlikuZUzytkownikami = "Uzytkownicy.txt";
  vector <Adresat> adresaci;
  vector <Uzytkownik> uzytkownicy = wczytywanieUzytkownikowDoStruktury(nazwaPlikuZUzytkownikami); 

  string nazwaPlikuZPelnaKsiazkaAdresowa = "Ksiazka_adresowa.txt";

  while(true)
    {
      czyscEkran();
      cout << "Ksiazka adresowa" << endl << endl;
      cout << "1. Logowanie" << endl;
      cout << "2. Rejestracja" << endl;
      cout << "9. Zakoncz program" << endl << endl;
      cout << "Wybierz numer i wciśnij ENTER" << endl;
      
      wybor = wczytajZnak();

      switch (wybor)
      {
        case '1':
            idZalogowanegoUzytkownika = logowanieUzytkownika(uzytkownicy);
            if (idZalogowanegoUzytkownika > 0)
            {
                pair < vector<Adresat>, int > wynik = wczytywanieZnajomychDoStruktury(nazwaPlikuZPelnaKsiazkaAdresowa, idZalogowanegoUzytkownika);
                adresaci = wynik.first;
                idOstatniegoAdresata = wynik.second;

                uruchomienieKsiazkiAdresowej(uzytkownicy, adresaci, idZalogowanegoUzytkownika, idOstatniegoAdresata);
            }
            break;
        case '2':
            rejestracjaNowegoUzytkownika(uzytkownicy);
            break;
        case '9':
            exit(0);
            break;
      }
    }

  return 0;
}

