-- Set the particle texture
SetTexture("star");
SetBlendMode(kBlendNormal);

SetNumParticles(16000);    -- Create some particles

pPosition:Init( fRange( Vec2(-400,-300), Vec2(400,-320) ) + dLocus );
pColor:Init( Color(1,1,1,1) );
pScale:Init( 0.2 );
pUp:Init( f2dRadius( 1 ) );   -- random radius

-- Move the particles
pPosition:Anim( pPosition + fTimeScale( Vec2(0,60) ) );

-- Expire particles lower than 300
Anim( fExpire( fGreater(pPosition[2],dLocus[2]+300) ) );

-- A function to run as we're executing
-- seconds - how many seconds have elapsed
function Update(seconds)
   -- Create 10 particles per second
   CreateParticles( seconds * 1600 );
end

