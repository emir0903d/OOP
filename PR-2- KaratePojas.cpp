#include<iostream>
#include<memory>
#include<tuple>
using namespace std;
/*
1. BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA. U SUPROTNOM SE RAD NECE BODOVATI
2. STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
3. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB160061.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA. NE PREDAVATI .TXT ILI .CPP FAJLOVE
4. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
5. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI CETE KOPIRATI VAŠA RJEŠENJA)
6. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAĐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
*/
//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)


enum VrstaAktivnosti { Tehnike, Kata, Borba };
const char*va[] = { "Tehnike","Kata","Borba" };
enum Pojas { Zuti, Narandzasti, Zeleni, Plavi, Smedji, Crni };
const char*p[] = { "Zuti","Narandzasti","Zeleni","Plavi","Smedji","Crni" };
struct Datum {
	int * _dan, *_mjesec, *_godina;
	void Unos(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	void Ispis() { cout << *_dan << "/" << *_mjesec << "/" << *_godina << endl; }
	void Dealociraj() { delete _dan; delete _mjesec; delete _godina; }
	bool IdenticanDatum(Datum dat) {
		if (*_dan == *dat._dan && *_mjesec == *dat._mjesec && *_godina == *dat._godina) {
			return true;
		}
		else return false;
	}
	bool ManjeOd15Dana(Datum dat) {
		if (abs((*_dan + *_mjesec * 30 + *_godina * 365) - (*dat._dan + *dat._mjesec * 30 + *dat._godina + 365)) <= 15) {
			return true;
		}
		else return false;
	}
};
struct Aktivnost {
	unique_ptr<VrstaAktivnosti> _vrsta;
	Datum _datumIzvrsenja;
	char * _nazivOpis;
	int _ocjena; // 1 - 10 
	void Unos(VrstaAktivnosti vrsta, Datum * datum, int ocjena, const char * nazivOpis) {
		_vrsta = make_unique<VrstaAktivnosti>(vrsta);
		_datumIzvrsenja.Unos(*datum->_dan, *datum->_mjesec, *datum->_godina);
		_ocjena = ocjena;
		int size = strlen(nazivOpis) + 1;
		_nazivOpis = new char[size];
		strcpy_s(_nazivOpis, size, nazivOpis);
	}
	void Ispis() {
		cout << va[*_vrsta] << " " << _ocjena << " " << _nazivOpis;
		
		_datumIzvrsenja.Ispis();
		cout << endl;
	}
	void Dealociraj() {
		_datumIzvrsenja.Dealociraj();
		delete[] _nazivOpis; _nazivOpis = nullptr;
	}
	bool IdenticnaAktivnost(Aktivnost &akt) {
		if (_datumIzvrsenja.IdenticanDatum(akt._datumIzvrsenja) && _vrsta==akt._vrsta) {
			return true;
		}
		else return false;
	}
};
struct KaratePojas {
	Datum * _datumPolaganja;//datum koji ce se evidentirati kao datum polaganja pojasa tj. kada su ispunjene sve aktivnosti/obaveze
	Pojas _pojas;
	Aktivnost * _listaIzvrsenihAktivnosti;
	int _trenutnoIzvrsenihAktivnosti;
	/*Na osnovu vrijednosti primljenog parametra osigurati dodavanje novoizvrsene aktivnosti za potrebe stjecanja odredjenog pojasa. Broj aktivnosti nije ogranicen.
	Identicna aktivnost se moze dodati jedino u slucaju kada je prethodna (identivna aktivnost po vrsti i datumu izvrsenja) imala ocjenu manju od 6.
	Uspjesnom aktivnoscu se smatraju one aktivnosti koje imaju ocjenu vecu od 5, a svaka naredna identicna aktivnost, bez obzira da li je uspjesna ili ne,
	moze biti dodana jedino ako je proslo najmanje 15 dana od izvrsenja prethodne. Onemoguciti dodavanje aktivnosti uspjesno polozenom pojasu.*/
	bool DodajIzvrsenuAktivnost(Aktivnost *akt) {
		if (_datumPolaganja != nullptr) {
			return false;
		}
		for (int i = 0; i < _trenutnoIzvrsenihAktivnosti; i++) {
			if (_listaIzvrsenihAktivnosti[i].IdenticnaAktivnost(*akt) == true) {
				if (_listaIzvrsenihAktivnosti[i]._ocjena >= 6) {
					return false;
				}
				if (_listaIzvrsenihAktivnosti[i]._datumIzvrsenja.ManjeOd15Dana(akt->_datumIzvrsenja) == true) {
					return false;
				}
			}
		}
		Aktivnost *temp = new Aktivnost[_trenutnoIzvrsenihAktivnosti + 1];
		for (int i = 0; i < _trenutnoIzvrsenihAktivnosti; i++) {
			temp[i].Unos(*_listaIzvrsenihAktivnosti[i]._vrsta, &_listaIzvrsenihAktivnosti[i]._datumIzvrsenja, _listaIzvrsenihAktivnosti[i]._ocjena, _listaIzvrsenihAktivnosti[i]._nazivOpis);
		}
		temp[_trenutnoIzvrsenihAktivnosti].Unos(*akt->_vrsta, &akt->_datumIzvrsenja, akt->_ocjena, akt->_nazivOpis);
		for (int i = 0; i < _trenutnoIzvrsenihAktivnosti; i++) {
			_listaIzvrsenihAktivnosti[i].Dealociraj();
		}
		delete[] _listaIzvrsenihAktivnosti;
		_listaIzvrsenihAktivnosti = temp;
		temp = nullptr;
		_trenutnoIzvrsenihAktivnosti++;
		return true;

	

	}
	void Unos(Pojas pojas) {
		_listaIzvrsenihAktivnosti = nullptr;
		_trenutnoIzvrsenihAktivnosti = 0;
		_datumPolaganja = nullptr;
		_pojas = pojas;
	}
	void Dealociraj() {
		if (_datumPolaganja != nullptr) {
			_datumPolaganja->Dealociraj();
			delete[] _datumPolaganja;
			_datumPolaganja = nullptr;
		}
		
		for (int i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
			_listaIzvrsenihAktivnosti[i].Dealociraj();
		delete[] _listaIzvrsenihAktivnosti;
		_listaIzvrsenihAktivnosti = nullptr;
	}
	void Ispis() {
		if(_datumPolaganja!=nullptr)
		_datumPolaganja->Ispis();
		cout << p[_pojas] << endl;
		for (int i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
			_listaIzvrsenihAktivnosti[i].Ispis();
	}
	void Sortiraj() {
		bool sort = false;
		while (sort = false) {
			sort = true;
			for (int i = 0; i < _trenutnoIzvrsenihAktivnosti - 1; i++) {
				if (_listaIzvrsenihAktivnosti[i]._ocjena > _listaIzvrsenihAktivnosti[i + 1]._ocjena) {
					Aktivnost temp;
					temp.Unos(*_listaIzvrsenihAktivnosti[i]._vrsta, &_listaIzvrsenihAktivnosti[i]._datumIzvrsenja, _listaIzvrsenihAktivnosti[i]._ocjena, _listaIzvrsenihAktivnosti[i]._nazivOpis);
					_listaIzvrsenihAktivnosti[i].Dealociraj();
					_listaIzvrsenihAktivnosti[i].Unos(*_listaIzvrsenihAktivnosti[i + 1]._vrsta, &_listaIzvrsenihAktivnosti[i + 1]._datumIzvrsenja, _listaIzvrsenihAktivnosti[i + 1]._ocjena, _listaIzvrsenihAktivnosti[i + 1]._nazivOpis);
					_listaIzvrsenihAktivnosti[i + 1].Dealociraj();
					_listaIzvrsenihAktivnosti[i + 1].Unos(*temp._vrsta, &temp._datumIzvrsenja, temp._ocjena, temp._nazivOpis);
					temp.Dealociraj();
					sort = false;
				}
			}
		}

	}
	/* Karate pojas zahtijeva uspjesnu realizaciju svih planiranih aktivnosti, a one se za jedan povecavaju sa svakim novim pojasom, npr.
	zuti pojas: 1 x tehnika, 1 x kata, 1 x borba;	narandzasti pojas: 2 x tehnika, 2 x kata, 2 x borba; i td...
	Funkcija vraca false u slucaju da: su kandidatu u listu aktivnosti evidentirane tri negativno ocijenjene identicne vrste aktivnosti 
	(npr. tri negativne ocjene iz borbi), onda se taj pojas ne moze smatrati uspjesno stecenim i, 
	te ukoliko nedostaje bilo koja od aktivnosti zahtijevanih u okviru tog pojasa. 
	Ukoliko je kandidat uspjesno realizovao sve aktivnost, datum polaganja se postavlja na datum posljednje uspjesno realizovane aktivnosti*/
	bool DaLiJePolozen() {
		int b_teh = 0;
		int b_kat = 0;
		int b_bor = 0;
		for (int i = 0; i < _trenutnoIzvrsenihAktivnosti; i++) {
			if (*_listaIzvrsenihAktivnosti[i]._vrsta == 0) {
				if(_listaIzvrsenihAktivnosti[i]._ocjena<=5)
					b_teh++;
			}
			if (*_listaIzvrsenihAktivnosti[i]._vrsta == 1) {
				if (_listaIzvrsenihAktivnosti[i]._ocjena <= 5)
					b_kat++;
			}
			if (*_listaIzvrsenihAktivnosti[i]._vrsta == 2) {
				if (_listaIzvrsenihAktivnosti[i]._ocjena <= 5)
					b_bor++;
			}
		}
		if (b_teh == 3 || b_kat == 3 || b_bor == 3) {
			return false;
		}
		b_teh = b_kat = b_bor = 0;
		for (int i = 0; i < _trenutnoIzvrsenihAktivnosti; i++) {

			if (*_listaIzvrsenihAktivnosti[i]._vrsta == 0) {
				if (_listaIzvrsenihAktivnosti[i]._ocjena > 5)
					b_teh++;
			}
			if (*_listaIzvrsenihAktivnosti[i]._vrsta == 1) {
				if (_listaIzvrsenihAktivnosti[i]._ocjena > 5)
					b_kat++;
			}
			if (*_listaIzvrsenihAktivnosti[i]._vrsta == 2) {
				if (_listaIzvrsenihAktivnosti[i]._ocjena > 5)
					b_bor++;
			}

		}
		if (b_teh == 1 && b_kat == 1 && b_bor == 1) {
			_datumPolaganja = new Datum;
			_datumPolaganja->Unos(*_listaIzvrsenihAktivnosti[_trenutnoIzvrsenihAktivnosti]._datumIzvrsenja._dan, *_listaIzvrsenihAktivnosti[_trenutnoIzvrsenihAktivnosti]._datumIzvrsenja._mjesec, *_listaIzvrsenihAktivnosti[_trenutnoIzvrsenihAktivnosti]._datumIzvrsenja._godina);
			return true;
		}
		if (b_teh == 2 && b_kat == 2 && b_bor == 2) {
			_datumPolaganja = new Datum;
			_datumPolaganja->Unos(*_listaIzvrsenihAktivnosti[_trenutnoIzvrsenihAktivnosti]._datumIzvrsenja._dan, *_listaIzvrsenihAktivnosti[_trenutnoIzvrsenihAktivnosti]._datumIzvrsenja._mjesec, *_listaIzvrsenihAktivnosti[_trenutnoIzvrsenihAktivnosti]._datumIzvrsenja._godina);
			return true;
		}
		if (b_teh == 3 && b_kat == 3 && b_bor == 3) {
			_datumPolaganja = new Datum;
			_datumPolaganja->Unos(*_listaIzvrsenihAktivnosti[_trenutnoIzvrsenihAktivnosti]._datumIzvrsenja._dan, *_listaIzvrsenihAktivnosti[_trenutnoIzvrsenihAktivnosti]._datumIzvrsenja._mjesec, *_listaIzvrsenihAktivnosti[_trenutnoIzvrsenihAktivnosti]._datumIzvrsenja._godina);
			return true;
		}
		if (b_teh == 4 && b_kat == 4 && b_bor == 4) {
			_datumPolaganja = new Datum;
			_datumPolaganja->Unos(*_listaIzvrsenihAktivnosti[_trenutnoIzvrsenihAktivnosti]._datumIzvrsenja._dan, *_listaIzvrsenihAktivnosti[_trenutnoIzvrsenihAktivnosti]._datumIzvrsenja._mjesec, *_listaIzvrsenihAktivnosti[_trenutnoIzvrsenihAktivnosti]._datumIzvrsenja._godina);
			return true;
		}
		if (b_teh == 5 && b_kat == 5 && b_bor == 5) {
			_datumPolaganja = new Datum;
			_datumPolaganja->Unos(*_listaIzvrsenihAktivnosti[_trenutnoIzvrsenihAktivnosti]._datumIzvrsenja._dan, *_listaIzvrsenihAktivnosti[_trenutnoIzvrsenihAktivnosti]._datumIzvrsenja._mjesec, *_listaIzvrsenihAktivnosti[_trenutnoIzvrsenihAktivnosti]._datumIzvrsenja._godina);
			return true;
		}
		if (b_teh == 6 && b_kat == 6 && b_bor == 6) {
			_datumPolaganja = new Datum;
			_datumPolaganja->Unos(*_listaIzvrsenihAktivnosti[_trenutnoIzvrsenihAktivnosti]._datumIzvrsenja._dan, *_listaIzvrsenihAktivnosti[_trenutnoIzvrsenihAktivnosti]._datumIzvrsenja._mjesec, *_listaIzvrsenihAktivnosti[_trenutnoIzvrsenihAktivnosti]._datumIzvrsenja._godina);
			return true;
		}
		cout << "U procesu polaganja!" << endl;
		return false;
	}
	/*Funkcija vraca prosjecnu ocjenu svih uspjesno realizovanih aktivnosti koja u nazivu ili opisu sadrze vrijednost primljenog parametra.
	Ukoliko smatrate da je potrebno, mozete dodati i druge parametre za potrebe implementacije ove funkcije*/
	float PretragaRekrzivno(const char* rijec, int brojac=0) {
		/*float suma = 0.0;
		int brojac = 0;
		for (int i = 0; i < _trenutnoIzvrsenihAktivnosti; i++) {
			if (strstr(_listaIzvrsenihAktivnosti[i]._nazivOpis, rijec) != nullptr) {
				if (_listaIzvrsenihAktivnosti[i]._ocjena > 5) {
					suma += _listaIzvrsenihAktivnosti[i]._ocjena;
					brojac++;
				}
				
			}
		}
		return suma/brojac;*/
		if (_listaIzvrsenihAktivnosti[brojac]._ocjena < 6) {
			return PretragaRekrzivno(rijec, ++brojac);
		}
		if (brojac == _trenutnoIzvrsenihAktivnosti-1) {
			return +PretragaRekrzivno(rijec,brojac)/brojac;
		}
		if (_listaIzvrsenihAktivnosti[brojac]._ocjena > 5) {
			if (strstr(_listaIzvrsenihAktivnosti[brojac]._nazivOpis, rijec) != nullptr) {
				return +PretragaRekrzivno(rijec, ++brojac);
			}
		}

	}
};

struct Kandidat {
	char * _imePrezime;
	shared_ptr<KaratePojas> _pojasevi[6];
	void Unos(const char * imePrezime) {
		int size = strlen(imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, imePrezime);
		for (int i = 0; i < 6; i++){
			_pojasevi[i] = nullptr;
		}
	}
	void Dealociraj() {
		delete[] _imePrezime; _imePrezime = nullptr;
		for (int i = 0; i < 6; i++)
			if (_pojasevi[i] != nullptr) {
				//_pojasevi[i]->Dealociraj();
				_pojasevi[i].reset();
			}
	}
	void Ispis() {
		cout << _imePrezime << endl;
		for (int i = 0; i < 6; i++)
			if (_pojasevi[i] != nullptr)
				_pojasevi[i]->Ispis();
	}
	/*
	Karate pojasevi se moraju dodavati po redoslijedu tj. ne smije se dozvoliti dodavanje zelenog pojasa ukoliko prethodno nije dodan zuti i narandzasti.
	Za provjeru lokacije (unutar funkcije DodajPojas) na koju ce se dodati novi karate pojas, te da li su nizi pojasevi prethodno dodani koristiti 
	lambda funkciju.
	*/
	bool DodajPojas(KaratePojas pojas) {
		bool add = false;

		auto funk = [&]() {
			if (pojas._pojas == 0) {
				if (_pojasevi[0] == nullptr) {
					add=true;
					return add;
				}
			}
			if (pojas._pojas == 1) {
				if (_pojasevi[0] != nullptr) {
					if (_pojasevi[0]->_pojas == 0) {
						add = true;
						return add;
					}
					
				}
			}
			if (pojas._pojas == 2) {
				if (_pojasevi[1] != nullptr) {
					if (_pojasevi[1]->_pojas == 1) {
						add = true;
						return add;
					}

				}
			}
			if (pojas._pojas == 3) {
				if (_pojasevi[2] != nullptr) {
					if (_pojasevi[2]->_pojas == 2) {
						add = true;
						return add;
					}

				}
			}
			if (pojas._pojas == 4) {
				if (_pojasevi[3] != nullptr) {
					if (_pojasevi[3]->_pojas == 3) {
						add = true;
						return add;
					}

				}
			}
			if (pojas._pojas == 5) {
				if (_pojasevi[4] != nullptr) {
					if (_pojasevi[4]->_pojas == 4) {
						add = true;
						return add;
					}

				}
			}
			return add;
		}();
		if (add == true) {
			for (int i = 0; i < 6; i++) {
				if (_pojasevi[i] == nullptr) {
					_pojasevi[i] = make_shared<KaratePojas>(pojas);
					return true;
				}
			}
		}
		else return false;
	}
	pair<Pojas, float>GetNajbolji() {
		float max = 0.0;
		int index = -1;
		for (int i = 0; i < 6; i++) {
			if (_pojasevi[i] != nullptr) {
				float suma = 0.0;
				int brojac = 0;
				for (int j = 0; j < _pojasevi[i]->_trenutnoIzvrsenihAktivnosti; j++) {
					if (_pojasevi[i]->_listaIzvrsenihAktivnosti[j]._ocjena > 5) {
						suma += _pojasevi[i]->_listaIzvrsenihAktivnosti[j]._ocjena;
						brojac++;
					}
					
				}
				if (suma / brojac >= max) {
					max = suma / brojac;
					index = i;
				}
			}
			
		}
		return make_pair(_pojasevi[index]->_pojas, max);
	}
};

void main() {

	//BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA
	//STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
	Datum datumPolaganja1, datumPolaganja2, datumPolaganja3, datumPolaganja4;
	datumPolaganja1.Unos(10, 6, 2018);
	datumPolaganja2.Unos(18, 6, 2018);
	datumPolaganja3.Unos(22, 3, 2018);
	datumPolaganja4.Unos(22, 7, 2018);

	Aktivnost aktivnost1, aktivnost2, aktivnost3, aktivnost4, aktivnost5;
	aktivnost1.Unos(Tehnike, &datumPolaganja1, 6, "Tehnike za zuti pojas");
	aktivnost2.Unos(Kata, &datumPolaganja1, 8, "Taiki joko shodan - zuti pojas");
	aktivnost3.Unos(Borba, &datumPolaganja1, 2, "Jednostavne borbene tehnike sa partnerom");
	aktivnost4.Unos(Borba, &datumPolaganja1, 6, "Jednostavne borbene tehnike sa partnerom");
	aktivnost5.Unos(Borba, &datumPolaganja4, 6, "Jednostavne borbene tehnike sa partnerom");

	KaratePojas pojasZuti, pojasNarandzasti, pojasZeleni;
	pojasZuti.Unos(Zuti);
	pojasNarandzasti.Unos(Narandzasti);
	pojasZeleni.Unos(Zeleni);

	
	if (pojasZuti.DodajIzvrsenuAktivnost(&aktivnost1))
		cout << "Aktivnost uspjesno dodana!" << endl;
	if (pojasZuti.DodajIzvrsenuAktivnost(&aktivnost2))
		cout << "Aktivnost uspjesno dodana!" << endl;
	if (pojasZuti.DodajIzvrsenuAktivnost(&aktivnost3))
		cout << "Aktivnost uspjesno dodana!" << endl;
	if (pojasZuti.DodajIzvrsenuAktivnost(&aktivnost4))//15 dana...
		cout << "Aktivnost uspjesno dodana!" << endl;
	if (pojasZuti.DodajIzvrsenuAktivnost(&aktivnost5))
		cout << "Aktivnost uspjesno dodana!" << endl;

	//Koristeci neki od obradjenih algoritama, po ocjeni sortirati aktivnosti u okviru odredjenog pojasa
	pojasZuti.Sortiraj();


	
	if (pojasZuti.DaLiJePolozen())
		pojasZuti.Ispis();

	
	cout << "Prosjecna ocjena za zuti pojas -> " << pojasZuti.PretragaRekrzivno("pojas") << endl;

	//ispisuje sve dostupne podatke o pojasu
	pojasZuti.Ispis();

	Kandidat jasmin;
	jasmin.Unos("Jasmin Azemovic");
	
	if (jasmin.DodajPojas(pojasZuti))
		cout << "Pojas uspjesno dodan!" << endl;
	if (jasmin.DodajPojas(pojasZeleni))//prethodno je trebao biti dodan narandzasti pojas
		cout << "Pojas uspjesno dodan!" << endl;
	if (jasmin.DodajPojas(pojasNarandzasti))
		cout << "Pojas uspjesno dodan!" << endl;

	float prosjek = 0;
	Pojas pojas;
	//Funkcija GetNajbolji vraca par koji sadrzi oznaku i prosjecnu ocjenu (uspjesno okoncanih aktivnosti) pojasa sa najvecim prosjekom
	tie(pojas, prosjek) = jasmin.GetNajbolji();
	cout << "Najbolji rezultat od " << prosjek << " je ostvaren tokom stjecanja pojasa " << p[pojas] << endl;
	
	datumPolaganja1.Dealociraj(); 
	datumPolaganja2.Dealociraj(); 
	datumPolaganja3.Dealociraj(); 
	datumPolaganja4.Dealociraj();
	aktivnost1.Dealociraj(); 
	aktivnost2.Dealociraj(); 
	aktivnost3.Dealociraj(); 
	aktivnost4.Dealociraj(); 
	aktivnost5.Dealociraj();
	pojasZuti.Dealociraj(); 
	pojasNarandzasti.Dealociraj(); 
	pojasZeleni.Dealociraj();
	jasmin.Dealociraj();

	system("pause");
}