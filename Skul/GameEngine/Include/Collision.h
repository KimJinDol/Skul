#pragma once

#include "GameEngine.h"

class CCollision
{
public:
	static bool CollisionBox2DToBox2D(HitResult& SrcResult, HitResult& DestResult,
		class CColliderBox2D* Src, class CColliderBox2D* Dest);
	static bool CollisionCircleToCircle(HitResult& SrcResult, HitResult& DestResult,
		class CColliderCircle* Src, class CColliderCircle* Dest);
	static bool CollisionBox2DToCircle(HitResult& SrcResult, HitResult& DestResult,
		class CColliderBox2D* Src, class CColliderCircle* Dest);
	static bool CollisionBox2DToPixel(HitResult& SrcResult, HitResult& DestResult,
		class CColliderBox2D* Src, class CColliderPixel* Dest);
	static bool CollisionCircleToPixel(HitResult& SrcResult, HitResult& DestResult,
		class CColliderCircle* Src, class CColliderPixel* Dest);
	static bool CollisionLineToBox2D(HitResult& SrcResult, HitResult& DestResult,
		class CColliderLine* Src, class CColliderBox2D* Dest);
    static bool CollisionLineToCircle(HitResult& SrcResult, HitResult& DestResult,
        class CColliderLine* Src, class CColliderCircle* Dest);


public:
	static bool CollisionBox2DToBox2D(HitResult& SrcResult, HitResult& DestResult, 
		const Box2DInfo& Src, const Box2DInfo& Dest);
	static bool CollisionCircleToCircle(HitResult& SrcResult, HitResult& DestResult,
		const CircleInfo& Src, const CircleInfo& Dest);
	static bool CollisionBox2DToCircle(HitResult& SrcResult, HitResult& DestResult,
		const Box2DInfo& Src, const CircleInfo& Dest);
	static bool CollisionBox2DToPixel(HitResult& SrcResult, HitResult& DestResult,
		const Box2DInfo& Src, const PixelInfo& Dest);
	static bool CollisionCircleToPixel(HitResult& SrcResult, HitResult& DestResult,
		const CircleInfo& Src, const PixelInfo& Dest);
	static bool CollisionBox2DToPoint(HitResult& SrcResult, HitResult& DestResult,
		const Box2DInfo& Src, const Vector2& Dest);
	static bool CollisionCircleToPoint(HitResult& SrcResult, HitResult& DestResult,
		const CircleInfo& Src, const Vector2& Dest);
	static bool CollisionPixelToPoint(HitResult& SrcResult, HitResult& DestResult,
		const PixelInfo& Src, const Vector2& Dest);
	static bool CollisionLineToBox2D(HitResult& SrcResult, HitResult& DestResult,
		const LineInfo& Src, const Box2DInfo& Dest);
	static bool CollisionLineToLine(HitResult& SrcResult, HitResult& DestResult,
		const LineInfo& Src, const LineInfo& Dest);
    static bool CollisionLineToCircle(HitResult& SrcResult, HitResult& DestResult,
        const LineInfo& Src, const CircleInfo& Dest);
};

