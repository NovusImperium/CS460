#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;

int main (int argc, char * argv [])
{
	ifstream input (argv[1]);
	int i = 0, n, p = 1;
	while (input >> n)
	{
		cout << left << setw (10) << n;
		if (++i == 8)
		{
			cout << endl;
			i = 0;
		}
		p *= n;
	}
	if (i)
		cout << endl;
	cout << "The product of the numbers is " << p << endl;
	return 0;
}
