#include <iostream>
#include<vector>
#include<string>
#include<regex>
#include<mutex>
#include<thread>
#include<sstream>
using namespace std;

const char *crt = "\n-------------------------------------------\n";
enum OznakaKrvneGrupe { O, A, B, AB };
ostream& operator<<(ostream& out, const OznakaKrvneGrupe& obj) {
	if (obj == 0)
		out << "O";
	if (obj == A)
		out << "A";
	if (obj == B)
		out << "B";
	if (obj == AB)
		out << "AB";

	return out;
}
enum Sortiranje { ASC, DESC };
const char* OznakaKrvneGrupeString[] = { "O", "A", "B", "AB" };

class Datum {
	int * _dan, *_mjesec, *_godina;
public:
	long long int operator-(Datum d) {
		long long int a = *_dan + *_mjesec * 30 + *_godina * 365;
		long long int b = *d._dan + *d._mjesec * 30 + *d._godina * 365;
		return a - b;
	}
	Datum(int dan=1, int mjesec=1, int godina=2000) {
		_dan = new int(dan), _mjesec = new int(mjesec), _godina = new int(godina);
	}
	Datum(const Datum& obj) {
		_dan = new int(*obj._dan);
		_mjesec = new int(*obj._mjesec);
		_godina = new int(*obj._godina);
	}
	Datum& operator=(const Datum& obj) {
		_dan = new int(*obj._dan);
		_mjesec = new int(*obj._mjesec);
		_godina = new int(*obj._godina);
		return *this;
	}
	~Datum() { delete _dan; delete _mjesec; delete _godina; }
	friend ostream &operator<<(ostream &COUT, const Datum &obj) {
		COUT << *obj._dan << " " << *obj._mjesec << " " << *obj._godina << endl;
		return COUT;
	}
};

template <class T1, class T2, int max, Sortiranje sortiranje = ASC>
class Kolekcija {
	T1 * _elementi1[max];
	T2 * _elementi2[max];
	int _trenutnoElemenata;
	bool _sortiranje;
public:
	Kolekcija(bool sortiranje = true) {
		_trenutnoElemenata = 0;
		_sortiranje = sortiranje;
		for (int i = 0; i < max; i++) {
			_elementi1[i] = nullptr;
			_elementi2[i] = nullptr;
		}
	}
	Kolekcija(const Kolekcija& obj) {
		_trenutnoElemenata = obj._trenutnoElemenata;
		_sortiranje = obj._sortiranje;
		for (int i = 0; i < _trenutnoElemenata; i++) {
			_elementi1[i] = new T1(*obj._elementi1[i]);
			_elementi2[i] = new T2(*obj._elementi2[i]);
		}
		for (int i = _trenutnoElemenata; i < max; i++) {
			_elementi1[i] = nullptr;
			_elementi2[i] = nullptr;
		}
	}
	Kolekcija& operator=(Kolekcija& obj) {
		_trenutnoElemenata = obj._trenutnoElemenata;
		_sortiranje = obj._sortiranje;
		for (int i = 0; i < _trenutnoElemenata; i++) {
			_elementi1[i] = new T1(*obj._elementi1[i]);
			_elementi2[i] = new T2(*obj._elementi2[i]);
		}
		for (int i = _trenutnoElemenata; i < max; i++) {
			_elementi1[i] = nullptr;
			_elementi2[i] = nullptr;
		}
		return *this;
	}

	~Kolekcija() {
		for (size_t i = 0; i < _trenutnoElemenata; i++) {
			delete _elementi1[i]; _elementi1[i] = nullptr;
			delete _elementi2[i]; _elementi2[i] = nullptr;
		}
	}
	bool AddElement(T1 ele1, T2 ele2) {
		/*ukoliko je vrijednost atributa _sortiranje true, prilikom svakog dodavanja elementa se treba vrsiti sortiranje po redoslijedu definisanom u formalnom template argumentu sortiranje*/
		if (_trenutnoElemenata == max)
			return false;
		_elementi1[_trenutnoElemenata] = new T1(ele1);
		_elementi2[_trenutnoElemenata] = new T2(ele2);
		_trenutnoElemenata++;

		if (_sortiranje == true) {
			if (sortiranje == ASC) {
				for (int i = 1; i < _trenutnoElemenata; i++) {
					int j = i;
					while (j > 0 && *_elementi1[j - 1] > *_elementi1[j]) {
						T1 temp1 = *_elementi1[j - 1];
						T2 temp2 = *_elementi2[j - 1];
						delete _elementi1[j - 1];
						delete _elementi2[j - 1];
						_elementi1[j - 1] = new T1(*_elementi1[j]);
						_elementi2[j - 1] = new T2(*_elementi2[j]);
						delete _elementi1[j];
						delete _elementi2[j];
						_elementi1[j] = new T1(temp1);
						_elementi2[j] = new T2[temp2];
						j--;
					}
				}
				return true;
			}
			if (sortiranje == DESC) {
				for (int i = 1; i < _trenutnoElemenata; i++) {
					int j = i;
					while (j > 0 && *_elementi1[j - 1] < *_elementi1[j]) {
						T1 temp1 = *_elementi1[j - 1];
						T2 temp2 = *_elementi2[j - 1];
						delete _elementi1[j - 1];
						delete _elementi2[j - 1];
						_elementi1[j - 1] = new T1(*_elementi1[j]);
						_elementi2[j - 1] = new T2(*_elementi2[j]);
						delete _elementi1[j];
						delete _elementi2[j];
						_elementi1[j] = new T1(temp1);
						_elementi2[j] = new T2[temp2];
						j--;
					}
				}
				return true;
			}
		}
		return true;
	}
	void SetElement2(int lokacija, T2 ele2) {
		delete _elementi2[lokacija];
		_elementi2[lokacija] = new T2(ele2);
	
	}
	void SetSortiranje(bool sortiranje) {
		_sortiranje = sortiranje;
	}
	int GetTrenutno() const { return _trenutnoElemenata; }
	int GetMax() const { return max; }

	T1 GetElement1(int lokacija) const {
		if (lokacija < 0 || lokacija >= _trenutnoElemenata)
			throw exception("Nepostojeca lokacija!");
		return *_elementi1[lokacija];
	}
	T2 GetElement2(int lokacija) const {
		if (lokacija < 0 || lokacija >= _trenutnoElemenata)
			throw exception("Nepostojeca lokacija!");
		return *_elementi2[lokacija];
	}
	friend ostream &operator<<(ostream &COUT, const Kolekcija &obj) {
		for (size_t i = 0; i < obj.GetTrenutno(); i++)
			COUT << obj.GetElement1(i) << " " << obj.GetElement2(i) << endl;
		return COUT;
	}
};

class KrvnaGrupa {
	/*postoje 4 krvne grupe koje su navedene u enumeracije OznakaKrvneGrupe, pri cemu svaka od navedenih moze imati pozitivan (+) i negativan (-) Rh faktor*/
	OznakaKrvneGrupe _oznaka;//npr: AB
	char _rhFaktor; // + ili -
					/*prethodno pomenuto je izuzetno bitno iz razloga sto postoje jasna pravila vezana za darivanje krvi tj. koji primalac moze primiti krv od kojeg donatora sto je prikazano u tabeli, a naredna dva vector-a su zaduzena da cuvaju informacije o tome, npr. za krvnu grupu A+ vector donatori ce cuvati vrijednosti: A+ i AB+ */
	vector<KrvnaGrupa *> _donatori; //krvne grupe kojima odredjena krvna grupa moze donirati krv.
	vector<KrvnaGrupa *> _primaoci; //krvne grupe od kojih OVA  krvna grupa moze primiti krv.
public:
	vector<KrvnaGrupa*> getPrimaoci() { return _primaoci; }
	bool operator<(KrvnaGrupa b) {
		stringstream s1, s2;
		s1 << _oznaka << _rhFaktor;
		s2 << b._oznaka << b._rhFaktor;
		if (s1.str() < s2.str()) {
			return true;
		}
		else return false;

	}
	bool operator>(KrvnaGrupa b) {
		stringstream s1, s2;
		s1 << _oznaka << _rhFaktor;
		s2 << b._oznaka << b._rhFaktor;
		if (s1.str() > s2.str()) {
			return true;
		}
		else return false;

	}
	bool operator==(KrvnaGrupa b) {
		if (_oznaka == b._oznaka && _rhFaktor == b._rhFaktor) {
			return true;
		}
		else return false;
	}
	string ToString() const {
	

		stringstream s1;
		s1 << _oznaka << _rhFaktor;
		
		return s1.str();
	}
	KrvnaGrupa(OznakaKrvneGrupe oznaka, char rhFaktor) :
		_oznaka(oznaka), _rhFaktor(rhFaktor) {}
	KrvnaGrupa(const KrvnaGrupa& obj) : _oznaka(obj._oznaka){
		_rhFaktor = obj._rhFaktor;
		_donatori = obj._donatori;
		_primaoci = obj._primaoci;
	}
	KrvnaGrupa& operator=(KrvnaGrupa& obj) {
		_oznaka = obj._oznaka;
		_rhFaktor = obj._rhFaktor;
		_donatori = obj._donatori;
		_primaoci = obj._primaoci;
		return *this;
	}
	void SetDonatori(vector<KrvnaGrupa *> donatori) {
		_donatori = donatori;
	}
	void SetPrimaoci(vector<KrvnaGrupa *> primaoci) {
		_primaoci = primaoci;
	}
	friend ostream& operator<<(ostream& out, const KrvnaGrupa& obj) {
		out << OznakaKrvneGrupeString[obj._oznaka] << "" << obj._rhFaktor << endl;
		out << "DONATORI::" << endl;
		for (int i = 0; i < obj._donatori.size(); i++) {

			out <<i+1<<". "<< obj._donatori[i]->ToString() << endl;
		}
		out << "PRIMAOCI::" << endl;
		for (int i = 0; i < obj._primaoci.size(); i++) {
			out << i + 1 << ". " << obj._primaoci[i]->ToString() << endl;
		}

		return out;
	}

};

class Osoba {
protected:
	char* _imePrezime;
	KrvnaGrupa * _krvnaGrupa;
public:
	KrvnaGrupa getKrvnaGrupa() { return *_krvnaGrupa; }
	bool operator==(Osoba* o) {
		if (strcmp(_imePrezime, o->_imePrezime) == 0) {
			return true;
		}
		else return false;
	}
	Osoba(const char* imePrezime, KrvnaGrupa *krvnaGrupa) : _krvnaGrupa(new KrvnaGrupa(*krvnaGrupa)) {
		_imePrezime = new char[strlen(imePrezime) + 1];
		strcpy_s(_imePrezime, strlen(imePrezime) + 1, imePrezime);
	}
	Osoba(const Osoba& obj) {
		_imePrezime = new char[strlen(obj._imePrezime) + 1];
		strcpy_s(_imePrezime, strlen(obj._imePrezime) + 1, obj._imePrezime);
		_krvnaGrupa = new KrvnaGrupa(*obj._krvnaGrupa);
	}
	Osoba& operator=(Osoba& obj) {
		_imePrezime = new char[strlen(obj._imePrezime) + 1];
		strcpy_s(_imePrezime, strlen(obj._imePrezime) + 1, obj._imePrezime);
		_krvnaGrupa = new KrvnaGrupa(*obj._krvnaGrupa);
		return *this;
	}
	virtual ~Osoba() {
		delete[] _imePrezime; _imePrezime = nullptr;
		delete _krvnaGrupa; _krvnaGrupa = nullptr;
	}
	virtual void Info() = 0;
};

class Donator : public Osoba {
	//u slucaju da email nije u validnom formatu potrebno ga je postaviti na podrazumijevanu vrijednost: info@donatori.ba
	string _email; //regex: nekiTekst@nekaDomena.com ili .ba ili .org
	Datum _datumPosljednjegDoniranja;
	bool _kontaktirajMe;
	bool ValidanFormat(string e_mail) {
		string pravilo1 = ("([a-zA-z]{1,})@([a-zA-Z]{1,})(\\.)(com|ba|org)");

		if (regex_match(e_mail,regex(pravilo1))) {
			return true;
		}
		else return false;

	}
public:
	bool getKontaktiraj() { return _kontaktirajMe; }
	void setDatum(Datum d) {
		_datumPosljednjegDoniranja = d;
	}
	Datum getDatum() { return _datumPosljednjegDoniranja; }
	Donator(const char* imePrezime, KrvnaGrupa *krvnaGrupa, string email, Datum dpd, bool contact = true)
		: Osoba(imePrezime, krvnaGrupa), _datumPosljednjegDoniranja(dpd) {
		_kontaktirajMe = contact;
		if (ValidanFormat(email))
			_email = email;
		else
			_email = "info@donatori.ba";
	}
	Donator(const Donator& obj) : Osoba(obj){
		_email = obj._email;
		_datumPosljednjegDoniranja = obj._datumPosljednjegDoniranja;
		_kontaktirajMe = obj._kontaktirajMe;
	}
	Donator& operator=(const Donator& obj) {
		_email = obj._email;
		_datumPosljednjegDoniranja = obj._datumPosljednjegDoniranja;
		_kontaktirajMe = obj._kontaktirajMe;
		return *this;
	}
	/*friend ostream& operator<<(ostream& out, const Donator& obj) {
		out << obj._imePrezime << " - " << obj._krvnaGrupa << endl;
		out << obj._email << endl;
		out << obj._datumPosljednjegDoniranja << endl;
		out << obj._kontaktirajMe << endl;
		return out;
	}*/
	void Info() {
		cout << Osoba::_imePrezime << " ";
		cout << *Osoba::_krvnaGrupa << endl;
		cout << _email << endl;
		cout << _datumPosljednjegDoniranja << endl;
		cout << _kontaktirajMe << endl;
	}
};

class Zahtjev {
	string _ustanova;
	KrvnaGrupa *_krvnaGrupa;
	double _kolicina;
public:
	Zahtjev(string ustanova, KrvnaGrupa *krvnaGrupa, double kolicina) :
		_ustanova(ustanova), _krvnaGrupa(new KrvnaGrupa(*krvnaGrupa)), _kolicina(kolicina) { }
	Zahtjev(const Zahtjev& obj) {
		_ustanova = obj._ustanova;
		_krvnaGrupa = new KrvnaGrupa(*obj._krvnaGrupa);
		_kolicina = obj._kolicina;

	}
	Zahtjev& operator=(Zahtjev& obj) {
		_ustanova = obj._ustanova;
		_krvnaGrupa = new KrvnaGrupa(*obj._krvnaGrupa);
		_kolicina = obj._kolicina;
		return *this;
	}
	friend ostream& operator<<(ostream& COUT, const Zahtjev& obj) {
		COUT << "Ustanova: " << obj._ustanova << endl;
		COUT << "Krvna grupa: " << obj._krvnaGrupa->ToString() << endl;
		COUT << "Kolicina: " << obj._kolicina << endl;
		return COUT;
	}
};
mutex m;
void e_mail(string sadrzaj) {
	m.lock();
	cout << sadrzaj << endl;
	m.unlock();
}
class TransfuzijskiCentar {
	//stanje zaliha za svaku pojedinu krvnu grupu
	Kolekcija<KrvnaGrupa, double, 8, DESC> _zalihe;
	//evidentira svaku donaciju krvi
	Kolekcija<Osoba *, double, 100> _donacije;
	vector<Zahtjev> _zahtjevi;
public:
	/*ispisuje matricu ili tablicu iz koje je vidljiva kompatibilnost izmeðu pojedinih krvnih grupa.
	pored naziva i rh faktora krvne grupe,
	u zagradi ispisati i trenutnu kolièinu na stanju. matrica prikazana na kraju zadatka*/
	void IspisiMatricuKompatibilnosti(vector<KrvnaGrupa*> krvnaGrupa) {

		for (int i = 0; i < _zalihe.GetTrenutno(); i++) {
			for (int j = 0; j < krvnaGrupa.size(); j++) {
				cout << krvnaGrupa[j]->ToString() << " -> ";
				if (*krvnaGrupa[j] == _zalihe.GetElement1(i)) {
					for(int k=0; k<_zalihe.GetElement1(i).getPrimaoci().size(); k++)
					cout << _zalihe.GetElement1(i).getPrimaoci()[k]->ToString();
				}
				
				cout << endl;
			}
		}

	}
	void DodajZahtjev(Zahtjev z) {
		_zahtjevi.push_back(z);
	}
	/*ukoliko transfuzijski centar posjeduje zahtijevane kolicine na stanju, odmah ce ih ustupiti prema zahtjevu,
	a u slucaju da ne posjeduje onda ce (koristeci multithreading) kontaktirati (poslati email poruku sa odgovarajucim sadrzajem)
	sve donore koji zadovoljavaju sljedece uslove:
	- mogu donirati krv zahtijevanoj krvnoj grupi
	- oznacili su da zele biti kontaktirani
	- nisu davali krv u posljednja 3 mjeseca
	*/
	double ProvjeriStanje(string oznaka_rhfaktor) {
		for (int i = 0; i < _zalihe.GetTrenutno(); i++) {
			if (_zalihe.GetElement1(i).ToString() == oznaka_rhfaktor) {
				return _zalihe.GetElement2(i);
			}
		}

		vector<KrvnaGrupa*> moguci_donatori;
		for (int i = 0; i < _donacije.GetTrenutno(); i++) {
			if (_donacije.GetElement1(i)->getKrvnaGrupa().ToString() == oznaka_rhfaktor) {
				moguci_donatori = _donacije.GetElement1(i)->getKrvnaGrupa().getPrimaoci();
				break;
			}
		}

		for (int i = 0; i < _donacije.GetTrenutno(); i++) {
			for (int j = 0; j < moguci_donatori.size(); j++) {
				if (_donacije.GetElement1(i)->getKrvnaGrupa() == *moguci_donatori[j]) {

					if (dynamic_cast<Donator*>(_donacije.GetElement1(i))->getKontaktiraj() == true) {
						if (abs(dynamic_cast<Donator*>(_donacije.GetElement1(i))->getDatum() - Datum(21, 12, 2018)) >= 90) {
							thread e(e_mail, "Molimo Vas da se javite u transfuzijski centar radi davanja krvi!");
							e.join();
						}

					}

				}
			}
		}
		return 0;
	}
	TransfuzijskiCentar() {

	}
	/*
	prilikom svake donacije je potrebno povecati zalihe za tu krvnu grupu
	ukoliko krvna grupa vec postoji na zalihi onda se uvecava samo njena kolicina
	takodjer, donatoru se postavlja nova vrijednost datuma posljednje donacije.
	ukoliko je neki od dodantora darivao krv prije isteka 3 mjeseca od posljednjeg
	darivanja, funkcija baca izuzetak
	*/
	//datum donacije, donator, kolicina
	void AddDonaciju(Datum datum, Osoba *osoba, double kolicina) {
	
		bool found_donator = false;
		for (int i = 0; i < _donacije.GetTrenutno(); i++) {
			if (_donacije.GetElement1(i) == osoba) {
				found_donator = true;
				if (abs(dynamic_cast<Donator*>(_donacije.GetElement1(i))->getDatum() - datum) < 90) {
					throw exception("Mora proci tri mjeseca od zadnje donacije!");
				}
				dynamic_cast<Donator*>(_donacije.GetElement1(i))->setDatum(datum);
			}
		}
		if (found_donator == false) {
			Donator* pok_d = dynamic_cast<Donator*>(osoba);
			_donacije.AddElement(pok_d, kolicina);
		}

		bool found_zalihe = false;
		for (int i = 0; i < _zalihe.GetTrenutno(); i++) {
			if (_zalihe.GetElement1(i) == osoba->getKrvnaGrupa()) {
				found_zalihe = true;
				_zalihe.SetElement2(i, _zalihe.GetElement2(i) + kolicina);
			}
		}
		if (found_zalihe == false) {
			_zalihe.AddElement(osoba->getKrvnaGrupa(), kolicina);
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
	Datum dat_12_03_2018(12, 3, 2018), dat_01_02_2018(1, 2, 2018);
	Datum danas(dat_12_03_2018);
	cout << danas << endl;

	Kolekcija<int, int, 10> kolekcija1;
	for (int i = 0; i < kolekcija1.GetMax() - 1; i++) {
		if (!kolekcija1.AddElement(10 + i, 170000 + i)) {
			cout << "Elementi " << 10 + i << " i " << 170000 + i << " nisu dodati u kolekciju" << endl;
		}
			
	}
		

	cout << kolekcija1 << endl;
	kolekcija1.SetSortiranje(DESC);
	kolekcija1.AddElement(16, 170016);
	cout << kolekcija1 << endl;

	Kolekcija<int, int, 10> kolekcija2(kolekcija1);
	cout << kolekcija2 << endl;


	vector<KrvnaGrupa*> krvneGrupe;
	krvneGrupe.push_back(new KrvnaGrupa(O, '+')); //0 - O_poz
	krvneGrupe.push_back(new KrvnaGrupa(O, '-')); //1 - O_neg
	krvneGrupe.push_back(new KrvnaGrupa(A, '+')); //2 - A_poz
	krvneGrupe.push_back(new KrvnaGrupa(A, '-')); //3 - A_neg
	krvneGrupe.push_back(new KrvnaGrupa(B, '+')); //4 - B_poz
	krvneGrupe.push_back(new KrvnaGrupa(B, '-')); //5 - B_neg
	krvneGrupe.push_back(new KrvnaGrupa(AB, '+'));//6 - AB_poz
	krvneGrupe.push_back(new KrvnaGrupa(AB, '-'));//7 - AB_neg

	vector<KrvnaGrupa*> donori_O_poz{ krvneGrupe[0], krvneGrupe[2], krvneGrupe[4], krvneGrupe[6] };
	krvneGrupe[0]->SetDonatori(donori_O_poz);
	//ili krace napisano
	krvneGrupe[0]->SetPrimaoci(vector<KrvnaGrupa*>{krvneGrupe[0], krvneGrupe[1]});
	krvneGrupe[2]->SetDonatori(vector<KrvnaGrupa*>{krvneGrupe[2], krvneGrupe[6]});
	krvneGrupe[2]->SetPrimaoci(vector<KrvnaGrupa*>{krvneGrupe[0], krvneGrupe[2], krvneGrupe[1], krvneGrupe[3]});

	krvneGrupe[4]->SetDonatori(vector<KrvnaGrupa *>{krvneGrupe[4], krvneGrupe[6]});
	krvneGrupe[4]->SetPrimaoci(vector<KrvnaGrupa *>{krvneGrupe[0], krvneGrupe[4], krvneGrupe[1], krvneGrupe[5]});

	Osoba * jasmin = new Donator("Jasmin Azemovic", krvneGrupe[4], "jasmin@fit.ba", Datum(12, 2, 2017), true);
	Osoba * adel = new Donator("Adel Handzic", krvneGrupe[3], "adelfit.ba", Datum(9, 1, 2017), true);
	Osoba * goran = new Donator("Goran Skondric", krvneGrupe[5], "goran@fit.net", Datum(9, 3, 2017), true);


	TransfuzijskiCentar tcMostar;
	/*
	prilikom svake donacije je potrebno povecati zalihe za tu krvnu grupu
	ukoliko krvna grupa vec postoji na zalihi onda se uvecava samo njena kolicina
	takodjer, donatoru se postavlja nova vrijednost datuma posljednje donacije.
	ukoliko je neki od dodantora darivao krv prije isteka 3 mjeseca od posljednjeg
	darivanja, funkcija baca izuzetak
	*/
	//datum donacije, donator, kolicina
	try {
		tcMostar.AddDonaciju(Datum(20, 5, 2017), jasmin, 2.5);
		tcMostar.AddDonaciju(Datum(20, 5, 2017), adel, 3);
		tcMostar.AddDonaciju(Datum(6, 5, 2017), goran, 1.2);
	}
	catch (exception & err) {
		cout << "Greska-> " << err.what() << endl;
	}
	try {
		tcMostar.AddDonaciju(Datum(10, 9, 2017), jasmin, 2);
		tcMostar.AddDonaciju(Datum(18, 10, 2017), adel, 1.8);
		tcMostar.AddDonaciju(Datum(15, 9, 2017), goran, 3.8);
	}
	catch (exception & err) {
		cout << "Greska-> " << err.what() << endl;
	}
	Zahtjev zahtjev_0_poz("Tranfuziologija KCUS", krvneGrupe[0], 15),
		zahtjev_0_neg("Tranfuziologija Bakir Nakas", krvneGrupe[1], 8);
	//ukoliko na stanju postoji dovoljno iste ili zamjense krvne grupe
	if (tcMostar.ProvjeriStanje("0+") >= 15) {
		tcMostar.DodajZahtjev(zahtjev_0_poz);
	}
	

	/*ukoliko transfuzijski centar posjeduje zahtijevane kolicine na stanju, odmah ce ih ustupiti prema zahtjevu,
	a u slucaju da ne posjeduje onda ce (koristeci multithreading) kontaktirati (poslati email poruku sa odgovarajucim sadrzajem)
	sve donore koji zadovoljavaju sljedece uslove:
	- mogu donirati krv zahtijevanoj krvnoj grupi
	- oznacili su da zele biti kontaktirani
	- nisu davali krv u posljednja 3 mjeseca
	*/
	if (tcMostar.ProvjeriStanje("0-") >= 8)
		tcMostar.DodajZahtjev(zahtjev_0_neg);


	/*ispisuje matricu ili tablicu iz koje je vidljiva kompatibilnost izmeðu pojedinih krvnih grupa. pored naziva i rh faktora krvne grupe, 
	u zagradi ispisati i trenutnu kolièinu na stanju. matrica prikazana na kraju zadatka*/
	tcMostar.IspisiMatricuKompatibilnosti(krvneGrupe);

	//delete adel;
	//delete jasmin;
	//delete goran;

	//for (size_t i = 0; i < krvneGrupe.size(); i++)
	//	delete krvneGrupe[i];

	system("PAUSE");
}
