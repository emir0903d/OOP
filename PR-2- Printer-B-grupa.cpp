/*
1. BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA, U SUPROTNOM SE RAD NECE BODOVATI
2. STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
3. KREIRAJTE .DOC FAJL SA VASIM BROJEM INDEKSA ( NPR. IB130030.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJESENJA VASIH ZADATAKA. NE PREDAVATI .CPP FAJLOVE
4. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORISTENJE HELP-A
5. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI CETE KOPIRATI VASA RJESENJA)
6. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URADJENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
*/
#include <iostream>
#include <memory>
#include <fstream>
#include <tuple>


using namespace std;

//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

const char *crt = "\n-------------------------------------------\n";

enum vrstaDokumenta { PDF, DOC, TXT, HTML };
const char * vrstaDokumentaChar[] = { "PDF", "DOC", "TXT", "HTML" };

enum vrstaAktivnosti { KREIRANJE, DODAVANJE, ZAMJENA, PRINTANJE };
const char * vrstaAktivnostiChar[] = { "KREIRANJE", "DODAVANJE", "ZAMJENA", "PRINTANJE" };

const int BROJ_ZNAKOVA_PO_STRANICI = 30;

struct DatumVrijeme {
	int *_dan, *_mjesec, *_godina, *_sati, *_minuti;
	void Unos(int dan = 1, int mjesec = 1, int godina = 2000, int sati = 1, int minuti = 1) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
		_sati = new int(sati);
		_minuti = new int(minuti);
	}
	void Dealociraj() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
		delete _sati; _sati = nullptr;
		delete _minuti; _minuti = nullptr;
	}
	void Ispis() {
		cout << *_dan << "." << *_mjesec << "." << *_godina << " " << *_sati << ":" << *_minuti << endl;
	}
	bool LaterThan(DatumVrijeme Od) {
		if (*_dan * 365 + *_mjesec * 30 + *_godina * 365 + *_sati + *_minuti >= *Od._dan * 365 + *Od._mjesec * 30 + *Od._godina * 365 + *Od._sati + *Od._minuti) {
			return true;
		}
		else return false;
	}
	bool EarlierThan(DatumVrijeme Od) {
		if (*_dan * 365 + *_mjesec * 30 + *_godina * 365 + *_sati + *_minuti <= *Od._dan * 365 + *Od._mjesec * 30 + *Od._godina * 365 + *Od._sati + *Od._minuti) {
			return true;
		}
		else return false;
	}
	bool Equal(DatumVrijeme Od) {
		if (*_dan == *Od._dan && *_mjesec == *Od._mjesec && *_godina == *Od._godina && *_sati == *Od._sati && *_minuti == *Od._minuti) {
			return true;
		}
		else return false;
		


	}
};

char * AlocirajNizKaraktera(const char * sadrzaj) {
	int vel = strlen(sadrzaj) + 1;
	char * temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}

struct Modifikacija {
	char * _korisnicnkoIme;
	vrstaAktivnosti _aktivnost;
	DatumVrijeme _vrijemeModifikacije;
	void Unos(const char * sadrzaj, vrstaAktivnosti aktivnost, DatumVrijeme vm) {//ovdje se treba proslijediti korisnicko ime
		_korisnicnkoIme = AlocirajNizKaraktera(sadrzaj);
		_aktivnost = aktivnost;
		_vrijemeModifikacije.Unos(*vm._dan, *vm._mjesec, *vm._godina, *vm._dan, *vm._minuti);
	}
	void Dealociraj() {
		delete[] _korisnicnkoIme; _korisnicnkoIme = nullptr;
		_vrijemeModifikacije.Dealociraj();
	}
	void Ispis() { cout << _korisnicnkoIme << " (" << vrstaAktivnostiChar[_aktivnost] << ")"; _vrijemeModifikacije.Ispis(); }
};

struct Dokument {
	unique_ptr<vrstaDokumenta> _vrsta;
	char * _naziv;
	char * _sadrzaj;
	Modifikacija * _modifikacije;
	int _brojModifikacija;
	void AddModifikacija(const char* ime, DatumVrijeme dat, vrstaAktivnosti akt = ZAMJENA) {
		Modifikacija *temp = new Modifikacija[_brojModifikacija + 1];
		for (int i = 0; i < _brojModifikacija; i++) {
			temp[i].Unos(_modifikacije[i]._korisnicnkoIme, _modifikacije[i]._aktivnost, _modifikacije[i]._vrijemeModifikacije);
			_modifikacije[i].Dealociraj();
		}
		delete[]_modifikacije; _modifikacije = nullptr;
		temp[_brojModifikacija].Unos(ime, akt, dat);
		_modifikacije = temp; temp = nullptr;
		_brojModifikacija++;
	}
	void Ispis2() {

		int broj_znakova = 0;
		for (int i = 0; i < strlen(_sadrzaj); i++) {

			if (broj_znakova == BROJ_ZNAKOVA_PO_STRANICI) {
				cout << endl << crt << endl;
				broj_znakova = 0;
			}
			cout << _sadrzaj[i];
			broj_znakova++;
		}
	}
	/*DODAJE SADRZAJ U FAJL, ZADRZAVAJUCI POSTOJECI. SVAKO DODAVANJE EVIDENTIRATI KAO MODIFIKACIJU.
	PARAMETRI:SADRZAJ, KORISNICKO IME, VRIJEME MODIFIKACIJE*/
	void Unos2(vrstaDokumenta vrst, const char* naziv, const char * sadrzaj, Modifikacija *mod, int br_mod) {
		_vrsta = make_unique<vrstaDokumenta>(vrst);
		_naziv = new char[strlen(naziv) + 1];
		strcpy_s(_naziv, strlen(naziv) + 1, naziv);
		_sadrzaj = new char[strlen(sadrzaj) + 1];
		strcpy_s(_sadrzaj, strlen(sadrzaj) + 1, sadrzaj);
		_brojModifikacija = br_mod;
		_modifikacije = new Modifikacija[_brojModifikacija];
		for (int i = 0; i < _brojModifikacija; i++) {
			_modifikacije[i].Unos(mod[i]._korisnicnkoIme, mod[i]._aktivnost, mod[i]._vrijemeModifikacije);
		}

	}
	void DodajSadrzaj(const char* sadrzaj, const char* ime, DatumVrijeme dat) {
		if (_sadrzaj != nullptr) {
			char *temp = new char[strlen(_sadrzaj) + 1 + strlen(sadrzaj) + 1];
			strcpy_s(temp, strlen(_sadrzaj) + 1, _sadrzaj);//
			delete[] _sadrzaj;
			_sadrzaj = temp;
			strcat_s(&_sadrzaj[strlen(_sadrzaj)], strlen(sadrzaj) + 1, sadrzaj);
			temp = nullptr;
		}

		if (_sadrzaj == nullptr) {
			_sadrzaj = AlocirajNizKaraktera(sadrzaj);
		}

		ofstream file;
		file.open(_naziv, ios::out | ios::binary | ios::app);
		file << _sadrzaj << endl;
		file.close();

		Modifikacija *temp = new Modifikacija[_brojModifikacija + 1];
		for (int i = 0; i < _brojModifikacija; i++) {
			temp[i].Unos(_modifikacije[i]._korisnicnkoIme, _modifikacije[i]._aktivnost, _modifikacije[i]._vrijemeModifikacije);
		}
		temp[_brojModifikacija].Unos(ime, DODAVANJE, dat);
		for (int i = 0; i < _brojModifikacija; i++) {
			_modifikacije[i].Dealociraj();
		}
		delete[] _modifikacije;
		_modifikacije = temp; temp = nullptr;
		_brojModifikacija++;
	}
	//vrijeme kreiranja dokumenta je ujedno i vrijeme posljednje modifikacije
	void Unos(vrstaDokumenta vrsta, const char * naziv, DatumVrijeme kreiran) {
		_naziv = AlocirajNizKaraktera(naziv);
		_vrsta = make_unique<vrstaDokumenta>(vrsta);
		_brojModifikacija = 0;
		_modifikacije = new Modifikacija[_brojModifikacija + 1];
		_modifikacije->Unos("", KREIRANJE, kreiran);
		_brojModifikacija++;
		_sadrzaj = nullptr;
	}
	void Dealociraj() {
		delete[] _naziv; _naziv = nullptr;

		delete[] _sadrzaj; _sadrzaj = nullptr;
		for (int i = 0; i < _brojModifikacija; i++)
			_modifikacije[i].Dealociraj();
		delete[] _modifikacije;
		_modifikacije = nullptr;
	}
	void Ispis() {
		cout << crt << _naziv << " " << vrstaDokumentaChar[*_vrsta] << " kreiran ";
		if (_sadrzaj != nullptr)
			cout << crt << _sadrzaj << crt;


		for (size_t i = 0; i < _brojModifikacija; i++)
			_modifikacije[i].Ispis();
	}
	bool SamoSlova() {
		for (int i = 0; i < strlen(_naziv); i++) {
			if (!((_naziv[i] >= 'A' && _naziv[i] <= 'Z') || (_naziv[i] >= 'a' && _naziv[i] <= 'z') || _naziv[i] == '.')) {
				return false;
			}
		}
		return true;
	}
	bool ExtenzijaOk(vrstaDokumenta vrsta) {
		/*int broj_karaktera = strlen(_naziv);
		char ext[4]="XXX";
		int br_ext = 2;
		for (int i = broj_karaktera-1; i >=broj_karaktera - 3; i--) {
		ext[br_ext] = toupper(_naziv[i]);
		br_ext--;
		}
		cout << "Poredi se: " << ext << " sa " << vrstaDokumentaChar[vrsta] << endl;*/

		/*char *ext = &_naziv[strlen(_naziv) - 3];
		cout << "Poredi se: " << ext << " sa " << vrstaDokumentaChar[vrsta] << endl; */
		//ili...
		char *ext2 = new char[4];
		strcpy_s(ext2, 4, &_naziv[strlen(_naziv) - 3]);

		for (int i = 0; i < 3; i++) {
			ext2[i] = toupper(ext2[i]);
		}
		//cout << "Poredi se: " << ext2 << " sa " << vrstaDokumentaChar[vrsta] << endl;

		if (strcmp(ext2, vrstaDokumentaChar[vrsta]) == 0) {
			return true;
		}
		else return false;
	}
};
struct Printer {
	char * _model;
	Dokument * _dokumenti;
	int _trenutnoDokumenata;
	//REKURZIVNA FUNKCIJA VRACA PROSJECAN BROJ MODIFIKACIJA ODREDJENE VRSTE KOJE SU VRSENE NAD PRINTANIM DOKUMENTIMA
	int GetProsjecanBrojModifikacijaByVrsta(vrstaAktivnosti vrst = DODAVANJE, int br_mod = 0, int br_dok = 0, int br = 0) {
		if (br_dok == _trenutnoDokumenata) {
			return (br / 2);
		}
		if (br_mod == _dokumenti[br_dok]._brojModifikacija) {
			br_dok++;
			br_mod = 0;
			GetProsjecanBrojModifikacijaByVrsta(vrst, br_mod, br_dok, br);
		}
		if (_trenutnoDokumenata != br_dok) {
			if (_dokumenti[br_dok]._modifikacije[br_mod]._aktivnost == vrst) {
				br_mod++;
				br++;
				GetProsjecanBrojModifikacijaByVrsta(vrst, br_mod, br_dok, br);
			}
			if (_dokumenti[br_dok]._modifikacije[br_mod]._aktivnost != vrst) {
				br_mod++;
				GetProsjecanBrojModifikacijaByVrsta(vrst, br_mod, br_dok, br);
			}
		}

	}

	/*
	DA BI PRINTER ISPRINTAO NEKI DOKUMENT MORAJU BITI ZADOVOLJENA SLJEDECA PRAVILA:
	1. NAZIV DOKUMENTA MOZE SADRZAVATI SAMO SLOVA, A EKSTENZIJA MORA BITI IDENTICNA ONOJ DEFINISANOJ VRIJEDNOSCU ATRIBUTA vrstaDokumenta
	2. NAD DOKUMENTOM MORAJU BITI IZVRSENE SLJEDECE MODIFIKACIJE:
	- 1 KREIRANJE
	- 1 ILI VIŠE DODAVANJA
	- 1 ILI VIŠE ZAMJENA SADRZAJA
	SAMO DOKUMENTI KOJI ZADOVOLJE USLOVE PRINTANJA TREBAJU BITI SACUVANI U NIZ _dokumenti. NA KONZOLU ISPISATI SADRZAJ DOKUMENTA KOJI SE PRINTA, A SA CRT LINIJOM ODVAJATI STRANICE DOKUMENTA.
	BROJ STRANICA DOKUMENTA SE AUTOMATSKI ODREDJUJE PRILIKOM  PRINTANJA, A ZAVISI OD VRIJEDNOSTI BROJ_ZNAKOVA_PO_STRANICI UKLJUCUJUCI RAZMAKE I DRUGE VRSTE ZNAKOVA.
	ONEMOGUCITI PRINTANJE DOKUMENTA OSOBAMA KOJE NISU NAPRAVILE NAJMANJE JEDNU MODIFIKACIJU NA TOM DOKUMENTU. NAKON PRINTANJA SADRZAJA DOKUMENTA, POTREBNO JE DOKUMENTU DODATI I MODIFIKACIJU "PRINTANJE".
	*/
	bool Printaj(Dokument &dok, const char* ime, DatumVrijeme dat) {
		if (!dok.SamoSlova()) {
			return false;
		}
		if (!dok.ExtenzijaOk(*dok._vrsta)) {
			return false;
		}
		bool kreiranje, dodavanje, zamjena;
		kreiranje = dodavanje = zamjena = false;
		bool printReady = false;

		for (int i = 0; i < dok._brojModifikacija; i++) {
			if (dok._modifikacije[i]._aktivnost == KREIRANJE) {
				kreiranje = true;
			}
			if (dok._modifikacije[i]._aktivnost == DODAVANJE) {
				dodavanje = true;
			}
			if (dok._modifikacije[i]._aktivnost == ZAMJENA) {
				zamjena = true;
			}
		}
		if (kreiranje == true && dodavanje == true && zamjena == true) {
			printReady = true;
		}
		if (printReady == true) {
			Dokument *temp = new Dokument[_trenutnoDokumenata + 1];
			for (int i = 0; i < _trenutnoDokumenata; i++) {
				temp[i].Unos2(*_dokumenti[i]._vrsta, _dokumenti[i]._naziv, _dokumenti[i]._sadrzaj, _dokumenti[i]._modifikacije, _dokumenti[i]._brojModifikacija);
				_dokumenti[i].Dealociraj();
			}
			if (_dokumenti != nullptr)
				delete[] _dokumenti; _dokumenti = nullptr;
			temp[_trenutnoDokumenata].Unos2(*dok._vrsta, dok._naziv, dok._sadrzaj, dok._modifikacije, dok._brojModifikacija);
			_dokumenti = temp; temp = nullptr;
			_trenutnoDokumenata++;

			dok.Ispis2();
			dok.AddModifikacija(ime, dat, PRINTANJE);
			cout << "ModifikacijaAddPrint" << endl;
			return true;
		}
		else return false;



	}
	void Unos(const char * model) {
		_model = AlocirajNizKaraktera(model);
		_dokumenti = nullptr;
		_trenutnoDokumenata = 0;
	}
	void Dealociraj() {
		delete[] _model; _model = nullptr;
		for (size_t i = 0; i < _trenutnoDokumenata; i++)
			_dokumenti[i].Dealociraj();
		delete[]_dokumenti; _dokumenti = nullptr;
	}
	void Ispis() {
		cout << _model << crt;
		for (size_t i = 0; i < _trenutnoDokumenata; i++)
			_dokumenti[i].Ispis();
	}
	pair<Modifikacija*, int>GetModifikacijeByDatum(DatumVrijeme Od, DatumVrijeme Do) {
		int br_mod = 0;
		for (int i = 0; i < _trenutnoDokumenata; i++) {
			for (int j = 0; j < _dokumenti[i]._brojModifikacija; j++) {
				if (_dokumenti[i]._modifikacije[j]._vrijemeModifikacije.LaterThan(Od) && _dokumenti[i]._modifikacije[j]._vrijemeModifikacije.LaterThan(Do)) {
					br_mod++;
				}
			}
		}
		Modifikacija *niz = new Modifikacija[br_mod];
		br_mod = 0;
		for (int i = 0; i < _trenutnoDokumenata; i++) {
			for (int j = 0; j < _dokumenti[i]._brojModifikacija; j++) {
				if (_dokumenti[i]._modifikacije[j]._vrijemeModifikacije.LaterThan(Od) && _dokumenti[i]._modifikacije[j]._vrijemeModifikacije.LaterThan(Do)) {
					niz[br_mod].Unos(_dokumenti[i]._modifikacije[j]._korisnicnkoIme, _dokumenti[i]._modifikacije[j]._aktivnost, _dokumenti[i]._modifikacije[j]._vrijemeModifikacije);
					br_mod++;
				}
			}
		}
		if (niz != nullptr)
			return make_pair(niz, br_mod);
		else return make_pair(nullptr, br_mod);
	}
	int Zamijeni(const char znak, const char znak2, const char* ime, DatumVrijeme dat) {
		int broj_znakova = 0;
		auto pok = [&]()->int {
			
			for (int i = 0; i < _trenutnoDokumenata; i++) {
				for (int j = 0; j < _dokumenti[i]._brojModifikacija; j++) {
					if (_dokumenti[i]._modifikacije[j]._aktivnost == PRINTANJE && _dokumenti[i]._modifikacije[j]._vrijemeModifikacije.Equal(dat)) {
						for (int z = 0; z < strlen(_dokumenti[i]._sadrzaj)-1; z++) {
							if (_dokumenti[i]._sadrzaj[z] == znak) {
								_dokumenti[i]._sadrzaj[z] = znak2;
								broj_znakova++;
							}
						}
						_dokumenti[i].AddModifikacija(ime, dat, ZAMJENA);
					}
				}

			}
			return broj_znakova;
		}();
		return broj_znakova;

	}

};


void main() {
	DatumVrijeme prije3Dana; prije3Dana.Unos(3, 2, 2018, 10, 15);
	DatumVrijeme danas1; danas1.Unos(6, 2, 2018, 10, 15);//U KONTEKSTU MODIFIKACIJE, UNOS PODRAZUMIJEVA KREIRANJE
	DatumVrijeme danas2; danas2.Unos(6, 2, 2018, 10, 16);
	DatumVrijeme za10Dana; za10Dana.Unos(16, 2, 2018, 10, 15);

	Dokument ispitPRII, ispitMAT, ispitUIT, ispitUITDrugiRok;
	ispitPRII.Unos(DOC, "ispitPRII.doc", prije3Dana);
	ispitMAT.Unos(DOC, "ispitMAT.doc", danas1);
	ispitUIT.Unos(DOC, "ispitUIT.doc", danas2);
	ispitUITDrugiRok.Unos(PDF, "ispitUITDrugiRok.pdf", za10Dana);

	/*DODAJE SADRZAJ U FAJL, ZADRZAVAJUCI POSTOJECI. SVAKO DODAVANJE EVIDENTIRATI KAO MODIFIKACIJU. PARAMETRI:SADRZAJ, KORISNICKO IME, VRIJEME MODIFIKACIJE*/
	ispitPRII.DodajSadrzaj("Programiranje ili racunarsko programiranje (engl. programming) jeste vjestina pomocu koje ", "denis.music", danas1);
	ispitPRII.DodajSadrzaj("korisnik stvara i izvrsava algoritme koristeci odredjene programske jezike da bi ... ", "indira.hamulic", danas2);

	ispitPRII.Ispis();//ISPISUJE SVE PODATKE O DOKUMENTU

	ispitMAT.DodajSadrzaj("Matematika se razvila iz potrebe da se obavljaju proracuni u trgovini, vrse mjerenja zemljista i predvidjaju ", "jasmin.azemovic", danas1);
	ispitMAT.DodajSadrzaj("astronomski dogadjaji, i ove tri primjene se mogu dovesti u vezu sa grubom podjelom matematike ", "adel.handzic", danas2);
	ispitMAT.AddModifikacija("Emir", za10Dana);
	ispitMAT.Ispis();
	Printer hp3200; hp3200.Unos("HP 3200");
	/*
	DA BI PRINTER ISPRINTAO NEKI DOKUMENT MORAJU BITI ZADOVOLJENA SLJEDECA PRAVILA:
	1. NAZIV DOKUMENTA MOZE SADRZAVATI SAMO SLOVA, A EKSTENZIJA MORA BITI IDENTICNA ONOJ DEFINISANOJ VRIJEDNOSCU ATRIBUTA vrstaDokumenta
	2. NAD DOKUMENTOM MORAJU BITI IZVRSENE SLJEDECE MODIFIKACIJE:
	- 1 KREIRANJE
	- 1 ILI VIŠE DODAVANJA
	- 1 ILI VIŠE ZAMJENA SADRZAJA
	SAMO DOKUMENTI KOJI ZADOVOLJE USLOVE PRINTANJA TREBAJU BITI SACUVANI U NIZ _dokumenti. NA KONZOLU ISPISATI SADRZAJ DOKUMENTA KOJI SE PRINTA, A SA CRT LINIJOM ODVAJATI STRANICE DOKUMENTA.
	BROJ STRANICA DOKUMENTA SE AUTOMATSKI ODREDJUJE PRILIKOM  PRINTANJA, A ZAVISI OD VRIJEDNOSTI BROJ_ZNAKOVA_PO_STRANICI UKLJUCUJUCI RAZMAKE I DRUGE VRSTE ZNAKOVA.
	ONEMOGUCITI PRINTANJE DOKUMENTA OSOBAMA KOJE NISU NAPRAVILE NAJMANJE JEDNU MODIFIKACIJU NA TOM DOKUMENTU. NAKON PRINTANJA SADRZAJA DOKUMENTA, POTREBNO JE DOKUMENTU DODATI I MODIFIKACIJU "PRINTANJE".
	*/
	if (hp3200.Printaj(ispitPRII, "denis.music", za10Dana))
		cout << "Printam -> " << ispitPRII._naziv << endl;
	if (hp3200.Printaj(ispitMAT, "denis.music", za10Dana))
		cout << "Printam -> " << ispitMAT._naziv << endl;
	if (hp3200.Printaj(ispitUIT, "iris.memic", za10Dana))
		cout << "Printam -> " << ispitUIT._naziv << endl;
	if (hp3200.Printaj(ispitUITDrugiRok, "goran.skondric", za10Dana))
		cout << "Printam -> " << ispitUITDrugiRok._naziv << endl;

	//REKURZIVNA FUNKCIJA VRACA PROSJECAN BROJ MODIFIKACIJA ODREDJENE VRSTE KOJE SU VRSENE NAD PRINTANIM DOKUMENTIMA
	cout << "Prosjecan broj modifikacija odrejdne vrste printanih dokumenata je -> " << hp3200.GetProsjecanBrojModifikacijaByVrsta(DODAVANJE,0,0,0) << crt;
	
	Modifikacija * pok = nullptr;
	int brojac = 0;
	//FUNKCIJA GetModifikacijeByDatum VRACA SVE MODIFIKACIJE KOJE SU NAPRAVLJENE NA DOKUMENTIMA U PERIODU OD - DO (PROSLIJEDJENOM KAO PARAMETAR)
	cout << "------------------------------------------------------------<<" << endl;
	tie(pok, brojac) = hp3200.GetModifikacijeByDatum(danas1, za10Dana);
	if (brojac > 0 && pok != nullptr)
		for (size_t i = 0; i < brojac; i++)
			pok[i].Ispis();

	//ISPISUJE INFORMACIJE O SVIM DOKUMENTIMA KOJI SU ISPRINTANI
	cout << "------------------------------------------------------------<<" << endl;
	hp3200.Ispis();
	cout << "------------------------------------------------------------<<" << endl;
	/*NAPISATI LAMBDA FUNKCIJU ZamijeniIPrintaj KOJA U SADRZAJU SVIH PRINTANIH DOKUMENATA MIJENJA PRVI POSLATI PARAMETAR SA VRIJEDNOSCU DRUGOG PARAMETRA, TE VRACA BROJ ZAMIJENJENIH ZNAKOVA*/
	//RAZMAK MIJENJA ZNAKOM CRTICA U SADRZAJU DOKUMENATA KOJI SU PRINTANI danas1. ZAMJENU REGISTROVATI KAO MODIFIKACIJU
	int zamijenjeno = hp3200.Zamijeni(' ', '-', "denis.music", za10Dana);//danas1
	cout << "Zamijenjeno -> " << zamijenjeno << " znakova" << endl;
	hp3200.Ispis();

	prije3Dana.Dealociraj(); danas1.Dealociraj(); danas2.Dealociraj(); za10Dana.Dealociraj();
	ispitMAT.Dealociraj(); ispitPRII.Dealociraj(); ispitUIT.Dealociraj(); ispitUITDrugiRok.Dealociraj();

	hp3200.Dealociraj();

	system("PAUSE");
}