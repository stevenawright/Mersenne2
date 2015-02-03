#ifndef BIGINTEGER_H_INCLUDED
#define	BIGINTEGER_H_INCLUDED

#include	<string>
#include	<vector>
//#include	<assert.h>
//#include	<iostream>

//#define	MAX_VALUE		9
//#define	OVERFLOW_VALUE	10

class BigInteger
{
	public:

		BigInteger(const long &p_l = 0);
		BigInteger(const std::string &p_s);
		~BigInteger();
		BigInteger(const BigInteger &);
		BigInteger& operator= (const BigInteger &);
		
		BigInteger&	operator++ ();
		BigInteger&	operator-- ();
		//BigInteger&	operator-  (const long);
		//BigInteger&	operator+  (const long);
		//BigInteger&	operator*  (const long);
		//BigInteger&	operator *= (const long);
		BigInteger&	operator<< (const unsigned long);
		BigInteger&	operator+= (const BigInteger &);
		BigInteger&	operator-= (const BigInteger &);
		BigInteger& operator%= (const BigInteger &);

		bool		operator>  (const BigInteger &) const;
		bool		operator>= (const BigInteger &) const;
		bool		operator== (const long) const;
		bool		operator!= (const long) const;
		bool		operator<  (const BigInteger &) const;
		bool		operator<= (const BigInteger &) const;

		void		square (bool	p_dbg = false);
		void		power2(UINT);
		BigInteger	modpower2(UINT);
		BigInteger	divpower2(UINT);
		void		sqmod(UINT);
		void		shiftleftplaces(const unsigned long);
		void		Mod_2_Pow_N_Minus_1(UINT);
		bool		is_valid(BigInteger	& p_bi);

		friend BigInteger 
			operator+ (const BigInteger &, const BigInteger &);
		friend BigInteger 
			operator- (const BigInteger &, const BigInteger &);
		friend BigInteger 
			operator* (const BigInteger &, const long);
		friend BigInteger 
			operator* (const BigInteger &, const BigInteger &);
		friend std::pair<BigInteger, BigInteger> 
			operator / (const BigInteger &, const BigInteger &);
		friend BigInteger 
			operator% (const BigInteger &, const BigInteger &);
		friend bool
			operator== (const BigInteger &, const BigInteger &);
		friend bool
			operator!= (const BigInteger &, const BigInteger &);
		friend std::ostream & 
			operator<< (std::ostream&, const BigInteger &);

	private:
		BigInteger & shiftleftbits(unsigned long			bit_shift);
		static long calc_bit_shift(unsigned long, unsigned long);

		std::vector<unsigned long>			values;		

};  // BigInteger

#endif	// #ifndef BIGINTEGER_H_INCLUDED
