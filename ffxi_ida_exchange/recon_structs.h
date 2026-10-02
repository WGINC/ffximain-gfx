#pragma pack(push, 1)
struct CYyObject {
  void ** vftable;
};
struct CMoTask {
  CYyObject base;
  float field_4;
  float field_8;
  float field_C;
  unsigned __int8 _pad_10[0x24];
};
struct ActorTechLoadPollingTask {
  CMoTask base;
  unsigned __int8 _pad_34[0x8];
};
struct CAcc {
  CYyObject base;
  unsigned __int8 _pad_4[0xC];
};
struct CAcc2 {
  CYyObject base;
  unsigned __int8 _pad_4[0x18];
};
struct CApp {
  CYyObject base;
  unsigned __int8 _pad_4[0x40];
};
struct CAtelIdleTask {
  CMoTask base;
};
struct CDx {
  CYyObject base;
  unsigned __int8 _pad_4[0xA4C];
};
struct CEnv {
  CYyObject base;
  unsigned __int8 _pad_4[0x20];
};
struct CMoActorColorDriveTask {
  CMoTask base;
  unsigned __int8 _pad_34[0x54];
};
struct CMoActorRotationDriveTask {
  CMoTask base;
  unsigned __int8 _pad_34[0x6C];
};
struct CMoAttachments {
  CYyObject base;
  unsigned __int8 _pad_4[0x2C];
  unsigned __int32 field_30;
  unsigned __int8 _pad_34[0xC];
};
struct CMoAttachments_VT_1032AE94 {
  CMoAttachments base;
};
struct CMoAttachments_VT_1032AF44 {
  CMoAttachments base;
};
struct CMoAttachments_VT_1032AF8C {
  CMoAttachments base;
};
struct CMoAttachments_VT_1032B140 {
  CMoAttachments base;
};
struct CMoAttachments_VT_1032B940 {
  CMoAttachments base;
};
struct CMoAttachments_VT_1032BAF0 {
  CMoAttachments base;
};
struct CMoAttachments_VT_1032BB38 {
  CMoAttachments base;
};
struct CMoAttachments_VT_1032BBAC {
  CMoAttachments base;
};
struct CMoAttachments_VT_1032BBF4 {
  CMoAttachments base;
};
struct CMoAttachments_VT_1032BC68 {
  CMoAttachments base;
};
struct CMoAttachments_VT_1032BC84 {
  CMoAttachments base;
};
struct CMoAttachments_VT_1032BCCC {
  CMoAttachments base;
};
struct CMoAttachments_VT_1032BD14 {
  CMoAttachments base;
};
struct CMoAttachments_VT_1032BD5C {
  CMoAttachments base;
};
struct CMoAttachments_VT_1032BDA4 {
  CMoAttachments base;
};
struct CMoAttachments_VT_1032BDEC {
  CMoAttachments base;
};
struct CMoAttachments_VT_1032BE34 {
  CMoAttachments base;
};
struct CMoAttachments_VT_1032BE7C {
  CMoAttachments base;
};
struct CMoAttachments_VT_1032BEC4 {
  CMoAttachments base;
};
struct CMoAttachments_VT_1032BF0C {
  CMoAttachments base;
};
struct CMoAttachments_VT_1032BF54 {
  CMoAttachments base;
};
struct CMoAttachments_VT_1032BF9C {
  CMoAttachments base;
};
struct CMoAttachments_VT_1032BFE4 {
  CMoAttachments base;
};
struct CMoAttachments_VT_1032C02C {
  CMoAttachments base;
};
struct CMoAttachments_VT_1032C4E8 {
  CMoAttachments base;
};
struct CMoAttachments_VT_1032C628 {
  CMoAttachments_VT_1032B140 base;
};
struct CMoAttachments_VT_1032E7EC {
  CMoAttachments base;
};
struct CYyObject_VT_103299F0 {
  CYyObject base;
};
struct CMoCamera {
  CYyObject_VT_103299F0 base;
  unsigned __int8 _pad_4[0xAC];
};
struct CMoCameraTask {
  CMoTask base;
  unsigned __int8 _pad_34[0x94];
};
struct CMoCelestial {
  CYyObject base;
};
struct CMoOtTask {
  CMoTask base;
  unsigned __int8 _pad_34[0x20];
};
struct CMoElem {
  CMoOtTask base;
  unsigned __int8 _pad_54[0xAC];
  unsigned __int32 field_100;
  unsigned __int8 _pad_104[0x34];
  unsigned __int32 field_138;
  unsigned __int8 _pad_13C[0x58];
};
struct CMoD3aElem {
  CMoElem base;
};
struct CMoD3aLightmapElem {
  CMoElem base;
  unsigned __int32 field_194;
};
struct CMoD3mSpecularElem {
  CMoElem base;
  unsigned __int8 _pad_194[0x2C];
};
struct CYyObject_VT_1032C128 {
  CYyObject base;
};
struct CMoDX {
  CYyObject_VT_1032C128 base;
  unsigned __int8 _pad_4[0x1EC8];
};
struct CMoDisintegrateProgElem {
  CMoElem base;
  unsigned __int8 _pad_194[0x4C];
};
struct CMoDist {
  CMoElem base;
  unsigned __int32 field_194;
};
struct CMoDistModelElem {
  CMoElem base;
  unsigned __int32 field_194;
};
struct CXi471011C8_VT_1032AC90 {
  CMoElem base;
};
struct CMoDistMorphElem {
  CXi471011C8_VT_1032AC90 base;
  unsigned __int8 _pad_194[0x30];
};
struct CMoDistRingElem {
  CMoElem base;
  unsigned __int8 _pad_194[0x22];
  unsigned __int32 field_1B6;
};
struct CMoDxStatusMng {
  CYyObject base;
  unsigned __int8 _pad_4[0xC];
};
struct CMoElem_VT_1032ADE8 {
  CMoElem base;
};
struct CMoGeneratorClone {
  CYyObject base;
  unsigned __int8 _pad_4[0xF8];
};
struct CMoResource {
  CYyObject base;
  unsigned __int32 field_4;
  unsigned __int8 _pad_8[0xC];
  unsigned __int32 field_14;
  unsigned __int8 _pad_18[0x18];
};
struct CMoGeneratorClone_VT_1032B114 {
  CMoResource base;
};
struct CMoGeneratorClone_VT_1032B69C {
  CMoGeneratorClone base;
};
struct CMoLfdElem {
  CMoElem base;
  unsigned __int8 _pad_194[0xC];
};
struct CMoLockColorDriveTask {
  CMoTask base;
  unsigned __int8 _pad_34[0x48];
};
struct CMoLockLookAtDriveTask {
  CMoTask base;
  unsigned __int8 _pad_34[0x4C];
};
struct CMoNullProgElem {
  CMoElem base;
};
struct CMoOT {
  CYyObject base;
  unsigned __int8 _pad_4[0x14];
};
struct CMoOcclusionMng {
  CYyObject base;
  unsigned __int8 _pad_4[0x18];
};
struct CMoOtTask_VT_1032AE64 {
  CMoOtTask base;
};
struct CMoPathDriveActorTask {
  CMoTask base;
  unsigned __int8 _pad_34[0x7C];
};
struct CMoPathObject {
  CYyObject base;
  unsigned __int8 _pad_4[0x78];
};
struct CMoPointLightProgElem {
  CMoElem base;
  unsigned __int8 _pad_194[0x20];
};
struct CMoProcessor_recon {
  CYyObject base;
  unsigned __int8 _pad_4[0xC];
};
struct CMoProcessor_VT_1032C1C8 {
  CMoProcessor_recon base;
};
struct CMoProcessor_VT_1032C298 {
  CMoProcessor_recon base;
};
struct CMoProcessor_VT_1032C368 {
  CMoProcessor_recon base;
};
struct CMoProcessor_VT_1032C434 {
  CMoProcessor_recon base;
};
struct CMoResourceMng {
  CYyObject base;
  unsigned __int8 _pad_4[0x4000C];
};
struct CMoResource_VT_103366C4 {
  CMoResource base;
};
struct CMoResource_VT_1032C55C {
  CMoResource_VT_103366C4 base;
  unsigned __int8 _pad_30[0x8];
};
struct CMoResource_VT_1032C588 {
  CMoResource_VT_103366C4 base;
};
struct CMoResource_VT_1032C5FC {
  CMoResource_VT_103366C4 base;
};
struct CMoResource_VT_1032C644 {
  CMoResource_VT_103366C4 base;
};
struct CMoResource_VT_1032C670 {
  CMoResource_VT_103366C4 base;
};
struct CMoResource_VT_1032C69C {
  CMoResource_VT_103366C4 base;
};
struct CMoResource_VT_1032C6C8 {
  CMoResource_VT_103366C4 base;
};
struct CMoResource_VT_1032C6F4 {
  CMoResource_VT_103366C4 base;
};
struct CMoResource_VT_1032C720 {
  CMoResource_VT_103366C4 base;
};
struct CMoResource_VT_1032C74C {
  CMoResource_VT_103366C4 base;
};
struct CMoResource_VT_1032C778 {
  CMoResource_VT_103366C4 base;
};
struct CMoResource_VT_1032C7A4 {
  CMoResource_VT_103366C4 base;
};
struct CMoResource_VT_1032C7D0 {
  CMoResource_VT_103366C4 base;
};
struct CMoResource_VT_1032C7FC {
  CMoResource_VT_103366C4 base;
};
struct CMoResource_VT_1032C828 {
  CMoResource_VT_103366C4 base;
};
struct CMoResource_VT_1032C854 {
  CMoResource_VT_103366C4 base;
  unsigned __int8 _pad_30[0x28];
};
struct CMoResource_VT_1032C880 {
  CMoResource_VT_103366C4 base;
};
struct CMoResource_VT_1032C8AC {
  CMoResource_VT_103366C4 base;
};
struct CMoResource_VT_1032C934 {
  CMoResource base;
};
struct CMoResource_VT_10336698 {
  CMoResource base;
};
struct CMoResource_VT_103366F0 {
  CMoResource base;
};
struct CMoResource_VT_103367E8 {
  CMoResource base;
};
struct CMoSchedularTask {
  CMoTask base;
  unsigned __int8 _pad_34[0x10C];
  unsigned __int8 field_140;
  unsigned __int8 field_141;
  unsigned __int8 _pad_142[0x1];
  unsigned __int8 field_143;
  unsigned __int8 field_144;
  unsigned __int8 field_145;
  unsigned __int32 field_146;
};
struct CMoSkeletonElem {
  CMoElem base;
  unsigned __int8 _pad_194[0x45];
};
struct CMoSphRes {
  CMoResource_VT_103366C4 base;
  unsigned __int8 _pad_30[0x20];
};
struct CMoSpline {
  CYyObject base;
  unsigned __int8 _pad_4[0x8];
  unsigned __int32 field_C;
  unsigned __int32 field_10;
  unsigned __int32 field_14;
  unsigned __int32 field_18;
  unsigned __int8 _pad_1C[0x4];
  unsigned __int32 field_20;
  unsigned __int8 _pad_24[0x4];
};
struct CMoTaskMng {
  CYyObject base;
  unsigned __int8 _pad_4[0xD60];
};
struct CMoTask_VT_10329C70 {
  CMoTask base;
};
struct CMoTask_VT_1032A904 {
  CMoTask base;
};
struct CMoTask_VT_1032A9BC {
  CMoTask base;
};
struct CMoTask_VT_1032AF60 {
  CMoTask base;
};
struct CMoTask_VT_1032AFA8 {
  CMoTask base;
};
struct CMoTask_VT_1032B654 {
  CMoTask base;
};
struct CMoTask_VT_1032B890 {
  CMoTask base;
};
struct CMoTask_VT_1032B8BC {
  CMoTask base;
};
struct CMoTask_VT_1032B8E8 {
  CMoTask base;
};
struct CMoTask_VT_1032B914 {
  CMoTask base;
};
struct CMoTask_VT_1032B95C {
  CMoTask base;
};
struct CMoTask_VT_1032BB0C {
  CMoTask base;
};
struct CMoTask_VT_1032BB54 {
  CMoTask base;
};
struct CMoTask_VT_1032BB80 {
  CMoTask base;
};
struct CMoTask_VT_1032BBC8 {
  CMoTask base;
};
struct CMoTask_VT_1032BC10 {
  CMoTask base;
};
struct CMoTask_VT_1032BC3C {
  CMoTask base;
};
struct CMoTask_VT_1032BCA0 {
  CMoTask base;
};
struct CMoTask_VT_1032BCE8 {
  CMoTask_VT_1032BCA0 base;
};
struct CMoTask_VT_1032BD30 {
  CMoTask_VT_1032BCA0 base;
};
struct CMoTask_VT_1032BD78 {
  CMoTask_VT_1032BCA0 base;
};
struct CMoTask_VT_1032BDC0 {
  CMoTask_VT_1032BCA0 base;
};
struct CMoTask_VT_1032BE08 {
  CMoTask base;
};
struct CMoTask_VT_1032BE50 {
  CMoTask base;
};
struct CMoTask_VT_1032BE98 {
  CMoTask base;
};
struct CMoTask_VT_1032BEE0 {
  CMoTask base;
};
struct CMoTask_VT_1032BF28 {
  CMoTask base;
};
struct CMoTask_VT_1032BF70 {
  CMoTask base;
};
struct CMoTask_VT_1032BFB8 {
  CMoTask base;
};
struct CMoTask_VT_1032C000 {
  CMoTask base;
};
struct CMoTask_VT_1032C048 {
  CMoTask base;
};
struct CMoTask_VT_1032C490 {
  CMoTask base;
};
struct CMoTask_VT_1032C4BC {
  CMoTask_VT_1032C490 base;
};
struct CMoTask_VT_1032C908 {
  CMoTask base;
};
struct CMoTask_VT_1032D6D4 {
  CMoTask base;
};
struct CMoTask_VT_1032E808 {
  CMoTask base;
};
struct CMoVuAnmElem {
  CMoElem base;
  unsigned __int8 _pad_194[0x9D];
};
struct CMoVuDist {
  CMoElem base;
  unsigned __int8 _pad_194[0x4C];
};
struct CMoVuLineProgElem {
  CMoElem base;
  unsigned __int8 _pad_194[0x54];
};
struct CShader {
  CYyObject base;
  unsigned __int32 field_4;
  unsigned __int32 field_8;
  unsigned __int32 field_C;
  unsigned __int32 field_10;
};
struct CTsZoneMap {
  CMoTask base;
  unsigned __int8 _pad_34[0x10];
  unsigned __int32 field_44;
  unsigned __int8 _pad_48[0x3956C];
};
struct CXi471011C8_VT_1032AD80 {
  CMoElem base;
};
struct CXi471011C8_VT_1032B3B8 {
  CMoElem base;
  unsigned __int8 _pad_194[0x4];
  unsigned __int32 field_198;
};
struct CXi471011C8_VT_1032B478 {
  CMoElem base;
};
struct CXiActor {
  CMoTask base;
  unsigned __int32 field_34;
  float field_38;
  unsigned __int32 field_3C;
  unsigned __int32 field_40;
  unsigned __int8 _pad_44[0x3C];
  unsigned __int32 field_80;
  unsigned __int32 field_84;
  unsigned __int32 field_88;
  unsigned __int32 field_8C;
  unsigned __int8 _pad_90[0x4];
  float field_94;
  float field_98;
  unsigned __int32 field_9C;
  unsigned __int8 _pad_A0[0x4];
  unsigned __int32 field_A4;
  unsigned __int32 field_A8;
  unsigned __int8 _pad_AC[0x6];
  unsigned __int8 field_B2;
  unsigned __int8 _pad_B3[0x1];
  unsigned __int32 field_B4;
  unsigned __int8 _pad_B8[0x4];
  unsigned __int32 field_BC;
  unsigned __int8 _pad_C0[0x4];
};
struct CXiActorDraw {
  CMoTask base;
};
struct CXiActorNameDraw {
  CMoTask base;
};
struct CXiAtelActor {
  CXiActor base;
  unsigned __int8 _pad_C4[0x10];
};
struct CXiControlActor {
  CXiActor base;
  unsigned __int8 _pad_C4[0x10];
  unsigned __int32 field_D4;
  unsigned __int32 field_D8;
  unsigned __int32 field_DC;
  unsigned __int32 field_E0;
  unsigned __int8 _pad_E4[0x15];
  unsigned __int8 field_F9;
  unsigned __int8 field_FA;
  unsigned __int8 field_FB;
  unsigned __int32 field_FC;
  unsigned __int8 _pad_100[0x3];
  unsigned __int8 field_103;
  unsigned __int8 field_104;
  unsigned __int8 _pad_105[0x3];
  unsigned __int32 field_108;
  unsigned __int32 field_10C;
  unsigned __int32 field_110;
  unsigned __int8 _pad_114[0x5C];
  unsigned __int32 field_170;
  unsigned __int32 field_174;
  unsigned __int32 field_178;
  unsigned __int8 field_17C;
  unsigned __int8 _pad_17D[0x447];
};
struct CXiCollisionActor {
  CXiControlActor base;
  unsigned __int8 _pad_5C4[0x1C];
  unsigned __int8 field_5E0;
  unsigned __int8 _pad_5E1[0x1];
  unsigned __int8 field_5E2;
  unsigned __int8 _pad_5E3[0x1];
  unsigned __int32 field_5E4;
  unsigned __int8 _pad_5E8[0x4];
  unsigned __int8 field_5EC;
  unsigned __int8 _pad_5ED[0xB];
};
struct CXiSkeletonActor_recon {
  CXiCollisionActor base;
  unsigned __int8 _pad_5F8[0x68];
  unsigned __int32 field_660;
  unsigned __int8 _pad_664[0x4];
  unsigned __int32 field_668;
  unsigned __int32 field_66C;
  unsigned __int8 _pad_670[0xC0];
  unsigned __int32 field_730;
  unsigned __int32 field_734;
  unsigned __int8 _pad_738[0x4];
  unsigned __int32 field_73C;
  unsigned __int32 field_740;
  unsigned __int32 field_744;
  unsigned __int32 field_748;
  unsigned __int32 field_74C;
  unsigned __int32 field_750;
  unsigned __int8 _pad_754[0x48];
  unsigned __int8 field_79C;
  unsigned __int8 _pad_79D[0x9B];
  unsigned __int32 field_838;
  unsigned __int8 _pad_83C[0x4];
  unsigned __int32 field_840;
  unsigned __int8 _pad_844[0x20];
  unsigned __int32 field_864;
  unsigned __int8 _pad_868[0x3D];
  unsigned __int8 field_8A5;
  unsigned __int8 field_8A6;
  unsigned __int8 field_8A7;
  unsigned __int8 field_8A8;
  unsigned __int8 field_8A9;
  unsigned __int8 field_8AA;
  unsigned __int8 field_8AB;
  unsigned __int8 field_8AC;
  unsigned __int8 _pad_8AD[0x14B];
  unsigned __int8 field_9F8;
  unsigned __int8 _pad_9F9[0xB];
  unsigned __int8 field_A04;
  unsigned __int8 _pad_A05[0x7];
};
struct CXiDancerActor {
  CXiSkeletonActor_recon base;
  unsigned __int8 _pad_A0C[0x2AC];
};
struct CXiDollActor {
  CXiActor base;
  unsigned __int8 _pad_C4[0x370];
  float field_434;
  float field_438;
  float field_43C;
};
struct CXiGeomActor {
  CXiDancerActor base;
};
struct CXiMannequinActor {
  CXiDancerActor base;
};
struct CXiMovie {
  CYyObject base;
  unsigned __int8 _pad_4[0x24];
};
struct CXiOpening {
  CYyObject base;
  unsigned __int8 _pad_4[0x124];
};
struct CXiSkeletonActorRes_recon {
  CMoTask base;
  unsigned __int32 field_34;
  unsigned __int8 _pad_38[0x10];
};
struct CXiSklFurnitureActor {
  CXiDancerActor base;
  unsigned __int8 _pad_CB8[0x4];
};
struct CXiTimer {
  CYyObject base;
  unsigned __int32 field_4;
  unsigned __int32 field_8;
};
struct CXiTimerHigh {
  CXiTimer base;
  unsigned __int8 _pad_C[0x4];
  __int64 field_10;
  unsigned __int8 _pad_18[0x28];
  __int64 field_40;
  unsigned __int8 _pad_48[0x28];
};
struct CXiTimerLow {
  CXiTimer base;
  unsigned __int8 _pad_C[0x14];
};
struct CYyAfterImage {
  CMoTask base;
  unsigned __int8 _pad_34[0x151C];
};
struct CYyQue {
  CYyObject base;
  unsigned __int32 field_4;
  unsigned __int8 _pad_8[0x1C];
};
struct CYyBlendQue {
  CYyQue base;
  unsigned __int8 _pad_24[0x8];
  float field_2C;
  unsigned __int8 _pad_30[0xC];
  unsigned __int32 field_3C;
  unsigned __int8 _pad_40[0x20];
  unsigned __int32 field_60;
  unsigned __int8 _pad_64[0x1C];
};
struct CYyCamMng2 {
  CYyObject base;
  unsigned __int8 _pad_4[0x114];
};
struct CYyMcb {
  CYyObject base;
  unsigned __int8 _pad_4[0x1C];
};
struct CYyDataMcb {
  CYyMcb base;
};
struct CYyDelayDeleteTask {
  void ** vftable;
  unsigned __int8 _pad_4[0x8];
};
struct CYyElemMcb {
  CYyMcb base;
};
struct CYyElemObject {
  CYyElemMcb base;
};
struct CYyElemMemory {
  CYyElemObject base;
};
struct CYyExMcb {
  CYyMcb base;
};
struct CYyExObject {
  CYyExMcb base;
};
struct CYyExMemory {
  CYyExObject base;
};
struct CYyIcon {
  CYyObject base;
  unsigned __int8 _pad_4[0x78];
};
struct CYyLoadMcb {
  CYyMcb base;
};
struct CYyLoadObject {
  CYyLoadMcb base;
};
struct CYyLoadMemory {
  CYyLoadObject base;
};
struct CYyLowerObject {
  CYyDataMcb base;
};
struct CYyLowerMemory {
  CYyLowerObject base;
};
struct CYyMenuMcb {
  CYyMcb base;
};
struct CYyMenuObject {
  CYyMenuMcb base;
};
struct CYyMenuMemory {
  CYyMenuObject base;
};
struct CYyModel {
  CYyObject base;
  unsigned __int8 _pad_4[0xB8];
};
struct CYyModelBase {
  CYyObject base;
  unsigned __int8 _pad_4[0xC4];
};
struct CYyModelDt {
  CYyObject base;
  unsigned __int8 _pad_4[0x38];
};
struct CYyModelDt_VT_1032A430 {
  CYyModelDt base;
};
struct CYyModelDt_VT_1032A72C {
  CYyModelDt base;
};
struct CYyMotionQue {
  CYyQue base;
  float field_24;
  unsigned __int32 field_28;
  unsigned __int8 _pad_2C[0x4];
  unsigned __int32 field_30;
  unsigned __int32 field_34;
  unsigned __int8 _pad_38[0x4];
  unsigned __int32 field_3C;
};
struct CYyMotionQueList {
  CYyObject base;
  unsigned __int32 field_4;
  unsigned __int32 field_8;
  unsigned __int8 _pad_C[0x8];
};
struct CYyMoveBlendQue {
  CYyBlendQue base;
};
struct CYyMusicLoadTask {
  CMoTask base;
  unsigned __int8 _pad_34[0x28];
};
struct CYyNode {
  CYyObject base;
  unsigned __int32 field_4;
  unsigned __int32 field_8;
};
struct CYyObject_VT_1032A9F8 {
  CYyObject base;
  unsigned __int8 field_4;
  unsigned __int8 field_5;
  unsigned __int8 field_6;
  unsigned __int8 _pad_7[0x1];
  unsigned __int32 field_8;
  unsigned __int32 field_C;
};
struct CYyObject_VT_1032AFD8 {
  CYyObject base;
};
struct CYyObject_VT_1032D69C {
  CYyObject base;
};
struct CYyObject_VT_1033666C {
  CYyObject base;
};
struct CYyObject_VT_103367B8 {
  CYyObject base;
};
struct CYyObject_VT_10336868 {
  CYyObject base;
};
struct CYyOs2VtxBuffer {
  CMoTask base;
  unsigned __int8 _pad_34[0x6A];
};
struct CYyOsmTask {
  CYyObject base;
  unsigned __int8 _pad_4[0x10];
};
struct CYyOsmTaskXform {
  CYyOsmTask base;
  unsigned __int8 _pad_14[0x4044];
};
struct CYyResfList {
  CYyNode base;
  unsigned __int8 _pad_C[0x4];
  unsigned __int32 field_10;
  unsigned __int8 _pad_14[0x4];
  unsigned __int32 field_18;
  unsigned __int32 field_1C;
  unsigned __int8 field_20;
  unsigned __int8 _pad_21[0x3];
};
struct CYySepRes {
  CMoResource_VT_103366C4 base;
  unsigned __int8 _pad_30[0x10];
};
struct CYySkl {
  CYyObject base;
  unsigned __int8 _pad_4[0x14];
};
struct CYySoundElem {
  CMoElem base;
  unsigned __int8 _pad_194[0x59];
};
struct CYyTexBase {
  CYyObject base;
  unsigned __int8 _pad_4[0x30];
};
struct CYyTex {
  CYyTexBase base;
  unsigned __int8 _pad_34[0x14];
};
struct CYyTexMng {
  CYyObject base;
  unsigned __int16 field_4;
  unsigned __int8 _pad_6[0x2];
  unsigned __int32 field_8;
  unsigned __int32 field_C;
  unsigned __int8 field_10;
  unsigned __int8 _pad_11[0x3];
};
struct CYyUpperObject {
  CYyDataMcb base;
};
struct CYyUpperMemory {
  CYyUpperObject base;
};
struct CYyVb {
  CYyObject base;
  unsigned __int8 _pad_4[0x1C];
};
struct CYyVbMng {
  CYyObject base;
  unsigned __int8 _pad_4[0x8];
};
struct CYyWorkObject {
  CYyElemMcb base;
};
struct CYyWorkMemory {
  CYyWorkObject base;
};
struct XiActorTimerTask {
  CMoTask base;
  unsigned __int8 _pad_34[0x48];
};
struct XiCapture {
  CYyObject base;
  unsigned __int32 field_4;
  unsigned __int32 field_8;
  unsigned __int8 _pad_C[0xC];
};
struct XiSkeletonActor2 {
  CXiSkeletonActor_recon base;
  unsigned __int8 field_A0C;
  unsigned __int8 _pad_A0D[0x27];
};
struct XiChocoboActor2 {
  XiSkeletonActor2 base;
};
struct XiDoorActor {
  CXiControlActor base;
  unsigned __int8 _pad_5C4[0x2D0];
};
struct XiModelActor {
  CXiControlActor base;
  unsigned __int8 _pad_5C4[0x1B4];
  unsigned __int8 field_778;
  unsigned __int8 _pad_779[0x1B];
};
struct XiFurnitureActor {
  XiModelActor base;
};
struct XiLiftActor {
  CXiAtelActor base;
  unsigned __int8 _pad_D4[0x52C];
};
struct XiPng {
  CYyObject base;
  unsigned __int8 _pad_4[0x208];
};
struct XiZone {
  CYyObject_VT_103367B8 base;
  unsigned __int8 _pad_4[0x1D8];
};
struct YkAlbum {
  CYyObject base;
  unsigned __int8 _pad_4[0x324];
};
struct YkBitmap {
  CYyObject base;
  unsigned __int8 _pad_4[0xC];
};
struct YkDiary {
  CYyObject base;
  unsigned __int8 _pad_4[0x3F0];
};
struct YkFurniture {
  CYyObject base;
  unsigned __int8 _pad_4[0xF0];
};
struct YkJpeg {
  CYyObject base;
  unsigned __int8 _pad_4[0xCF8];
};
struct YkJpgBit {
  CYyObject base;
  unsigned __int8 _pad_4[0x10];
};
struct YkMyroom {
  CYyObject_VT_1032D69C base;
  unsigned __int8 _pad_4[0x858];
};
struct YkSoldier {
  CYyObject base;
  unsigned __int8 _pad_4[0x324];
};
struct YkZip {
  CYyObject base;
  unsigned __int8 _pad_4[0x4];
  unsigned __int32 field_8;
  unsigned __int8 _pad_C[0xC];
};
struct YmCombineWeather {
  CYyObject base;
  unsigned __int8 _pad_4[0x42];
};
#pragma pack(pop)
