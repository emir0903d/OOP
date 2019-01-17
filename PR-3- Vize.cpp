#include<iostream>
#include<vector>
#include<string>
#include<thread>
#include<mutex>
#include<regex>
using namespace std;

const char *crt = "\n-----------------------------------------------\n";

enum Status { ZAHTJEV_ZAPRIMLJEN, ODUSTAO, ODBIJENA, IZDATA, ISTEKLA, PONISTENA };
ostream& operator<<(ostream& out, Status& obj) {
	if (obj == ZAHTJEV_ZAPRIMLJEN) {
		out << "ZAHTJEV_ZAPRIMLJEN";
	}
	if (obj == ODUSTAO) {
		out << "ODUSTAO";
	}
	if (obj == ODBIJENA) {
		out << "ODBIJENA";
	}
	if (obj == IZDATA) {
		out << "IZDATA";
	}
	if (obj == ISTEKLA) {
		out << "ISTEKLA";
	}
	if (obj == PONISTENA) {
		out << "PONISTENA";
	}
	return out;
}
enum Sortiranje { RASTUCI, OPADAJUCI };
enum Drzava { SAD, AUSTRALIJA, ENGLESKA };
ostream& operator<<(ostream& out, Drzava& obj) {
	if (obj == SAD) {
		out << "SAD";
	}
	if (obj == AUSTRALIJA) {
		out << "AUSTRALIJA";
	}
	if (obj == ENGLESKA) {
		out << "ENGLESKA";
	}
	return out;
}
class Datum {
	int *_dan, *_mjesec, *_godina;
public:
	bool operator>(Datum d) {
		long long int a = *_dan + *_mjesec * 30 + *_godina * 365;
		long long int b = *d._dan + *d._mjesec * 30 + *d._godina * 365;
		if (a > b) {
			return true;
		}
		else return false;
	}
	bool Under10Years(int god) {
		if ((abs(god - *_godina)) < 10) {
			return true;
		}
		return false;
	}
	bool operator==(Datum obj) {
		if (*_dan == *obj._dan && *_mjesec == *obj._mjesec && *_godina == *obj._godina) {
			return true;
		}
		else return false;
	}
	Datum & operator++() {
		(*_dan)++;
		return*this;
	}
	Datum(const Datum& obj) {
		_dan = new int(*obj._dan);
		_mjesec = new int(*obj._mjesec);
		_godina = new int(*obj._godina);
	}
	Datum& operator=(Datum& obj) {
		_dan = new int(*obj._dan);
		_mjesec = new int(*obj._mjesec);
		_godina = new int(*obj._godina);
		return *this;
	}
	Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
	}
	~Datum() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}
	friend ostream& operator<< (ostream &COUT, Datum &obj) {
		COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina;
		return COUT;
	}
};

template<class T1, class T2, int max>
class FITKolekcija {
	T1 * _elementi1[max];
	T2 * _elementi2[max];
	int _trenutno;
public:
	void Sortiraj(Sortiranje a) {

		if (a == Sortiranje::RASTUCI) {
			for (int i = 1; i < _trenutno; i++) {
				int j = i;
				while (j > 0 && *_elementi1[j] < *_elementi1[j - 1]) {

					T1 temp1 = *_elementi1[j];
					T2 temp2 = *_elementi2[j];
					delete _elementi1[j];
					delete _elementi2[j];
					_elementi1[j] = new T1(*_elementi1[j - 1]);
					_elementi2[j] = new T2(*_elementi2[j - 1]);
					delete _elementi1[j - 1];
					delete _elementi2[j - 1];
					_elementi1[j - 1] = new T1(temp1);
					_elementi2[j - 1] = new T2(temp2);

					j--;
				}
			}
		}
		if (a == Sortiranje::OPADAJUCI) {
			for (int i = 1; i < _trenutno; i++) {
				int j = i;
				while (j > 0 && *_elementi1[j] > *_elementi1[j - 1]) {

					T1 temp1 = *_elementi1[j];
					T2 temp2 = *_elementi2[j];
					delete _elementi1[j];
					delete _elementi2[j];
					_elementi1[j] = new T1(*_elementi1[j - 1]);
					_elementi2[j] = new T2(*_elementi2[j - 1]);
					delete _elementi1[j - 1];
					delete _elementi2[j - 1];
					_elementi1[j - 1] = new T1(temp1);
					_elementi2[j - 1] = new T2(temp2);

					j--;
				}
			}
		}

		
	}
	void Dodaj(T1 e1, T2 e2) {
		if (_trenutno == max) {
			throw exception("Dostigli ste limit kolekcije!");
		}
		_elementi1[_trenutno] = new T1(e1);
		_elementi2[_trenutno] = new T2(e2);
		_trenutno++;
	}
	FITKolekcija(const FITKolekcija& obj) {
		for (int i = 0; i < obj._trenutno; i++) {
			_elementi1[i] = new T1(*obj._elementi1[i]);
			_elementi2[i] = new T2(*obj._elementi2[i]);
		}
		for (int i = obj._trenutno; i < max; i++) {
			_elementi1[i] = nullptr;
			_elementi2[i] = nullptr;
		}
		_trenutno = obj._trenutno;
	}
	FITKolekcija& operator=(FITKolekcija& obj) {
		for (int i = 0; i < obj._trenutno; i++) {
			_elementi1[i] = new T1(*obj._elementi1[i]);
			_elementi2[i] = new T2(*obj._elementi2[i]);
		}
		for (int i = obj._trenutno; i < max; i++) {
			_elementi1[i] = nullptr;
			_elementi2[i] = nullptr;
		}
		_trenutno = obj._trenutno;
		return *this;
	}
	FITKolekcija() {
		for (size_t i = 0; i < max; i++) {
			_elementi1[i] = nullptr;
			_elementi2[i] = nullptr;
		}
		_trenutno = 0;
	}
	~FITKolekcija() {
		for (size_t i = 0; i < max; i++) {//???
			delete _elementi1[i]; _elementi1[i] = nullptr;
			delete _elementi2[i]; _elementi2[i] = nullptr;
		}
	}
	T1 ** GetT1() { return _elementi1; }
	T2 ** GetT2() { return _elementi2; }
	int GetTrenutno() { return _trenutno; }

	friend ostream& operator<< (ostream &COUT, FITKolekcija &obj) {
		for (size_t i = 0; i < obj._trenutno; i++)
			COUT << *obj._elementi1[i] << " " << *obj._elementi2[i] << endl;
		return COUT;
	}
};


class Viza {
protected:
	Drzava _drzava;
	Datum _vazenjeOD;
	Datum _vazenjeDO;
	char * _napomena;
	// string se odnosi na razlog promjene statusa, a moguæe je samo 10 izmjena statusa
	FITKolekcija<Status, string, 10> _statusi;
public:
	void setNapomena(string s) {
		if (_napomena != nullptr) {
			delete[]_napomena;
		}
		_napomena = new char[s.size() + 1];
		strcpy_s(_napomena, s.size() + 1, s.c_str());
	}
	bool operator==(Viza& v) {
		if (_drzava == v._drzava) {
			return true;
		}
		else return false;
	}
	Drzava getDrzava() { return _drzava; }
	Viza(const Viza& obj):_drzava(obj._drzava), _vazenjeOD(obj._vazenjeOD), _vazenjeDO(obj._vazenjeDO) {

		if (obj._napomena != nullptr) {
			_napomena = new char[strlen(obj._napomena) + 1];
			strcpy_s(_napomena, strlen(obj._napomena) + 1, obj._napomena);
		}
		else
			_napomena = nullptr;

	}
	Viza& operator=(Viza& obj) {
		
		_drzava = obj._drzava;
		_vazenjeOD = obj._vazenjeOD;
		_vazenjeDO = obj._vazenjeDO;
		if (obj._napomena != nullptr) {
			_napomena = new char[strlen(obj._napomena) + 1];
			strcpy_s(_napomena, strlen(obj._napomena) + 1, obj._napomena);
		}
		else
			_napomena = nullptr;
			return *this;
	}
	Viza(Drzava drzava, Datum _OD, Datum _DO, const char * napomena = nullptr) :_vazenjeDO(_DO), _vazenjeOD(_OD)
	{
		if (napomena != nullptr) {
			_napomena = new char[strlen(napomena) + 1];
			strcpy_s(_napomena, strlen(napomena) + 1, napomena);
		}
		else
			_napomena = nullptr;
		_drzava = drzava;
	}
	virtual ~Viza() { delete[] _napomena; _napomena = nullptr; }
	Datum GetVazenjeOD() { return _vazenjeOD; }
	Datum GetVazenjeDO() { return _vazenjeDO; }
	char * GetNapomena() { return _napomena; }
	FITKolekcija<Status, string, 10> & GetStatuse() { return _statusi; }
	virtual void Info() = 0;
};

class PoslovnaViza : public Viza {
	string _kompanija; //naziv kompanije koja potvrðuje da posluje sa aplikantom
public:

	void Info() {
		cout << _drzava << endl;
		cout << _vazenjeOD << " - " << _vazenjeDO << endl;
		if(_napomena!=nullptr)
		cout << _napomena << endl;
		cout << _kompanija << endl;
	}
	PoslovnaViza(const PoslovnaViza& obj) : Viza(obj) {
		_kompanija = obj._kompanija;
	}
	PoslovnaViza& operator=(PoslovnaViza& obj) {
		_drzava = obj._drzava;
		_vazenjeOD = obj._vazenjeOD;
		_vazenjeDO = obj._vazenjeDO;
		if (obj._napomena != nullptr) {
			_napomena = new char[strlen(obj._napomena) + 1];
			strcpy_s(_napomena, strlen(obj._napomena) + 1, obj._napomena);
		}
		else _napomena = nullptr;
		_kompanija = obj._kompanija;
		return *this;
	}
	PoslovnaViza(Drzava drzava, Datum _OD, Datum _DO, string kompanija, char * napomena = nullptr) : Viza(drzava, _OD, _DO, napomena) {
		_kompanija = kompanija;
	}
	friend ostream & operator << (ostream & COUT, PoslovnaViza & obj) {
		COUT << obj._drzava << " " << obj._vazenjeOD << " " << obj._vazenjeDO << " " << obj._kompanija << endl;
		COUT << ((obj._napomena != nullptr) ? obj._napomena : "") << endl;
		COUT << "STATUSI -> " << obj._statusi << endl;
		return COUT;

	}

};
class TuristickaViza : public Viza {
	string _adresa; //adresa hotele ili drugog smještaja na kojoj æe aplikant boraviti
public:
	void Info() {
		cout << _drzava << endl;
		cout << _vazenjeOD << " - " << _vazenjeDO << endl;
		cout << _napomena << endl;
		cout << _adresa << endl;
	}
	TuristickaViza(const TuristickaViza& obj) : Viza(obj) {
		_adresa = obj._adresa;
	}
	TuristickaViza& operator=(TuristickaViza& obj) {
		_drzava = obj._drzava;
		_vazenjeOD = obj._vazenjeOD;
		_vazenjeDO = obj._vazenjeDO;
		if (obj._napomena != nullptr) {
			_napomena = new char[strlen(obj._napomena) + 1];
			strcpy_s(_napomena, strlen(obj._napomena) + 1, obj._napomena);
		}
		else _napomena = nullptr;
		_adresa = obj._adresa;
		return *this;
	}
	TuristickaViza(Drzava drzava, Datum _OD, Datum _DO, string adresa, const char * napomena = nullptr) : Viza(drzava, _OD, _DO, napomena) {
		_adresa = adresa;
	}
	friend ostream & operator << (ostream & COUT, TuristickaViza & obj) {
		COUT << obj._drzava << " " << obj._vazenjeOD << " " << obj._vazenjeDO << " " << obj._adresa << endl;
		COUT << ((obj._napomena != nullptr) ? obj._napomena : "") << endl;
		COUT << "STATUSI ---> " << obj._statusi << endl;
		return COUT;

	}
};
mutex m;
void Print(Viza *v) {
	m.lock();
	TuristickaViza *pokt = dynamic_cast<TuristickaViza*>(v);
	PoslovnaViza *pokp = dynamic_cast<PoslovnaViza*>(v);
	if (pokt != nullptr) {
		cout << *pokt << endl;
	}
	else {
		cout << *pokp << endl;
	}
	m.unlock();
}
class Aplikant {
	char * _imePrezime;
	Datum * _datumRodjenja;
	char * _brojPasosa;
	vector<Viza *> _vize;
public:
	void ZamijeniRijec(string s1, string s2) {
		regex r("(\\b" + s1 + "\\b)|(\\B" + s1 + "\\B)");

		for (int i = 0; i < _vize.size(); i++) {
			
			if (_vize[i]->GetNapomena() != nullptr) {
				string zamijeni(_vize[i]->GetNapomena());
				string zamijeni_rijec = regex_replace(zamijeni, r, s2);
				_vize[i]->setNapomena(zamijeni_rijec);
			}
			
		}
	}
	//VRAÆA PODATKE O SVIM VIZAMA KOJE SU IZDATE ZA ODREÐENU DRŽAVU, A KOJE SU IMALE ODREÐENI STATUS
	vector<Viza*> GetVizeByDrzava(Drzava drzava, Status status) {
		vector<Viza*> temp;
		bool found = false;
		for (int i = 0; i < _vize.size(); i++) {
			if (_vize[i]->getDrzava() == drzava) {
				for (int j = 0; j < _vize[i]->GetStatuse().GetTrenutno(); j++) {
					if (*_vize[i]->GetStatuse().GetT1()[j] == status) {
						found = true;
					
					}
				}
				if (found == true) {
					TuristickaViza* t = dynamic_cast<TuristickaViza*>(_vize[i]);
					PoslovnaViza* p = dynamic_cast<PoslovnaViza*>(_vize[i]);

					if (t != nullptr) {
						temp.push_back(new TuristickaViza(*t));
					}
					if (p != nullptr) {
						temp.push_back(new PoslovnaViza(*p));
					}
				}
				found = false;
				
			}
		}
		return temp;
	}

	bool PrintajVizu(Viza *v) {
		for (int i = 0; i < _vize.size(); i++) {
			if (*_vize[i] == *v) {
				this_thread::sleep_for(chrono::seconds(1));
				thread t(Print, _vize[i]);
				
				t.join();
				return true;

			}
		}
		return false;
	}
	/*
	STATUSI SE MOGU DODAVATI SAMO U REDOSLIJEDU U KOJEM SE NALAZE U ENUMERACIJI NPR.
	- NIJE MOGUÆE ODBITI VIZU AKO PRETHODNO NIJE IMALA STATUS DA JE ZAHTJEV_ZAPRIMLJEN;
	- NIJE MOGUÆE PONIŠTITI VIZU KOJA PRETHODNO NIJE IZDATA
	*/
	bool DodajStatus(Viza* viza, Status stat, string napomena=""){
		bool add = false;
		for (int i = 0; i < _vize.size(); i++) {
			//PoslovnaViza * pokp = dynamic_cast<PoslovnaViza*>(viza);
			//TuristickaViza* pokt = dynamic_cast<TuristickaViza*>(viza);

			//PoslovnaViza *pokp_v = dynamic_cast<PoslovnaViza*>(_vize[i]);
			//TuristickaViza*pokt_v = dynamic_cast<TuristickaViza*>(_vize[i]);

			if (*_vize[i] == *viza) {

				if (stat == ZAHTJEV_ZAPRIMLJEN) {
					if (_vize[i]->GetStatuse().GetTrenutno() == 0) {
						add = true;
					}
				}
				if (stat == Status::ODUSTAO) {
					if (*_vize[i]->GetStatuse().GetT1()[_vize[i]->GetStatuse().GetTrenutno() - 1] == Status::ZAHTJEV_ZAPRIMLJEN) {
						add = true;
					}
				}
				if (stat == Status::ODBIJENA) {
					if (*_vize[i]->GetStatuse().GetT1()[_vize[i]->GetStatuse().GetTrenutno() - 1] == Status::ZAHTJEV_ZAPRIMLJEN) {
						add = true;
					}
				}
				if (stat == Status::IZDATA) {
					if (*_vize[i]->GetStatuse().GetT1()[_vize[i]->GetStatuse().GetTrenutno() - 1] == Status::ZAHTJEV_ZAPRIMLJEN) {
						add = true;
					}
				}
				if (stat == Status::ISTEKLA) {
					if (*_vize[i]->GetStatuse().GetT1()[_vize[i]->GetStatuse().GetTrenutno() - 1] == Status::IZDATA) {
						add = true;
					}
				}
				if (stat == Status::PONISTENA) {
					if (*_vize[i]->GetStatuse().GetT1()[_vize[i]->GetStatuse().GetTrenutno() - 1] == Status::IZDATA) {
						add = true;
					}
				}


				if (add == true) {
					_vize[i]->GetStatuse().Dodaj(stat, napomena);
					return true;
				}
				else return false;
			}
		}
		return false;
}

	/*
	NOVA VIZA SE NE MOŽE DODATI UKOLIKO:
	- JE APLIKANT MLAÐI OD 10 GODINA
	- U TOM PERIODU (ODNOSI SE NA PERIOD VIZE OD - DO) NE POSTOJI VAŽEÆA (POJAM VAŽEÆA PODRAZUMIJEVA DA JE POSLJEDNJI STATUS VIZE BIO IZDATA TJ. DA VIZA NIJE PONIŠTENA ILI ISTEKLA) VIZA ZA TU DRŽAVU, BEZ OBZIRA NA NJENU VRSTU.
	- JE NEKA OD RANIJIH VIZA PONIŠTENA (OVO SE ODNOSI SAMO NA VIZE ZA DRŽAVU: SAD)
	*/
	bool DodajVizu(Viza* v) {
	
		if (_datumRodjenja->Under10Years(2018)) {
			return false;
		}

		for (int i = 0; i < _vize.size(); i++) {
			if (_vize[i]->GetVazenjeDO() > v->GetVazenjeOD()) {
				if (*_vize[i]->GetStatuse().GetT1()[_vize[i]->GetStatuse().GetTrenutno()-1] == IZDATA) {
					return false;
				}
			}
			if (v->getDrzava() == SAD) {
				if (_vize[i]->getDrzava() == SAD) {
					for (int j = 0; j < _vize[i]->GetStatuse().GetTrenutno(); j++) {
						if (*_vize[i]->GetStatuse().GetT1()[j] == PONISTENA) {
							return false;
						}
					}
				}
			}
		
		}

		PoslovnaViza *pok_p = dynamic_cast<PoslovnaViza*>(v);
		TuristickaViza *pok_t = dynamic_cast<TuristickaViza*>(v);


		if (pok_p != nullptr) {
			_vize.push_back(new PoslovnaViza(*pok_p));
			return true;
		}
		if (pok_t != nullptr) {
			_vize.push_back(new TuristickaViza(*pok_t));
			return true;
		}

		return false;
	}
	Aplikant & operator=(Aplikant& obj) {
		int vel = strlen(obj._imePrezime) + 1;
		_imePrezime = new char[vel];
		strcpy_s(_imePrezime, vel, obj._imePrezime);
		vel = strlen(obj._brojPasosa) + 1;
		_brojPasosa = new char[vel];
		strcpy_s(_brojPasosa, vel, obj._brojPasosa);
		_datumRodjenja = new Datum(*obj._datumRodjenja);
		_vize = obj._vize;
		return *this;
	}
	Aplikant(const Aplikant& obj) {
		int vel = strlen(obj._imePrezime) + 1;
		_imePrezime = new char[vel];
		strcpy_s(_imePrezime, vel, obj._imePrezime);
		vel = strlen(obj._brojPasosa) + 1;
		_brojPasosa = new char[vel];
		strcpy_s(_brojPasosa, vel, obj._brojPasosa);
		_datumRodjenja = new Datum(*obj._datumRodjenja);
		_vize = obj._vize;
	}
	Aplikant(const char * imePrezime, Datum datumRodjenja, const char * brPasosa) {
		int vel = strlen(imePrezime) + 1;
		_imePrezime = new char[vel];
		strcpy_s(_imePrezime, vel, imePrezime);
		vel = strlen(brPasosa) + 1;
		_brojPasosa = new char[vel];
		strcpy_s(_brojPasosa, vel, brPasosa);
		_datumRodjenja = new Datum(datumRodjenja);
	}
	~Aplikant() {
		delete[]_imePrezime; _imePrezime = nullptr;
		delete[]_brojPasosa; _brojPasosa = nullptr;
		delete _datumRodjenja; _datumRodjenja = nullptr;
		for (size_t i = 0; i < _vize.size(); i++)
		{
			delete _vize[i]; _vize[i] = nullptr;
		}
	}
	friend ostream & operator << (ostream & COUT, Aplikant & obj) {
		COUT << obj._imePrezime << " " << *obj._datumRodjenja << " " << obj._brojPasosa << endl;
		COUT << "VIZE -> " << endl;
		for (int i = 0; i < obj._vize.size(); i++) {
			PoslovnaViza *pokp = dynamic_cast<PoslovnaViza*>(obj._vize[i]);
			TuristickaViza *pokt = dynamic_cast<TuristickaViza*>(obj._vize[i]);

			if (pokp != nullptr) {
				COUT << *pokp << endl;
			}
			if (pokt != nullptr) {
				COUT << *pokt << endl;
			}

		}
			
			
		
		return COUT;
	}

	vector<Viza * > GetVize() { return _vize; }
};

void main() {

	/****************************************************************************
	1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
	2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ÆE BITI OZNACENO KAO "RE"
	3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEÈILO GUBLJENJE URAÐENOG ZADATKA
	4. PROGRAMSKI CODE SE TAKOÐER NALAZI U FAJLU CODE.TXT
	5. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIÈNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUÈAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOÆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
	6. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAÈENO.
	****************************************************************************/



	Datum danas(15, 2, 2017);
	Datum sutra(++danas); // inkrementiranjem se uvecava dan za 1
	Datum prekosutra;
	Datum dat15022017(15, 2, 2017), dat18082017(18, 8, 2017), dat11052018(11, 5, 2018), dat11122017(11, 12, 2017);


	prekosutra = danas;
	if (prekosutra == danas)
		cout << "Datumi su jednaki" << endl;
	cout << danas << endl << sutra << endl << prekosutra << crt;

	int v6 = 6, v13 = 13, v32 = 32, v63 = 63, v98 = 98, v109 = 109, v196 = 196;

	FITKolekcija<int, int, 7> brojevi;
	brojevi.Dodaj(v196, v6);
	brojevi.Dodaj(v13, v32);
	brojevi.Dodaj(v98, v196);
	brojevi.Dodaj(v63, v13);
	brojevi.Dodaj(v98, v196);
	brojevi.Dodaj(v196, v6);

	cout << brojevi << crt;
	//NA KOJI NAÈIN ÆE SE VRŠITI SORTIRANJE PRVENSTVENO ZAVISI OD VRIJEDNOSTI PARAMETRA, A VRŠI SE NA OSNOVU VRIJEDNOSTI TIPA T1
	brojevi.Sortiraj(RASTUCI);
	cout << brojevi << crt;
	brojevi.Sortiraj(OPADAJUCI);
	cout << brojevi << crt;

	try
	{
		//U SLUÈAJU PREKORAÈENJA DEFINISANE VELIÈINE KOLEKCIJE, POTREBNO JE BACITI IZUZETAK
		brojevi.Dodaj(v13, v196);
	}
	catch (exception & err)
	{
		cout << "Error -> " << err.what() << endl;
	}

	Aplikant jasmin("Jasmin Azemovic", Datum(15, 5, 1979), "BS6985645");
	PoslovnaViza poslovna(SAD, dat15022017, dat18082017, "Microsoft");

	/*
	NOVA VIZA SE NE MOŽE DODATI UKOLIKO:
	- JE APLIKANT MLAÐI OD 10 GODINA
	- U TOM PERIODU (ODNOSI SE NA PERIOD VIZE OD - DO) NE POSTOJI VAŽEÆA (POJAM VAŽEÆA PODRAZUMIJEVA DA JE POSLJEDNJI STATUS VIZE BIO IZDATA TJ. DA VIZA NIJE PONIŠTENA ILI ISTEKLA) VIZA ZA TU DRŽAVU, BEZ OBZIRA NA NJENU VRSTU.
	- JE NEKA OD RANIJIH VIZA PONIŠTENA (OVO SE ODNOSI SAMO NA VIZE ZA DRŽAVU: SAD)
	*/
	if (jasmin.DodajVizu(&poslovna))
		cout << "Podaci o vizi uspješno zaprimljeni" << endl;
	/*
	STATUSI SE MOGU DODAVATI SAMO U REDOSLIJEDU U KOJEM SE NALAZE U ENUMERACIJI NPR.
	- NIJE MOGUÆE ODBITI VIZU AKO PRETHODNO NIJE IMALA STATUS DA JE ZAHTJEV_ZAPRIMLJEN;
	- NIJE MOGUÆE PONIŠTITI VIZU KOJA PRETHODNO NIJE IZDATA
	*/
	if (jasmin.DodajStatus(&poslovna, ZAHTJEV_ZAPRIMLJEN, "provjeriti navode sa predstavnicima kompanije Microsoft"))
		cout << "Status vize uspijesno promijenjen" << endl;
	else
		cout << "Aplikant " << jasmin << " ne posjeduje evidentiranu trazenu vizu ili status vize nema odgovarajucu vrijednost" << endl;

	if (jasmin.DodajStatus(&poslovna, IZDATA, "privremeno")) {
		cout << "Izdata privremena viza!!!";
	}

	if (jasmin.DodajStatus(&poslovna, PONISTENA, "predstavnicima kompanije Microsoft nemaju informaciju o posjeti navedenog aplikanta"))
		cout << "Status vize uspijesno promijenjen" << endl;
	else
		cout << "Aplikant " << jasmin << " ne posjeduje evidentiranu trazenu vizu ili status vize nema odgovarajucu vrijednost" << endl;

	TuristickaViza turisticka(ENGLESKA, dat18082017, dat11122017, "Whitehall Pl, London SW1A 2BD, UK","bla ba bnla nemaju informaciju ba ba ab abab a");
	jasmin.DodajVizu(&turisticka);
	if (jasmin.DodajStatus(&turisticka, ZAHTJEV_ZAPRIMLJEN))
		cout << "Status vize uspijesno promijenjen" << endl;
	else
		cout << "Aplikant " << jasmin << " ne posjeduje evidentiranu trazenu vizu ili status vize nema odgovarajucu vrijednost" << endl;

	cout << jasmin << endl;
	if (jasmin.DodajStatus(&turisticka, IZDATA)) {
		cout << "Status vize uspijesno promijenjen!!!" << endl;
		//U ZASEBNOJ NITI URADITI PRINTANJE VIZE...KORISTITI METODU SLEEP_FOR
		jasmin.PrintajVizu(&turisticka);
	}
	else
		cout << "Aplikant " << jasmin << " ne posjeduje evidentiranu trazenu vizu ili status vize nema odgovarajucu vrijednost" << endl;
	//VRAÆA PODATKE O SVIM VIZAMA KOJE SU IZDATE ZA ODREÐENU DRŽAVU, A KOJE SU IMALE ODREÐENI STATUS
	vector<Viza *> jasminVize = jasmin.GetVizeByDrzava(SAD, PONISTENA);
	
	for (int i = 0; i < jasminVize.size(); i++)
		jasminVize[i]->Info();
	//UNUTAR NAPOMENE MIJENJA SADRŽAJ "nemaju informaciju" SA "su upoznti sa" - KORISTITI REGEX
	jasmin.ZamijeniRijec("nemaju informaciju", "su upoznti sa");
	cout << jasmin << endl;

	system("PAUSE");
}