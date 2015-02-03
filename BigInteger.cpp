#include	"stdafx.h"
//#include	<afxwin.h>

#include	<string>
#include	<vector>
#include	<assert.h>
#include	<fstream>

#include	"BigInteger.h"

using namespace std;

#define	DBG_COPY_CTOR					0
#define	DBG_OPERATOR_EQ					0
#define	DBG_OPERATOR_MINUS_MINUS		0
#define	DBG_OPERATOR_MINUS				0
#define	DBG_MODPOWER2					0
#define	DBG_DIVPOWER2					0
#define	DBG_SQUARE						0
#define	DBG_POWER2						0
#define	DBG_OPERATOR_MULTIPLY			0
#define	DBG_OPERATOR_SHIFT_LEFT_BITS	0
#define	DBG_OPERATOR_DIV				0
#define	DBG_SQMOD						0

extern ofstream	dout;
//
//	NUMBER_OF_PLACES is the power of two in each place of the array.
//	If NUMBER_OF_PLACES is 4, then we are working in base 16.  OVERFLOW_VALUE MUST
//	reflect this value.
//	OVERFLOW_VALUE MUST be a power of two for the (mod/div)power2 to work.
//	It it assumed that all numbers are positive.  The numbers in the array are all 
//	unsigned.  NUMBER_OF_PLACES MUST BE no greater than half of the number that it
//	represents (e.g. sizeof(unsigned long) = 32, therefore NUMBER_OF_PLACES <= 16.
//	This is necessary when we square the value.
//
#define	NUMBER_OF_PLACES	16
#define	OVERFLOW_VALUE		65536
#define	MAX_VALUE			OVERFLOW_VALUE - 1

BigInteger::BigInteger(
	const long						& p_l
)
{
	values.push_back(p_l);
};  // C-TOR

BigInteger::BigInteger(
   const std::string				& p_s
)
{
}  // C-TOR

BigInteger::~BigInteger()
{
}  // D-TOR

BigInteger::BigInteger
(
	const BigInteger				& p_big_integer
)
{
	#if(DBG_COPY_CTOR > 0)
	dout << "Enter COPY-TOR with " << p_big_integer << endl;
	#endif

	for (int i = 0; i != p_big_integer.values.size(); ++i)
	{
		values.push_back(p_big_integer.values[i]);
	}

	#if(DBG_COPY_CTOR > 0)
	dout << "	COPY-TOR " << *this << " size " << values.size() << endl;
	#endif

	for (int j = values.size() -1; j != 0; --j)
	{
		if (values[j] == 0)
		{
			values.pop_back();
		}
		else
		{
			break;
		}
	}

	#if(DBG_COPY_CTOR > 0)
	dout <<	"Exit COPY-TOR " << *this << endl;
	#endif

}  // COPY-TOR

BigInteger&
BigInteger::operator=
(
	const BigInteger				& p_big_integer
)
{
	#if(DBG_OPERATOR_EQ > 0)
	dout << "Enter operator= " << p_big_integer << " and this " << *this << endl;	
	#endif

	values[0] = p_big_integer.values[0];

	#if(DBG_OPERATOR_EQ > 0)
	dout <<	"	operator= " << *this << endl;
	#endif

	for (int i = 1; i != p_big_integer.values.size(); ++i)
	{
		if (values.size() == i)
		{
			values.push_back(p_big_integer.values[i]);
		}
		else
		{
			values[i] = p_big_integer.values[i];
		}
	}

	#if(DBG_OPERATOR_EQ > 0)
	dout << "	operator= " << *this << " values.size and i " << values.size() << " " << i << endl;
	#endif

	for (int j = values.size() -1; j >= i; --j)
	{
		values.pop_back();
	}
	#if(DBG_OPERATOR_EQ > 0)
	dout << "	operator= " << *this << " size " << values.size() << endl;
	#endif
	//
	// remove leading 0s
	//
	for (int k = values.size() -1; k != 0; --k)
	{
		if (values[k] == 0)
		{
			values.pop_back();
		}
		else
		{
			break;
		}
	}
	#if(DBG_OPERATOR_EQ > 0)
	dout << "Exit operator= " << *this << " size " << values.size() << endl;
	#endif

	return(*this);

}  // ASSIGN-TOR

BigInteger&
BigInteger::operator++()
{
	int								position = 0;
	bool							carry = false;

	values[0] += 1;
	if (values[0] > MAX_VALUE)
	{
		values[0] = 0;
		carry = true;
	}
	++position;

	while (carry)
	{
		carry = false;

		if (values.size() > position)
		{
			values[position] += 1;

			if (values[position] > MAX_VALUE)
			{
				values[position] = 0;
				carry = true;
			}
		}
		else
		{
			values.push_back(1);
		}

		++position;
	}

	return (*this);

}  // operator++


BigInteger&
BigInteger::operator+=(
	const BigInteger &	p_bi
)
{
	int								position = 0;
	int								carry = 0;
	int								cycles = max(values.size(), p_bi.values.size());
	//BigInteger						big_int;


	long							t_l = values[position];
	long							t_r = p_bi.values[position];

	if ((t_l + t_r) > MAX_VALUE)
	{
		values[0] = t_l + t_r - OVERFLOW_VALUE;
		carry = 1;
	}
	else
	{
		values[0] = t_l + t_r;
	}
	++position;
	--cycles;

	#if(DBG_OPERATOR_PLUS > 0)
	dout << "	operator + carry = " << carry << endl;
	#endif

	while ((carry > 0) || (cycles > 0))
	{
		t_l = t_r = 0;

		if (values.size() > position)
		{
			t_l = values[position];
		}
		if (p_bi.values.size() > position)
		{
			t_r = p_bi.values[position];
		}

		long						t = t_l + t_r;

		#if(DBG_OPERATOR_PLUS > 0)
		dout << "	operator + t_l " << t_l << " t_r " << t_r << endl;
		dout << "	operator + t + carry " << (t + carry) << endl;
		#endif

		if ((t + carry) > MAX_VALUE)
		{
			values[t_l + t_r - OVERFLOW_VALUE + carry];
			carry = 1;
		}
		else
		{
			values[t_l + t_r + carry];
			carry = 0;
		}

		++position;
		--cycles;
	}

	#if(DBG_OPERATOR_PLUS > 0)
	dout << "Exit operator + " << big_int << endl;
	#endif

	return(*this);

//	int			cycles = max(values.size(), p_bi.values.size());
//	bool		carry = false;

	values[0] = values[0] + p_bi.values[0];
	if (values[0] > MAX_VALUE)
	{
		values[0] -= OVERFLOW_VALUE;
		carry = true;
	}
	for (int i = 1; i != cycles; ++i)
	{
		values.push_back(values[i] + p_bi.values[i] + carry?1:0);
		carry = false;
		if (values[i] > MAX_VALUE)
		{
			values[i] -= OVERFLOW_VALUE;
			carry = true;
		}
	}
	if (carry)
	{
		values.push_back(1);
	}

	return (*this);

}  // operator += BigInteger


/*
BigInteger
BigInteger::operator+(
	const long p_l
)
{
	int			position = 0;
	bool		carry = false;

	values[0] += p_l;
	if (values[0] > MAX_VALUE)
	{
		values[0] -= OVERFLOW_VALUE;
		carry = true;
	}
	++position;

	while (carry)
	{
		carry = false;

		if (values.size() > position)
		{
			values[position] += 1;

			if (values[position] > MAX_VALUE)
			{
				values[position] = 0;
				carry = true;
			}
		}
		else
		{
			values.push_back(1);
		}

		++position;
	}

	return (*this);

}  // operator++
*/

BigInteger&
BigInteger::operator--()
{
	int								position = 0;
	bool							borrow = false;

	#if(DBG_OPERATOR_MINUS_MINUS > 0)
	dout << " Enter operator-- " << *this << endl;
	#endif

	if (values[0] == 0)
	{
		#if(DBG_OPERATOR_MINUS_MINUS > 2)
		dout << " values[0] < 0 ==> " << MAX_VALUE << endl;
		#endif

		values[0] = MAX_VALUE;
		borrow = true;
	}
	else
	{
		values[0] -= 1;
	}
	#if(DBG_OPERATOR_MINUS_MINUS > 2)
	dout << " values[0] -= 1 ==> " << values[0] << endl;
	#endif
	
	++position;

	while (borrow)
	{
		borrow = false;

		#if(DBG_OPERATOR_MINUS_MINUS > 2)
		dout << "while borrow value.size() " << values.size() << " position " << position << endl;
		dout << "	values[position] " << values[position] << endl;
		#endif

		assert(values.size() > position);

		if (values[position] == 0)
		{
			#if(DBG_OPERATOR_MINUS_MINUS > 2)
			dout << " values[position] < 0 ==> " << MAX_VALUE << endl;
			#endif

			values[position] = MAX_VALUE;
			borrow = true;
		}
		else
		{
			values[position] -= 1;
		}
		//values[position] -= 1;
		#if(DBG_OPERATOR_MINUS_MINUS > 2)
		dout << "new values[pos] " << values[position] << endl;
		#endif

		//if (values[position] < 0)
		//{
		//	#if(DBG_OPERATOR_MINUS_MINUS > 2)
		//	dout << " values[position] < 0 ==> " << MAX_VALUE << endl;
		//	#endif
		//	values[position] = MAX_VALUE;
		//	borrow = true;
		//}

		++position;
	}

	#if(DBG_OPERATOR_MINUS_MINUS > 0)
	dout << "Exit operator-- " << *this << endl;
	#endif

	return (*this);

}  // operator--

BigInteger
operator-(
	const BigInteger				& p_l,
	const BigInteger				& p_r
)
{
	int								borrow = 0;
	BigInteger						big_integer;

	#if(DBG_OPERATOR_MINUS > 0)
	//dout << "Enter operator- values.size() = " << values.size() << " p_bi.size() " << p_bi.values.size() << endl;
	dout << "Enter operator- " << p_l << " minus " << p_r << endl;
	#endif

	assert (p_l.values.size() >= p_r.values.size());
	assert (p_l >= p_r);

	if (p_l.values[0] < p_r.values[0])
	{
		big_integer.values[0] = p_l.values[0] + OVERFLOW_VALUE - p_r.values[0];
		borrow = 1;
	}
	else
	{
		big_integer.values[0] = p_l.values[0] - p_r.values[0];
	}
	//if (big_integer.values[0] < 0)
	//{
	//	big_integer.values[0] += OVERFLOW_VALUE;
	//	borrow = 1;
	//}
	#if(DBG_OPERATOR_MINUS > 0)
	dout << "	operator- " << big_integer << endl;
	#endif

	int								i = 1;

	for ( ; i != p_r.values.size(); ++i)
	{
		if (p_l.values[i] < p_r.values[i])
		{
			big_integer.values.push_back(p_l.values[i] + OVERFLOW_VALUE - p_r.values[i] - borrow);
			borrow = 1;
			if (big_integer.values[i] > MAX_VALUE)
			{
				dout << " operator - [i] > MAX_VALUE IF" << endl;
				dout << "	p_l.val[i] " << p_l.values[i] << endl;
				dout << "	p_r.val[i] " << p_r.values[i] << endl;
				dout << "	borrow     " << borrow << endl;
			}
		}
		else if	((p_l.values[i] == p_r.values[i]) && (borrow == 1))
		{
			big_integer.values.push_back(MAX_VALUE);
			borrow = 1;
		}
		else
		{
			big_integer.values.push_back(p_l.values[i] - p_r.values[i] - borrow);
			if (big_integer.values[i] > MAX_VALUE)
			{
				dout << " operator - [i] > MAX_VALUE ELSE" << endl;
				dout << "	p_l.val[i] " << p_l.values[i] << endl;
				dout << "	p_r.val[i] " << p_r.values[i] << endl;
				dout << "	borrow     " << borrow << endl;
			}
			borrow = 0;
		}
		//if (big_integer.values[i] < 0)
		//{
		//	big_integer.values[i] += OVERFLOW_VALUE;
		//	borrow = 1;
		//}
		#if(DBG_OPERATOR_MINUS > 2)
		if (!big_integer.is_valid(big_integer))
		{
			dout << "	operator - big_integer, Invalid data, Exit " << big_integer << endl;
			return (0);
		}
		#endif
	}
	#if(DBG_OPERATOR_MINUS > 0)
	dout << "	operator- " << big_integer << endl;
	#endif

	while ((borrow > 0) || i < p_l.values.size())
	{
		if (p_l.values[i] == 0)
		{
			big_integer.values.push_back(OVERFLOW_VALUE);
			borrow = 1;
		}
		else
		{
			big_integer.values.push_back(p_l.values[i] - borrow);
			borrow = 0;
		}
		#if(DBG_OPERATOR_MINUS > 2)
		if (!big_integer.is_valid(big_integer))
		{
			dout << "	operator - big_integer, while borrow Invalid data, Exit " << big_integer << endl;
			return (0);
		}
		#endif
		//big_integer.values.push_back(p_l.values[i] - borrow);
		//borrow = 0;
		//if (big_integer.values[i] < 0)
		//{
		//	big_integer.values[i] += OVERFLOW_VALUE;
		//	borrow = 1;
		//}
		++i;
	}

	#if(DBG_OPERATOR_MINUS > 0)
	dout << "Exit operator- " << big_integer << endl;
	#endif

	#if(DBG_OPERATOR_MINUS > 2)
	if (!big_integer.is_valid(big_integer))
	{
		dout << "	Exit operator - big_integer, Invalid data, Exit " << big_integer << endl;
		return (0);
	}
	#endif

	return (big_integer);

}  // operator - BigInteger

BigInteger&
BigInteger::operator-=
(
	const BigInteger				& p_r
)
{
	int								borrow = 0;

	#if(DBG_OPERATOR_MINUS > 0)
	//dout << "Enter operator- values.size() = " << values.size() << " p_bi.size() " << p_bi.values.size() << endl;
	dout << "Enter operator- " << *this << " minus " << p_r << endl;
	#endif

	assert (values.size() >= p_r.values.size());
	assert (*this >= p_r);

	if (values[0] < p_r.values[0])
	{
		values[0] = values[0] + OVERFLOW_VALUE - p_r.values[0];
		borrow = 1;
	}
	else
	{
		values[0] = values[0] - p_r.values[0];
	}

	#if(DBG_OPERATOR_MINUS > 0)
	dout << "	operator- " << *this << endl;
	#endif

	int								i = 1;

	for ( ; i != p_r.values.size(); ++i)
	{
		if (values[i] < p_r.values[i])
		{
			values[i] = values[i] + OVERFLOW_VALUE - p_r.values[i] - borrow;
			borrow = 1;
			if (values[i] > MAX_VALUE)
			{
				dout << " operator - [i] > MAX_VALUE IF" << endl;
				dout << "	p_l.val[i] " << values[i] << endl;
				dout << "	p_r.val[i] " << p_r.values[i] << endl;
				dout << "	borrow     " << borrow << endl;
			}
		}
		else if	((values[i] == p_r.values[i]) && (borrow == 1))
		{
			values[i] = MAX_VALUE;
			borrow = 1;
		}
		else
		{
			values[i] = values[i] - p_r.values[i] - borrow;
			if (values[i] > MAX_VALUE)
			{
				dout << " operator - [i] > MAX_VALUE ELSE" << endl;
				dout << "	p_l.val[i] " << values[i] << endl;
				dout << "	p_r.val[i] " << p_r.values[i] << endl;
				dout << "	borrow     " << borrow << endl;
			}
			borrow = 0;
		}
		//if (big_integer.values[i] < 0)
		//{
		//	big_integer.values[i] += OVERFLOW_VALUE;
		//	borrow = 1;
		//}
		#if(DBG_OPERATOR_MINUS > 2)
		if (!is_valid(*this))
		{
			dout << "	operator - big_integer, Invalid data, Exit " << *this << endl;
			return (0);
		}
		#endif
	}
	#if(DBG_OPERATOR_MINUS > 0)
	dout << "	operator- " << *this << endl;
	#endif

	while ((borrow > 0) || i < values.size())
	{
		if (values[i] == 0)
		{
			values[i] = OVERFLOW_VALUE;
			borrow = 1;
		}
		else
		{
			values[i] = values[i] - borrow;
			borrow = 0;
		}
		#if(DBG_OPERATOR_MINUS > 2)
		if (!is_valid(*this))
		{
			dout << "	operator - big_integer, while borrow Invalid data, Exit " << *this << endl;
			return (0);
		}
		#endif
		//big_integer.values.push_back(p_l.values[i] - borrow);
		//borrow = 0;
		//if (big_integer.values[i] < 0)
		//{
		//	big_integer.values[i] += OVERFLOW_VALUE;
		//	borrow = 1;
		//}
		++i;
	}

	#if(DBG_OPERATOR_MINUS > 0)
	dout << "Exit operator- " << *this << endl;
	#endif

	#if(DBG_OPERATOR_MINUS > 2)
	if (!is_valid(*this))
	{
		dout << "	Exit operator - big_integer, Invalid data, Exit " << *this << endl;
		return (0);
	}
	#endif

	return (*this);

}  // operator - BigInteger

/*
	int			position = 0;
	bool		borrow = false;

	values[0] -= p_l;
	if (values[0] < 0)
	{
		values[0] += OVERFLOW_VALUE;
		borrow = true;
	}
	++position;

	while (borrow)
	{
		borrow = false;

		assert(values.size() > position);
		values[position] -= 1;

		if (values[position] < 0)
		{
			values[position] = MAX_VALUE;
			borrow = true;
		}

		++position;
	}

	return (*this);

}  // operator--
*/

BigInteger
operator+ (
	const BigInteger				& p_l,
	const BigInteger				& p_r
)
{

	#if(DBG_OPERATOR_PLUS > 0)
	dout << "Enter operator + " << p_l << " plus " << p_r << endl;
	#endif
	int								position = 0;
	int								carry = 0;
	int								cycles = max(p_l.values.size(), p_r.values.size());
	BigInteger						big_int;


	long							t_l = p_l.values[position];
	long							t_r = p_r.values[position];

	if ((t_l + t_r) > MAX_VALUE)
	{
		big_int.values[0] = t_l + t_r - OVERFLOW_VALUE;
		carry = 1;
	}
	else
	{
		big_int.values[0] = t_l + t_r;
	}
	++position;
	--cycles;

	#if(DBG_OPERATOR_PLUS > 0)
	dout << "	operator + carry = " << carry << endl;
	#endif

	while ((carry > 0) || (cycles > 0))
	{
		t_l = t_r = 0;

		if (p_l.values.size() > position)
		{
			t_l = p_l.values[position];
		}
		if (p_r.values.size() > position)
		{
			t_r = p_r.values[position];
		}

		long						t = t_l + t_r;

		#if(DBG_OPERATOR_PLUS > 0)
		dout << "	operator + t_l " << t_l << " t_r " << t_r << endl;
		dout << "	operator + t + carry " << (t + carry) << endl;
		#endif

		if ((t + carry) > MAX_VALUE)
		{
			big_int.values.push_back(t_l + t_r - OVERFLOW_VALUE + carry);
			carry = 1;
		}
		else
		{
			big_int.values.push_back(t_l + t_r + carry);
			carry = 0;
		}

		++position;
		--cycles;
	}

	#if(DBG_OPERATOR_PLUS > 0)
	dout << "Exit operator + " << big_int << endl;
	#endif

	return(big_int);

}  // friend operator +

BigInteger
operator*
(
	const BigInteger				& p_big_integer,
	const long						p_long
)
{
	#if(DBG_OPERATOR_MULTIPLY > 0)
	dout << "Enter operator* friend p_long with " << p_long << " * " << p_big_integer << endl;
	#endif
	BigInteger						big_integer = p_big_integer;
	long							carry = 0;

	for (int i = 0; i != big_integer.values.size(); ++i)
	{
		#if(DBG_OPERATOR_MULTIPLY > 2)
		dout << "	Top of FOR, i " << i << ", size " << big_integer.values.size() << endl;
		dout << "	big[i] == " << big_integer.values[i] << "carry " << carry << endl;
		#endif

		big_integer.values[i] = big_integer.values[i] * p_long + carry;

		#if(DBG_OPERATOR_MULTIPLY > 2)
		dout << "	big[i] now == " << big_integer.values[i] << endl;
		#endif

		carry = 0;
		if (big_integer.values[i] > MAX_VALUE)
		{
			carry = big_integer.values[i] / OVERFLOW_VALUE;
			big_integer.values[i] %= OVERFLOW_VALUE;
			#if(DBG_OPERATOR_MULTIPLY > 2)
			dout << "		> MAX_VALUE" << endl;
			dout << "		big[i] now == " << big_integer.values[i] << endl;
			dout << "		carry == " << carry << endl;
			#endif
		}
	}
	if (carry > 0)
	{
		big_integer.values.push_back(carry);
	}
	
	#if(DBG_OPERATOR_MULTIPLY > 0)
	dout << "Exit operator*(long with value of " << big_integer << endl;
	#endif

	return(big_integer);

}  // operator*

/*
BigInteger&
BigInteger::operator*(
	const long	p_l
)
{

dout << "Enter operator*(long with " << p_l << " * " << *this << endl;
	long		carry = 0;

	for (int i = 0; i != values.size(); ++i)
	{
		values[i] *= p_l + carry;
		carry = 0;
		if (values[i] > MAX_VALUE)
		{
			carry = values[i] / OVERFLOW_VALUE;
			values[i] %= OVERFLOW_VALUE;
		}
	}
	if (carry > 0)
	{
		values.push_back(carry);
	}
	
dout << "Exit operator*(long with *this " << *this << endl;

	return(*this);

}  // operator* (long)
*/

/*
BigInteger&
BigInteger::operator*=(
	const long	p_l
)
{

dout << "Enter BigInteger::operator*= (long of " << p_l << " and size of " << values.size() << endl;
dout << " *this is " << *this << endl;

	long		carry = 0;

	for (int i = 0; i != values.size(); ++i)
	{
		values[i] *= p_l + carry;
dout << "operator*= values[i] " << values[i] << " p_l " << p_l << " carry " << carry << endl;
		carry = 0;
		if (values[i] > MAX_VALUE)
		{
dout << " v > MAX_VALUE " << MAX_VALUE << " OVERFLOW_VALUE " << OVERFLOW_VALUE << endl;
			carry = values[i] / MAX_VALUE;
			values[i] %= OVERFLOW_VALUE;
		}
	}
	if (carry > 0)
	{
dout << "carry > 0 " << endl;

		values.push_back(carry);
	}
	
dout << "Exit operator*=(long with *this " << *this << endl;
	return(*this);

}  // operator* (long)
*/

#if(0)
BigInteger&
BigInteger::operator << 
(
	const int						p_i
)
{
	#if(DBG_OPERATOR_SHIFT_LEFT > 0)
	dout << "Enter BigInteger::operator<< with p_i " << p_i  << " *this " << *this << endl;
	#endif

	if (p_i == 0)
	{
		return(*this);
	}
	if ((values.size() == 1) && (values[0] == 0))
	{
		return (*this);
	}

	BigInteger						big_int;

	for (int i = 1; i < p_i; ++i)
	{
		#if(DBG_OPERATOR_SHIFT_LEFT > 0)
		dout << "shifting left" << endl;
		#endif

		big_int.values.push_back(0);
	}

	for (int j = 0; j != values.size(); ++j)
	{
		big_int.values.push_back(values[j]);
	}

	#if(DBG_OPERATOR_SHIFT_LEFT > 0)
	dout << "Exit BigInteger::operator << with big_int " << big_int << endl;
	#endif

	*this = big_int;
	return (*this);

}  // operator << shift
#endif

//BigInteger&
void
BigInteger::shiftleftplaces 
(
	const unsigned long				p_i
)
{
	#if(DBG_SHIFT_LEFT_PLACES > 0)
	dout << "Enter BigInteger::operator<< with p_i " << p_i  << " *this " << *this << endl;
	#endif

	if (p_i == 0)
	{
		return;
	}
	if ((values.size() == 1) && (values[0] == 0))
	{
		return;
	}

	BigInteger						big_int;

	for (int i = 1; i < p_i; ++i)
	{
		#if(DBG_SHIFT_LEFT_PLACES > 0)
		dout << "shifting left" << endl;
		#endif

		big_int.values.push_back(0);
	}

	for (int j = 0; j != values.size(); ++j)
	{
		big_int.values.push_back(values[j]);
	}

	#if(DBG_SHIFT_LEFT_PLACES > 0)
	dout << "Exit BigInteger::operator << with big_int " << big_int << endl;
	#endif

	*this = big_int;

	//return (*this);

}  // shiftleftplaces

BigInteger&
BigInteger::operator << 
(
	unsigned long					p_shift
)
{
	#if(DBG_OPERATOR_SHIFT_LEFT_BITS > 0)
	dout << "Enter BigInteger::shiftleftbits with p_i " << p_shift  << " *this " << *this << endl;
	#endif

	if (p_shift == 0)
	{
		return(*this);
	}
	if ((values.size() == 1) && (values[0] == 0))
	{
		return (*this);
	}

	unsigned long					place_bits = p_shift / NUMBER_OF_PLACES;
	unsigned long					bit_shifts = p_shift % NUMBER_OF_PLACES;
	unsigned long					overrun = 0;
	BigInteger						big_int(this->values.size() + place_bits);

	for (unsigned long i = 0; i < values.size() - 1; ++i)
	{
		unsigned long				value = values[i];
		unsigned long				new_overrun = value << bit_shifts;

		if (new_overrun >= OVERFLOW_VALUE)
		{
			new_overrun >>= NUMBER_OF_PLACES;
		}

		value &= MAX_VALUE;
		value += overrun;
		overrun = new_overrun;
		
		big_int.values[i+place_bits] = value;

		#if(DBG_OPERATOR_SHIFT_LEFT_BITS > 1)
		dout << "values[i] " << values[i] << ", overrun " << overrun << endl;
		#endif
	}

	if (overrun > 0)
	{
		big_int.values.push_back(overrun);
	}

	#if(DBG_OPERATOR_SHIFT_LEFT_BITS > 0)
	dout << "Exit BigInteger::shiftleftbits result " << big_int << endl;
	#endif

	*this = big_int;

	return (*this);

}  // shiftleftbits

bool
BigInteger::operator==(
	const long						p_l
)
const
{
	
	return ((values.size() == 1) && (values[0] == p_l));

}  // operator == (long)

bool
BigInteger::operator!=(
	const long						p_l
)
const
{
	return (!(*this == p_l));

}  // operator != (long)

bool
operator==(
	const BigInteger				& p_l,
	const BigInteger				& p_r
)
{
	
	if (p_l.values.size() != p_r.values.size())
	{
		return (false);
	}

	for (size_t i = 0; i != p_l.values.size(); ++i)
	{
		if (p_l.values[i] != p_r.values[i])
		{
			return (false);
		}
	}

	return (true);

}  // operator == (BigInteger, BigInteger)

bool
operator!=(
	const BigInteger				& p_l,
	const BigInteger				& p_r
)
{
	return (!(p_l == p_r));

}  // operator != (BigInteger, BigInteger)

BigInteger
operator* (
	const BigInteger				& p_l,
	const BigInteger				& p_r
)
{

	BigInteger						big_int = 0;

	#if(DBG_OPERATOR_MULTIPLY > 0)
	dout << "Enter BigInteger::operator* == " << p_l << " times " << p_r << endl;
	#endif

	for (unsigned long i = 0; i != p_r.values.size(); ++i)
	{
		#if(DBG_OPERATOR_SHIFT_LEFT > 0)
		dout << " operator* , i = " << i << " big_int = " << big_int << endl;
		#endif

		BigInteger					big_int_t;

		big_int_t = p_l * p_r.values[i];

		#if(DBG_OPERATOR_SHIFT_LEFT > 0)
		dout << " operator* big_int_t " << big_int_t << " p_l " << p_l << " times " << p_r.values[i] << endl;
		#endif

		//big_int_t << i;
		big_int_t.shiftleftplaces(i);

		#if(DBG_OPERATOR_SHIFT_LEFT > 0)
		dout << " operator* big_int_t after shift by i " << big_int_t << endl;
		#endif

		big_int = big_int + big_int_t;

		#if(DBG_OPERATOR_SHIFT_LEFT > 0)
		dout << " operator* big_int = " << big_int << endl;
		#endif
	}

	#if(DBG_OPERATOR_SHIFT_LEFT > 0)
	dout << "Exit BigInteger::operator* == " << big_int << endl;
	#endif
	
	return (big_int);

}  // operator*

void
BigInteger::square 
(
	bool							p_dbg
)
{

	BigInteger						big_int = 0;

	#if(DBG_SQUARE > 0)
	if (p_dbg) dout << "Enter BigInteger::square == " << *this << " size of " << values.size() << endl;
	if (!is_valid(*this))
	{
		dout << "	Square, Invalid data, Exit" << endl;
		*this = 0;
		return;
	}
	#endif

	for (unsigned long i = 0; i != values.size(); ++i)
	{
		#if(DBG_SQUARE > 1)
		if (p_dbg) dout << " square , i = " << i << " values.size() = " << values.size() << endl;
		#endif

		BigInteger					big_int_t;

		big_int_t = (*this) * values[i];

		#if(DBG_SQUARE > 2)
		if (p_dbg) dout << " square big_int_t " << big_int_t << " (*this) " << *this << " values[i] " << values[i] << endl;
		if (!is_valid(big_int_t))
		{
			dout << "	Square, Invalid data, Exit" << endl;
			*this = 0;
			return;
		}
		#endif
		
		//big_int_t << i;
		big_int_t.shiftleftplaces(i);

		#if(DBG_SQUARE > 2)
		if (p_dbg) dout << " square big_int_t after shift by i " << big_int_t << endl;
		if (!is_valid(big_int_t))
		{
			dout << "	Square, Invalid data, Exit" << endl;
			*this = 0;
			return;
		}
		#endif
		
		big_int = big_int + big_int_t;

		#if(DBG_SQUARE > 1)
		if (p_dbg) dout << " square big_int = " << big_int << " size() = " << big_int.values.size() << endl;
		if (!is_valid(big_int))
		{
			dout << "	Square, Invalid data, Exit" << endl;
			*this = 0;
			return;
		}
		#endif
	}

	*this = big_int;

	#if(DBG_SQUARE > 0)
	if (p_dbg) dout << "Exit BigInteger::square == " << *this << endl;
	if (!is_valid(*this))
	{
		dout << "	Square, Invalid data, Exit" << endl;
		*this = 0;
		return;
	}
	#endif

}  // square

void
BigInteger::power2(
	UINT							p_power
)
{
	#if(DBG_POWER2 > 0)
	dout << "BigInteger::power2, Enter, with p_power of " << p_power << endl;
	#endif
	ULONG							place_holders;
	ULONG							bit_holders;

	place_holders = p_power / NUMBER_OF_PLACES;
	bit_holders = p_power % NUMBER_OF_PLACES;

	#if(DBG_POWER2 > 0)
	dout << "BigInteger::power place_holders = " << place_holders << endl;
	dout << "BigInteger::power bit_holders = " << bit_holders << endl;
	#endif
	BigInteger						t = 1 << bit_holders;

	#if(DBG_POWER2 > 0)
	dout << "BigInteger::power, t = " << t << endl;
	#endif

	//t << place_holders;
	t.shiftleftplaces(place_holders);

	#if(DBG_POWER2 > 0)
	dout << "BigIntger::power, after t << place_holders = " << t << endl;
	#endif

	*this = t;

}  // power2

BigInteger
BigInteger::modpower2(
	UINT							p_power
)
{
	#if(DBG_MODPOWER2 > 0)
	dout << "Enter BigIntger::modpower2, p_power = " << p_power << endl;
	#endif

	ULONG							place_holders = p_power / NUMBER_OF_PLACES;
	ULONG							bit_holders = p_power % NUMBER_OF_PLACES;

	#if(DBG_MODPOWER2 > 0)
	dout << "BigIntger::modpower2, place_holders = " << place_holders << endl;
	dout << "BigIntger::modpower2, bit_holders = " << bit_holders << endl;
	#endif

	if (this->values.size() < (place_holders + 1))
	{
		return(*this);
	}

	BigInteger						t;

	t.values[0] = this->values[0];
	for (ULONG i = 1; i <= place_holders; ++i)
	{
		t.values.push_back(this->values[i]);
	}

	#if(DBG_MODPOWER2 > 0)
	dout << "BigIntger::modpower2, t = " << t << endl;
	#endif

	ULONG							mask = ~(~0 << bit_holders);

	#if(DBG_MODPOWER2 > 0)
	dout << "BigIntger::modpower2, mask = " << mask << endl;
	#endif

	t.values[place_holders] = this->values[place_holders] & mask;

	#if(DBG_MODPOWER2 > 0)
	dout << "Exit, BigIntger::modpower2, t = " << t << endl;
	#endif

	return(t);

}  // modpower2

BigInteger
BigInteger::divpower2(
	UINT							p_power
)
{
	#if(DBG_DIVPOWER2 > 0)
	dout << "Enter -- BigInteger::divpower2, p_power = " << p_power << endl;
	#endif
	ULONG							MASK = ~(~0 << NUMBER_OF_PLACES);
	ULONG							place_holders = p_power / NUMBER_OF_PLACES;
	ULONG							bit_holders = p_power % NUMBER_OF_PLACES;

	if (this->values.size() < place_holders)
	{
		return(0);
	}

	BigInteger						t;
	ULONG							old_position = place_holders;
	ULONG							new_position = 0;
	ULONG							temp1;
	ULONG							temp2;

	temp1 = this->values[old_position++] >> bit_holders;
	if (old_position >= this->values.size())
	{
		temp2 = 0;
	}
	else
	{
		temp2 = this->values[old_position] << (NUMBER_OF_PLACES - bit_holders);
		temp2 &= MASK;
	}
	t.values[new_position] = temp2 | temp1;

	#if(DBG_DIVPOWER2 > 0)
	dout << "	BigInteger::divpower2, old_position = " << old_position << endl;
	dout << "	BigInteger::divpower2, new_position = " << new_position << endl;
	dout << "	BigInteger::divpower2, temp2 = " << hex << temp2 << ", temp1 = " << temp1 << endl;
	dout << "	BigInteger::divpower2, t = " << t << endl;
	#endif
	for ( ; old_position < this->values.size(); )
	{
		temp1 = this->values[old_position++] >> bit_holders;
		if (old_position >= this->values.size())
		{
			temp2 = 0;
		}
		else
		{
		    temp2 = this->values[old_position] << (NUMBER_OF_PLACES - bit_holders);
			temp2 &= MASK;
		}
		t.values.push_back(temp2 | temp1);
		++new_position;
	#if(DBG_DIVPOWER2 > 0)
	dout << "		BigInteger::divpower2, old_position = " << old_position << endl;
	dout << "		BigInteger::divpower2, new_position = " << new_position << endl;
	dout << "		BigInteger::divpower2, temp2 = " << hex << temp2 << ", temp1 = " << temp1 << endl;
	dout << "		BigInteger::divpower2, if for loop, t = " << t << endl;
	#endif
	}
/*
	temp1 = t.values[new_position] >> bit_holders;
	t.values[new_position] = temp1;
*/
	#if(DBG_DIVPOWER2 > 0)
	dout << "Exit -- BigInteger::divpower2, after for loop, t = " << t << endl;
	#endif

	return(t);

}  // divpower2

//
//	N % (a - 1) == N % a + N / a;
//
void
BigInteger::Mod_2_Pow_N_Minus_1
(
	UINT							p_exp
)
{
	BigInteger						t = 1;

	t.power2(p_exp);
	--t;

	while (*this >= t)
	{
		BigInteger				a1 = this->modpower2(p_exp);

		BigInteger				a2 = this->divpower2(p_exp);

		if ((a1 > *this) || (a2 > *this))
		{
			dout << "==========	BigInteger::Mod_2_Pow_N_Minus_1, a1 or a2 > u, break " << a1 << ", " << a2 << ", " << *this << endl;
			return;
		}
		*this = a1 + a2;
		
		if (*this >= t)
		{
			*this = *this - t;
		}

	}  // while (*this >= t)

}	// Mod_2_Pow_N_Minus_1

void
BigInteger::sqmod
(
	UINT							p_exp
)
{
	square();
	Mod_2_Pow_N_Minus_1(p_exp);

}	// sqmod

bool
BigInteger::operator<
(
	const BigInteger				& p_bi
)
	const
{
	return (!(*this >= p_bi));

}	// operator<

bool
BigInteger::operator<=
(
	const BigInteger				& p_bi
)
	const
{
	return (!(*this > p_bi));

}	// operator<=

bool
BigInteger::operator> 
(
	const BigInteger				& p_bi
)
const
{
	#if(DBG_OPERATOR_GT > 0)
	dout << "Enter operator> " << *this << " > " << p_bi << endl;
	#endif

	if (values.size() > p_bi.values.size())
	{
		return (true);
	}
	if (values.size() < p_bi.values.size())
	{
		return (false);
	}

	for (int i = values.size() - 1; i >= 0; --i)
	{
		if (values[i] > p_bi.values[i])
		{
			return (true);
		}
		if (values[i] < p_bi.values[i])
		{
			return (false);
		}
	}

	return (false);

}  // operator>

bool
BigInteger::operator>= (
	const BigInteger				& p_bi
)
const
{
	#if(DBG_OPERATOR_GE > 0)
	dout << "Enter operator> " << *this << " > " << p_bi << endl;
	#endif

	if (values.size() > p_bi.values.size())
	{
		return (true);
	}
	if (values.size() < p_bi.values.size())
	{
		return (false);
	}

	for (int i = values.size() - 1; i >= 1; --i)
	{
		if (values[i] > p_bi.values[i])
		{
			return (true);
		}
		if (values[i] < p_bi.values[i])
		{
			return (false);
		}
	}

	if (values[0] >= p_bi.values[0])
	{
			return (true);
	}
	return (false);

}  // operator>=

/*
BigInteger&
BigInteger::operator%= (
	const BigInteger & p_big_integer
)
{

dout << "Enter operator% (bi) with " << *this << " mod " << p_big_integer << endl;

	while (*this > p_big_integer)
	{
dout << " %%%%%%%%%%% operator% (bi) with " << *this << " mod " << p_big_integer << endl;
		*this -= p_big_integer;
dout << "%%%%%%%%%%%% operator% (bi) with " << *this << " mod " << p_big_integer << endl;
	}

dout << "Exit operator% with " << *this << endl;

	return (*this);

}  // operator % friend
*/

BigInteger
operator% (
	const BigInteger				& p_l,
	const BigInteger				& p_r
)
{
	BigInteger						big_integer = p_l;

	#if(DBG_OPERATOR_MOD > 0)
	dout << "Enter operator% (bi,bi) with " << p_l << " mod " << p_r << endl;
	#endif

	while (big_integer >= p_r)
	{
		#if(DBG_OPERATOR_MOD > 0)
		dout << " %%%%%%%%%%% operator% (bi,bi) with " << big_integer << " mod " << p_r << endl;
		#endif

		big_integer = big_integer - p_r;

		#if(DBG_OPERATOR_MOD > 0)
		dout << "%%%%%%%%%%%% operator% (bi,bi) with " << big_integer << " mod " << p_r << endl;
		#endif
	}

	#if(DBG_OPERATOR_MOD > 0)
	dout << "Exit operator% with " << big_integer << endl;
	#endif

	return (big_integer);

}  // operator % friend

std::pair<BigInteger, BigInteger>
operator / (
	const BigInteger				& p_l,
	const BigInteger				& p_r
)
{
	#if(DBG_OPERATOR_DIV > 0)
	dout << "Enter operator/ (bi,bi) with " << p_l << " div " << p_r << endl;
	#endif

	if (p_r > p_l)
	{
		return (std::pair<BigInteger, BigInteger>(0, p_r));
	}

	if (p_r == p_l)
	{
		return (std::pair<BigInteger, BigInteger>(1, 0));
	}

	BigInteger						remainder = p_l;
	BigInteger						quotent = 0;

	while (remainder > p_r)
	{
		unsigned long				place_shift = p_l.values.size() - p_r.values.size();
		long						bit_shift;
		
		bit_shift = BigInteger::calc_bit_shift (remainder.values[remainder.values.size()], p_r.values[p_r.values.size()]);

		BigInteger					s;

		s = p_r;
		s << (place_shift + bit_shift);

		if (s > remainder) dout << "Divide ERROR s > remainder" << endl;
	
		BigInteger					add_value = 1;

		add_value.power2(place_shift + bit_shift);
		quotent += add_value;

		remainder -= s;

		while (remainder >= s)
		{
			quotent += add_value;
			remainder -= s;
		}

	}	// while (remainder > p_r)

	return (std::pair<BigInteger,BigInteger>(quotent, remainder));

	unsigned long					left_bits  = p_l.values.size() - 1;
	unsigned long					right_bits = p_r.values.size() - 1;
	//
	// How many bits are in the most significant value.
	//
	unsigned long					msb = p_l.values[p_l.values.size() - 1];

	#if(DBG_OPERATOR_DIV > 2)
	dout << "	left bits " << left_bits << ", right_bits " << right_bits << ", msb " << msb << endl;
	#endif

	while (msb > 0)
	{
		msb >>= 1;
		++left_bits;
	}

	msb = p_r.values[p_r.values.size() - 1];

	while (msb > 0)
	{
		msb >>= 1;
		++right_bits;
	}

	unsigned long					shift_difference = left_bits - right_bits - 1;
	unsigned long					bit_shift = shift_difference % NUMBER_OF_PLACES;
	unsigned long					place_shift = shift_difference / NUMBER_OF_PLACES;

	BigInteger						result(p_l.values.size());
//	BigInteger						remainder = p_l;

	#if(DBG_OPERATOR_DIV > 2)
	dout << "	left bits " << left_bits << ", right_bits " << right_bits << endl;
	dout << "	shift_difference " << shift_difference << ", bit_shift " << bit_shift << ", place shift " << place_shift << endl;
	#endif

	while (place_shift > 0)
	{
		#if(DBG_OPERATOR_DIV > 2)
		#endif
		result.values[place_shift-1] = 0;

		while (bit_shift > 0)
		{
			#if(DBG_OPERATOR_DIV > 3)
			dout << "			bit shift " << bit_shift << endl;
			#endif

			BigInteger						divisor = p_r;

			//divisor.shiftleftbits(bit_shift);
			divisor << bit_shift;

			#if(DBG_OPERATOR_DIV > 3)
			dout << "			divisor w/ shift left bits " << divisor << endl;
			#endif
			
			//divisor << place_shift;
			divisor.shiftleftplaces(place_shift);

			#if(DBG_OPERATOR_DIV > 3)
			dout << "			divisor w/ place shift " << divisor << endl;
			#endif

			while (remainder > divisor)
			{
				unsigned long		value = 1 << bit_shift;

				result.values[place_shift-1] += value;
				remainder = remainder - divisor;

				#if(DBG_OPERATOR_DIV > 3)
				dout << "			result " << result << ", remainder " << remainder << endl;
				#endif
			}
			
			--bit_shift;
		}

		--place_shift;
	}

	#if(DBG_OPERATOR_DIV > 0)
	dout << "Exit operator / result " << result << " remainder " << remainder << endl;
	#endif

	return (std::pair<BigInteger, BigInteger>(result, remainder));

}  // operator / friend

long
BigInteger::calc_bit_shift
(
	unsigned long					p_l,
	unsigned long					p_r
)
{
	if (p_l > p_r)
	{
		unsigned long				msb = p_l/p_r;
		long						shift_bits = 0;

		while (msb > 1)
		{
			msb >>= 1;
			++shift_bits;
		}
		
		return (shift_bits);
	}

	if (p_l == p_r)
	{
		return (0);
	}

	unsigned long				msb = p_r/p_l;
	long						shift_bits = 0;

	while (msb > 1)
	{
		msb >>= 1;
		++shift_bits;
	}
	
	return (-shift_bits);
	
}	// calc_bit_shift

std::ostream&
operator<< (
	std::ostream					& p_os,
	const BigInteger				& p_bi
)
{
	bool							positive = true;

	for (int i = p_bi.values.size() -1; i >= 0; --i)
	{
		p_os << p_bi.values[i] << " ";
		if (p_bi.values[i] < 0)
		{
			positive = false;
		}
	}

	assert(positive);

	return (p_os);

}  // operator << ostream

bool
BigInteger::is_valid
(
	BigInteger						& p_bi
)
{
	for (unsigned long i = 0; i < p_bi.values.size(); ++i)
	{
		if (p_bi.values[i] > MAX_VALUE)
		{
			return (false);
		}
	}

	return (true);

}	// is_valid
