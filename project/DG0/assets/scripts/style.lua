standardFont = "fonts/arial.mvec";

BlackColor = Color(0,0,0,255);
BlueColor = Color(77,115,178,255);
DarkBlueColor = Color(40,60,120,255);
WhiteColor = Color(255,255,255,255);
MenuButtonFontColor = Color(255,255,255,255);

StandardButtonGraphics = {
	"buttons/buttonup",
	"buttons/buttondown",
	"buttons/buttonrollover"
};

LongButtonGraphics = {
	"buttons/long_button_up.png",
	"buttons/long_button_down.png",
	"buttons/long_button_over.png"
};

CheckboxButtonGraphics = {
	"buttons/checkup",
	"buttons/checkdown",
	"buttons/checkover",
	"buttons/checkdownover"
};

StandardButtonFont = {
  standardFont,
  18,
  BlackColor
};

MenuButtonFont = {
  standardFont,
  22,
  MenuButtonFontColor
};

DialogTitleFont = {
  standardFont,
  20,
  BlackColor
};

DialogBodyFont = {
  standardFont,
  18,
  BlackColor
};

DefaultStyle = {
	font=DialogBodyFont
};

DialogTitleText = {
	parent=DefaultStyle,
	font = DialogTitleFont,
	flags = kVAlignCenter + kHAlignLeft,
	x=16,y=12,w=kMax,h=30
};

DialogBodyText = {
	parent=DefaultStyle,
	font = DialogBodyFont,
	flags = kVAlignCenter + kHAlignCenter,
	x=4,y=46,w=kMax-4,h=kMax-60
};

SliderStyle =
{
	parent=DefaultStyle,
	railtop = "slider/sliderrailtop",
	railmid = "slider/sliderrailmid",
	railbot = "slider/sliderrailbot",
	sliderimage = "slider/sliderknob",
	sliderrollimage = "slider/sliderknobover",

	yoffset = -1, -- scoot the slider down by one pixel to center it.
};

kDefaultButtonSound="audio/sfx/buttonclick.ogg";

MenuButtonStyle = {
	parent = DefaultStyle,
	font = MenuButtonFont,
	sound = kDefaultButtonSound,
	graphics = {},
	w=169, h=40
};

ButtonStyle = {
	parent = DefaultStyle,
	font = StandardButtonFont,
	sound = kDefaultButtonSound,
	type= kPush,
	graphics = StandardButtonGraphics,
};

LongButtonStyle = {
	parent = ButtonStyle,
	graphics = LongButtonGraphics
};

CheckboxButtonStyle = {
	parent = DefaultStyle,
	font = StandardButtonFont,
	type = kToggle,
	sound = kDefaultButtonSound,
	graphics = CheckboxButtonGraphics,
	tx=28,
};

SetDefaultStyle(DefaultStyle);

-- Create a TButton with a label
--
-- We're overriding the default "Button" here with
-- some custom behavior. Specifically, we're layering
-- a Text{} object on top of every button, and it's placed
-- on all layers (up, down, roll-over).
function Button( button )
	return function()
		-- Grab the label from the button or current style
		if (GetTag(button,"label")) then

			-- Look for text dimensions
			local tx = GetTag(button,"tx");
			local ty = GetTag(button,"ty");
			local tw = GetTag(button,"tw");
			local th = GetTag(button,"th");

			-- Set defaults
			if (not tx) then
				tx = 0;
			end
			if (not ty) then
				ty = 0;
			end
			if (not tw) then
				tw=kMax ;
			end
			if (not th) then
				th=kMax ;
			end

-- 4.0.15 -- Begin new hotkey support
			-- Get the label
			local label = GetTag(button,"label");
			local hotkey = GetTag(button,"hotkey");

			label,labelkey = StripKeyFromLabel( GetString(label) );

			-- If no hotkey was specified explicitly, then
			-- use the translated key.
			if not hotkey then
				-- change our local
				hotkey = labelkey
			end;

			RegisterHotkey( hotkey, GetTag(button,"name") );
-- 4.0.15 -- End new hotkey support (more below!)

			local defflags = kPushButtonAlignment ;
			if (GetTag(button,"type")==kToggle) then
				defflags = kToggleButtonAlignment ;
			elseif (GetTag(button,"type")==kRadio) then
				defflags = kRadioButtonAlignment ;
			end

			local textfactory = GetTag(button,"textfactory");
			if (not textfactory) then
				textfactory = "_text"
			end
			table.insert(button,
				SelectLayer( kAllLayers )
			);
			-- Grow our window to encompass any children we've added
			-- already.
			table.insert(button,
				FitToChildren()
			);

			table.insert(button,
				AppendStyle{ font=button.font; flags=button.flags; }
			);

			table.insert(button,
				Text
				{
-- 4.0.15 -- Begin new hotkey support
					label="#"..TagHotKey( hotkey, label ),
-- 4.0.15 -- End new hotkey support
					x=tx, y=ty, w=tw, h=th,
					name='label',
					defflags =defflags
				}
			)
		end

		-- Type name of window to create; in our case, "Button".
		-- If you want to create a derived button type, you can do that here.
		button.typename='Button';
		DoWindow( button )
	end
end

-- Create several bits of text that combine to make a blended shadow
function ColorShadowText( x,y,size, label, color )

	return Group(
		{
		-- Add these traits to the current style
			AppendStyle{ x=x, y=y, w=kMax, h=kMax, label=label },
			AppendStyle{ font = { standardFont, size, Color(0,0,0,40)} },
			Text{ y=y+4 },
			AppendStyle{ font = { standardFont, size, Color(0,0,0,255)} },
			Text{ y=y+2 },
			AppendStyle{ font = { standardFont, size, color } } ,
			Text{},
		}
	)
end

kMenuButtonFontSize = 22;

function MenuButtonLabel( label, color, colorHighlight )

	if not color then
		color =Color(192,207,255,255);	-- the default colors
		colorHighlight=Color(255,255,255,255)
	end

	return Group{
		Bitmap
		{
			image="backgrounds/bar",
			x=0,y=0,scale=1
		},
		SelectLayer(0),
		ColorShadowText(5,8,kMenuButtonFontSize, label, color ),
		SelectLayer(1),
		ColorShadowText(5,8,kMenuButtonFontSize, label, colorHighlight ),
		SelectLayer(2),
		ColorShadowText(5,8,kMenuButtonFontSize, label, colorHighlight )
	}

end

-- Create a more limited-use button type here; the label is defined inline
-- with MenuButtonLabel above
function MenuButton( button )
	return function()
		-- Type name of window to create; in our case, "Button".
		-- If you want to create a derived button type, you can do that here.
		button.typename='Button';
		DoWindow( button )
	end
end

kIllegalNameChars = "!@#$%^&*()><\\\"\'[]{}|?/+=~`.,;:-";