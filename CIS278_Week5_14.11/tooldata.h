// tooldata.h
// Class ToolData definition.
#ifndef TOOLDATA_H
#define TOOLDATA_H

#include <iostream>
#include <iomanip>

using namespace std;

class ToolData {

public:
	// Default ToolData constructor.
	ToolData(int = 0, int = 0, const std::string& = "", double = 0.0);

	// Accessor functions for tool number.
	void setToolNumber(int);
	int getToolNumber() const;

	// Accessor functions for quantity.
	void setToolQuantity(int);
	int getToolQuantity() const;

	// Accessor functions for tool name.
	void setToolName(const string&);
	std::string getToolName() const;

	// Accessor functions for cost.
	void setToolCost(double);
	double getToolCost() const;

	// Print formatted tool record. 
	friend ostream& operator<< (ostream& os, const ToolData& tool)
	{
		return os << left << setw(13) << tool.getToolNumber()
			<< setw(22) << tool.getToolName()
			<< setw(3) << tool.getToolQuantity()
			<< setw(14) << right << fixed << showpoint << setprecision(2) << tool.getToolCost()
			<< endl;
	}

private:
	int toolNumber;
	char toolName[20];
	int toolQuantity;
	double toolCost;

};

#endif
