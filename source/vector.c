#include <math.h>
#include "vector.h"

Vector3 MVMultiply(Matrix3 m, Vector3 v)
{
    Vector3 d;
    d.X = m.a1 * v.X + m.a2 * v.Y + m.a3 * v.Z + m.a4 * v.W;
    d.Y = m.b1 * v.X + m.b2 * v.Y + m.b3 * v.Z + m.b4 * v.W;
    d.Z = m.c1 * v.X + m.c2 * v.Y + m.c3 * v.Z + m.c4 * v.W;
    d.W = m.d1 * v.X + m.d2 * v.Y + m.d3 * v.Z + m.d4 * v.W;
    return d;
}

Vector3 TransformCoordinate(Matrix3 m, Vector3 v)
{
    Vector3 w;
    float invW = 1.0f / (v.X * m.a4 + v.Y * m.b4 + v.Z * m.c4 + m.d4);

    w.X = (v.X * m.a1 + v.Y * m.b1 + v.Z * m.c1 + m.d1) * invW;
    w.Y = (v.X * m.a2 + v.Y * m.b2 + v.Z * m.c2 + m.d2) * invW;
    w.Z = (v.X * m.a3 + v.Y * m.b3 + v.Z * m.c3 + m.d3) * invW;
    w.W = invW;

    return w;
}

Matrix3 MMMultiply(Matrix3 m1, Matrix3 m2)
{
    Matrix3 m;
    
    m.a1 = m1.a1*m2.a1 + m1.a2*m2.b1 + m1.a3*m2.c1 + m1.a4*m2.d1;
    m.a2 = m1.a1*m2.a2 + m1.a2*m2.b2 + m1.a3*m2.c2 + m1.a4*m2.d2;
    m.a3 = m1.a1*m2.a3 + m1.a2*m2.b3 + m1.a3*m2.c3 + m1.a4*m2.d3;
    m.a4 = m1.a1*m2.a4 + m1.a2*m2.b4 + m1.a3*m2.c4 + m1.a4*m2.d4;
    m.b1 = m1.b1*m2.a1 + m1.b2*m2.b1 + m1.b3*m2.c1 + m1.b4*m2.d1;
    m.b2 = m1.b1*m2.a2 + m1.b2*m2.b2 + m1.b3*m2.c2 + m1.b4*m2.d2;
    m.b3 = m1.b1*m2.a3 + m1.b2*m2.b3 + m1.b3*m2.c3 + m1.b4*m2.d3;
    m.b4 = m1.b1*m2.a4 + m1.b2*m2.b4 + m1.b3*m2.c4 + m1.b4*m2.d4;
    m.c1 = m1.c1*m2.a1 + m1.c2*m2.b1 + m1.c3*m2.c1 + m1.c4*m2.d1;
    m.c2 = m1.c1*m2.a2 + m1.c2*m2.b2 + m1.c3*m2.c2 + m1.c4*m2.d2;
    m.c3 = m1.c1*m2.a3 + m1.c2*m2.b3 + m1.c3*m2.c3 + m1.c4*m2.d3;
    m.c4 = m1.c1*m2.a4 + m1.c2*m2.b4 + m1.c3*m2.c4 + m1.c4*m2.d4;
    m.d1 = m1.d1*m2.a1 + m1.d2*m2.b1 + m1.d3*m2.c1 + m1.d4*m2.d1;
    m.d2 = m1.d1*m2.a2 + m1.d2*m2.b2 + m1.d3*m2.c2 + m1.d4*m2.d2;
    m.d3 = m1.d1*m2.a3 + m1.d2*m2.b3 + m1.d3*m2.c3 + m1.d4*m2.d3;
    m.d4 = m1.d1*m2.a4 + m1.d2*m2.b4 + m1.d3*m2.c4 + m1.d4*m2.d4;
    
    return m;
}

Vector3 VectorCross(Vector3 v1, Vector3 v2)
{
	Vector3 v;
	v.X = v1.Y * v2.Z - v1.Z * v2.Y;
	v.Y = v1.Z * v2.X - v1.X * v2.Z;
	v.Z = v1.X * v2.Y - v1.Y * v2.X;
	return v;
}

float VectorDot(Vector3 v1, Vector3 v2)
{
	float d = v1.X*v2.X + v1.Y*v2.Y + v1.Z*v2.Z;
	return d;
}

float Vec2Dot(Vector2 v1, Vector2 v2)
{
	float d = v1.X*v2.X + v1.Y*v2.Y;
	return d;
}

float VectorLength(Vector3 v)
{
	return (float)sqrt(v.X*v.X + v.Y*v.Y + v.Z*v.Z);
}

float Vec2Length(Vector2 v)
{
	return (float)sqrt(v.X*v.X + v.Y*v.Y);
}

Vector3 VectorNormal(Vector3 v)
{
    float lengthSq = v.X * v.X + v.Y * v.Y + v.Z * v.Z;
    if (lengthSq == 0.0f) return v; // Vermeidung von Division durch Null
    float invLength = 1.0f / sqrtf(lengthSq);

    return (Vector3){v.X * invLength, v.Y * invLength, v.Z * invLength, v.W};
}

Vector3 VectorSub (Vector3 v1, Vector3 v2)
{
	Vector3 r;
	
	r.X = v1.X - v2.X;
	r.Y = v1.Y - v2.Y;
	r.Z = v1.Z - v2.Z;
	r.W = v1.W;
	
	return r;
}

Vector3 VectorAdd (Vector3 v1, Vector3 v2)
{
	Vector3 r;
	
	r.X = v1.X + v2.X;
	r.Y = v1.Y + v2.Y;
	r.Z = v1.Z + v2.Z;
	r.W = v1.W;
	
	return r;
}

Vector3 VectorScalar(Vector3 v, float s)
{
	Vector3 d;
	d.X = v.X *s;
	d.Y = v.Y *s;
	d.Z = v.Z *s;
	
	return d;
}

Matrix3 PerspectiveFovRH(float fovY, float aspect, float znear, float zfar)
{
	// This function uses the following formula to compute the returned matrix:
	// The view space height is represented by h. It is calculated from h = cot(fieldOfViewY/2). 
	// The view space width is represented by w. It is calculated from h = w / aspectRatio.
	// w       0       0                                              0
	// 0       h       0                                              0
	// 0       0       zfarPlane/(znearPlane-zfarPlane)              -1
	// 0       0       znearPlane*zfarPlane/(znearPlane-zfarPlane)    0
	// taken from https://docs.microsoft.com/en-us/previous-versions/ms128711(v=vs.100)
	// Matrix.PerspectiveFovRH Method
        float f = 1.0f / tanf(fovY * 0.5f);
        float nf = 1.0f / (znear - zfar);

        Matrix3 m = {
            f / aspect, 0, 0, 0,
            0, f, 0, 0,
            0, 0, zfar * nf, -1,
            0, 0, znear * zfar * nf, 0
        };

        return m;
    }

Matrix3 LookAtLH(Vector3 Position, Vector3 Target)
{
	// This function uses the following formula to compute the returned matrix:
	// zaxis = normal(cameraTarget - cameraPosition)
	// xaxis = normal(cross(cameraUpVector, zaxis))
	// yaxis = cross(zaxis, xaxis)
	//
	// xaxis.x           yaxis.x           zaxis.x          0
	// xaxis.y           yaxis.y           zaxis.y          0
	// xaxis.z           yaxis.z           zaxis.z          0
	// -dot(xaxis, cameraPosition)  -dot(yaxis, cameraPosition)  -dot(zaxis, cameraPosition)  1
	// taken from https://docs.microsoft.com/en-us/previous-versions/windows/desktop/bb281710(v=vs.85)
	// Matrix.LookAtLH(Vector3,Vector3,Vector3) Method (Microsoft.DirectX)
	// cameraUpVector is always {0,1,0,0}
	
	Matrix3 m;
	Vector3 zaxis = VectorNormal(VectorSub(Target, Position));
	Vector3 xaxis = VectorNormal(VectorCross((Vector3){0,1,0,0}, zaxis));
	Vector3 yaxis = VectorCross(zaxis, xaxis);
	
	m.a1 = xaxis.X;
	m.a2 = yaxis.X;
	m.a3 = zaxis.X;
	m.a4 = 0;
	m.b1 = xaxis.Y;
	m.b2 = yaxis.Y;
	m.b3 = zaxis.Y;
	m.b4 = 0;
	m.c1 = xaxis.Z;
	m.c2 = yaxis.Z;
	m.c3 = zaxis.Z;
	m.c4 = 0;
	m.d1 = (-1)*VectorDot(xaxis, Position);
	m.d2 = (-1)*VectorDot(yaxis, Position);
	m.d3 = (-1)*VectorDot(zaxis, Position);
	m.d4 = 1;
	
	return m;
}

Matrix3 RotationYawPitchRoll(float yaw, float pitch, float roll)
{
    float cosYaw = cosf(yaw), sinYaw = sinf(yaw);
    float cosPitch = cosf(pitch), sinPitch = sinf(pitch);
    float cosRoll = cosf(roll), sinRoll = sinf(roll);

    Matrix3 rx = {
        1, 0, 0, 0,
        0, cosYaw, -sinYaw, 0,
        0, sinYaw, cosYaw, 0,
        0, 0, 0, 1
    };
    Matrix3 ry = {
        cosPitch, 0, sinPitch, 0,
        0, 1, 0, 0,
        -sinPitch, 0, cosPitch, 0,
        0, 0, 0, 1
    };
    Matrix3 rz = {
        cosRoll, -sinRoll, 0, 0,
        sinRoll, cosRoll, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };

    return MMMultiply(MMMultiply(rz, ry), rx);
}

Matrix3 Translation (Vector3 v)
{
	Matrix3 m;
	
	m.a1 = 1;
	m.a2 = 0;
	m.a3 = 0;
	m.a4 = 0; v.X;
	m.b1 = 0;
	m.b2 = 1;
	m.b3 = 0;
	m.b4 = 0; v.Y;
	m.c1 = 0;
	m.c2 = 0;
	m.c3 = 1;
	m.c4 = 0; v.Z;
	m.d1 = v.X;
	m.d2 = v.Y;
	m.d3 = v.Z;
	m.d4 = 1;

	return m;
}
