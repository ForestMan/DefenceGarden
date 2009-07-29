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
 * Interface for class TTaskList
 */

#ifndef TASKLIST_H_INCLUDED
#define TASKLIST_H_INCLUDED

#ifndef TASK_H_INCLUDED
#include "task.h"
#endif

#include <list>

/**
 * A list of TTask-derived objects.
 */
class PFLIB_API TTaskList
{
public:

	TTaskList() : mProcessing(false)
	{}

	/**
	 * Destructor
	 */
	~TTaskList();

	/**
	 * Remove a task from the task list. Does not
	 * delete the task, but rather releases ownership;
	 * calling function now owns task.
	 *
	 * @param task   Task to remove.
	 * @return true if task was removed, false if task was not found
	 */
	bool OrphanTask( TTask * task );

	/**
	 * Add a task to the task list. Task list takes ownership of the
	 * task and will delete it when the task notifies that it is complete.
	 *
	 * @param task	Task to add.
	 */
	void AdoptTask( TTask * task );

	/**
	 * The internal task list type.
	 */
	typedef std::list<TTask*> TaskList ;

	/**
     * Perform all of the tasks in the task list.
     *
     * @param context A calling context, if any. Used to trigger
     *                tasks when drawing, for example.
	 */
	void DoAll( TTask::ETaskContext context= TTask::eNormal );

	/**
	 * Destroy all the tasks in the list.
	 */
	void DestroyAll();

private:
	bool		mProcessing ;
	TaskList	mTaskList ;
	TaskList	mTasksToOrphan ;
	TaskList	mTasksToDelete ;
};


#endif // TASKLIST_H_INCLUDED
