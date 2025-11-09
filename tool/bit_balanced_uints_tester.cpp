#include <algorithm>
#include <bit>
#include <bitset>
#include <format>
#include <iostream>
#include <utility>
#include <cstdint>
#include <vector>
#include <tuple>

// clang-format off
#define ADD_UINT(vints_, uinteger_, name_, comment_) \
  vints_.emplace_back(uinteger_, name_, #uinteger_, comment_)
// clang-format on
constexpr std::string_view decl_fmt = "static constexpr bit_balanced_uint{} {} = make_enumerator({});{}";

void bit_balanced_uint32s()
{
    std::vector<std::tuple<uint64_t, std::string, std::string, std::string>> ints;
    // clang-format off
    // hexadecimal l33t:
    ADD_UINT(ints, 0xcaac'caac, "caac", "");
    ADD_UINT(ints, 0xc0de'c0de, "c0de", "");
    // seq:
    ADD_UINT(ints, 1'2'3'5'7'11'13, "primes_rseq", " // https://oeis.org/A008578");
    ADD_UINT(ints, 13'11'7'5'3'2'1, "primes_lseq", " // https://oeis.org/A008578");
    ADD_UINT(ints, 1'1'2'3'5'8'13'21, "fibonacci_rseq", " // https://oeis.org/A000045");
    ADD_UINT(ints, 21'13'8'5'3'2'1'1, "fibonacci_lseq", " // https://oeis.org/A000045");
    ADD_UINT(ints, 1'4'9'16'25'36, "squares_rseq", " // https://oeis.org/A000290");
    ADD_UINT(ints, 36'25'16'9'4'1, "squares_lseq", " // https://oeis.org/A000290");
    ADD_UINT(ints, 1'3'6'10'15'21, "triangulars_rseq", " // https://oeis.org/A000217");
    ADD_UINT(ints, 21'15'10'6'3'1, "triangulars_lseq", " // https://oeis.org/A000217");
    ADD_UINT(ints, 1'1'2'6'24'120, "factorials_rseq", " // https://oeis.org/A000142");
    ADD_UINT(ints, 120'24'6'2'1'1, "factorials_lseq", " // https://oeis.org/A000142");
    ADD_UINT(ints, 1'1'2'5'14'42, "catalan_rseq", " // https://oeis.org/A000108");
    ADD_UINT(ints, 42'14'5'2'1'1, "catalan_lseq", " // https://oeis.org/A000108");
    ADD_UINT(ints, 3'7'31'127, "mersenne_rseq", " // https://oeis.org/A001348");
    ADD_UINT(ints, 127'31'7'3, "mersenne_lseq", " // https://oeis.org/A001348");
    ADD_UINT(ints, 6'28'496, "perfects_rseq", " // https://oeis.org/A000396");
    ADD_UINT(ints, 496'28'6, "perfects_lseq", " // https://oeis.org/A000396");
    ADD_UINT(ints, 1'5'12'22'35'51, "pentagonals_rseq", " // https://oeis.org/A000326");
    ADD_UINT(ints, 35'22'12'5'1, "pentagonals_lseq", " // https://oeis.org/A000326");
    ADD_UINT(ints, 1'6'15'28'45'66, "hexagonals_rseq", " // https://oeis.org/A000384");
    ADD_UINT(ints, 45'28'15'6'1, "hexagonals_lseq", " // https://oeis.org/A000384");
    ADD_UINT(ints, 1'1'2'4'9'21'51, "motzkin_rseq", " // https://oeis.org/A001006");
    ADD_UINT(ints, 51'21'9'4'2'1'1, "motzkin_lseq", " // https://oeis.org/A001006");
    ADD_UINT(ints, 1'1'2'5'15'52, "bell_rseq", " // https://oeis.org/A000110");
    ADD_UINT(ints, 52'15'5'2'1'1, "bell_lseq", " // https://oeis.org/A000110");
    ADD_UINT(ints, 1'1'1'2'3'4'6'9'13, "narayana_cows_rseq", " // https://oeis.org/A000930");
    ADD_UINT(ints, 13'9'6'4'3'2'1'1'1, "narayana_cows_lseq", " // https://oeis.org/A000930");
    ADD_UINT(ints, 3'0'2'3'2'5'5'7'10, "perrin_rseq", " // https://oeis.org/A001608");
    ADD_UINT(ints, 10'7'5'5'2'3'2'0'3, "perrin_lseq", " // https://oeis.org/A001608");
    ADD_UINT(ints, 1'1'1'3'5'9'17'31, "tribonacci_rseq", " // https://oeis.org/A000213");
    ADD_UINT(ints, 31'17'9'5'3'1'1'1, "tribonacci_lseq", " // https://oeis.org/A000213");
    // digits:
    ADD_UINT(ints, 3'141592653, "pi_rdigits", "");
    ADD_UINT(ints, 356295141'3, "pi_ldigits", "");
    ADD_UINT(ints, 1'618033988, "phi_rdigits", "");
    ADD_UINT(ints, 89330816'1, "phi_ldigits", "");
    ADD_UINT(ints, 1'414213562, "sqrt2_rdigits", "");
    ADD_UINT(ints, 265312414'1, "sqrt2_ldigits", "");
    ADD_UINT(ints, 2'718281828, "e_rdigits", "");
    ADD_UINT(ints, 28182817'2, "e_ldigits", "");
    ADD_UINT(ints, 6'28318530, "tau_rdigits", "");
    ADD_UINT(ints, 3581382'6, "tau_ldigits", "");
    ADD_UINT(ints, 1'465571231, "psi_rdigits", "");
    ADD_UINT(ints, 132175564'1, "psi_ldigits", "");
    // clang-format on

    unsigned counter = 0;
    for (const auto& [n64, name, n_str, comment] : ints)
    {
        uint32_t n32(n64);
        if (n32 != n64)
        {
            std::cout << "  ERR: " << name << std::endl;
            exit(1);
        }
        unsigned nb_ones = std::popcount(n32);
        const std::string result = (nb_ones >= 15 && nb_ones <= 17) ? "OK" : "  ";
//        std::cout << "0b" << std::bitset<32>(n32) << "  " << nb_ones << "/32 " << result << "  " << name << std::endl;
        if (result != "OK")
            continue;
        std::cout << std::format(decl_fmt, "32", name, n_str, comment) << std::endl;
        ++counter;
    }
    std::cout << "count: " << counter << std::endl;
}

void bit_balanced_uint64s()
{
    std::vector<std::tuple<uint64_t, std::string, std::string, std::string>> ints;
    // clang-format off
    // hexadecimal l33t:
    ADD_UINT(ints, 0xcaac'caac'caac'caacull, "caac", "");
    ADD_UINT(ints, 0xc0de'c0de'c0de'c0deull, "c0de", "");
    // seq:
    ADD_UINT(ints, 1'2'3'5'7'11'13'17'19'23'29'31ull, "primes_rseq", " // https://oeis.org/A008578");
    ADD_UINT(ints, 31'29'23'19'17'13'11'7'5'3'2'1, "primes_lseq", " // https://oeis.org/A008578");
    ADD_UINT(ints, 1'1'2'3'5'8'13'21'34'55'89'144ull, "fibonacci_rseq", " // https://oeis.org/A000045");
    ADD_UINT(ints, 144'89'55'34'21'13'8'5'3'2'1'1, "fibonacci_lseq", " // https://oeis.org/A000045");
    ADD_UINT(ints, 1'4'9'16'25'36'49'64'91'100ull, "squares_rseq", " // https://oeis.org/A000290");
    ADD_UINT(ints, 100'91'64'49'36'25'16'9'4'1ull, "squares_lseq", " // https://oeis.org/A000290");
    ADD_UINT(ints, 1'3'6'10'15'21'28'36'45'55'66ull, "triangulars_rseq", " // https://oeis.org/A000217");
    ADD_UINT(ints, 66'55'45'36'28'21'15'10'6'3'1ull, "triangulars_lseq", " // https://oeis.org/A000217");
    ADD_UINT(ints, 1'1'2'6'24'120'720'5040ull, "factorials_rseq", " // https://oeis.org/A000142");
    ADD_UINT(ints, 5040'720'120'24'6'2'11ull, "factorials_lseq", " // https://oeis.org/A000142");
    ADD_UINT(ints, 1'1'2'5'14'42'132'429'1430ull, "catalan_rseq", " // https://oeis.org/A000108");
    ADD_UINT(ints, 1430'429'132'42'14'5'2'1'1ull, "catalan_lseq", " // https://oeis.org/A000108");
    ADD_UINT(ints, 3'7'31'127'2047'8191ull, "mersenne_rseq", " // https://oeis.org/A001348");
    ADD_UINT(ints, 8191'2047'127'31'7'3ull, "mersenne_lseq", " // https://oeis.org/A001348");
    ADD_UINT(ints, 6'28'496'8128'33550336ull, "perfects_rseq", " // https://oeis.org/A000396");
    ADD_UINT(ints, 33550336'8128'496'28'6ull, "perfects_lseq", " // https://oeis.org/A000396");
    ADD_UINT(ints, 1'5'12'22'35'51'70'92'117'145ull, "pentagonals_rseq", " // https://oeis.org/A000326");
    ADD_UINT(ints, 145'117'92'70'51'35'22'12'5'1ull, "pentagonals_lseq", " // https://oeis.org/A000326");
    ADD_UINT(ints, 1'6'15'28'45'66'91'120'153ull, "hexagonals_rseq", " // https://oeis.org/A000384");
    ADD_UINT(ints, 153'120'91'66'45'28'15'6'1ull, "hexagonals_lseq", " // https://oeis.org/A000384");
    ADD_UINT(ints, 1'1'2'4'9'21'51'127'323'835ull, "motzkin_rseq", " // https://oeis.org/A001006");
    ADD_UINT(ints, 835'323'127'51'21'9'4'2'1'1ull, "motzkin_lseq", " // https://oeis.org/A001006");
    ADD_UINT(ints, 1'1'2'5'15'52'203'877'4140ull, "bell_rseq", " // https://oeis.org/A000110");
    ADD_UINT(ints, 4140'877'203'52'15'5'2'1'1ull, "bell_lseq", " // https://oeis.org/A000110");
    ADD_UINT(ints, 1'1'1'2'3'4'6'9'13'19'28'41'60'88ull, "narayana_cows_rseq", " // https://oeis.org/A000930");
    ADD_UINT(ints, 60'41'28'19'13'9'6'4'3'2'1'1'1ull, "narayana_cows_lseq", " // https://oeis.org/A000930");
    ADD_UINT(ints, 3'0'2'3'2'5'5'7'10'12'17'22'29ull, "perrin_rseq", " // https://oeis.org/A001608");
    ADD_UINT(ints, 29'22'17'12'10'7'5'5'2'3'2'0'3ull, "perrin_lseq", " // https://oeis.org/A001608");
    ADD_UINT(ints, 1'1'1'3'5'9'17'31'57'105'193ull, "tribonacci_rseq", " // https://oeis.org/A000213");
    ADD_UINT(ints, 193'105'57'31'17'9'5'3'1'1'1ull, "tribonacci_lseq", " // https://oeis.org/A000213");
    // constants digits:
    ADD_UINT(ints, 3'141592653589793238ull, "pi_rdigits", "");
    ADD_UINT(ints, 832397985356295141'3ull, "pi_ldigits", "");
    ADD_UINT(ints, 1'6180339887498948482ull, "phi_rdigits", "");
    ADD_UINT(ints, 848498947889330816'1ull, "phi_ldigits", "");
    ADD_UINT(ints, 1'4142135623730950488ull, "sqrt2_rdigits", "");
    ADD_UINT(ints, 840590373265312414'1ull, "sqrt2_ldigits", "");
    ADD_UINT(ints, 2'718281828459045235ull, "e_rdigits", "");
    ADD_UINT(ints, 532540954828182817'2ull, "e_ldigits", "");
    ADD_UINT(ints, 6'283185307179586476ull, "tau_rdigits", "");
    ADD_UINT(ints, 74685971703581382'6ull, "tau_ldigits", "");
    ADD_UINT(ints, 1'4655712318767680266ull, "psi_rdigits", "");
    ADD_UINT(ints, 620867678132175564'1ull, "psi_ldigits", "");
    // clang-format on

    unsigned counter = 0;
    for (const auto& [n64, name, n_str, comment] : ints)
    {
        unsigned nb_ones = std::popcount(n64);
        const std::string result = (nb_ones >= 31 && nb_ones <= 33) ? "OK" : "  ";
//        std::cout << "0b" << std::bitset<64>(n64) << "  " << nb_ones << "/64 " << result << "  " << name << std::endl;
        if (result != "OK")
            continue;
        std::cout << std::format(decl_fmt, "64", name, n_str, comment) << std::endl;
        ++counter;
    }
    std::cout << "count: " << counter << std::endl;
}

void print_reserved(std::string str)
{
    std::ranges::reverse(str);
    std::cout << str << std::endl;
}

int main()
{
    bit_balanced_uint32s();
    std::cout << '\n';
    bit_balanced_uint64s();
    return 0;
}
