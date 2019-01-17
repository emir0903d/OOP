#include<iostream>
#include<string>
#include<vector>
#include<exception>
#include<regex>
#include<mutex>
#include<thread>
using namespace std;
const char* crt = "-------------------------------------------";

class Datum {
	int *_dan, *_mjesec, *_godina;
public:
	long long int get3Dana(Datum d) {
		long long int d1 = *_dan + *_mjesec * 30 + *_godina * 365;
		long long int d2 = *d._dan + *d._mjesec * 30 + *d._godina * 365;
		return abs(d2 - d1);
	}
	int getDan() { return *_dan; }
	bool operator==(Datum d) {
		if (*_dan == *d._dan && *_mjesec == *d._mjesec && *_godina == *d._godina) {
			return true;
		}
		else return false;
	}
	Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
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

	~Datum() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}
	friend ostream &operator<<(ostream &COUT, const Datum &obj) {
		COUT << *obj._dan << " " << *obj._mjesec << " " << *obj._godina;
		return COUT;
	}

};
class Izuzetak : public exception {
	string _funkcija;
public:
	Izuzetak(string info, string funkc):exception(info.c_str()),_funkcija(funkc) {
		
	}
	friend ostream& operator<<(ostream& out, Izuzetak& i) {
		out<<i.what();
		out << endl;
		out << i._funkcija << endl;
		return out;
	}

};

template <class T1, class T2>
class Kolekcija {
	T1 * _elementi1;
	T2 * _elementi2;
	int _trenutnoElemenata;
	bool _omoguciDupliranjeElemenata;
public:
	void setT2(int indeks, T2 el) {
		_elementi2[indeks] =el;
	}
	/*
	AddElement :: omogucava dodavanje novog elementa u kolekciju. Ukoliko je moguce, osigurati automatsko prosiranje kolekcije prilikom dodavanja svakog novog elementa, te onemoguciti ponavljanje elemenata
	RemoveElement :: na osnovu parametra tipa T1 uklanja elemente iz kolekcije i ukoliko je moguce smanjuje velicinu niza/kolekcije. Prilikom uklanjanja elemenata ocuvati redoslijed njihovog dodavanja
	*/
	void RemoveElement(T1 e1) {
	
	int indeks = -1;
		for (int i = 0; i < _trenutnoElemenata; i++) {
			if (_elementi1[i] == e1) {
				indeks = i;
			}
		}
		if (indeks != -1) {
			for (int j = indeks; j < _trenutnoElemenata - 1; j++) {
				_elementi1[j] = _elementi1[j + 1];
				_elementi2[j] = _elementi2[j + 1];
			}
			_trenutnoElemenata--;
			T1 *_temp1 = new T1[_trenutnoElemenata];
			T2 *_temp2 = new T2[_trenutnoElemenata];
			for (int i = 0; i < _trenutnoElemenata; i++) {
				_temp1[i] = _elementi1[i];
				_temp2[i] = _elementi2[i];
			}
			delete[] _elementi1; _elementi1 = _temp1; _temp1 = nullptr;
			delete[] _elementi2; _elementi2 = _temp2; _temp2 = nullptr;
		}
		
	


	}
	bool AddElement(T1 e1, T2 e2) {
		if (_omoguciDupliranjeElemenata == true) {
			for (int i = 0; i < _trenutnoElemenata; i++) {
				if (_elementi1[i] == e1 || _elementi2[i] == e2) {
					throw Izuzetak("Element vec postoji u kolekciji", __FUNCTION__);
				}
			}
		}
		
		T1 *_temp1 = new T1[_trenutnoElemenata + 1];
		T2 *_temp2 = new T2[_trenutnoElemenata + 1];
		for (int i = 0; i < _trenutnoElemenata; i++) {
			_temp1[i] = _elementi1[i];
			_temp2[i] = _elementi2[i];
		}
		_temp1[_trenutnoElemenata] = e1;
		_temp2[_trenutnoElemenata] = e2;
		_trenutnoElemenata++;

		delete[] _elementi1; _elementi1 = _temp1; _temp1 = nullptr;
		delete[] _elementi2; _elementi2 = _temp2; _temp2 = nullptr;
		return true;

	}
	Kolekcija(bool omoguciDupliranjeElemenata = false) {
		_trenutnoElemenata = 0;
		_omoguciDupliranjeElemenata = omoguciDupliranjeElemenata;
		_elementi1 = nullptr;
		_elementi2 = nullptr;
	}
	Kolekcija(const Kolekcija& obj) {
		_elementi1 = new T1[obj._trenutnoElemenata];
		_elementi2 = new T2[obj._trenutnoElemenata];
		for (int i = 0; i < obj._trenutnoElemenata; i++) {
			_elementi1[i] = obj._elementi1[i];
			_elementi2[i] = obj._elementi2[i];
		}
		_trenutnoElemenata = obj._trenutnoElemenata;
		_omoguciDupliranjeElemenata = obj._omoguciDupliranjeElemenata;
	}
	Kolekcija& operator=(Kolekcija& obj) {
		_elementi1 = new T1[obj._trenutnoElemenata];
		_elementi2 = new T2[obj._trenutnoElemenata];
		for (int i = 0; i < obj._trenutnoElemenata; i++) {
			_elementi1[i] = obj._elementi1[i];
			_elementi2[i] = obj._elementi2[i];
		}
		_trenutnoElemenata = obj._trenutnoElemenata;
		_omoguciDupliranjeElemenata = obj._omoguciDupliranjeElemenata;
		return *this;
	}
	~Kolekcija() {
		delete[]_elementi2; _elementi2 = nullptr;
		delete[]_elementi1; _elementi1 = nullptr;
		_trenutnoElemenata = 0;
	}

	int GetTrenutno() const { return _trenutnoElemenata; }
	T1& GetElement1(int lokacija) const
	{
		if (lokacija < 0 || lokacija >= _trenutnoElemenata)
			throw Izuzetak("Nepostojeca lokacija", __FUNCTION__);
		return _elementi1[lokacija];
	}

	T2& GetElement2(int lokacija) const
	{
		if (lokacija < 0 || lokacija >= _trenutnoElemenata)
			throw Izuzetak("Nepostojeca lokacija", __FUNCTION__);
		return _elementi2[lokacija];
	}

	friend ostream &operator<<(ostream &COUT, const Kolekcija &obj) {
		for (size_t i = 0; i < obj.GetTrenutno(); i++)
			COUT << obj._elementi1[i] << " " << obj._elementi2[i] << endl;
		return COUT;
	}
};

class Dogadjaj
{
	Datum _datumOdrzavanja;
	Kolekcija<string, bool> * _obaveze; //cuva informaciju o obavezama koje je potrebno ispuniti prije samog dogadjaja, string se odnosi na opis, a bool na izvrsenje te obaveze (da li je zavrsena ili ne)

	char *_naziv;
	int _notificirajPrije; //oznacava broj dana prije samog dogadjaja kada ce krenuti notifikacija/podsjetnik
	bool _rekurzivnaNotifikacija; //ako je vrijednost true onda se korisnik notificira svaki dan do _datumaOdrzavanja dogadjaja, a pocevsi prije dogadjaja za _brojDanaZaNotifikaciju

public:
	int getNotifyPrije() { return _notificirajPrije; }
	bool getRekurzivnaNotify() { return _rekurzivnaNotifikacija; }
	Datum getDatum() { return _datumOdrzavanja; }
	bool operator==(Dogadjaj dog) {
		if (strcmp(_naziv, dog._naziv) == 0) {
			return true;
		}
		else return false;
	}
	/*po vlasitom izboru definisati listu zabranjenih rijeci koje ce onemoguciti dodavanje odredjene obaveze. Prilikom provjere koristiti regex*/
	//onemoguciti dupliranje obaveza
	bool AddObavezu(string info) {
		string zabranjena_rijec1 = "Rat";
		string zabranjena_rijec2 = "Mrziti";

		regex pravilo(zabranjena_rijec1 + "|" + zabranjena_rijec2);
		if (regex_search(info, pravilo)) {
			cout << "ZABRANJENO!!!" << endl;
			return false;
		}
		
		
		if (_obaveze == nullptr) {
			_obaveze = new Kolekcija<string, bool>;
		}

		bool found = false;
		for (int i = 0; i < _obaveze->GetTrenutno(); i++) {
			if (_obaveze->GetElement1(i) == info) {
				found = true;
			}
		}
		if (found == false) {
			_obaveze->AddElement(info, false);
			return true;
		}
		else return false;
	
	}
	Dogadjaj(Datum datumOdrzavanja, const char *naziv, int brojDana = 1,
		bool rekurzivnaNotifikacija = false) : _datumOdrzavanja(datumOdrzavanja)
	{
		_naziv = new char[strlen(naziv) + 1];
		strcpy_s(_naziv, strlen(naziv) + 1, naziv);

		_notificirajPrije = brojDana;
		_rekurzivnaNotifikacija = rekurzivnaNotifikacija;
		_obaveze = nullptr;
	}

	Dogadjaj(const Dogadjaj &obj) : _datumOdrzavanja(obj._datumOdrzavanja)
	{
		_naziv = new char[strlen(obj._naziv) + 1];
		strcpy_s(_naziv, strlen(obj._naziv) + 1, obj._naziv);

		_notificirajPrije = obj._notificirajPrije;
		_rekurzivnaNotifikacija = obj._rekurzivnaNotifikacija;
		_obaveze = new Kolekcija<string,bool>(*obj._obaveze);
	}
	Dogadjaj& operator=(Dogadjaj& obj) {
		_datumOdrzavanja = obj._datumOdrzavanja;
		_naziv = new char[strlen(obj._naziv) + 1];
		strcpy_s(_naziv, strlen(obj._naziv) + 1, obj._naziv);

		_notificirajPrije = obj._notificirajPrije;
		_rekurzivnaNotifikacija = obj._rekurzivnaNotifikacija;
		_obaveze = new Kolekcija<string, bool>(*obj._obaveze);
		return *this;
	}
	~Dogadjaj()
	{
		delete[] _naziv;
		_naziv = nullptr;
		delete _obaveze;
		_obaveze = nullptr;
	}
	char *GetNaziv() { return _naziv; }
	Kolekcija<string, bool> *GetObaveze() { return _obaveze; }

};

class Student
{
	int _indeks;
	string _imePrezime;
	vector<Dogadjaj> _dogadjaji;
public:
	string getImePrezime() { return _imePrezime; }
	bool operator==(Student s) {
		if (_indeks == s._indeks) {
			return true;
		}
		else return false;
	}
	//onemoguciti dupliranje dogadjaja
	bool AddDogadjaj(Dogadjaj & dog) {
		bool found = false;

		vector<Dogadjaj>::iterator it = _dogadjaji.begin();
		while (it != _dogadjaji.end()) {
			if (*it == dog) {
				found = true;
			}
			it++;
		}
		if (found == false) {
			_dogadjaji.push_back(dog);
			return true;
		}
		else return false;

	}
	Student(int indeks=0, string imePrezime="") : _indeks(indeks), _imePrezime(imePrezime) {}
	Student(const Student& obj) {
		_indeks = obj._indeks;
		_imePrezime = obj._imePrezime;
		_dogadjaji = obj._dogadjaji;
	}
	Student& operator=(Student& obj) {
		_indeks = obj._indeks;
		_imePrezime = obj._imePrezime;
		_dogadjaji = obj._dogadjaji;
		return *this;
	}
	int GetIndeks() const { return _indeks; }

	vector<Dogadjaj>& GetDogadjaji() { return _dogadjaji; }

	friend ostream &operator<<(ostream &COUT, const Student &obj)
	{
		COUT << obj._imePrezime << " (" << obj._indeks << ")" << endl;
		return COUT;
	}
};
mutex m;
void Notify(Student s, Dogadjaj d, Datum dat) {
	m.lock();
	int br_obaveza = 0;
	int br_izvrsenih_obaveza = 0;
	for (int i = 0; i < d.GetObaveze()->GetTrenutno(); i++) {
		
		br_obaveza++;
	
		if (d.GetObaveze()->GetElement2(i) == true) {
			br_izvrsenih_obaveza++;
	
		}
	}

	cout << "Poštovani " << s.getImePrezime() << endl;
	cout << "Dogadjaj " << d.GetNaziv() << " je zakazan za " <<d.getDatum().get3Dana(dat)<< " dana," << endl;
	cout << " a do sada ste obavili " << (float(br_izvrsenih_obaveza)/float(br_obaveza))*100 << "% obaveza vezanih za ovaj dogadjaj." << endl;
	cout << "Neispunjene obaveze su:" << endl;
	for (int i = 0; i < d.GetObaveze()->GetTrenutno(); i++) {
		if (d.GetObaveze()->GetElement2(i) == false) {
			cout <<i+1<<". "<< d.GetObaveze()->GetElement1(i) << endl;
		}
	}
	cout << "Predlazemo Vam da ispunite i ostale planirane obaveze." << endl;
	cout << "FIT Reminder" << endl;
	m.unlock();
}
class DLWMSReminder
{
	vector<Student> _reminderList;
public:
	/*metodi PosaljiNotifikacije se salje trenutni datum na osnovu cega ona pretrazuje sve studente koje treba podsjetiti/notoficirati o dogadjajima koji se priblizavaju.
	Koristeci multithread-ing, svim studentima se salju notifikacije sa sljedecim sadrzajem:
	-------------------------------------------------------------------------
	Postovani Jasmin Azemovic,
	Dogadjaj Ispit iz PRIII je zakazan za 3 dana, a do sada ste obavili 56% obaveza vezanih za ovaj dogadjaj. Neispunjene obaveze su:
	1.Preraditi ispitne zadatke
	2.Samostalno vjezbati
	Predlazemo Vam da ispunite i ostale planirane obaveze.
	FIT Reminder
	-------------------------------------------------------------------------
	Dakle, notifikacije ce biti poslane svim studentima koji su dodali dogadjaj za 30.01.2018. godine i oznacili da zele da budu podsjecani ponovo/rekurzivno najmanje 2 dana prije samog dogadjaja (podaci se odnose na konkretan dogadjaj: Ispit iz PRIII)

	*/
	int PosaljiNotifikacije(Datum danas) {
		int brojac = 0;
		vector<Student>::iterator its = _reminderList.begin();
		while (its != _reminderList.end()) {
			
			for (int i = 0; i < its->GetDogadjaji().size(); i++) {
				
				if (its->GetDogadjaji()[i].getDatum().get3Dana(danas)<=its->GetDogadjaji()[i].getNotifyPrije()) {
					thread t(Notify, *its, its->GetDogadjaji()[i], danas);
					t.join();
					brojac++;
					
				}
			}
			
			its++;
		}
		return brojac;

	}
	//da bi bila oznacena kao zavrsena, obaveza mora postojati i mora biti oznacena kao nezavrsena (false)
	bool OznaciObavezuKaoZavrsenu(int indeks, string naziv_dogadjaja, string obaveza_info) {
		for (int i = 0; i < _reminderList.size(); i++) {
			if (_reminderList[i].GetIndeks() == indeks) {
				for (int j = 0; j < _reminderList[i].GetDogadjaji().size(); j++) {
					if (_reminderList[i].GetDogadjaji()[j].GetNaziv() == naziv_dogadjaja) {
						for (int k = 0; k < _reminderList[i].GetDogadjaji()[j].GetObaveze()->GetTrenutno(); k++) {
							if (_reminderList[i].GetDogadjaji()[j].GetObaveze()->GetElement2(k) == false) {
								if (_reminderList[i].GetDogadjaji()[j].GetObaveze()->GetElement1(k) == obaveza_info) {
									_reminderList[i].GetDogadjaji()[j].GetObaveze()->setT2(k,true);
									return true;
								}
							}
						}
					}
				}
			}
		}
		return false;
	}
	void AddStudent(Student& s) {

		vector<Student>::iterator it = _reminderList.begin();
		while (it != _reminderList.end()) {
			if (*it == s)
				throw Izuzetak("Student vec postoji", __FUNCTION__);
			it++;
		}


		_reminderList.push_back(s);

	}
	
	DLWMSReminder(): _reminderList(){

	}
	DLWMSReminder(const DLWMSReminder& obj) :_reminderList(obj._reminderList) {
		 
	}
	DLWMSReminder& operator=(DLWMSReminder& obj) {
		_reminderList = obj._reminderList;
	}
};

void main() {

	/**************************************************************************
	1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
	2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR CE BITI OZNACENO KAO "RE"
	3. SPASAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URADJENOG ZADATKA
	4. PROGRAMSKI CODE SE TAKODJER NALAZI U FAJLU CODE_ParcijalniII.TXT
	5. NAZIVI FUNKCIJA MORAJU BITI IDENTIÈNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U.OSTALE, POMOÆNE FUNKCIJE MOŽETE IMENOVATI PO ŽELJI.
	****************************************************************************/

#pragma region Datum

	Datum danas(28, 1, 2018), sutra(29, 1, 2018);
	Datum datumIspitaPRIII(30, 1, 2018), datumIspitBPII(31, 1, 2018);
	Datum prekosutra(danas);
	prekosutra = danas;
	cout << danas << endl
		<< sutra << endl
		<< prekosutra << crt;

#pragma endregion

#pragma region Kolekcija

	
	const int brElemenata = 10;
	Kolekcija<int, float> kolekcija1;
	for (int i = 0; i < brElemenata; i++)
		if (!kolekcija1.AddElement(i, i + (0.6 * i)))
			cout << "Elementi " << i << " i " << i + (0.6 * i) << " nisu dodati u kolekciju" << endl;

	cout << kolekcija1.GetElement1(0) << " " << kolekcija1.GetElement2(0) << endl;
	cout << kolekcija1 << endl;

	kolekcija1.RemoveElement(1);

	Kolekcija<int, float> kolekcija2;
	kolekcija2 = kolekcija1;
	cout << kolekcija2 << crt;

	if (kolekcija1.GetTrenutno() == kolekcija2.GetTrenutno())
		cout << "ISTI BROJ ELEMENATA" << endl;

	Kolekcija<int, float> kolekcija3(kolekcija2);
	cout << kolekcija3 << crt;

#pragma endregion

#pragma region Dogadjaj

	Dogadjaj ispitPRIII(datumIspitaPRIII, "Ispit iz PRIII", 5, true),
		ispitBPII(datumIspitBPII, "Ispit iz BPII", 7, true);

	if (ispitPRIII.AddObavezu("Preraditi pdf materijale"))cout << "Obaveza dodana!" << endl;

	if (!ispitPRIII.AddObavezu("Preraditi pdf materijale"))cout << "Obaveza nije dodana!" << endl;
	if (ispitPRIII.AddObavezu("Pregledati video materijale"))cout << "Obaveza dodana!" << endl;
	if (ispitPRIII.AddObavezu("Preraditi ispitne zadatke"))cout << "Obaveza dodana!" << endl;
	if (ispitPRIII.AddObavezu("Samostalno vjezbati"))cout << "Obaveza dodana!" << endl;

	if (ispitBPII.AddObavezu("Preraditi knjigu SQL za 24 h"))cout << "Obaveza dodana!" << endl;
	if (ispitBPII.AddObavezu("Pregledati video materijale"))cout << "Obaveza dodana!" << endl;
	if (ispitBPII.AddObavezu("Napraviti bazu za konkretnu aplikaciju"))cout << "Obaveza dodana!" << endl;

	Student jasmin(150051, "Jasmin Azemovic"), adel(160061, "Adel Handzic");

	if (jasmin.AddDogadjaj(ispitPRIII) && jasmin.AddDogadjaj(ispitBPII))
		cout << "Dogadjaj uspjesno dodan!" << endl;

	if (adel.AddDogadjaj(ispitPRIII) && adel.AddDogadjaj(ispitBPII))
		cout << "Dogadjaj uspjesno dodan!" << endl;
	
	if (!adel.AddDogadjaj(ispitPRIII))
		cout << "Dogadjaj nije uspjesno dodan!" << endl;

	DLWMSReminder reminder;

	try
	{
		reminder.AddStudent(jasmin);
		reminder.AddStudent(adel);
		//u slucaju dupliranja studenata funkcija baca izuzetak tipa Izuzetak
		reminder.AddStudent(jasmin);
	}
	catch (Izuzetak& err) {
		cout << err << endl;
	}
	catch (exception &err)
	{
		cout<<err.what();
	}
	catch (...) {
		cout << "PROBLM!!!";
	}



	if (reminder.OznaciObavezuKaoZavrsenu(150051, "Ispit iz PRIII", "Pregledati video materijale"))
		cout << "Obaveza oznacena kao zavrsena" << endl;

	
	int poslato = 0;
	//funkcija vraca broj poslatih podsjetnika/notifikacija
	poslato = reminder.PosaljiNotifikacije(danas);
	cout << "Za " << danas << " poslato ukupno " << poslato << " podsjetnika!" << endl;
	poslato = reminder.PosaljiNotifikacije(sutra);
	cout << "Za " << sutra << " poslato ukupno " << poslato << " podsjetnika!" << endl;
//
//#pragma endregion

	system("pause");
}