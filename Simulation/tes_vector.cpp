#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>

using namespace std;

bool b2s (int i,int j) { return (i>j); }

int main()
{
	ofstream output;
	string out;
	out = "test_out.txt";
	output.open (out);
	
	vector<int> vehicles;
	int a, b;
	while (true)
	{
		cout << endl << "pilih 1 untuk nambah, 2 untuk hapus, 3 untuk keluar " << endl;
		cin >> a;
		if (a == 1)
		{	
			cin >> b;
			vehicles.push_back(b);
			if(vehicles.size() > 1)
				sort (vehicles.begin(), vehicles.end(), b2s);
			int k=0;
			while(k<vehicles.size())
			{
				cout << vehicles[k] << " ";
				k++;
			}
			cout << endl;
		}
		else if(a == 2)
		{
			vehicles.pop_back();
			int k=0;
			while(k < vehicles.size())
			{
				cout <<vehicles[k] << " ";
				++k;
			}
			cout << endl;
		}
		else
			break;
	}
	output.close();
	return 0;
}