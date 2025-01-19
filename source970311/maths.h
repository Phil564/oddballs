#ifndef __MATHS_H
#define __MATHS_H

extern VECTOR zero;

#define DotProduct(v1,v2)	((((v1)->v[X])*((v2)->v[X])) + (((v1)->v[Y])*((v2)->v[Y])) + (((v1)->v[Z])*((v2)->v[Z])))
#define DotProduct2D(v1,v2)	((((v1)->v[X])*((v2)->v[X])) + (((v1)->v[Z])*((v2)->v[Z])))
#define ACCURATE_TEST		0
#define APPROXIMATE_TEST	1


int Rand();
void SeedRandom(int seed);


int IntersectEdges(float *x, float *y, POINT2D *v1, POINT2D *v2, POINT2D *v3, POINT2D *v4);
BOOL IsPointWithinBox(BOX *box, VECTOR *pos);
void AddOneScaledVector2D(VECTOR *result,VECTOR *operand1,VECTOR *operand2,double scale2);
double DistanceBetweenPoints2D(VECTOR *v1, VECTOR *v2);
double DistanceBetweenPoints(VECTOR *v1, VECTOR *v2);
double Aabs(double angle);
double SineWave(double freq,double phase);
double FindShortestAngle(double val1, double val2);
double FindShortestAngleSigned(double ang1,double ang2);
double Magnitude(VECTOR *vect);
double Magnitude2D(VECTOR *vect);
double MagnitudeSquared(VECTOR *vect);
void MakeUnit(VECTOR *vect);
void ScaleVector(VECTOR *vect,double scale);
void ScaleVector2D(VECTOR *vect,double scale);
void SetVector(VECTOR *v1,VECTOR *v2);
void SetVector2D(VECTOR *v1,VECTOR *v2);
void AddVector(VECTOR *result,VECTOR *operand1,VECTOR *operand2);
void AddVector2D(VECTOR *result,VECTOR *operand1,VECTOR *operand2);
void AddScaledVector(VECTOR *result,VECTOR *operand1,double scale1,VECTOR *operand2,double scale2);
void AddScaledVector2D(VECTOR *result,VECTOR *operand1,double scale1,VECTOR *operand2,double scale2);
void SubVector(VECTOR *result,VECTOR *operand1,VECTOR *operand2);
void SubVector2D(VECTOR *result,VECTOR *operand1,VECTOR *operand2);
void FlipVector(VECTOR *vector);
double PlaneCentroidHeight(PLANE *plane);
BOOL PlaneIntersectsBox(PLANE *plane,BOX *box);
BOOL SphereIntersectsBox(VECTOR *pos,double radius,BOX *box,int accuracy);
void RotateVector2D(VECTOR *dest, VECTOR *source, double angle);
void RotateVector2DXYZ(VECTOR *dest, VECTOR *source, double angle, BYTE axis);
void MakeUnit2D(VECTOR *vect);
void AddOneScaledVector(VECTOR *result,VECTOR *operand1,VECTOR *operand2,double scale2);


#endif

