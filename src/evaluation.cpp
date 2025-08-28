#include <ranges>

#include "common.hpp"
#include "position.hpp"

#include "evaluation.hpp"

#include "eval_types.hpp"
#include "square.hpp"

namespace Clockwork {

// clang-format off
const PScore PAWN_MAT     = S(147,238);
const PScore KNIGHT_MAT   = S(763,737);
const PScore BISHOP_MAT   =S(835,718);
const PScore ROOK_MAT     = S(1003, 1222);
const PScore QUEEN_MAT    = S(2549, 1853);
const PScore MOBILITY_VAL = S(15, 13);
const PScore TEMPO_VAL    = S(20, 5);

const std::array<PScore, 48> PAWN_PSQT = {
    S(222,572),     S(114,659),     S(255,566),     S(298,503),     S(302,393),     S(427,330),     S(271,426),     S(331,427),
    S(66,354),      S(114,419),     S(169,347),     S(225,311),     S(182,270),     S(151,245),     S(87,283),      S(-26,327),
    S(50,219),      S(118,211),     S(97,176),      S(121,156),     S(121,135),     S(39,147),      S(47,180),      S(-28,229),
    S(23,170),      S(111,168),     S(85,117),      S(103,119),     S(93,102),      S(19,109),      S(19,169),      S(-67,190),
    S(28,133),      S(124,118),     S(105,100),     S(85,119),      S(58,114),      S(-3,106),      S(0,144),       S(-51,150),
    S(17,144),      S(117,123),     S(86,125),      S(72,129),      S(15,112),      S(-12,135),     S(-16,159),     S(-67,176),
};

const std::array<PScore, 64> KNIGHT_PSQT = {
    S(-229,-70),    S(76,148),      S(-81,147),     S(290,110),     S(170,136),     S(78,189),      S(-287,199),    S(-190,-62),
    S(114,84),      S(136,164),     S(216,129),     S(158,175),     S(270,139),     S(164,128),     S(160,114),     S(90,57),
    S(101,180),     S(214,145),     S(331,127),     S(313,111),     S(312,112),     S(211,163),     S(183,150),     S(130,107),
    S(168,133),     S(120,167),     S(212,149),     S(130,201),     S(156,199),     S(156,177),     S(115,148),     S(133,122),
    S(96,124),      S(126,129),     S(112,156),     S(119,163),     S(103,166),     S(117,174),     S(119,131),     S(68,114),
    S(16,74),       S(69,106),      S(55,90),       S(119,133),     S(115,135),     S(35,104),      S(43,124),      S(12,33),
    S(-32,119),     S(34,132),      S(70,87),       S(89,84),       S(77,87),       S(32,75),       S(60,84),       S(4,-8),
    S(-124,116),    S(60,30),       S(44,51),       S(52,71),       S(21,94),       S(1,26),        S(46,20),       S(-110,-8),
};

const std::array<PScore, 64> BISHOP_PSQT = {
    S(46,232),      S(88,209),      S(-18,220),     S(8,227),       S(-17,234),     S(-11,227),     S(12,280),      S(11,264),
    S(14,182),      S(41,229),      S(128,200),     S(103,199),     S(131,197),     S(126,210),     S(80,224),      S(-12,183),
    S(168,162),     S(169,206),     S(200,198),     S(254,145),     S(232,168),     S(162,194),     S(167,192),     S(93,180),
    S(156,161),     S(110,209),     S(181,184),     S(184,173),     S(190,176),     S(141,184),     S(98,209),      S(91,160),
    S(125,135),     S(148,152),     S(90,182),      S(151,155),     S(144,174),     S(64,195),      S(128,163),     S(101,158),
    S(97,156),      S(88,154),      S(114,159),     S(90,175),      S(79,184),      S(95,168),      S(93,137),      S(101,114),
    S(100,116),     S(112,147),     S(103,132),     S(89,155),      S(77,147),      S(91,137),      S(91,127),      S(84,97),
    S(51,176),      S(39,156),      S(59,119),      S(4,152),       S(31,136),      S(93,109),      S(54,124),      S(-35,156),
};

const std::array<PScore, 64> ROOK_PSQT = {
    S(493,291),     S(345,361),     S(296,401),     S(274,372),     S(367,321),     S(429,297),     S(389,315),     S(323,337),
    S(425,312),     S(250,399),     S(399,357),     S(404,350),     S(442,331),     S(385,332),     S(259,370),     S(317,340),
    S(285,356),     S(336,345),     S(350,367),     S(386,339),     S(398,322),     S(333,344),     S(302,335),     S(213,370),
    S(168,340),     S(146,380),     S(203,375),     S(208,340),     S(214,348),     S(200,358),     S(124,377),     S(128,370),
    S(61,336),      S(73,362),      S(75,372),      S(90,353),      S(104,348),     S(67,375),      S(33,377),      S(4,384),
    S(-23,329),     S(37,321),      S(37,342),      S(66,330),      S(59,329),      S(33,330),      S(4,342),       S(-32,346),
    S(-59,351),     S(12,326),      S(43,336),      S(82,307),      S(72,313),      S(41,317),      S(-3,337),      S(-45,330),
    S(-12,348),     S(30,344),      S(47,348),      S(109,304),     S(96,321),      S(65,326),      S(36,328),      S(3,333),
};

const std::array<PScore, 64> QUEEN_PSQT = {
    S(397,908),     S(507,878),     S(488,930),     S(396,1014),    S(466,926),     S(451,916),     S(414,893),     S(287,941),
    S(337,987),     S(230,1074),    S(398,1052),    S(320,1051),    S(287,1124),    S(206,1111),    S(174,1006),    S(290,888),
    S(317,999),     S(359,1003),    S(369,1005),    S(387,994),     S(332,991),     S(265,983),     S(278,968),     S(244,937),
    S(254,1050),    S(195,1093),    S(250,1065),    S(221,1090),    S(188,1098),    S(218,1009),    S(184,981),     S(231,895),
    S(199,931),     S(223,990),     S(181,988),     S(196,997),     S(168,1011),    S(167,952),     S(192,891),     S(197,892),
    S(216,815),     S(226,814),     S(204,901),     S(202,849),     S(188,822),     S(181,823),     S(201,779),     S(209,833),
    S(227,648),     S(242,599),     S(245,693),     S(258,648),     S(230,687),     S(241,645),     S(200,715),     S(223,806),
    S(189,788),     S(257,505),     S(187,671),     S(250,567),     S(265,615),     S(236,604),     S(226,648),     S(188,788),
};

const std::array<PScore, 64> KING_PSQT = {
    S(468,-245),    S(399,-84),     S(409,-68),     S(530,-14),     S(0,0), S(0,0), S(0,0), S(0,0),
    S(40,-35),      S(-38,74),      S(41,54),       S(55,41),       S(0,0), S(0,0), S(0,0), S(0,0),
    S(-108,89),     S(-108,134),    S(-84,125),     S(-135,99),     S(0,0), S(0,0), S(0,0), S(0,0),
    S(-289,74),     S(-105,94),     S(-176,102),    S(-198,87),     S(0,0), S(0,0), S(0,0), S(0,0),
    S(-220,36),     S(-125,24),     S(-216,58),     S(-190,61),     S(0,0), S(0,0), S(0,0), S(0,0),
    S(-63,-51),     S(-61,-43),     S(-163,4),      S(-196,31),     S(0,0), S(0,0), S(0,0), S(0,0),
    S(79,-118),     S(27,-103),     S(-25,-68),     S(-95,-33),     S(0,0), S(0,0), S(0,0), S(0,0),
    S(147,-187),    S(162,-164),    S(36,-109),     S(21,-114),     S(0,0), S(0,0), S(0,0), S(0,0),
};

// clang-format on

Score evaluate_white_pov(Position pos) {

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

    PScore material = PAWN_MAT
                      * (pos.piece_count(Color::White, PieceType::Pawn)
                         - pos.piece_count(Color::Black, PieceType::Pawn))
                    + KNIGHT_MAT
                        * (pos.piece_count(Color::White, PieceType::Knight)
                           - pos.piece_count(Color::Black, PieceType::Knight))
                    + BISHOP_MAT
                        * (pos.piece_count(Color::White, PieceType::Bishop)
                           - pos.piece_count(Color::Black, PieceType::Bishop))
                    + ROOK_MAT
                        * (pos.piece_count(Color::White, PieceType::Rook)
                           - pos.piece_count(Color::Black, PieceType::Rook))
                    + QUEEN_MAT
                        * (pos.piece_count(Color::White, PieceType::Queen)
                           - pos.piece_count(Color::Black, PieceType::Queen));

    i32 mob_count = 0;
    for (u64 x : std::bit_cast<std::array<u64, 16>>(pos.attack_table(Color::White))) {
        mob_count += std::popcount(x);
    }
    for (u64 x : std::bit_cast<std::array<u64, 16>>(pos.attack_table(Color::Black))) {
        mob_count -= std::popcount(x);
    }

    PScore psqt = PSCORE_ZERO;

    for (Color c : {Color::Black, Color::White}) {
        auto& pieces  = pos.piece_list(c);
        auto& squares = pos.piece_list_sq(c);

        auto king_side = pos.king_side(c);

        for (size_t i = 0; i < 16; ++i) {
            PieceType pt = pieces[i];
            if (pt == PieceType::None) {
                continue;
            }

            u8 sq = squares[i].raw;

            // Mirror board for White
            if (c == Color::White) {
                sq ^= 56;
            }
            if (king_side) {
                sq ^= 7;
            }

            switch (pt) {
            case PieceType::Pawn:
                psqt = psqt + (c == Color::White ? PAWN_PSQT[sq - 8] : -PAWN_PSQT[sq - 8]);
                break;
            case PieceType::Knight:
                psqt = psqt + (c == Color::White ? KNIGHT_PSQT[sq] : -KNIGHT_PSQT[sq]);
                break;
            case PieceType::Bishop:
                psqt = psqt + (c == Color::White ? BISHOP_PSQT[sq] : -BISHOP_PSQT[sq]);
                break;
            case PieceType::Rook:
                psqt = psqt + (c == Color::White ? ROOK_PSQT[sq] : -ROOK_PSQT[sq]);
                break;
            case PieceType::Queen:
                psqt = psqt + (c == Color::White ? QUEEN_PSQT[sq] : -QUEEN_PSQT[sq]);
                break;
            case PieceType::King:
                psqt = psqt + (c == Color::White ? KING_PSQT[sq] : -KING_PSQT[sq]);
                break;
            default:
                break;
            }
        }
    }

    PScore mobility = MOBILITY_VAL * mob_count;

    PScore tempo = (us == Color::White) ? TEMPO_VAL : -TEMPO_VAL;
    PScore sum   = material + mobility + tempo + psqt;
#ifdef EVAL_TUNING
    return sum->phase<24.0>(static_cast<f64>(phase));
#else
    return sum.phase<24>(phase);
#endif
};

Score evaluate_stm_pov(Position pos) {
    const Color us = pos.active_color();
    return (us == Color::White) ? evaluate_white_pov(pos) : -evaluate_white_pov(pos);
}

}  // namespace Clockwork
