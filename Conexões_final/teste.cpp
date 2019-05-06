#include <iostream>

using namespace std;

int current_port = 5000;

int main(int argc, char const *argv[])
{
	int pixels = 1000;
	int	number_workers;
	int pixels_worker;
	cout << "Quantos workers? ";
	cin >> number_workers; 

	for (int i = 0; i < number_workers; i++)
	{
		pixels_worker = pixels/number_workers;
		cout << "Worker " << i << " pela porta " << current_port << " para calcular " << endl;
		current_port++;
	}

	return 0;
}