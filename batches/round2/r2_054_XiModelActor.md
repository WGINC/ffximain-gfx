# Batch r2_054_XiModelActor: 2 functions of `XiModelActor`

Name these functions: see `batches/README.md`. Write `data/names/r2_054_XiModelActor.json`.
2 of them were sampled in a running frame (data/frame_set.json).

## `0x100C3FB0` fn_100C3FB0

- **Gate:** none; **frame set:** self
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `XiFurnitureActor` slot 111 (+0x1BC), `XiModelActor` slot 111 (+0x1BC)
- **Named before** (a naming session's reading, low): `XiModelActor_GetPtr734`; name it again from this dossier, or leave it out to keep that name

```cpp
XI_API int __thiscall XiModelActor_GetPtr734(void * self_)
{
  return (int)self_ + 0x734;
}
```

## `0x100C3FC0` fn_100C3FC0

- **Gate:** none; **frame set:** self
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `XiFurnitureActor` slot 112 (+0x1C0), `XiModelActor` slot 112 (+0x1C0)
- **Named before** (a naming session's reading, low): `XiModelActor_GetPtr744`; name it again from this dossier, or leave it out to keep that name

```cpp
XI_API int __thiscall XiModelActor_GetPtr744(void * self_)
{
  return (int)self_ + 0x744;
}
```
