#include <iostream>
#include <fstream>
#include <string>

using namespace std; 

int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		cout << "Usage: exe <input_file> <output_file>" << endl;
		return -1;
	}
	//get number of elements
	int numElements = 0;
	string line = "";
	ifstream in(argv[1]);
	while(getline(in, line))
		++numElements;
	in.close();
	
	//open file and store in array
	int* data = new int[numElements];
	int i = 0;
	ifstream in2(argv[1]);
	while(!in2.eof() || in2.peek() != EOF)
	{
		in2 >> ws;
		if(in2.eof() || in2.peek() == EOF)
			break;
		in2 >> data[i];
		++i;
	}
	in2.close();

	//write array to file
	FILE* fp = fopen(argv[2], "w");
	for(int i=0; i<numElements; ++i)
		fprintf(fp, "%d\n", data[i]);
	fclose(fp);

	//cleanup
	delete [] data;

	return 0;
}

