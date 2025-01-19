/*
 *	Project:	oddballs
 *	File:		maths.cpp
 *	Description:handles all mathematical functions
 */

#include "oddballs.h"
#include "maths.h"

VECTOR	zero = {0,0,0};

int randomPointer = 0;
short randomTable[] = 
{
		346, 130, 10982, 1090, 11656, 7117, 17595, 6415, 22948, 31126, 9004, 14558, 3571, 22879, 18492, 1360, 
		5412, 26721, 22463, 25047, 27119, 31441, 7190, 13985, 31214, 27509, 30252, 26571, 14779, 19816, 21681, 19651, 
		17995, 23593, 3734, 13310, 3979, 21995, 15561, 16092, 18489, 11288, 28466, 8664, 5892, 13863, 22766, 5364, 
		17639, 21151, 20427, 100, 25795, 8812, 15108, 12666, 12347, 19042, 19774, 9169, 5589, 26383, 9666, 10941, 
		13390, 7878, 13565, 1779, 16190, 32233, 53, 13429, 2285, 2422, 8333, 31937, 11636, 13268, 6460, 6458, 
		6936, 8160, 24842, 29142, 29667, 24115, 15116, 17418, 1156, 4279, 15008, 15859, 19561, 8297, 3755, 22981, 
		21275, 29040, 28690, 1401, 18137, 16735, 20343, 25267, 8312, 7111, 7733, 10993, 18554, 4353, 20126, 1018, 
		31086, 6970, 26484, 20614, 23431, 23999, 18086, 18730, 5504, 10891, 28492, 27015, 20143, 31246, 32484, 32180, 
		24168, 16704, 9679, 23528, 9365, 20966, 16135, 5740, 18323, 12580, 25378, 12736, 21327, 4164, 4748, 11020, 
		30113, 21445, 21249, 23243, 21480, 21672, 23625, 32691, 13799, 18422, 12344, 32231, 480, 30870, 14821, 7776, 
		17903, 16205, 20522, 23192, 19113, 25878, 14172, 121, 27381, 23461, 32332, 14982, 11562, 30774, 21118, 18505, 
		22889, 7323, 10152, 29436, 2365, 31365, 20079, 2683, 27762, 23826, 22109, 30313, 16179, 9367, 22310, 10146, 
		11623, 24752, 32028, 30177, 8013, 13446, 29935, 13747, 11094, 29025, 4778, 4763, 10563, 4974, 20459, 31111, 
		8831, 3281, 32099, 8051, 7103, 2798, 17294, 28764, 29656, 8693, 20147, 29287, 21472, 2732, 16926, 26962, 
		27785, 24193, 20125, 17948, 31930, 32207, 2104, 12016, 23918, 3184, 22326, 11096, 3794, 12421, 8269, 16543, 
		666, 32642, 40, 2695, 8100, 17788, 13855, 7212, 15057, 4791, 6706, 13624, 288, 15211, 14918, 5961
};







int Rand()
{
	if(++randomPointer > 255)
		randomPointer = 0;
	return randomTable[randomPointer];
}

void SeedRandom(int seed)
{
	randomPointer = seed % 256;
}

/*	--------------------------------------------------------------------------------
	Function 	: AddOneScaledVector()
	Purpose 	: adds 2 vectors together, multilpying each by a scaling factor into a resultant vector
	Parameters 	: pointers to resultant and two operand vectors, two scale factors
	Returns 	: none
	Info 		:
*/
void AddOneScaledVector(VECTOR *result,VECTOR *operand1,VECTOR *operand2,double scale2)
{
	result->v[X] = operand1->v[X] + operand2->v[X]*scale2;
	result->v[Y] = operand1->v[Y] + operand2->v[Y]*scale2;
	result->v[Z] = operand1->v[Z] + operand2->v[Z]*scale2;
}

/*	--------------------------------------------------------------------------------
	Function 	: AddOneScaledVector()
	Purpose 	: adds 2 vectors together, multilpying each by a scaling factor into a resultant vector
	Parameters 	: pointers to resultant and two operand vectors, two scale factors
	Returns 	: none
	Info 		:
*/
void AddOneScaledVector2D(VECTOR *result,VECTOR *operand1,VECTOR *operand2,double scale2)
{
	result->v[X] = operand1->v[X] + operand2->v[X]*scale2;
	result->v[Z] = operand1->v[Z] + operand2->v[Z]*scale2;
}

/*	--------------------------------------------------------------------------------
	Function 	: DistanceBetweenPoints2D()
	Purpose 	: finds the distance between two points
	Parameters 	: vector 1, vector 2
	Returns 	: distance
	Info 		:
*/
double DistanceBetweenPoints2D(VECTOR *v1, VECTOR *v2)
{
	VECTOR tempVect;

	SubVector2D(&tempVect,v1,v2);
	return Magnitude2D(&tempVect);
}

/*	--------------------------------------------------------------------------------
	Function 	: DistanceBetweenPoints()
	Purpose 	: finds the distance between two points
	Parameters 	: vector 1, vector 2
	Returns 	: distance
	Info 		:
*/
double DistanceBetweenPoints(VECTOR *v1, VECTOR *v2)
{
	VECTOR tempVect;

	SubVector(&tempVect,v1,v2);
	return Magnitude(&tempVect);
}

/*	--------------------------------------------------------------------------------
	Function 	: IsPointWithinBox
	Purpose 	: determine whether a point is within a box
	Parameters 	: box* vector*
	Returns 	: BOOL
	Info 		:
*/
BOOL IsPointWithinBox(BOX *box, VECTOR *pos)
{

	if((pos->v[X] >= box->pos.v[X]) && (pos->v[X] <= box->pos.v[X] + box->size.v[X]) &&
	   (pos->v[Y] >= box->pos.v[Y]) && (pos->v[Y] <= box->pos.v[Y] + box->size.v[Y]) &&
	   (pos->v[Z] >= box->pos.v[Z]) && (pos->v[Z] <= box->pos.v[Z] + box->size.v[Z]))
	   return TRUE;

	return FALSE;
}
/*	--------------------------------------------------------------------------------
	Function 	: RotateVector2D
	Purpose 	: rotates the X and Z points around the Y axis, by given angle
	Parameters 	: result vector, source vector, angle
	Returns 	: 
	Info 		:
*/
void RotateVector2D(VECTOR *dest, VECTOR *source, double angle)
{
	VECTOR temp;
	angle = -angle;
	temp.v[Y] = source->v[Y];
	temp.v[X] = (cos(angle) * source->v[X]) + (-sin(angle) * source->v[Z]);
	temp.v[Z] = (cos(angle) * source->v[Z]) + (sin(angle) * source->v[X]);

	SetVector(dest, &temp);
//	nx=(cos(ang)*objects[ob].face[f].vertex[v].xoff)+(-sin(ang)*objects[ob].face[f].vertex[v].zoff);
//	nz=(cos(ang)*objects[ob].face[f].vertex[v].zoff)+(sin(ang)*objects[ob].face[f].vertex[v].xoff);
}
/*	--------------------------------------------------------------------------------
	Function 	: RotateVector2D
	Purpose 	: rotates the X and Z points around the Y axis, by given angle
	Parameters 	: result vector, source vector, angle
	Returns 	: 
	Info 		:
*/
void RotateVector2DXYZ(VECTOR *dest, VECTOR *source, double angle, BYTE axis)
{
	VECTOR	temp;
	BYTE	x,z;

	switch(axis)
	{
		case X:
			x = Y;
			z = Z;
			break;
		case Y:
			x = X;
			z = Z;
			break;
		case Z:
			x = X;
			z = Y;
			break;
	}

	angle = -angle;
	temp.v[axis] = source->v[axis];
	temp.v[x] = (cos(angle) * source->v[x]) + (-sin(angle) * source->v[z]);
	temp.v[z] = (cos(angle) * source->v[z]) + (sin(angle) * source->v[x]);

	SetVector(dest, &temp);
}
/*	--------------------------------------------------------------------------------
	Function 	: FlipVector()
	Purpose 	: flips a vector
	Parameters 	: pointer to vector
	Returns 	: none
	Info 		:
*/
void FlipVector(VECTOR *vector)
{
	vector->v[X] = -vector->v[X];
	vector->v[Y] = -vector->v[Y];
	vector->v[Z] = -vector->v[Z];
}

/*	--------------------------------------------------------------------------------
	Function 	: SetVector()
	Purpose 	: sets one vector equal to another
	Parameters 	: pointer to result vector, pointer to original vector
	Returns 	: none
	Info 		:
*/
void SetVector(VECTOR *v1,VECTOR *v2)
{
	memcpy(v1,v2,sizeof(VECTOR));
}

/*	--------------------------------------------------------------------------------
	Function 	: SetVector2D()
	Purpose 	: sets one vector equal to another - only in X and Z axis
	Parameters 	: pointer to result vector, pointer to original vector
	Returns 	: none
	Info 		:
*/
void SetVector2D(VECTOR *v1,VECTOR *v2)
{
	v1->v[X] = v2->v[X];
	v1->v[Z] = v2->v[Z];
}

/*	--------------------------------------------------------------------------------
	Function 	: AddVector()
	Purpose 	: adds 2 vectors together into a resultant vector
	Parameters 	: pointers to resultant and two operand vectors
	Returns 	: none
	Info 		:
*/
void AddVector(VECTOR *result,VECTOR *operand1,VECTOR *operand2)
{
	result->v[X] = operand1->v[X] + operand2->v[X];
	result->v[Y] = operand1->v[Y] + operand2->v[Y];
	result->v[Z] = operand1->v[Z] + operand2->v[Z];
}

/*	--------------------------------------------------------------------------------
	Function 	: AddVector2D()
	Purpose 	: adds 2 vectors together into a resultant vector
	Parameters 	: pointers to resultant and two operand vectors
	Returns 	: none
	Info 		:
*/
void AddVector2D(VECTOR *result,VECTOR *operand1,VECTOR *operand2)
{
	result->v[X] = operand1->v[X] + operand2->v[X];
	result->v[Z] = operand1->v[Z] + operand2->v[Z];
}

/*	--------------------------------------------------------------------------------
	Function 	: AddScaledVector()
	Purpose 	: adds 2 vectors together, multilpying each by a scaling factor into a resultant vector
	Parameters 	: pointers to resultant and two operand vectors, two scale factors
	Returns 	: none
	Info 		:
*/
void AddScaledVector(VECTOR *result,VECTOR *operand1,double scale1,VECTOR *operand2,double scale2)
{
	result->v[X] = operand1->v[X]*scale1 + operand2->v[X]*scale2;
	result->v[Y] = operand1->v[Y]*scale1 + operand2->v[Y]*scale2;
	result->v[Z] = operand1->v[Z]*scale1 + operand2->v[Z]*scale2;
}

/*	--------------------------------------------------------------------------------
	Function 	: AddScaledVector2D()
	Purpose 	: adds 2 vectors together, multilpying each by a scaling factor into a resultant vector
	Parameters 	: pointers to resultant and two operand vectors, two scale factors
	Returns 	: none
	Info 		:
*/
void AddScaledVector2D(VECTOR *result,VECTOR *operand1,double scale1,VECTOR *operand2,double scale2)
{
	result->v[X] = operand1->v[X]*scale1 + operand2->v[X]*scale2;
	result->v[Z] = operand1->v[Z]*scale1 + operand2->v[Z]*scale2;
}

/*	--------------------------------------------------------------------------------
	Function 	: SubVector()
	Purpose 	: subtracts one vector from another
	Parameters 	: pointers to resultant and two operand vectors
	Returns 	: none
	Info 		:
*/
void SubVector(VECTOR *result,VECTOR *operand1,VECTOR *operand2)
{
	result->v[X] = operand1->v[X] - operand2->v[X];	
	result->v[Y] = operand1->v[Y] - operand2->v[Y];	
	result->v[Z] = operand1->v[Z] - operand2->v[Z];	
}

/*	--------------------------------------------------------------------------------
	Function 	: SubVector2D()
	Purpose 	: subtracts one vector from another
	Parameters 	: pointers to resultant and two operand vectors
	Returns 	: none
	Info 		:
*/
void SubVector2D(VECTOR *result,VECTOR *operand1,VECTOR *operand2)
{
	result->v[X] = operand1->v[X] - operand2->v[X];	
	result->v[Z] = operand1->v[Z] - operand2->v[Z];	
}

/*	--------------------------------------------------------------------------------
	Function 	: FindShortestAngle()
	Purpose 	: returns the smallest angle between 2 directions (rads)
	Parameters 	: angle1, angle2
	Returns 	: angle
	Info 		:
*/
double FindShortestAngle(double val1, double val2)
{
	double temp;

	if(fabs(val1 - val2) > PI)	
	{
		if(val1 > val2)
		{
			temp = val2 - val1;
		}
		else
		{
			temp = val1 - val2;
		}
		temp += PI * 2;
		return temp;
	}
	else
	{
	   return fabs(val1 - val2);
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
double FindShortestAngleSigned(double ang1,double ang2)
{
	double temp = ang1 - ang2;
	if((temp > -PI) && (temp <= PI))
		return temp;
	else if(temp > PI)
		return ang1 - ang2 - 2*PI;
	else
		return ang1 + 2*PI - ang2;
}

/*	--------------------------------------------------------------------------------
	Function 	: Magnitude()
	Purpose 	: find the magnitude of a vector
	Parameters 	: pointer to vector
	Returns 	: magnitude
	Info 		:
*/
double Magnitude(VECTOR *vect)
{
	return sqrt(vect->v[X]*vect->v[X] + vect->v[Y]*vect->v[Y] + vect->v[Z]*vect->v[Z]);
}

/*	--------------------------------------------------------------------------------
	Function 	: Magnitude()
	Purpose 	: find the magnitude of a vector
	Parameters 	: pointer to vector
	Returns 	: magnitude
	Info 		:
*/
double MagnitudeSquared(VECTOR *vect)
{
	return (vect->v[X]*vect->v[X] + vect->v[Y]*vect->v[Y] + vect->v[Z]*vect->v[Z]);
}

/*	--------------------------------------------------------------------------------
	Function 	: Magnitude2D()
	Purpose 	: find the 2-dimensional (XZ) magnitude of a vector
	Parameters 	: pointer to vector
	Returns 	: magnitude
	Info 		:
*/
double Magnitude2D(VECTOR *vect)
{
	return sqrt(vect->v[X]*vect->v[X] + vect->v[Z]*vect->v[Z]);
}

/*	--------------------------------------------------------------------------------
	Function 	: MakeUnit()
	Purpose 	: makes a vector unit magnitude
	Parameters 	: pointer to vector
	Returns 	: none
	Info 		:
*/
void MakeUnit(VECTOR *vect)
{
	int i;
	double m = Magnitude(vect);

	if(m != 0)
	{
		vect->v[X] /= m;
		vect->v[Y] /= m;
		vect->v[Z] /= m;
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: MakeUnit()
	Purpose 	: makes a vector unit magnitude
	Parameters 	: pointer to vector
	Returns 	: none
	Info 		:
*/
void MakeUnit2D(VECTOR *vect)
{
	double m = Magnitude2D(vect);

	if(m != 0)
	{
		vect->v[X] /= m;
		vect->v[Z] /= m;
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: ScaleVector()
	Purpose 	: multiplies a vector's length by a scaling factor
	Parameters 	: pointer to vector, scale
	Returns 	: none
	Info 		:
*/
void ScaleVector(VECTOR *vect,double scale)
{
	vect->v[X] *= scale;
	vect->v[Y] *= scale;
	vect->v[Z] *= scale;
}
/*	--------------------------------------------------------------------------------
	Function 	: ScaleVector2D()
	Purpose 	: multiplies a vector's length by a scaling factor
	Parameters 	: pointer to vector, scale
	Returns 	: none
	Info 		:
*/
void ScaleVector2D(VECTOR *vect,double scale)
{
	vect->v[X] *= scale;
	vect->v[Z] *= scale;
}



/*	--------------------------------------------------------------------------------
	Function 	: Aabs()
	Purpose 	: calculates a valid angle (makes sure it is between 0 and PI*2)
	Parameters 	: angle
	Returns 	: absolute angle
	Info 		:
*/
double Aabs(double angle)
{

	angle = angle > (2 * PI) ? angle - 2 * PI : angle;
	angle = angle < 0 ? angle += 2 * PI : angle;

	return angle;
}

/*	--------------------------------------------------------------------------------
	Function 	: arse
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
double SineWave(double freq,double phase)
{
	float temp = frameCount + phase;
	return (sin(temp/freq));
}
/*	--------------------------------------------------------------------------------
	Function 	: PlaneCentroidHeight()
	Purpose 	: calculates the Y co-ordinate of a polygon's centroid
	Parameters 	: pointer to plane
	Returns 	: height
	Info 		:
*/
double PlaneCentroidHeight(PLANE *plane)
{
	double	height = 0;
	int		j;

	for(j = 0;j < plane->numVertices;j++)
		height += plane->vertex[j].v[Y];

	return height/plane->numVertices;
}

/*	--------------------------------------------------------------------------------
	Function 	: PlaneIntersectsBox()
	Purpose 	: checks whether any part of a polygon is within a box
	Parameters 	: plane pointer, box pointer
	Returns 	: TRUE or FALSE
	Info 		:
*/
BOOL PlaneIntersectsBox(PLANE *plane,BOX *box)
{
	char	j = 0,k = 0,l = 0,i = 0;
	VECTOR	normal = {0,0,0},line = {0,0,0},point = {0,0,0};
	double	J = 0,t = 0;
	double	u[3] = {0,0,0},v[3] = {0,0,0},alpha = -1,beta = 0;

	//check if all vertices of the polygon lie to one side of any plane of the box		
	for(i = X;i <= Z;i++)
	{
		for(j = 0;j < plane->numVertices;j++)
		{
			if(plane->vertex[j].v[i] >= box->pos.v[i])
				break;
		}
		if(j == plane->numVertices)
			return FALSE;
		for(j = 0;j < plane->numVertices;j++)
		{
			if(plane->vertex[j].v[i] <= box->pos.v[i] + box->size.v[i])
				break;
		}
		if(j == plane->numVertices)
			return FALSE;
	}

	for(j = 0;j < plane->numVertices;j++)
	{
		//check if any vertex of the polygon lies within the box
		if((plane->vertex[j].v[X] >= box->pos.v[X]) && (plane->vertex[j].v[X] <= box->pos.v[X] + box->size.v[X])
		&& (plane->vertex[j].v[Y] >= box->pos.v[Y]) && (plane->vertex[j].v[Y] <= box->pos.v[Y] + box->size.v[Y])
		&& (plane->vertex[j].v[Z] >= box->pos.v[Z]) && (plane->vertex[j].v[Z] <= box->pos.v[Z] + box->size.v[Z]))
			return TRUE;

		l = (j + 1) MOD plane->numVertices;

		SubVector(&line,&plane->vertex[l],&plane->vertex[j]);
		//check if any edge of the polygon intersects any plane of the box
		for(k = 0;k < 6;k++)
		{
			if(k < 2)
			{
				normal.v[X] = 1;
				normal.v[Y] = 0;
				normal.v[Y] = 0;
			}
			else if(k < 4)
			{
				normal.v[X] = 0;
				normal.v[Y] = 1;
				normal.v[Z] = 0;
			}
			else
			{
				normal.v[X] = 0;
				normal.v[Y] = 0;
				normal.v[Z] = 1;
			}

			if(DotProduct(&line,&normal) != 0)
			{
				for(i = X;i <= Z;i++)
					if(k == i*2 + 1)
						point.v[i] = box->pos.v[i] + box->size.v[i];
					else
						point.v[i] = box->pos.v[i];

				J = -DotProduct(&point,&normal);

				t = J + DotProduct(&plane->vertex[j],&normal);
				t /= DotProduct(&line,&normal);
				t = -t;

				if((t >= 0) && (t <= 1))
				{
					AddOneScaledVector(&point,&plane->vertex[j],&line,t);

					switch(k)
					{
						case 0:
						case 1:
							if((point.v[Y] >= box->pos.v[Y]) && (point.v[Y] <= box->pos.v[Y] + box->size.v[Y])
							&& (point.v[Z] >= box->pos.v[Z]) && (point.v[Z] <= box->pos.v[Z] + box->size.v[Z]))
								return TRUE;
							break;

						case 2:
						case 3:
							if((point.v[X] >= box->pos.v[X]) && (point.v[X] <= box->pos.v[X] + box->size.v[X])
							&& (point.v[Z] >= box->pos.v[Z]) && (point.v[Z] <= box->pos.v[Z] + box->size.v[Z]))
								return TRUE;
							break;

						default:
							if((point.v[Y] >= box->pos.v[Y]) && (point.v[Y] <= box->pos.v[Y] + box->size.v[Y])
							&& (point.v[X] >= box->pos.v[X]) && (point.v[X] <= box->pos.v[X] + box->size.v[X]))
								return TRUE;
							break;
					}

/*					if((point.v[X] >= box->pos.v[X]-0.00000001) && (point.v[X] <= box->pos.v[X] + box->size.v[X]+0.00000001)
					&& (point.v[Y] >= box->pos.v[Y]-0.00000001) && (point.v[Y] <= box->pos.v[Y] + box->size.v[Y]+0.00000001)
					&& (point.v[Z] >= box->pos.v[Z]-0.00000001) && (point.v[Z] <= box->pos.v[Z] + box->size.v[Z]+0.00000001))
						return TRUE;
*/				}
			}
		}
	}
	//check if any diagonal of the box intersects the polygon
	for(k = 0;k < 4;k++)
	{
		switch(k)
		{
			case 0:
				SetVector(&point,&box->pos);
				SetVector(&line,&box->size);
				break;

			case 1:
				SetVector(&point,&box->pos);
				point.v[X] += box->size.v[X];
				SetVector(&line,&box->size);
				line.v[X] -= box->size.v[X]*2;
				break;

			case 2:
				point.v[X] = box->pos.v[X] + box->size.v[X];
				point.v[Z] = box->pos.v[Z] + box->size.v[Z];
				point.v[Y] = box->pos.v[Y];
				line.v[X] = -box->size.v[X];
				line.v[Z] = -box->size.v[Z];
				line.v[Y] = box->size.v[Y];
				break;

			case 3:
				point.v[X] = box->pos.v[X];
				point.v[Z] = box->pos.v[Z] + box->size.v[Z];
				point.v[Y] = box->pos.v[Y];
				line.v[X] = box->size.v[X];
				line.v[Z] = -box->size.v[Z];
				line.v[Y] = box->size.v[Y];
				break;
		}

		if(DotProduct(&line,&plane->normal) != 0)
		{
			t = plane->J + DotProduct(&point,&plane->normal);
			t /= DotProduct(&line,&plane->normal);
			t = -t;

			if((t >= 0) && (t <= 1))
			{
				AddOneScaledVector(&point,&box->pos,&line,t);
	
				u[0] = point.v[plane->i[0]] - plane->vertex[0].v[plane->i[0]];
				u[1] = plane->u1;
				u[2] = plane->u2;
		
				v[0] = point.v[plane->i[1]] - plane->vertex[0].v[plane->i[1]];
				v[1] = plane->v1;
				v[2] = plane->v2;

				if(u[1] == 0)
				{
					beta = u[0]/u[2];
					if((beta >= 0.0) && (beta <= 1.0))
						alpha = (v[0] - beta*v[2])/v[1];
				}
				else
				{
					beta = (v[0]*u[1] - u[0]*v[1])/(v[2]*u[1] - u[2]*v[1]);
					if((beta >= 0.0) && (beta <= 1.0))
						alpha = (u[0] - beta*u[2])/u[1];
				}

				if((alpha >= 0.0) && (alpha+beta <= 1.0))
					return TRUE;
			}
		}
	}
	return FALSE;
}


/*	--------------------------------------------------------------------------------
	Function 	: SphereIntersectsBox()
	Purpose 	: tests whether a sphere intersects a box
	Parameters 	: centre and radius of sphere, pointer to box
	Returns 	: TRUE or FALSE
	Info 		:
*/
BOOL SphereIntersectsBox(VECTOR *pos,double radius,BOX *box,int accuracy)
{
	char	i;
	double	d;
	register double v;

	if(accuracy == APPROXIMATE_TEST)
	{
		for(i = X;i <= Z;i++)
		{
			if((pos->v[i] + radius < box->pos.v[i]) || (pos->v[i] - radius > box->pos.v[i] + box->size.v[i]))
				return FALSE;
		}			
		return TRUE;
	}
	else
	{
		d = 0;
		for(i = X;i <= Z;i++)
		{
			if(pos->v[i] < box->pos.v[i])
			{
				v = pos->v[i] - box->pos.v[i];
				d += v*v;
			}
			else if(pos->v[i] > box->pos.v[i] + box->size.v[i])
			{
				v = pos->v[i] - box->pos.v[i] - box->size.v[i];
				d += v*v;
			}
		}
		if(d <= radius*radius)
			return TRUE;
		else
			return FALSE;
	}

}

/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/

// --------------------
// Structures & Classes
/*
typedef struct _BoundaryVertexType {			// Boundary vertex
	float 			x;							// Map x coord
	float 			y;							// Map y coord
	float			normx,normy;				// normal at this point
	float			normangle;					// angle between normal and adjacent boundary segments
} BoundaryVertexType;

typedef struct _BoundaryType {					// Boundary data
	int						numVerts;			// Number of vertices
	BoundaryVertexType		*vList;				// Ptr to array of vertices [numVerts]
	float 					*edgeLength;		// Ptr to array of edge length [numVerts]
	float 					x0,y0,x1,y1;		// Bounding box coords (x0,y0)->(x1,y1)
	struct _BoundaryType	*prev, *next;		// Boundary list ptrs
} BoundaryType;
*/
int IntersectEdges(float *x, float *y, POINT2D *v1, POINT2D *v2, 
					POINT2D *v3, POINT2D *v4)
{
	float hi, lo, ax, bx, cx, ay, by, cy, d, e, f;


	ax = v2->x - v1->x;
	bx = v3->x - v4->x;

	if (ax<(float)0.0) 							// X bound box test
	{
		lo = v2->x;
		hi = v1->x;
	}
	else
	{
		hi = v2->x;
		lo = v1->x;
	}
	if (bx>(float)0.0)
	{
		if ((hi<v4->x)||(v3->x<lo))
			return 0;
	}
	else
	{
		if ((hi<v3->x)||(v4->x<lo))
			return 0;
	}

	ay = v2->z - v1->z;
	by = v3->z - v4->z;

	if (ay<(float)0.0)						// Y bound box test
	{
		lo = v2->z;
		hi = v1->z;
	}
	else
	{
		hi = v2->z;
		lo = v1->z;
	}
	if (by>(float)0.0)
	{
		if ((hi < v4->z) || (v3->z<lo))
			return 0;
	}
	else
	{
		if ((hi<v3->z) || (v4->z<lo))
			return 0;
	}

	cx = v1->x - v3->x;
	cy = v1->z - v3->z;
	d = by * cx - bx * cy;						// Calculate line fraction
	f = ay * bx - ax * by;
	if (f>(float)0.0)							// On line tests
	{
		if ((d < (float)0.0) || (d > f))
			return 0;
	}
	else
	{
		if ((d > (float)0.0) || (d < f))
			return 0;
	}
	e = ax * cy - ay * cx;
	if (f > (float)0.0)
	{
		if ((e < (float)0.0) || (e > f))
			return 0;
	}
	else
	{
		if ((e > (float)0.0) || (e < f))
			return 0;
	}
	if (f == (float)0.0)						// Coolinear test
		return 0;

	*x = v1->x + (d * ax) / f;					// Intersection point
	*y = v1->z + (d * ay) / f;
	return 1;
}


