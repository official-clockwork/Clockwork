#include <ranges>

#include "common.hpp"
#include "position.hpp"
#include "psqt_state.hpp"

#include "evaluation.hpp"

#include "eval_types.hpp"
#include "square.hpp"

namespace Clockwork {

// clang-format off
const PScore PAWN_MAT     = S(265,462);
const PScore KNIGHT_MAT   = S(748,950);
const PScore BISHOP_MAT   = S(764,994);
const PScore ROOK_MAT     = S(1176,1691);
const PScore QUEEN_MAT    = S(2568,3065);
const PScore MOBILITY_VAL = S(21,7);
const PScore TEMPO_VAL    = S(30,13);

const PScore BISHOP_PAIR_VAL = S(54,208);
const PScore DOUBLED_PAWN_VAL = S(-72,-117);

const std::array<PScore, 48> PAWN_PSQT = {
    S(-170,511),    S(0,465),       S(76,493),      S(248,248),     S(209,255),     S(281,330),     S(119,379),     S(207,375),
    S(-23,155),     S(111,207),     S(66,97),       S(181,10),      S(113,16),      S(25,80),       S(-56,137),     S(-108,110),
    S(-38,-15),     S(-1,-25),      S(-29,-69),     S(-16,-109),    S(-30,-88),     S(-88,-90),     S(-119,-20),    S(-164,16),
    S(-29,-114),    S(32,-93),      S(-38,-125),    S(-50,-119),    S(-50,-145),    S(-113,-128),   S(-148,-52),    S(-200,-47),
    S(22,-154),     S(112,-145),    S(8,-110),      S(-66,-93),     S(-76,-111),    S(-132,-115),   S(-145,-72),    S(-194,-71),
    S(-24,-145),    S(172,-162),    S(49,-111),     S(-95,-59),     S(-148,-121),   S(-156,-98),    S(-173,-49),    S(-207,-55),
};

const std::array<PScore, 64> KNIGHT_PSQT = {
    S(-380,-221),   S(-29,-96),     S(-457,241),    S(113,42),      S(-163,124),    S(-222,157),    S(-205,-42),    S(-475,-48),
    S(76,-96),      S(89,81),       S(269,-22),     S(128,59),      S(204,71),      S(152,-32),     S(15,-24),      S(68,-95),
    S(110,7),       S(162,118),     S(317,61),      S(260,31),      S(220,54),      S(120,54),      S(48,105),      S(-63,19),
    S(177,-15),     S(101,89),      S(196,67),      S(105,101),     S(140,97),      S(72,91),       S(2,71),        S(-8,43),
    S(9,0),         S(139,49),      S(46,80),       S(49,80),       S(4,112),       S(11,98),       S(-27,86),      S(-60,-53),
    S(-67,-37),     S(11,-4),       S(-18,-1),      S(25,52),       S(33,44),       S(-92,40),      S(-58,8),       S(-114,-37),
    S(-19,-10),     S(-3,-1),       S(-33,9),       S(-39,24),      S(-41,-7),      S(-104,14),     S(-116,-25),    S(-169,-123),
    S(-234,-31),    S(-49,-110),    S(-32,-75),     S(-17,-45),     S(-46,-50),     S(-54,-137),    S(-120,-74),    S(-243,-66),
};

const std::array<PScore, 64> BISHOP_PSQT = {
    S(-175,94),     S(-76,35),      S(-493,124),    S(-237,101),    S(-349,161),    S(-435,132),    S(-72,115),     S(-7,67),
    S(-44,-30),     S(-38,104),     S(35,47),       S(33,20),       S(-47,86),      S(-24,75),      S(-15,48),      S(-135,44),
    S(72,43),       S(86,66),       S(272,26),      S(120,40),      S(70,58),       S(1,84),        S(98,29),       S(20,43),
    S(84,-38),      S(-5,74),       S(114,18),      S(94,42),       S(135,18),      S(38,58),       S(0,58),        S(-90,105),
    S(-3,-27),      S(12,27),       S(0,39),        S(57,34),       S(31,72),       S(-18,55),      S(-16,24),      S(-40,-36),
    S(57,-32),      S(28,-13),      S(69,-20),      S(-19,49),      S(-26,61),      S(-40,70),      S(7,-24),       S(17,-50),
    S(-17,-99),     S(103,-54),     S(49,-36),      S(-7,-4),       S(-27,-23),     S(6,-19),       S(-37,0),       S(23,-164),
    S(-52,5),       S(-64,14),      S(-74,22),      S(-37,-47),     S(-55,-52),     S(-30,-51),     S(-36,-78),     S(-87,-21),
};

const std::array<PScore, 64> ROOK_PSQT = {
    S(223,-24),     S(371,-26),     S(102,63),      S(131,43),      S(204,-9),      S(69,46),       S(136,22),      S(125,34),
    S(275,-16),     S(185,51),      S(345,-9),      S(187,70),      S(199,62),      S(85,95),       S(51,86),       S(91,73),
    S(85,49),       S(298,-15),     S(219,9),       S(188,13),      S(89,39),       S(23,95),       S(41,62),       S(-35,84),
    S(-6,58),       S(13,43),       S(42,46),       S(55,3),        S(16,23),       S(-50,113),     S(-56,73),      S(-139,95),
    S(-126,-26),    S(8,-44),       S(-78,38),      S(-76,22),      S(-117,32),     S(-152,87),     S(-189,84),     S(-193,64),
    S(-123,-51),    S(15,-120),     S(-79,-30),     S(-107,-13),    S(-94,-35),     S(-173,38),     S(-185,19),     S(-199,18),
    S(-143,-1),     S(-46,-96),     S(-46,-68),     S(-81,-37),     S(-90,-35),     S(-122,-10),    S(-178,-23),    S(-197,-33),
    S(-137,-14),    S(-73,9),       S(-67,-11),     S(-37,-28),     S(-68,-3),      S(-79,-12),     S(-97,-32),     S(-134,-19),
};

const std::array<PScore, 64> QUEEN_PSQT = {
    S(175,52),      S(157,42),      S(168,165),     S(164,130),     S(141,102),     S(-5,224),      S(72,83),       S(-1,62),
    S(168,108),     S(-69,301),     S(107,251),     S(-77,401),     S(-52,296),     S(-123,297),    S(-93,181),     S(-2,93),
    S(159,158),     S(218,112),     S(219,173),     S(71,232),      S(-63,281),     S(-78,220),     S(-15,97),      S(25,11),
    S(73,142),      S(0,216),       S(-26,203),     S(-72,253),     S(-81,190),     S(-51,88),      S(-61,62),      S(-47,73),
    S(-21,125),     S(15,82),       S(-89,208),     S(-86,140),     S(-127,150),    S(-126,107),    S(-57,-39),     S(-77,-13),
    S(15,-101),     S(-9,-17),      S(-41,47),      S(-106,58),     S(-75,-25),     S(-76,-50),     S(-57,-70),     S(-63,-19),
    S(34,-224),     S(24,-295),     S(-7,-155),     S(6,-143),      S(-51,-90),     S(-10,-194),    S(-47,-111),    S(-78,-22),
    S(-70,-157),    S(-36,-400),    S(-47,-251),    S(-4,-270),     S(46,-234),     S(28,-265),     S(-47,-118),    S(-100,0),
};

const std::array<PScore, 64> KING_PSQT = {
    S(586,-528),    S(359,-90),     S(-334,161),    S(-431,161),    S(0,0), S(0,0), S(0,0), S(0,0),
    S(230,-32),     S(-214,151),    S(-188,161),    S(-93,121),     S(0,0), S(0,0), S(0,0), S(0,0),
    S(-176,128),    S(-138,125),    S(-54,104),     S(-87,55),      S(0,0), S(0,0), S(0,0), S(0,0),
    S(-321,124),    S(-253,150),    S(-117,102),    S(-205,82),     S(0,0), S(0,0), S(0,0), S(0,0),
    S(-247,44),     S(-251,83),     S(-209,90),     S(-366,143),    S(0,0), S(0,0), S(0,0), S(0,0),
    S(-143,-17),    S(-104,-10),    S(-262,64),     S(-370,128),    S(0,0), S(0,0), S(0,0), S(0,0),
    S(144,-122),    S(64,-105),     S(-81,-33),     S(-229,32),     S(0,0), S(0,0), S(0,0), S(0,0),
    S(220,-242),    S(189,-184),    S(12,-127),     S(36,-177),     S(0,0), S(0,0), S(0,0), S(0,0),
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
    for (PieceId id : pos.get_piece_mask(Color::White)) {
        mob_count += pos.mobility_of(Color::White, id);
    }
    for (PieceId id : pos.get_piece_mask(Color::Black)) {
        mob_count -= pos.mobility_of(Color::Black, id);
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
