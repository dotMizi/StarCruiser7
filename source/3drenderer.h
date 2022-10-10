extern void setCamera (Vector3 Camera, Vector3 Target);
extern void setFrameBuffer(u8* fbAdr_l, u8* fbAdr_r);
extern void Clear (u32 color);
extern void Render(Mesh mesh, bool view, enum object_type type);
extern Matrix3 RotationYawPitchRoll(float X, float Y, float Z);
extern Vector3 Project(Vertex vertex, Matrix3 transMat);
extern Vector3 TransformCoordinate(Matrix3 m, Vector3 v);