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
 * Interface for base class of TAsset.
 */

#ifndef ASSET_H_INCLUDED
#define ASSET_H_INCLUDED

// Include forward declarations here, since just about anyone using an
// asset will need them.
#ifndef FORWARD_H_INCLUDED
#include "forward.h"
#endif

#ifndef PFTYPEINFO_H_INCLUDED
#include "pftypeinfo.h"
#endif

#include <boost/enable_shared_from_this.hpp>

/** 
 * The interface class for game assets.
 * 
 * Playground-defined TAssets are managed internally by the engine such that
 * if you have a reference to one anywhere in your game, and you request a new one
 * by the same name, it will retrieve the reference rather than reloading the 
 * asset.
 */
class PFLIB_API TAsset : public enable_shared_from_this<TAsset>
{
	PFSHAREDTYPEDEFBASE(TAsset);
public:

	/// Destructor
	virtual ~TAsset() ;

#if USER_DOCS
	/// Get a reference to this asset. Do not call in a constructor!
	TAssetRef GetRef();
#endif

protected:
	friend class TAssetManager ;

	/// Release an asset.
	virtual void Release() {} ;

	/// Restore an asset.
	virtual void Restore() {} ;
};

#endif // ASSET_H_INCLUDED

