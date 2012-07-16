#include <stdio.h>
#include <stdlib.h>

#define MAKEFOURCC(ch0, ch1, ch2, ch3)  \ 
    ((unsigned int)(unsigned char)(ch0) | ((unsigned int)(unsigned char)(ch1) << 8) |  \ 
    ((unsigned int)(unsigned char)(ch2) << 16) | ((unsigned int)(unsigned char)(ch3) << 24 )); 

#define FOURCC_MHDR         MAKEFOURCC('M','H','D','R')
#define FOURCC_MVTX         MAKEFOURCC('M','V','T','X')
#define FOURCC_MTXM         MAKEFOURCC('M','T','X','M')
#define FOURCC_MEND         MAKEFOURCC('M','E','N','D')

#pragma pack(1)
/* structure of floating point xyz coordinates */
typedef struct{
    float x;
    float y;
    float z;
}SU3_VTX3F;

typedef struct{
    float u;
    float v;
}SU3_TEX2F;

typedef struct{
unsigned int chunkLength;
unsigned int chunkType;
}SU3_CHUNK;

typedef struct{
unsigned int faceCount;
unsigned int modelHints;
unsigned char textureFormat;
unsigned char vertexFormat;
}SU3_MHDR;

//header sequence including 'SU3'
static unsigned char fileHeader[] = {0x89, 0x53, 0x55, 0x33, 0x0D, 0x0A, 0x1A, 0x0A};

static SU3_VTX3F FlexVerts[] = {
 {-10.000000f, 0.000000f, 10.000000f},  //triangle 1 x
 {-10.000000f, 0.000000f, -10.000000f},  //triangle 1 y
 {10.000000f, 0.000000f, -10.000000f}, //triangle 1 z
 {10.000000f, 0.000000f, -10.000000f}, //triangle 2 x
 {10.000000f, 0.000000f, 10.000000f},  //triangle 2 y
 {-10.000000f, 0.000000f, 10.000000f},  //triangle 2 z
 {-10.000000f, 20.000000f, 10.000000f},  //triangle 3 x
 {10.000000f, 20.000000f, 10.000000f}, //triangle 3 y
 {10.000000f, 20.000000f, -10.000000f}, //triangle 3 z
 {10.000000f, 20.000000f, -10.000000f}, //triangle 4 x
 {-10.000000f, 20.000000f, -10.000000f},  //triangle 4 y
 {-10.000000f, 20.000000f, 10.000000f},  //triangle 4 z
 {-10.000000f, 0.000000f, 10.000000f}, //triangle 5 x
 {10.000000f, 0.000000f, 10.000000f},  //triangle 5 y
 {10.000000f, 20.000000f, 10.000000f},  //triangle 5 z
 {10.000000f, 20.000000f, 10.000000f}, //triangle 6 x
 {-10.000000f, 20.000000f, 10.000000f},  //triangle 6 y
 {-10.000000f, 0.000000f, 10.000000f},  //triangle 6 z
 {10.000000f, 0.000000f, 10.000000f},  //triangle 7 x
 {10.000000f, 0.000000f, -10.000000f},  //triangle 7 y
 {10.000000f, 20.000000f, -10.000000f}, //triangle 7 z
 {10.000000f, 20.000000f, -10.000000f}, //triangle 8 x
 {10.000000f, 20.000000f, 10.000000f},  //triangle 8 y
 {10.000000f, 0.000000f, 10.000000f},  //triangle 8 z
 {10.000000f, 0.000000f, -10.000000f},  //triangle 9 x
 {-10.000000f, 0.000000f, -10.000000f}, //triangle 9 y
 {-10.000000f, 20.000000f, -10.000000f}, //triangle 9 z
 {-10.000000f, 20.000000f, -10.000000f}, //triangle 10 x
 {10.000000f, 20.000000f, -10.000000f},  //triangle 10 y
 {10.000000f, 0.000000f, -10.000000f},  //triangle 10 z
 {-10.000000f, 0.000000f, -10.000000f}, //triangle 11 x
 {-10.000000f, 0.000000f, 10.000000f},  //triangle 11 y
 {-10.000000f, 20.000000f, 10.000000f},  //triangle 11 z
 {-10.000000f, 20.000000f, 10.000000f}, //triangle 12 x
 {-10.000000f, 20.000000f, -10.000000f},  //triangle 12 y
 {-10.000000f, 0.000000f, -10.000000f},  //triangle 12 z
};

static SU3_TEX2F FlexTexMap[36] = {
 {0.001200f, 0.665600f}, //triangle 1
 {0.001200f, 0.335200f}, //triangle 1
 {0.331900f, 0.335200f}, //triangle 1
 {0.331900f, 0.335200f}, //triangle 2
 {0.331900f, 0.665600f}, //triangle 2
 {0.001200f, 0.665600f}, //triangle 2
 {0.664100f, 0.333200f}, //triangle 3
 {0.333700f, 0.333200f}, //triangle 3
 {0.333700f, 0.000800f}, //triangle 3
 {0.333700f, 0.000800f}, //triangle 4
 {0.664100f, 0.000800f}, //triangle 4
 {0.664100f, 0.333200f}, //triangle 4
 {0.999300f, 0.333400f}, //triangle 5
 {0.665300f, 0.333400f}, //triangle 5
 {0.665300f, 0.000800f}, //triangle 5
 {0.665300f, 0.000800f}, //triangle 6
 {0.999300f, 0.000800f}, //triangle 6
 {0.999300f, 0.333400f}, //triangle 6
 {1.000000f, 0.666000f}, //triangle 7
 {0.665600f, 0.666000f}, //triangle 7
 {0.665600f, 0.334200f}, //triangle 7
 {0.665600f, 0.334200f}, //triangle 8
 {1.000000f, 0.334200f}, //triangle 8
 {1.000000f, 0.666000f}, //triangle 8
 {0.663000f, 0.665300f}, //triangle 9
 {0.334800f, 0.665300f}, //triangle 9
 {0.334800f, 0.333800f}, //triangle 9
 {0.334800f, 0.333800f}, //triangle 10
 {0.663000f, 0.333800f}, //triangle 10
 {0.663000f, 0.665300f}, //triangle 10
 {0.331900f, 0.332100f}, //triangle 11
 {0.000400f, 0.332100f}, //triangle 11
 {0.000400f, 0.002000f}, //triangle 11
 {0.000400f, 0.002000f}, //triangle 12
 {0.331900f, 0.002000f}, //triangle 12
 {0.331900f, 0.332100f}, //triangle 12
};

#pragma pack()

int main(int argc, char *argv[])
{
    char *ptrVerts;
    unsigned int crc;
    SU3_CHUNK chk;
    SU3_MHDR mHDR;

    FILE *fp = NULL;

    fp = fopen("c:\\file.SU3", "wb");
    
    fwrite(fileHeader, sizeof(fileHeader), 1, fp);

    // build and write the model header chunk
    chk.chunkLength = sizeof(mHDR);
    chk.chunkType = FOURCC_MHDR;
    fwrite(&chk, sizeof(SU3_CHUNK), 1, fp);
    mHDR.faceCount = 12;
    mHDR.modelHints = 0x01;
    mHDR.textureFormat = 0x00;
    mHDR.vertexFormat = 0x01;
    fwrite(&mHDR, sizeof(SU3_MHDR), 1, fp);
    crc = 0x12345678; //bogus placeholder value
    fwrite(&crc, sizeof(crc), 1, fp);
    
    //build and write the model vertex chunk
    chk.chunkLength = sizeof(FlexVerts);
    chk.chunkType = FOURCC_MVTX;
    fwrite(&chk, sizeof(SU3_CHUNK), 1, fp);
    fwrite(&FlexVerts, sizeof(FlexVerts), 1, fp);
    crc = 0x12345678; //bogus placeholder value
    fwrite(&crc, sizeof(crc), 1, fp);

    //build and write the texture map chunk
    chk.chunkLength = sizeof(FlexTexMap);
    chk.chunkType = FOURCC_MTXM;
    fwrite(&chk, sizeof(SU3_CHUNK), 1, fp);
    fwrite(&FlexTexMap, sizeof(FlexTexMap), 1, fp);
    crc = 0x12345678; //bogus placeholder value
    fwrite(&crc, sizeof(crc), 1, fp);

    //build and write the model end chunk
    chk.chunkLength = 0;
    chk.chunkType = FOURCC_MEND;
    fwrite(&chk, sizeof(SU3_CHUNK), 1, fp);
    crc = 0x12345678; //bogus placeholder value
    fwrite(&crc, sizeof(crc), 1, fp);
    
    system("PAUSE");
    return EXIT_SUCCESS;
}

