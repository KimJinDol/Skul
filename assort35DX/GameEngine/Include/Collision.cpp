#include "Collision.h"
#include "Component/ColliderBox2D.h"
#include "Component/ColliderCircle.h"
#include "Component/ColliderPixel.h"
#include "Component/ColliderLine.h"
#include <math.h>

bool CCollision::CollisionBox2DToBox2D(HitResult& SrcResult, HitResult& DestResult, 
	CColliderBox2D* Src, CColliderBox2D* Dest)
{
	if (CollisionBox2DToBox2D(SrcResult, DestResult, Src->GetInfo(), Dest->GetInfo()))
	{
		SrcResult.DestCollider = Dest;
		SrcResult.DestObject = Dest->GetOwner();

		DestResult.DestCollider = Src;
		DestResult.DestObject = Src->GetOwner();

		return true;
	}

	SrcResult.DestCollider = nullptr;
	SrcResult.DestObject = nullptr;

	DestResult.DestCollider = nullptr;
	DestResult.DestObject = nullptr;

	return false;
}

bool CCollision::CollisionCircleToCircle(HitResult& SrcResult, HitResult& DestResult, 
	CColliderCircle* Src, CColliderCircle* Dest)
{
	if (CollisionCircleToCircle(SrcResult, DestResult, Src->GetInfo(), Dest->GetInfo()))
	{
		SrcResult.DestCollider = Dest;
		SrcResult.DestObject = Dest->GetOwner();

		DestResult.DestCollider = Src;
		DestResult.DestObject = Src->GetOwner();
		return true;
	}

	SrcResult.DestCollider = nullptr;
	SrcResult.DestObject = nullptr;

	DestResult.DestCollider = nullptr;
	DestResult.DestObject = nullptr;

	return false;
}

bool CCollision::CollisionBox2DToCircle(HitResult& SrcResult, HitResult& DestResult, 
	CColliderBox2D* Src, CColliderCircle* Dest)
{
	if (CollisionBox2DToCircle(SrcResult, DestResult, Src->GetInfo(), Dest->GetInfo()))
	{
		SrcResult.DestCollider = Dest;
		SrcResult.DestObject = Dest->GetOwner();

		DestResult.DestCollider = Src;
		DestResult.DestObject = Src->GetOwner();
		return true;
	}

	SrcResult.DestCollider = nullptr;
	SrcResult.DestObject = nullptr;

	DestResult.DestCollider = nullptr;
	DestResult.DestObject = nullptr;

	return false;
}

bool CCollision::CollisionBox2DToPixel(HitResult& SrcResult, HitResult& DestResult, 
	CColliderBox2D* Src, CColliderPixel* Dest)
{
	if (CollisionBox2DToPixel(SrcResult, DestResult, Src->GetInfo(), Dest->GetInfo()))
	{
		SrcResult.DestCollider = Dest;
		SrcResult.DestObject = Dest->GetOwner();

		DestResult.DestCollider = Src;
		DestResult.DestObject = Src->GetOwner();
		return true;
	}

	SrcResult.DestCollider = nullptr;
	SrcResult.DestObject = nullptr;

	DestResult.DestCollider = nullptr;
	DestResult.DestObject = nullptr;

	return false;
}

bool CCollision::CollisionCircleToPixel(HitResult& SrcResult, HitResult& DestResult, 
	CColliderCircle* Src, CColliderPixel* Dest)
{
	if (CollisionCircleToPixel(SrcResult, DestResult, Src->GetInfo(), Dest->GetInfo()))
	{
		SrcResult.DestCollider = Dest;
		SrcResult.DestObject = Dest->GetOwner();

		DestResult.DestCollider = Src;
		DestResult.DestObject = Src->GetOwner();
		return true;
	}

	SrcResult.DestCollider = nullptr;
	SrcResult.DestObject = nullptr;

	DestResult.DestCollider = nullptr;
	DestResult.DestObject = nullptr;

	return false;
}

bool CCollision::CollisionLineToBox2D(HitResult& SrcResult, HitResult& DestResult, CColliderLine* Src, CColliderBox2D* Dest)
{
	if (CollisionLineToBox2D(SrcResult, DestResult, Src->GetInfo(), Dest->GetInfo()))
	{
		SrcResult.DestCollider = Dest;
		SrcResult.DestObject = Dest->GetOwner();

		DestResult.DestCollider = Src;
		DestResult.DestObject = Src->GetOwner();
		return true;
	}

	SrcResult.DestCollider = nullptr;
	SrcResult.DestObject = nullptr;

	DestResult.DestCollider = nullptr;
	DestResult.DestObject = nullptr;

	return false;
}

bool CCollision::CollisionLineToCircle(HitResult& SrcResult, HitResult& DestResult, CColliderLine* Src, CColliderCircle* Dest)
{
    if (CollisionLineToCircle(SrcResult, DestResult, Src->GetInfo(), Dest->GetInfo()))
    {
        SrcResult.DestCollider = Dest;
        SrcResult.DestObject = Dest->GetOwner();

        DestResult.DestCollider = Src;
        DestResult.DestObject = Src->GetOwner();
        return true;
    }

    SrcResult.DestCollider = nullptr;
    SrcResult.DestObject = nullptr;

    DestResult.DestCollider = nullptr;
    DestResult.DestObject = nullptr;

    return false;
}

bool CCollision::CollisionBox2DToBox2D(HitResult& SrcResult, HitResult& DestResult, 
	const Box2DInfo& Src, const Box2DInfo& Dest)
{
	Vector2 CenterDir = Src.Center - Dest.Center;

	// Src�� X������ ����
	Vector2 Axis = Src.Axis[AXIS_X];

	float	CenterDot = abs(CenterDir.Dot(Axis));

	float   r1, r2;

	r1 = Src.Length[0];
	r2 = abs(Dest.Axis[AXIS_X].Dot(Axis)) * Dest.Length[0] +
		abs(Dest.Axis[AXIS_Y].Dot(Axis)) * Dest.Length[1];

	if (CenterDot > r1 + r2)
		return false;

	Axis = Src.Axis[AXIS_Y];

	CenterDot = abs(CenterDir.Dot(Axis));

	r1 = Src.Length[1];
	r2 = abs(Dest.Axis[AXIS_X].Dot(Axis)) * Dest.Length[0] +
		abs(Dest.Axis[AXIS_Y].Dot(Axis)) * Dest.Length[1];

	if (CenterDot > r1 + r2)
		return false;

	Axis = Dest.Axis[AXIS_X];

	CenterDot = abs(CenterDir.Dot(Axis));

	r1 = Dest.Length[0];
	r2 = abs(Src.Axis[AXIS_X].Dot(Axis)) * Src.Length[0] +
		abs(Src.Axis[AXIS_Y].Dot(Axis)) * Src.Length[1];

	if (CenterDot > r1 + r2)
		return false;

	Axis = Dest.Axis[AXIS_Y];

	CenterDot = abs(CenterDir.Dot(Axis));

	r1 = Dest.Length[1];
	r2 = abs(Src.Axis[AXIS_X].Dot(Axis)) * Src.Length[0] +
		abs(Src.Axis[AXIS_Y].Dot(Axis)) * Src.Length[1];

	if (CenterDot > r1 + r2)
		return false;

	return true;
}

bool CCollision::CollisionCircleToCircle(HitResult& SrcResult, HitResult& DestResult,
	const CircleInfo& Src, const CircleInfo& Dest)
{
	float	Distance = Src.Center.Distance(Dest.Center);

	bool	Result = Distance <= Src.Radius + Dest.Radius;

	if (Result)
	{
		Vector2	Center = (Src.Center + Dest.Center) / 2.f;
		SrcResult.HitPoint = Vector3(Center.x, Center.y, 0.f);
		DestResult.HitPoint = Vector3(Center.x, Center.y, 0.f);
	}

	return Result;
}

bool CCollision::CollisionBox2DToCircle(HitResult& SrcResult, HitResult& DestResult, 
	const Box2DInfo& Src, const CircleInfo& Dest)
{
	Vector2 CenterDir = Src.Center - Dest.Center;

	// Src�� X������ ����
	Vector2 Axis = Src.Axis[AXIS_X];
	Axis.Normalize();

	float	CenterDot = abs(CenterDir.Dot(Axis));

	float   r1, r2;

	r1 = Src.Length[0];
	r2 = Dest.Radius;

	if (CenterDot > r1 + r2)
		return false;

	Axis = Src.Axis[AXIS_Y];
	Axis.Normalize();

	CenterDot = abs(CenterDir.Dot(Axis));

	r1 = Src.Length[1];
	r2 = Dest.Radius;

	if (CenterDot > r1 + r2)
		return false;


	Axis = CenterDir;
	Axis.Normalize();

	CenterDot = abs(CenterDir.Dot(Axis));

	r1 = abs(Src.Axis[AXIS_X].Dot(Axis)) * Src.Length[0] +
		abs(Src.Axis[AXIS_Y].Dot(Axis)) * Src.Length[1];
	r2 = Dest.Radius;

	if (CenterDot > r1 + r2)
		return false;

	return true;
}

bool CCollision::CollisionBox2DToPixel(HitResult& SrcResult, HitResult& DestResult, 
	const Box2DInfo& Src, const PixelInfo& Dest)
{
	if (!CollisionBox2DToBox2D(SrcResult, DestResult, Src, Dest.Box))
		return false;

	// ������ �簢���� ���Ѵ�.
	float	Left = Src.Min.x < Dest.Min.x ? Dest.Min.x : Src.Min.x;
	float	Right = Src.Max.x > Dest.Max.x ? Dest.Max.x : Src.Max.x;
	float	Top = Src.Max.y > Dest.Max.y ? Dest.Max.y : Src.Max.y;
	float	Bottom = Src.Min.y < Dest.Min.y ? Dest.Min.y : Src.Min.y;

	// �ȼ� �浹ü�� �� �ϴ� ��ǥ�� ���Ѵ�.
	Vector2	LeftBottom = Dest.Box.Center - Vector2(Dest.Box.Length[0], Dest.Box.Length[1]);
	// LeftBottom�� �������ν� �ȼ� �浹ü�� LeftBottom�� �������� �Ͽ� ����� �� �ִ�. �̴� �ȼ��� �ε����� ����Ϸ��� ���� �ϴ� ����!
	Left -= LeftBottom.x;
	Right -= LeftBottom.x;

	Top -= LeftBottom.y;
	Bottom -= LeftBottom.y;

	Left = Left < 0.f ? 0.f : Left;
	Right = Right >= (float)Dest.Width ? (float)Dest.Width - 1 : Right; // �ε����� ó���� ���̱� ������ -1�� ���ִ� ��.

	Bottom = Bottom < 0.f ? 0.f : Bottom;
	Top = Top >= (float)Dest.Height ? (float)Dest.Height - 1 : Top;

	Top = Dest.Height - Top;		// Top�� Bottom�� �����Ѵ�.
	Bottom = Dest.Height - Bottom;

	// Top -> Bottom������.
	for (int y = (int)Top; y < (int)Bottom; ++y)
	{
		for (int x = (int)Left; x <= (int)Right; ++x)
		{
			int	Index = y * (int)Dest.Width * 4 + x * 4; // 4�� �����ִ� ������ �ȼ��� r, g, b, a �� 4����, �� ���� Dest.Width * 4���� �����Ǿ� �ֱ� ���÷�.

			switch (Dest.Type)
			{
			case PixelCollision_Type::Color_Ignore:
			{
				if (Dest.Pixel[Index] == Dest.Color[0] &&
					Dest.Pixel[Index + 1] == Dest.Color[1] &&
					Dest.Pixel[Index + 2] == Dest.Color[2])
					continue;

				// ������ ������ �ƴ϶�� �� �ȼ��� ���� Box2D �ȿ� ������ �Ǵ��Ѵ�.
				// �ȼ��� ����󿡼��� ��ġ�� ������.
                // �Ʊ� �ε����� ����ϱ� ���� LeftBottom�� ����� ������, �ٽ� �����ִ� ��.
				Vector2	PixelPos = LeftBottom + Vector2((float)x, (float)Dest.Height - y); // Dest.Height - y�� �ϴ� ������, 264��° �ٿ��� ž�� ������ ü���� �߱� ������ �� �� �� ü���� �ϴ� ����

				// ���� ������ �ȼ� ������ �ڽ�, �� �浹�� �̿��� ���� �浹�� �Ͼ���� �˻��Ѵ�.
				if (CollisionBox2DToPoint(SrcResult, DestResult, Src, PixelPos))
				{
					SrcResult.HitPoint = Vector3(PixelPos.x, PixelPos.y, 0.f);
					DestResult.HitPoint = Vector3(PixelPos.x, PixelPos.y, 0.f);
					return true;
				}
			}
				break;
			case PixelCollision_Type::Color_Confirm:
			{
				if (Dest.Pixel[Index] != Dest.Color[0] ||
					Dest.Pixel[Index + 1] != Dest.Color[1] ||
					Dest.Pixel[Index + 2] != Dest.Color[2])
					continue;

				// ������ ������ �ƴ϶�� �� �ȼ��� ���� Box2D �ȿ� ������ �Ǵ��Ѵ�.
				// �ȼ��� ����󿡼��� ��ġ�� ������.
				Vector2	PixelPos = LeftBottom + Vector2((float)x, (float)Dest.Height - y);

				if (CollisionBox2DToPoint(SrcResult, DestResult, Src, PixelPos))
				{
					SrcResult.HitPoint = Vector3(PixelPos.x, PixelPos.y, 0.f);
					DestResult.HitPoint = Vector3(PixelPos.x, PixelPos.y, 0.f);
					return true;
				}
			}
				break;
			case PixelCollision_Type::Alpha_Ignore:
			{
				if (Dest.Pixel[Index + 3] == Dest.Color[3])
					continue;

				// ������ ������ �ƴ϶�� �� �ȼ��� ���� Box2D �ȿ� ������ �Ǵ��Ѵ�.
				// �ȼ��� ����󿡼��� ��ġ�� ������.
				Vector2	PixelPos = LeftBottom + Vector2((float)x, (float)Dest.Height - y);

				if (CollisionBox2DToPoint(SrcResult, DestResult, Src, PixelPos))
				{
					SrcResult.HitPoint = Vector3(PixelPos.x, PixelPos.y, 0.f);
					DestResult.HitPoint = Vector3(PixelPos.x, PixelPos.y, 0.f);
					return true;
				}
			}
				break;
			case PixelCollision_Type::Alpha_Confirm:
			{
				if (Dest.Pixel[Index + 3] != Dest.Color[3])
					continue;

				// ������ ������ �ƴ϶�� �� �ȼ��� ���� Box2D �ȿ� ������ �Ǵ��Ѵ�.
				// �ȼ��� ����󿡼��� ��ġ�� ������.
				Vector2	PixelPos = LeftBottom + Vector2((float)x, (float)Dest.Height - y);

				if (CollisionBox2DToPoint(SrcResult, DestResult, Src, PixelPos))
				{
					SrcResult.HitPoint = Vector3(PixelPos.x, PixelPos.y, 0.f);
					DestResult.HitPoint = Vector3(PixelPos.x, PixelPos.y, 0.f);
					return true;
				}
			}
				break;
			}
		}
	}

	return false;
}

bool CCollision::CollisionCircleToPixel(HitResult& SrcResult, HitResult& DestResult, 
	const CircleInfo& Src, const PixelInfo& Dest)
{
	if (!CollisionBox2DToCircle(DestResult, SrcResult, Dest.Box, Src))
		return false;

	// ������ �簢���� ���Ѵ�.
	float	Left = Src.Min.x < Dest.Min.x ? Dest.Min.x : Src.Min.x;
	float	Right = Src.Max.x > Dest.Max.x ? Dest.Max.x : Src.Max.x;
	float	Top = Src.Max.y > Dest.Max.y ? Dest.Max.y : Src.Max.y;
	float	Bottom = Src.Min.y < Dest.Min.y ? Dest.Min.y : Src.Min.y;

	// �ȼ� �浹ü�� �� �ϴ� ��ǥ�� ���Ѵ�.
	Vector2	LeftBottom = Dest.Box.Center - Vector2(Dest.Box.Length[0], Dest.Box.Length[1]);

	Left -= LeftBottom.x;
	Right -= LeftBottom.x;

	Top -= LeftBottom.y;
	Bottom -= LeftBottom.y;

	Left = Left < 0.f ? 0.f : Left;
	Right = Right >= (float)Dest.Width ? (float)Dest.Width - 1 : Right;

	Bottom = Bottom < 0.f ? 0.f : Bottom;
	Top = Top >= (float)Dest.Height ? (float)Dest.Height - 1 : Top;

	Top = Dest.Height - Top;
	Bottom = Dest.Height - Bottom;

	for (int y = (int)Top; y < (int)Bottom; ++y)
	{
		for (int x = (int)Left; x <= (int)Right; ++x)
		{
			int	Index = y * (int)Dest.Width * 4 + x * 4;

			switch (Dest.Type)
			{
			case PixelCollision_Type::Color_Ignore:
			{
				if (Dest.Pixel[Index] == Dest.Color[0] &&
					Dest.Pixel[Index + 1] == Dest.Color[1] &&
					Dest.Pixel[Index + 2] == Dest.Color[2])
					continue;

				// ������ ������ �ƴ϶�� �� �ȼ��� ���� Box2D �ȿ� ������ �Ǵ��Ѵ�.
				// �ȼ��� ����󿡼��� ��ġ�� ������.
				Vector2	PixelPos = LeftBottom + Vector2((float)x, (float)Dest.Height - y);

				if (CollisionCircleToPoint(SrcResult, DestResult, Src, PixelPos))
				{
					SrcResult.HitPoint = Vector3(PixelPos.x, PixelPos.y, 0.f);
					DestResult.HitPoint = Vector3(PixelPos.x, PixelPos.y, 0.f);
					return true;
				}
			}
			break;
			case PixelCollision_Type::Color_Confirm:
			{
				if (Dest.Pixel[Index] != Dest.Color[0] ||
					Dest.Pixel[Index + 1] != Dest.Color[1] ||
					Dest.Pixel[Index + 2] != Dest.Color[2])
					continue;

				// ������ ������ �ƴ϶�� �� �ȼ��� ���� Box2D �ȿ� ������ �Ǵ��Ѵ�.
				// �ȼ��� ����󿡼��� ��ġ�� ������.
				Vector2	PixelPos = LeftBottom + Vector2((float)x, (float)Dest.Height - y);

				if (CollisionCircleToPoint(SrcResult, DestResult, Src, PixelPos))
				{
					SrcResult.HitPoint = Vector3(PixelPos.x, PixelPos.y, 0.f);
					DestResult.HitPoint = Vector3(PixelPos.x, PixelPos.y, 0.f);
					return true;
				}
			}
			break;
			case PixelCollision_Type::Alpha_Ignore:
			{
				if (Dest.Pixel[Index + 3] == Dest.Color[3])
					continue;

				// ������ ������ �ƴ϶�� �� �ȼ��� ���� Box2D �ȿ� ������ �Ǵ��Ѵ�.
				// �ȼ��� ����󿡼��� ��ġ�� ������.
				Vector2	PixelPos = LeftBottom + Vector2((float)x, (float)Dest.Height - y);

				if (CollisionCircleToPoint(SrcResult, DestResult, Src, PixelPos))
				{
					SrcResult.HitPoint = Vector3(PixelPos.x, PixelPos.y, 0.f);
					DestResult.HitPoint = Vector3(PixelPos.x, PixelPos.y, 0.f);
					return true;
				}
			}
			break;
			case PixelCollision_Type::Alpha_Confirm:
			{
				if (Dest.Pixel[Index + 3] != Dest.Color[3])
					continue;

				// ������ ������ �ƴ϶�� �� �ȼ��� ���� Box2D �ȿ� ������ �Ǵ��Ѵ�.
				// �ȼ��� ����󿡼��� ��ġ�� ������.
				Vector2	PixelPos = LeftBottom + Vector2((float)x, (float)Dest.Height - y);

				if (CollisionCircleToPoint(SrcResult, DestResult, Src, PixelPos))
				{
					SrcResult.HitPoint = Vector3(PixelPos.x, PixelPos.y, 0.f);
					DestResult.HitPoint = Vector3(PixelPos.x, PixelPos.y, 0.f);
					return true;
				}
			}
			break;
			}
		}
	}

	return false;
}

bool CCollision::CollisionBox2DToPoint(HitResult& SrcResult, HitResult& DestResult, 
	const Box2DInfo& Src, const Vector2& Dest)
{
	Vector2 CenterDir = Src.Center - Dest;

	// Src�� X������ ����
	Vector2 Axis = Src.Axis[AXIS_X];

	float	CenterDot = abs(CenterDir.Dot(Axis));

	if (CenterDot > Src.Length[0])
		return false;

	Axis = Src.Axis[AXIS_Y];

	CenterDot = abs(CenterDir.Dot(Axis));

	if (CenterDot > Src.Length[1])
		return false;

	return true;
}

bool CCollision::CollisionCircleToPoint(HitResult& SrcResult, HitResult& DestResult, 
	const CircleInfo& Src, const Vector2& Dest)
{
	float	Dist = Src.Center.Distance(Dest);

	return Dist <= Src.Radius;
}

bool CCollision::CollisionPixelToPoint(HitResult& SrcResult,
	HitResult& DestResult, const PixelInfo& Src, const Vector2& Dest)
{
	if (Src.Min.x <= Dest.x && Dest.x <= Src.Max.x &&
		Src.Min.y <= Dest.y && Dest.y <= Src.Max.y)
	{
		int	x = (int)(Dest.x - Src.Min.x);
		int	y = (int)(Dest.y - Src.Min.y);

		int	Index = y * (int)Src.Width * 4 + x * 4;

		switch (Src.Type)
		{
		case PixelCollision_Type::Color_Ignore:
		{
			if (Src.Pixel[Index] == Src.Color[0] &&
				Src.Pixel[Index + 1] == Src.Color[1] &&
				Src.Pixel[Index + 2] == Src.Color[2])
				return false;

			SrcResult.HitPoint = Vector3(Dest.x, Dest.y, 0.f);
			DestResult.HitPoint = Vector3(Dest.x, Dest.y, 0.f);
		}
		break;
		case PixelCollision_Type::Color_Confirm:
		{
			if (Src.Pixel[Index] != Src.Color[0] ||
				Src.Pixel[Index + 1] != Src.Color[1] ||
				Src.Pixel[Index + 2] != Src.Color[2])
				return false;

			SrcResult.HitPoint = Vector3(Dest.x, Dest.y, 0.f);
			DestResult.HitPoint = Vector3(Dest.x, Dest.y, 0.f);
		}
		break;
		case PixelCollision_Type::Alpha_Ignore:
		{
			if (Src.Pixel[Index + 3] == Src.Color[3])
				return false;

			SrcResult.HitPoint = Vector3(Dest.x, Dest.y, 0.f);
			DestResult.HitPoint = Vector3(Dest.x, Dest.y, 0.f);
		}
		break;
		case PixelCollision_Type::Alpha_Confirm:
		{
			if (Src.Pixel[Index + 3] != Src.Color[3])
				return false;

			SrcResult.HitPoint = Vector3(Dest.x, Dest.y, 0.f);
			DestResult.HitPoint = Vector3(Dest.x, Dest.y, 0.f);
		}
		break;
		}

		return true;
	}

	return false;
}

bool CCollision::CollisionLineToBox2D(HitResult& SrcResult, HitResult& DestResult, const LineInfo& Src, const Box2DInfo& Dest)
{
	Vector2 BoxStart = Dest.Point[0];
	Vector2 BoxEnd = Dest.Point[2];
	LineInfo DestInfo = {};
	DestInfo.Start = BoxStart;
	DestInfo.End = BoxEnd;

	if (CollisionLineToLine(SrcResult, DestResult, Src, DestInfo))
	{
		return true;
	}

	BoxStart = Dest.Point[0];
	BoxEnd = Dest.Point[1];
	DestInfo.Start = BoxStart;
	DestInfo.End = BoxEnd;

	if (CollisionLineToLine(SrcResult, DestResult, Src, DestInfo))
	{

		return true;
	}

	BoxStart = Dest.Point[1];
	BoxEnd = Dest.Point[3];
	DestInfo.Start = BoxStart;
	DestInfo.End = BoxEnd;

	if (CollisionLineToLine(SrcResult, DestResult, Src, DestInfo))
	{

		return true;
	}

	BoxStart = Dest.Point[2];
	BoxEnd = Dest.Point[3];
	DestInfo.Start = BoxStart;
	DestInfo.End = BoxEnd;

	if (CollisionLineToLine(SrcResult, DestResult, Src, DestInfo))
	{

		return true;
	}

	return false;
}
bool CCollision::CollisionLineToLine(HitResult& SrcResult, HitResult& DestResult, const LineInfo& Src, const LineInfo& Dest)
{
	float   Under = (Dest.End.y - Dest.Start.y) * (Src.End.x - Src.Start.x) - (Dest.End.x - Dest.Start.x) * (Src.End.y - Src.Start.y);

	if (Under == 0.f) // �� ���� �����ϴ�.
		return false;

	float   t, t1;

	t = ((Dest.End.x - Dest.Start.x) * (Src.Start.y - Dest.Start.y) - (Dest.End.y - Dest.Start.y) * (Src.Start.x - Dest.Start.x));
	t1 = (Src.End.x - Src.Start.x) * (Src.Start.y - Dest.Start.y) - (Src.End.y - Src.Start.y) * (Src.Start.x - Dest.Start.x);

	if (t == 0.f && t1 == 0.f)
		return false;

	float   Result1, Result2;
	Result1 = t / Under;
	Result2 = t1 / Under;

	if (Result1 < 0.f || Result1 > 1.f || Result2 < 0.f || Result2 > 1.f)
		return false;

	SrcResult.HitPoint.x = Dest.Start.x + (Result1 * (Dest.End.x - Dest.Start.x));
	SrcResult.HitPoint.y = Dest.Start.y + (Result2 * (Dest.End.y - Dest.Start.y));

	return true;
}

bool CCollision::CollisionLineToCircle(HitResult& SrcResult, HitResult& DestResult, const LineInfo& Src, const CircleInfo& Dest)
{
	if (Src.Start.x <= Dest.Center.x && Src.End.x >= Dest.Center.x && Dest.Center.y > Src.Start.y && Dest.Center.y - Src.Start.y <= Dest.Radius)
		return true;

	else if ((Dest.Center.Distance(Src.Start) <= Dest.Radius || Dest.Center.Distance(Src.End) <= Dest.Radius) 
		&& Dest.Center.y >= Src.Start.y && Dest.Center.y - Src.Start.y <= Dest.Radius)
		return true;

	return false;
}
