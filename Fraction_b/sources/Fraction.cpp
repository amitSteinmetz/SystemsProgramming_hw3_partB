#include "Fraction.hpp"
#include <stdbool.h>
#include <stdexcept>
#include <limits>

namespace ariel
{
/* Help functions */
int findGCD(int num1, int num2) {
// If den is negative, flip the sign
if (num2 < 0) num2 *= -1;
else if (num1 < 0) num1 *= -1;

if (num2 == 0) { 
	return num1;
}
// Recursive algorithm
return findGCD(num2, num1 % num2);
}
void reduceFraction(int& numerator, int& denominator) {
// First, find the gcd, and then divide the numbers with it
int gcd = findGCD(numerator, denominator);
numerator /= gcd;
denominator /= gcd;
}
int findLCM(int num1, int num2)
{
// Using common formula for finding lcm
return (num1 * num2) / findGCD(num1, num2);
}

/* Ctors */
Fraction::Fraction(int numer, int denom)
{
	// Denom can't be zero
	if (denom != 0)
	{
		if (denom < 0)
		{
			denom *= -1;
			numer *= -1;
		}
		// Reduce the fraction
		reduceFraction(numer, denom);
		
		// Init the members
		nom = numer;
		den = denom;
	}

	else throw std::invalid_argument("Division by zero");
}
Fraction::Fraction(float value)
{
	// In order to take the 3 digits after the decimal point and add them to the fraction,
	// we multiply the value by 1000, and to keep the same value as before, we set denom to 1000
	int numerator = int(value * 1000);
	int denominator = 1000;

	// Reduce the fraction
	reduceFraction(numerator, denominator);

	// Init the
	nom = numerator;
	den = denominator;
}

/* Overloading operator methods */

// Postfix and prefix
Fraction Fraction::operator++(int postfix_sign)
{
	Fraction temp = *this;

	// Increasing nom by den, eqaul to increase the whole fraction by 1
	this->setNumerator(this->nom + this->den);
	return temp;
};
Fraction Fraction::operator--(int postfix_sign)
{
	Fraction temp = *this;

	// Subtracting nom by den, eqaul to subtract the whole fraction by 1
	this->setNumerator(this->nom - this->den);
	return temp;
};
Fraction& Fraction::operator--()
{
	Fraction& rfrac = *this;
	rfrac.nom -= rfrac.den;
	return rfrac;
};
Fraction& Fraction::operator++()
{
	Fraction& rfrac = *this;
	rfrac.nom += rfrac.den;
	return rfrac;
}

// Stream
ostream& operator<<(ostream& ost, const Fraction& frac)
{
	return ost << frac.nom << "/" << frac.den;
}
istream& operator>>(istream& ist, Fraction& frac)
{
	int numer, denom;

	// check if input is two integers
	if (!(ist >> numer >> denom)) throw std::runtime_error("Invalid input for Fraction");
	
	// Denom can't be zero
	if (denom == 0) throw std::runtime_error("Invalid input for Fraction");

	// If denom is negative, flip the sign to the nom
	if (denom < 0)
	{
		denom *= -1;
		numer *= -1;
	}

	frac.nom = numer;
	frac.den = denom;

	return ist;
}

// Add
Fraction operator+(const Fraction& frac1, const Fraction& frac2)
{
	// First, find the lcm, and then multiply the noms with lcm/den. After that we can add the noms
	int lcm = findLCM(frac1.den, frac2.den);
	int frac1_temp_nom = frac1.nom * (lcm/frac1.den);
	int frac2_temp_nom = frac2.nom * (lcm/frac2.den);

	// Check if the sum of the numerators will cause an integer overflow
	if (((long long)frac1_temp_nom + (long long)frac2_temp_nom  > std::numeric_limits<int>::max()) ||
	((long long)frac1_temp_nom + (long long)frac2_temp_nom < std::numeric_limits<int>::min())) {
		throw std::overflow_error("Overflow");
	}

	int sum = frac1_temp_nom + frac2_temp_nom;

	// Reduce the fraction
	reduceFraction(sum,lcm);

	return Fraction(sum, lcm);
}
Fraction operator+(const Fraction& frac, const float& flo)
{
	// Convert frac to float
	float frac_to_float =  static_cast<float>(frac.nom) / frac.den;

	// Round to 3 digits after the point
	frac_to_float = std::round(frac_to_float * 1000) / 1000;

	return std::round((frac_to_float + flo) * 1000) / 1000;
}
float operator+(const float& flo, const Fraction& frac)
{
	// Convert frac to float
	float frac_to_float =  static_cast<float>(frac.nom) / frac.den;

	// Round to 3 digits after the point
	frac_to_float = std::round(frac_to_float * 1000) / 1000;

	return flo + frac_to_float;
}

// Subtract
Fraction operator-(const Fraction& frac1, const Fraction& frac2)
{
	// First, find the lcm, and then multiply the noms with lcm/den. After that we can subtract the noms
	int lcm = findLCM(frac1.den, frac2.den);
	int frac1_temp_nom = frac1.nom * (lcm/frac1.den);
	int frac2_temp_nom = frac2.nom * (lcm/frac2.den);

	// Check if the subtraction of the numerators will cause an integer overflow
	if (((long long)frac1_temp_nom - (long long)frac2_temp_nom > std::numeric_limits<int>::max()) ||
	((long long)frac1_temp_nom - (long long)frac2_temp_nom < std::numeric_limits<int>::min())) {
		throw std::overflow_error("Overflow");
	}
		
	int subtract = frac1_temp_nom - frac2_temp_nom;

	// Reduce the fraction
	reduceFraction(subtract,lcm);

	return Fraction(subtract, lcm);
}
float operator-(const float& flo, const Fraction& frac)
{
	// Convert frac to float
	float frac_to_float =  static_cast<float>(frac.nom) / frac.den;

	// Round to 3 digits after the point
	frac_to_float = std::round(frac_to_float * 1000) / 1000;

	return flo - frac_to_float;
}

// Divide and multiply
Fraction operator/(const Fraction& frac1, const Fraction& frac2)
{
	// frac2 can't be zero
	if (frac2 == 0) throw std::runtime_error("Division by zero");

	// Divide is actually multiply with opposite fraction
	Fraction opposite(frac2.den,frac2.nom);

	return frac1*opposite;
}
Fraction operator*(const Fraction& frac1, const Fraction& frac2)
{
	/* Check if the multiplaction of the numerators will cause an integer overflow */
	if ((long long)frac1.nom * (long long)frac2.nom > std::numeric_limits<int>::max()) {
		throw std::overflow_error("Overflow");
	}
	else if ((long long)frac1.den * (long long)frac2.den > std::numeric_limits<int>::max()) {
		throw std::overflow_error("Overflow");
	}

	// Multiply nom with nom, den with den
	return Fraction(frac1.nom*frac2.nom,frac1.den*frac2.den);
}

// Comparison
bool operator>=(const Fraction& frac1, const Fraction& frac2)
{
	// First, find the lcm, and then multiply the noms with lcm/den. After that we can compare between the noms
	int lcm = findLCM(frac1.den, frac2.den);
	int frac1_newNom = frac1.nom * (lcm/frac1.den);
	int frac2_newNom = frac2.nom * (lcm/frac2.den);

	if (frac1_newNom >= frac2_newNom) return true;

	return false;
} 
bool operator>(const Fraction& frac1, const Fraction& frac2) 
{
	// First, find the lcm, and then multiply the noms with lcm/den. After that we can compare between the noms
	int lcm = findLCM(frac1.den, frac2.den);
	int frac1_newNom = frac1.nom * (lcm/frac1.den);
	int frac2_newNom = frac2.nom * (lcm/frac2.den);

	if (frac1_newNom > frac2_newNom) return true;

	return false;
} 
bool operator<=(const Fraction& frac1, const Fraction& frac2)
{
	// First, find the lcm, and then multiply the noms with lcm/den. After that we can compare between the noms
	int lcm = findLCM(frac1.den, frac2.den);
	int frac1_newNom = frac1.nom * (lcm/frac1.den);
	int frac2_newNom = frac2.nom * (lcm/frac2.den);

	if (frac1_newNom <= frac2_newNom) return true;

	return false;
} 
bool operator<(const Fraction& frac1, const Fraction& frac2)
{
	// First, find the lcm, and then multiply the noms with lcm/den. After that we can compare between the noms
	int lcm = findLCM(frac1.den, frac2.den);
	int frac1_newNom = frac1.nom * (lcm/frac1.den);
	int frac2_newNom = frac2.nom * (lcm/frac2.den);

	if (frac1_newNom < frac2_newNom) return true;

	return false;
} 
bool operator==(const Fraction& frac1, const Fraction& frac2)
{
	// First, find the lcm, and then multiply the noms with lcm/den. After that we can compare between the noms
	int lcm = findLCM(frac1.den, frac2.den);
	int frac1_newNom = frac1.nom * (lcm/frac1.den);
	int frac2_newNom = frac2.nom * (lcm/frac2.den);

	if (frac1_newNom == frac2_newNom) return true;

	return false;
} 

/* Setters and Getters */
void Fraction::setDenominator(int newDenominator)
{
	den = newDenominator;
}
int Fraction::getNumerator()
{
	return this->nom;
}
int Fraction::getDenominator()
{
	return this->den;
}
void Fraction::setNumerator(int newNumerator) {
		nom = newNumerator; 
	}
}
