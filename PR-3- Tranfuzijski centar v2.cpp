#include<iostream>
#include<vector>
#include<regex>
#include<thread>
#include<mutex>
#include<string>
#include<sstream>

using namespace std;

const char *crt = "\n-------------------------------------------\n";
enum OznakaKrvneGrupe { O, A, B, AB };
const char* OznakaKrvneGrupeString[] = { "O", "A", "B", "AB" };

class Datum {
	int _dan, _mjesec, _godina;
public:
	long long int operator-(Datum d) {
		long long int a = _dan + _mjesec * 30 + _godina * 365;
		long long int b = d._dan + d._mjesec * 30 + d._godina * 365;
		return abs(a - b);
	}
	string ToString(const char* separator=".") const {
		return to_string(_dan) + string(separator) + to_string(_mjesec) + string(separator) + to_string(_godina);
	}
	Datum(const Datum& obj) {
		_dan = obj._dan;
		_mjesec = obj._mjesec;
		_godina = obj._godina;
	}
	Datum& operator=(const Datum& obj) {
		_dan = obj._dan;
		_mjesec = obj._mjesec;
		_godina = obj._godina;
		return *this;
	}
	Datum(int dan=1, int mjesec=1, int godina=2000) :
		_dan(dan), _mjesec(mjesec), _godina(godina) {}
	friend ostream& operator<<(ostream& out, Datum& obj) {
		out << obj._dan << "." << obj._mjesec << "." << obj._godina;
		return out;
	}
};

template <class T1, class T2, int max>
class Kolekcija {
	T1 _elementi1[max];
	T2 _elementi2[max];
	int _trenutnoElemenata;
	int _keyElement; // kljucni element, a podrazumjevano T1 predstavlja kljucni element i tada je vrijednost 1, 
	//a u slucaju da je T2 onda ce vrijednost biti 2
public:
	void Sort(string sort) {

		if (_keyElement == 1) {
			if (sort == "ASC") {
				for (int i = 1; i < _trenutnoElemenata; i++) {
					int j = i;
					while (j > 0 && _elementi1[j] < _elementi1[j - 1]) {
						T1 temp01 = _elementi1[j];
						T2 temp02 = _elementi2[j];
						_elementi1[j] = _elementi1[j - 1];
						_elementi2[j] = _elementi2[j - 1];
						_elementi1[j - 1] = temp01;
						_elementi2[j - 1] = temp02;
						j--;
					}
				}
			}
			if (sort == "DESC") {
				for (int i = 1; i < _trenutnoElemenata; i++) {
					int j = i;
					while (j > 0 && _elementi1[j] > _elementi1[j - 1]) {
						T1 temp01 = _elementi1[j];
						T2 temp02 = _elementi2[j];
						_elementi1[j] = _elementi1[j - 1];
						_elementi2[j] = _elementi2[j - 1];
						_elementi1[j - 1] = temp01;
						_elementi2[j - 1] = temp02;
						j--;
					}
				}
			}
		}

		if (_keyElement == 2) {
			if (sort == "ASC") {
				for (int i = 1; i < _trenutnoElemenata; i++) {
					int j = i;
					while (j > 0 && _elementi2[j] < _elementi2[j - 1]) {
						T1 temp01 = _elementi1[j];
						T2 temp02 = _elementi2[j];
						_elementi1[j] = _elementi1[j - 1];
						_elementi2[j] = _elementi2[j - 1];
						_elementi1[j - 1] = temp01;
						_elementi2[j - 1] = temp02;
						j--;
					}
				}
			}
			if (sort == "DESC") {
				for (int i = 1; i < _trenutnoElemenata; i++) {
					int j = i;
					while (j > 0 && _elementi2[j] > _elementi2[j - 1]) {
						T1 temp01 = _elementi1[j];
						T2 temp02 = _elementi2[j];
						_elementi1[j] = _elementi1[j - 1];
						_elementi2[j] = _elementi2[j - 1];
						_elementi1[j - 1] = temp01;
						_elementi2[j - 1] = temp02;
						j--;
					}
				}
			}
		}
		
	}
	Kolekcija & operator=(const Kolekcija& obj) {
		_trenutnoElemenata = obj._trenutnoElemenata;
		_keyElement = obj._keyElement;
		for (int i = 0; i < _trenutnoElemenata; i++) {
			_elementi1[i] = obj._elementi1[i];
			_elementi2[i] = obj._elementi2[i];
		}
		return *this;
	}
	Kolekcija(const Kolekcija& obj) {
		_trenutnoElemenata = obj._trenutnoElemenata;
		_keyElement = obj._keyElement;
		for (int i = 0; i < _trenutnoElemenata; i++) {
			_elementi1[i] = obj._elementi1[i];
			_elementi2[i] = obj._elementi2[i];
		}
	}
	Kolekcija(int keyElement = 1) {
		_trenutnoElemenata = 0;
		_keyElement = keyElement;
	}
	bool AddElement(T1 ele1, T2 ele2) {
		if (_trenutnoElemenata == max)
			return false;
		_elementi1[_trenutnoElemenata] = ele1;
		_elementi2[_trenutnoElemenata] = ele2;
		_trenutnoElemenata++;
		return true;
	}

	void SetElement2(int lokacija, T2 ele2) {
		_elementi2[lokacija] = ele2;
	}
	void SetKeyElement(int keyElement) {
		if (keyElement < 1 || keyElement>2)
			throw exception("Nedozvoljena vrijednost!");
		_keyElement = keyElement;
	}
	int GetTrenutno() const { return _trenutnoElemenata; }
	int GetMax() const { return max; }

	T1 GetElement1(int lokacija) const {
		if (lokacija < 0 || lokacija >= _trenutnoElemenata)
			throw exception("Nepostojeca lokacija!");
		return _elementi1[lokacija];
	}
	T2 GetElement2(int lokacija) const {
		if (lokacija < 0 || lokacija >= _trenutnoElemenata)
			throw exception("Nepostojeca lokacija!");
		return _elementi2[lokacija];
	}
	friend ostream &operator<<(ostream &COUT, const Kolekcija &obj) {
		for (int i = 0; i < obj.GetTrenutno(); i++)
			COUT << obj.GetElement1(i) << " " << obj.GetElement2(i) << endl;
		return COUT;
	}
};

class KrvnaGrupa {
	/*postoje 4 krvne grupe koje su navedene u enumeracije OznakaKrvneGrupe, pri cemu svaka od navedenih moze imati pozitivan (+) 
	i negativan (-) Rh faktor*/
	OznakaKrvneGrupe _oznaka;//npr: AB
	char _rhFaktor; // + ili -
					/*prethodno pomenuto je izuzetno bitno iz razloga sto postoje jasna pravila vezana za darivanje krvi 
					tj. koji primalac moze primiti krv od kojeg donatora sto je prikazano u tabeli, 
					a naredna dva vector-a su zaduzena da cuvaju informacije o tome, 
					npr. za krvnu grupu A+ vector donatori ce cuvati vrijednosti: A+ i AB+ */
	vector<KrvnaGrupa> _donatori; //krvne grupe kojima odredjena krvna grupa moze donirati krv.
	vector<KrvnaGrupa> _primaoci; //krvne grupe od kojih odredjena krvna grupa moze primiti krv.
public:
	vector<KrvnaGrupa> getDonatori() { return _donatori; }
	bool operator==(KrvnaGrupa b) {
		if (_oznaka == b._oznaka && _rhFaktor == b._rhFaktor) {
			return true;
		}
		else return false;
	}
	string ToString() const {
		stringstream s1;
		s1 <<OznakaKrvneGrupeString[_oznaka] << _rhFaktor;
		return s1.str();
	}
	KrvnaGrupa() {

	}
	KrvnaGrupa & operator=(const KrvnaGrupa& obj) {
		_oznaka = obj._oznaka;
		_rhFaktor = obj._rhFaktor;
		_donatori = obj._donatori;
		_primaoci = obj._primaoci;
		return *this;
	}
	KrvnaGrupa(const KrvnaGrupa& obj) :_oznaka(obj._oznaka){
		_rhFaktor = obj._rhFaktor;
		_donatori = obj._donatori;
		_primaoci = obj._primaoci;
	}
	KrvnaGrupa(OznakaKrvneGrupe oznaka, char rhFaktor) :
		_oznaka(oznaka), _rhFaktor(rhFaktor) {}
	void SetDonatori(vector<KrvnaGrupa> donatori) {
		_donatori = donatori;
	}
	void SetPrimaoci(vector<KrvnaGrupa> primaoci) {
		_primaoci = primaoci;
	}
	/*
	Ispis podataka o krvnoj grupi treba biti u formatu:
	-------------------------------------------
	Krvna grupa -> 0+
	-------------------------------------------
	Donori  ->  0+, A+, B+, AB+
	Primaoci ->	0+, 0-
	-------------------------------------------
	*/
	friend ostream& operator<<(ostream& out, KrvnaGrupa& obj) {
		out << crt << endl;
		out << "Krvna grupa -> " << OznakaKrvneGrupeString[obj._oznaka] << obj._rhFaktor << endl;
		out << crt << endl;
		out << "Donori ->";
		for (int i = 0; i < obj._donatori.size(); i++)
			out << obj._donatori[i].ToString() << ", ";
		out << endl;
		out << "Primaoci ->";
		for(int i=0; i<obj._primaoci.size(); i++)
			out << obj._primaoci[i].ToString()<<", ";
		out << endl;
		out << crt << endl;
		return out;
	}
};

//Klasu proglasiti apstraktnom
class Osoba {
protected:
	char* _imePrezime;
	KrvnaGrupa _krvnaGrupa;
public:
	string getImePrezime() { return string(_imePrezime); }
	KrvnaGrupa getKrvnaGrupa() { return _krvnaGrupa; }
	Osoba & operator=(const Osoba& obj) {
		_krvnaGrupa = obj._krvnaGrupa;
		int size = strlen(obj._imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, obj._imePrezime);
		return *this;
	}
	Osoba(const Osoba& obj): _krvnaGrupa(obj._krvnaGrupa) {
		int size = strlen(obj._imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, obj._imePrezime);
	}
	Osoba() : _krvnaGrupa(){
		_imePrezime = nullptr;
	}
	Osoba(const char* imePrezime, KrvnaGrupa krvnaGrupa) : _krvnaGrupa(krvnaGrupa) {
		int size = strlen(imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, imePrezime);
	}
	virtual ~Osoba() {
		delete[] _imePrezime;
		_imePrezime = nullptr;
	}
	virtual void Info() = 0 {

	}
};

class Donator : public Osoba {
	//u slucaju da broj telefona nije u validnom formatu potrebno ga je postaviti na podrazumijevanu vrijednost: 000-000-000
	string _telefon; //regex: 000/000-000 ili 000-000-000
	Datum _datumPosljednjegDoniranja;
	bool _podsjetiMe;
	bool _kontaktirajMe;
	bool ValidanFormat(string telefon) {
		string pravilo1 = ("([0-9]{3,3})(\\/)([0-9]{3,3})(\\-)([0-9]{3,3})");
		string pravilo2 = ("([0-9]{3,3})(\\-)([0-9]{3,3})(\\-)([0-9]{3,3})");

		if (regex_match(telefon, regex(pravilo1)) || regex_match(telefon, regex(pravilo2))) {
			return true;
		}
		else return false;
	}
public:
	bool getKontakt() { return _kontaktirajMe; }
	void setDatumDoniranja(Datum d) {
		_datumPosljednjegDoniranja = d;
	}
	Datum getDatumPDoniranja() { return _datumPosljednjegDoniranja; }
	void Info() {
		cout << Osoba::_imePrezime << " -- "<<Osoba::_krvnaGrupa.ToString()<< endl;
		cout << _telefon << endl;
		cout << _datumPosljednjegDoniranja << endl;

	}
	Donator & operator=(Donator& obj) {
		_krvnaGrupa = obj._krvnaGrupa;
		int size = strlen(obj._imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, obj._imePrezime);
		_telefon = obj._telefon;
		_datumPosljednjegDoniranja = obj._datumPosljednjegDoniranja;
		_kontaktirajMe = obj._kontaktirajMe;
		_podsjetiMe = obj._podsjetiMe;
		return *this;
	}
	Donator(const Donator& obj) : Osoba(obj), _datumPosljednjegDoniranja(obj._datumPosljednjegDoniranja){
		if (ValidanFormat(obj._telefon)) {
			_telefon = obj._telefon;
		}
		else _telefon = "000-000-000";
		_podsjetiMe = obj._podsjetiMe;
		_kontaktirajMe = obj._kontaktirajMe;

	}
	Donator(const char* imePrezime, KrvnaGrupa krvnaGrupa, string telefon, Datum dpd, bool remind = true, bool contact = true)
		: Osoba(imePrezime, krvnaGrupa), _datumPosljednjegDoniranja(dpd) {
		if (ValidanFormat(telefon))
			_telefon = telefon;
		else
			_telefon = "000-000-000";
		_podsjetiMe = remind;
		_kontaktirajMe = contact;
	}
};

class Zahtjev {
	string _ustanova;
	Datum _datumZahtjeva;
	KrvnaGrupa _krvnaGrupa;
	double _kolicina;
public:
	Datum getDatum() { return _datumZahtjeva; }
	double getKolicina() { return _kolicina; }
	KrvnaGrupa getKrvnaGrupa() { return _krvnaGrupa; }
	Zahtjev & operator=(const Zahtjev& obj) {
		_ustanova = obj._ustanova;
		_datumZahtjeva = obj._datumZahtjeva;
		_krvnaGrupa = obj._krvnaGrupa;
		_kolicina = obj._kolicina;
		return *this;
	}
	Zahtjev(const Zahtjev& obj) : _ustanova(obj._ustanova), _datumZahtjeva(obj._datumZahtjeva), _krvnaGrupa(obj._krvnaGrupa){
		_kolicina = obj._kolicina;

	}
	Zahtjev(string ustanova, Datum datum, KrvnaGrupa krvnaGrupa, double kolicina) :
		_ustanova(ustanova), _datumZahtjeva(datum), _krvnaGrupa(krvnaGrupa), _kolicina(kolicina) { }

	friend ostream& operator<<(ostream& COUT, const Zahtjev& obj) {
		COUT << "Ustanova: " << obj._ustanova << endl;
		COUT << "Datum: " << obj._datumZahtjeva.ToString() << endl;
		COUT << "Krvna grupa: " << obj._krvnaGrupa.ToString() << endl;
		COUT << "Kolicina: " << obj._kolicina << endl;
		return COUT;
	}
};
mutex m;
void SENDSMS(string imePrezime) {
	m.lock();
	cout << "Poštovani " << imePrezime << ", " << endl;
	cout << " molimo Vas da hitno donirate krv!!!" << endl;
	m.unlock();
}
class TransfuzijskiCentar {
	//stanje zaliha za svaku pojedinu krvnu grupu
	Kolekcija<KrvnaGrupa*, double, 8> _zalihe;
	//evidentira svaku donaciju krvi
	Kolekcija<Osoba *, double, 100> _donacije;
	vector<Zahtjev> _zahtjevi;
public:
	~TransfuzijskiCentar() {
		for (int i = 0; i < _zalihe.GetTrenutno(); i++) {
			delete _zalihe.GetElement1(i);
		}
		for (int i = 0; i < _donacije.GetTrenutno(); i++) {
			delete _donacije.GetElement1(i);
		}
	}
	//printa zahvalnice (zahvaljujemo se ime i prezime donatoru na ukupno doniranih X doza krvi) za TOP 2 donatora krvi
	void GetZahvalniceZahvalnice(int top) {
		Kolekcija<Osoba*, double, 100> zahvalnice = _donacije;
		zahvalnice.SetKeyElement(2);
		zahvalnice.Sort("DESC");

		if (top >= zahvalnice.GetTrenutno()) {
			return;
		}

		for (int i = 0; i < top; i++) {
			cout << "Zahvaljujemo se " << zahvalnice.GetElement1(i)->getImePrezime() << " donatoru na ukupno doniranih " << zahvalnice.GetElement2(i) << " doza krvi!" << endl;
		}



	}
	/*ukoliko transfuzijski centar posjeduje zahtijevane kolicine na stanju, odmah ce ih ustupiti prema zahtjevu,
	a u slucaju da ne posjeduje onda ce (koristeci multithreading) kontaktirati (poslati SMS poruku sa odgovarajucim sadrzajem)
	sve donore koji zadovoljavaju sljedece uslove:
	- mogu donirati krv zahtijevanoj krvnoj grupi
	- oznacili su da zele biti kontaktirani
	- nisu davali krv u posljednja 3 mjeseca
	*/
	void DodajZahtjev(Zahtjev zahtjev) {
		bool found = false;
		for (int i = 0; i < _zalihe.GetTrenutno(); i++) {
			if (*_zalihe.GetElement1(i) == zahtjev.getKrvnaGrupa()) {
				if (_zalihe.GetElement2(i) == zahtjev.getKolicina()) {
					found = true;
				}
			}
		}

		if (found == true) {
			_zahtjevi.push_back(zahtjev);

		}
		else {
			for (int i = 0; i < _donacije.GetTrenutno(); i++) {
				if (dynamic_cast<Donator*>(_donacije.GetElement1(i))->getKontakt() == true) {
					if (dynamic_cast<Donator*>(_donacije.GetElement1(i))->getDatumPDoniranja() - zahtjev.getDatum() >= 90) {
						bool moze_donirati = false;
						for (int j = 0; j < zahtjev.getKrvnaGrupa().getDonatori().size(); j++) {

							if (_donacije.GetElement1(i)->getKrvnaGrupa() == zahtjev.getKrvnaGrupa().getDonatori()[j]) {
								moze_donirati = true;
							}

						}
						if (moze_donirati == true) {
							thread sms(SENDSMS, _donacije.GetElement1(i)->getImePrezime());
							sms.join();
						}

					}
				}
			}
		}

	}
	/*
	prilikom svake donacije je potrebno povecati zalihe za tu krvnu grupu
	ukoliko krvna grupa vec postoji na zalihi onda se uvecava samo njena kolicina
	takodjer, donatoru se postavlja nova vrijednost datuma posljednje donacije
	*/
	//datum donacije, donator, kolicina
	void AddDonaciju(Datum datum, Osoba* osoba, double kolicina) {
		bool found_zalihe = false;
		int indeks_zaliha = -1;
		bool found_donacije = false;
		int indeks_donacije = -1;
		for (int i = 0; i < _zalihe.GetTrenutno(); i++) {
			if (*_zalihe.GetElement1(i) == osoba->getKrvnaGrupa()) {
				found_zalihe = true;
				indeks_zaliha = i;
			}
		}
		for (int i = 0; i < _donacije.GetTrenutno(); i++) {
			if (dynamic_cast<Donator*>(_donacije.GetElement1(i)) == dynamic_cast<Donator*>(osoba)) {
				found_donacije = true;
				indeks_donacije = i;
			}
		}
		if (found_zalihe == true) {
			_zalihe.SetElement2(indeks_zaliha, _zalihe.GetElement2(indeks_zaliha) + kolicina);
		}
		else {
			_zalihe.AddElement(new KrvnaGrupa(osoba->getKrvnaGrupa()), kolicina);
		}
		if (found_donacije == true) {
			_donacije.SetElement2(indeks_donacije, _donacije.GetElement2(indeks_donacije) + kolicina);
			dynamic_cast<Donator*>(osoba)->setDatumDoniranja(datum);
		}
		else {
			_donacije.AddElement(new Donator(*dynamic_cast<Donator*>(osoba)), kolicina);
			dynamic_cast<Donator*>(osoba)->setDatumDoniranja(datum);
		}

	}
};

void main() {

	/**************************************************************************
	1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
	2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR CE BITI OZNACENO KAO "RE"
	3. SPASAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URADJENOG ZADATKA
	4. PROGRAMSKI CODE SE TAKODJER NALAZI U FAJLU code.TXT
	5. NAZIVI FUNKCIJA MORAJU BITI IDENTIÈNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U.OSTALE, POMOÆNE FUNKCIJE MOŽETE IMENOVATI PO ŽELJI. TAKOÐER, MOŽETE DODAVATI KOLIKO ŽELITE TESTNIH PODATAKA
	****************************************************************************/
	Datum dat_12_01_2018(12, 1, 2018), dat_01_02_2018(1, 2, 2018);
	cout << dat_01_02_2018.ToString() << endl; //podrazumijevani separator je tacka ".", pa ispisuje 1.2.2018
	cout << dat_12_01_2018.ToString("/") << endl; //separator je proslijedjen, pa ispisuje 12/1/2018

	Kolekcija<int, int, 10> kolekcija1;
	for (size_t i = 0; i < kolekcija1.GetMax(); i++)
		if (!kolekcija1.AddElement(i, 170000 + i))
			cout << "Elementi " << i << " i " << 170000 + i << " nisu dodati u kolekciju" << endl;

	cout << kolekcija1.GetElement1(0) << " " << kolekcija1.GetElement2(0) << endl;

	kolekcija1.Sort("ASC"); //kljucne rijeci za sortiranje su ASC i DESC, a sortiranje se vrsi prema kljucnom elementu
	cout << kolekcija1 << endl << endl << endl;
	kolekcija1.Sort("DESC");
	cout << kolekcija1 << endl;

	kolekcija1.SetKeyElement(2);//postavlja kljucni element na T2

	kolekcija1.Sort("DESC"); //sada se sortiranje vrsi po elementu T2
	cout << kolekcija1 << endl;

	KrvnaGrupa
		O_poz(O, '+'), O_neg(O, '-'),
		A_poz(A, '+'), A_neg(A, '-'),
		B_poz(B, '+'), B_neg(B, '-'),
		AB_poz(AB, '+'), AB_neg(AB, '-');

	vector<KrvnaGrupa> donori_O_poz{ O_poz, A_poz, B_poz, AB_poz };
	O_poz.SetDonatori(donori_O_poz);
	//ili krace napisano
	O_poz.SetPrimaoci(vector<KrvnaGrupa>{O_poz, O_neg});
	A_poz.SetDonatori(vector<KrvnaGrupa>{A_poz, AB_poz});
	A_poz.SetPrimaoci(vector<KrvnaGrupa>{O_poz, A_poz, O_neg, A_neg});

	B_poz.SetDonatori(vector<KrvnaGrupa>{B_poz, AB_poz});
	B_poz.SetPrimaoci(vector<KrvnaGrupa>{O_poz, B_poz, O_neg, B_neg});
	/*
	Ispis podataka o krvnoj grupi treba biti u formatu:
	-------------------------------------------
	Krvna grupa -> 0+
	-------------------------------------------
	Donori  ->  0+, A+, B+, AB+
	Primaoci ->	0+, 0-
	-------------------------------------------
	*/
	cout << O_poz.ToString() << endl;
	cout << O_poz << endl;
	Osoba * jasmin = new Donator("Jasmin Azemovic", B_poz, "061-111-222", Datum(12, 2, 2017), true, true);
	Osoba * adel = new Donator("Adel Handzic", A_neg, "061-222-333", Datum(9, 1, 2017), true, true);
	Osoba * goran = new Donator("Goran Skondric", B_neg, "061-333-444", Datum(9, 3, 2017), true, true);

	jasmin->Info();
	TransfuzijskiCentar tcMostar;
	/*
	prilikom svake donacije je potrebno povecati zalihe za tu krvnu grupu
	ukoliko krvna grupa vec postoji na zalihi onda se uvecava samo njena kolicina
	takodjer, donatoru se postavlja nova vrijednost datuma posljednje donacije
	*/
	//datum donacije, donator, kolicina
	tcMostar.AddDonaciju(Datum(20, 5, 2017), jasmin, 2.5);
	tcMostar.AddDonaciju(Datum(20, 5, 2017), adel, 3);
	tcMostar.AddDonaciju(Datum(6, 5, 2017), goran, 1.2);


	tcMostar.AddDonaciju(Datum(10, 9, 2017), jasmin, 2);
	tcMostar.AddDonaciju(Datum(18, 10, 2017), adel, 1.8);
	tcMostar.AddDonaciju(Datum(15, 9, 2017), goran, 3.8);

	Zahtjev zahtjev_0_poz("Tranfuziologija KCUS", Datum(18, 2, 2018), O_poz, 15),
		zahtjev_0_neg("Tranfuziologija Bakir Nakas", Datum(20, 2, 2018), O_neg, 8);

	/*ukoliko transfuzijski centar posjeduje zahtijevane kolicine na stanju, odmah ce ih ustupiti prema zahtjevu,
	a u slucaju da ne posjeduje onda ce (koristeci multithreading) kontaktirati (poslati SMS poruku sa odgovarajucim sadrzajem)
	sve donore koji zadovoljavaju sljedece uslove:
	- mogu donirati krv zahtijevanoj krvnoj grupi
	- oznacili su da zele biti kontaktirani
	- nisu davali krv u posljednja 3 mjeseca
	*/
	tcMostar.DodajZahtjev(zahtjev_0_poz);
	tcMostar.DodajZahtjev(zahtjev_0_neg);

	//printa zahvalnice (zahvaljujemo se ime i prezime donatoru na ukupno doniranih X doza krvi) za TOP 2 donatora krvi
	tcMostar.GetZahvalniceZahvalnice(2);

	delete adel;
	delete jasmin;
	delete goran;
	system("PAUSE");
}