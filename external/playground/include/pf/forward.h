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
 * @file
 * Forward declarations for Playground.
 */

#ifndef FORWARD_H_INCLUDED
#define FORWARD_H_INCLUDED

#ifndef REF_H_INCLUDED
#include "ref.h"
#endif

#ifndef TAGGING_IGNORE

class TTexture ;
class TAnimatedTexture ;
class TAnimatedSprite ;
class TSound ;
class TSoundInstance ;
class TModel ;
class TAsset ;
class TMessage;
class TSprite ;
class TTextSprite ;
class TScriptCode ;

class TWindow;
class TModalWindow;

#endif

typedef shared_ptr<TTexture> 			TTextureRef ;			///< A reference to a TTexture
typedef shared_ptr<TAnimatedTexture> 	TAnimatedTextureRef ;	///< A reference to a TAnimatedTexture
typedef shared_ptr<TSprite> 			TSpriteRef ;			///< A reference to a TSprite
typedef shared_ptr<TTextSprite>			TTextSpriteRef ;		///< A reference to a TTextSprite
typedef shared_ptr<TAnimatedSprite>		TAnimatedSpriteRef ;	///< A reference to a TAnimatedSprite
typedef shared_ptr<TScriptCode> 		TScriptCodeRef ;		///< A reference to a TScriptCode object
typedef shared_ptr<TSound> 				TSoundRef ;				///< A reference to a TSound
typedef shared_ptr<TSoundInstance>		TSoundInstanceRef ;		///< A reference to a TSoundInstance
typedef shared_ptr<TModel> 				TModelRef ;				///< A reference to a TModel
typedef shared_ptr<TAsset>				TAssetRef ;				///< A reference to a TAsset

#endif // FORWARD_H_INCLUDED
