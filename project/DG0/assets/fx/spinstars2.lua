gBlendMode = GUI_Select(
   kBlendNormal,
   "Blend Mode",
   {
      { description="Normal", value=kBlendNormal},
      { description="Additive Alpha", value=kBlendAdditiveAlpha},
      { description="Subtractive", value=kBlendSubtractive},
      { description="Multiplicative", value=kBlendMultiplicative},
      { description="Opaque", value=kBlendOpaque}
   }
);

gGravity = GUI_Slider(
   1064.00,
   "Gravity",
   0,2000);

gTexture = GUI_PickTexture("star","Texture");


gParticlesPerSecond = GUI_Slider(
   20.50,
   "Particle Creation Rate",
   1,200);

gXLaunchRange1 = GUI_Slider(
   -687.86,
   "X minimum Launch Velocity",
   -1000,1000);

gXLaunchRange2 = GUI_Slider(
   514.45,
   "X Maximum Launch Velocity",
   -1000,1000);

gYLaunchRange1 = GUI_Slider(
   179.19,   "Y Minimum Launch Velocity",   -1000,1000);

gYLaunchRange2 = GUI_Slider(
   -526.01,   "Y Maximum Launch Velocity",   -1000,1000);

gFadeSpeed = GUI_Slider(
   809.25,
   "Fade-out Duration",
   0,1000);

gDelayFade = GUI_Slider(
   -69.36,
   "Fade-out start",
   -1000,0);


-- ------------------------------------------------------------
-- Initialization phase

-- Define our custom particle properties
pVelocity = Allocate(2) ; -- Allocate a Vec2 velocity member
pAge = Allocate(1) ; -- Allocate a TReal age member
pSpin = Allocate(1); -- Allocate a TReal spin member
pSpinSpeed = Allocate(1); -- Allocate a TReal spin member

-- dLocus is a data source: An external connection to the particle
-- system. Here we define dLocus for FluidFX, since FluidFX doesn't know
-- to provide the data source; in the game it's defined in
-- swarm.cpp as a data source, and it gets the mouse position.
if not dLocus then
   dLocus= Vec2(0,0) ;
end

-- Set the particle texture
SetTexture(gTexture);

-- Set the blend mode
SetBlendMode(gBlendMode);

-- LAST, we set the total number of particles in the particle
-- system. This doesn't 'create' the particles, but rather
-- allocates a block of 'dead' particles that can be created
-- below.
SetNumParticles(400);

-- ------------------------------------------------------------
-- Action Phase

-- Particle Initialization Rules.
--
-- The following rules define how particles are created.
--
-- Any parameter below in initialization or animation rules that
-- is a scalar, Color(), or Vec2() can alternaly take a function
-- that returns the same type--and most functions can take any type
-- and will return that same type. The exception is any function
-- that's expecting a scalar in certain parameter positions.

-- Pick one of two positions randomly, and add dLocus.
-- dLocus is defined in swarm.cpp and added to the random
-- range
pPosition:Init( fPick( Vec2(-10,0), Vec2(10,0) ) + dLocus );

-- Pick a velocity from a range
pVelocity:Init( fRange( Vec2(gXLaunchRange1,gYLaunchRange1), Vec2(gXLaunchRange2,gYLaunchRange2) ) );

-- Start color (tint) off as white (natural color of image)
pColor:Init( Color(1,1,1,1) );

-- Start scale out as 0.5
pScale:Init( 0.5 );

-- Start age as 0 milliseconds
pAge:Init(0);

-- Start initial rotation as a random angle
pSpin:Init( fRange( 0, 2*3.1415927 ) );

-- Start spin velocity random from -10 to 10
pSpinSpeed:Init( fRange( -10, 10 ) );

-- ------------------------------------------------------------
-- Particle Parameter Animation Functions
--
-- The following rules define how particles animate.
-- Each rule is applied on each frame of animation.

-- Add velocity scaled by time to position
pPosition:Anim( pPosition + fTimeScale(pVelocity) );

pScale:Anim( pScale + fTimeScale(1) );
pVelocity:Anim( pVelocity + fTimeScale(Vec2(0,gGravity)) );
pColor:Anim( fFade( pAge+gDelayFade,Color(1,1,1,1), gFadeSpeed, Color(0,0,0,0) ) );
pSpin:Anim( pSpin + fTimeScale( pSpinSpeed ) );
pUp:Anim( f2dRotation( pSpin ) );
-- Add time to the age
pAge:Anim( pAge+fAge() );

-- Expire particles over 2500 ms old
Anim( fExpire( fGreater(pAge,2500) ) );

-- Create an initial particle (optional)
-- CreateParticles( 1 );

-- A global variable that we can set from outside
gActive = true

-- A function to run as we're executing
-- seconds - how many seconds have elapsed
function Update(seconds)
   if gActive then
      -- Create 10 particles per second
      CreateParticles( seconds * gParticlesPerSecond );
   end
end

