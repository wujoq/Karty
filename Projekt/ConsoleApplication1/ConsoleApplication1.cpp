#include<iostream>
#include<algorithm>
#include<random>
#include<thread>
#include<vector>
#include<string>


using namespace std;


class Gracz{
public:
	Gracz() {};
	~Gracz() {};
	//wektor kart na rece gracza
	vector<string> Stosy;
	vector<string> Reka;
	//wektor stosow, wołanie 
	int IleNaRece(string co, vector<string>r)
	{
		int ile = 0;

		for (int i =0 ; i<r.size(); i++)
		{
			if (co == r[i].substr(0, r[i].size() - 1))
			{
				ile++;
			}
		}
		return ile;
	}
	void SprawdzStosy(vector<string> &s, vector<string> &r) {
		string co;

		for (int i = 0; i < r.size(); i++)
		{
			co = r[i].substr(0, r[i].size() - 1);
			if (IleNaRece(co, r)==4) {
				for (int j = 0; j < r.size(); j++)
				{
					if (co == r[j].substr(0, r[j].size() - 1)) {
						r.erase(r.begin() +j);
						j--;
					}
				}
				s.push_back(co);
			}
		}
		
	}
	void Dobierz(vector<string> &T)
	{
		if (T.size() > 0) {
			Reka.push_back(T[T.size() - 1]);
			T.pop_back();
		}
		else cout << "Brak kart, nie mozna dobrac";
	}

	bool CzyNaRece(vector<string> R1,string pytanie) {
			for (int i = 0; i < R1.size(); i++) {
				if (pytanie == R1[i].substr(0, pytanie.size())) {
					return true;
				}
			}
		return false;
	}

	void Przekaz(vector<string>& R1, vector<string>& R2, string pytanie ){
		cout << "Dostajesz:";

		for (int i = 0; i < R2.size(); ++i)
		{

			if (pytanie == R2[i].substr(0, pytanie.size()))
			{
				R1.push_back(R2[i]);
				cout << R2[i] << " ";
				R2.erase(R2.begin() + i);
				i--;
			}
		}
		
	}

	void Wolanie(vector<string>& R1, vector<string>& R2, vector<string>& V) {
		string pytanie;
		cout << endl<< "Pytaj o: ";
		cin >> pytanie;
		if (pytanie[0] >= 'a' && pytanie[0] < 'z')
			pytanie[0] -= 32;
		if (CzyNaRece(R1, pytanie))
		{
			if (CzyNaRece(R2, pytanie))
			{
				Przekaz(R1, R2, pytanie);
				cout << endl << "Reka gracza" << endl;
				for (int i = 0; i < R1.size(); i++)
					cout << R1[i] << " ";
				Wolanie(R1, R2, V);
			}
			else {
				Dobierz(V);
			}
		}
		else {
			cout << "Nie mozesz prosic o te karte";
			Wolanie(R1, R2, V);
		}

	}

	void Wolanie_komp(vector<string>& R1, vector<string>& R2, vector<string>& V)
	{
		string pytanie;
		srand(time(0));
		int ran = rand() % R1.size();
		pytanie = R1[ran].substr(0, R1[ran].size()-1);
		cout << pytanie <<" ";
		if (CzyNaRece(R2, pytanie))
		{
			Przekaz(R1, R2, pytanie);			
			Wolanie_komp(R1, R2, V);		
		}
		else {
			Dobierz(V);
		}
	}
private:

};

void Tasowanie(vector<string> &V)
{
	srand(time(0));
	vector<string>PomVector;
	while (V.size() > 0)
	{
			int ran = rand() % V.size();
			PomVector.push_back(V[ran]);
			V.erase(V.begin() + ran);
	}
	for (int i = 0; i < PomVector.size() ; i++)
	{
		V.push_back(PomVector[i]);
	}
}

void Rozdanie(vector<string> &T, vector<string> &R1, vector<string> &R2) {
	for (int i = 0; i < 7; i++)
	{
		R1.push_back(T[i]);
		T.erase(T.begin() + i);

		R2.push_back(T[i]);
		T.erase(T.begin() + i);
	}
}

int main()
{
	//stworzenie talii kart
	vector<string> Talia = {"2K","3K","4K","5K","6K","7K","8K","9K","10K","JK","DK","KK","AK",
							"2T","3T","4T","5T","6T","7T","8T","9T","10T","JT","DT","KT","AT",
							"2k","3k","4k","5k","6k","7k","8k","9k","10k","Jk","Dk","Kk","Ak",
							"2P","3P","4P","5P","6P","7P","8P","9P","10P","JP","DP","KP","AP" };
	for (int i = 0; i < Talia.size() - 1; i++)
		cout << Talia[i] << " ";
	cout << endl<<endl;
	
	//tasowanie w osobnym wątku
	thread T(Tasowanie, ref(Talia));
	T.join();
	 
	Gracz ja;
	Gracz pc;

	Rozdanie(Talia, ja.Reka, pc.Reka);
	while (Talia.size()>0)
	{
		cout << endl << "Reka gracza" << endl;
		for (int i = 0; i < ja.Reka.size(); i++)
			cout << ja.Reka[i] << " ";

		cout << endl << "Reka komputera" << endl;
		for (int i = 0; i < pc.Reka.size(); i++)
			cout << pc.Reka[i] << " ";

		cout <<endl << "gracz "; ja.Wolanie(ja.Reka, pc.Reka, Talia);
		ja.SprawdzStosy(ja.Stosy, ja.Reka);
		cout <<endl<<  "komputer "; pc.Wolanie_komp(pc.Reka, ja.Reka, Talia);
		pc.SprawdzStosy(pc.Stosy,pc.Reka);

	}
}
