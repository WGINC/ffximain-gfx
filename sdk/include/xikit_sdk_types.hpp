// xikit_sdk_types.hpp -- the function-pointer type a hook, its previous, and a call through a game entry have:
// xikit::fn<decltype(&Class::Method)> is R (__thiscall *)(Class *, A...) for a member function (MSVC6 and GCC
// i686 both pass `this` in ECX), and decltype(&Function) itself for a free or static function (its calling
// convention is part of the type).
#pragma once
namespace xikit {
template <class T> struct fn_of { typedef T type; };
template <class R, class C, class... A> struct fn_of<R (C::*)(A...)> { typedef R (__attribute__((thiscall)) *type)(C *, A...); };
template <class R, class C, class... A> struct fn_of<R (C::*)(A...) const> { typedef R (__attribute__((thiscall)) *type)(const C *, A...); };
template <class R, class C, class... A> struct fn_of<R (C::*)(A..., ...)> { typedef R (__attribute__((cdecl)) *type)(C *, A..., ...); };
template <class T> using fn = typename fn_of<T>::type;
}
