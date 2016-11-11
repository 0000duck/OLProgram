#ifndef _POSINV_H_
#define _POSINV_H_


#define ERR_OUTOFRANGE_AXIS_1 0x0001
#define ERR_OUTOFRANGE_AXIS_2 0x0002
#define ERR_OUTOFRANGE_AXIS_3 0x0004
#define ERR_OUTOFRANGE_AXIS_4 0x0008
#define ERR_OUTOFRANGE_AXIS_5 0x0010
#define ERR_OUTOFRANGE_AXIS_6 0x0020
#define ERR_OUTOFRANGE_AXIS_7 0x0040
#define ERR_OUTOFRANGE_AXIS_8 0x0080


void GenToolPosToJoint(PNT3D cutPnt, VEC3D cutVec, VEC3D toolVec, double dJoints[6], int& nErrFlag,double ToolRotaMatrix[4][4] );
#endif // _POSINV_H_

