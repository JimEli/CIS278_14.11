// tooldata.cpp
// Class ToolData stores tool's information.
#include <string>
#include "ToolData.h"

using namespace std;

// Default ToolData constructor.
ToolData::ToolData(int number, int quantity, const string& name, double cost)
	: toolNumber(number), toolQuantity(quantity), toolCost(cost)
{
	setToolName(name);
}

// Get/set record number.
int ToolData::getToolNumber() const { return toolNumber; }
void ToolData::setToolNumber(int number) 
{ 
	// Validate.
	if (number >= 0 && number <= 100)
		toolNumber = number;
	else
		toolNumber = 0;
}

// Get/set tool name.
string ToolData::getToolName() const { return string(toolName); }
void ToolData::setToolName(const string& name) {
	// Copy at most 20 characters from string to toolName.
	size_t length{ name.size() };
	length = (length < 20) ? length : 19;
	name.copy(toolName, length);
	// Append null character.
	toolName[length] = '\0';
}

// Get/set quantity.
int ToolData::getToolQuantity() const { return toolQuantity; }
void ToolData::setToolQuantity(int quantity) { toolQuantity = quantity; }

// Get/set balance value
double ToolData::getToolCost() const { return toolCost; }
void ToolData::setToolCost(double cost) { toolCost = cost; }
