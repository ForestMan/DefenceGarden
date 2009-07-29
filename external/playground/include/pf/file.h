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
 * Interface for class TFile
 */

#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

#if !defined(PFCONFIG_H_INCLUDED)
#include "pfconfig.h"
#endif

#ifndef PFTYPES_H_INCLUDED
#include "pftypes.h"
#endif

/// The position relative to which you want to seek.
typedef enum eFileSeek
{
	kSeekDelta,	///< Seek relative to your current position.
	kSeekStart,	///< Seek relative to the start of the file.
	kSeekEnd	///< Seek relative to the end of the file. Negative numbers will seek within the file.
} eFileSeek ;

/// The mode you use to open a file.
/// @see TFile::Open
typedef enum eFileMode
{
	kReadBinary,	///< Read a binary file
	kReadText,		///< Read a text file
/// Constant for the first write mode. Not a real mode, but a
/// delimiter: Any mode greater or equal to this value is a
/// mode that writes to the file.
	kFirstWriteMode,
	kWriteBinary=kFirstWriteMode,	///< Write a binary file
	kWriteText,		///< Write a text file
	kAppendBinary,	///< Append to the end of a binary file
	kAppendText,	///< Append to the end of a text file
} eFileMode;

#if __cplusplus

#ifndef FIXBYTEORDER_H_INCLUDED
#include "fixbyteorder.h"
#endif

#include <set>
#ifndef STR_H_INCLUDED
#include "str.h"
#endif

#include <map>

class TFileData ;

/**
 * A file reading abstraction.  All file access from Playground
 * games should be handled through this abstraction.
 *
 * File names must consist ONLY of the following characters:
 *
 * - a-z : Lowercase letters
 * - 0-9 : Digits
 * - - : Hyphen
 * - _ : Underscore
 * - . : Dot
 *
 * Paths must be separated by forward slash ("/"). All files
 * must be specified without a leading slash.
 *
 * A file in "assets/bitmaps/" called "my.png" would be loaded with
 * the handle "bitmaps/my.png".
 *
 * To access a writable folder, prefix the file name with either
 * user:, common:, or desktop: to get to either the user's personal
 * data folder, the system's common data folder, or the desktop
 * folder. You may NOT write to the assets folder during the game
 * --it will ASSERT in debug build if you try.
 *
 * There is a set of C stdio-compatible routines that you can
 * use to port existing fopen-style interfaces:
 *
 * - pf_open()
 * - pf_close()
 * - pf_seek()
 * - pf_tell()
 * - pf_getc()
 * - pf_gets()
 * - pf_read()
 * - pf_write() (only to user:, common: and desktop: folders)
 * - pf_eof()
 * - pf_error()
 * - pf_ungetc()
 *
 * If you absolutely must use fscanf(), you can use pf_fgets to
 * get a line of text, and then use sscanf() to parse the line.
 * However, in general it is be much better to read and write
 * the data as XML using TXmlNode.
 */
#if TAGGING_IGNORE
class PFLIB_API TFile
#else
struct PFLIB_API TFile
#endif
{
public:
	/// Default constructor.
	TFile();

	/// Destructor
	~TFile();

	/**
	 * Open an existing file. Closes any file this TFile previously
	 * had open, and attempts to open the given file.
	 *
	 * @param path   Path to file
	 * @param mode   Mode to open file
	 *
	 * @return true on success.
	 */
	bool Open( str path, eFileMode mode=kReadBinary );

	/**
	 * Return true if the file was opened successfully.
	 *
	 * @return True on success; false on failure.
	 */
	bool IsValid() const ;

	/**
	 * Return true if the file is at the EOF.
	 *
	 * @return True on EOF.
	 */
	bool AtEOF() ;

	/**
	 * Close a file. Optional; file is automatically
	 * closed on destruction of the TFile
	 */
	void Close() ;

	/**
	 * Seek to a specific file position.
	 *
	 * @param offset Offset from reference position.
	 * @param seek   Which reference position to use.
	 */
	void Seek( long offset, eFileSeek seek );

	/**
	 * Get the current file position.
	 *
	 * @return Current offset into the file.
	 */
	long Tell();

	/**
	 * Get the file's size.
	 *
	 * @return File size in bytes.
	 */
	long Size();

	/**
	 * Read bytes into buffer.
	 *
	 * @param buffer Buffer to fill.
	 * @param bytes  Bytes to read.
	 *
	 * @return bytes read
	 */
	long Read( void * buffer, unsigned long bytes );

	/**
	 * Write bytes to file from buffer.
	 *
	 * @param buffer Buffer to read bytes from.
	 * @param bytes  Bytes to write.
	 *
	 * @return bytes written
	 */
	long Write( const void * buffer, unsigned long bytes );

	/**
	 * "Unget" one character. Semantics are similar to ungetc,
	 * in that you can only ever "unget" one character.
	 *
	 * Unlike ungetc, it will only unget exactly the previous
	 * character you just read--there is no option to select
	 * a character to unget.
	 *
	 * Ungetting past the beginning of the file is not allowed.
	 */
	void Unget();

	/**
	 * Test to see if a file exists. Expects a standard
	 * resource handle.
	 *
	 * @param handle Resource handle.
	 *
	 * @return True if file exists
	 */
	static bool Exists( str handle );

	/**
	 * Return the source of the file.
	 *
	 * @param handle Handle of the file to query.
	 *
     * @return "filesystem" if it's a normal file, "memoryfile" if
     *         it's a memory file, an empty string if file isn't
     *         found, or the name of the source .pfp file.
	 */
	static str Source( str handle );

	/**
	 * Get the current working directory.
	 *
	 * @return Current working directory.
	 */
	static str GetCWD();

	/**
	 * Iterate through the files in a folder.
	 *
	 * @param folder     Folder to search. File globs are NOT supported currently.
	 * @param file       Pointer to a string to receive each file name.
	 *
	 *                   Start iteration with an empty string, and pass the value
	 *                   you received previously to get the next value in the
	 *                   iteration.
	 * @param subfolders True to return files in subfolders as well.
	 *
	 * @return True if successful; false to signal end of iteration.
	 */
	static bool GetNextFile( str folder, str * file, bool subfolders=false );

	/**
	 * Internal function: Scan for updated file resources.
	 *
	 * Called by Playground on application startup. If the
	 * file system changes this needs to be called again to
	 * update the internal cache.
	 */
	static void ScanForResources();
#ifndef USER_DOCS
	static void SetUserDataDirs( str userData, str commonData, str desktopData )
	{
		mUserDir = userData ;
		mCommonDir = commonData ;
		mDesktopDir = desktopData ;
	}
	/**
	 * Flag to tell the file system that we're in "First Peek"
	 * mode: i.e., a limited-functionality beta.
     *
     * @param bOn    True to enable first-peek mode.
	 */
	static void SetFirstPeek(bool bOn) { mFirstPeek = bOn; }
#endif
	/**
	 * Clear out the file system cache.
	 */
	static void ShutDownFileSystem();

	/**
	 * Scan a folder (and subfolder) for files.
	 *
	 * @param folder Folder to scan.
	 * @param prefix File system prefix to expect.
	 */
	static void ScanFolderForResources( str folder, str prefix="" );

	/**
     * Add a virtual file to the file system. After you call this
     * function, opening a file with the given name for read will
     * supply the given data. The data is not copied, so you are
     * responsible for ensuring that the data buffer continues to
     * exist for as long as the memory file is available. In other
     * words, the client owns the memory block, and must ensure its
     * lifetime exceeds that of the memory file entry.
     *
     * <b>Opening the file for write is not currently supported.</b>
	 *
	 * @param memoryFileName
	 *               Name of the virtual file to add.
     * @param base   Pointer to the start of the virtual file data.
     *               Pass NULL to delete the virtual file.
	 * @param size   Size of the virtual file.
	 */
    static void AddMemoryFile( str memoryFileName, void * base, uint32_t size );

	/**
	 * Add a set of files-to-be-ignored to TFile. These files will
	 * not be "visible" to the TFile file system.
	 *
	 * Paths should be given the same as a path would be given
	 * to TFile::Open().
	 *
	 * Entries in this string are space (or line) separated.
	 *
	 * Each entry matches either one complete file path, or one
     * complete folder path. Folders are NOT matched in user: or
     * common:--only complete file paths are matched.
	 *
	 * Passing in an entry of -* will clear the entire file mask. If you
	 * put this at the start of your string, it will clear the file mask
     * before adding the rest of the files in your string.
	 *
	 * @param mask   A space-delimited string of complete file or folder paths.
	 * @param add    True to add the files to the mask (ignore the files in the list);
	 *               false to remove the files from the mask (to make them show
	 *               up again).
	 */
	static void AddFileMask( const char * mask, bool add=true );

	/**
	 * Delete a file from user: or common:, or dereference a memory
     * file. For a memory file, just removes the entry in the file
     * table: No memory is deallocated, as the client owns the
     * memory.
     *
     * Note that if you try to delete a file that you currently have
     * open, the behavior is undefined.
	 *
	 * @param filename Filename to delete. Must be prefixed by user: or common:.
	 *
	 * @return True on success.
	 */
	static bool DeleteFile( str filename );

#ifndef USER_DOCS
    /**
     * @nodox
	 * Translate a file handle into a native filesystem file name,
     * if one exists; otherwise, return a unique internal key that
     * is used to identify the file to Playground.
	 *
	 * @param handle     File handle to translate.
	 * @param existsOnly Only return the path to files that exist on disk.
	 *
	 * @return A system-specific filename, if the resulting path refers to
	 *         a file on disk. Otherwise a coded string indicating to
	 *         Playground where the file is located.
	 */
	static str TranslateResource( str handle, bool existsOnly = false );
#endif

private:
	TFileData * mData;

	static bool InternalAddFileMask( const char * mask, bool add=true );

	static bool FileAllowed( str file );

	friend class TPlatformData ;
	static str						mActiveDir ;
	static str						mCommonDir ;
	static str						mUserDir ;
	static str						mDesktopDir ;
	static std::map<str,str>		mResourceMap ;
	static std::set<str>			mIgnoredFiles ;
	static bool						mFirstPeek ;
};

#else // __cplusplus
typedef struct TFile TFile;
#endif


#if __cplusplus
extern "C" {
#endif

/**
 * C interface to open a file.
 *
 * @param filename Name of file to open.
 * @param mode     Mode to open file.
 *
 * @return A new TFile pointer with that file open, or NULL.
 * @see TFile
 * @see eFileMode
 */
PFLIB_API TFile *pf_open( const char * filename, eFileMode mode );

/**
 * C interface to close a file.
 *
 * @param file   File to close.
 *
 * @see TFile
 */
PFLIB_API void pf_close( TFile* file );

/**
 * C interface to seek to a file position
 *
 * @param file  File to seek.
 * @param offset Delta to apply to file position.
 * @param type  type of seek.
 *
 * @see TFile
 * @see eFileSeek
 */
PFLIB_API void pf_seek( TFile* file, long offset, eFileSeek type);

/**
 * Emulation of C fgets(); not particularly fast, so not
 * recommended for new code.
 *
 * @param file   File to read.
 * @param buffer Buffer to file with characters.
 * @param num    Maximum number of characters to read.
 */
PFLIB_API char * pf_gets( TFile* file, char * buffer, unsigned long num );

/**
 * C interface to get the current position in a file.
 *
 * @param file File to report on.
 * @return Current position.
 * @see TFile
 */
PFLIB_API long pf_tell( TFile* file );

/**
 * C interface to read bytes into a buffer.
 *
 * Note that the file parameters are in a different order than
 * fread.
 *
 * @param file   File to read.
 * @param buffer Buffer to read into.
 * @param size   Number of bytes to read.
 *
 * @return Number of bytes actually read.
 * @see TFile
 */
PFLIB_API long pf_read( TFile* file, void * buffer, unsigned long size );

/**
 * C interface to write bytes from a buffer
 *
 * Note that the file parameters are in a different order than
 * fwrite.
 *
 * @param file   File to write to.
 * @param buffer Buffer to write from.
 * @param size   Number of bytes to write.
 *
 * @return Number of bytes actually written.
 * @see TFile
 */
PFLIB_API long pf_write(TFile * file, const void * buffer, unsigned long size );

/**
 * return non-zero if TFile is at EOF
 *
 * @param file   File to test.
 *
 * @return Nonzero if EOF.
 */
PFLIB_API int32_t pf_eof( TFile* file );

/**
 * Get a character from the input stream
 *
 * @param file   File to read.
 *
 * @return Character to read, or -1 on EOF.
 */
PFLIB_API int32_t pf_getc( TFile* file );

/**
 * "Unget" a character from the input stream.
 *
 * Rewinds input stream by one character.
 *
 * @param file   File to rewind.
 */
PFLIB_API void pf_ungetc( TFile* file );

/**
 * Test the stream for an error condition.
 *
 * @param file File to test.
 *
 * @return 0 if the stream is OK, or non-zero if there was an error.
 */
PFLIB_API int32_t pf_error( TFile* file );

#if __cplusplus
}

/**
 * Read a value in a byte-order agnostic way.
 *
 * @param f      File to read.
 * @param data   Destination for data. Based on sizeof(data),
 *               ReadBinary will do the right thing, as long as
 *               data is a basic type (not itself a struct).
 */
template <typename T>
inline void ReadBinary(TFile * f, T* data)
{
	pf_read(f, data, sizeof(T));
	FixByteOrder(data);
}
#endif

#endif // FILE_H_INCLUDED

