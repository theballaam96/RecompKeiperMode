#include "modding.h"
#include "ultra64.h"
#include "enums.h"
#include "common_structs.h"

typedef struct {
    u8  loaded;
    u8  unk1; // used
    u8  unk2;
    u8  unk3;
    u8  unk4;
    u8  visible; // 0x5 -- if 0x02 visible, else invisible
    u8  unk6;
    u8  unk7;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    void *unk14;
    s32 unk18;
    void *unk1C; // TODO: Same struct as unk14?
    s32 unk20;
    s32 unk24;
    u8  pad8[0x4C - 0x28];
    void *unk4C;
    s32 unk50;
    u8  pad54[0x60 - 0x54];
    s32 unk60[1]; // TODO: How many?
    s32 unk64;
    s32 deload1; // 0x68
    s32 deload2; // 0x6C
    s32 deload3; // 0x70
    s32 deload4; // 0x74
    void *unk78; // First in array?
    void *unk7C; // Last in array?
    s16 unk80; // Used
    s16 unk82; // Used
    s16 unk84; // Used
    s16 unk86; // Used
    u8  pad2[0x1C8 - 0x88]; // total size 0x1C8
} Chunk;

void func_global_asm_80659670(f32 arg0, f32 arg1, f32 arg2, s16 arg3); // set chunk lighting
extern s32 D_global_asm_807F6C28; // Chunk count
extern u32 global_properties_bitfield;
extern Chunk *chunk_array_pointer;

typedef struct {
    Actor* unk0;
    s32 unk4;
} GlobalASMStruct53;

extern u16 D_global_asm_807FBB34;
extern GlobalASMStruct53 D_global_asm_807FB930[];
extern f32 *D_global_asm_8076A0B8;
extern f32 *D_global_asm_8076A0BC;
extern f32 *D_global_asm_8076A0B4;
extern f32 *D_global_asm_8076A0B8;
extern f32 *D_global_asm_8076A0BC;

void setDayNightLighting(void) {
    s32 j;
    u16 k;
    f32 *lzcontrolleraad;

    if ((global_properties_bitfield & 0x10) == 0) return;
    for (j = 0; j < D_global_asm_807F6C28; j++) {
        // Turn off both green/blue channels
        D_global_asm_8076A0B8[j] = 0.0f;
        D_global_asm_8076A0BC[j] = 0.0f;
        chunk_array_pointer[j].unk3 = 1;
    }
    for (k = 0; k < D_global_asm_807FBB34; k++) {
        if (D_global_asm_807FB930[k].unk0) {
            if (D_global_asm_807FB930[k].unk0->unk58 == ACTOR_LOADING_ZONE_CONTROLLER) {
                lzcontrolleraad = D_global_asm_807FB930[k].unk0->AAD_as_array[0];
                lzcontrolleraad[4] = 0.0f;
                lzcontrolleraad[5] = 0.0f;
            }
        }
    }
}

RECOMP_PATCH void func_global_asm_80659670(f32 arg0, f32 arg1, f32 arg2, s16 arg3) {
    u8 var_v1;
    s32 iChunk;

    if (D_global_asm_807F6C28 <= 0) {
        var_v1 = 1;
    } else {
        var_v1 = D_global_asm_807F6C28;
    }
    arg1 = 0.0f;
    arg2 = 0.0f;
    if (arg3 != -1) {
        if (arg3 < var_v1) {
            D_global_asm_8076A0B4[arg3] = arg0 <= 1.0f ? arg0 : 1.0f;
            D_global_asm_8076A0B8[arg3] = arg1 <= 1.0f ? arg1 : 1.0f;
            D_global_asm_8076A0BC[arg3] = arg2 <= 1.0f ? arg2 : 1.0f;
        }
    } else {
        for (iChunk = 0; iChunk < var_v1; iChunk++) {
            D_global_asm_8076A0B4[iChunk] = arg0 <= 1.0f ? arg0 : 1.0f;
            D_global_asm_8076A0B8[iChunk] = arg1 <= 1.0f ? arg1 : 1.0f;
            D_global_asm_8076A0BC[iChunk] = arg2 <= 1.0f ? arg2 : 1.0f;
        }
    }
}

RECOMP_CALLBACK("*", dk64recomp_every_frame) void displayRedWrapper(void) {
    setDayNightLighting();
}