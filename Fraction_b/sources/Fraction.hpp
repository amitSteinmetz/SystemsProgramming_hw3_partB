#pragma once
#include <iostream>
#include <cmath>

using namespace std;

namespace ariel { 

class Fraction
{
/* Attributes */
int nom; // short of "numerator"
int den; // short of "denominator"

/* Ctors */
public:
Fraction() : nom(0), den(0) {}
Fraction(int numer, int denom); 
Fraction(float value); // Ctor that get a float and convert it to a fraction

/* Methods */
int getNumerator();
int getDenominator();
void setNumerator(int newNumerator);
void setDenominator(int newDenominator);

friend void reduceFraction(int& numerator, int& denominator);
friend int findGCD(int num1, int num2);
friend int findLCM(int num1, int num2);

/* Operator loading methods */
friend ostream& operator<<(ostream& ost, const Fraction& frac);
friend istream& operator>>(istream& ist, Fraction& frac);

friend Fraction operator+(const Fraction& frac1, const Fraction& frac2);
friend Fraction operator-(const Fraction& frac1, const Fraction& frac2);
friend Fraction operator/(const Fraction& frac1, const Fraction& frac2);
friend Fraction operator*(const Fraction& frac1, const Fraction& frac2);

friend Fraction operator+(const Fraction& frac, const float& flo);
friend float operator+(const float& flo, const Fraction& frac);
friend float operator-(const float& flo, const Fraction& frac);

friend bool operator>=(const Fraction& frac1, const Fraction& frac2); 
friend bool operator>(const Fraction& frac1, const Fraction& frac2); 
friend bool operator<=(const Fraction& frac1, const Fraction& frac2); 
friend bool operator<(const Fraction& frac1, const Fraction& frac2); 
friend bool operator==(const Fraction& frac1, const Fraction& frac2); 

Fraction operator++(int postfix_sign);
Fraction operator--(int postfix_sign);
Fraction& operator--();
Fraction& operator++();
};  

}



