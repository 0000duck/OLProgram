#include "stdafx.h"
#include "MathBase.h"
#include "math.h"

// ȥ��CString�ַ���С����֮���ĩβ0
void TrimZeroForCSring(CString& str)
{
	int pos = str.Find('.');
	if (-1 == pos)
		return;
	int nLength = str.GetLength();
	for (int i=0; i<nLength; i++)
	{
		CString tmp = str.Right(1);
		if (tmp == '0')
		{
			str = str.Left(str.GetLength()-1);
		}
		else if(tmp == '.')
		{
			str = str.Left(str.GetLength()-1);
			return;
		}
	}
}

// ��㵽ֱ�ߵľ���
double mathDistPntLin(PNT3D p, PNT3D begin, VEC3D dir)
{   
	double t ;
	VEC3D v ;

	t = (p[0]-begin[0])*dir[0]+(p[1]-begin[1])*dir[1]+(p[2]-begin[2])*dir[2] ;
	v[0] = begin[0]+dir[0]*t-p[0] ;
	v[1] = begin[1]+dir[1]*t-p[1] ;
	v[2] = begin[2]+dir[2]*t-p[2] ;

	return sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]) ;
}

// ����3D��ľ���
double mathDis3D(PNT3D pt1, PNT3D pt2)
{
	return sqrt((pt1[0]-pt2[0])*(pt1[0]-pt2[0])+(pt1[1]-pt2[1])*(pt1[1]-pt2[1])+(pt1[2]-pt2[2])*(pt1[2]-pt2[2]));
};

// ������������axis�͵�pivot��ת�Ƕ�angle���õ����������rot_v
void mathRotVec(VEC3D axis,   // [IN] ��ת�᷽��
	            PNT3D pivot,  // [IN] ��ת�����
				double angle, // [IN] ��ת�Ƕ�
				VEC3D v,      // [IN] ԭʼ����
				VEC3D rot_v)  // [OUT]��ת������
{
	int k ;
	PNT3D p ;

	for( k = 0 ; k < 3 ; k++ )
		p[k] = pivot[k]+v[k] ;
	mathRotPnt(axis, pivot, angle, p, p) ;
	for( k = 0 ; k < 3 ; k++ )
		rot_v[k] = p[k]-pivot[k] ;
}

// ���p��ֱ�ߣ����Ϊbegin������Ϊdir���ϵ�ͶӰ��prj_p
void mathPrjPntLin(PNT3D p,      // [IN]  ԭʼ��
				   PNT3D begin,  // [IN]  ֱ�����
				   VEC3D dir,    // [IN]  ֱ�߷���
				   PNT3D prj_p)  // [OUT] ͶӰ��
{
	double d ;

	d = (p[0]-begin[0])*dir[0]+(p[1]-begin[1])*dir[1]+(p[2]-begin[2])*dir[2] ;
	prj_p[0] = begin[0]+d*dir[0] ;
	prj_p[1] = begin[1]+d*dir[1] ;
	prj_p[2] = begin[2]+d*dir[2] ;

	return ;
}

// �������ᾭ��p�����ת��axis��ת
void mathRotPnt(VEC3D axis,   // [IN] ��ת�᷽��
				PNT3D pivot,  // [IN] ��ת�����
				double angle, // [IN] ��ת�Ƕ�
				PNT3D p,      // [IN] ԭʼ��
				PNT3D rot_p)  // [IN] ��ת��ĵ�
{
	int k ;
	double cosRotateAngle, sinRotateAngle ;
	VEC3D e1, e2 ;
	PNT3D temp ;

	mathPrjPntLin(p, pivot, axis, temp) ; 
	for( k = 0 ; k < 3 ; k++ )
		e1[k] = p[k]-temp[k] ;
	mathVProduct(axis, e1, e2) ;
	cosRotateAngle = cos(angle) ;
	sinRotateAngle = sin(angle) ;
	for( k = 0 ; k < 3 ; k++ )
		rot_p[k] = temp[k]+(cosRotateAngle*e1[k]+sinRotateAngle*e2[k]) ;

	return ;
}

// ������ϵ���ᾭ��p�����ת��axis��ת
void mathRotateRFrame(PNT3D p,     // [IN]	   ��ת�����
					  VEC3D axis,  // [IN]	   ��ת��
					  double angle,// [IN]     ��ת�Ƕ�
					  RFRAME& pLF) // [IN&OUT] ������������ϵ
{
	mathRotPnt(axis, p, angle, pLF.O, pLF.O) ;
	mathRotVec(axis, p, angle, pLF.X, pLF.X) ;
	mathRotVec(axis, p, angle, pLF.Y, pLF.Y) ;
	mathRotVec(axis, p, angle, pLF.Z, pLF.Z) ;

	return ;
}

// ��ʼ������ϵ
void mathInitRFrame(RFRAME& pLF)
{
	pLF.O[0] = pLF.O[1] = pLF.O[2] = 0.0 ;
	pLF.X[0] = 1.0 ;
	pLF.X[1] = pLF.X[2] = 0.0 ;
	pLF.Y[1] = 1.0 ;
	pLF.Y[0] = pLF.Y[2] = 0.0 ;
	pLF.Z[2] = 1.0 ;
	pLF.Z[1] = pLF.Z[0] = 0.0 ;
//	pLF.scale = 1.0 ;

	return ;
}

// ƽ������ϵ
void mathMoveRFrame(VEC3D translateVector, // [IN]     ƽ������
	                RFRAME& pLF)           // [IN&OUT] ������������ϵ
{
	pLF.O[0] += translateVector[0] ;
	pLF.O[1] += translateVector[1] ;
	pLF.O[2] += translateVector[2] ;

	return ;
}

// ��֪����������ϵ�µ�����㣬�����ھֲ�����ϵ�е�����ֵ
void mathTransWorldPnt3D(RFRAME& local_frame, // [IN] �ֲ�����ϵ
						 PNT3D world_point,   // [IN] ��������ϵ�µĵ�����
						 PNT3D local_point)   // [OUT]�ֲ�����ϵ�µĵ�����
{
	VEC3D trans_point ;

	trans_point[0] = world_point[0]-local_frame.O[0] ;
	trans_point[1] = world_point[1]-local_frame.O[1] ;
	trans_point[2] = world_point[2]-local_frame.O[2] ;

	local_point[0] = trans_point[0]*local_frame.X[0] +
		trans_point[1]*local_frame.X[1] +
		trans_point[2]*local_frame.X[2] ;
	local_point[1] = trans_point[0]*local_frame.Y[0] +
		trans_point[1]*local_frame.Y[1] +
		trans_point[2]*local_frame.Y[2] ;
	local_point[2] = trans_point[0]*local_frame.Z[0] +
		trans_point[1]*local_frame.Z[1] +
		trans_point[2]*local_frame.Z[2] ;
}

// ��֪���ھֲ�����ϵ�µ�����㣬��������������ϵ�е�����ֵ
void mathTransLocalPnt3D(RFRAME& local_frame, // [IN] �ֲ�����ϵ
						 PNT3D local_point,   // [IN] �ֲ�����ϵ�µĵ�����
						 PNT3D world_point)   // [OUT]��������ϵ�µĵ�����
{
	int k ;
	PNT3D p ;

	for( k = 0 ; k < 3 ; k++ )
		p[k] = local_point[0]*local_frame.X[k] +
		local_point[1]*local_frame.Y[k] +
		local_point[2]*local_frame.Z[k] +
		local_frame.O[k] ;
	world_point[0] = p[0] ;
	world_point[1] = p[1] ;
	world_point[2] = p[2] ;
}

// �������
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

// ��λ������
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

// ������e1��e2��תangle�Ƕ�
void mathRotVecXY(VEC3D e1,     // [IN] ����ת����
				  VEC3D e2,     // [IN] ��֮��ת������
				  double angle, // [IN] ��ת�Ƕ�
				  VEC3D rot_v)  // [OUT]��ת�������
{
	int k ;
	double cosRotateAngle, sinRotateAngle ;

	cosRotateAngle = cos(angle) ;
	sinRotateAngle = sin(angle) ;
	for( k = 0 ; k < 3 ; k++ )
		rot_v[k] = cosRotateAngle*e1[k]+sinRotateAngle*e2[k] ;
}

// ���㷴����
double mathACos(double cosAlfa)
{
	if( cosAlfa > 0.999999999999 )
		return 0.0 ;
	else
		if( cosAlfa < -0.999999999999 )
			return PI ;
		else
			return acos(cosAlfa) ;
}
 
// ��������3D�����ļн�: 0.0 <= angle <= PI1
double mathGetAngle(VEC3D v1, VEC3D v2, double min_len)
{
    double d1, d2 ;

	d1 = sqrt(v1[0]*v1[0]+v1[1]*v1[1]+v1[2]*v1[2]) ;
	d2 = sqrt(v2[0]*v2[0]+v2[1]*v2[1]+v2[2]*v2[2]) ;
	if( d1 < min_len ||
		d2 < min_len )
		return -1.0 ;
	else
	    return mathACos((v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])/(d1*d2)) ;
}
