/*************************************************************************
* Title: Numeric input validation
* File: input.cpp
* Author: James Eli
* Date: 12/29/2017
*
* Integer/float console input validation. Checks string input for valid
* characters, and range. Will repeat input prompting for specified
* number of attempts. Can be expanded to additional types. Invlid input
* throws an exception, all of which are caught inside the getNumber
* function.
*
* Notes:
*  (1) Requires C++11 or greater.
*  (2) Compiled with MS Visual Studio 2017 Community (v141), and/or
*      Eclipse GCC 5.3.0.
*  (2) Floating point input rejects scientific notation.
*
* Submitted in partial fulfillment of the requirements of PCC CIS-278.
*************************************************************************
* Change Log:
*   12/29/2017: Initial release. JME
*   01/22/2018: Add type_traits check, static_assert and constants. JME
*************************************************************************/
#ifndef NUMERIC_INPUT_VALIDATION_H
#define NUMERIC_INPUT_VALIDATION_H

#include <iostream>    // console input/output
#include <iomanip>     // setprecision
#include <string>      // string handling
#include <limits>      // numeric limits
#include <cassert>     // assertions
#include <type_traits> // is_arithmetic

using namespace std;

// Maximum input attempts.
const unsigned int MAX_INPUT_ATTEMPTS = { 3 };
// Maximum characters allowed in input string.
const size_t MAX_INPUT_SIZE = { 16 };

// Trim trailing ws from string.
static void trim(string& s)
{
	size_t p;

	if ((p = s.find_last_not_of(" \f\n\r\t\v")) != string::npos)
		s.erase(p + 1);
}

// Individual type validations.
template<typename T>
inline const bool validateNumber(T& t, const string s)
{
	return false; // Fail for types other than int, long & double (see below).
}

// Handle integer type.
template<>
inline const bool validateNumber<int>(int& i, const string s)
{
	string::size_type pos = 0;

	i = stoi(s, &pos);
	if (pos != s.length())
		throw runtime_error("Trailing characters");

	return true;
}

// Handle unsigned integer type.
template<>
inline const bool validateNumber<unsigned>(unsigned& i, const string s)
{
	string::size_type pos = 0;

	i = stoi(s, &pos);
	if (pos != s.length())
		throw runtime_error("Trailing characters");

	return true;
}

// Handle long type.
template<>
inline const bool validateNumber<long>(long& l, const string s)
{
	string::size_type pos = 0;

	l = stol(s, &pos);
	if (pos != s.length())
		throw runtime_error("Trailing characters");

	return true;
}

// Handle double type.
template<>
inline const bool validateNumber<double>(double& d, const string s)
{
	string::size_type pos = 0;

	d = stod(s, &pos);
	if (pos != s.length())
		throw runtime_error("Trailing characters");

	return true;
}

// Validate string characters and attempt conversion.
template<typename T>
static const bool isNumber(T& n, string& s)
{
	static_assert(is_arithmetic<T>::value, "T must be numeric");

	trim(s); // Remove trailing ws, leading ws is ignored by stoX functions.

	if (!s.empty())
	{
		string validChars(" +-1234567890");

		if (!numeric_limits<T>::is_integer)
			// Add decimal point for floats.
			validChars += ".";

		// Throw exception if invalid chars found.
		if (s.find_first_not_of(validChars) != string::npos)
			throw runtime_error("Invalid characters");

		// Attempt to validate number by type.
		if (validateNumber<T>(n, s))
			return true;
	}

	return false;
}

// Input loop.
template<
	typename T,
	typename = typename enable_if_t<is_arithmetic<T>::value, T>>
	const bool getNumber(const string prompt, T& n, const T min, const T max) noexcept
{
	assert(!prompt.empty());

	unsigned int attempts = MAX_INPUT_ATTEMPTS;

	while (attempts--)
	{
		// Temporary holds input.
		string buffer;

		// Prompt and get input.
		cout << prompt;
		getline(cin, buffer, '\n');

		try
		{
			// Limit buffer length to reasonable amount.
			if (buffer.size() > MAX_INPUT_SIZE)
				throw length_error("Exceeded max_input_size");

			if (isNumber<T>(n, buffer)) {
				// Check if value between min/max.
				if (n >= min && n <= max)
					return true;
				else
					throw out_of_range("Outside min/max");
			}
		}
		catch (out_of_range&)
		{
			cout << "Input outside range (" << fixed << setprecision(1) << min << " to " << max << ")" << endl;
		}
		catch (length_error&)
		{
			cout << "Too many characters input" << endl;
		}
		catch (...)
		{
			// all other exceptions caught here.
			cout << "Invalid input: " << buffer << endl;
		}
	}

	return false;
}

#endif
