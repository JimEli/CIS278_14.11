/*************************************************************************
* Title: Hardware Inventory
* File: CIS278_Week5_14.11.cpp
* Author: James Eli
* Date: 2/2/2018
*
* Exercise 14.11 on page 651-652, Hardware Inventory
* You own a hardware store and need to keep an inventory that can tell you 
* what different tools you have, how many of each you have on hand and the 
* cost of each one. Write a program that initializes the random-access 
* file hardware.dat to 100 empty records, lets you input the data 
* concerning each tool, enables you to list all your tools, lets you delete 
* a record for a tool that you no longer have and lets you update any 
* information in the file. The tool identification number should be the 
* record number. Use the information below to start your file.
*
*   Record #  Tool name        Quantity  Cost
*   3         Electric sander  7         57.98
*   17        Hammer           76        11.99
*   24        Jig saw          21        11.00
*   39        Lawn mower       3         79.50
*   56        Power saw        18        99.99
*   68        Sledge hammer    11        21.50
*   83        Wrench           34        7.50
*
* Notes:
*  (1) Compiled with MS Visual Studio 2017 Community (v141).
*
* Submitted in partial fulfillment of the requirements of PCC CIS-278.
*************************************************************************
* Change Log:
*   02/2/2018: Initial release. JME
*************************************************************************/
#include <iostream>   // cin/cout/endl
#include <string>     // string
#include <fstream>    // file streams
#include <iomanip>    // cout manipulators.
#include <cstdlib>    // file streams

#include "ToolData.h" // ToolData class definition
#include "input.cpp"  // numeric input routines

using namespace std;

// Menu choice enumerations.
enum class Choice { DISPLAY = 1, UPDATE, NEW, DELETE, END, INVALID };

// Return file size.
int fSize(fstream& file) {
	file.seekg(0, ios::end);
	int fileSize = static_cast<int>(file.tellg());
	file.seekg(0, ios::beg);
	return fileSize;
}

// Test if file is empty.
bool fEmpty(fstream& fs)
{
	if (fs.peek() == fstream::traits_type::eof())
		return true;
	return false;
}

// File seek and read record.
void fileSeekRead(fstream &fs, const int number, ToolData& Tool)
{
	// Move file-position pointer to correct record in file.
	fs.seekg((number - 1) * sizeof(ToolData));
	// Create record object and read first record from file.
	fs.read(reinterpret_cast<char*>(&Tool), sizeof(ToolData));
}

// File seek and write record.
void fileSeekWrite(fstream& fs, const int number, ToolData tool)
{
	// Move file-position pointer to correct record.
	fs.seekp((number - 1) * sizeof(ToolData));
	// Write updated record over old record in file.
	fs.write(reinterpret_cast<const char*>(&tool), sizeof(ToolData));
}

// Enable user to input menu choice.
Choice enterChoice()
{
	char input;

	// Display available options.
	cout << "\nEnter your choice\n"
		<< "1 - Display all tools\n"
		<< "2 - Update a tool\n"
		<< "3 - Add a new tool\n"
		<< "4 - Delete a tool\n"
		<< "5 - End program\n? ";

	// Input menu selection from user.
	cin >> input;
	cin.ignore();
	if (input >= '1' && input <= '5')
		return static_cast<Choice>(input - '0');
	return Choice::INVALID;
}

// Capture user string input of tool name. 
void getName(string &name)
{
	string temp("");

	do 
	{
		cout << "Please enter the tool name [20 chars max]: ";
		getline(cin, temp, '\n');
		cin.clear();
	} while (temp.size() > 19);


	// If empty, no change.
	if (!temp.empty())
		name = temp;
}

// Update record.
void updateRecord(fstream& fs) 
{
	// Number of tool to update.
	int toolNumber, recordNumber{ -1 };
	getNumber<int>("Enter tool number: ", recordNumber, 1, 100);

	// Move file-position pointer, create record object and read first record from file.
	ToolData Tool;
	fileSeekRead(fs, recordNumber, Tool);

	// Update record.
	if (Tool.getToolNumber() != 0) 
	{
		// Display record.
		cout << Tool;

		string name;
		int quantity;
		double cost;

		getName(name);
		Tool.setToolName(name);
		if (getNumber<int>("Enter tool number: ", toolNumber, 0, 100))
			Tool.setToolNumber(toolNumber);
		if (getNumber<int>("Enter tool quantity: ", quantity, 0, 1000))
			Tool.setToolQuantity(quantity);
		if (getNumber<double>("Enter tool cost: ", cost, 0., 10000.))
			Tool.setToolCost(cost);

		// Move file-position pointer & write updated record over old record in file.
		fileSeekWrite(fs, toolNumber, Tool);

		if (toolNumber != recordNumber) 
		{
			// Delete recordnumber.
			const ToolData blankTool;
			// Move file-position pointer & replace existing record with blank record.
			fileSeekWrite(fs, recordNumber, blankTool);
			cout << "Tool #" << recordNumber << " deleted.\n";
		}
	}
	else 
	{
		// display error if tool does not exist.
		cerr << "Tool #" << recordNumber << " has no information.\n";
	}
}

// Create and insert record.
void newRecord(fstream& fs) 
{
	// Obtain number of tool to create.
	int recordNumber{ -1 };
	getNumber<int>("Enter tool number: ", recordNumber, 1, 100);

	// Move file-position pointer to correct record in file.
	// Read record from file.
	ToolData Tool;
	fileSeekRead(fs, recordNumber, Tool);

	// Create record, if record does not previously exist.
	if (Tool.getToolNumber() == 0) 
	{
		string name;
		int quantity;
		double cost;

		Tool.setToolNumber(recordNumber);
		getName(name);
		Tool.setToolName(name);
		if (getNumber<int>("Enter tool quantity: ", quantity, 0, 1000))
			Tool.setToolQuantity(quantity);
		if (getNumber<double>("Enter tool cost: ", cost, 0., 10000.))
			Tool.setToolCost(cost);

		// Move file-position pointer to correct record & insert record in file.
		fileSeekWrite(fs, recordNumber, Tool);
	}
	else 
	{ 
		// Display error if tool already exists.
		cerr << "Tool #" << recordNumber << " already contains information.\n";
	}
}

// Delete an existing record.
void deleteRecord(fstream& file) 
{
	// Obtain number of tool to delete.
	int recordNumber{ -1 };
	getNumber<int>("Enter tool number: ", recordNumber, 1, 100);

	// Move file-position pointer to correct record in file.
	// Read record from file.
	ToolData Tool;
	fileSeekRead(file, recordNumber, Tool);

	// Delete record, if record exists in file.
	if (Tool.getToolNumber() != 0) 
	{
		// Create blank record.
		ToolData blankTool;
		// Move file-position pointer & replace existing record with blank record.
		fileSeekWrite(file, recordNumber, blankTool);
		cout << "Tool #" << recordNumber << " deleted.\n";
	}
	else 
	{ 
		// Display error if record does not exist.
		cerr << "Tool #" << recordNumber << " is empty.\n";
	}
}

// Display formatted inventory file contents.
void displayFile(fstream& fs)
{
	// Output column heads.
	cout << "Tool Number  Tool Name             Quantity  Cost\n";

	// Set file-position pointer to beginning of file & read first record.
	ToolData Tool;
	fileSeekRead(fs, 1, Tool);

	// Copy all records from file into output file.
	while (!fs.eof())
	{
		// Write single record to output stream, skipping empty records.
		if (Tool.getToolNumber() != 0)
			cout << Tool;

		// Read next record from record file.
		fs.read(reinterpret_cast<char*>(&Tool), sizeof(ToolData));
	}
}

int main() {
	Choice choice;   // Stores user menu choice.
	fstream fsTools; // File stream.
	string filename = "hardware.dat";

	// Attempt to open file for reading/writing.
	fsTools.open(filename, ios::in | ios::out | ios::binary);
	// attempt to create the file with 100 blank records.
	if (!fsTools.is_open())
	{
		ofstream ofs;

		ofs.open(filename, ios::out | ios::binary);
		if (ofs) {
			ToolData Tool;

			for (int i = 0; i < 100; i++)
				ofs.write(reinterpret_cast<const char*>(&Tool), sizeof(ToolData));
		}
		ofs.close();
		cerr << "New \"" << filename << "\" file created.\n";
		// Attempt to re-open file for reading and writing.
		fsTools.open(filename, ios::in | ios::out | ios::binary);
	}

	// Exit program if fstream cannot open file.
	if (!fsTools)
	{
		cerr << "File could not be opened.\n";
		exit(EXIT_FAILURE);
	}

#ifndef NDEBUG
	// Debugging stuff.
	cout << "Debug file stream info:\n";
	cout << boolalpha << "File is open: " << fsTools.is_open() << endl;
	cout << "File size: " << fSize(fsTools) << endl;
	cout << "File empty: " << fEmpty(fsTools) << endl;
	cout << "Fail flag: " << fsTools.fail() << endl;
	//fsTools.close();
	//fsTools.clear();
#endif

	// Enable user to specify action.
	while ((choice = enterChoice()) != Choice::END)
	{
		switch (choice)
		{
		case Choice::DISPLAY:
			// Display file contents.
			displayFile(fsTools);
			break;

		case Choice::UPDATE:
			// Update record.
			updateRecord(fsTools);
			break;
		
		case Choice::NEW:
			// Create record.
			newRecord(fsTools);
			break;
		
		case Choice::DELETE:
			// Delete existing record.
			deleteRecord(fsTools);
			break;
		
		case Choice::INVALID:
		default:
			// Display error if user does not select valid choice.
			cerr << "Incorrect choice\n";
			break;
		}

		// Reset end-of-file indicator.
		fsTools.clear();
	}

	fsTools.close();
}
