// =================================================================
//
// File: main.cpp
// Author: Fernanda T
// Date:
//
// =================================================================
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;

vector<string> readLines(string filename){
	vector<string> lines;
	ifstream file(filename);
	string line;
	while (getline(file, line)){
		lines.push_back(line);
	}
	return lines;
}

vector<string> split(string line){
	vector<string> words;
	string word;
	for (int i = 0; i < line.length(); i++){
		if (line[i] == ' '){
			words.push_back(word);
			word = "";
		}
		else{
			word += line[i];
		}
	}
	words.push_back(word);
	return words;
}

vector<string> filter(vector<string> lines, string word){
	vector<string> filteredLines;
	for (string line : lines) {
		if (line.find(word) != string::npos) {
			filteredLines.push_back(line);
		}
	}
	return filteredLines;
}

// =================================================================
// Usamos sort() para el filtro por fecha.

vector<string> sortLines(vector<string> lines){
	sort(lines.begin(), lines.end(), [](string a, string b) {
		vector<string> aSplit;
		vector<string> bSplit;
		string delimiter = "-";
		size_t pos = 0;
		string token;
		while ((pos = a.find(delimiter)) != string::npos) {
			token = a.substr(0, pos);
			aSplit.push_back(token);
			a.erase(0, pos + delimiter.length());
		}
		aSplit.push_back(a);
		while ((pos = b.find(delimiter)) != string::npos) {
			token = b.substr(0, pos);
			bSplit.push_back(token);
			b.erase(0, pos + delimiter.length());
		}
		bSplit.push_back(b);
		return pair<string, string>(aSplit[1], aSplit[0]) < 
		pair<string, string>(bSplit[1], bSplit[0]);
	});
	return lines;
}
// =================================================================
vector<string> keepLastColumn(vector<string> lines){
	vector<string> lastColumn;
	for (string line : lines) {
		vector<string> words = split(line);
		lastColumn.push_back(words[words.size() - 1]);
	}
	return lastColumn;
}

void month(vector<string> filteredLines, ofstream& output, string month, string date){
	vector<string> aux, M, R, lastColumnM, lastColumnR;

	output << month << endl;
	aux = filter(filteredLines, date);
	M = filter(aux, " M ");
	R = filter(aux, " R ");
	lastColumnM = keepLastColumn(M);
	lastColumnR = keepLastColumn(R);
	if(M.size() > 0){
		output << "M " << M.size() << ": "; 
		for (string line : lastColumnM){
			output << line << " ";
		}
		output << endl;
	}
	if(R.size() > 0){
		output << "R " << R.size() << ": ";
		for (string line : lastColumnR){
			output << line << " ";
		}
	}
	output << endl;
}


int main(int argc, char* argv[]) {
	string delimiter = " ", token, firstLine;
	vector<string> lines, firstLineSplit, filteredLines;
	string monthArray[12] = {"jan", "feb", "mar", "apr", "may", "jun",
	 "jul", "aug", "sep", "oct", "nov", "dec"};
	string dateArray[12] = {"-01-", "-02-", "-03-", "-04-", "-05-", "-06-",
	 "-07-", "-08-", "-09-", "-10-", "-11-", "-12-"};
	ifstream input(argv[1]);
	ofstream output(argv[2]);

	// error handling
	if (argc != 3) {
		cout << "Usage: " << " <input file> <output file>" << endl;
		return -1;
	}else if (!input) {
		cout << "Error: Could not open input file " << argv[1] << endl;
		return -1;
	}else if (!output) {
		cout << "Error: Could not open output file " << argv[2] << endl;
		return -1;
	}

	// read input and store it in a vector
	lines = readLines(argv[1]);

	// get first line and split it
	firstLine = lines[0];
	firstLineSplit = split(firstLine);

	filteredLines = filter(lines, firstLineSplit[1]);
	filteredLines.erase(filteredLines.begin());

	filteredLines = sortLines(filteredLines);

	for (int i = 0; i < 12; i++){
		month(filteredLines, output, monthArray[i], dateArray[i]);
	}

	input.close();
	output.close();

	return 0;
}
