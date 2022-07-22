#include "Transform.h"
#include "Math.h"

/* TODO : Analytic mult. */
Transform operator*(Transform const& a, Transform const& b) {
  return Transform_Matrix(a.GetMatrix() * b.GetMatrix());
}

Transform Mix(Transform const& a, Transform const& b, double t) {
  Transform self;
  self.pos = Mix(a.pos, b.pos, t);
  self.look = Mix(a.look, b.look, t);
  self.up = Mix(a.up, b.up, t);
  self.scale = Mix(a.scale, b.scale, t);
  self.Orthogonalize();
  return self;
}

FreeFunction(V3F, Transform_GetDir,
  "Return the direction 'dir' under 'transform'",
  Transform, transform,
  V3F, dir)
{
  return transform.TransformDir(dir);
} FunctionAlias(Transform_GetDir, GetDir);

FreeFunction(V3F, Transform_GetVector,
  "Return the vector 'vector' under 'transform'",
  Transform, transform,
  V3F, vector)
{
  return transform.TransformVector(vector);
} FunctionAlias(Transform_GetVector, GetVector);

FreeFunction(V3F, Transform_GetPoint,
  "Return the point 'point' under 'transform'",
  Transform, transform,
  V3D, point)
{
  return transform.TransformPoint(point);
} FunctionAlias(Transform_GetPoint, GetPoint);

FreeFunction(V3F, Transform_GetInverseDir,
  "Return the direction that, under 'transform', yields 'dir",
  Transform, transform,
  V3F, dir)
{
  return transform.InverseDir(dir);
} FunctionAlias(Transform_GetInverseDir, GetInverseDir);

DefineFunction(Transform_Identity) {
  return Transform(0, V3F(1, 0, 0), V3F(0, 1, 0), V3F(0, 0, 1), V3F(1));
}

/* TODO : Analytic inverse. */
DefineFunction(Transform_Inverse) {
  return Transform_Matrix(args.source.GetMatrix().Inverse());
}

DefineFunction(Transform_Look) {
  Transform self(args.pos, V3F(1, 0, 0), OrthoVector(args.look), args.look, V3F(1));
  self.Orthogonalize();
  return self;
}

DefineFunction(Transform_LookUp) {
  Transform self(args.pos, V3F(1, 0, 0), args.up, args.look, V3F(1));
  self.Orthogonalize();
  return self;
}

DefineFunction(Transform_Matrix) {
  V3D pos(args.matrix[12], args.matrix[13], args.matrix[14]);
  V3F right((float)args.matrix[0], (float)args.matrix[1], (float)args.matrix[2]);
  V3F up((float)args.matrix[4], (float)args.matrix[5], (float)args.matrix[6]);
  V3F look((float)args.matrix[8], (float)args.matrix[9], (float)args.matrix[10]);
  V3F scale(Length(right), Length(up), Length(look));
  return Transform(pos, right / scale.x, up / scale.y, look / scale.z, scale);
}

DefineFunction(Transform_Rotate) {
  RotateBasis(
    Mutable(args.source.right),
    Mutable(args.source.up),
    Mutable(args.source.look),
    args.rotation);
} FunctionAlias(Transform_Rotate, Rotate);

DefineFunction(Transform_Scale) {
  return Transform(0, V3F(1, 0, 0), V3F(0, 1, 0), V3F(0, 0, 1), args.scale);
}

DefineFunction(Transform_Translation) {
  return Transform(args.pos, V3F(1, 0, 0), V3F(0, 1, 0), V3F(0, 0, 1), V3F(1));
}

DefineFunction(Transform_ST) {
  return Transform_Translation(args.pos) * Transform_Scale(args.scale);
}
