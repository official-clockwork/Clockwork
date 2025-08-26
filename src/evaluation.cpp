#include <ranges>

#include "common.hpp"
#include "position.hpp"

#include "evaluation.hpp"

#include "eval_types.hpp"
#include "square.hpp"

namespace Clockwork {

// clang-format off
const PScore PAWN_MAT     = S(199, 432);
const PScore KNIGHT_MAT   = S(843, 842);
const PScore BISHOP_MAT   = S(934, 870);
const PScore ROOK_MAT     = S(1087, 1553);
const PScore QUEEN_MAT    = S(2797, 2696);
const PScore MOBILITY_VAL = S(15, 13);
const PScore TEMPO_VAL    = S(20, 5);

const std::array<PScore, 48> PAWN_PSQT = {
    S(287,303),     S(170,364),     S(339,261),     S(227,245),     S(259,261),     S(113,318),     S(54,381),      S(149,321),
    S(-51,168),     S(25,167),      S(85,123),      S(126,101),     S(182,93),      S(103,101),     S(59,165),      S(-25,127),
    S(-45,38),      S(8,18),        S(-2,-19),      S(75,-43),      S(64,-49),      S(27,-40),      S(40,-7),       S(-46,22),
    S(-84,-4),      S(-8,-22),      S(-11,-77),     S(46,-82),      S(46,-83),      S(7,-79),       S(20,-24),      S(-72,-21),
    S(-70,-50),     S(-20,-58),     S(-32,-88),     S(13,-72),      S(24,-80),      S(25,-89),      S(29,-65),      S(-66,-54),
    S(-85,-26),     S(-34,-49),     S(-45,-59),     S(-23,-77),     S(9,-76),       S(7,-66),       S(16,-52),      S(-77,-40),
};

const std::array<PScore, 64> KNIGHT_PSQT = {
    S(-309,-112),   S(-316,116),    S(25,28),       S(47,69),       S(253,-40),     S(-277,115),    S(-63,4),       S(-256,-252),
    S(-20,-37),     S(47,41),       S(71,30),       S(169,51),      S(85,48),       S(160,2),       S(57,24),       S(67,-37),
    S(4,39),        S(98,49),       S(140,54),      S(224,2),       S(245,0),       S(216,25),      S(136,24),      S(65,37),
    S(51,11),       S(40,44),       S(72,66),       S(85,74),       S(43,106),      S(134,41),      S(35,55),       S(90,29),
    S(-13,18),      S(24,25),       S(44,52),       S(24,62),       S(42,50),       S(28,60),       S(62,19),       S(22,2),
    S(-67,-57),     S(-36,13),      S(-36,-8),      S(35,28),       S(42,23),       S(-27,-15),     S(-7,2),        S(-59,-61),
    S(-81,-86),     S(-23,1),       S(-53,-12),     S(2,-23),       S(9,-20),       S(4,-43),       S(-39,-1),      S(-104,-22),
    S(-222,-61),    S(-30,-83),     S(-75,-52),     S(-51,-22),     S(-29,-28),     S(-31,-85),     S(-19,-84),     S(-182,-57),
};

const std::array<PScore, 64> BISHOP_PSQT = {
    S(-80,109),     S(-88,108),     S(-102,69),     S(-95,59),      S(-96,88),      S(-125,66),     S(-26,69),      S(-62,77),
    S(-83,12),      S(-45,81),      S(30,41),       S(43,39),       S(-1,45),       S(-13,70),      S(-33,61),      S(-97,42),
    S(-24,46),      S(74,37),       S(38,46),       S(160,0),       S(116,6),       S(133,34),      S(55,52),       S(83,-12),
    S(3,-2),        S(0,49),        S(64,24),       S(92,17),       S(88,19),       S(64,34),       S(13,57),       S(41,17),
    S(4,23),        S(42,-2),       S(-27,35),      S(64,11),       S(37,6),        S(-11,31),      S(37,9),        S(29,-42),
    S(12,-29),      S(-8,-8),       S(12,6),        S(-15,27),      S(-7,21),       S(2,10),        S(-4,-16),      S(-7,-12),
    S(-9,-48),      S(7,-27),       S(-3,-19),      S(-12,-4),      S(-12,-4),      S(0,-22),       S(4,-11),       S(0,-51),
    S(-99,0),       S(-63,-2),      S(0,-48),       S(-61,-10),     S(-99,-9),      S(-38,-31),     S(-10,-41),     S(-75,11),
};

const std::array<PScore, 64> ROOK_PSQT = {
    S(263,-5),      S(307,-12),     S(291,-8),      S(245,4),       S(237,18),      S(311,7),       S(257,9),       S(391,-38),
    S(226,13),      S(153,52),      S(286,14),      S(347,7),       S(326,6),       S(295,4),       S(176,43),      S(350,-33),
    S(120,47),      S(186,23),      S(213,37),      S(277,18),      S(351,-30),     S(312,-9),      S(316,-30),     S(222,1),
    S(28,35),       S(26,54),       S(114,35),      S(111,19),      S(142,1),       S(122,25),      S(85,25),       S(104,5),
    S(-72,41),      S(-54,44),      S(-23,48),      S(8,19),        S(9,17),        S(0,29),        S(-4,24),       S(-20,9),
    S(-131,12),     S(-86,12),      S(-60,5),       S(-30,3),       S(-11,-11),     S(-40,0),       S(-35,-20),     S(-81,-8),
    S(-134,7),      S(-97,6),       S(-45,-4),      S(1,-26),       S(-15,-18),     S(-40,-10),     S(-50,-15),     S(-127,-2),
    S(-88,12),      S(-56,7),       S(-15,0),       S(11,-14),      S(31,-30),      S(-28,2),       S(-32,-13),     S(-81,-5),
};

const std::array<PScore, 64> QUEEN_PSQT = {
    S(52,58),       S(212,9),       S(178,74),      S(151,146),     S(283,46),      S(344,17),      S(246,37),      S(184,63),
    S(28,49),       S(-82,163),     S(-29,236),     S(70,207),      S(64,241),      S(134,223),     S(15,191),      S(143,93),
    S(-22,89),      S(10,119),      S(6,125),       S(117,104),     S(122,160),     S(161,130),     S(159,130),     S(132,135),
    S(-38,91),      S(-59,133),     S(-27,170),     S(-46,227),     S(-5,227),      S(34,175),      S(-5,197),      S(31,169),
    S(-30,-11),     S(-48,65),      S(-72,91),      S(-55,151),     S(-42,126),     S(-35,103),     S(1,84),        S(-26,130),
    S(-28,-23),     S(-25,-112),    S(-41,-29),     S(-42,-37),     S(-22,-33),     S(-32,11),      S(-3,-7),       S(0,-58),
    S(-22,-111),    S(-24,-166),    S(13,-225),     S(5,-178),      S(21,-209),     S(22,-169),     S(20,-240),     S(39,-94),
    S(-42,-67),     S(4,-259),      S(10,-262),     S(34,-257),     S(24,-301),     S(-24,-216),    S(24,-259),     S(-39,-81),
};

const std::array<PScore, 64> KING_PSQT = {
    S(803,-293),    S(438,-34),     S(361,-29),     S(530,-25),     S(457,59),      S(340,-8),      S(295,-35),     S(60,-124),
    S(-16,-16),     S(-28,85),      S(-26,79),      S(86,39),       S(-38,83),      S(6,93),        S(-117,132),    S(20,20),
    S(-204,130),    S(-155,150),    S(-140,136),    S(-141,106),    S(-204,132),    S(-108,165),    S(-111,169),    S(-88,109),
    S(-239,85),     S(-111,112),    S(-227,119),    S(-187,100),    S(-249,108),    S(-177,129),    S(-129,117),    S(-338,104),
    S(-130,27),     S(-97,28),      S(-188,62),     S(-186,77),     S(-222,78),     S(-258,88),     S(-166,58),     S(-270,68),
    S(-70,-32),     S(-60,-31),     S(-178,14),     S(-214,48),     S(-202,44),     S(-182,29),     S(-73,-19),     S(-78,-36),
    S(7,-83),       S(0,-84),       S(-61,-49),     S(-110,-20),    S(-96,-19),     S(-25,-54),     S(24,-87),      S(80,-112),
    S(-28,-64),     S(111,-136),    S(66,-102),     S(-65,-78),     S(27,-94),      S(7,-99),       S(174,-165),    S(144,-201),
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
