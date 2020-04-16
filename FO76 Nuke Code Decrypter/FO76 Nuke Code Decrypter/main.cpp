#include <iostream>
#include <fstream>
using namespace std;

struct codePiece {
	char letter;
	char decoded;
	char number;
};

const string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

string toUppercase(string in);
string buildKeyedAlphabet(string code, string baseAlphabet);
char decryptCodePieces(char codePieces, string keyedAlphabet, string baseAlphabet);
string* loadWordFile(string filename);

int main()
{
	fstream storedCodes;
	bool run = true;
	storedCodes.open("StoredCodes.txt");
	if (storedCodes.is_open())
	{
		cout << "Previous codes found:" << endl;
		string temp;
		while (!storedCodes.eof())
		{
			if (storedCodes.eof())
			{
				continue;
			}
			else
			{
				storedCodes >> temp;
				cout << temp << " ";
			}
		}
		cout << endl;
		storedCodes.close();
	}
	while (run)
	{
		string input;
		codePiece codes[8];

		//build keyed alphabet with provided weekly codeword (converted to uppercase letters)
		cout << "--------------------" << endl << "Weekly Codeword: ";
		cin >> input;
		if (input == "0")
		{
			cout << "Shutting down.";
			run = false;
			return 0;
		}
		string weeklyCodeword = toUppercase(input);
		string keyedAlphabet = buildKeyedAlphabet(weeklyCodeword, alphabet);
		cout << "--------------------" << endl << "Keyed Alphabet: " << keyedAlphabet << endl;

		//take the keyed alphabet and use it to decrypt the codepieces
		cout << "--------------------" << endl << "Codepiece Pairs (A1B2C3...): ";
		cin >> input;
		int count = 0;
		string codePieces = toUppercase(input);
		for (int i = 0; i < codePieces.length(); i++)
		{
			codes[count].letter = codePieces.at(i);
			i++;
			codes[count].number = codePieces.at(i);
			count++;
		}

		for (int i = 0; i < 8; i++)
		{
			codes[i].decoded = decryptCodePieces(codes[i].letter, keyedAlphabet, alphabet);
		}

		cout << "--------------------" << endl;
		ifstream file;
		file.open("Words.txt");
		storedCodes.open("StoredCodes.txt", fstream::out);
		if (file.is_open())
		{
			string temp;
			bool flag = false;
			while (!file.eof())
			{
				file >> temp;
				for (int x = 0; x < temp.length(); x++)
				{
					flag = false;
					for (int y = 0; y < 8; y++)
					{
						if (temp[x] == codes[y].decoded)
						{
							flag = true;
						}
						if (flag) { break; }
					}
					if (flag) { continue; }
					else { break; }
				}
				if (flag)
				{
					cout << "Possible Match: " << temp << " || ";
					if (storedCodes.is_open())
					{
						storedCodes << "\n";
					}
					for (int x = 0; x < 8; x++)
					{
						for (int y = 0; y < 8; y++)
						{
							if (temp.at(x) == codes[y].decoded)
							{
								cout << codes[y].number;
								if (storedCodes.is_open())
								{
									storedCodes << codes[y].number;
								}
								break;
							}
						}
					}
					cout << endl;
				}
			}
			storedCodes.close();
			file.close();
		}
	}
	return 0;
}

string toUppercase(string in)
{
	string out = "";
	for (int i = 0; i < in.length(); i++)
	{
		if (in.at(i) >= 97 && in.at(i) <= 122)
		{
			out += (in.at(i) - 32);
		}
		else
		{
			out += (in.at(i));
		}
	}

	return out;
}

string buildKeyedAlphabet(string code, string baseAlphabet)
{
	string out = code;
	for (int i = 0; i < baseAlphabet.length(); i++)
	{
		int counter;
		bool flag = false;

		for (counter = 0; counter < code.length(); counter++)
		{
			if (baseAlphabet.at(i) == code.at(counter))
			{
				flag = true;
				break;
			}
		}

		if (flag == false)
		{
			out += baseAlphabet.at(i);
			continue;
		}
	}
	return out;
}

char decryptCodePieces(char codePieces, string keyedAlphabet, string baseAlphabet)
{
	for (int x = 0; x < keyedAlphabet.length(); x++)
	{
		if (codePieces == keyedAlphabet.at(x))
		{
			return baseAlphabet.at(x);
		}
	}
}

string* loadWordFile(string filename)
{
	ifstream input;
	string* output = new string[1];
	int count = 0;
	input.open(filename);
	if (input.is_open())
	{
		string temp;
		while (!input.eof())
		{
			input >> temp; ;
			count++;
		}
		input.close();

	}

	input.open(filename);
	if (input.is_open())
	{
		output = new string[count - 1];

		for (int i = 0; i < count; i++)
		{
			string temp;
			input >> temp;
			output[i] = toUppercase(temp);
		}
		input.close();
	}
	return output;
}
