#include <ranges>

#include "common.hpp"
#include "position.hpp"
#include "psqt_state.hpp"

#include "evaluation.hpp"

#include "eval_types.hpp"
#include "square.hpp"

namespace Clockwork {

// clang-format off
const PScore PAWN_MAT     = S(153,233);
const PScore KNIGHT_MAT   = S(774,716);
const PScore BISHOP_MAT   = S(785,698);
const PScore ROOK_MAT     = S(1028,1192);
const PScore QUEEN_MAT    = S(2618,1770);
const PScore MOBILITY_VAL = S(15, 13);
const PScore TEMPO_VAL    = S(20, 5);

const PScore BISHOP_PAIR_VAL = S(75,158);
const PScore DOUBLED_PAWN_VAL = S(-59,-124);

const std::array<PScore, 48> PAWN_PSQT = {
    S(213,570),     S(110,657),     S(260,568),     S(297,502),     S(309,395),     S(417,330),     S(270,428),     S(326,422),
    S(60,358),      S(109,423),     S(171,351),     S(225,312),     S(185,270),     S(147,249),     S(81,288),      S(-31,327),
    S(45,226),      S(110,215),     S(95,186),      S(122,164),     S(123,141),     S(40,153),      S(40,184),      S(-33,232),
    S(16,172),      S(105,172),     S(88,125),      S(106,126),     S(96,109),      S(24,113),      S(12,172),      S(-74,190),
    S(22,136),      S(122,122),     S(115,118),     S(89,125),      S(61,121),      S(6,121),       S(-1,148),      S(-56,152),
    S(9,146),       S(113,129),     S(87,134),      S(73,138),      S(16,119),      S(-10,142),     S(-22,164),     S(-76,176),
};

const std::array<PScore, 64> KNIGHT_PSQT = {
    S(-217,-82),    S(48,138),      S(-108,145),    S(287,113),     S(196,121),     S(83,184),      S(-290,194),    S(-190,-53),
    S(134,86),      S(149,160),     S(221,126),     S(171,166),     S(277,135),     S(162,127),     S(165,112),     S(90,72),
    S(108,173),     S(223,146),     S(335,124),     S(305,109),     S(311,106),     S(206,163),     S(194,147),     S(129,102),
    S(177,131),     S(123,163),     S(211,142),     S(130,198),     S(158,198),     S(151,175),     S(116,149),     S(140,116),
    S(98,124),      S(125,127),     S(113,150),     S(120,158),     S(105,161),     S(118,173),     S(125,128),     S(70,109),
    S(18,73),       S(71,104),      S(59,92),       S(122,131),     S(118,130),     S(39,107),      S(45,125),      S(14,32),
    S(-31,113),     S(34,127),      S(74,85),       S(92,86),       S(79,88),       S(37,72),       S(61,85),       S(7,-4),
    S(-125,123),    S(61,24),       S(46,51),       S(59,66),       S(28,88),       S(11,27),       S(46,16),       S(-113,-9),
};

const std::array<PScore, 64> BISHOP_PSQT = {
    S(35,237),      S(75,204),      S(-7,212),      S(29,219),      S(-28,234),     S(-22,227),     S(33,272),      S(5,258),
    S(10,183),      S(44,224),      S(110,203),     S(107,197),     S(118,200),     S(110,209),     S(73,219),      S(-17,183),
    S(164,156),     S(169,202),     S(192,198),     S(246,141),     S(224,164),     S(165,189),     S(164,190),     S(93,165),
    S(148,158),     S(100,203),     S(167,182),     S(172,167),     S(179,171),     S(130,184),     S(90,201),      S(83,148),
    S(121,135),     S(139,146),     S(80,181),      S(143,151),     S(139,170),     S(56,191),      S(119,158),     S(97,151),
    S(92,147),      S(80,160),      S(104,158),     S(83,175),      S(72,177),      S(86,165),      S(87,132),      S(95,112),
    S(94,122),      S(105,140),     S(100,131),     S(82,147),      S(71,138),      S(84,135),      S(83,121),      S(79,101),
    S(46,172),      S(41,156),      S(51,111),      S(2,150),       S(27,133),      S(85,104),      S(53,120),      S(-32,152),
};

const std::array<PScore, 64> ROOK_PSQT = {
    S(497,284),     S(345,355),     S(281,401),     S(254,372),     S(356,320),     S(414,295),     S(375,313),     S(324,330),
    S(428,307),     S(254,396),     S(399,353),     S(399,346),     S(427,329),     S(382,329),     S(254,368),     S(324,334),
    S(278,352),     S(332,342),     S(339,368),     S(387,334),     S(397,318),     S(344,336),     S(305,330),     S(210,366),
    S(166,337),     S(148,375),     S(211,371),     S(203,337),     S(211,343),     S(197,354),     S(130,372),     S(131,366),
    S(57,340),      S(76,359),      S(73,368),      S(85,352),      S(101,346),     S(68,374),      S(38,374),      S(2,383),
    S(-28,327),     S(38,317),      S(38,338),      S(65,326),      S(52,326),      S(26,330),      S(4,339),       S(-36,347),
    S(-65,344),     S(12,322),      S(41,333),      S(76,305),      S(67,308),      S(38,315),      S(-1,331),      S(-52,327),
    S(-18,340),     S(28,343),      S(43,347),      S(106,301),     S(91,318),      S(63,325),      S(34,325),      S(-2,329),
};

const std::array<PScore, 64> QUEEN_PSQT = {
    S(396,893),     S(512,861),     S(474,931),     S(365,1030),    S(455,921),     S(446,913),     S(418,884),     S(288,926),
    S(334,997),     S(236,1062),    S(399,1039),    S(319,1049),    S(289,1109),    S(212,1102),    S(177,1001),    S(311,880),
    S(322,981),     S(366,990),     S(364,1009),    S(384,993),     S(327,992),     S(258,982),     S(278,969),     S(246,944),
    S(258,1034),    S(199,1080),    S(247,1067),    S(215,1092),    S(186,1091),    S(210,1021),    S(189,980),     S(232,897),
    S(203,912),     S(229,977),     S(183,981),     S(193,996),     S(172,1002),    S(167,954),     S(189,901),     S(197,899),
    S(220,817),     S(229,812),     S(204,903),     S(205,852),     S(190,817),     S(183,821),     S(203,788),     S(210,833),
    S(227,651),     S(246,602),     S(248,697),     S(260,647),     S(233,685),     S(244,641),     S(204,723),     S(222,810),
    S(196,791),     S(258,503),     S(186,683),     S(253,565),     S(266,610),     S(238,607),     S(228,647),     S(178,808),
};

const std::array<PScore, 64> KING_PSQT = {
    S(454,-240),    S(420,-83),     S(426,-69),     S(554,-16),     S(0,0), S(0,0), S(0,0), S(0,0),
    S(70,-35),      S(-21,71),      S(57,52),       S(80,40),       S(0,0), S(0,0), S(0,0), S(0,0),
    S(-93,89),      S(-90,132),     S(-77,125),     S(-103,95),     S(0,0), S(0,0), S(0,0), S(0,0),
    S(-300,79),     S(-88,92),      S(-165,102),    S(-174,83),     S(0,0), S(0,0), S(0,0), S(0,0),
    S(-223,41),     S(-119,26),     S(-201,58),     S(-176,61),     S(0,0), S(0,0), S(0,0), S(0,0),
    S(-53,-46),     S(-57,-39),     S(-160,8),      S(-190,36),     S(0,0), S(0,0), S(0,0), S(0,0),
    S(86,-107),     S(34,-98),      S(-19,-64),     S(-91,-28),     S(0,0), S(0,0), S(0,0), S(0,0),
    S(159,-180),    S(169,-157),    S(44,-105),     S(29,-112),     S(0,0), S(0,0), S(0,0), S(0,0),
};

// clang-format on

Score evaluate_white_pov(const Position& pos, const PsqtState& psqt_state) {

    const Color us    = pos.active_color();
    i32         phase = pos.piece_count(Color::White, PieceType::Knight)
              + pos.piece_count(Color::Black, PieceType::Knight)
              + pos.piece_count(Color::White, PieceType::Bishop)
              + pos.piece_count(Color::Black, PieceType::Bishop)
              + 2
                  * (pos.piece_count(Color::White, PieceType::Rook)
                     + pos.piece_count(Color::Black, PieceType::Rook))
              + 4
                  * (pos.piece_count(Color::White, PieceType::Queen)
                     + pos.piece_count(Color::Black, PieceType::Queen));

    phase = std::min<i32>(phase, 24);

    i32 mob_count = 0;
    for (u64 x : std::bit_cast<std::array<u64, 16>>(pos.attack_table(Color::White))) {
        mob_count += std::popcount(x);
    }
    for (u64 x : std::bit_cast<std::array<u64, 16>>(pos.attack_table(Color::Black))) {
        mob_count -= std::popcount(x);
    }

    const std::array<Bitboard, 2> pawns = {pos.board().bitboard_for(Color::White, PieceType::Pawn),
                                           pos.board().bitboard_for(Color::Black, PieceType::Pawn)};

    PScore doubled_pawns_bonus = DOUBLED_PAWN_VAL
                               * ((pawns[0] & pawns[0].shift(Direction::North)).popcount()
                                  - (pawns[1] & pawns[1].shift(Direction::South)).popcount());

    PScore bishop_pair_bonus = BISHOP_PAIR_VAL
                             * ((pos.piece_count(Color::White, PieceType::Bishop) >= 2)
                                - (pos.piece_count(Color::Black, PieceType::Bishop) >= 2));

    PScore mobility = MOBILITY_VAL * mob_count;

    PScore tempo = (us == Color::White) ? TEMPO_VAL : -TEMPO_VAL;
    PScore sum   = psqt_state.score() + mobility + tempo + bishop_pair_bonus + doubled_pawns_bonus;
    return sum->phase<24>(phase);
};

Score evaluate_stm_pov(const Position& pos, const PsqtState& psqt_state) {
    const Color us = pos.active_color();
    return (us == Color::White) ? evaluate_white_pov(pos, psqt_state)
                                : -evaluate_white_pov(pos, psqt_state);
}

}  // namespace Clockwork
