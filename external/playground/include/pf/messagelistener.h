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
 * Interface for class TMessageListener
 */


#ifndef MESSAGELISTENER_H_INCLUDED
#define MESSAGELISTENER_H_INCLUDED

#ifndef TAGGING_IGNORE
class TMessage;
#endif

/**
 * A message listener--a class that you override and register
 * with the TWindowManager if you want to listen for broadcast
 * messages.
 */
class TMessageListener
{
public:
	/// Destructor
	virtual ~TMessageListener() {};

    /**
     * This function will be called for each broadcast message 
     * that's delivered.
     * 
     * @param message The message being delievered.
     * 
     * @return True if the message was processed. No more searching
     *         for handlers will occur, and the message will be
     *         deleted by the caller if necessary.
     */
	virtual bool OnMessage( TMessage * message )=0;

};

#endif // MESSAGELISTENER_H_INCLUDED
