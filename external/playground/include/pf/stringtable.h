//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------

/**
 * @condfile
 * Interface for TStringTable
 */

#ifndef STRINGTABLE_H_INCLUDED
#define STRINGTABLE_H_INCLUDED


#ifndef STR_H_INCLUDED
#include "str.h"
#endif

#ifndef DEBUG_H_INCLUDED
#include "debug.h"
#endif

#include <map>


/**
 * The interface class for a string table.
 *
 * The global string table contains a mapping of string
 * identifiers to localized strings. When each string is
 * requested at run time using GetString(), the parameters
 * passed into GetString() are substituted into slots specified
 * by \%1%, \%2%, \%3%... etc.
 *
 * You can also use %'s to reference other lookups, such as "Today's date is %date%", where
 * date is another string to look up in the table.
 *
 * To create "%" in a string, use "%%".
 *
 * Each string supports up to 9 parameters.
 *
 * A string can contain formatting information as described in the documentation for TTextGraphic.
 *
 * See @ref strings for a general description of the string table requirements.
 */
class PFLIB_API TStringTable
{
public:

	/**
	 * Get a string - get a string from the string table, filling in the string with passed in strings
	 *
	 * @param id		id of string to look up in the string table.
     * @param param1-param5	Optional strings used to fill in the
     *                  placeholders in the string returned from the
     *                  string table. If you want a parameter to be
     *                  looked up in the string table as well,
     *                  enclose it with \%param\% to signify that it
     *                  should be looked up in the string table (use
     *                  "%%" to just output a "%").
	 *
	 *
	 * @return	a formatted str that is the result of looking up the id in the string table and adding
	 *			in all the optional parameters. If any of the ids do not exist in the str table, the
	 *			string returned will be "#####" to signify an invalid lookup.
	 */
	str GetString(str id, str param1=str(), str param2=str(), str param3=str(), str param4=str(), str param5=str());

	/**
	 * Get a string - get a string from the string table, filling in the string with passed in array of strings
	 *
	 * @param id		id of string to look up in the string table.
	 * @param numStr	number of Str in the str array
	 * @param strArray	Pointer to array of strings used to fill in the placeholders in the string returned from
	 *					the string table. If you want a parameter to be looked up in the string
     *                  table as well, enclose it with \%param\% to
     *                  signify that it should be looked up in the
     *                  string table (use "%%" to just output a
     *                  "%").
	 *
	 *
	 * @return	a formatted str that is the result of looking up the id in the string table and adding
	 *			in all the optional parameters. If any of the ids do not exist in the str table, the
	 *			string returned will be "#####" to signify an invalid lookup.
	 */
	str GetString(str id, uint32_t numStr, str *strArray);

	/**
	 * Set the string table to not write errors for missing strings.
	 *
	 * @param silent True to suppress errors.
	 *
	 * @return Old value of silent.
	 */
	bool	SetSilent( bool silent ) { bool old = mSilent; mSilent = silent; return old;	}
#if !USER_DOCS

	/**
	 * @nouser
	 *
	 * Constructor for TStringTable
	 */
	TStringTable();

	/**
	 * @nouser
	 *
	 * Destructor for TStringTable
	 */
	~TStringTable();

	/**
	 * @nouser
	 *
	 * Load in a string table file.
	 *
	 * @param filePath complete file name of file to load in
	 *
	 * @return true if file exists and loads, false otherwise.
	 */
	bool LoadStringTable(str filePath);

	/**
	 * @nouser
	 *
     * Merge a string table file with the existing strings.
	 *
	 * @param filePath complete file name of file to load in
	 *
	 * @return true if file exists and loads, false otherwise.
	 */
	bool MergeStringTable(str filePath);


	/**
	 * @nouser
	 *
     * Fills in a  list of all entries in the string table that
	 * begin with "FLASH_". These strings are designated as
	 * strings that appear in Flash movies.
	 *
	 *
	 * @return semi-colon delimited list of string names.
	 */
	void GetFlashStrings(std::list<str> &stringList);
#endif


private:
	std::map<str,str>	mStringMap;
	bool				mSilent ;
};

#endif // STRINGTABLE_H_INCLUDED
