#include <ranges>

#include "common.hpp"
#include "position.hpp"
#include "psqt_state.hpp"

#include "evaluation.hpp"

#include "eval_types.hpp"
#include "square.hpp"

namespace Clockwork {

// clang-format off
const PScore PAWN_MAT     = S(296,430);
const PScore KNIGHT_MAT   = S(850,736);
const PScore BISHOP_MAT   = S(872,782);
const PScore ROOK_MAT     = S(1302,1385);
const PScore QUEEN_MAT    = S(3608,1859);
const PScore MOBILITY_VAL = S(16,8);
const PScore TEMPO_VAL    = S(23,5);

const PScore BISHOP_PAIR_VAL = S(76,172);
const PScore DOUBLED_PAWN_VAL = S(-68,-124);

const std::array<PScore, 48> PAWN_PSQT = {
    S(55,296),      S(9,400),       S(305,295),     S(270,207),     S(270,149),     S(368,189),     S(193,281),     S(238,274),
    S(2,122),       S(150,124),     S(98,63),       S(94,42),       S(73,27),       S(51,35),       S(-22,89),      S(-133,121),
    S(-43,-20),     S(7,-30),       S(-15,-73),     S(-5,-100),     S(-17,-95),     S(-81,-78),     S(-95,-29),     S(-158,12),
    S(-50,-85),     S(27,-87),      S(-30,-121),    S(-25,-128),    S(-37,-134),    S(-92,-116),    S(-112,-63),    S(-182,-49),
    S(-2,-119),     S(93,-126),     S(-17,-100),    S(-63,-106),    S(-75,-113),    S(-120,-110),   S(-124,-75),    S(-176,-71),
    S(-37,-112),    S(124,-124),    S(12,-96),      S(-104,-77),    S(-135,-115),   S(-144,-91),    S(-144,-55),    S(-193,-47),
};

const std::array<PScore, 64> KNIGHT_PSQT = {
    S(-398,-36),    S(-108,24),     S(-351,148),    S(139,-10),     S(-84,84),      S(-131,92),     S(-266,38),     S(-482,34),
    S(10,-68),      S(34,46),       S(235,-26),     S(122,64),      S(168,74),      S(95,-2),       S(26,2),        S(-1,-48),
    S(62,40),       S(192,58),      S(265,40),      S(255,18),      S(193,38),      S(127,48),      S(100,22),      S(16,-8),
    S(138,-2),      S(63,64),       S(145,54),      S(80,74),       S(85,86),       S(78,72),       S(33,36),       S(28,-12),
    S(19,-12),      S(78,18),       S(30,58),       S(59,58),       S(13,76),       S(38,66),       S(36,14),       S(-24,-46),
    S(-60,-60),     S(28,-22),      S(7,-34),       S(34,26),       S(31,28),       S(-49,-4),      S(-37,-18),     S(-79,-86),
    S(-10,-50),     S(-27,0),       S(-8,-30),      S(-11,-18),     S(-4,-38),      S(-38,-56),     S(-71,-32),     S(-90,-152),
    S(-205,24),     S(-42,-142),    S(-55,-76),     S(-5,-74),      S(-70,-52),     S(-77,-116),    S(-62,-160),    S(-248,-128),
};

const std::array<PScore, 64> BISHOP_PSQT = {
    S(-104,40),     S(-80,42),      S(-91,14),      S(-128,34),     S(-176,60),     S(-248,76),     S(-129,90),     S(-65,66),
    S(-63,-2),      S(-82,66),      S(24,30),       S(31,22),       S(-79,74),      S(-7,44),       S(-14,32),      S(-73,16),
    S(44,40),       S(158,20),      S(273,-12),     S(143,-2),      S(100,-2),      S(71,28),       S(92,24),       S(25,2),
    S(32,-34),      S(9,36),        S(97,6),        S(84,12),       S(109,6),       S(50,22),       S(-6,54),       S(-18,6),
    S(32,-46),      S(39,-6),       S(-12,36),      S(64,0),        S(49,30),       S(-22,50),      S(10,-2),       S(4,-44),
    S(47,-30),      S(28,-14),      S(37,-26),      S(-8,26),       S(-8,28),       S(-14,24),      S(20,-38),      S(-10,-40),
    S(26,-98),      S(83,-52),      S(47,-48),      S(-2,-24),      S(-32,-26),     S(25,-56),      S(-3,-50),      S(38,-114),
    S(-25,-76),     S(-140,22),     S(-67,-36),     S(-49,-42),     S(-60,-54),     S(-22,-96),     S(-20,-70),     S(-58,-38),
};

const std::array<PScore, 64> ROOK_PSQT = {
    S(312,-49),     S(281,-9),      S(87,77),       S(118,56),      S(172,5),       S(181,-8),      S(145,-1),      S(117,13),
    S(222,-1),      S(167,25),      S(270,15),      S(194,45),      S(240,5),       S(150,25),      S(76,32),       S(79,34),
    S(46,64),       S(176,32),      S(199,30),      S(161,31),      S(99,23),       S(32,49),       S(75,27),       S(-15,64),
    S(-33,40),      S(-5,40),       S(8,46),        S(26,12),       S(24,15),       S(-34,46),      S(-43,32),      S(-80,35),
    S(-158,9),      S(-60,-4),      S(-105,33),     S(-81,17),      S(-102,23),     S(-139,54),     S(-152,48),     S(-182,41),
    S(-158,-18),    S(-43,-61),     S(-99,-41),     S(-110,-40),    S(-110,-30),    S(-155,-3),     S(-144,-30),    S(-184,-20),
    S(-125,-41),    S(-59,-71),     S(-69,-63),     S(-89,-53),     S(-84,-58),     S(-106,-51),    S(-143,-60),    S(-189,-60),
    S(-130,-59),    S(-105,-12),    S(-65,-30),     S(-25,-67),     S(-44,-47),     S(-66,-39),     S(-84,-62),     S(-112,-49),
};

const std::array<PScore, 64> QUEEN_PSQT = {
    S(129,80),      S(142,75),      S(168,124),     S(69,191),      S(57,148),      S(-30,191),     S(-9,152),      S(-19,83),
    S(115,97),      S(-84,254),     S(88,207),      S(-63,310),     S(-58,251),     S(-69,210),     S(-112,155),    S(4,41),
    S(95,150),      S(162,154),     S(132,164),     S(35,211),      S(-51,231),     S(-53,150),     S(-9,70),       S(11,18),
    S(59,104),      S(-15,195),     S(-20,160),     S(-75,189),     S(-98,192),     S(-59,97),      S(-68,70),      S(-9,-8),
    S(-28,111),     S(8,39),        S(-65,138),     S(-53,55),      S(-90,115),     S(-85,49),      S(-63,4),       S(-42,-56),
    S(-2,-57),      S(13,-59),      S(-22,-2),      S(-52,-41),     S(-51,-84),     S(-78,-25),     S(-35,-121),    S(-26,-83),
    S(34,-303),     S(48,-387),     S(38,-262),     S(22,-201),     S(-4,-178),     S(14,-254),     S(-11,-164),    S(-5,-152),
    S(102,-558),    S(-14,-566),    S(-38,-361),    S(54,-359),     S(49,-257),     S(52,-341),     S(11,-230),     S(-30,-120),
};

const std::array<PScore, 64> KING_PSQT = {
    S(568,-472),    S(314,11),      S(165,80),      S(-155,40),     S(0,0),       S(0,0),       S(0,0),       S(0,0),
    S(339,-93),     S(49,101),      S(-20,96),      S(263,46),      S(0,0),       S(0,0),       S(0,0),       S(0,0),
    S(132,41),      S(-44,77),      S(38,72),       S(-93,65),      S(0,0),       S(0,0),       S(0,0),       S(0,0),
    S(47,9),        S(95,39),       S(64,48),       S(-33,60),      S(0,0),       S(0,0),       S(0,0),       S(0,0),
    S(-102,-4),     S(-64,19),      S(-50,46),      S(-106,66),     S(0,0),       S(0,0),       S(0,0),       S(0,0),
    S(-142,-15),    S(-80,-13),     S(-182,37),     S(-198,62),     S(0,0),       S(0,0),       S(0,0),       S(0,0),
    S(45,-79),      S(-7,-62),      S(-105,-14),    S(-190,9),      S(0,0),       S(0,0),       S(0,0),       S(0,0),
    S(116,-173),    S(108,-123),    S(-19,-87),     S(47,-148),     S(0,0),       S(0,0),       S(0,0),       S(0,0),
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
