#pragma once

#include <pf/window.h>

class GameWindow : public TWindow
{
	PFTYPEDEF_DC(GameWindow,TWindow)
public:
	GameWindow();
	~GameWindow();

	void Draw();

	/**
	 * Send a message to the game.
	 *
	 * @param message Game message.
	 */
	void SendGameMessage( str message );

	/**
	 * Mouse down handler.
	 *
	 * @param point      Location of mouse press in client coordinates.
	 *
	 * @return true if message was handled, false to keep searching
	 *         for a handler.
	 */
	virtual bool OnMouseDown(const TPoint &point);
	/**
	 * Initialize the Window. Called by the system only in
	 * Lua initialization.
	 *
	 * When you create your own custom window, this is where you put
	 * your own custom initialization that needs to happen before
	 * children are created. Fundamental window initialization is
	 * handled in every class by this function, so <b>when you
	 * override this function you almost always want to call your
	 * base class to handle base class initialization.</b>
	 */
	virtual void Init(TWindowStyle &style);

	/**
	 * Called if you have initiated a window animation with
	 * TWindow::StartWindowAnimation.
	 *
	 * @return True to continue animating. False to stop.
	 */
	virtual bool OnTaskAnimate();

	virtual bool OnChar(char key);
private:

	int	mRows;		// Number of rows in the square
	int mColumns;	// Number of columns in the square

	TReal mOffset ;

	TTextureRef	mBigTex;

	class TTextGraphic * mTG ;
};