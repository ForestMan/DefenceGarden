//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------

#ifndef XML_H_INCLUDED
#define XML_H_INCLUDED

#include "str.h"
#if _DEBUG
// Only include debug.h in debug builds.
#include "debug.h"
#endif
#include <list>

//------------------------------------------------------------------
class TXmlNode;
class TXmlAttribute;
class TGetChar;

typedef std::list<TXmlNode *> TXmlNodeList;
typedef std::list<TXmlAttribute *> TXmlAttributeList;

/**
 * The TXmlNode class is a limited XML parser. It does not
 * support comments with nested tags, nor most non-trivial XML
 * extensions.
 *
 * Any XML files in your assets folder will be obfuscated by
 * default (in addition to being included in the flat file) in a
 * production build, so if you need them to remain
 * human-readable, let your producer know.
 */
class PFLIB_API TXmlNode
{
public:
	/**
	 * Default constructor.
	 */
	TXmlNode();
	/**
	 * Create this node with a name.
	 *
	 * @param name   Initial name for the node.
	 */
	TXmlNode(const char *name);
	/**
	 * Destructor.
	 */
	virtual ~TXmlNode();

	/**
	 * Parse a stream as XML, loading contents as children of this node.
	 * Does not consume all data. Consumes a balanced open/close tag (and everything in between)
	 *
	 * @param data		buffer to parse
	 * @param len		length of buffer
	 * @param bOneTag	normally false, set to true to consume just one tag, not a balanced open/close
	 *
	 * @return Number of Characters consumed from the buffer - will be 0, if not enough data available
	 */
	uint32_t			ParseStream(const char* data, uint32_t len, bool bOneTag = false);

	/**
	 * Parse a string as XML, loading contents as children of this node.
	 *
	 * @param data   String to parse.
	 */
	void				ParseString(const char* data);

	/**
	 * Parse a file as XML, loading contents as children of this node.
	 *
	 * @param filename File to read.
	 */
	void				ParseFile(const char* filename);

	/**
	 * Remove all children and attributes.
	 */
	void				Clear();

	/**
	 * Query whether this node has children
	 *
	 * @return True if has children.
	 */
	bool				HasChildren() const;

	/**
	 * Get a pointer to a child of the node.
	 *
	 * @param name   The name of the child to find.
	 *
	 * @return A pointer to the child. The child is still owned by the parent,
	 *         so this pointer will become invalid when the parent is
	 *         deleted.
	 */
	TXmlNode*			GetChild(const char* name);

	/**
	 * Remove a child from the parent and take ownership.
	 *
	 * @param child  A pointer to the child.
	 */
	void				OrphanChild(TXmlNode *child);

	/**
	 * Delete a child from this node.
	 *
	 * @param child  Child to delete.
	 */
	void				DeleteChild(TXmlNode *child);

	/**
	 * Reset internal child iterator.
	 *
	 * @see GetNextChild
	 */
	void				ResetChildren();

	/**
	 * Iterate through children. MUST call ResetChildren() to start iteration.
	 *
	 * @param pName		Pointer to str to receive name of child that was found
	 * @param pChild	Pointer to receive next child in iteration.
	 *
	 * @return
	 */
	bool				GetNextChild(str* pName, TXmlNode** pChild);

	/**
     * Create a new child and add it to our list of children.
	 *
	 * @param name   Name of the new child.
	 *
     * @return A pointer to the new child.
	 */
	TXmlNode*			CreateChild(const char *name);

	/**
	 * Set the name of this node.
	 *
	 * @param name   New name.
	 */
	void				SetName(const char* name);
	/**
	 * Get the name of this node.
	 *
	 * @return The node's name.
	 */
	str					GetName() const;
	/**
	 * Get the content of this node (the part between the opening
	 * and closing tags).
	 *
	 * @return The node content.
	 */
	str					GetContent();

	/**
	 * Set the content of this node.
	 *
	 * @param content New content.
	 */
	void				SetContent(const char* content);

	/**
	 * Get an attribute of this node's tag.
	 *
	 * @param name     Name of attribute to query.
     * @param pbQuoted Whether the attribute's value is surrounded
     *                 by quotes.
	 *
     * @return          The value of the attribute, if found.
     *                  Otherwise an empty string.
	 */
	str					GetAttribute(const char* name, bool* pbQuoted = NULL);
	/**
	 * Set an attribute to a particular value.
	 *
	 * @param name   Name of the attribute to set.
	 * @param value  New value for that attribute.
	 */
	void				SetAttribute(const char* name, const char* value);
	/**
	 * Set an attribute to a particular value.
	 *
	 * @param name   Name of the attribute to set.
	 * @param value  New value for that attribute.
	 */
	void				SetAttribute(const char* name, int32_t value);

	/**
	 * Reset the internal attribute iterator.
	 */
	void				ResetAttributes();

	/**
	 * Get the next attribute in an iteration.  Call ResetAttributes()
	 * to reset the iteration.
	 *
	 * @param pName  [return] Name of the attribute.
	 * @param pValue [return] Value of the attribute.
	 *
	 * @return True if another attribute was found.
	 */
	bool				GetNextAttribute(str* pName, str* pValue);

	/**
	 * Get the content of a child node.
	 *
	 * @param name   Node of child to find.
	 *
	 * @return Child content.
	 */
	str					GetChildContent(const char* name);

	/**
	 * Parse the XML tree into an XML formatted string that can
	 * be written to a file.
	 *
	 * @return The data as a string.
	 */
	str					AsString();

private:
	void				DecompressAndParse( const char * buf );

	virtual TXmlNode*	OnOpenTag(const str& name);
	virtual void		OnEndTag(const str& name);
	virtual void		OnContent(const str& content, int32_t len);
	virtual void		OnAttribute(const str& name, const str& value, bool bQuoted);

	void				AsString(str* s, uint32_t* pLen, int indent);
	void				Parse(TGetChar* data);
	TXmlNode*			ParseTag(TGetChar* data, bool* pbEnd);

private:
	str							mContent;
	uint32_t					mContentLen;
	uint32_t					mAsStringReserve ;
	str							mName;
	TXmlNodeList				mChildren;
	TXmlNodeList::iterator		mChildrenIterator;
	TXmlAttributeList			mAttributes;
	TXmlAttributeList::iterator	mAttributesIterator;
};

//------------------------------------------------------------------
#endif // XML_H_INCLUDED
