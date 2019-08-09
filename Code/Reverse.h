using namespace std;

int reverse(int array[], int count)
{
	int temp, i;
	for (i = 0; i < count / 2; ++i) {
		temp = array[count - i - 1];
		array[count - i - 1] = array[i];
		array[i] = temp;
	}

	for (i = 0; i < count; ++i) {
		cout << array[i] << " ";
	}
	return 0;
}