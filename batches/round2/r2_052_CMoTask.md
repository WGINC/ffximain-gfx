# Batch r2_052_CMoTask: 2 functions of `CMoTask`

Name these functions: see `batches/README.md`. Write `data/names/r2_052_CMoTask.json`.
2 of them were sampled in a running frame (data/frame_set.json).

## `0x10011930` CMoTask::vf009

- **Gate:** none; **frame set:** self
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoTask_VT_1032C000` slot 9 (+0x24), `CMoTask_VT_1032BDC0` slot 9 (+0x24), `CMoTask_VT_1032A904` slot 9 (+0x24), `CXiAtelActor` slot 9 (+0x24), `CXiSklFurnitureActor` slot 9 (+0x24), `CMoTask_VT_1032BC3C` slot 9 (+0x24), `CMoD3mSpecularElem` slot 9 (+0x24), `CXiActorNameDraw` slot 9 (+0x24) and 84 more
- **Named before** (a naming session's reading, low): `CMoTask_NoOp9`; name it again from this dossier, or leave it out to keep that name

```cpp
void CMoTask::vf009()
{
  ;
}
```

## `0x10042640` CMoTask::vf008

- **Gate:** none; **frame set:** self
- **Facts:** `ret 0`; sets only AL of EAX; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoTask` slot 8 (+0x20), `CMoTask_VT_1032B654` slot 8 (+0x20), `CMoOtTask` slot 8 (+0x20), `CMoTask_VT_1032C908` slot 8 (+0x20)
- **Named before** (a naming session's reading, low): `CMoTask_ReturnFalse8`; name it again from this dossier, or leave it out to keep that name

```cpp
char CMoTask::vf008()
{
  return 0;
}
```
