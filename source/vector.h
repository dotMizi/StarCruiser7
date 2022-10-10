typedef struct Vector2 {
	float X;
	float Y;
	float W;
} Vector2;

typedef struct Vector3 {
	float X;
	float Y;
	float Z;
	float W;
} Vector3;	

typedef struct Matrix3 {
	float a1, a2, a3, a4;
	float b1, b2, b3, b4;
	float c1, c2, c3, c4;
	float d1, d2, d3, d4;
} Matrix3;

extern Vector3 VectorNormal(Vector3 v);
extern Vector3 VectorSub (Vector3 v1, Vector3 v2);
extern Vector3 VectorAdd (Vector3 v1, Vector3 v2);
extern float VectorDot(Vector3 v1, Vector3 v2);
extern float Vec2Dot(Vector2 v1, Vector2 v2);
extern float VectorLength(Vector3 v);
extern float Vec2Length(Vector2 v);
extern Vector3 VectorCross(Vector3 v1, Vector3 v2);
extern Vector3 VectorScalar(Vector3 v, float s);
extern Vector3 MVMultiply(Matrix3 m, Vector3 v);
extern Vector3 TransformCoordinate(Matrix3 m, Vector3 v);
extern Matrix3 MMMultiply(Matrix3 m1, Matrix3 m2);
extern Matrix3 LookAtLH(Vector3 Position, Vector3 Target);
extern Matrix3 PerspectiveFovRH(float fieldOfViewY, float aspectRatio, float znearPlane, float zfarPlane);
extern Matrix3 RotationYawPitchRoll(float yaw, float pitch, float roll);
extern Matrix3 Translation (Vector3 v);

