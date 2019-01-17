#include<iostream>
#include<string>
#include<exception>
#include<thread>
#include<vector>
#include<regex>
#include<mutex>
using namespace std;
const char *crt = "\n---------------------------------------\n";

class DatumVrijeme {
	int *_dan, *_mjesec, *_godina, *_sati, *_minuti;
public:
	bool operator==(DatumVrijeme& obj) {
		if (*_dan == *obj._dan && *_mjesec == *obj._mjesec && *_godina == *obj._godina && *_sati == *obj._sati) {
			return true;
		}
		else return false;
	}
	DatumVrijeme(const DatumVrijeme & obj) {
		_dan = new int(*obj._dan);
		_mjesec = new int(*obj._mjesec);
		_godina = new int(*obj._godina);
		_sati = new int(*obj._sati);
		_minuti = new int(*obj._minuti);
	}
	DatumVrijeme & operator=(DatumVrijeme& obj) {
		_dan = new int(*obj._dan);
		_mjesec = new int(*obj._mjesec);
		_godina = new int(*obj._godina);
		_sati = new int(*obj._sati);
		_minuti = new int(*obj._minuti);
		return *this;
	}
	DatumVrijeme(int dan = 1, int mjesec = 1, int godina = 2000, int sati = 0, int minuti = 0) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
		_sati = new int(sati);
		_minuti = new int(minuti);

	}
	~DatumVrijeme() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
		delete _sati; _sati = nullptr;
		delete _minuti; _minuti = nullptr;

	}
	friend ostream& operator<< (ostream &COUT, DatumVrijeme &obj) {
		COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina << " " << *obj._sati << ":" << *obj._minuti;
		return COUT;
	}
};

template<class T1, class T2 = int>
class FITKolekcija {
	T1 * _elementi1;
	T2 * _elementi2;
	int _trenutno;
public:
	void RemoveElement(int lokacija) {
		for (int i = lokacija; i < _trenutno-1; i++) {
			_elementi1[i] = _elementi1[i + 1];
			_elementi2[i] = _elementi2[i + 1];
		}
		_trenutno--;
		T1* t1 = new T1[_trenutno];
		T2* t2 = new T2[_trenutno];
		for (int i = 0; i < _trenutno; i++) {
			t1[i] = _elementi1[i];
			t2[i] = _elementi2[i];
		}
		delete[] _elementi1;
		delete[] _elementi2;
		_elementi1 = t1;
		_elementi2 = t2;
		t1 = nullptr;
		t2 = nullptr;
	}

	/*METODA GetJedinstveni VRAÆA LISTU JEDINSTVENIH ELEMENATA TJ. ELEMENATA KOJI NE SADRŽE DUPLIKATE (POJAM DUPLIKAT SE ODNOSI NA ISTE VRIJEDNOSTI ELEMENATA T1 I T2). ELEMENTI KOJI SE DUPLIRAJU SE U OVOJ LISTI TREBAJU POJAVITI SAMO JEDNOM.*/
	FITKolekcija * GetJedinstveni() {
		FITKolekcija *jedinstveni = new FITKolekcija<T1, T2>;
		bool duplikat = false;
		
		for (int i = 0; i < _trenutno; i++) {

			for (int j = 0; j < jedinstveni->_trenutno; j++) {
				if (jedinstveni->_elementi1[j] == _elementi1[i] && jedinstveni->_elementi2[j] == _elementi2[i]) {
					duplikat = true;
				}
			}

			if (!duplikat) {
				jedinstveni->Dodaj(_elementi1[i], _elementi2[i]);
				
			}
			duplikat = false;
		}
		return jedinstveni;

	}
	void DodajNaLokaciju(T1& e1, T2& e2, int lokacija) {
		
		if (lokacija<0 || lokacija>_trenutno) {
			throw exception("Nedozvoljena lokacija");
		}
		
		lokacija--;
		_elementi1[lokacija] = e1;
		_elementi2[lokacija] = e2;
	}
	void Sortiraj(){
	
		for (int i = 1; i < _trenutno; i++) {
			int j = i;
			while (j > 0 && _elementi1[j] < _elementi1[j-1]) {
				T1 temp1 = _elementi1[j];
				T2 temp2 = _elementi2[j];
				_elementi1[j] = _elementi1[j - 1];
				_elementi2[j] = _elementi2[j - 1];
				_elementi1[j - 1] = temp1;
				_elementi2[j - 1] = temp2;

				j--;
			}
		}
	
	}

	void Dodaj(T1& e1, T2& e2) {
		T1* temp_01 = new T1[_trenutno + 1];
		T2* temp_02 = new T2[_trenutno + 1];
		for (int i = 0; i < _trenutno; i++) {
			temp_01[i] = _elementi1[i];
			temp_02[i] = _elementi2[i];
		}
		temp_01[_trenutno] = e1;
		temp_02[_trenutno] = e2;
		_trenutno++;

		delete[] _elementi1;
		delete[] _elementi2;
		_elementi1 = temp_01; temp_01 = nullptr;
		_elementi2 = temp_02; temp_02 = nullptr;

	}
	FITKolekcija(const FITKolekcija& obj) {
		_trenutno = obj._trenutno;
		_elementi1 = new T1[_trenutno];
		_elementi2 = new T2[_trenutno];
		for (int i = 0; i < _trenutno; i++) {
			_elementi1[i] = obj._elementi1[i];
			_elementi2[i] = obj._elementi2[i];
		}
	}
	FITKolekcija& operator=(const FITKolekcija& obj) {
		_trenutno = obj._trenutno;
		_elementi1 = new T1[_trenutno];
		_elementi2 = new T2[_trenutno];
		for (int i = 0; i < _trenutno; i++) {
			_elementi1[i] = obj._elementi1[i];
			_elementi2[i] = obj._elementi2[i];
		}
		return *this;
	}
	FITKolekcija() {
		_elementi1 = nullptr;
		_elementi2 = nullptr;
		_trenutno = 0;
	}
	~FITKolekcija() {
		delete[] _elementi1; _elementi1 = nullptr;
		delete[] _elementi2; _elementi2 = nullptr;
	}
	T1 * GetT1() { return _elementi1; }
	T2 * GetT2() { return _elementi2; }
	int GetTrenutno() { return _trenutno; }

	friend ostream& operator<< (ostream &COUT, FITKolekcija &obj) {
		for (size_t i = 0; i < obj._trenutno; i++)
			COUT << obj._elementi1[i] << " " << obj._elementi2[i] << endl;
		return COUT;
	}
};

enum vrstaDokumenta { PDF, DOC, TXT, HTML };
ostream& operator<<(ostream& out, vrstaDokumenta& obj) {
	if (obj == PDF) {
		out << "PDF";
	}
	if (obj == DOC) {
		out << "DOC";
	}
	if (obj == TXT) {
		out << "TXT";
	}
	if (obj == HTML) {
		out << "HTML";
	}
	return out;
}

class Dokument {
	vrstaDokumenta _vrsta;
	string _naziv;
	string _sadrzaj;
	int _brojStranica;
public:
	//BROJ STRANICA DOKUMENTA SE AUTOMATSKI ODREÐUJE PRILIKOM DODAVANJA SADRŽAJA. ZA POTREBE ISPITA PRETPOSTAVLJAMO DA NA 
	//JEDNU STRANICU MOŽE STATI 30 ZNAKOVA UKLJUÈUJUÆI I RAZMAKE

	void DodajSadrzaj(string sadrzaj) {
		_sadrzaj += " " + sadrzaj;

		for (int i = 0; i < _sadrzaj.size(); i++) {
			if (i % 30 == 0) {
				_brojStranica++;
			}
		}

	}
	Dokument(const Dokument& obj) : _vrsta(obj._vrsta){
		_naziv = obj._naziv;
		_sadrzaj = obj._sadrzaj;
		_brojStranica = obj._brojStranica;
	}
	Dokument& operator=(Dokument& obj) {
		_vrsta = obj._vrsta;
		_naziv = obj._naziv;
		_sadrzaj = obj._sadrzaj;
		_brojStranica = obj._brojStranica;
		return *this;
	}
	Dokument() { _brojStranica = 0; }
	Dokument(vrstaDokumenta vrsta, string naziv) {
		_naziv = naziv; _vrsta = vrsta; _brojStranica = 0;
	}
	string GetNaziv() { return _naziv; };
	string GetSadrzaj() { return _sadrzaj; };
	vrstaDokumenta GetVrsta() { return _vrsta; };
	void UvecajBrojStranica() { _brojStranica++; }
	int GetBrojStranica() { return _brojStranica; }
	friend ostream & operator<<(ostream&COUT, Dokument & obj) {
		COUT << obj._naziv << "." << obj._vrsta << " (" << obj._brojStranica << ")\n" << obj._sadrzaj << endl;
		return COUT;
	}
};
class Uredjaj {
protected:
	char * _proizvodjac;
	char * _model;
public:
	Uredjaj(const Uredjaj& obj) {
		int vel = strlen(obj._proizvodjac) + 1;
		_proizvodjac = new char[vel];
		strcpy_s(_proizvodjac, vel, obj._proizvodjac);
		vel = strlen(obj._model) + 1;
		_model = new char[vel];
		strcpy_s(_model, vel, obj._model);
	}
	Uredjaj& operator=(Uredjaj& obj) {
		int vel = strlen(obj._proizvodjac) + 1;
		_proizvodjac = new char[vel];
		strcpy_s(_proizvodjac, vel, obj._proizvodjac);
		vel = strlen(obj._model) + 1;
		_model = new char[vel];
		strcpy_s(_model, vel, obj._model);
		return *this;
	}
	Uredjaj(const char * proizvodjac="", const char * model="") {
		int vel = strlen(proizvodjac) + 1;
		_proizvodjac = new char[vel];
		strcpy_s(_proizvodjac, vel, proizvodjac);
		vel = strlen(model) + 1;
		_model = new char[vel];
		strcpy_s(_model, vel, model);
	}
	~Uredjaj() {
		delete[] _proizvodjac; _proizvodjac = nullptr;
		delete[] _model; _model = nullptr;
	}
	friend ostream & operator<<(ostream&COUT, Uredjaj & obj) {
		COUT << obj._proizvodjac << "." << obj._model << endl;
		return COUT;
	}
};
mutex m;
void ispisi(string dok_sadrzaj) {
	m.lock();
	cout << dok_sadrzaj << endl;
	m.unlock();
}
class Printer : public Uredjaj{
	FITKolekcija<DatumVrijeme, Dokument> _printaniDokumenti;
	vector<string> _zabranjeneRijeci;
public:
	void UkloniDokumente() {

		int lok = -1;
		
		for (int i = 0; i < _printaniDokumenti.GetTrenutno(); i++) {
			for (int j = 0; j < _zabranjeneRijeci.size(); j++) {
				if (regex_search(_printaniDokumenti.GetT2()[i].GetSadrzaj(), regex(_zabranjeneRijeci[j]))) {
					lok = i;
				}
			}
			if (lok != -1) {
				_printaniDokumenti.RemoveElement(lok);
			}
		}
	

	}
	string GetTopZabranjenuRijec() {
		
		int max = 0;
		int top = 0;
		string temp;
	
		for (int i = 0; i < _printaniDokumenti.GetTrenutno(); i++) {
			for (int j = 0; j <_zabranjeneRijeci.size(); j++) {

				if (regex_search(_printaniDokumenti.GetT2()[i].GetSadrzaj(), regex(_zabranjeneRijeci[j]),regex_constants::match_any)) {
					top++;
				}
				if (top > max) {
					max = top;
					temp = _zabranjeneRijeci[j];
				}
				top = 0;
			}
		
		}
		cout << max << endl;
		return temp;
		

	}
	int GetProsjecanBrojStranicaPoDatumu(DatumVrijeme dat_vri) {
		int br_str = 0;
		int br_dok = 0;
		for (int i = 0; i < _printaniDokumenti.GetTrenutno(); i++) {
			if (_printaniDokumenti.GetT1()[i] == dat_vri) {
				br_str += _printaniDokumenti.GetT2()[i].GetBrojStranica();
				br_dok++;
			}
		}
		return br_str / br_dok;
	}
		/*
		DA BI PRINTER ISPRINTAO NEKI DOKUMENT MORAJU BITI ZADOVOLJENA SLJEDEÆA PRAVILA:
		1. NAZIV DOKUMENTA MORA BITI U SLJEDEÆEM FORMATU npr: ispitPRIII.doc
		NAZIV DOKUMENTA MOŽE SADRŽAVATI SAMO SLOVA (NAJMANJE 5, NAJVIŠE 15), A EKSTENZIJA MOŽE
		BITI SAMO NEKA OD DOZVOLJENIH VRSTOMDOKUMENTA
		2. SADRŽAJ DOKUMENTA U SEBI NE SMIJE POSJEDOVATI NITI JEDNU ZABRANJENU RIJEÈ
		UKOLIKO NEKI OD NAVEDENIH USLOVA NIJE ZADOVOLJEN FUNKCIJA TREBA DA BACI EXCEPTION SA
		ODGOVARAJUÆOM PORUKOM, ALI ÆE DOKUMENT BITI POHRANJEN U KOLEKCIJU _printaniDokumenti.
		UKOLIKO DOKUMENT ISPUNJAVA SVE USLOVE ZA PRINTANJE POTREBNO JE NA KONZOLU ISPISATI
		SADRŽAJ DOKUMENTA KOJI SE PRINTA

		*/
	void Printaj(DatumVrijeme dat_vri, Dokument dok) {
		
		string pravilo = ("([A-Za-z]{5,15})(\\.)(doc|txt|html|pdf)");
		if (!regex_match(dok.GetNaziv(), regex(pravilo))) {
			throw exception("Nedozvoljen naziv ili ekstenzija!");
		}
		bool zabranjenaRijec = false;
		string pravilo2 = "(";
		for (int i = 0; i < _zabranjeneRijeci.size(); i++) {
			pravilo2 += _zabranjeneRijeci[i];
			if (i != _zabranjeneRijeci.size() - 1) {
				pravilo2 += "|";
			}
		}
		pravilo2 += ")";
		if (regex_search(dok.GetSadrzaj(), regex(pravilo2))) {
			zabranjenaRijec = true;
		}

		if (zabranjenaRijec == true) {
			_printaniDokumenti.Dodaj(dat_vri, dok);
			throw exception("Postoji zabranjena rijec!");
		}
		else {
			_printaniDokumenti.Dodaj(dat_vri, dok);
			this_thread::sleep_for(chrono::seconds(3));
			thread t(ispisi, dok.GetSadrzaj());
			t.join();
		}

	}
	void DodajZabranjenuRijec(string rijec) {
		vector<string>::iterator it_r = _zabranjeneRijeci.begin();
		bool dupla = false;
		while (it_r != _zabranjeneRijeci.end()) {
			if (*it_r == rijec) {
				dupla = true;
			}
			it_r++;
		}
		if (!dupla) {
			_zabranjeneRijeci.push_back(rijec);
		}
		
	}
	Printer(const char* proizvodjac="", const char* model="") : Uredjaj(proizvodjac,model){

	}
	Printer(const Printer& obj) :Uredjaj(obj) {
		_printaniDokumenti = obj._printaniDokumenti;
		_zabranjeneRijeci = obj._zabranjeneRijeci;
	}
	Printer& operator=(Printer&obj) {
		int vel = strlen(obj._proizvodjac) + 1;
		_proizvodjac = new char[vel];
		strcpy_s(_proizvodjac, vel, obj._proizvodjac);
		vel = strlen(obj._model) + 1;
		_model = new char[vel];
		strcpy_s(_model, vel, obj._model);
		_printaniDokumenti = obj._printaniDokumenti;
		_zabranjeneRijeci = obj._zabranjeneRijeci;
		return *this;
	}
	FITKolekcija<DatumVrijeme, Dokument> & GetPrintaniDokumenti() { return _printaniDokumenti; };
	vector<string> & GetZabranjeneRijeci() { return _zabranjeneRijeci; };
};

void main()
{
	/****************************************************************************
	1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
	2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ÆE BITI OZNACENO KAO "RE"
	3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEÈILO GUBLJENJE URAÐENOG ZADATKA
	4. PROGRAMSKI CODE SE TAKOÐER NALAZI U FAJLU CODE.TXT
	5. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIÈNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUÈAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOÆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
	6. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAÈENO.
	****************************************************************************/

#pragma region TestiranjeDatumVrijeme
	DatumVrijeme danas(1, 2, 2017, 10, 15);
	DatumVrijeme sutra(danas);
	DatumVrijeme prekosutra;
	prekosutra = danas;
	cout << danas << endl << sutra << endl << prekosutra << crt;
#pragma endregion
#pragma region FITKolekcija
	int v6 = 6, v13 = 13, v32 = 32, v63 = 63, v98 = 98, v109 = 109, v196 = 196;

	FITKolekcija<int, int> brojevi;
	brojevi.Dodaj(v196, v6);
	brojevi.Dodaj(v13, v32);
	brojevi.Dodaj(v98, v196);
	brojevi.Dodaj(v63, v13);
	brojevi.Dodaj(v98, v196);
	brojevi.Dodaj(v196, v6);

	cout << brojevi << crt;
	//SORTIRANJE ÈLANOVA KOLEKCIJE SE VRŠI U RASTUÆEM REDOSLIJEDU NA OSNOVU VRIJEDNOSTI ELEMENTA TIPA T1
	brojevi.Sortiraj();
	cout << brojevi << crt;
	/*BROJ 2 SE ODNOSI NA LOKACIJU/INDEKS UNUTAR LISTE NA KOJI JE POTREBNO DODATI NOVE ELEMENTE*/
	brojevi.DodajNaLokaciju(v109, v6, 2);
	cout << brojevi << crt;
	brojevi.Sortiraj();
	cout << brojevi << crt;
	/*METODA GetJedinstveni VRAÆA LISTU JEDINSTVENIH ELEMENATA TJ. ELEMENATA KOJI NE SADRŽE DUPLIKATE (POJAM DUPLIKAT SE ODNOSI NA ISTE VRIJEDNOSTI ELEMENATA T1 I T2). ELEMENTI KOJI SE DUPLIRAJU SE U OVOJ LISTI TREBAJU POJAVITI SAMO JEDNOM.*/
	FITKolekcija<int, int> * jedinstveni = brojevi.GetJedinstveni();
	cout << *jedinstveni << crt;
	*jedinstveni = brojevi;
	cout << *jedinstveni << crt;

	Dokument ispitPRIII(DOC, "ispitPRIII.doc");
	//BROJ STRANICA DOKUMENTA SE AUTOMATSKI ODREÐUJE PRILIKOM DODAVANJA SADRŽAJA. ZA POTREBE ISPITA PRETPOSTAVLJAMO DA NA JEDNU STRANICU MOŽE STATI 30 ZNAKOVA UKLJUÈUJUÆI I RAZMAKE
	ispitPRIII.DodajSadrzaj("NAMJERNO IZOSTAVLJANJE KOMPLETNIH RE I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ÆE BITI OZNACENO KAO RE");
	cout << "Broj stranica -> " << ispitPRIII.GetBrojStranica() << endl;
	Dokument ispitBaze(DOC, "ispitBaze.doc");
	ispitBaze.DodajSadrzaj("PROGRAMSKI CODE SE TAKOÐER NALAZI U FAJLU CODE.TXT");
	cout << ispitBaze << endl;//ISPISUJE SVE DOSTUPNE PODATKE O DOKUMENTU

	Printer hp3200("HP", "3200");
	//PRINTER NEÆE DOZVOLITI PRINTANJE DOKUMENATA U ÈIJEM SADRŽAJU SE NALAZI NEKA OD ZABRANJENIH RIJEÈI
	hp3200.DodajZabranjenuRijec("RE");// :)
	hp3200.DodajZabranjenuRijec("RAT");
	hp3200.DodajZabranjenuRijec("UBITI");
	hp3200.DodajZabranjenuRijec("RE");// ONEMOGUÆITI PONAVLJANJE ZABRANJENIH RIJEÈI, KORISTITI ITERATORE
	hp3200.DodajZabranjenuRijec("MRZITI");
	try
	{
		/*
		DA BI PRINTER ISPRINTAO NEKI DOKUMENT MORAJU BITI ZADOVOLJENA SLJEDEÆA PRAVILA:
		1. NAZIV DOKUMENTA MORA BITI U SLJEDEÆEM FORMATU npr: ispitPRIII.doc
		NAZIV DOKUMENTA MOŽE SADRŽAVATI SAMO SLOVA (NAJMANJE 5, NAJVIŠE 15), A EKSTENZIJA MOŽE
		BITI SAMO NEKA OD DOZVOLJENIH VRSTOMDOKUMENTA
		2. SADRŽAJ DOKUMENTA U SEBI NE SMIJE POSJEDOVATI NITI JEDNU ZABRANJENU RIJEÈ
		UKOLIKO NEKI OD NAVEDENIH USLOVA NIJE ZADOVOLJEN FUNKCIJA TREBA DA BACI EXCEPTION SA
		ODGOVARAJUÆOM PORUKOM, ALI ÆE DOKUMENT BITI POHRANJEN U KOLEKCIJU _printaniDokumenti.
		UKOLIKO DOKUMENT ISPUNJAVA SVE USLOVE ZA PRINTANJE POTREBNO JE NA KONZOLU ISPISATI
		SADRŽAJ DOKUMENTA KOJI SE PRINTA

		*/
		hp3200.Printaj(danas, ispitBaze);
		hp3200.Printaj(danas, ispitPRIII);

	

		/*
		IMPLEMENTIRATI PRINTANJE U ZASEBNIM NITIMA UZ KORIŠTENJE sleep_for FUNKCIJE
		VODITI RAÈUNA O DIJELJENJU ZAJEDNIÈKIH RESURSA
		*/

	}
	catch (exception& err)
	{
		cout << err.what() << endl;
	}

	//KOD POREÐENJA DATUMA PRINTANJA NIJE POTREBNO POREDITI MINUTE, SAMO DATUM I SATE
	cout << "Prosjecan broj printanih stranica za " << danas << " je -> " << hp3200.GetProsjecanBrojStranicaPoDatumu(danas) << crt;
	cout << "Najcesce koristena zabranjena rijec je -> " << hp3200.GetTopZabranjenuRijec() << crt;

	Printer hp4000(hp3200);
	cout << "Prosjecan broj printanih stranica za " << danas << " je -> " << hp4000.GetProsjecanBrojStranicaPoDatumu(danas) << crt;
	cout << "Najcesce koristena zabranjena rijec je -> " << hp4000.GetTopZabranjenuRijec() << crt;
	hp4000.UkloniDokumente();//UKLANJA SVE DOKUMENTE KOJI NISU ZADOVOLJILI USLOVE ZA PRINTANJE
	cout << hp4000 << crt;

#pragma endregion
	system("PAUSE");
}