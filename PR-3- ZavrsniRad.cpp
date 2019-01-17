#include<iostream>
#include<string>
#include<vector>
#include<thread>
#include<mutex>
#include<regex>

/****************************************************************************
1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ÆE BITI OZNACENO KAO "RE"
3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEÈILO GUBLJENJE URAÐENOG ZADATKA
4. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIÈNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUÈAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOÆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
5. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAÈENO.
****************************************************************************/

using namespace std;

enum Oblast { SoftverskiInzinjering, KomunikacijskiSistemi, SigurnostInformacijskihSistema, InteligentniSistemi };
const char* oblast_txt[] = { "Softverski inzinjering", "Komunikacijski sistemi", "Sigurnost informacijskih sistema", "Inteligentni sistemi" };

template<class T1, class T2, int max>
class Kolekcija {
	T1 _elementi1[max];
	T2 _elementi2[max];
	int _trenutno;
public:
	Kolekcija & operator=(const Kolekcija& obj) {
		_trenutno = obj._trenutno;
		for (int i = 0; i < _trenutno; i++) {
			_elementi1[i] = obj._elementi1[i];
			_elementi2[i] = obj._elementi2[i];
		}
		return *this;
	}
	Kolekcija(const Kolekcija& obj) {
		
		_trenutno = obj._trenutno;
		for (int i = 0; i < _trenutno; i++) {
			_elementi1[i] = obj._elementi1[i];
			_elementi2[i] = obj._elementi2[i];
		}

	}
	Kolekcija()
	{
		_trenutno = 0;
	}

	void AddElement(const T1& elem1, const T2& elem2)
	{
		if (_trenutno == max)
			throw exception("Dostigli ste maksimalan broj elemenata u kolekciji!");

		for (int i = 0; i < _trenutno; i++)
		{
			if (_elementi1[i] == elem1 || _elementi2[i] == elem2)
				throw exception("Nije moguce dodati duple elemente u kolekciju!");
		}

		_elementi1[_trenutno] = elem1;
		_elementi2[_trenutno++] = elem2;

	}

	T1 * getElementi1Pok() { return _elementi1; }
	T2 * getElementi2Pok() { return _elementi2; }
	T1 & getElement1(int lokacija) { return _elementi1[lokacija]; }
	T2 & getElement2(int lokacija) { return _elementi2[lokacija]; }
	int getTrenutno() { return _trenutno; }

	friend ostream & operator<<(ostream & COUT, Kolekcija<T1, T2, max> & obj)
	{
		for (size_t i = 0; i < obj._trenutno; i++)
		{
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
		}
		return COUT;
	}
};

//Deklaracija klase Nastavnik omogucava njeno koristenje u klasi ZavrsniRad, a definicija je data naknadno 
class Nastavnik;

class ZavrsniRad
{
	char* _nazivTeme;
	Oblast* _oblastTeme;
	string _datumPrijave; //Format: dd.MM.gggg
						  //U vector pohraniti samo adresu objekta tipa Nastavnik, dakle bez alokacije nove memorije
	vector<Nastavnik*> _komisija;
	string _datumOdbrane; //Format: dd.MM.gggg (najranije 2 mjeseca od datuma prijave)
	int _ocjena;
public:
	void setOcjena(int ocjena) {
		_ocjena = ocjena;
	}
	void SET(string datum_odbrane, vector<Nastavnik*>komisija) {
		_datumOdbrane = datum_odbrane;
		_komisija = komisija;
	}
	string getDatumPrijave() { return _datumPrijave; }
	bool operator==(ZavrsniRad obj) {
		if (strcmp(_nazivTeme, obj._nazivTeme) == 0 && *_oblastTeme == *obj._oblastTeme) {
			return true;
		}
		else return false;
	}
	ZavrsniRad & operator=(const ZavrsniRad& obj) {
		_oblastTeme = new Oblast(*obj._oblastTeme);
		_nazivTeme = new char[strlen(obj._nazivTeme) + 1];
		strcpy_s(_nazivTeme, strlen(obj._nazivTeme) + 1, obj._nazivTeme);
		_datumPrijave = obj._datumPrijave;
		_datumOdbrane = obj._datumOdbrane;
		_komisija = obj._komisija;
		_ocjena = obj._ocjena;
		return *this;
	}
	ZavrsniRad(const ZavrsniRad& obj) :_oblastTeme(new Oblast(*obj._oblastTeme)){
		_nazivTeme = new char[strlen(obj._nazivTeme) + 1];
		strcpy_s(_nazivTeme, strlen(obj._nazivTeme) + 1, obj._nazivTeme);
		_datumPrijave = obj._datumPrijave;
		_datumOdbrane = obj._datumOdbrane;
		_komisija = obj._komisija;
		_ocjena = obj._ocjena;

	}
	ZavrsniRad() :_nazivTeme(nullptr), _oblastTeme(nullptr), _datumPrijave("NEMA VRIJEDNOST"), _datumOdbrane("NEMA VRIJEDNOST"), _ocjena(5)
	{ }

	ZavrsniRad(string nazivTeme, Oblast oblastTeme, string datumPrijave) : _datumPrijave(datumPrijave), _oblastTeme(new Oblast(oblastTeme))
	{
		_nazivTeme = new char[nazivTeme.size() + 1];
		strcpy_s(_nazivTeme, nazivTeme.size() + 1, nazivTeme.c_str());
		_ocjena = 5;
	}

	~ZavrsniRad()
	{
		delete[] _nazivTeme; _nazivTeme = nullptr;
		delete _oblastTeme; _oblastTeme = nullptr;
		/*cout << "DEALOCIRAM KOMISIJU" << endl;
		for (int i = 0; i < _komisija.size(); i++) {
			delete _komisija[i];
			_komisija[i] = nullptr;
		}*/
	}

	char* GetNazivTeme()const { return _nazivTeme; }
	Oblast GetOblastTeme()const { return *_oblastTeme; }
	string GetDatumOdbrane() { return _datumOdbrane; }
	int GetOcjena() { return _ocjena; }
	vector<Nastavnik*>& GetKomisija() { return _komisija; };

	friend ostream& operator<<(ostream& COUT, ZavrsniRad& r)
	{
		COUT << "Tema rada: " << r._nazivTeme << endl;
		COUT << "Oblast teme: " << oblast_txt[*r._oblastTeme] << endl;
		COUT << "Datum prijave rada: " << r._datumPrijave << endl;
		//Podatke o nastavnicima nije moguce ispisati ovdje jer klasa jos nije definisana
		return COUT;
	}
};

class Nastavnik
{
	string _imePrezime;
	Oblast _oblastIzboraUZvanje;
	//Parametar string predstavlja broj indeksa studenta koji prijavljuje zavrsni rad kod odredjenog nastavnika
	Kolekcija<string, ZavrsniRad, 10> _teme;

public:
	friend ostream& operator<<(ostream& out, Nastavnik& obj) {
		out << obj._imePrezime << " " << oblast_txt[obj._oblastIzboraUZvanje] << endl;
		for (int i = 0; i < obj._teme.getTrenutno(); i++) {
			out << obj._teme.getElement1(i) << endl;
			out << obj._teme.getElement2(i) << endl;
			for (int j = 0; j < obj._teme.getElement2(i).GetKomisija().size(); j++) {
				out << obj._teme.getElement2(i).GetKomisija()[j]->GetImePrezime() << endl;
			}
			out << "--------------------------------------------" << endl;
			
		}
		
		return out;
	}
	/*Studentu sa brojem indeksa IB130011 dodjeljuje ocjenu 8 na zavrsnom radu. 
	Uslov za dodjelu ocjene je da student posjeduje definisan datum odbrane i listu clanova komisije. 
	U zavisnosti od uspjesnosti izvrsenja, funkcija vraca true ili false*/
	bool operator()(string indeks, int ocjena) {
		for (int i = 0; i < _teme.getTrenutno(); i++) {
			if (_teme.getElement1(i) == indeks) {

				if (_teme.getElement2(i).GetDatumOdbrane().empty()) {
					throw exception("Nije definisan datum odbrane!");
				}
				if (_teme.getElement2(i).GetKomisija().empty()) {
					throw exception("Nije definisana lista clanova komisije!");
				}
				_teme.getElement2(i).setOcjena(ocjena);
				return true;

			}
		}
		return false;
	}
	/*Funkcija ZakaziOdbranuRada ima zadatak da studentu sa proslijedjenim brojem indeksa zakaze odbranu zavrsnog rada sto 
	podrazumijeva definisanje datuma odbrane i liste clanova komisije pred kojima ce student braniti zavrsni rad. Odbrana rada se 
	moze zakazati samo studentu koji je rad prethodno prijavio. Komisiju trebaju ciniti najmanje 2 nastavnika koji imaju izbor u 
	zvanje u oblasti kojoj pripada tema rada. Datum odbrane ne smije biti manji od datuma prijave. U slucaju da bilo koji od navedenih
	uslova nije ispunjen funkcija treba da baci izuzetak*/
	void ZakaziOdbranuRada(string indeks, string datum_odbrane, vector<Nastavnik*>komisija) {
		bool postoji_prijava = false;
		for (int i = 0; i < _teme.getTrenutno(); i++) {
			if (indeks == _teme.getElement1(i)) {
				postoji_prijava = true;
				if (datum_odbrane < _teme.getElement2(i).getDatumPrijave()) {
					throw exception("Datum odbrane manji od datuma prijave!");
				}
			}
			
		}
		if (!postoji_prijava) {
			throw exception("Student nije prijavio rad!");
		}
		int br_cl_komisije = 0;
		for (int i = 0; i < komisija.size(); i++) {
			if (komisija[i]->GetOblast() == _oblastIzboraUZvanje) {
				br_cl_komisije++;
			}
		}
		if (br_cl_komisije < 2) {
			throw exception("Nedovoljan broj clanova komisije");
		}
		for (int i = 0; i < _teme.getTrenutno(); i++) {
			if (_teme.getElement1(i) == indeks) {
				_teme.getElement2(i).SET(datum_odbrane, komisija);
				cout << indeks << " uspjesno prijavio rad!" << endl;
			}
		}
	}

	/*Funkcija DodajZavrsniRad ima zadatak da odredjenom nastavniku dodijeli mentorstvo na zavrsnom radu. 
	Sprijeciti dodavanje zavrsnih radova sa istom temom vise puta. Nastavnik moze imati (mentorisati) samo radove iz oblasti za 
	koju posjeduje izbor u zvanje.U slucaju da se nastavniku pokusa dodati rad koji nije iz njegove oblasti funkcija treba da 
	baci izuzetak sa odgovarajucom porukom */

	//indeks, naslov, oblast, datum prijave 
	void DodajZavrsniRad(string indeks, string naslov, Oblast oblast, string datum_prijave) {

		if (_oblastIzboraUZvanje != oblast) {
			throw exception("Nastavnik nije mentor u toj oblasti!");
		}
		for (int i = 0; i < _teme.getTrenutno(); i++) {
			if (naslov == _teme.getElement2(i).GetNazivTeme()) {
				throw exception("Tema vec postoji!");
			}
		}
		_teme.AddElement(indeks, ZavrsniRad(naslov, oblast, datum_prijave));

	}

	Nastavnik & operator=(const Nastavnik& obj) {
		_imePrezime = obj._imePrezime;
		_oblastIzboraUZvanje = obj._oblastIzboraUZvanje;
		_teme = obj._teme;
		return *this;
	}
	Nastavnik(const Nastavnik& obj) :_imePrezime(obj._imePrezime), _oblastIzboraUZvanje(obj._oblastIzboraUZvanje){
		_teme = obj._teme;
	}
	Nastavnik() {

	}
	Nastavnik(string imePrezime, Oblast oblastIzboraUZvanje) : _imePrezime(imePrezime), _oblastIzboraUZvanje(oblastIzboraUZvanje)
	{}

	string GetImePrezime() { return _imePrezime; }
	Oblast GetOblast() { return _oblastIzboraUZvanje; }
	Kolekcija<string, ZavrsniRad, 10>& GetTeme() { return _teme; };
};
mutex m;
void email1(string indeks) {
	m.lock();
	cout << "mail adresa: " << indeks << "@edu.fit.ba" << endl;
	cout << "Uspjesno ste odbranili rad sa ocjenom vecom od prosjeka" << endl;

	m.unlock();
}
void email2(string indeks) {
	m.lock();
	cout << "mail adresa: " << indeks << "@edu.fit.ba" << endl;
	cout << "Uspjesno ste odbranili rad sa ocjenom manjom od prosjeka" << endl;

	m.unlock();
}
/*Funkcija PronadjiNajStudenta ima zadatak da pronadje broj indeksa studenta koji je na zavrsnom radu ostvario najvecu
ocjenu kod nastavnika koji posjeduje najvecu prosjecnu ocjenu na zavrsnim radovima. Ukoliko se kod nastavnika sa najvecom
prosjecnom ocjenom pojavi vise studenata sa istom ocjenom, onda funkcija vraca broj indeksa prvog pronadjenog studenta.
Svim studentima koji su odbranili rad kod nastavnika sa najvecom prosjecno ocjenom, u zasebnom thread.u, poslati email poruku
(mail adresa: brojIndeksa@edu.fit.ba) sa sadrzajem da su svoj zavrsni rad uspjesno odbranili sa vecom ili manjom ocjenom od
prosjecne. Ukoliko niti jedan od nastavnika ne posjeduje evidentirano mentorstvo na zavrsnom radu, funkcija vraca tekst:
NIJE PRONADJEN*/
string PronadjiNajStudenta(Nastavnik* niz[], int vel) {
	int lokacija_nastavnika = -1;
	float max_prosjek = 0.0;
	float suma_ocjena = 0.0;
	int br_studenata = 0;
	int max_ocjena = 0;
	string naj_indeks = "";
	for (int i = 0; i < vel; i++) {
		for (int j = 0; j < niz[i]->GetTeme().getTrenutno(); j++) {
			if (niz[i]->GetTeme().getElement2(j).GetOcjena() > 5) {
				suma_ocjena += niz[i]->GetTeme().getElement2(j).GetOcjena();
				br_studenata++;
			}
		
		}
		if (suma_ocjena / br_studenata >= max_prosjek) {
			max_prosjek = suma_ocjena / br_studenata;
			lokacija_nastavnika = i;
		}
		suma_ocjena = 0;
		br_studenata = 0;
	}

	if (lokacija_nastavnika == -1) {
		return "NIJE PRONADJEN";
	}
	else {
	
		for (int j = 0; j < niz[lokacija_nastavnika]->GetTeme().getTrenutno(); j++) {
			if (niz[lokacija_nastavnika]->GetTeme().getElement2(j).GetOcjena() > 5) {
				if (niz[lokacija_nastavnika]->GetTeme().getElement2(j).GetOcjena() > max_ocjena) {
					max_ocjena = niz[lokacija_nastavnika]->GetTeme().getElement2(j).GetOcjena();
					naj_indeks = niz[lokacija_nastavnika]->GetTeme().getElement1(j);
				}
			}
			
			
		}
		for (int k = 0; k < niz[lokacija_nastavnika]->GetTeme().getTrenutno(); k++) {
			if (niz[lokacija_nastavnika]->GetTeme().getElement2(k).GetOcjena() > 5) {
				if (niz[lokacija_nastavnika]->GetTeme().getElement2(k).GetOcjena() > max_ocjena) {
					thread t1(email1, niz[lokacija_nastavnika]->GetTeme().getElement1(k));
					t1.join();
				}
				else {
					thread t2(email2, niz[lokacija_nastavnika]->GetTeme().getElement1(k));
					t2.join();
				}
			}
			
		}

		return naj_indeks;
	}

}
void main()
{
	const int max = 4;
	Nastavnik* nastavnici[max];

	nastavnici[0] = new Nastavnik("Denis Music", SoftverskiInzinjering);
	nastavnici[1] = new Nastavnik("Zanin Vejzovic", KomunikacijskiSistemi);
	nastavnici[2] = new Nastavnik("Jasmin Azemovic", SigurnostInformacijskihSistema);
	nastavnici[3] = new Nastavnik("Emina Junuz", SoftverskiInzinjering);

	try
	{
		/*Funkcija DodajZavrsniRad ima zadatak da odredjenom nastavniku dodijeli mentorstvo na zavrsnom radu. Sprijeciti dodavanje zavrsnih radova sa istom temom vise puta. Nastavnik moze imati (mentorisati) samo radove iz oblasti za koju posjeduje izbor u zvanje.U slucaju da se nastavniku pokusa dodati rad koji nije iz njegove oblasti funkcija treba da baci izuzetak sa odgovarajucom porukom */

		//indeks, naslov, oblast, datum prijave 
		nastavnici[0]->DodajZavrsniRad("IB130011", "Multimedijalni informacijski sistem za visoko-obrazovnu ustanovu", SoftverskiInzinjering, "01.04.2017");
		nastavnici[0]->DodajZavrsniRad("IB120051", "Sistem za podršku rada kablovskog operatera", SoftverskiInzinjering, "03.03.2017");

		nastavnici[1]->DodajZavrsniRad("IB140102", "Praktièna analiza sigurnosti bežiènih raèunarskih mreža", KomunikacijskiSistemi, "22.08.2017");

		nastavnici[2]->DodajZavrsniRad("IB140002", "Primjena teorije informacija u procesu generisanja kriptografskih kljuèeva", SigurnostInformacijskihSistema, "10.09.2017");
	}
	catch (exception& ex)
	{
		cout << "GRESKA -> " << ex.what() << endl;
	}
	try{

		vector<Nastavnik*> komisija;//formira se komisija
		komisija.push_back(nastavnici[0]);
		komisija.push_back(nastavnici[2]);
		komisija.push_back(nastavnici[3]);

		/*Funkcija ZakaziOdbranuRada ima zadatak da studentu sa proslijedjenim brojem indeksa zakaze odbranu zavrsnog rada sto podrazumijeva definisanje datuma odbrane i liste clanova komisije pred kojima ce student braniti zavrsni rad. Odbrana rada se moze zakazati samo studentu koji je rad prethodno prijavio. Komisiju trebaju ciniti najmanje 2 nastavnika koji imaju izbor u zvanje u oblasti kojoj pripada tema rada. Datum odbrane ne smije biti manji od datuma prijave. U slucaju da bilo koji od navedenih uslova nije ispunjen funkcija treba da baci izuzetak*/
		nastavnici[0]->ZakaziOdbranuRada("IB130011", "25.09.2017", komisija);
		/*nastavnici[0]->ZakaziOdbranuRada("IB130111", "25.09.2017", komisija);*///student sa brojem indeksa IB130111 jos uvijek nije prijavio rad

/*Studentu sa brojem indeksa IB130011 dodjeljuje ocjenu 8 na zavrsnom radu. Uslov za dodjelu ocjene je da student posjeduje definisan datum odbrane i listu clanova komisije. U zavisnosti od uspjesnosti izvrsenja, funkcija vraca true ili false*/

		if ((*nastavnici[0])("IB130011", 8))
			cout << "Uspjesno ste ocijenili zavrsni rad!" << endl;


		/*Ispisuje sve podatke o nastavniku i njegovim mentorstvima. Za clanove komisije je dovoljno ispisati samo ime i prezime.*/
		cout << *nastavnici[0] << endl;
	
		/*Funkcija PronadjiNajStudenta ima zadatak da pronadje broj indeksa studenta koji je na zavrsnom radu ostvario najvecu 
		ocjenu kod nastavnika koji posjeduje najvecu prosjecnu ocjenu na zavrsnim radovima. Ukoliko se kod nastavnika sa najvecom 
		prosjecnom ocjenom pojavi vise studenata sa istom ocjenom, onda funkcija vraca broj indeksa prvog pronadjenog studenta. 
		Svim studentima koji su odbranili rad kod nastavnika sa najvecom prosjecno ocjenom, u zasebnom thread.u, poslati email poruku
		(mail adresa: brojIndeksa@edu.fit.ba) sa sadrzajem da su svoj zavrsni rad uspjesno odbranili sa vecom ili manjom ocjenom od 
		prosjecne. Ukoliko niti jedan od nastavnika ne posjeduje evidentirano mentorstvo na zavrsnom radu, funkcija vraca tekst: 
		NIJE PRONADJEN*/

		cout << "Najsupjesniji student: " << PronadjiNajStudenta(nastavnici, max) << endl;

		//Baca izuzetak zbog neadekvatnog izbora u zvanje, odnosno oblasti
		nastavnici[2]->DodajZavrsniRad("IB150008", "Razvoj sistema autentifikacije na osnovu biometrije glasa", InteligentniSistemi, "15.05.2017");

	}
	catch (exception& ex)
	{
		cout << "GRESKA -> " << ex.what() << endl;
	}
	for (int i = 0; i < max; i++)
	{
		delete nastavnici[i];
		nastavnici[i] = nullptr;
	}
	system("PAUSE");
}
