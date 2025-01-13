/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOficial/nodepp/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef NODEPP_TYPE
#define NODEPP_TYPE

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace type {

    struct false_type { static constexpr bool value = false; using type = false_type; };
    struct true_type  { static constexpr bool value = true;  using type = true_type;  };
    
    /*─······································································─*/

    template <bool B, typename T, typename F> struct conditional { using type = T; };
    template <typename T, typename F> struct conditional<false, T, F> { using type = F; };
    
    /*─······································································─*/

    template <bool B, typename T = void> struct enable_if {};
    template <typename T> struct enable_if<true, T> { using type = T; };
    
    /*─······································································─*/

    template <typename T> struct is_array : false_type {};
    template <typename T> struct is_array<T[]> : true_type {};
    template <typename T, ulong N> struct is_array<T[N]> : true_type {};
    
    /*─······································································─*/

    template <typename T, typename U> struct is_same : false_type {};
    template <typename T> struct is_same<T, T> : true_type {};
    
    /*─······································································─*/

    template <typename T, typename U> struct is_different : true_type {};
    template <typename T> struct is_different<T, T> : false_type {};
    
    /*─······································································─*/

    template <typename T> struct is_pointer : false_type {};
    template <typename T> struct is_pointer<T*> : true_type {};
        
    /*─······································································─*/

    template <typename T> struct is_void : false_type {};

    template <> struct is_void<const volatile void> : true_type {};

    template <> struct is_void<volatile void> : true_type {};

    template <> struct is_void<const void> : true_type {};

    template <> struct is_void<void> : true_type {};
    
    /*─······································································─*/

    template <typename T, T v> struct integral_constant {
        static constexpr T value = v;

        using value_type = T;
        using type = integral_constant<T, v>;

        constexpr operator value_type()   const noexcept { return value; }
        constexpr value_type operator()() const noexcept { return value; }
    };
    
    /*─······································································─*/

    template <typename T> struct is_character : false_type {};

    template <> struct is_character<char> : true_type {};

    template <> struct is_character<uchar> : true_type {};
    
    /*─······································································─*/

    template <typename T> struct is_integral : false_type {};

    template <> struct is_integral<int> : true_type {};

    template <> struct is_integral<bool> : true_type {};

    template <> struct is_integral<char> : true_type {};

    template <> struct is_integral<long> : true_type {};

    template <> struct is_integral<uint> : true_type {};

    template <> struct is_integral<short> : true_type {};

    template <> struct is_integral<llong> : true_type {};

    template <> struct is_integral<ulong> : true_type {};

    template <> struct is_integral<uchar> : true_type {};

    template <> struct is_integral<ushort> : true_type {};

    template <> struct is_integral<ullong> : true_type {};

    template <> struct is_integral<wchar_t> : true_type {};

    template <> struct is_integral<char16_t> : true_type {};

    template <> struct is_integral<char32_t> : true_type {};

    /*─······································································─*/

    template <typename T> struct is_floating_point : false_type {};

    template <> struct is_floating_point<float> : true_type {};

    template <> struct is_floating_point<double> : true_type {};

    template <> struct is_floating_point<ldouble> : true_type {};
    
    /*─······································································─*/

    template <typename T> struct is_cr : false_type {};

    template <typename T> struct is_cr<const T&> : true_type {};

    template <typename T> struct is_cr<const T&&> : true_type {};
    
    /*─······································································─*/

    template <typename T> struct is_const : false_type {};

    template <typename T> struct is_const<const T> : true_type {};
    
    /*────────────────────────────────────────────────────────────────────────────*/

    template <typename T> struct is_volatile : false_type {};

    template <typename T> struct is_volatile<volatile T> : true_type {};
    
    /*────────────────────────────────────────────────────────────────────────────*/

    template <typename T> struct is_reference : false_type {};

    template <typename T> struct is_reference<T&> : true_type {};

    template <typename T> struct is_reference<T&&> : true_type {};
    
    /*────────────────────────────────────────────────────────────────────────────*/

    template <typename T> struct is_lvalue_reference : false_type {};

    template <typename T> struct is_lvalue_reference<T&> : true_type {};
    
    /*─······································································─*/

    template <typename T> struct is_rvalue_reference : false_type {};

    template <typename T> struct is_rvalue_reference<T&&> : true_type {};
    
    /*─······································································─*/

    template <typename T, ulong N = 0> struct add_extent { typedef T type[N]; };

    template <typename T> struct add_extent<T[], 0> { typedef T type[]; };

    template <typename T, ulong N> struct add_extent<T[], N> { typedef T type[N]; };
    
    /*─······································································─*/

    template <typename T> struct remove_extent { typedef T type; };

    template <typename T, ulong N> struct remove_extent<T[N]> { typedef T type; };
    
    /*─······································································─*/

    template<typename T> struct add_const { using type = const T; };

    template<typename T> struct remove_const { using type = T; };

    template<typename T> struct remove_const<const T> { using type = T; };
    
    /*─······································································─*/

    template<typename T> struct add_volatile { using type = volatile T; };

    template<typename T> struct remove_volatile { using type = T; };

    template<typename T> struct remove_volatile<volatile T> { using type = T; };
    
    /*─······································································─*/

    template<typename T> struct remove_reference { using type = T; };

    template<typename T> struct remove_reference<T&> { using type = T; };

    template<typename T> struct remove_reference<T&&> { using type = T; };

    /*─······································································─*/

    template<typename T> typename remove_reference<T>::type&& move(T&& arg) { return static_cast<typename remove_reference<T>::type&&>( arg ); }

    template<typename T> typename remove_reference<T>::type&  copy(T& arg) { return static_cast<typename remove_reference<T>::type&>( arg ); }

    template<typename T> typename remove_reference<T>::type&& forward(T&& arg) { return move(arg); }

    template<typename T> typename remove_reference<T>::type&  forward(T& arg) { return copy(arg); }
    
    /*─······································································─*/

    template<typename T> struct add_pointer { using type = T*; };

    template<typename T> struct remove_pointer { using type = T; };

    template<typename T> struct remove_pointer<T*> { using type = T; };

    template<typename T> struct remove_pointer<T* const> { using type = T; };

    template<typename T> struct remove_pointer<T* volatile> { using type = T; };

    template<typename T> struct remove_pointer<T* const volatile> { using type = T; };
    
    /*─······································································─*/
    
    template<typename T> struct add_cv { using type = const volatile T; };

    template<typename T> struct remove_cv { using type = typename remove_volatile<typename remove_const<T>::type>::type; };
    
    /*─······································································─*/

    template<typename T> struct add_lvalue_reference { using type = T&; };

    template<typename T> struct add_rvalue_reference { using type = T&&; };

    template<typename T> typename add_rvalue_reference<T>::type add_rvalue_reference_fn(T&& t) { return static_cast<typename add_rvalue_reference<T>::type>(t); };

    template<typename T> struct add_reference : public conditional<is_lvalue_reference<T>::value, add_lvalue_reference<typename remove_reference<T>::type>, add_rvalue_reference<typename remove_reference<T>::type>>::type {};
    
    /*─······································································─*/

    template<typename T> void swap( T& a, T& b ) noexcept { T temp = a; a = b; b = temp; }

    /*─······································································─*/

    template<typename T1, typename T2, typename... Ts> struct max {
        using larger_type = typename conditional< (sizeof(T1) > sizeof(T2)), T1, T2>::type;
        using type = typename max<larger_type, Ts...>::type;
    };

    template<typename T1, typename T2> struct max< T1, T2 > {
        static constexpr bool t1_greater = sizeof(T1) > sizeof(T2);
        using type = typename conditional<t1_greater, T1, T2>::type;
    };

    /*─······································································─*/

    template<typename T1, typename T2, typename... Ts> struct min {
        using larger_type = typename conditional<sizeof(T1) < sizeof(T2), T1, T2>::type;
        using type = typename min<larger_type, Ts...>::type;
    };

    template<typename T1, typename T2> struct min< T1, T2 > {
        static constexpr bool t1_greater = sizeof(T1) < sizeof(T2);
        using type = typename conditional<t1_greater, T1, T2>::type;
    };

    /*─······································································─*/
    
    template< typename... Ts > struct get_size {
        static constexpr ulong value = sizeof...(Ts);
    };

    /*─······································································─*/

    template <typename T>
    struct is_member_pointer : false_type {};

    template <typename T, typename U>
    struct is_member_pointer<T U::*> : true_type {};

    template <typename T, typename U>
    struct is_member_pointer<T (U::*)()> : true_type {};

    /*─······································································─*/
    
    template< int V, typename... Ts > struct is_greater_than {
        static constexpr bool value = sizeof...(Ts) > V;
    };
    
    template< int V, typename... Ts > struct is_lower_than {
        static constexpr bool value = sizeof...(Ts) < V;
    };
    
    template< int V, typename... Ts > struct is_equal_to {
        static constexpr bool value = sizeof...(Ts) == V;
    };

    /*─······································································─*/

    template<typename T> struct is_trivially_constructible {
        static constexpr bool value = __is_trivially_constructible(T);
    };

    template<typename T> struct is_trivially_destructible {
        static constexpr bool value = __has_trivial_destructor(T);
    };

    template<typename T> struct is_virtually_destructible {
        static constexpr bool value = __has_virtual_destructor(T);
    };

    template<typename T> struct is_trivially_copiable {
        static constexpr bool value = __is_trivially_copyable(T);
    };

    template<typename T> struct is_trivially_assigned {
        static constexpr bool value = __has_trivial_assign(T);
    };

    template<typename T, typename V> struct is_trivially_assignable {
        static constexpr bool value = __is_trivially_assignable(T,V);
    };

    /*─······································································─*/

    template< typename T, typename V > struct is_base_of {
        static constexpr bool value = __is_base_of( T, V );
    };

    template<typename T> struct is_polimorfic {
        static constexpr bool value = __is_polymorphic(T);
    };

    template<typename T> struct is_abstract {
        static constexpr bool value = __is_abstract(T);
    };

    template<typename T> struct is_class {
        static constexpr bool value = __is_class(T);
    };

    template<typename T> struct is_empty {
        static constexpr bool value = __is_empty(T);
    };

    template<typename T> struct is_union {
        static constexpr bool value = __is_union(T);
    };

    template<typename T> struct is_enum {
        static constexpr bool value = __is_enum(T);
    };

    template<typename T> struct is_pod {
        static constexpr bool value = __is_pod(T);
    };

    /*─······································································─*/

    template<typename T, typename U>
    struct pair { T first; U second; };

    /*─······································································─*/

    template< typename T, ulong N > 
    struct list { T buffer[N];

        const T  operator []( ulong i ) const noexcept { return       buffer[i%N]; }
        explicit operator bool()        const noexcept { return (bool)buffer;      }
        const T* operator &()           const noexcept { return       buffer;      }

        /*─······································································─*/

        T* begin() const noexcept { return buffer + 0; }
        T* end()   const noexcept { return buffer + N; }

        /*─······································································─*/

        ulong size()   const noexcept { return N; }

    };

}}

/*────────────────────────────────────────────────────────────────────────────*/

#include "ptr.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace type {

    template< class T, class V > T* cast( ptr_t<V>& object ){ if( object==nullptr ){ return nullptr; } return ( T* )( object ); }
    template< class T, class V > T* cast(       V*  object ){ if( object==nullptr ){ return nullptr; } return ( T* )( object ); }
    template< class T, class V > T  cast(       V   object ){ return ( T )( object ); }

    template<class T> ptr_t<T>      bind( ptr_t<T>& object ){ if( object==nullptr ){ return nullptr; } return    object.copy(); }
    template<class T> ptr_t<T>      bind(       T*  object ){ if( object==nullptr ){ return nullptr; } return new T( *object ); }
    template<class T> ptr_t<T>      bind(       T   object ){ return new T( object ); }

}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif
