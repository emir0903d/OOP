#include <iostream>
#include<string>
using namespace std;


/****************************************************************************
1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ÆE BITI OZNACENO KAO "RE"
3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEÈILO GUBLJENJE URAÐENOG ZADATKA
4. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIÈNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUÈAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOÆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
5. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAÐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
6. ZA POTREBE TESTIRANJA, UNUTAR MAIN FUNKCIJE MOZETE DODAVATI NOVE TESTNE PODATKE
****************************************************************************/
//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

const char * crt = "\n----------------------------------------------------\n";
class Datum {
	int * _dan, *_mjesec, *_godina;
public:
	Datum & operator=(const Datum& obj) {
		_dan = new int(*obj._dan);
		_mjesec = new int(*obj._mjesec);
		_godina = new int(*obj._godina);
		return *this;
	}
	Datum(const Datum& obj) {
		_dan = new int(*obj._dan);
		_mjesec = new int(*obj._mjesec);
		_godina = new int(*obj._godina);
	}
	Datum(int d=1, int m=1, int g=2000) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	~Datum() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}
	friend ostream & operator<<(ostream & COUT, const Datum& obj) {
		COUT << *obj._dan << "/" << *obj._mjesec << "/" << *obj._godina;
		return COUT;
	}
};

template<class T, int velicina>
class FITArray {
	T _niz[velicina];
	int _trenutno;
	bool _dozvoliDupliciranje;
public:
	//OSTALE FUNKCIJE CLANICE DEFINISATI VAN TEMPLATE KLASE
	FITArray(bool dozvoliDupliciranje = true) { 
		_dozvoliDupliciranje = dozvoliDupliciranje; 
		_trenutno = 0;
	/*	for (int i = 0; i < velicina; i++)
			_niz[i] = 0;*/
	}
	int GetTrenutno() { return _trenutno; }
	T * GetNiz() { return _niz; }
	friend ostream& operator<<<>(ostream&, const FITArray&);
	bool operator+=(T);
	bool operator-=(T);
	FITArray(const FITArray&);
	FITArray<T,velicina> operator()(int, int);
	FITArray<T, velicina> operator=(const FITArray<T, velicina>&);
	void Dodaj(T el) {
		_niz[_trenutno] = el;
		_trenutno++;
	}
};
template<class T, int velicina>
FITArray<T, velicina> FITArray<T, velicina>::operator=(const FITArray<T, velicina>&b)
{
	_trenutno = b._trenutno;
	for (int i = 0; i < _trenutno; i++) {
		_niz[i] = b._niz[i];
	}
	_dozvoliDupliciranje = b._dozvoliDupliciranje;
	return *this;
}

template<class T, int velicina>
FITArray<T,velicina> FITArray<T, velicina>::operator()(int Od, int Do)
{
	Od--;
	Do--;
	FITArray<T, velicina> temp;
	if (Do>=_trenutno) {
		for (int i = Od; i < _trenutno; i++) {
			temp += _niz[i];
		}
	}
	else {
		for (int i = Od; i <= Do; i++) {
			temp += _niz[i];
		}
	}
		
	
	return temp;
}



template<class T, int velicina>
FITArray<T, velicina>::FITArray(const FITArray &b)
{
	_trenutno = b._trenutno;
	_dozvoliDupliciranje = b._dozvoliDupliciranje;
	for (int i = 0; i < _trenutno; i++) {
	
		_niz[i] = b._niz[i];
	
	}
}

template<class T, int velicina>
bool FITArray<T,velicina>::operator+=(T b) {
	if (_dozvoliDupliciranje == false) {
		for (int i = 0; i < _trenutno; i++) {
			if (b == _niz[i]) {
				return false;
			}
		}
	}
	_niz[_trenutno] = 0;//
	_niz[_trenutno] += b;
	_trenutno++;
	return true;
}
template<class T, int velicina>
bool FITArray<T, velicina>::operator-=(T b) {
	for (int i = 0; i < _trenutno; i++) {
		if (b == _niz[i]) {
			
			for (int j = i; j < _trenutno - 1; j++) {
				_niz[j] = _niz[j + 1];
			}
			_trenutno--;
			return true;

		}
		
	}
	return false;
}






template<class T, int velicina>
ostream& operator<<<>(ostream& out, const FITArray<T,velicina>& obj) {
	for (int i = 0; i < obj._trenutno; i++)
		out << obj._niz[i] << " ";
	return out;
}

class PolozeniPredmet {
	char * _naziv;
	int _ocjena;
	Datum _datumPolaganja;
public:
	int getOcjena() { return _ocjena; }
	PolozeniPredmet(const PolozeniPredmet& obj): _datumPolaganja(obj._datumPolaganja) {
		_naziv = new char[strlen(obj._naziv) + 1];
		strcpy_s(_naziv, strlen(obj._naziv) + 1, obj._naziv);
		_ocjena = obj._ocjena;
		
	}
	PolozeniPredmet& operator=(const PolozeniPredmet& obj)
	{
		_naziv = new char[strlen(obj._naziv)+1];
		strcpy_s(_naziv, strlen(obj._naziv) + 1,obj._naziv);
		_ocjena = obj._ocjena;
		_datumPolaganja = obj._datumPolaganja;
		return *this;
	}
	PolozeniPredmet(Datum d, const char* naziv, int ocjena): _datumPolaganja(d) {
		int size = strlen(naziv) + 1;
		_naziv = new char[size];
		strcpy_s(_naziv, size, naziv);
		_ocjena = ocjena;
		
	}
	PolozeniPredmet() : _datumPolaganja(){
		_naziv = nullptr;
		_ocjena = 0;

	}
	~PolozeniPredmet() { 
		if(_naziv!=nullptr)
		delete[] _naziv; 
		_naziv = nullptr; 
	}
	friend ostream & operator<<(ostream & COUT, const PolozeniPredmet & obj) {
		COUT << obj._naziv << "(" << obj._ocjena << ")" << obj._datumPolaganja << endl;
		return COUT;
	}
};

class Student {
	static int _indeksCounter; //sluzi za odredjivanje narednog broja indeksa koji ce biti dodijeljen studentu, pocevsi od 160000
	const int _indeks;
	char * _imePrezime;
	FITArray<PolozeniPredmet, 40> _polozeniPredmeti;
public:
	Student & operator=(Student& obj) {
		
		_imePrezime = new char[strlen(obj._imePrezime) + 1];
		strcpy_s(_imePrezime, strlen(obj._imePrezime) + 1, obj._imePrezime);
		_polozeniPredmeti = obj._polozeniPredmeti;
		return *this;
	}
	float GetProsjek() {
		float suma_ocjena = 0.0;
		int br_ocjena = 0;
		for (int i = 0; i < _polozeniPredmeti.GetTrenutno(); i++) {
			suma_ocjena += _polozeniPredmeti.GetNiz()[i].getOcjena();
			br_ocjena++;
		}
		return suma_ocjena / br_ocjena;
	}
	void DodajPredmet(PolozeniPredmet& obj) {
		_polozeniPredmeti.Dodaj(obj);
	}
	Student(string imePrezime=""):_polozeniPredmeti(), _indeks(++_indeksCounter) {
		

		_imePrezime = new char[imePrezime.size() + 1];
		strcpy_s(_imePrezime, imePrezime.size() + 1, imePrezime.c_str());

	}
	~Student() {
		delete[] _imePrezime; _imePrezime = nullptr;
	}
	friend ostream & operator<<(ostream & COUT, Student & obj) {
		COUT << obj._indeks << " " << obj._imePrezime << endl << obj._polozeniPredmeti;
		return COUT;
	}
};
int Student::_indeksCounter = 160000;

void main() {
	const int max = 20;

	Datum jucer(20, 11, 2017), danas(21, 11, 2017), sutra(22, 11, 2017);
	cout << jucer<<" " << danas << " " << sutra << endl;
	jucer = danas;
	cout << jucer << " " << danas << " " << sutra << endl;

	//parametar tipa bool odredjuje da li je u nizu dozvoljeno dupliciranje elemenata
	FITArray<int, max> nizIntegera(false);

	for (int i = 0; i < max - 1; i++)
		nizIntegera += i;//dodaje novog clana niza
	cout << nizIntegera;

	if (!(nizIntegera += 6))//pokusavamo dodati dupli clan niza
		cout << "Element nije dodan u niz" << endl;

	cout << crt << nizIntegera << crt; //ispisuje sve clanove niza

	nizIntegera -= 9; //uklanja clan niza sa vrijednoscu 9
	nizIntegera -= 17;

	cout << nizIntegera;
	FITArray<int, max> noviNizIntegera(nizIntegera);

	cout << crt << noviNizIntegera << crt;

	/*parametri odredjuju lokacije (indekse u nizu) elemenata OD - DO koje je potrebno vratiti. u slucaju da u nizu ne postoji trazeni broj elemenata funkcija treba da vrati sve element od lokacije OD pa do posljednje dostupnog elementa */
	cout << "Clanovi niza od lokacije 2 do lokacije 5 su -> " << noviNizIntegera(2, 5) << crt;

	Student adel("Adel Handzic"), jasmin("Jasmin Azemovic");

	PolozeniPredmet prII(jucer, "PRII", 8);
	PolozeniPredmet prIII(sutra, "PRIII", 7);

	adel.DodajPredmet(prII);
	adel.DodajPredmet(prIII);
	cout << adel << endl;
	//vraca prosjecnu ocjenu studenta
	cout << "Prosjecna ocjena -> " << adel.GetProsjek() << crt;

	jasmin = adel;
	cout << jasmin << endl;
	cout << "Prosjecna ocjena -> " << jasmin.GetProsjek() << crt;
	system("PAUSE");
}

