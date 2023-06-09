//=========================================================
// LPC1114 Project
//=========================================================
// File Name : fixedpoint.c
// Function  : Fixed Point Library Routine
//---------------------------------------------------------
// Rev.01 2010.08.17 Munetomo Maruyama
//---------------------------------------------------------
// Copyright (C) 2010-2011 Munetomo Maruyama
//=========================================================
// ---- License Information -------------------------------
// Anyone can FREELY use this code fully or partially
// under conditions shown below.
// 1. You may use this code only for individual purpose,
//    and educational purpose.
//    Do not use this code for business even if partially.
// 2. You should use this code under the GNU GPL.
// 3. You should remain this header text in your codes
//   including Copyright credit and License Information.
// 4. Your codes should inherit this license information.
//=========================================================
// ---- Patent Notice -------------------------------------
// I have not cared whether this system (hw + sw) causes
// infringement on the patent, copyright, trademark,
// or trade secret rights of others. You have all
// responsibilities for determining if your designs
// and products infringe on the intellectual property
// rights of others, when you use technical information
// included in this system for your business.
//=========================================================
// ---- Disclaimers ---------------------------------------
// The function and reliability of this system are not
// guaranteed. They may cause any damages to loss of
// properties, data, money, profits, life, or business.
// By adopting this system even partially, you assume
// all responsibility for its use.
//=========================================================

#include "fixedpoint.h"

//=====================
// Table for sin, tan
//=====================
static const uint32_t TBL_SIN[90] =
{
    0x00000000, 0x0477c2cb, 0x08ef2c65, 0x0d65e3a4, 0x11db8f6d,
    0x164fd6b9, 0x1ac2609b, 0x1f32d44c, 0x23a0d92d, 0x280c16cf,
    0x2c7434fc, 0x30d8dbbb, 0x3539b359, 0x3996646e, 0x3dee97e6,
    0x4241f706, 0x46902b75, 0x4ad8df3e, 0x4f1bbcdd, 0x53586f40,
    0x578ea1d2, 0x5bbe007f, 0x5fe637bc, 0x6406f48a, 0x681fe484,
    0x6c30b5dd, 0x7039176b, 0x7438b8ad, 0x782f49d1, 0x7c1c7bb8,
    0x80000000, 0x83d98908, 0x87a8c9f5, 0x8b6d76bc, 0x8f274421,
    0x92d5e7c4, 0x96791824, 0x9a108ca3, 0x9d9bfd8e, 0xa11b2422,
    0xa48dba92, 0xa7f37c0a, 0xab4c24b7, 0xae9771cc, 0xb1d52187,
    0xb504f334, 0xb826a735, 0xbb39ff06, 0xbe3ebd42, 0xc134a5a5,
    0xc41b7d16, 0xc6f309a9, 0xc9bb12a0, 0xcc736075, 0xcf1bbcdd,
    0xd1b3f2c9, 0xd43bce6d, 0xd6b31d45, 0xd919ae16, 0xdb6f50f4,
    0xddb3d743, 0xdfe713bf, 0xe208da7b, 0xe41900ea, 0xe6175ddb,
    0xe803c981, 0xe9de1d78, 0xeba634c1, 0xed5bebcd, 0xeeff2078,
    0xf08fb213, 0xf20d8160, 0xf378709a, 0xf4d06374, 0xf6153f1b,
    0xf746ea3a, 0xf8654cfc, 0xf970510a, 0xfa67e194, 0xfb4beb4a,
    0xfc1c5c64, 0xfcd924a1, 0xfd82354a, 0xfe17812d, 0xfe98fca8,
    0xff069da1, 0xff605b8c, 0xffa62f69, 0xffd813c6, 0xfff604c0
};
static const uint32_t TBL_TAN[45] =
{
    0x00000000, 0x0477ef65, 0x08f0914b, 0x0d6a98a2, 0x11e6b93a, 
    0x1665a835, 0x1ae81c75, 0x1f6ecf1a, 0x23fa7bf1, 0x288be1f7, 
    0x2d23c3d8, 0x31c2e86f, 0x366a1b59, 0x3b1a2d83, 0x3fd3f5c2, 
    0x4498517a, 0x49682546, 0x4e445dae, 0x532defed, 0x5825dabe, 
    0x5d2d273d, 0x6244e9d5, 0x676e4343, 0x6caa61ae, 0x71fa81d3, 
    0x775ff04e, 0x7cdc0afd, 0x82704286, 0x881e1bfc, 0x8de732b0, 
    0x93cd3a2d, 0x99d20064, 0x9ff77017, 0xa63f9377, 0xacac9715, 
    0xb340cd1d, 0xb9feb0ed, 0xc0e8eb11, 0xc80255b7, 0xcf4e01a3, 
    0xd6cf3bc2, 0xde89935e, 0xe680e123, 0xeeb94f06, 0xf737612a
};

//==============================
// Make Fixed Point from Integer
//==============================
// [Note] Take care Overflow
fix32_t FIX_INT(int32_t ia)
{
    fix32_t fa;
    fa = (fix32_t) (ia << FIXQ);
    return fa;
}

//==============================
// Make Integer from Fixed Point
//==============================
int32_t INT_FIX(fix32_t fa)
{
    int32_t ia;
    ia = ((int32_t) fa) >> FIXQ;
    return ia;
}

//==============================
// Make Fixed Point from Double
//==============================
fix32_t FIX_DBL(double da)
{
    int32_t ia;
    fix32_t fa;

    da = da * (double)(1 << FIXQ);
    ia = (int32_t) da;
    fa = (fix32_t) ia;
    return fa;
}

//=============================
// Make Double from Fixed Point
//=============================
double DBL_FIX(fix32_t fa)
{
    int32_t ia;
    double  da;

    ia = (fix32_t) fa;
    da = (double) ia;
    da = da / ((double) (1 << FIXQ));
    return da;
}

//========================
// Fixed Point Add
//========================
// [Note] Take care Overflow
fix32_t FIX_Add(fix32_t fa, fix32_t fb)
{
    fix32_t fc;
    fc = fa + fb;
    return fc;
}

//========================
// Fixed Point Sub
//========================
// [Note] Take care Overflow
fix32_t FIX_Sub(fix32_t fa, fix32_t fb)
{
    fix32_t fc;
    fc = fa - fb;
    return fc;
}

//=======================
// Fixed Point Absolute
//=======================
fix32_t FIX_Abs(fix32_t fa)
{
    fix32_t fb;
    fb = (fa >= 0)? (+fa) : (-fa);
    return fb;
}

//=======================
// Fixed Point Multiply
//=======================
// [Note] Take care Overflow
fix32_t FIX_Mul(fix32_t fa, fix32_t fb)
{
    int64_t ia64, ib64;
    int64_t iab64;
    int32_t iab32;
    fix32_t fc;

    ia64 = (int64_t) ((int32_t) fa);
    ib64 = (int64_t) ((int32_t) fb);
    iab64 = ia64 * ib64;    
    iab32 = (int32_t) (iab64 >> FIXQ);
    fc = (fix32_t) iab32;
    return fc;
}

//======================
// Fixed Point Divide
//======================
fix32_t FIX_Div(fix32_t fa, fix32_t fb)
{
    int64_t ia64, ib64;
    int64_t iab64;
    int32_t iab32;
    fix32_t fc;

    if (fb == 0) return 0; // div by zero

    ia64 = ((int64_t) ((int32_t) fa)) << FIXQ;
    ib64 = (int64_t) ((int32_t) fb);
    iab64 = ia64 / ib64;
    iab32 = (int32_t) iab64;
    fc = (fix32_t) iab32;
    return fc;
}

//========================
// Integer Square Root
//========================
int64_t INT_Sqrt(int64_t x)
{
    int64_t s, t;

    if (x <= 0) return 0;

    s = 1;
    t = x;
    while (s < t)
    {
        s <<= 1;
        t >>= 1;
    }
    do
    {
        t = s;
        s = (x / s + s) >> 1;
    }
    while (s < t);

    return t;
}

//=========================
// Fixed Point Square Root
//=========================
fix32_t FIX_Sqrt(fix32_t fa)
{
    int64_t ia64;
    int64_t ib64;
    int32_t ib32;
    fix32_t fb;

    ia64 = ((int64_t) ((int32_t) fa)) << FIXQ;
    ib64 = INT_Sqrt(ia64);
    ib32 = (int32_t) ib64;
    fb = (fix32_t) ib32;
    return fb;
}

//===========================
// Fixed Point Sin in Radian
//===========================
fix32_t FIX_Sin(fix32_t frad)
{
    fix32_t fdeg;
    fix32_t fsin;

    fdeg = FIX_Deg_Rad(frad);
    fsin = FIX_Sin_Deg(fdeg);
    return fsin;
}

//===========================
// Fixed Point Cos in Radian
//===========================
fix32_t FIX_Cos(fix32_t frad)
{
    fix32_t fdeg;
    fix32_t fcos;

    fdeg = FIX_Deg_Rad(frad);
    fcos = FIX_Cos_Deg(fdeg);
    return fcos;
}

//============================
// Fixed Point Atan in Radian
//============================
fix32_t FIX_Atan(fix32_t fval)
{
    fix32_t fdeg;
    fix32_t frad;
    
    fdeg = FIX_Atan_Deg(fval);
    frad = FIX_Rad_Deg(fdeg);
    return frad;
}

//============================
// Fixed Point Atan2 in Radian
//============================
fix32_t FIX_Atan2(fix32_t fnum, fix32_t fden)
{
    fix32_t fdeg;
    fix32_t frad;
    
    fdeg = FIX_Atan2_Deg(fnum, fden);
    frad = FIX_Rad_Deg(fdeg);
    return frad;
}

//===============================
// Fixed Point Degree from Radian
//===============================
fix32_t FIX_Deg_Rad(fix32_t frad)
{
    fix32_t fdeg;
    fdeg = FIX_Mul(180 << FIXQ, FIX_Div(frad, FIX_PI));
    return fdeg;
}

//===============================
// Fixed Point Radian from Degree
//===============================
fix32_t FIX_Rad_Deg(fix32_t fdeg)
{
    fix32_t frad;
    frad = FIX_Mul(FIX_PI, FIX_Div(fdeg, (180 << FIXQ)));
    return frad;
}

//===========================
// Fixed Point Sin in Degree
//===========================
fix32_t FIX_Sin_Deg(fix32_t fdeg)
{
    fix32_t  fdeg_0;
    fix32_t  fsign;
    uint32_t quadrant;
    fix32_t  fsin_0;
    fix32_t  fdeg_A, fdeg_B;
    int32_t  ideg_A, ideg_B;
    fix32_t  fsin_A, fsin_B;
    fix32_t  fsin;

    fsign = (fdeg >= 0)? (1 << FIXQ) : (0 - (1 << FIXQ));
    fdeg_0 = FIX_Abs(fdeg);
    //
    quadrant = 0;
    while(fdeg_0 >= (90 << FIXQ))
    {
        fdeg_0 = fdeg_0 - (90 << FIXQ);
        quadrant++;
    }
    quadrant = quadrant & 0x03; // in 0~3 only
    //
    if (quadrant == 0)
    {
        // do nothing
    }
    else if (quadrant == 1)
    {
        fdeg_0 = (90 << FIXQ) - fdeg_0;
    }
    else if (quadrant == 2)
    {
        fsign = -fsign;
    }
    else // if (quadrant == 3)
    {
        fdeg_0 = (90 << FIXQ) - fdeg_0;
        fsign = -fsign;
    }
    //
    if (fdeg_0 >= (90 << FIXQ))
    {
        fsin_0 = (1 << FIXQ);
    }
    else
    {
        fdeg_A = (fdeg_0 >> FIXQ) << FIXQ;
        ideg_A = INT_FIX(fdeg_A);
        fsin_A = TBL_SIN[ideg_A] >> (32 - FIXQ);
        //
        fdeg_B = fdeg_A + (1 << FIXQ);
        ideg_B = INT_FIX(fdeg_B);
        fsin_B = (fdeg_0 < (89 << FIXQ))? (TBL_SIN[ideg_B] >> (32 - FIXQ)) : (1 << FIXQ);
        //
        fsin_0 = fsin_A + FIX_Mul(FIX_Div(fsin_B - fsin_A, fdeg_B - fdeg_A), fdeg_0 - fdeg_A);
    }
    //
    fsin = FIX_Mul(fsin_0, fsign);
    return fsin;
}

//===========================
// Fixed Point Cos in Degree
//===========================
fix32_t FIX_Cos_Deg(fix32_t fdeg)
{
    fix32_t  fcos;

    fcos = FIX_Sin_Deg(fdeg + (90 << FIXQ));
    return fcos;

}

//==============================
// Fixed Point Atan in Degree
//==============================
// return -90~+90
fix32_t FIX_Atan_Deg(fix32_t fval)
{
    fix32_t  fsign;
    fix32_t  fval_0, fval_1;
    uint32_t over_one;
    uint32_t indexA, indexB, indexM;
    fix32_t  ftanM0, ftanM1;
    fix32_t  fdegM0, fdegM1, fdeg;
    

    fsign = (fval >= 0)? (1 << FIXQ) : (0 - (1 << FIXQ));
    fval_0 = FIX_Abs(fval);
    //
    if (fval_0 < (1 << FIXQ))
    {
        fval_1 = fval_0;
        over_one = 0;
    }
    else
    {
        fval_1 = FIX_Div((1 << FIXQ), fval_0);
        over_one = 1;
    }
    //
    // Search
    indexA =  0;
    indexB = 44;
    while(indexA <= indexB)
    {
        indexM = (indexA + indexB) / 2;
        //
        ftanM0 = TBL_TAN[indexM    ] >> (32 - FIXQ);
        ftanM1 = TBL_TAN[indexM + 1] >> (32 - FIXQ);
        if ((ftanM0 <= fval_1) && (fval_1 < ftanM1)) break;
        //
        if (fval_1 < ftanM0)
        {
            indexB = indexM - 1;
        }
        else
        {
            indexA = indexM + 1;
        }
    }    
    //
    // Interpolate
    fdegM0 = FIX_INT(indexM); 
    fdegM1 = FIX_INT(indexM + 1);
    fdeg  = fdegM0 + FIX_Mul(FIX_Div(fdegM1 - fdegM0, ftanM1 - ftanM0), fval_1 - ftanM0);
    //
    if (over_one) fdeg = (90 << FIXQ) - fdeg;
    fdeg = FIX_Mul(fsign, fdeg);
    
    return fdeg;
}

//=============================
// Fixed Point Atan2 in Degree
//=============================
// tan-1(fnum/fden), return -180~+180
fix32_t FIX_Atan2_Deg(fix32_t fnum, fix32_t fden)
{
    uint32_t quadrant;
    fix32_t  fnum_0, fden_0;
    fix32_t  fval_0, fdeg_0;
    fix32_t  fdeg;

    
    quadrant = ((fnum >= 0) && ((fden >= 0)))? 0 :
               ((fnum >= 0) && ((fden <  0)))? 1 :
               ((fnum <  0) && ((fden <  0)))? 2 : 
                                               3;
    //
    if (fden == 0)
    {
        fdeg = (fnum >= 0)? (90 << FIXQ) : -(90 << FIXQ);
    }
    else
    {
        fnum_0 = FIX_Abs(fnum);
        fden_0 = FIX_Abs(fden);
        fval_0 = FIX_Div(fnum_0, fden_0);
        fdeg_0 = FIX_Atan_Deg(fval_0);
        //               
        fdeg = (quadrant == 0)? ( fdeg_0) :
               (quadrant == 1)? ( (180 << FIXQ) - fdeg_0) :
               (quadrant == 2)? (-(180 << FIXQ) + fdeg_0) :
                                (-fdeg_0);
    }
    return fdeg;
}

//=========================================================
// End of Program
//=========================================================
