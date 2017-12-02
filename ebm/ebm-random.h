#ifndef  __EBM_random_h__
#define  __EBM_random_h__
/*_____________________________________________________________________________
  |                                                                           |__
  | Objetive: Define Random Functions for ANSI C99 and C++98.                 | |
  |  Versión: 1.0 Crossover Platform.                                         | |
  |  License: Copyright 2010-2012. Arequipa, Perú.                            | |
  |   Author: Edgard Bernardo  Medina Quispe, <edgardmedina@yahoo.com>        | |
  |___________________________________________________________________________| |
    |___________________________________________________________________________|*/

/*********************************************************************************************
 * Initialize Seed for Random functions
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * void Randomize( [size_t  p_nSeed] );
 *
 *
 * Random Template function
 * ~~~~~~~~~~~~~~~~~~~~~~~~
 * <TNumber> Random( <TNumber> );                                                              // Random between 0 .. RAND_MAX_{TYPE}
 * <TNumber> Random( <TNumber>, <TNumber> p_nTop);                                             // Random between 0 .. (p_nTop-1),  p_nTop <= RAND_MAX_{TYPE}
 * <TNumber> Random( <TNumber>, <TNumber> p_nMin, <TNumber> p_nMax);                           // Random between 0 .. p_nMax,      p_nMax <= RAND_MAX_{TYPE}
 * <TNumber> Random( <TNumber>, <TNumber> p_nMin, <TNumber> p_nMax, <TNumber> p_nMultiple_Of ) // Random between 0 .. p_nMax,  p_nMax <= RAND_MAX_{TYPE}
 *
 * <TNumber> RANDMAX( <TNumber> );
 *  
 *  Where:
 *  
 *     <TNumber> ::= <Int_Type> 
 *                 | <Float_Type>
 *  
 *     <Int_Type> ::= ( int8_t   | char   | byte )              //                        -128 .. ~~~~~~~~~~~~~~~~~~~~~~~ 127
 *                  | ( int16_t  | short  )                     //                     -32 768 .. ~~~~~~~~~~~~~~~~~~~~ 32'767
 *                  | ( int32_t  | int    | long )              //              -2 147 483 648 .. ~~~~~~~~~~~~~ 2'147'483'647
 *                  | ( int64_t  | llong  )                     // -9 223 372 036 854 775 8080 .. ~ 9 223 372 036 854 775 807 LL
 *                  | ( uint8_t  | uchar  | ubyte  | Byte )     //                           0 .. ~~~~~~~~~~~~~~~~~~~~~~~ 255 u
 *                  | ( uint16_t | ushort | )                   //                           0 .. ~~~~~~~~~~~~~~~~~~~~ 65'535 u
 *                  | ( uint32_t | uint   | ulong)              //                           0 .. ~~~~~~~~~~~~~ 4'294'967'295 u
 *                  | ( uint64_t | ullong |                     //                           0 ..  18'446'744'073'709'551'615 lllu
 * 
 *   <Float_Type> ::= float                                     // 0.0f  .. 1.0f
 *                  | double                                    // 0.0   .. 1.0
 *                  | ldouble                                   // 0.0lf .. 1.0lf
 *
 * Random type functions
 * ~~~~~~~~~~~~~~~~~~~~~
 *
 *  --------------------------------+--------------------------------
 *  |   Signed Functions            |  Unsigned Functions           |
 *  |-------------------------------+-------------------------------|
 *  |    char  random_char()        |    uchar  random_uchar()      |
 *  |    byte  random_byte()        |    ubyte  random_ubyte()      |
 *  |   short  random_short()       |   ushort  random_ushort()     |
 *  |     int  random_int()         |     uint  random_uint()       |
 *  |    long  random_long()        |    ulong  random_ulong()      |
 *  |   llong  random_llong()       |   ullong  random_ullong()     |
 *  |                               |                               |
 *  |   float  random_float()       |                               |
 *  |  double  random_double()      |                               |
 *  | ldouble  random_ldouble()     |                               |
 *  |-------------------------------+-------------------------------|
 *
 * Constants defined:
 * ~~~~~~~~~~~~~~~~~
 *
 *   (references)                       INT8_MIN  =                       (-128)        = 0x0000 0000 0000 0080
 *   (references)                       INT16_MIN =                    (-32'768)        = 0x0000 0000 0000 8000
 *   (references)                       INT32_MIN =             (-2'147'483'647 - 1)    = 0x0000 0000 7FFF FFFE
 *   (references)                       INT64_MIN = (-9'223'372'036'854'775'807 LL - 1) = 0x7FFF FFFF FFFF FFFEll
 *
 *   RAND_MAX_CHAR   = RAND_MAX_INT8   = INT8_MAX  =                       127    = 0x0000 0000 0000 007F
 *   RAND_MAX_BYTE   = RAND_MAX_INT8   = INT8_MAX  =                       127    = 0x0000 0000 0000 007F
 *   RAND_MAX_SHORT  = RAND_MAX_INT16  = RAND_MAX  =                    32'767    = 0x0000 0000 0000 7FFF
 *   RAND_MAX_INT    = RAND_MAX_INT32  = INT32_MAX =             2'147'483'647    = 0x0000 0000 7FFF FFFF
 *   RAND_MAX_LLONG  = RAND_MAX_INT64  = INT64_MAX = 9'223'372'036'854'775'807 LL = 0x7FFF FFFF FFFF FFFF
 *
 *   RAND_MAX_UCHAR  = RAND_MAX_UINT8  = UINT8_MAX  = RAND_MAX_BYTE  = BYTE_MAX  =                        255u   = 0x0000 0000 0000 00FF
 *   RAND_MAX_UBYTE  = RAND_MAX_UINT8  = UINT8_MAX  = RAND_MAX_BYTE  = BYTE_MAX  =                        255u   = 0x0000 0000 0000 00FF
 *   RAND_MAX_USHORT = RAND_MAX_UINT16 = UINT16_MAX = RAND_MAX_WORD  = WORD_MAX  =                     65'535u   = 0x0000 0000 0000 FFFF
 *   RAND_MAX_UINT   = RAND_MAX_UINT32 = UINT32_MAX = RAND_MAX_DWORD = DWORD_MAX =              4'294'967'295u   = 0x0000 0000 FFFF FFFF
 *   RAND_MAX_ULLONG = RAND_MAX_UINT64 = UINT64_MAX = RAND_MAX_QWORD = QWORD_MAX = 18'446'744'073'709'551'615ull = 0xFFFF FFFF FFFF FFFFull
 *
 *
 * Template Procedure for testing Random Template Function:
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *  void Random_Test(size_t p_nItems, <TNumber> );
 *  void Random_Test(size_t p_nItems, <TNumber>,  <TNumber> p_nTop );
 *  void Random_Test(size_t p_nItems, <TNumber>,  <TNumber> p_nMin,  <TNumber> p_nMax );
 *  void Random_Test(size_t p_nItems, <TNumber>,  <TNumber> p_nMin,  <TNumber> p_nMax,  <TNumber> p_nMultiple_Of );
 *
 *******************************************************************************************/


#ifdef __cplusplus
    extern "C" {
#endif

#define RAND_MAX_INT8   INT8_MAX
#define RAND_MAX_INT16  RAND_MAX
#define RAND_MAX_INT32  INT32_MAX
#define RAND_MAX_INT64  INT64_MAX

#define RAND_MAX_UINT8  UINT8_MAX
#define RAND_MAX_UINT16 UINT16_MAX
#define RAND_MAX_UINT32 UINT32_MAX
#define RAND_MAX_UINT64 UINT64_MAX

#define RAND_MAX_CHAR   RAND_MAX_INT8
#define RAND_MAX_SHORT  RAND_MAX_INT16
#define RAND_MAX_INT    RAND_MAX_INT32
#define RAND_MAX_LLONG  RAND_MAX_INT64

#define RAND_MAX_UCHAR  RAND_MAX_UINT8
#define RAND_MAX_USHORT RAND_MAX_UINT16
#define RAND_MAX_UINT   RAND_MAX_UINT32
#define RAND_MAX_ULLONG RAND_MAX_UINT64

#define RAND_MAX_BYTE   BYTE_MAX
#define RAND_MAX_WORD   WORD_MAX
#define RAND_MAX_DWORD  DWORD_MAX
#define RAND_MAX_QWORD  QWORD_MAX

#define RANDMAX(_type) (_type)csname2(__RANDMAX_, _type )
#define __RANDMAX_char   RAND_MAX_INT8
#define __RANDMAX_byte   RAND_MAX_INT8
#define __RANDMAX_short  RAND_MAX_INT16
#define __RANDMAX_int    RAND_MAX_INT32
#define __RANDMAX_long   RAND_MAX_INT32
#define __RANDMAX_llong  RAND_MAX_INT64

#define __RANDMAX_uchar  RAND_MAX_UINT8
#define __RANDMAX_ubyte  RAND_MAX_UINT8
#define __RANDMAX_ushort RAND_MAX_UINT16
#define __RANDMAX_uint   RAND_MAX_UINT32
#define __RANDMAX_ulong  RAND_MAX_UINT32
#define __RANDMAX_ullong RAND_MAX_UINT64

// -------------------------------------------------------------

#define _rand_3bits(_Type)   ( (_Type)rand() And (_Type)MASK_3BITS )
#define _rand_4bits(_Type)   ( (_Type)rand() And (_Type)MASK_4BITS )
#define _rand_7bits(_Type)   ( (_Type)rand() And (_Type)MASK_7BITS  )
#define _rand_8bits(_Type)   ( (_Type)rand() And (_Type)MASK_8BITS  )
#define _rand_12bits(_Type)  ( (_Type)rand() And (_Type)MASK_12BITS  )
#define _rand_15bits(_Type)  ( (_Type)rand() )
#define _rand_16bits(_Type)  ( ((_Type)rand()<<1) Or ( (_Type)rand() And (_Type)MASK_1BITS ) )
#define _rand_24bits(_Type)  (_rand_16bits(_Type) Or (_rand_8bits(_Type)<<16))
#define _rand_31bits(_Type)  (_rand_16bits(_Type) Or (_rand_15bits(_Type)<<16))
#define _rand_32bits(_Type)  (_rand_16bits(_Type) Or (_rand_16bits(_Type)<<16))

#define _rand_40bits(_Type)  (_rand_32bits(_Type) Or (_rand_8bits(_Type) <<32))
#define _rand_48bits(_Type)  (_rand_32bits(_Type) Or (_rand_16bits(_Type)<<32))
#define _rand_56bits(_Type)  (_rand_48bits(_Type) Or (_rand_8bits(_Type) <<48))
#define _rand_63bits(_Type)  (_rand_48bits(_Type) Or (_rand_15bits(_Type)<<48))
#define _rand_64bits(_Type)  (_rand_48bits(_Type) Or (_rand_16bits(_Type)<<48))


// -------------------------------------------------------------
#define  random_byte()  _rand_7bits(int8_t)
#define  random_ubyte() _rand_8bits(uint8_t)

inline int16_t random_short() {
    int16_t _nRnd;
    switch( _rand_3bits(int) ) {
        case 0: return _rand_4bits( int16_t);
        case 1: return _rand_8bits( int16_t);
        case 2: return _rand_12bits(int16_t);
        case 3: return _rand_15bits(int16_t);
        case 4: return _rand_4bits( int16_t);
        case 5: return _rand_8bits( int16_t); 
        case 6: return _rand_12bits(int16_t);
    }
    return _rand_15bits(int16_t);
}

inline uint16_t random_ushort() {
    uint16_t _nRnd;
    switch( _rand_3bits(int) ) {
        case 0: return _rand_4bits(  uint16_t); 
        case 1: return _rand_8bits(  uint16_t); 
        case 2: return _rand_15bits( uint16_t); 
        case 3: return _rand_16bits( uint16_t); 
        case 4: return _rand_8bits(  uint16_t); 
        case 5: return _rand_12bits( uint16_t); 
        case 6: return _rand_15bits( uint16_t); 
    }
    return _rand_16bits(uint16_t);
}

inline int32_t  random_int() {
    int32_t _nRnd;
    switch( _rand_3bits(int) ) {
        case 0:  return _rand_8bits( int32_t); 
        case 1:  return _rand_16bits(int32_t); 
        case 2:  return _rand_24bits(int32_t); 
        case 3:  return _rand_31bits(int32_t); 
        case 4:  return _rand_8bits( int32_t); 
        case 5:  return _rand_16bits(int32_t); 
        case 6:  return _rand_24bits(int32_t); 
    }
    return _rand_31bits(int32_t);
}

inline uint32_t random_uint() {
    uint32_t _nRnd;
    switch( _rand_3bits(int) ) {
        case 0:  return _rand_8bits( uint32_t); 
        case 1:  return _rand_16bits(uint32_t); 
        case 2:  return _rand_24bits(uint32_t); 
        case 3:  return _rand_32bits(uint32_t); 
        case 4:  return _rand_8bits( uint32_t); 
        case 5:  return _rand_16bits(uint32_t); 
        case 6:  return _rand_24bits(uint32_t); 
    }
    return _rand_32bits(uint32_t);
}

inline int64_t random_llong() {
    int64_t _nRnd;
    switch( _rand_3bits(int) ) {
        case 0:  return _rand_8bits( int64_t); 
        case 1:  return _rand_16bits(int64_t); 
        case 2:  return _rand_24bits(int64_t); 
        case 3:  return _rand_32bits(int64_t); 
        case 4:  return _rand_40bits(int64_t); 
        case 5:  return _rand_48bits(int64_t); 
        case 6:  return _rand_56bits(int64_t); 
    }
    return _rand_63bits(int64_t);
}

inline  uint64_t random_ullong() {
    uint64_t _nRnd;
    switch( _rand_3bits(int) ) {
        case 0: return _rand_8bits( uint64_t); 
        case 1: return _rand_16bits(uint64_t); 
        case 2: return _rand_24bits(uint64_t); 
        case 3: return _rand_32bits(uint64_t); 
        case 4: return _rand_40bits(uint64_t); 
        case 5: return _rand_48bits(uint64_t); 
        case 6: return _rand_56bits(uint64_t); 
    }
    return _rand_64bits(uint64_t);
}

inline float   random_float()   { return   (float)random_uint()   /  (float)RAND_MAX_UINT32; }
inline double  random_double()  { return  (double)random_ullong() / (double)RAND_MAX_UINT64; }
inline ldouble random_ldouble() { return (ldouble)random_ullong() /(ldouble)RAND_MAX_UINT64; }

// -------------------------------------------------------
#define Random( _Type, ...) csname3( Random_, _Type, Count_Args(__VA_ARGS__) )( __VA_ARGS__)

#define Random_char         Random_byte
#define Random_uchar        Random_ubyte
#define Random_long   (long)Random_int
#define Random_ulong (ulong)Random_uint

// -------------------------------------------------------------
#define       Random_char0  Random_byte0
#define       Random_char1  Random_byte1
#define       Random_char2  Random_byte2
#define       Random_char3  Random_byte3


#define       Random_byte0()  random_byte()
inline int8_t Random_byte1( int8_t p_nTop ) { return Random_byte0() % p_nTop ; }
inline int8_t Random_byte2( int8_t p_nButtom, int8_t p_nTop ) {
        Swapif(p_nButtom > p_nTop, p_nButtom, p_nTop );
        return p_nButtom + Random_byte1(p_nTop - p_nButtom + 1);
}
inline int8_t Random_byte3( int8_t p_nButtom, int8_t p_nTop, int8_t _step  ) {
        int8_t _nRnd = Random_byte2( p_nButtom, p_nTop ),
               _nRem = _nRnd % _step;
        _nRnd -= _nRem;
        if( _nRnd < p_nButtom ) _nRnd += _step ;
        return _nRnd;
}

// -------------------------------------------------------------
/* This function use a formule that don`t work, see Random_ubyte2(...) function */
// inline int8_t Random_byte1( int8_t p_nTop ) { return  random_byte() / ((int8_t)RAND_MAX_INT8 / p_nTop + 1); }

// -------------------------------------------------------------
/* This formula in this function sucks, don`t work with 'char' nither 'uchar', max value obtain is much less to desired.
//  // inline uint8_t Random_ubyte1( uint8_t p_nTop ) { return  random_ubyte() / ((uint8_t)RAND_MAX_UINT8 / p_nTop+1); }
    for(uint8_t nTop=2; i<=RAND_MAX_UINT8; ++i)
        printf(" %d/(%d/%d+1)=%d   =%d\n", 255, RAND_MAX_UINT8, nTop,
            (uint8_t)255/(uint8_t)(RAND_MAX_UINT8/nTop+1), nTop-((uint8_t)255/(uint8_t)(RAND_MAX_UINT8/nTop+1)) );
*/

#define       Random_uchar0  Random_ubyte0
#define       Random_uchar1  Random_ubyte1
#define       Random_uchar2  Random_ubyte2
#define       Random_uchar3  Random_ubyte3

#define        Random_ubyte0()   random_ubyte()
inline uint8_t Random_ubyte1( uint8_t p_nTop ) { return Random_ubyte0() % p_nTop ; }
inline uint8_t Random_ubyte2( uint8_t p_nButtom, uint8_t p_nTop ) {
        Swapif(p_nButtom > p_nTop, p_nButtom, p_nTop );
        return p_nButtom + Random_ubyte1( p_nTop - p_nButtom + 1);
}
inline uint8_t  Random_ubyte3( uint8_t p_nButtom, uint8_t p_nTop, uint8_t _step  ) {
        uint8_t _nRnd = Random_ubyte2( p_nButtom, p_nTop ),
               _nRem = _nRnd % _step;
        _nRnd -= _nRem;
        if( _nRnd < p_nButtom ) _nRnd += _step ;
        return _nRnd;
}

//---------------------------------------------------------------------------
#define        Random_short0()  random_short()
inline int16_t Random_short1( int16_t p_nTop ) { return Random_short0() % p_nTop ; }
inline int16_t Random_short2( int16_t p_nButtom, int16_t p_nTop ) {
        Swapif(p_nButtom > p_nTop, p_nButtom, p_nTop );
        return p_nButtom + Random_short1(p_nTop - p_nButtom + 1);
}
inline int16_t Random_short3( int16_t p_nButtom, int16_t p_nTop, int16_t _step  ) {
        int16_t _nRnd = Random_short2( p_nButtom, p_nTop ),
            _nRem = _nRnd % _step;
        _nRnd -= _nRem;
        if( _nRnd < p_nButtom ) _nRnd += _step ;
        return _nRnd;
}
//  // This formula sucks:
//  // inline int16_t random_short1( int16_t p_nTop ) { return random_short() / ((int16_t)RAND_MAX_INT16 / p_nTop + 1); }
//---------------------------------------------------------------------------
#define         Random_ushort0()  random_ushort()
inline uint16_t Random_ushort1( uint16_t p_nTop ) { return Random_ushort0() % p_nTop ; }
inline uint16_t Random_ushort2( uint16_t p_nButtom, uint16_t p_nTop ) {
        Swapif(p_nButtom > p_nTop, p_nButtom, p_nTop );
        return p_nButtom + Random_ushort1(p_nTop - p_nButtom + 1);
}
inline uint16_t  Random_ushort3( uint16_t p_nButtom, uint16_t p_nTop, uint16_t _step  ) {
        uint16_t _nRnd = Random_ushort2( p_nButtom, p_nTop ),
                 _nRem = _nRnd % _step;
        _nRnd -= _nRem;
        if( _nRnd < p_nButtom ) _nRnd += _step ;
        return _nRnd;
}

//---------------------------------------------------------------------------
#define       Random_long0  Random_int0
#define       Random_long1  Random_int1
#define       Random_long2  Random_int2
#define       Random_long3  Random_int3

#define        Random_int0()  random_int()
inline int32_t Random_int1( int32_t p_nTop ) { return Random_int0() % p_nTop; }
inline int32_t Random_int2( int32_t p_nButtom, int32_t p_nTop ) {
        Swapif(p_nButtom > p_nTop, p_nButtom, p_nTop );
        return p_nButtom + Random_int1(p_nTop - p_nButtom + 1);
}
inline int32_t  Random_int3( int32_t p_nButtom, int32_t p_nTop, int32_t _step  ) {
        int32_t _nRnd = Random_int2( p_nButtom, p_nTop ),
            _nRem = _nRnd % _step;
        _nRnd -= _nRem;
        if( _nRnd < p_nButtom ) _nRnd += _step ;
        return _nRnd;
}
// /// inline int32_t Random_int1( int32_t p_nTop ) { return random_int() / ((int32_t)RAND_MAX_INT32 / p_nTop + 1); }

//---------------------------------------------------------------------------
#define       Random_ulong0  Random_uint0
#define       Random_ulong1  Random_uint1
#define       Random_ulong2  Random_uint2
#define       Random_ulong3  Random_uint3

#define         Random_uint0()  random_uint()
inline uint32_t Random_uint1( uint32_t p_nTop ) { return Random_uint0() % p_nTop; }
inline uint32_t Random_uint2( uint32_t p_nButtom, uint32_t p_nTop ) {
    Swapif(p_nButtom > p_nTop, p_nButtom, p_nTop );
    return p_nButtom + Random_uint1(p_nTop - p_nButtom + 1);
}
inline uint32_t  Random_uint3( uint32_t p_nButtom, uint32_t p_nTop, uint32_t _step  ) {
        uint32_t _nRnd = Random_uint2( p_nButtom, p_nTop ),
                 _nRem = _nRnd % _step;
        _nRnd -= _nRem;
        if( _nRnd < p_nButtom ) _nRnd += _step ;
        return _nRnd;
}
// // inline uint32_t Random_uint1( uint32_t p_nTop ) { return random_uint() / ((uint32_t)RAND_MAX_UINT32 / p_nTop + 1); }

//---------------------------------------------------------------------------
#define        Random_llong0()  random_llong()
inline int64_t Random_llong1( int64_t p_nTop ) { return Random_llong0() % p_nTop; }
inline int64_t Random_llong2( int64_t p_nButtom, int64_t p_nTop ) {
    Swapif(p_nButtom > p_nTop, p_nButtom, p_nTop );
    return p_nButtom + Random_llong1(p_nTop - p_nButtom + 1);
}
inline int64_t Random_llong3( int64_t p_nButtom, int64_t p_nTop, int64_t _step  ) {
        int64_t _nRnd = Random_llong2( p_nButtom, p_nTop ),
            _nRem = _nRnd % _step;
        _nRnd -= _nRem;
        if( _nRnd < p_nButtom ) _nRnd += _step ;
        return _nRnd;
}
//  // inline int64_t Random_llong1( int64_t p_nTop ) { return random_llong() / ((int64_t)RAND_MAX_INT64 / p_nTop + 1); }

//---------------------------------------------------------------------------
#define         Random_ullong0()  random_ullong()
inline uint64_t Random_ullong1( uint64_t p_nTop ) { return Random_ullong0() % p_nTop ; }
inline uint64_t Random_ullong2( uint64_t p_nButtom, uint64_t p_nTop ) {
    Swapif(p_nButtom > p_nTop, p_nButtom, p_nTop );
    return p_nButtom + Random_ullong1(p_nTop - p_nButtom + 1);
}
inline uint64_t Random_ullong3( uint64_t p_nButtom, uint64_t p_nTop, uint64_t _step  ) {
    uint64_t _nRnd = Random_ullong2( p_nButtom, p_nTop ),
             _nRem = _nRnd % _step;
    _nRnd -= _nRem;
    if( _nRnd < p_nButtom ) _nRnd += _step ;
    return _nRnd;
}
// // inline uint64_t Random_ullong1( uint64_t p_nTop ) { return random_ullong() / ((uint64_t)RAND_MAX_UINT64 / p_nTop + 1); }

inline float   random_float()  { return  (float)random_uint()   /  (float)RAND_MAX_UINT32; }
inline double  random_double() { return (double)random_ullong() / (double)RAND_MAX_UINT64; }
inline ldouble random_ldouble()   { return   (ldouble)random_ullong() /   (ldouble)RAND_MAX_UINT64; }
//---------------------------------------------------------------------------
#define      Random_float0()  random_float()
inline float Random_float1( float p_nTop ) { return (float)random_int() / ((float)RAND_MAX_INT32/p_nTop); }
inline float Random_float2( float p_nButtom, float p_nTop ) {
    Swapif(p_nButtom > p_nTop, p_nButtom, p_nTop );
    return p_nButtom + Random_float1( p_nTop - p_nButtom );
}
inline float __Random_float_3(float p_nButtom, float p_nTop, float _step ) {
    float _nRnd = Random_float2( p_nButtom, p_nTop ),
          _nRem = mod(_nRnd,  _step);
    _nRnd -= _nRem;
    if( _nRnd < p_nButtom ) _nRnd += _step - _nRem ;
    return _nRnd;
}
//---------------------------------------------------------------------------
#define       Random_double0()  random_double()
inline double Random_double1( double p_nTop ) { return (double)random_llong() / ((double)RAND_MAX_INT64/p_nTop); }
inline double Random_double2( double p_nButtom, double p_nTop ) {
    Swapif(p_nButtom > p_nTop, p_nButtom, p_nTop );
    return p_nButtom + Random_double1( p_nTop - p_nButtom );
}
inline double Random_double3(double p_nButtom, double p_nTop, double _step ) {
    double _nRnd = Random_double2( p_nButtom, p_nTop ),
           _nRem = mod( _nRnd,  _step);
    _nRnd -= _nRem;
    if( _nRnd < p_nButtom ) _nRnd += _step - _nRem ;
    return _nRnd;
}
//---------------------------------------------------------------------------
// #define Random_ldouble(...) __Random_ldouble_DO(Count_Args(__VA_ARGS__), ##__VA_ARGS__)
#define     Random_ldouble0()  random_ldouble()
inline ldouble Random_ldouble1( ldouble p_nTop ) { return (ldouble)random_llong() / ((ldouble)RAND_MAX_INT64/p_nTop); }
inline ldouble Random_ldouble2( ldouble p_nButtom, ldouble p_nTop ) {
    Swapif(p_nButtom > p_nTop, p_nButtom, p_nTop );
    return p_nButtom +  Random_ldouble1( p_nTop - p_nButtom );
}
inline ldouble Random_ldouble3(ldouble p_nButtom, ldouble p_nTop, ldouble _step ) {
    ldouble _nRnd = Random_ldouble2( p_nButtom, p_nTop ),
           _nRem = mod(_nRnd,  _step);
    _nRnd -= _nRem;
    if( _nRnd < p_nButtom ) _nRnd += _step - _nRem ;
    return _nRnd;
}
//---------------------------------------------------------------------------

#define Random_Test(_nItems, _Type_, ... ) \
    Procedure \
        _Type_ aArgs[]={__VA_ARGS__};\
        int    nArgs=sizeofarray(aArgs);\
        printf("\n\n************************["); show(i, sizeof( _Type_), "" ); \
        printf(" ]************************\n"); \
        _Type_ *aRand= New( _Type_, _nItems);\
        Foreach( iter, inArrayN, aRand, _nItems, 0,ALL) Do  *iter=Random(_Type_ ,  ##__VA_ARGS__ ); EndForeach\
        printf("  ****< Random Distribution: "); show_proc( Random(_Type_,   ##__VA_ARGS__  ), ">****\n" );\
        Foreach( iter, inArrayN, aRand, _nItems, 0,ALL) Do  printf(" " FMT(_Type_), *iter);   EndForeach\
        printf("\n\n  ***< Random Sorted >***   ");\
        show_proc( QuickSort( _Type_, aRand, _nItems ), nl);\
        Foreach( iter, inArrayN, aRand, _nItems, 0,ALL) Do  printf(" " FMT(_Type_), *iter);   EndForeach/**/\
        Delete(aRand,0);\
    EndProcedure


#ifdef __cplusplus
    }
#endif

#endif
