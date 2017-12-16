#include "stdafx.h"
#include <math.h>
#include "PVRTMatrix.h"
#include <string.h>

/****************************************************************************
** Constants
****************************************************************************/
static const PVRTMATRIXf	c_mIdentity = {
	{
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
	}
};

/*!***************************************************************************
 @Function			PVRTMatrixIdentityF
 @Output			mOut	Set to identity
 @Description		Reset matrix to identity matrix.
*****************************************************************************/
void PVRTMatrixIdentityF(PVRTMATRIXf &mOut)
{
	memset(&mOut, 0, sizeof(PVRTMATRIXf));
	mOut.f[ 0]=1.0f;
	mOut.f[ 5]=1.0f;
	mOut.f[10]=1.0f;
	mOut.f[15]=1.0f;
}

/*!***************************************************************************
 @Function Name		PVRTMatrixTranslationF
 @Output			mOut	Translation matrix
 @Input				fX		X component of the translation
 @Input				fY		Y component of the translation
 @Input				fZ		Z component of the translation
 @Description		Build a transaltion matrix mOut using fX, fY and fZ.
*****************************************************************************/
void PVRTMatrixTranslationF(
	PVRTMATRIXf	&mOut,
	const float fX,
	const float fY,
	const float fZ)
{
	memcpy(&mOut,&c_mIdentity,sizeof(PVRTMATRIXf));
	mOut.f[12]=fX;
	mOut.f[13]=fY;
	mOut.f[14]=fZ;
	/*
	mOut.f[ 0]=1.0f;	mOut.f[ 4]=0.0f;	mOut.f[ 8]=0.0f;	mOut.f[12]=fX;
	mOut.f[ 1]=0.0f;	mOut.f[ 5]=1.0f;	mOut.f[ 9]=0.0f;	mOut.f[13]=fY;
	mOut.f[ 2]=0.0f;	mOut.f[ 6]=0.0f;	mOut.f[10]=1.0f;	mOut.f[14]=fZ;
	mOut.f[ 3]=0.0f;	mOut.f[ 7]=0.0f;	mOut.f[11]=0.0f;	mOut.f[15]=1.0f;
	*/
}


/*!***************************************************************************
 @Function Name		PVRTMatrixScalingF
 @Output			mOut	Scale matrix
 @Input				fX		X component of the scaling
 @Input				fY		Y component of the scaling
 @Input				fZ		Z component of the scaling
 @Description		Build a scale matrix mOut using fX, fY and fZ.
*****************************************************************************/
void PVRTMatrixScalingF(
	PVRTMATRIXf	&mOut,
	const float fX,
	const float fY,
	const float fZ)
{
	mOut.f[ 0]=fX;		mOut.f[ 4]=0.0f;	mOut.f[ 8]=0.0f;	mOut.f[12]=0.0f;
	mOut.f[ 1]=0.0f;	mOut.f[ 5]=fY;		mOut.f[ 9]=0.0f;	mOut.f[13]=0.0f;
	mOut.f[ 2]=0.0f;	mOut.f[ 6]=0.0f;	mOut.f[10]=fZ;		mOut.f[14]=0.0f;
	mOut.f[ 3]=0.0f;	mOut.f[ 7]=0.0f;	mOut.f[11]=0.0f;	mOut.f[15]=1.0f;
}

/*!***************************************************************************
 @Function Name		PVRTMatrixRotationXF
 @Output			mOut	Rotation matrix
 @Input				fAngle	Angle of the rotation
 @Description		Create an X rotation matrix mOut.
*****************************************************************************/
void PVRTMatrixRotationXF(
	PVRTMATRIXf	&mOut,
	const float fAngle)
{
	float		fCosine, fSine;
	fCosine =	cos(fAngle);
    fSine =		sin(fAngle);

    memcpy(&mOut,&c_mIdentity,sizeof(PVRTMATRIXf)); 
	mOut.f[ 5]=fCosine;		mOut.f[ 9]=fSine;	
	mOut.f[ 6]=-fSine;		mOut.f[ 10]=fCosine;	
	/* Create the trigonometric matrix corresponding to X Rotation */
	//mOut.f[ 0]=1.0f;	mOut.f[ 4]=0.0f;	mOut.f[ 8]=0.0f;	mOut.f[12]=0.0f;
	//mOut.f[ 1]=0.0f;	mOut.f[ 5]=fCosine;	mOut.f[ 9]=fSine;	mOut.f[13]=0.0f;
	//mOut.f[ 2]=0.0f;	mOut.f[ 6]=-fSine;	mOut.f[10]=fCosine;	mOut.f[14]=0.0f;
	//mOut.f[ 3]=0.0f;	mOut.f[ 7]=0.0f;	mOut.f[11]=0.0f;	mOut.f[15]=1.0f;
}

/*!***************************************************************************
 @Function Name		PVRTMatrixRotationZF
 @Output			mOut	Rotation matrix
 @Input				fAngle	Angle of the rotation
 @Description		Create an Z rotation matrix mOut.
*****************************************************************************/
void PVRTMatrixRotationZF(
	PVRTMATRIXf	&mOut,
	const float fAngle)
{
	float		fCosine, fSine;
	fCosine =	cos(fAngle);
    fSine =		sin(fAngle);
     
    memcpy(&mOut,&c_mIdentity,sizeof(PVRTMATRIXf)); 
	mOut.f[ 0]=fCosine;		mOut.f[ 4]=fSine;	
	mOut.f[ 1]=-fSine;		mOut.f[ 5]=fCosine;	

	/* Create the trigonometric matrix corresponding to Z Rotation */
	// mOut.f[ 0]=fCosine;		mOut.f[ 4]=fSine;	mOut.f[ 8]=0.0f;	mOut.f[12]=0.0f;
	// mOut.f[ 1]=-fSine;		mOut.f[ 5]=fCosine;	mOut.f[ 9]=0.0f;	mOut.f[13]=0.0f;
	// mOut.f[ 2]=0.0f;		mOut.f[ 6]=0.0f;	mOut.f[10]=1.0f;	mOut.f[14]=0.0f;
	// mOut.f[ 3]=0.0f;		mOut.f[ 7]=0.0f;	mOut.f[11]=0.0f;	mOut.f[15]=1.0f;
}

/*!***************************************************************************
 @Function			PVRTMatrixMultiplyF
 @Output			mOut	Result of mA x mB
 @Input				mA		First operand
 @Input				mB		Second operand
 @Description		Multiply mA by mB and assign the result to mOut
					(mOut = p1 * p2). A copy of the result matrix is done in
					the function because mOut can be a parameter mA or mB.
*****************************************************************************/
void PVRTMatrixMultiplyF(
	PVRTMATRIXf			&mOut,
	const PVRTMATRIXf	&mA,
	const PVRTMATRIXf	&mB)
{
	PVRTMATRIXf mRet;

	/* Perform calculation on a dummy matrix (mRet) */
	mRet.f[ 0] = mA.f[ 0]*mB.f[ 0] + mA.f[ 4]*mB.f[ 1] + mA.f[ 8]*mB.f[ 2] + mA.f[12]*mB.f[3];
	mRet.f[ 1] = mA.f[ 1]*mB.f[ 0] + mA.f[ 5]*mB.f[ 1] + mA.f[ 9]*mB.f[ 2] + mA.f[13]*mB.f[3];
	mRet.f[ 2] = mA.f[ 2]*mB.f[ 0] + mA.f[ 6]*mB.f[ 1] + mA.f[10]*mB.f[ 2] + mA.f[14]*mB.f[3];
	mRet.f[ 3] = mA.f[ 3]*mB.f[ 0] + mA.f[ 7]*mB.f[ 1] + mA.f[11]*mB.f[ 2] + mA.f[15]*mB.f[3];

	mRet.f[ 4] = mA.f[ 0]*mB.f[ 4] + mA.f[ 4]*mB.f[ 5] + mA.f[ 8]*mB.f[ 6] + mA.f[12]*mB.f[7];
	mRet.f[ 5] = mA.f[ 1]*mB.f[ 4] + mA.f[ 5]*mB.f[ 5] + mA.f[ 9]*mB.f[ 6] + mA.f[13]*mB.f[7];
	mRet.f[ 6] = mA.f[ 2]*mB.f[ 4] + mA.f[ 6]*mB.f[ 5] + mA.f[10]*mB.f[ 6] + mA.f[14]*mB.f[7];
	mRet.f[ 7] = mA.f[ 3]*mB.f[ 4] + mA.f[ 7]*mB.f[ 5] + mA.f[11]*mB.f[ 6] + mA.f[15]*mB.f[7];

	mRet.f[ 8] = mA.f[ 0]*mB.f[ 8] + mA.f[ 4]*mB.f[ 9] + mA.f[ 8]*mB.f[10] + mA.f[12]*mB.f[11];
	mRet.f[ 9] = mA.f[ 1]*mB.f[ 8] + mA.f[ 5]*mB.f[ 9] + mA.f[ 9]*mB.f[10] + mA.f[13]*mB.f[11];
	mRet.f[10] = mA.f[ 2]*mB.f[ 8] + mA.f[ 6]*mB.f[ 9] + mA.f[10]*mB.f[10] + mA.f[14]*mB.f[11];
	mRet.f[11] = mA.f[ 3]*mB.f[ 8] + mA.f[ 7]*mB.f[ 9] + mA.f[11]*mB.f[10] + mA.f[15]*mB.f[11];

	mRet.f[12] = mA.f[ 0]*mB.f[12] + mA.f[ 4]*mB.f[13] + mA.f[ 8]*mB.f[14] + mA.f[12]*mB.f[15];
	mRet.f[13] = mA.f[ 1]*mB.f[12] + mA.f[ 5]*mB.f[13] + mA.f[ 9]*mB.f[14] + mA.f[13]*mB.f[15];
	mRet.f[14] = mA.f[ 2]*mB.f[12] + mA.f[ 6]*mB.f[13] + mA.f[10]*mB.f[14] + mA.f[14]*mB.f[15];
	mRet.f[15] = mA.f[ 3]*mB.f[12] + mA.f[ 7]*mB.f[13] + mA.f[11]*mB.f[14] + mA.f[15]*mB.f[15];
    

    /* Old wariant */
    /*
    mRet.f[0] = mA.f[0] * mB.f[0] + mA.f[1] * mB.f[4] + mA.f[2] * mB.f[8] + mA.f[3] * mB.f[12];
    mRet.f[1] = mA.f[0] * mB.f[1] + mA.f[1] * mB.f[5] + mA.f[2] * mB.f[9] + mA.f[3] * mB.f[13];
    mRet.f[2] = mA.f[0] * mB.f[2] + mA.f[1] * mB.f[6] + mA.f[2] * mB.f[10] + mA.f[3] * mB.f[14];
    mRet.f[3] = mA.f[0] * mB.f[3] + mA.f[1] * mB.f[7] + mA.f[2] * mB.f[11] + mA.f[3] * mB.f[15];

    mRet.f[4] = mA.f[4] * mB.f[0] + mA.f[5] * mB.f[4] + mA.f[6] * mB.f[8] + mA.f[7] * mB.f[12];
    mRet.f[5] = mA.f[4] * mB.f[1] + mA.f[5] * mB.f[5] + mA.f[6] * mB.f[9] + mA.f[7] * mB.f[13];
    mRet.f[6] = mA.f[4] * mB.f[2] + mA.f[5] * mB.f[6] + mA.f[6] * mB.f[10] + mA.f[7] * mB.f[14];
    mRet.f[7] = mA.f[4] * mB.f[3] + mA.f[5] * mB.f[7] + mA.f[6] * mB.f[11] + mA.f[7] * mB.f[15];

    mRet.f[8] = mA.f[8] * mB.f[0] + mA.f[9] * mB.f[4] + mA.f[10] * mB.f[8] + mA.f[11] * mB.f[12];
    mRet.f[9] = mA.f[8] * mB.f[1] + mA.f[9] * mB.f[5] + mA.f[10] * mB.f[9] + mA.f[11] * mB.f[13];
    mRet.f[10] = mA.f[8] * mB.f[2] + mA.f[9] * mB.f[6] + mA.f[10] * mB.f[10] + mA.f[11] * mB.f[14];
    mRet.f[11] = mA.f[8] * mB.f[3] + mA.f[9] * mB.f[7] + mA.f[10] * mB.f[11] + mA.f[11] * mB.f[15];

    mRet.f[12] = mA.f[12] * mB.f[0] + mA.f[13] * mB.f[4] + mA.f[14] * mB.f[8] + mA.f[15] * mB.f[12];
    mRet.f[13] = mA.f[12] * mB.f[1] + mA.f[13] * mB.f[5] + mA.f[14] * mB.f[9] + mA.f[15] * mB.f[13];
    mRet.f[14] = mA.f[12] * mB.f[2] + mA.f[13] * mB.f[6] + mA.f[14] * mB.f[10] + mA.f[15] * mB.f[14];
    mRet.f[15] = mA.f[12] * mB.f[3] + mA.f[13] * mB.f[7] + mA.f[14] * mB.f[11] + mA.f[15] * mB.f[15];
    */

	/* Copy result in pResultMatrix */
	mOut = mRet;
}

/*!***************************************************************************
 @Function		PVRTMatrixOrthoLHF
 @Output		mOut		Orthographic matrix
 @Input			w			Width of the screen
 @Input			h			Height of the screen
 @Input			zn			Near clipping distance
 @Input			zf			Far clipping distance
 @Input			bRotate		Should we rotate it ? (for upright screens)
 @Description	Create an orthographic matrix.
*****************************************************************************/
void PVRTMatrixOrthoLHF(
	PVRTMATRIXf	&mOut,
	const float w,
	const float h,
	const float zn,
	const float zf
	)
{
	mOut.f[ 0] = 2 / w;
	mOut.f[ 1] = 0;
	mOut.f[ 2] = 0;
	mOut.f[ 3] = 0;

	mOut.f[ 4] = 0;
	mOut.f[ 5] = 2 / h;
	mOut.f[ 6] = 0;
	mOut.f[ 7] = 0;

	mOut.f[ 8] = 0;
	mOut.f[ 9] = 0;
	mOut.f[10] = 2 / (zf - zn);
	mOut.f[11] = 0;

	mOut.f[12] = 0;
	mOut.f[13] = 0;
	mOut.f[14] = (zn+zf)/(zn-zf);
	mOut.f[15] = 1;

}//PVRTMatrixOrthoLHF----------------------


// rotate around vector Vx, Vy, Vz
// A - угол поворота
void glmRotate_V(PVRTMATRIXf	&mOut,const float A, float Vx, float Vy, float Vz)
{
	float	CosA, SinA;
	CosA =	cos(A);
    SinA =	sin(A);

	mOut.f[ 0]=CosA+(1-CosA)*Vx*Vx; mOut.f[ 1]=(1-CosA)*Vx*Vy-SinA*Vz; 
					mOut.f[ 2]=(1-CosA)*Vx*Vz+SinA*Vy;  mOut.f[ 3]=0;
    mOut.f[ 4]=(1-CosA)*Vy*Vx+SinA*Vz; mOut.f[ 5]=CosA+(1-CosA)*Vy*Vy;
					mOut.f[ 6]=(1-CosA)*Vy*Vz-SinA*Vx;  mOut.f[ 7]=0;
	mOut.f[ 8]=(1-CosA)*Vz*Vx-SinA*Vy; mOut.f[ 9]=(1-CosA)*Vz*Vy+SinA*Vx;
					mOut.f[10]=CosA+(1-CosA)*Vz*Vz;   mOut.f[11]=0;
	mOut.f[12]=0;  mOut.f[13]=0;  mOut.f[14]=0;  mOut.f[15]=1;


}//glmRotate_V------------------------------

