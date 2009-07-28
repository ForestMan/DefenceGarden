-- Define our particle characteristics
pAge = Allocate(1) ; -- Allocate a TReal age member
pSpin = Allocate(1); -- Allocate a TReal spin member
pRadius = Allocate(1); -- Allocate a TReal radius member
pSpinSpeed = Allocate(1); -- Allocate a TReal spin member

-- Set the particle texture
SetTexture("star");
SetBlendMode(kBlendNormal);

SetNumParticles(400);    -- Create some particles

pRadius:Init( fRange( 20,60 ) );
pColor:Init( Color(1,1,1,1) );
pScale:Init( 0.5 );
pAge:Init(0);
pSpin:Init( fRange( 0, 2*3.1415927 ) );
pSpinSpeed:Init( fRange( -10, 10 ) );

pPosition:Anim( f2dRotation(pSpin)*pRadius + dLocus );
pPosition:Anim( pPosition );
pScale:Anim( pScale + fTimeScale(1) );
pColor:Anim( fFade( pAge,Color(1,1,1,1), 500, Color(1,0,0,1), 1000, Color(0,0,0,0) ) );
pSpin:Anim( pSpin + fTimeScale( pSpinSpeed ) );
pUp:Anim( f2dRotation( pSpin ) );
-- Add time to the age
pAge:Anim( pAge+fAge() );

-- Expire particles over 1500 ms old
Anim( fExpire( fGreater(pAge,2500) ) );

-- Create an initial particle
CreateParticles( 1 );

-- A function to run as we're executing
-- seconds - how many seconds have elapsed
function Update(seconds)
   -- Create 10 particles per second
   CreateParticles( seconds * 10 );
end

