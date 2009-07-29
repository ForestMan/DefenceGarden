//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------

#ifndef ASSETMAP_H_INCLUDED
#define ASSETMAP_H_INCLUDED

#ifndef FORWARD_H_INCLUDED
#include "forward.h"
#endif

#ifndef STR_H_INCLUDED
#include "str.h"
#endif

#include <map>

/**
 * A collection of assets that simplifies asset reference-holding.
 *
 * If your game has sections or levels that use a particular set of
 * assets, you can allocate them and store them into a TAssetMap.
 * Then when the level is complete you can release the TAssetMap.
 * This will prevent any glitches from dynamically loading art while
 * the game is running.
 *
 * Simple usage looks like:
 *
 * @code
 * // In your class:
 * TAssetMap mMyMap ;
 *
 * // In your initialization code:
 * mMyMap.AddAsset("image1");
 * mMyMap.AddAsset("image2");
 *
 * // Later in your game code:
 * TTextureRef myImage = mMyMap.GetTexture("image1");
 *
 * @endcode
 *
 * After you've referenced an image, calling TTexture::Get() on that image
 * will retrieve the reference; however, calling TAssetMap::GetTexture
 * will help you in one of two ways, depending on the state of
 * TAssetMap::SetAutoLoad():
 *
 * - If SetAutoLoad is false (default), it will ASSERT in debug build that
 *   the texture isn't found if you forgot to add it to the map.
 * - If SetAutoLoad is true, it will load it and <i>hold</i> the reference until
 *   the map is destroyed. That way the next time you reference it, it will be able
 *   to retrieve the already loaded version, even if you've released the in-game
 *   reference.
 *
 * @see \ref assets
 * @see TAsset
 */
class PFLIB_API TAssetMap
{
public:
	/// Default Constructor
	TAssetMap();

	/// Destructor
	virtual ~TAssetMap();

	/**
	 * Activate auto-loading of textures.
	 *
	 * When this flag is false, requesting a texture that doesn't exist
	 * will assert. When it's true, it will simply attempt to load it on
	 * demand.
	 *
	 * @param autoLoad True to auto-load.
	 */
	void SetAutoLoad(bool autoLoad)
	{
		mAutoLoad = autoLoad ;
	}

	/**
	 * Add an asset to the map.
	 *
	 * @param assetHandle
	 *               Name of asset to add.
	 * @param asset  Pointer to asset to associate with assetHandle. Optional.
     * @param silent True to supress warnings.
	 */
	void AddAsset( str assetHandle, TAssetRef asset= TAssetRef(), bool silent=false );

	/**
	 * Add an array of assets to the map. Array is NULL terminated.
	 *
	 * For example:
	 * @code
	 * const char * assets[]=
	 * {
	 *	"checkers/flipper",
	 *	"checkers/exploder",
	 *	"checkers/heavy",
	 *	"checkers/helium",
	 *	"checkers/rowClear",
	 *	NULL
     * }
     * 
	 * TAssetMap map;
	 * map.AddAssets(assets);
     * @endcode
     * 
     * Note you can add flags to TSound and TTexture references.
     * See TSound::Get() and TTexture::Get() for details.
     * 
	 * @param assets Array of assets to add.
	 */
	void AddAssets( const char * assets[] );

	/**
	 * Get a sound asset from the map.
	 *
     * @param asset  Asset handle to retrieve. Must be exactly the
     *               same string that was specified to add the
     *               asset.
	 *
     * @return A TSoundRef. Will be empty (NULL) if the asset is not
	 *         found, or is of an improper type.
	 */
	TSoundRef GetSound( str asset );

	/**
	 * Get a model asset from the map.
	 *
     * @param asset  Asset handle to retrieve. Must be exactly the
     *               same string that was specified to add the
     *               asset.
	 *
     * @return A TModelRef. Will be empty (NULL) if the asset is not
     *         found, or is of an improper type.
	 */
	TModelRef GetModel( str asset );

	/**
	 * Get a texture asset from the map.
	 *
     * @param asset  Asset handle to retrieve. Must be exactly the
     *               same string that was specified to add the
     *               asset.
	 *
     * @return A TTextureRef. Will be empty (NULL) if the asset is
     *         not found, or is of an improper type. Note that if
     *         you use GetTexture() to retrieve a TAnimatedTexture,
     *         it will succeed and return the TAnimatedTextureRef
     *         cast to a TTextureRef.
	 */
	TTextureRef GetTexture( str asset );

	/**
	 * Get an animated texture asset from the map.
	 *
     * @param asset  Asset handle to retrieve. Must be exactly the
     *               same string that was specified to add the
     *               asset.
	 *
     * @return A TAnimatedTextureRef. Will be empty (NULL) if the asset is not
	 *         found, or is of an improper type.
	 */
	TAnimatedTextureRef GetAnimatedTexture( str asset );

	/**
	 * Get a reference to a pre-loaded, pre-compiled Lua script.
	 *
     * @param asset  Asset handle to retrieve. Must be exactly the
     *               same string that was specified to add the
     *               asset.
	 *
	 * @return A TScriptCodeRef. Will be empty (NULL) if the asset is not
	 *         found, or is of an improper type.
	 */
	TScriptCodeRef GetScriptCode( str asset );

	/**
	 * Query the TAssetMap for an asset; will return true if an
     * asset has been registered successfully with the map.
	 * 
	 * @param asset  Asset to query.
	 * 
	 * @return True if found.
	 */
	bool QueryAsset( str asset );

	/**
     * Release ALL managed assets. All previously added assets are
     * released and their records are purged. After calling this you
     * can again add assets as usual.
     *
     * Note that assets that are still referenced elsewhere in your
     * code (or by, e.g., existing TSpriteRefs) will not be
     * destroyed.
	 *
	 * A release will happen implicitly on destruction of the TAssetMap.
	 */
	void Release();

private:
	std::map<str,TAssetRef> 	mAssetMap ;
	bool						mAutoLoad ;
};

#endif // ASSETMAP_H_INCLUDED
