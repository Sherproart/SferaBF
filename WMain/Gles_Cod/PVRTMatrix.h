#ifndef _PVRTMATRIX_H_
#define _PVRTMATRIX_H_

//#include "PVRTGlobal.h"


class PVRTMATRIXf
{
public:
    float* operator [] ( const int Row )
	{
		return &f[Row<<2];
	}
	float f[16];	/*!< Array of float */
};

/*!***************************************************************************
 @Function			PVRTMatrixIdentityF
 @Output			mOut	Set to identity
 @Description		Reset matrix to identity matrix.
*****************************************************************************/
void PVRTMatrixIdentityF(PVRTMATRIXf &mOut);

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
	const float	fX,
	const float	fY,
	const float	fZ);
	
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
	const float fZ);


/*!***************************************************************************
 @Function Name		PVRTMatrixRotationXF
 @Output			mOut	Rotation matrix
 @Input				fAngle	Angle of the rotation
 @Description		Create an X rotation matrix mOut.
*****************************************************************************/
void PVRTMatrixRotationXF(PVRTMATRIXf	&mOut, const float fAngle);


/*!***************************************************************************
 @Function Name		PVRTMatrixRotationZF
 @Output			mOut	Rotation matrix
 @Input				fAngle	Angle of the rotation
 @Description		Create an Z rotation matrix mOut.
*****************************************************************************/
void PVRTMatrixRotationZF(
	PVRTMATRIXf	&mOut,
	const float fAngle);
	
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
	const PVRTMATRIXf	&mB);
	
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
	const float zf);

// поворот вокруг произвольной оси, которая задается вектором Vx, Vy, Vz
// rotate around vector Vx, Vy, Vz
// A - rotate angle
void glmRotate_V(PVRTMATRIXf	&mOut,const float A, float Vx, float Vy, float Vz);
	
		
	
#endif


