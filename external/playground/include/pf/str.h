//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------

#ifndef STR_H_INCLUDED
#define STR_H_INCLUDED

/**
 * @condfile
 *
 * Interface for str class.
 *
 * The str class has an interface modelled on std::string, with some
 * extensions.
 *
 * See, for instance, str::getFormatted(), str::find() and str::replace().
 * Note the kCaseInsensitive and kReplaceAll flags
 *
 */

#ifndef PFCONFIG_H_INCLUDED
#include "pfconfig.h"
#endif

#ifndef PFTYPES_H_INCLUDED
#include "pftypes.h"
#endif

#include <stdarg.h>
#if NEED_MALLOC_H
#include <malloc.h>
#endif
#if NEED_ALLOCA_H
#include <alloca.h>
#endif
#if NEED_STRING_H
#include <string.h>
#endif
#include <memory.h>

#define BOOST_QUICK_ALLOC 0

#if BOOST_QUICK_ALLOC
#include <boost/detail/quick_allocator.hpp>
#endif

class TStringData;

/**
 * Reference-counted string class.
 *
 * By default, str treats its string as a null-terminated string
 * of ASCII characters. Sorting (collation) is handled, by
 * default, in ASCII-value order. On the Windows platform,
 * however, there's a way to insert an alternate string compare
 * operator: If you add a DLL with the name pflocale.dll to the
 * folder with the game, it will attempt to import a string
 * compare function from that DLL named pfCollate() that takes
 * two char* parameters and returns -1, 0, or 1, as per
 * str::compare(). A UTF-8-aware comparison function is included
 * in pflocale.dll in the bin folder of the distribution.
 *
 * If you embed a null in a string using the str(char*,int)
 * constructor, then some functions will not work as expected;
 * str::length() in particular.
 */
class PFLIB_API str
{
	typedef str this_type;
	class TStringData {
	public:
		typedef TStringData this_type;
/*
		void * operator new(std::size_t)
		{
			return std::allocator<this_type>().allocate(1, static_cast<this_type *>(0));
		}

		void operator delete(void * p)
		{
			std::allocator<this_type>().deallocate(static_cast<this_type *>(p), 1);
		}*/
#if BOOST_QUICK_ALLOC
		void * operator new(std::size_t)
		{
			return boost::detail::quick_allocator<this_type>::alloc();
		}

		void operator delete(void * p)
		{
			boost::detail::quick_allocator<this_type>::dealloc(p);
		}
#endif
		TStringData(const char* s);
		TStringData(int size);
		~TStringData();
		void Reserve( uint32_t size );

		char*		mString;
		int			mRefCount;
//		int			mLength; // a super-important optimization...
		uint32_t	mSize ;
	private:
		TStringData( const TStringData & );
	};
public:
#if BOOST_QUICK_ALLOC
	void * operator new(std::size_t)
	{
		return boost::detail::quick_allocator<this_type>::alloc();
	}

	void operator delete(void * p)
	{
		boost::detail::quick_allocator<this_type>::dealloc(p);
	}
#endif

	static const int32_t npos =-1;	///< Non-position in string. Similar to STL.

	/**
	 * Create an empty string.
	 */
	str();

	/**
	 * Create a string from a null-terminated string.
	 *
	 * @param s      Pointer to a null terminated string.
	 */
	str(const char* s);

	/**
	 * Create a string from a buffer and length. Embedded zeros in
	 * the source buffer will cause length() to return shorter than
	 * len.
	 *
	 * @param s      Pointer to a buffer.
	 * @param len    Length of buffer. String will be created at this length.
	 */
	str(const char* s,size_t len);

	/**
	 * Copy constructor.
	 *
	 * @param s      String to copy and add a reference to.
	 */
	str(const str& s);

	/**
	 * Destructor.
	 */
	~str();

    /**
     * Compare this string with another.
     *
     * @param s      String to compare with.
     *
     * @return Similar to strcmp
     *         - -1 when this string is less than other string
     *         - 0 when strings are equal
     *         - 1 when this string is greater than other string
     */
	int_fast8_t		compare(const str& s) const;

	/**
	 * @name Operator Definitions
	 * @{
	 */

    /**
     * Read-only character access.
     */
	const char 		operator[](uint32_t i) const;

    /**
     * Assignment operator.
     *
     * @param s      Source string.
     *
     * @return A reference to this string.
     */
	str& 			operator=(const str& s);

    /**
     * Assignment operator.
     *
     * @param p      Source char * (c-style string).
     *
     * @return A reference to this string.
     */
	str& 			operator=(const char * p);

	/// Concatenation
	str 			operator+(const str&) const;

    /// Concatenation
	str 			operator+(char) const;

	/// Equality/inequality
	bool 			operator==(const str& ) const;
	bool 			operator<(const str& ) const;
	bool 			operator>(const str& ) const;
	bool 			operator<=(const str& ) const;
	bool 			operator>=(const str& ) const;
    /// Equality/inequality
    /// @param s String to compare against.
	bool			operator!=(const str& s) const { return compare(s)!=0 ; }

	/// Concatenation
	str& 			operator+=(const str& s);
	str& 			operator+=(const char c);

	/** @} */

	/**
	 * Get the current string length.
	 *
	 * @return Length of string not counting null character.
	 */
	uint32_t		length() const ;

	/**
	 * Get the current string length.
	 *
	 * @return Length of string not counting null character.
	 */
	uint32_t		size() const { return length() ; }

	/**
	 * Test whether the string is empty.
	 *
	 * @return True if empty.
	 */
	bool			empty() const { return length()==0; }

	/**
	 * Assign a certain number of characters to str.
	 *
	 * @param s      Base of string to copy.
	 * @param len    Number of characters.
	 *
	 * @return A reference to this.
	 */
    str & 			assign( const char * s, size_t len );

	/**
	 * Append a certain number of characters to str.
	 *
	 * @param s      Base of string to copy.
	 * @param len    Number of characters.
	 *
	 * @return A reference to this.
	 */
	str &			append( const char * s, size_t len );

	/**
	 * Extract a substring.
	 *
	 * @param pos    Position to start extracting.
	 * @param length Number of characters to extract.
	 *
	 * @return A new string with the specified characters.
	 */
	str 			substr( uint32_t pos, int32_t length=npos ) const ;

	/**
	 * Convert a string to an integer.
	 *
	 * @return An integer conversion of the string. If the string begins with non-digits, returns 0.
	 */
	int32_t			to_int() const ;

	/**
	 * Convert a string to a floating point value.
	 *
	 * @return A float conversion of the string. If the string begins with non-digits, returns 0.
	 */
	TReal			to_float() const ;

	/**
	 * Reserve at least size bytes in the internal string
	 * buffer.
	 *
	 * @param size   Number of bytes to reserve.
	 */
	inline void reserve( uint32_t size )
	{
		if (mData && size<=mData->mSize)
		{
			return; // Already big enough
		}
		_reserve(size);
	}

	/**
	 * Get a const char *.
	 *
	 * @return A const char * to the internal data.
	 */
	const char*		c_str() const;

	/**
	 * Format a string using a printf-style format string.
	 *
     * @warning Strings for \%s must be passed as char* arguments!
	 * @param formatstring
	 *               Format string.
	 */
	void			format( const char * formatstring, ... );

	/**
	 * Create a string consisting of a number of identical
	 * characters.
	 *
     * @param number Number of characters.
	 * @param c      Character to duplicate.
	 *
	 * @return A str with the requested duplicated characters.
	 */
	static str		dupchar( uint32_t number, char c=' ' )
	{
		char * s = (char*)alloca(number+1);

		s[number]=0;
		memset(s,c,number);
		return s ;
	}

	/**
	 * Create a formatted string using a printf-style format.
	 *
     * @warning Strings for \%s must be passed as char* arguments!
	 * @param formatstring
	 *               Format string.
	 */
	static str		getFormatted( const char * formatstring, ... );

	/**
	 * Create a formatted string using a printf-style format.
	 *
     * @warning Strings for \%s must be passed as char* arguments!
	 * @param formatstring
	 *               Format string.
	 * @param va  var-args argument list.
	 */
	static str		getFormattedV( const char * formatstring, va_list va);

	/**
	 * Find a substring.
	 *
	 * @param searchString
	 *               String to find.
	 * @param flags  eFlags for options.
	 * @param start  Search start.
	 *
	 * @return An offset into the string where found; npos if not found.
	 */
	int32_t			find( str searchString, uint32_t flags=0, uint32_t start=0 ) const;

	/**
	 * Find a character in this string.
	 *
	 * @param searchChar Character to search for.
	 * @param flags      eFlags for options.
	 * @param start      Start search position.
	 *
	 * @return An offset into the string where found; npos if not found.
	 */
	int32_t			find( char searchChar, uint32_t flags=0, uint32_t start=0 ) const ;

	/**
	 * Search-and-replace a substring.
	 *
	 * @param searchString
	 *               String to find.
	 * @param replaceString
	 *               String to replace found string with (can be empty).
	 * @param flags  eFlags for options.
	 * @param start  Search start.
	 *
     * @return A reference to this str.
	 */
	str & 			replace( str searchString, str replaceString, uint32_t flags=0, uint32_t start=0 );

	/**
	 * Erase a range of characters in the string
	 *
	 * @param start  First character to erase.
	 * @param count  Number of characters to erase. npos for the rest of the
	 *               string.
	 */
	void			erase( uint32_t start, int32_t count=npos );

	/**
	 * Force this instance of this string to be unique; prepare
	 * for modification.
	 */
	inline void unique()
	{
		if (mData && mData->mRefCount>1)
		{
			*(this) = mData->mString ;
		}
	}

	/// Flags for str::find and str::replace
	enum eFlags
	{
		kCaseInsensitive= 1,	///< Case insensitive search.
		kReplaceAll= 2,			///< Replace all instead of just the first match.
		kReverse=4				///< Search starting from end of string and working backwards. \c start in this case means characters from end of string.
	};

	/**
	 * Overlay a string into the current string.
	 *
     * @param start      Start of the new overlay as an index into
     *                   the current string. Can be (or extend)
     *                   beyond the end of the string. Can also be
     *                   npos, to indicate the end of the string.
	 * @param buffer     String to overlay. Is 8-bit safe (can contain NULL bytes).
	 * @param count      Size of string to overlay.
	 * @param bTerminate True to add a NULL character in the str at the end of
	 *                   this overlay.
	 *
	 * @return The character index past the end of the overlayed characters.
	 */
	uint32_t		overlay( int32_t start, const char* buffer, uint32_t count, bool bTerminate = true );

	/**
     * Convert this string to lower-case in place.
	 */
	void			downcase();

	/**
	 * Find the first character that matches the set given by s.
	 *
	 * @param s      Set of characters to search for.
	 * @param start  First character to inspect.
	 *
     * @return The offset of the first character that matches, or
     *         str::length() no characters match.
	 */
	unsigned int	find_first_of(str s, unsigned int start = 0);

	/**
     * Find the first character that doesn't match any in
     * the set given by s.
	 *
     * @param s      Set of characters to compare with.
	 * @param start  First character to inspect.
	 *
     * @return The offset of the first character that does not
     *         match, or str::length() if all characters match.
	 */
	unsigned int	find_first_not_of(str s, unsigned int start = 0);

	/**
	 * Calculate the size of a UTF8 character.
	 *
	 * @param s      Pointer to the character.
	 *
     * @return Number of bytes in this character. Zero if the
     *         character is a null terminator.
	 */
	static int sizeof_utf8_char( const char * s )
	{
		if (!*s)
		{
			return 0;	// NULL terminator
		}
		int c = *(const unsigned char *)s ;
		if (c >= 240)
		{
			return 4;
		}
		else if (c >= 224)
		{
			return 3;
		}
		else if (c >= 192)
		{
			return 2;
		}
		return 1;
	}

	/**
	 * Calculate the length, in characters, of a UTF-8 string.
	 *
	 * @param p      Pointer to const char * string of UTF-8 characters.
	 *
	 * @return The number of UTF-8 characters in a string.
	 */
	static inline uint32_t utf8length( const char * p )
	{
		uint32_t count=0 ;
		while ( *p )
		{
			++count;
			p+= str::sizeof_utf8_char(p);
		}
		return count ;
	}

	/**
	 * Convert a UTF-32 character code point to UTF-8.
	 *
	 * @param utf    UTF-32 value.
	 *
	 * @return A str with a single UTF-8 character.
	 */
	static str from_utf32( uint32_t utf );

	/**
	 * Convert a UTF-8 character to a UTF-32 character.
	 *
	 * @param utf8   Pointer to a 1-4 byte UTF-8 character.
	 *
	 * @return A UTF-32 character.
	 */
	static uint32_t to_utf32( const char * utf8);
private:
	void 			_reserve( uint32_t size );
	void			RemoveRef();
	TStringData*	mData;
	static uint32_t	mFormatBufferSize ;
};

#endif // STR_H_INCLUDED

