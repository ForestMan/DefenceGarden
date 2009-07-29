//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------

#ifndef DIALOG_H_INCLUDED
#define DIALOG_H_INCLUDED

/**
 * @condfile 
 * Interface for TDialog.
 * \ingroup windowgroup
 */	
				 
#ifndef MODALWINDOW_H_INCLUDED
#include "modalwindow.h"
#endif

#ifndef STR_H_INCLUDED
#include "str.h"
#endif
				
/**
 * A generic modal dialog
 * \ingroup windowgroup
 */
class PFLIB_API TDialog : public TModalWindow
{
	PFTYPEDEF(TDialog,TModalWindow);
public:
	enum
	{
		kResponseOK
	};

	/**
	 * Default Constructor
	 *
	 * @param initFileName
	 *                  Dialog spec file.
	 * @param bodyText  Text for the body of the dialog
	 * @param titleText Text for the title of the dialog.
	 */
	TDialog(str initFileName, str bodyText, str titleText);

	/// Destructor
	virtual ~TDialog();

	virtual bool OnMessage(TMessage *message);

	/**
     * Do the real initialization on being added to the hierarchy.
     * 
	 * @return true on success.
	 */
	virtual bool OnNewParent();

protected:
	enum
	{
		kHttpLink = 30000
	};
private:
	TDialog( const TDialog & button );
	const TDialog & operator=( const TDialog & button );

	str mSpecFile ;
	str mBodyText ;
	str mTitleText ;
	str mHttpLink ;	// Could be a list

};

#endif // DIALOG_H_INCLUDED
