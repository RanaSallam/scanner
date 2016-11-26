#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>


using namespace std;

void scan(){
	const string reservedWords[8] = { "if", "then", "else", "end", "repeat", "until", "read", "write" };
	const string specialSymbols[10] = { "+", "-", "*", "/", "=", "<", "(", ")", ";", ":=" };

	string codeLine;
	ifstream readFile("tiny_sample_code.txt"); //read
	ofstream writeFile1("scanner_output.txt"); //write
	ofstream writeFile2("scanner_output2.txt"); //write (parser-input version)

	if (readFile.is_open() && writeFile1.is_open() && writeFile2.is_open())
	{
		string token="";
		int state = 1;
		char c;

		while (getline(readFile, codeLine))
		{
			for (int n = 0; n <= codeLine.length(); n++)
			{
				c = codeLine[n];
				switch (state)
				{
				case 1:
					//START
					if (isdigit(c))
					{
						//INNUM
						token = token + c;
						state = 2;
					}
					else if (isalpha(c))
					{
						//INID
						token = token + c;
						state = 3;
					}
					else if (c == ':')
					{
						//INASSIGN
						token = token + c;
						state = 4;
					}
					else if (c == '{')
					{
						//INCOMMENT
						state = 5;
					}
					else if (c == ' ')
					{
						state = 1;
					}
					else
					{
						//DONE
						token = token + c;
						state = 6;
						n--;
					}
					break;

				case 2:
					//INNUM
					if (isdigit(c))
					{
						token = token + c;
						state = 2;
					}
					else
					{
						state = 6;
						n = n - 2;
					}
					break;

				case 3:
					//INID
					if (isalpha(c))
					{
						token = token + c;
						state = 3;
					}
					else
					{
						state = 6;
						n = n - 2;
					}
					break;

				case 4:
					//INASSIGN
					if (c == '=')
					{
						token = token + c;
						state = 6;
						n--;
					}
					else
					{
						state = 6;
						n = n - 2;
					}
					break;

				case 5:
					//INCOMMENT
					if (c == '}')
						state = 1;
					else
						state = 5;
					break;

				case 6:
					//DONE
					for (int i = 0; i < 8; i++)
					{
						if (token == reservedWords[i])
						{
							cout << token << setw(35 - token.length()) << " : reserved Word" << endl;
							writeFile1 << token << setw(35 - token.length()) << " : reserved Word" << endl;

							if (token == "then" || token == "repeat")
								writeFile2 << token << " \n";
							else if (token == "end")
								writeFile2 << "\n" << token << " \n";
							else
								writeFile2 << token << " ";
							
							token = "";						
							state = 1;
							break;
						}
					}
					for (int i = 0; i < 10; i++)
					{
						if (token == specialSymbols[i])
						{
							cout << token << setw(35 - token.length()) << " : special Symbol" << endl;
							writeFile1 << token << setw(35 - token.length()) << " : special Symbol" << endl;

							if (token == ";")
								writeFile2 << token << " \n";
							else
								writeFile2 << token << " ";

							token = "";
							state = 1;
							break;
						}
					}
					if (isalpha(token[0]))
					{
						cout << token << setw(35 - token.length()) << " : identifier" << endl;
						writeFile1 << token << setw(35 - token.length()) << " : identifier" << endl;
						writeFile2 << "identifier ";
						token = "";
						state = 1;
						break;
					}
					else if (isdigit(token[0]))
					{
						cout << token << setw(35 - token.length()) << " : number" << endl;
						writeFile1 << token << setw(35 - token.length()) << " : number" << endl;
						writeFile2 << "number ";
						token = "";
						state = 1;
						break;
					}
					else
					{
						token = "";
						state = 1;
						break;
					}
					
				default: cout << "state: " << state << endl;
					break;
				}
			}
		}
		readFile.close();
		writeFile1.close();
		writeFile2.close();
	}
	else {
		cout << "Specify a file.txt to read from";
	}
}

void main() {
	scan();	
	system("pause");

}