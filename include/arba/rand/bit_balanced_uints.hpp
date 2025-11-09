#pragma once

#include <arba/meta/type_traits/enum.hpp>

inline namespace arba
{
namespace rand
{

class bit_balanced_uint32s;

class bit_balanced_uint32
    : public meta::enumerator<bit_balanced_uint32, bit_balanced_uint32s, uint32_t>
{
    ARBA_META_ENUMERATOR(bit_balanced_uint32)
};

class bit_balanced_uint32s : public meta::enumeration<bit_balanced_uint32, bit_balanced_uint32s>
{
public:
    // clang-format off
    // hexadecimal l33t:
    static constexpr bit_balanced_uint32 caac = make_enumerator(0xcaac'caac);
    static constexpr bit_balanced_uint32 c0de = make_enumerator(0xc0de'c0de);
    // number sequences:
    static constexpr bit_balanced_uint32 primes_rseq = make_enumerator(1'2'3'5'7'11'13);       // https://oeis.org/A008578
    static constexpr bit_balanced_uint32 primes_lseq = make_enumerator(13'11'7'5'3'2'1);       // https://oeis.org/A008578
    static constexpr bit_balanced_uint32 fibonacci_rseq = make_enumerator(1'1'2'3'5'8'13'21);  // https://oeis.org/A000045
    static constexpr bit_balanced_uint32 squares_lseq = make_enumerator(36'25'16'9'4'1);       // https://oeis.org/A000290
    static constexpr bit_balanced_uint32 triangulars_lseq = make_enumerator(21'15'10'6'3'1);   // https://oeis.org/A000217
    static constexpr bit_balanced_uint32 factorials_lseq = make_enumerator(120'24'6'2'1'1);    // https://oeis.org/A000142
    static constexpr bit_balanced_uint32 catalan_rseq = make_enumerator(1'1'2'5'14'42);        // https://oeis.org/A000108
    static constexpr bit_balanced_uint32 mersenne_rseq = make_enumerator(3'7'31'127);          // https://oeis.org/A001348
    static constexpr bit_balanced_uint32 pentagonals_rseq = make_enumerator(1'5'12'22'35'51);  // https://oeis.org/A000326
    static constexpr bit_balanced_uint32 motzkin_lseq = make_enumerator(51'21'9'4'2'1'1);      // https://oeis.org/A001006
    static constexpr bit_balanced_uint32 bell_lseq = make_enumerator(52'15'5'2'1'1);           // https://oeis.org/A000110
    static constexpr bit_balanced_uint32 perrin_rseq = make_enumerator(3'0'2'3'2'5'5'7'10);    // https://oeis.org/A001608
    static constexpr bit_balanced_uint32 tribonacci_lseq = make_enumerator(31'17'9'5'3'1'1'1); // https://oeis.org/A000213
    // constants digits:
    static constexpr bit_balanced_uint32 pi_rdigits = make_enumerator(3'141592653);
    static constexpr bit_balanced_uint32 phi_ldigits = make_enumerator(89330816'1);
    static constexpr bit_balanced_uint32 sqrt2_rdigits = make_enumerator(1'414213562);
    static constexpr bit_balanced_uint32 sqrt2_ldigits = make_enumerator(265312414'1);
    static constexpr bit_balanced_uint32 psi_ldigits = make_enumerator(132175564'1);

    static constexpr std::array enumerator_names =
    {
        "caac",
        "c0de",
        "primes_rseq",
        "primes_lseq",
        "fibonacci_rseq",
        "squares_lseq",
        "triangulars_lseq",
        "factorials_lseq",
        "catalan_rseq",
        "mersenne_rseq",
        "pentagonals_rseq",
        "motzkin_lseq",
        "bell_lseq",
        "perrin_rseq",
        "tribonacci_lseq",
        "pi_rdigits",
        "phi_ldigits",
        "sqrt2_rdigits",
        "sqrt2_ldigits",
        "psi_ldigits",
    };

    static constexpr std::array enumerators =
    {
        caac,
        c0de,
        primes_rseq,
        primes_lseq,
        fibonacci_rseq,
        squares_lseq,
        triangulars_lseq,
        factorials_lseq,
        catalan_rseq,
        mersenne_rseq,
        pentagonals_rseq,
        motzkin_lseq,
        bell_lseq,
        perrin_rseq,
        tribonacci_lseq,
        pi_rdigits,
        phi_ldigits,
        sqrt2_rdigits,
        sqrt2_ldigits,
        psi_ldigits,
    };
    // clang-format on
};

class bit_balanced_uint64s;

class bit_balanced_uint64
    : public meta::enumerator<bit_balanced_uint64, bit_balanced_uint64s, uint64_t>
{
    ARBA_META_ENUMERATOR(bit_balanced_uint64)
};

class bit_balanced_uint64s : public meta::enumeration<bit_balanced_uint64, bit_balanced_uint64s>
{
public:
    // clang-format off
    // hexadecimal l33t:
    static constexpr bit_balanced_uint64 caac = make_enumerator(0xcaac'caac'caac'caacull);
    static constexpr bit_balanced_uint64 c0de = make_enumerator(0xc0de'c0de'c0de'c0deull);
    // number sequences:
    static constexpr bit_balanced_uint64 fibonacci_rseq = make_enumerator(1'1'2'3'5'8'13'21'34'55'89'144ull);        // https://oeis.org/A000045
    static constexpr bit_balanced_uint64 fibonacci_lseq = make_enumerator(144'89'55'34'21'13'8'5'3'2'1'1);           // https://oeis.org/A000045
    static constexpr bit_balanced_uint64 triangulars_lseq = make_enumerator(66'55'45'36'28'21'15'10'6'3'1ull);       // https://oeis.org/A000217
    static constexpr bit_balanced_uint64 catalan_lseq = make_enumerator(1430'429'132'42'14'5'2'1'1ull);              // https://oeis.org/A000108
    static constexpr bit_balanced_uint64 mersenne_lseq = make_enumerator(8191'2047'127'31'7'3ull);                   // https://oeis.org/A001348
    static constexpr bit_balanced_uint64 pentagonals_rseq = make_enumerator(1'5'12'22'35'51'70'92'117'145ull);       // https://oeis.org/A000326
    static constexpr bit_balanced_uint64 bell_rseq = make_enumerator(1'1'2'5'15'52'203'877'4140ull);                 // https://oeis.org/A000110
    static constexpr bit_balanced_uint64 narayana_cows_rseq = make_enumerator(1'1'1'2'3'4'6'9'13'19'28'41'60'88ull); // https://oeis.org/A000930
    static constexpr bit_balanced_uint64 narayana_cows_lseq = make_enumerator(60'41'28'19'13'9'6'4'3'2'1'1'1ull);    // https://oeis.org/A000930
    // constants digits:
    static constexpr bit_balanced_uint64 pi_rdigits = make_enumerator(3'141592653589793238ull);
    static constexpr bit_balanced_uint64 pi_ldigits = make_enumerator(832397985356295141'3ull);
    static constexpr bit_balanced_uint64 phi_rdigits = make_enumerator(1'6180339887498948482ull);
    static constexpr bit_balanced_uint64 sqrt2_rdigits = make_enumerator(1'4142135623730950488ull);
    static constexpr bit_balanced_uint64 sqrt2_ldigits = make_enumerator(840590373265312414'1ull);
    static constexpr bit_balanced_uint64 e_rdigits = make_enumerator(2'718281828459045235ull);
    static constexpr bit_balanced_uint64 tau_rdigits = make_enumerator(6'283185307179586476ull);
    static constexpr bit_balanced_uint64 tau_ldigits = make_enumerator(74685971703581382'6ull);
    static constexpr bit_balanced_uint64 psi_rdigits = make_enumerator(1'4655712318767680266ull);

    static constexpr std::array enumerator_names =
    {
        "caac",
        "c0de",
        "fibonacci_rseq",
        "fibonacci_lseq",
        "triangulars_lseq",
        "catalan_lseq",
        "mersenne_lseq",
        "pentagonals_rseq",
        "bell_rseq",
        "narayana_cows_rseq",
        "narayana_cows_lseq",
        "pi_rdigits",
        "pi_ldigits",
        "phi_rdigits",
        "sqrt2_rdigits",
        "sqrt2_ldigits",
        "e_rdigits",
        "tau_rdigits",
        "tau_ldigits",
        "psi_rdigits",
    };

    static constexpr std::array enumerators =
    {
        caac,
        c0de,
        fibonacci_rseq,
        fibonacci_lseq,
        triangulars_lseq,
        catalan_lseq,
        mersenne_lseq,
        pentagonals_rseq,
        bell_rseq,
        narayana_cows_rseq,
        narayana_cows_lseq,
        pi_rdigits,
        pi_ldigits,
        phi_rdigits,
        sqrt2_rdigits,
        sqrt2_ldigits,
        e_rdigits,
        tau_rdigits,
        tau_ldigits,
        psi_rdigits,
    };
    // clang-format on
};

} // namespace rand
} // namespace arba
