-- Define our particle characteristics
pVelocity = Allocate(2) ; -- Allocate a Vec2 velocity member
pAge = Allocate(1) ; -- Allocate a TReal age member
pSpin = Allocate(1); -- Allocate a TReal spin member
pSpinSpeed = Allocate(1); -- Allocate a TReal spin member

-- Set the particle texture
SetTexture("star");
SetBlendMode(kBlendNormal);

SetNumParticles(3000);    -- Create some particles

if not dLocus then
   dLocus= Vec2(0,0) ;
end

pPosition:Init( fPick( Vec2(-200,0.2), Vec2(200,0) ) + dLocus );
pVelocity:Init( fRange( Vec2(-200,-500), Vec2(200,-120) ) );
pColor:Init( Color(1,1,1,1) );
pScale:Init( 0.5 );
pAge:Init(0);
pSpin:Init( fRange( 0, 2*3.1415927 ) );
pSpinSpeed:Init( fRange( -10, 10 ) );

pPosition:Anim( pPosition + fTimeScale(pVelocity) );
pPosition:Anim( pPosition );
pScale:Anim( pScale + fTimeScale(0.4) );
pVelocity:Anim( pVelocity + fTimeScale(Vec2(0,800)) );
pColor:Anim( fFade( pAge,Color(1,1,1,1), 500, Color(1,0,0,1), 1000, Color(0,0,0,0) ) );
pSpin:Anim( pSpin + fTimeScale( pSpinSpeed ) );
pUp:Anim( f2dRotation( pSpin ) );
-- Add time to the age
pAge:Anim( pAge+fAge() );

-- Expire particles over 1500 ms old
Anim( fExpire( fGreater(pAge,1500) ) );

-- Create an initial particle
CreateParticles( 1 );

-- A function to run as we're executing
-- seconds - how many seconds have elapsed
function Update(seconds)
   -- Create 10 particles per second
   CreateParticles( seconds * 300 );
end

