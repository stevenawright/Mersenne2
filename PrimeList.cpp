// PrimeList.cpp: implementation of the PrimeList class.
//
//////////////////////////////////////////////////////////////////////

#include	"stdafx.h"
#include	"M_Test.h"
#include	"PrimeList.h"
#include	<math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PrimeList::PrimeList
(
	unsigned long					p_value
)
{
	m_primes.reserve(p_value);

	m_primes[0] = 2;
	m_primes[1] = 3;
	m_primes[2] = 5;

	size_t							index = 3;
	unsigned long					canidate = 7;

	while (canidate < p_value)
	{
		double						max_test = canidate;

		max_test = sqrt(max_test);

		unsigned long				max_int_to_test = max_test;
		bool						is_prime = true;

		{for (size_t i = 1; i <= max_int_to_test; ++i)
		{
			if (canidate % m_primes[i] == 0)
			{
				is_prime = false;
				break;
			}
		}}

		if (is_prime)
		{
			m_primes[index++] = canidate;
		}

		canidate += 4;

		max_test = canidate;

		max_test = sqrt(max_test);

		max_int_to_test = max_test;

		is_prime = true;

		{for (size_t i = 1; i <= max_int_to_test; ++i)
		{
			if (canidate % m_primes[i] == 0)
			{
				is_prime = false;
				break;
			}
		}}

		if (is_prime)
		{
			m_primes[index++] = canidate;
		}

		canidate += 2;
	}

	m_max_known_prime = m_primes[index - 1];

}	// C-TOR

PrimeList::~PrimeList()
{

}	// D-TOR

bool
PrimeList::is_prime
(
	unsigned long					p_value
)
{
	double						max_test = p_value;

	max_test = sqrt(max_test);

	unsigned long				max_int_to_test = max_test;

	if (max_int_to_test > m_max_known_prime)
	{
		return (false);
	}

	bool						is_prime = true;

	for (size_t i = 0; m_primes[i] <= max_int_to_test; ++i)
	{
		if (p_value % m_primes[i] == 0)
		{
			return(false);
		}
	}

	return(true);
	
}	// is_prime