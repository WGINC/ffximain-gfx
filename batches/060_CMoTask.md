# Batch 060_CMoTask: 4 functions of `CMoTask`

Name these functions: see `batches/README.md`. Write `data/names/060_CMoTask.json`.
2 of them were sampled in a running frame (data/frame_set.json).

## `0x10011920` CMoTask::vf007

- **Gate:** none
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoTask_VT_1032C000` slot 7 (+0x1C), `CMoTask_VT_1032BDC0` slot 7 (+0x1C), `CMoTask_VT_1032A904` slot 7 (+0x1C), `CXiAtelActor` slot 7 (+0x1C), `CXiSklFurnitureActor` slot 7 (+0x1C), `CMoTask_VT_1032BC3C` slot 7 (+0x1C), `CMoD3mSpecularElem` slot 7 (+0x1C), `CXiActorNameDraw` slot 7 (+0x1C) and 86 more

```cpp
void CMoTask::vf007()
{
  ;
}
```

## `0x10011930` CMoTask::vf009

- **Gate:** none; **frame set:** self
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoTask_VT_1032C000` slot 9 (+0x24), `CMoTask_VT_1032BDC0` slot 9 (+0x24), `CMoTask_VT_1032A904` slot 9 (+0x24), `CXiAtelActor` slot 9 (+0x24), `CXiSklFurnitureActor` slot 9 (+0x24), `CMoTask_VT_1032BC3C` slot 9 (+0x24), `CMoD3mSpecularElem` slot 9 (+0x24), `CXiActorNameDraw` slot 9 (+0x24) and 84 more

```cpp
void CMoTask::vf009()
{
  ;
}
```

## `0x10011940` CMoTask::vf010

- **Gate:** none
- **Facts:** `ret 4`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoTask_VT_1032C000` slot 10 (+0x28), `CMoTask_VT_1032BDC0` slot 10 (+0x28), `CMoTask_VT_1032A904` slot 10 (+0x28), `CXiAtelActor` slot 10 (+0x28), `CXiSklFurnitureActor` slot 10 (+0x28), `CMoTask_VT_1032BC3C` slot 10 (+0x28), `CMoD3mSpecularElem` slot 10 (+0x28), `CXiActorNameDraw` slot 10 (+0x28) and 85 more

```cpp
void CMoTask::vf010(int a1)
{
  ;
}
```

## `0x10042640` CMoTask::vf008

- **Gate:** none; **frame set:** self
- **Facts:** `ret 0`; sets only AL of EAX; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoTask` slot 8 (+0x20), `CMoTask_VT_1032B654` slot 8 (+0x20), `CMoOtTask` slot 8 (+0x20), `CMoTask_VT_1032C908` slot 8 (+0x20)

```cpp
char CMoTask::vf008()
{
  return 0;
}
```
