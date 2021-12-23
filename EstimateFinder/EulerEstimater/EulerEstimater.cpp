#include <iostream>
#include <iomanip>
#include <random>
#include <time.h>
#include <string.h>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;


const int decimalPrecision = 8;
const long int precision = pow(10, decimalPrecision);

random_device rand_dev;
mt19937 generator(rand_dev());
uniform_int_distribution<int> distr(0, precision);

const double e = 2.718281828459045;
int64_t loopCount = 1000000;
int64_t printFrequency = 100000;
bool doDebug = false;
bool doOutput = false;



string formatInt(int64_t intToFormat) {
	string number = to_string(intToFormat);
	int length = number.length();
	string output = "";

	for (int i = 0; i < length; i++) {
		output += number[i];
		if ((length - (i + 1)) % 3 == 0 && (i+1) != length) {
			output += " ";
		}
	}

	return output;
}

string colorString(string stringToColor, int foregroundColor = 37, int backgroundColor = 40) {
	/*
	* Color   : fg : bg
	*
	* Black   : 30 : 40
	* Red     : 31 : 41
	* Green   : 32 : 42
	* Yellow  : 33 : 43
	* Blue    : 34 : 44
	* Magenta : 35 : 45
	* Cyan    : 36 : 46
	* White   : 37 : 47
	*/

	return "\033[" + to_string(foregroundColor) + ";" + to_string(backgroundColor) + "m" + stringToColor
		+ "\033[0m";
}

int rollTheDice() {
	int rolls = 0;
	long double runningCount = 0.0;

	while (runningCount < 1) {
		int randNum = distr(generator);
		runningCount += randNum / (double)precision;
		rolls++;
	}

	return rolls;
}



long double simulateToFile() {
	time_t t = time(0);
	cout << endl << "Timestamp: " << t << endl;

	ifstream i("Results.json");
	json j;
	if (i)
		i >> j;
	int64_t currentIndex = 0;

	int index = j.size();

	j[index]["Update frequency"] = printFrequency;
	j[index]["Timestamp"] = t;
	j[index]["Loop count"] = loopCount;

	long double currentResult = 0.0;

	for (int64_t i = 1; i <= loopCount; i++) {
		long double result = rollTheDice();

		long double sum = currentResult * (i-1) + result;
		currentResult = sum / i;

		if (i % printFrequency == 0) {
			j[index]["Results"][currentIndex++] = currentResult;

			if (doDebug)
				cout << "Current estimation at " << formatInt(i) << " iterations: " << setprecision(16) << currentResult << endl;
		}
	}

	j[index]["Estimation"] = currentResult;
	std::ofstream o("Results.json");
	o << std::setw(4) << j << std::endl;

	return currentResult;
}

long double simulate() {
	long double currentResult = 0.0;
	int64_t loopsFinished = 0;

	if (!doDebug)
		for (int64_t i = 0; i < loopCount; i++) {
			long double result = rollTheDice();

			long double sum = currentResult * loopsFinished + result;
			currentResult = sum / ++loopsFinished;
		}
	else
		for (int64_t i = 0; i < loopCount; i++) {
			long double result = rollTheDice();

			long double sum = currentResult * loopsFinished + result;
			currentResult = sum / ++loopsFinished;

			if ((i + 1) % printFrequency == 0) {
				cout << "Current estimation at " << formatInt(loopsFinished) << " iterations: " << setprecision(16) << currentResult << endl;
			}
		}

	return currentResult;
}

void changeSettings() {

	while (true) {
		system("CLS");


		cout	<< "---------------------" << endl
				<< "Settings" << endl
				<< "---------------------" << endl << endl;

		cout << "Current settings:" << endl;
		cout << "Number of simulations: " << formatInt(loopCount) << endl;
		cout << "Print frequency: " << formatInt(printFrequency) << endl;
		cout << "Debug mode " << (doDebug ? colorString("enabled", 32) : colorString("disabled", 31)) << endl;
		cout << "Output mode " << (doOutput ? colorString("enabled", 32) : colorString("disabled", 31)) << endl << endl;

		cout << "Enter 1 to set how many loops the simulation should do" << endl;
		cout << "Enter 2 to set how many updates of the simulation should be printed" << endl;
		cout << "Enter 3 to toggle debug state" << endl;
		cout << "Enter 4 to toggle output to file" << endl;
		cout << "Enter 0 to return" << endl << endl;

		string input;
		int inputInt;

		cin >> input;
		inputInt = stoi(input);
		cout << endl << endl;

		switch (inputInt) {
		case 1:
			cout << "Currently doing " << formatInt(loopCount) << " loops" << endl;
			cout << "New loop count: ";
			cin >> input;

			loopCount = stoll(input);
			break;
		case 2:
			cout << "Currently printing every " << formatInt(printFrequency) << " simulations" << endl;
			cout << "New print frequency: ";
			cin >> input;

			printFrequency = stoll(input);
			break;
		case 3:
			doDebug = !doDebug;
			break;
		case 4:
			doOutput = !doOutput;
			break;
		case 0:
			return;
		default:
			break;
		}
	}
}

string getDeviation(long double result) {
	long double deviation = result - e;
	if (deviation < 0)
		return to_string(deviation);
	else
		return "+" + to_string(deviation);
}


int main() {
	srand(time(NULL));

	while (true) {
		system("CLS");

		// Menu structure
		cout	<< "---------------------" << endl
				<< "Main Menu" << endl
				<< "---------------------" << endl << endl;
		

		string input;
		int inputInt;
		long double lastResult;

		cout << "Enter 1 to simulate using the current settings" << endl;
		cout << "Enter 2 to change settings" << endl;
		cout << "Enter 0 to exit" << endl << endl;
		cin >> input;
		inputInt = stoi(input);

		switch (inputInt) {
		case 0:
			return 0;
			break;
		case 1:
			lastResult = (doOutput ? simulateToFile() : simulate());
			cout << endl << endl <<	"Estimation of e after " << formatInt(loopCount) << " simulations: " << setprecision(16) << lastResult << endl;
			cout << "Actual value of e: " << e << endl;
			cout << "Deviation: " << setprecision(16) << getDeviation(lastResult) << endl;
			system("PAUSE");
			break;
		case 2:
			changeSettings();
			break;
		default:
			cout << input << " is not a valid entry, please try again." << endl;
			break;
		}
	}
}