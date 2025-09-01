#include <ranges>

#include "common.hpp"
#include "position.hpp"
#include "psqt_state.hpp"

#include "evaluation.hpp"

#include "eval_types.hpp"
#include "square.hpp"

namespace Clockwork {

// clang-format off
const PScore PAWN_MAT     = S(147,234);
const PScore KNIGHT_MAT   = S(763,739);
const PScore BISHOP_MAT   = S(774,714);
const PScore ROOK_MAT     = S(1009,1221);
const PScore QUEEN_MAT    = S(2580,1833);
const PScore MOBILITY_VAL = S(15, 13);
const PScore TEMPO_VAL    = S(20, 5);

const PScore BISHOP_PAIR_VAL = S(69,156);

const std::array<PScore, 48> PAWN_PSQT = {
    S(222,576),     S(118,663),     S(255,569),     S(300,506),     S(300,398),     S(427,334),     S(273,430),     S(332,431),
    S(65,357),      S(116,422),     S(170,350),     S(225,313),     S(182,273),     S(151,248),     S(88,287),      S(-26,330),
    S(51,222),      S(119,214),     S(98,179),      S(122,160),     S(121,139),     S(39,151),      S(47,183),      S(-28,232),
    S(24,174),      S(112,171),     S(85,121),      S(104,122),     S(93,106),      S(19,112),      S(19,172),      S(-67,193),
    S(29,135),      S(125,121),     S(106,104),     S(86,123),      S(59,118),      S(-2,110),      S(0,147),       S(-50,153),
    S(18,147),      S(119,127),     S(86,129),      S(73,132),      S(16,114),      S(-11,138),     S(-16,163),     S(-67,179),
};

const std::array<PScore, 64> KNIGHT_PSQT = {
    S(-219,-69),    S(79,141),      S(-82,143),     S(293,105),     S(172,132),     S(79,185),      S(-272,192),    S(-178,-55),
    S(117,82),      S(139,159),     S(220,126),     S(158,170),     S(271,134),     S(168,124),     S(162,109),     S(90,55),
    S(101,176),     S(218,140),     S(332,122),     S(313,107),     S(314,107),     S(212,159),     S(186,147),     S(131,104),
    S(171,129),     S(123,166),     S(213,143),     S(133,198),     S(160,196),     S(157,173),     S(117,146),     S(136,119),
    S(98,124),      S(126,125),     S(115,152),     S(122,160),     S(106,163),     S(120,170),     S(122,128),     S(70,113),
    S(19,73),       S(72,107),      S(58,92),       S(121,131),     S(117,132),     S(38,105),      S(46,125),      S(15,32),
    S(-30,117),     S(36,128),      S(73,86),       S(92,83),       S(80,87),       S(35,73),       S(62,81),       S(6,-11),
    S(-123,115),    S(62,32),       S(45,50),       S(55,69),       S(24,94),       S(3,25),        S(48,22),       S(-107,-10),
};

const std::array<PScore, 64> BISHOP_PSQT = {
    S(45,235),      S(82,211),      S(-24,220),     S(7,227),       S(-18,235),     S(-19,227),     S(17,279),      S(6,266),
    S(9,181),       S(36,230),      S(120,202),     S(99,199),      S(126,198),     S(119,210),     S(72,223),      S(-15,182),
    S(163,158),     S(163,206),     S(195,200),     S(242,144),     S(226,167),     S(155,195),     S(163,192),     S(86,173),
    S(150,160),     S(105,207),     S(173,185),     S(180,174),     S(183,176),     S(136,185),     S(90,206),      S(88,157),
    S(120,133),     S(142,151),     S(85,182),      S(144,155),     S(139,175),     S(57,193),      S(124,161),     S(93,151),
    S(90,151),      S(82,155),      S(106,159),     S(85,176),      S(72,182),      S(90,168),      S(86,134),      S(96,111),
    S(95,115),      S(105,141),     S(99,136),      S(81,152),      S(72,144),      S(83,133),      S(86,125),      S(76,95),
    S(43,172),      S(35,158),      S(51,113),      S(0,152),       S(23,134),      S(87,105),      S(45,123),      S(-40,155),
};

const std::array<PScore, 64> ROOK_PSQT = {
    S(493,291),     S(340,362),     S(296,401),     S(271,373),     S(365,321),     S(428,297),     S(388,317),     S(323,337),
    S(423,313),     S(248,400),     S(396,358),     S(397,352),     S(434,333),     S(382,333),     S(256,370),     S(315,341),
    S(282,357),     S(333,346),     S(345,368),     S(383,340),     S(396,323),     S(329,345),     S(298,336),     S(209,371),
    S(164,342),     S(146,381),     S(202,375),     S(205,340),     S(211,348),     S(199,358),     S(123,377),     S(126,371),
    S(58,338),      S(68,363),      S(74,371),      S(88,352),      S(102,348),     S(65,376),      S(31,378),      S(1,385),
    S(-26,330),     S(35,321),      S(35,343),      S(63,331),      S(56,329),      S(32,330),      S(1,342),       S(-35,348),
    S(-63,352),     S(10,326),      S(41,336),      S(79,308),      S(69,313),      S(38,317),      S(-5,337),      S(-48,331),
    S(-16,350),     S(27,345),      S(44,349),      S(106,306),     S(93,322),      S(62,328),      S(33,329),      S(0,335),
};

const std::array<PScore, 64> QUEEN_PSQT = {
    S(401,905),     S(503,880),     S(484,930),     S(393,1016),    S(460,928),     S(448,917),     S(416,890),     S(290,937),
    S(339,983),     S(232,1070),    S(399,1050),    S(322,1050),    S(285,1124),    S(207,1109),    S(175,1004),    S(292,886),
    S(318,996),     S(361,998),     S(365,1006),    S(385,993),     S(333,991),     S(265,980),     S(279,965),     S(245,935),
    S(257,1045),    S(198,1093),    S(250,1063),    S(223,1087),    S(189,1094),    S(218,1008),    S(187,984),     S(233,891),
    S(201,932),     S(225,989),     S(184,984),     S(197,993),     S(170,1008),    S(169,946),     S(193,890),     S(199,893),
    S(217,817),     S(229,816),     S(205,903),     S(204,843),     S(191,816),     S(183,824),     S(203,783),     S(210,837),
    S(229,646),     S(244,599),     S(248,695),     S(260,653),     S(233,687),     S(243,647),     S(201,719),     S(225,807),
    S(190,790),     S(258,504),     S(187,674),     S(251,568),     S(267,617),     S(237,608),     S(227,650),     S(188,792),
};

const std::array<PScore, 64> KING_PSQT = {
    S(457,-243),    S(397,-83),     S(407,-66),     S(533,-13),     S(0,0), S(0,0), S(0,0), S(0,0),
    S(42,-34),      S(-43,76),      S(39,56),       S(53,43),       S(0,0), S(0,0), S(0,0), S(0,0),
    S(-112,90),     S(-109,135),    S(-86,126),     S(-136,99),     S(0,0), S(0,0), S(0,0), S(0,0),
    S(-288,74),     S(-105,95),     S(-175,103),    S(-194,86),     S(0,0), S(0,0), S(0,0), S(0,0),
    S(-217,36),     S(-123,25),     S(-215,58),     S(-189,61),     S(0,0), S(0,0), S(0,0), S(0,0),
    S(-58,-51),     S(-57,-43),     S(-160,4),      S(-192,31),     S(0,0), S(0,0), S(0,0), S(0,0),
    S(84,-117),     S(32,-102),     S(-21,-68),     S(-91,-33),     S(0,0), S(0,0), S(0,0), S(0,0),
    S(151,-187),    S(166,-163),    S(40,-109),     S(25,-115),     S(0,0), S(0,0), S(0,0), S(0,0),
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

    PScore bishop_pair_bonus = BISHOP_PAIR_VAL
                             * ((pos.piece_count(Color::White, PieceType::Bishop) >= 2)
                                - (pos.piece_count(Color::Black, PieceType::Bishop) >= 2));

    PScore mobility = MOBILITY_VAL * mob_count;

    PScore tempo = (us == Color::White) ? TEMPO_VAL : -TEMPO_VAL;
    PScore sum   = psqt_state.score() + mobility + tempo + bishop_pair_bonus;
    return sum->phase<24>(phase);
};

Score evaluate_stm_pov(const Position& pos, const PsqtState& psqt_state) {
    const Color us = pos.active_color();
    return (us == Color::White) ? evaluate_white_pov(pos, psqt_state)
                                : -evaluate_white_pov(pos, psqt_state);
}

}  // namespace Clockwork
