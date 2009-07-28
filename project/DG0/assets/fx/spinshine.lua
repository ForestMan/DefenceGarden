-- ------------------------------------------------------------
-- Define our particle characteristics

pVelocity = Allocate(2) ; -- Allocate a Vec2 velocity member
pAge = Allocate(1) ; -- Allocate a TReal age member
pSpin = Allocate(1); -- Allocate a TReal spin member
pSpinSpeed = Allocate(1); -- Allocate a TReal spin member

-- Set the particle texture
SetTexture("snowflake");

-- Set the blend mode
SetBlendMode(kBlendNormal);

-- LAST, we set the total number of particles in the particle
-- system. This doesn't 'create' the particles, but rather
-- allocates a block of 'dead' particles that can be created
-- below.
SetNumParticles(400);

-- ------------------------------------------------------------
-- Initialization rules.
-- 
-- The following rules define how particles are created.
--
-- Any parameter below in initialization or animation rules that 
-- is a scalar, Color(), or Vec2() can alternaly take a function 
-- that returns the same type--and most functions can take any type
-- and will return that same type. The exception is any function
-- that's expecting a scalar in certain parameter positions.

-- Define dLocus for FluidFX; in the game it's defined in 
-- swarm.cpp as a data source, and it gets the mouse position.
if not dLocus then
   dLocus= Vec2(0,0) ; 
end

-- Pick a position randomly, within a range of values.
-- dLocus is defined in swarm.cpp and added to the random
-- range
pPosition:Init( fRange( Vec2(-10,-10), Vec2(10,10) )+dLocus );

-- Pick a velocity within a range
pVelocity:Init( fRange( Vec2(-50,-200), Vec2(50,-320) ) );

-- Start scale at 0.5
pScale:Init( 0.5 );

-- Start age as 0 milliseconds
pAge:Init(0);

-- Start initial rotation as a random angle
pSpin:Init( fRange( 0, 2*3.1415927 ) );

-- Start spin velocity random from -10 to 10
pSpinSpeed:Init( fRange( -3, 3 ) );

-- ------------------------------------------------------------
-- Particle Parameter Animation Functions
--
-- The following rules define how particles animate.
-- Each rule is applied on each frame of animation.

-- Add velocity scaled by time to position
pPosition:Anim( pPosition + fTimeScale(pVelocity) );

-- Add 1/second to scale
pScale:Anim( pScale + fTimeScale(1) );

-- Add gravity scaled by time to velocity
pVelocity:Anim( pVelocity + fTimeScale(Vec2(0,250)) );

-- Fade (using age) from white to yellow to black. Assumes
-- the first parameter is a number of elapsed milliseconds,
-- though can work to linearly interpolate using first 
-- scalar parameter between any value types.
pColor:Anim( 
   fFade( pAge,
     Color(0,0,1,1), 500, -- set color, fade over 300ms
     Color(1,1,0,1), 300, -- to second color, fade 300ms
     Color(1,0.5,0,1), 100, -- to third color, fade 200ms
     Color(1,0.4,0,0) ) -- to final color
   );

-- Spin by pSpinSpeed scaled by time
pSpin:Anim( pSpin + fTimeScale( pSpinSpeed ) );

-- Set the particle's "Up" vector to the current user variable
-- "pSpin"
pUp:Anim( f2dRotation( pSpin ) );

-- Age the particle using age function
pAge:Anim( pAge+fAge() );

-- General Anim functions
-- Expire particles over 700 ms old
Anim( fExpire( fGreater(pAge,1000) ) );

-- ------------------------------------------------------------
-- Create initial particles (optional)
-- CreateParticles( 1 );

-- ------------------------------------------------------------
-- Update function: A short bit of Lua that can be used to
-- create particles or change rules at run-time. Rule changing is
-- expensive and would require you to reset the current rule set,
-- so is not recommended as a frequent operation.

-- A global variable that we can set from C++
gActive = true

-- A function to run as we're executing
-- seconds - how many seconds have elapsed
function Update(seconds)
   if gActive then
      -- Create 80 particles per second
      CreateParticles( seconds * 80 );
   end
end

