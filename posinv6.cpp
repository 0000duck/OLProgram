#include "StdAfx.h"
#include <math.h>
#include <string.h>
#include "MathBase.h"

#define g_a1	138//150
#define g_a2	870//580
#define g_a3	124//210
#define g_d3	0
#define g_d4	975//617
#define radianPerAngle	(PI/180)
#define anglePerRadian (180/PI)
#define AXIS_COUNT 6

double  em = 0.0000000001;

static double max_ang[8]={PI,0,0.5*PI,2.0*PI,PI,2.0*PI,2.0*PI,2.0*PI};
static double min_ang[8]={-PI,-PI,-PI,-2.0*PI,-PI,-2.0*PI,-2.0*PI,-2.0*PI};
static int range[8]={0,0,0,0,0,0,0,0};


double tool_pos[6] = {-60,0,400,0.0,0.0,0.0};

//{-24.601,96.774,416.857,0.0,0.0,0.0};
//{26.145,10.409,267.700,0.0,0.0,0.0};



void CaliMatrixMultiplySlow(double *op1,double *op2,double *out,int m,int n,int l)
{
    int i,j,k;
    for(i = 0; i < m; i++ )
    {
        for(j = 0; j < l; j++ )
        {
            out[i*l+j] = 0.0;
            for( k = 0; k < n; k++ )
                out[i*l+j] += op1[i*n+k]*op2[k*l+j];
        }
    }
}


static void LU(double lineA[],double lineL[],double lineU[],int num){

    int i,j,k;
    double temp;

    for(i=0;i<num;i++){
        for(j=0;j<num;j++){
            if(i==j){
                lineL[i*num+j] = 1;
                lineU[i*num+j] = 0;
            }
            else
            {
                lineL[i*num+j] = 0;
                lineU[i*num+j] = 0;
            }
        }

    }

    for(j=0;j<num;j++){
        lineU[j] = lineA[j];
    }

    for(i=0;i<num;i++){
        lineL[i*num] = lineA[i*num]/lineU[0];
    }
    //U[1][] L[][1] U-L
    for(i=1;i<num;i++){

        for(j=i;j<num;j++){
            temp = 0;
            for(k=0;k<i;k++){
                temp += lineL[i*num+k]*lineU[k*num+j];
            }
            lineU[i*num+j] = lineA[i*num+j] - temp;
        }  //2

        for(j=i+1;j<num;j++){
            temp = 0;
            for(k=0;k<i;k++){
                temp += lineL[j*num+k]*lineU[k*num+i];
            }
            lineL[j*num+i] = (lineA[j*num+i] - temp)/lineU[i*num+i];
        }
    }
}

static void lowerTriangularMatrixInver(double lineLower[],double lineInver[],int num)
{

    int i,j,k;
    double temp;

    for(i=0;i<num;i++){
        lineInver[i*num+i] = 1;
    }

    for(j=0;j<num;j++){
        for(i=j+1;i<num;i++){
            lineInver[j*num+i] = 0;
        }
    }

    for(i=1;i<num;i++){
        for(j=0;j<=i-1;j++){
            temp = 0.0;
            for(k=j;k<=i-1;k++){
                temp += lineLower[i*num+k]*lineInver[k*num+j];
            }
            lineInver[i*num+j] = -temp;
        }
    }


}

static void upperTriangularMatrixInver(double lineUpper[],double lineInver[],int num)
{

    int i,j,k;
    double temp;

    for(i=0;i<num;i++){
        lineInver[i*num+i] = 1/lineUpper[i*num+i];
    }

    for(j=0;j<num;j++){
        for(i=0;i<j;i++){
            lineInver[j*num+i] = 0;
        }
    }

    for(j=1;j<num;j++){

        for(i=j-1;i>=0;i--){
            temp = 0.0;
            for(k=i+1;k<=j;k++){
                temp += lineUpper[i*num+k]*lineInver[k*num+j];
            }
            lineInver[i*num+j] = -temp/lineUpper[i*num+i];
        }

    }
}

void matrixInver(double lineA[],double lineAInver[], int num)
{
	//int num = 4;
    double lineL[4*4],lineU[4*4];
    double lineLInver[4*4],lineUInver[4*4];

    LU(lineA,lineL,lineU,4);
    lowerTriangularMatrixInver(lineL,lineLInver,4);

    upperTriangularMatrixInver(lineU,lineUInver,4);

    CaliMatrixMultiplySlow(lineUInver,lineLInver,lineAInver,4,4,4);
}


double Atan3(double y, double x)
{
    double at=0.0,at2=0.0;
    if (fabs(x)<em){
        if(y>0)
            at2=PI/2;
        else if (y<0)
            at2=3*PI/2;
        return at2;
    }
    at=atan(y/x);
    if ( x>0&&y>0 )
        at2=at;
    else if (x>0&&y<0)
        at2=2*PI+at;
    else if ((x<0)&&(y>0))
        at2=at+PI;
    else if ((x<0)&&(fabs(y)<em))
        at2=PI;
    else if ((x>0)&&(fabs(y)<em))
        at2=0;
    else if ((x<0)&&(y<0))
        at2=PI+at;
    return at2;
}
double Atan4(double y, double x)
{
    double at=0.0,at2=0.0;
    if (fabs(x)<em){
        if(y>0)
            at2=-3*PI/2;
        else if (y<0)
            at2=-PI/2;
        return at2;
    }
    at=atan(y/x);
    if ( x>0&&y>0 ){
        at2=at-2*PI;
    } else if ( x>0&&y<0 ){
        at2=at;
    } else if ( (x<0)&&(y>0) ){
        at2=at-PI;
    } else if ( (x<0)&&(fabs(y)<em) ){
        at2=-PI;
    } else if ( (x>0)&&(fabs(y)<em) ){
        at2=0;
    } else if ( (x<0)&&(y<0) ){
        at2=-PI+at;
    }
    return at2;
}
double AtanX(double x,int m)
{
    double at=0.0;
    if ( m==0 ){
        at=x;
    } else{
        if ( x>0.0 )
            at=x-2.0*PI;
        else
            at=x+2.0*PI;
    }
    return at;
}
double SelectAngle(double d_v1,double d_v2,double joint)
{
    double att=joint*radianPerAngle;

    if(fabs(d_v2-att)>PI/2.0&&fabs(d_v1-att)>PI/2.0){
        return (fabs(d_v1-att)>fabs(d_v2-att)?d_v1:d_v2);
    }
    else{
        return (fabs(d_v1-att)<fabs(d_v2-att)?d_v1:d_v2);
    }
}

double Acos2(double x,int m)
{
    double at=0.0,at2=0.0;
    if ( x>1 ) x=1;
    else if ( x<-1 ) x=-1;

    at=acos(x);
    if ( m==0.0 )
        at2=at;
    else
        at2=at-2*PI;

    return at2;
}
double atan2PI(double y, double x,double angle)
{
    double taa=0.0;
    if ( angle>=90 && angle<=360 ){
        taa=Atan3(y,x);
    }
    else if ( angle>=-360 && angle<= -90 ){
        taa=Atan4(y,x);
    }
    else if ( angle>-90 && angle<90 ){
        taa=atan2(y,x);
    }

    return taa;
}

void MULmatrix(double a[4][4], double b[4][4], double c[4][4])
{
    int i, j, k;
    double temp[4] = {0};
    for(i = 0; i < 4; i++){
        for(k = 0; k < 4; k ++)
            temp[k] = 0;
        for(j = 0; j < 4; j++){//当前行的每个元素
            for(k = 0; k < 4; k++){
                temp[k] += a[i][j] * b[j][k];
            }
        }
        for(k = 0; k < 4; k++){
            c[i][k] = temp[k];
        }
    }
}

void pos2tool_matrix(double pos[6],double matrix[4][4])
{
    double alfa,beta,gama;
    alfa =  pos[5];
    beta = pos[4];
    gama = pos[3];
    matrix[0][0] = cos(alfa)*cos(beta);
    matrix[0][1] = cos(alfa)*sin(beta)*sin(gama) - sin(alfa)*cos(gama);
    matrix[0][2] = cos(alfa)*sin(beta)*cos(gama) + sin(alfa)*sin(gama);
    matrix[0][3] = pos[0];

    matrix[1][0] = sin(alfa)*cos(beta);
    matrix[1][1] = sin(alfa)*sin(beta)*sin(gama) + cos(alfa)*cos(gama);
    matrix[1][2] = sin(alfa)*sin(beta)*cos(gama) - cos(alfa)*sin(gama);
    matrix[1][3] = pos[1];

    matrix[2][0] = -sin(beta);
    matrix[2][1] = cos(beta)*sin(gama);
    matrix[2][2] = cos(beta)*cos(gama);
    matrix[2][3] = pos[2];

    matrix[3][0] = 0;
    matrix[3][1] = 0;
    matrix[3][2] = 0;
    matrix[3][3] = 1;
}

void axis6_joint2matrix(double matrix[][4],double af[])
{
    int j=0;
    double Mutiply1 =  0.0;
    double Mutiply2 =  0.0;
    double Mutiply3 =  0.0;
    double Mutiply4 =  0.0;
    double Mutiply5 =  0.0;
    double Mutiply6 =  0.0;
    double Mutiply7 =0.0;
    double Mutiply8 = 0.0;
    double Mutiply9 = 0.0;
    double Mutiply10 = 0.0;
    double Mutiply11 = 0.0;
    double Mutiply12 = 0.0;
    double Mutiply13 = 0.0;
    double Mutiply14 = 0.0;
    double Mutiply15 = 0.0;

    double Mutiplay1=0.0;
    double Mutiplay2=0.0;
    double Mutiplay3=0.0;
    double Mutiplay4=0.0;
    double s[AXIS_COUNT]={0.0,0.0,0.0,0.0,0.0,0.0};
    double c[AXIS_COUNT]={0.0,0.0,0.0,0.0,0.0,0.0};
    double tempmatrix[4][4];
    double toolmatrix[4][4];

    for ( j=0;j<AXIS_COUNT;j++ ){
        s[j]=sin(af[j]*radianPerAngle);
        c[j]=cos(af[j]*radianPerAngle);
    };

    Mutiply1=s[0]*c[3];
    Mutiply2=c[0]*s[1]*s[2];
    Mutiply3=c[0]*c[1]*c[2];
    Mutiply4=s[0]*s[3];
    Mutiply5=c[0]*c[1]*s[2];
    Mutiply6=c[0]*s[1]*c[2];
    Mutiplay1=Mutiply2-Mutiply3;
    Mutiplay2=Mutiply5+Mutiply6;

    Mutiply7=s[0]*s[1]*s[2];
    Mutiply8=s[0]*c[1]*c[2];
    Mutiply9=s[0]*c[1]*s[2];
    Mutiply10=s[0]*s[1]*c[2];
    Mutiply11=c[0]*c[3];
    Mutiply12=c[0]*s[3];
    Mutiplay3=Mutiply7-Mutiply8;
    Mutiplay4=Mutiply9+Mutiply10;

    Mutiply13=c[1]*s[2]+s[1]*c[2];
    Mutiply14=c[1]*c[2]-s[1]*s[2];
    Mutiply15=c[1]*s[2]+s[1]*c[2];

    matrix[0][0]=s[5]*(Mutiply1+s[3]*Mutiplay1)+c[5]*(c[4]*(Mutiply4-c[3]*Mutiplay1)-s[4]*Mutiplay2);
    matrix[1][0]=-s[5]*(Mutiply11-s[3]*Mutiplay3)-c[5]*(c[4]*(Mutiply12+c[3]*Mutiplay3)+s[4]*Mutiplay4);
    matrix[2][0]=s[3]*s[5]*Mutiply13-c[5]*(s[4]*Mutiply14+c[3]*c[4]*Mutiply15);
    matrix[3][0]=0;
    matrix[0][1]=c[5]*(Mutiply1+s[3]*Mutiplay1)-s[5]*(c[4]*(Mutiply4-c[3]*Mutiplay1)-s[4]*Mutiplay2);
    matrix[1][1]=-c[5]*(Mutiply11-s[3]*Mutiplay3)+s[5]*(c[4]*(Mutiply12+c[3]*Mutiplay3)+s[4]*Mutiplay4);
    matrix[2][1]=s[3]*c[5]*Mutiply13+s[5]*(s[4]*Mutiply14+c[3]*c[4]*Mutiply15);
    matrix[3][1]=0;
    matrix[0][2]=-s[4]*(Mutiply4-c[3]*Mutiplay1)-c[4]*Mutiplay2;
    matrix[1][2]=s[4]*(Mutiply12+c[3]*Mutiplay3)-c[4]*Mutiplay4;
    matrix[2][2]=c[3]*s[4]*Mutiply13-c[4]*Mutiply14;
    matrix[3][2]=0;
    matrix[0][3]=g_a1*c[0]-g_d4*Mutiplay2-g_a3*Mutiplay1-g_d3*s[0]+g_a2*c[0]*c[1];
    matrix[1][3]=g_a1*s[0]-g_d4*Mutiplay4-g_a3*Mutiplay3+g_d3*c[0]+g_a2*s[0]*c[1];
    matrix[2][3]=-g_a3*Mutiply13-g_d4*Mutiply14-g_a2*s[1];
    matrix[3][3]=1;

    pos2tool_matrix(tool_pos,toolmatrix);


    MULmatrix(matrix, toolmatrix,tempmatrix);
    memcpy(matrix,tempmatrix,sizeof(double)*16);
}
void axis6_matrix2joint(double xp_joint[],double T[4][4],int& outside)
{
    double px=0.0,py=0.0,pz=0.0;
    double ox=0.0,oy=0.0,oz=0.0;
    double ax=0.0,ay=0.0,az=0.0;
    double s[6]={0.0,},c[6]={0.0,};
    double u=0.0,v=0.0,m=0.0;
    double genhao=0.0;						  //判断根号下的数据的变量
    double Ra[6]={0.0,},Fa[6]={0.0,},Ta[6]={0.0,};	 //关节角度未判断前的Atan2值

    double toolmatrix[4][4];
    double inverse_matrix[4][4];
    double temp[4][4];
    int i,j;
    double a2Square = 0.0;
    double d4Square = 0.0;
    double a3Square = 0.0;
    double vSquare = 0.0;
    double uSquare = 0.0;
    //公式简化变量
    double At2_VU = 0.0;			//代表Atan2(-v,u)
    double At2SgM = 0.0;			//代表Atan2(sqrt(genhao),m)
    double At2_D4A3 = 0.0;			//代表Atan2(-d4,a3)
    double xp_j0=0.0;
    double xp_j1=0.0,xp_j3=0.0;
    double cc=0.0;				 //cos(Ta[1]+Ta[2])
    double ss=0.0;				 //sin(Ta[1]+Ta[2])
    double zya=-g_d4;
    double zyb=g_a3;
    double originalmatrix[4][4];
    double tempLine[16];
    double invLine[16];

    //	double tmpmatrix[4][4];
    //	double invmatrix[4][4];
    pos2tool_matrix(tool_pos,toolmatrix);
    // memcpy(originalmatrix,T,sizeof(double)*16);
    // GetMatrix(L,toolmatrix);


    for (i = 0; i <4; i++){
        for (j = 0; j <4; j++){
            tempLine[i*4+j] = toolmatrix[i][j];
        }
    }

    matrixInver(tempLine,invLine,4);

    for (i = 0; i < 4; i++){
        for (j = 0; j <4; j++){
            inverse_matrix[i][j] = invLine[i*4+j];
        }
    }

    for(i=0;i<4;i++){
        for(j=0;j<4;j++){
            temp[i][j]=inverse_matrix[i][j];
        }
    }
    //memcpy(temp,inverse_matrix,sizeof(double)*16);
    MULmatrix(T, temp,originalmatrix);


    a2Square = g_a2*g_a2;
    d4Square = g_d4*g_d4;
    a3Square = g_a3*g_a3;

    xp_j0=xp_joint[0];						///备份theta1的值
    xp_j1=xp_joint[3];						///备份theta4的值
    xp_j3=xp_joint[5];						///备份theta6的值

    px=originalmatrix[0][3];
    py=originalmatrix[1][3];
    pz=originalmatrix[2][3];

    ox=originalmatrix[0][1];
    oy=originalmatrix[1][1];
    oz=originalmatrix[2][1];

    ax=originalmatrix[0][2];
    ay=originalmatrix[1][2];
    az=originalmatrix[2][2];

    //******************************计算theta1********************************//
    genhao=sqrt(px*px+py*py-g_d3*g_d3);
    //	Ra[0]=atan2PI(py,px,xp_j0);
    //	Fa[0]=atan2PI(-py,-px,xp_j0);
    Ra[0]=atan2(py,px)-atan2(g_d3,genhao);
    Fa[0]=atan2(py,px)-atan2(g_d3,-genhao);
    //	Ra[0]=atan2(-px,py)+atan2(genhao,g_d3);
    //	Fa[0]=atan2(-px,py)-atan2(genhao,g_d3);
    Ra[0]=AtanX(Ra[0],range[0]);
    Fa[0]=AtanX(Fa[0],range[0]);
    Ta[0]=SelectAngle(Ra[0],Fa[0],xp_joint[0]);
    if ( (Ta[0]>=min_ang[0])&&(Ta[0]<=max_ang[0]) ){
        xp_joint[0]=Ta[0]*anglePerRadian;
    } else{
        outside|=0x01;
    }

    //******************************计算theta2********************************//
    c[0]=cos(Ta[0]);
    s[0]=sin(Ta[0]);
    u=c[0]*px+s[0]*py-g_a1;
    v=pz;
    //计算V 和U 的平方
    vSquare = v*v;
    uSquare = u*u;

    m=a3Square+d4Square-uSquare-vSquare-a2Square;
    m=-0.5*m/g_a2;
    genhao=vSquare+uSquare-m*m;
    if ( genhao>=0 ){
        At2_VU = atan2(-v,u);
        At2SgM = atan2(sqrt(genhao),m);
        Ra[1]=At2_VU+At2SgM;
        Fa[1]=At2_VU-At2SgM;
        Ra[1]=AtanX(Ra[1],range[1]);
        Fa[1]=AtanX(Fa[1],range[1]);
        Ta[1]=SelectAngle(Ra[1],Fa[1],xp_joint[1]);

        if ( (Ta[1]>=min_ang[1])&&(Ta[1]<=max_ang[1]) ){
            xp_joint[1]=Ta[1]*anglePerRadian;
        } else{
            outside|=0x02;
        }
    } else{
        outside|=0x02;
    }

    //******************************计算theta3********************************//
    m=a2Square+d4Square-uSquare-vSquare+a3Square;
    m=-m/(2*g_a2);
    genhao=d4Square+a3Square-m*m;

    if ( genhao>=0 ){
        At2_D4A3= atan2(zya,zyb);
        At2SgM = atan2(sqrt(genhao),m);
        Ra[2]=At2_D4A3+At2SgM;
        Fa[2]=At2_D4A3-At2SgM;
        Ra[2]=AtanX(Ra[2],range[2]);
        Fa[2]=AtanX(Fa[2],range[2]);
        Ta[2]=SelectAngle(Ra[2],Fa[2],xp_joint[2]);
        if ( (Ta[2]>=min_ang[2])&&(Ta[2]<=max_ang[2]) ){
            xp_joint[2]=Ta[2]*anglePerRadian;
        } else{
            outside|=0x04;
        }
    } else{
        outside|=0x04;
    }

    //******************************计算theta5********************************//
    s[1]=sin(Ta[1]);
    s[2]=sin(Ta[2]);

    c[1]=cos(Ta[1]);
    c[2]=cos(Ta[2]);
    cc=cos(Ta[1]+Ta[2]);
    ss=sin(Ta[1]+Ta[2]);

    u=c[2]*(-c[0]*s[1]*ax-s[0]*s[1]*ay-c[1]*az);
    v=s[2]*(c[0]*c[1]*ax+s[0]*c[1]*ay-s[1]*az);
    Ra[4]=Acos2(u-v,range[4]);
    Fa[4]=-Ra[4];
    Ta[4]=SelectAngle(Ra[4],Fa[4],xp_joint[4]);

    //******************************计算theta4********************************//
    if ( fabs(cos(Ta[4]))!=1 )
    {
        s[4]=sin(Ra[4]) ;
        c[4]=cos(Ra[4]);
        u=(c[0]*ay-s[0]*ax)/s[4];
        v=(c[0]*cc*ax+s[0]*cc*ay-ss*az)/(-s[4]);

        Ra[3]=atan2PI(u, v,xp_j1);
        Ra[3]=AtanX(Ra[3],range[3]);


        s[4]=sin(Fa[4]) ;
        c[4]=cos(Fa[4]);
        u=(c[0]*ay-s[0]*ax)/s[4];
        v=(c[0]*cc*ax+s[0]*cc*ay-ss*az)/(-s[4]);

        Fa[3]=atan2PI(u, v,xp_j1);
        Fa[3]=AtanX(Fa[3],range[3]);
        Ta[3]=SelectAngle(Ra[3],Fa[3],xp_joint[3]);
        if ( Ta[3]==Fa[3]){
            Ta[4]=Fa[4];
        } else{
            Ta[4]=Ra[4];
        }
        if ( (Ta[3]>=min_ang[3])&&(Ta[3]<=max_ang[3]) ){
            xp_joint[3]=Ta[3]*anglePerRadian;
        } else{
            outside|=0x08;
        }
    }
    else{
        Ta[3]=xp_joint[3]*radianPerAngle;
        xp_joint[3]=xp_j1;
    }
    if ( (Ta[4]>=min_ang[4])&&(Ta[4]<=max_ang[4]) ){
        xp_joint[4]=Ta[4]*anglePerRadian;
    } else{
        outside|=0x10;
    }

    //******************************计算theta6********************************//
    s[3]=sin(Ta[3]);
    c[3]=cos(Ta[3]);
    s[4]=sin(Ta[4]);
    c[4]=cos(Ta[4]);

    u=(((c[4]*c[3]*c[2]-s[4]*s[2])*c[1]-(c[4]*c[3]*s[2]+s[4]*c[2])*s[1])*c[0]+c[4]*s[3]*s[0])*ox+(((c[4]*c[3]*c[2]-s[4]*s[2])*c[1]-(c[4]*c[3]*s[2]+s[4]*c[2])*s[1])*s[0]-c[4]*s[3]*c[0])*oy+(-(c[4]*c[3]*c[2]-s[4]*s[2])*s[1]-(c[4]*c[3]*s[2]+s[4]*c[2])*c[1])*oz;
    u=-u;

    v=((s[3]*c[2]*c[1]-s[3]*s[2]*s[1])*c[0]-c[3]*s[0])*ox+((s[3]*c[2]*c[1]-s[3]*s[2]*s[1])*s[0]+c[3]*c[0])*oy+(-s[3]*c[2]*s[1]-s[3]*s[2]*c[1])*oz;
    v=-v;

    Ra[5]=atan2PI(u, v,xp_j3);
    Fa[5]=atan2PI(-u, -v,xp_j3);

    Ra[5]=AtanX(Ra[5],range[5]);
    Fa[5]=AtanX(Fa[5],range[5]);
    Ta[5]=SelectAngle(Ra[5],Fa[5],xp_j3);

    if ( (Ta[5]>=min_ang[5])&&(Ta[5]<=max_ang[5]) ){
        xp_joint[5]=Ta[5]*anglePerRadian;
    } else{
        outside|=0x20;
    }
}

//已知执行器方向，首先构造工具坐标系。其z轴为工具方向，x轴为前进方向，y轴为ZxX方向。进而获得工具坐标系相对于基准系的姿态旋转矩阵
// orient:[IN] 工具法向
// toolVec:[IN] 工具姿态方向 （设置为1,0,0）
// ToolRotaMatrix3:[OUT] 旋转矩阵
// 此处输入都为机器人坐标系下的坐标
static void GenToolRotaMatrix4(PNT3D cutPnt, VEC3D cutVec, VEC3D toolVec, double ToolRotaMatrix[4][4])
{
	VEC3D X,Y,Z;
	
	for (int i=0; i<3; i++)
	{
		Z[i] = -cutVec[i];
	}

	double err=fabs((fabs(mathOProduct(toolVec,Z))-1));
	if(err<MIN_LEN)
	{
		AfxMessageBox(_T("the move direction is not correct, please check!"));
		exit(-1);
		return;
	}
	else
	{
		mathVProduct(toolVec,Z,X);
		mathUniVec(X);
		mathVProduct(Z,X,Y);
	}

	ToolRotaMatrix[0][0]=X[0];
	ToolRotaMatrix[0][1]=Y[0];
	ToolRotaMatrix[0][2]=Z[0];
	ToolRotaMatrix[0][3] = cutPnt[0];

	ToolRotaMatrix[1][0]=X[1];
	ToolRotaMatrix[1][1]=Y[1];
	ToolRotaMatrix[1][2]=Z[1];
	ToolRotaMatrix[1][3] = cutPnt[1];

	ToolRotaMatrix[2][0]=X[2];
	ToolRotaMatrix[2][1]=Y[2];
	ToolRotaMatrix[2][2]=Z[2];
	ToolRotaMatrix[2][3] = cutPnt[2];

	ToolRotaMatrix[3][0] = 0;
	ToolRotaMatrix[3][1] = 0;
	ToolRotaMatrix[3][2] = 0;
	ToolRotaMatrix[3][3] = 1;
}

#define iszero( x)	(fabs(x)<em?1:0)

void Matrix2pose(double pos[6], double matrix[][4])
{
	double nx=0.0,ny=0.0,nz=0.0,ox=0.0,oy=0.0,oz=0.0,az=0.0,px=0.0,py=0.0,pz=0.0;
	double r_y = 0.0;
	double r_x = 0.0;
	double r_z = 0.0;

	nx=matrix[0][0];
	ny=matrix[1][0];
	nz=matrix[2][0];

	ox=matrix[0][1];
	oy=matrix[1][1];
	oz=matrix[2][1];

	az=matrix[2][2];

	px=matrix[0][3];
	py=matrix[1][3];
	pz=matrix[2][3];
	r_y=atan2(-nz,sqrt(nx*nx+ny*ny));  //beta
	if ( iszero(cos(r_y))==0 ){
		r_z=atan2(ny,nx);							  //alfa
		r_x=atan2(oz,az);							//gama
	} 
	else{
		if ( r_y>0 ){
			r_z=0;
			r_x=atan2(ox,oy);
		}
		else{
			r_z=0;
			r_x=-atan2(ox,oy);
		}
	}
	pos[0]=px;
	pos[1]=py;
	pos[2]=pz;
	pos[3]=r_x;
	pos[4]=r_y;
	pos[5]=r_z;
}
// 
// void MatrixBuit(double Matrix[][4],double pos[])
// {
// 	double s[3]={0,};
// 	double c[3]={0,}; 
// 	c[0]=cos(pos[3]);
// 	c[1]=cos(pos[4]);
// 	c[2]=cos(pos[5]);
// 	s[0]=sin(pos[3]);
// 	s[1]=sin(pos[4]);
// 	s[2]=sin(pos[5]);
// 
// 	Matrix[0][0]=c[2]*c[1]; 					 
// 	Matrix[0][1]=c[2]*s[1]*s[0]-s[2]*c[0];
// 	Matrix[0][2]= c[2]*s[1]*c[0]+s[2]*s[0];
// 	Matrix[0][3]=pos[0];
// 
// 	Matrix[1][0]=s[2]*c[1];
// 	Matrix[1][1]=s[2]*s[1]*s[0]+c[2]*c[0];
// 	Matrix[1][2]=s[2]*s[1]*c[0]-c[2]*s[0];
// 	Matrix[1][3]=pos[1];
// 
// 	Matrix[2][0]=-s[1];
// 	Matrix[2][1]=c[1]*s[0];
// 	Matrix[2][2]=c[1]*c[0];
// 	Matrix[2][3]=pos[2];
// 
// 	Matrix[3][0]=0;
// 	Matrix[3][1]=0;
// 	Matrix[3][2]=0;
// 	Matrix[3][3]=1;
//
void GenToolPosToJoint(PNT3D cutPnt, VEC3D cutVec, VEC3D toolVec, double dJoint[6], int& outside,double ToolRotaMatrix[4][4] )
{
	//double ToolRotaMatrix[4][4];
	GenToolRotaMatrix4(cutPnt, cutVec, toolVec, ToolRotaMatrix);
	axis6_matrix2joint(dJoint,ToolRotaMatrix,outside);
}