#pragma once

#include "stdafx.h"
#include "math.h"
typedef double PNT3D[3];
typedef double VEC3D[3];

void mathVProduct(VEC3D v1, VEC3D v2, VEC3D rtvec)
{
	VEC3D vector ;

	vector[0] = v1[1]*v2[2]-v1[2]*v2[1] ;
	vector[1] = v1[2]*v2[0]-v1[0]*v2[2] ;
	vector[2] = v1[0]*v2[1]-v1[1]*v2[0] ;

	rtvec[0] = vector[0] ;
	rtvec[1] = vector[1] ;
	rtvec[2] = vector[2] ;

	return ;
}

BOOL mathUniVec(VEC3D v, double min_len)
{
	double len ;

	len = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]) ;
	if( len < min_len )
	{
		v[0] = 0.0 ;
		v[1] = 0.0 ;
		v[2] = 0.0 ;
		return FALSE ;
	}
	else
	{
		v[0] /= len ;
		v[1] /= len ;
		v[2] /= len ;
		return TRUE ;
	}
}

void mathRotVecXY(VEC3D e1, VEC3D e2, double angle, VEC3D rot_v)
{
	int k ;
	double cosRotateAngle, sinRotateAngle ;

	cosRotateAngle = cos(angle) ;
	sinRotateAngle = sin(angle) ;
	for( k = 0 ; k < 3 ; k++ )
		rot_v[k] = cosRotateAngle*e1[k]+sinRotateAngle*e2[k] ;
}

void mathRotVecXZ(VEC3D e1, VEC3D e3, double angle, VEC3D rot_v)
{
	int k ;
	VEC3D e2 ;
	double cosRotateAngle, sinRotateAngle ;

	mathVProduct(e3, e1, e2) ;
	cosRotateAngle = cos(angle) ;
	sinRotateAngle = sin(angle) ;
	for( k = 0 ; k < 3 ; k++ )
		rot_v[k] = cosRotateAngle*e1[k]+sinRotateAngle*e2[k] ;
}
