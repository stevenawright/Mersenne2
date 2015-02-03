// PrimeList.h: interface for the PrimeList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRIMELIST_H__36AEF291_D368_4316_97C8_E34AB08A21BE__INCLUDED_)
#define AFX_PRIMELIST_H__36AEF291_D368_4316_97C8_E34AB08A21BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include	<vector>

class PrimeList  
{
	public:
		PrimeList(unsigned long		p_value = 100000);
		~PrimeList();

		bool
		is_prime(unsigned long		p_value);

	private:
		std::vector<long>			m_primes;
		unsigned long				m_max_known_prime;

};

#endif // !defined(AFX_PRIMELIST_H__36AEF291_D368_4316_97C8_E34AB08A21BE__INCLUDED_)
