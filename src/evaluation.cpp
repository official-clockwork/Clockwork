#include <array>
#include <ranges>

#include "bitboard.hpp"
#include "common.hpp"
#include "eval_constants.hpp"
#include "position.hpp"
#include "psqt_state.hpp"

#include "evaluation.hpp"

#include "eval_types.hpp"
#include "square.hpp"

namespace Clockwork {

// clang-format off
const PScore PAWN_MAT     = S(296,435);
const PScore KNIGHT_MAT   = S(838,740);
const PScore BISHOP_MAT   = S(861,786);
const PScore ROOK_MAT     = S(1279,1401);
const PScore QUEEN_MAT    = S(3564,1882);
const PScore MOBILITY_VAL = S(16,8);
const PScore TEMPO_VAL    = S(23,4);

const PScore BISHOP_PAIR_VAL = S(75,174);
const PScore DOUBLED_PAWN_VAL = S(-68,-124);

const std::array<PScore, 9> KNIGHT_MOBILITY = {
    S(-216,-267), S(-135,-133), S(-82,-41), S(-43,-3), S(2,15), S(25,61), S(53,64), S(87,56), S(134,-19),
};
const std::array<PScore, 14> BISHOP_MOBILITY = {
    S(-190,-287), S(-135,-186), S(-73,-142), S(-51,-81), S(-11,-43), S(6,-17), S(33,-16), S(42,-3), S(50,14), S(47,11), S(75,2), S(166,-66), S(195,-104), S(290,-164),
};
const std::array<PScore, 15> ROOK_MOBILITY = {
    S(-269,-360), S(-106,-104), S(-65,-45), S(-51,-30), S(-34,-8), S(-21,-1), S(-2,17), S(28,11), S(60,12), S(82,32), S(109,24), S(122,35), S(179,19), S(142,34), S(265,-77),
};
const std::array<PScore, 28> QUEEN_MOBILITY = {
    S(-134,0), S(-241,-179), S(-199,-493), S(-177,-263), S(-131,-325), S(-114,-243), S(-144,-42), S(-114,-69), S(-96,-44), S(-81,-27), S(-86,25), S(-77,53), S(-46,39), S(-43,51), S(-23,48), S(-28,68), S(-33,99), S(-14,67), S(-3,52), S(46,-12), S(82,-50), S(147,-124), S(136,-109), S(459,-426), S(412,-409), S(1006,-695), S(578,-406), S(1279,-960),
};
const std::array<PScore, 9> KING_MOBILITY = {
    S(429,740), S(148,437), S(30,480), S(20,507), S(-10,493), S(-40,469), S(-47,481), S(-40,451), S(57,321),
};

const std::array<PScore, 3> KNIGHT_KING_RING = {
    CS(0,0), S(89,-43), S(125,-89),
};
const std::array<PScore, 3> BISHOP_KING_RING = {
    CS(0,0), S(65,-10), S(147,-52),
};
const std::array<PScore, 5> ROOK_KING_RING = {
    CS(0,0), S(78,-57), S(176,-94), S(233,-93), S(325,-147),
};
const std::array<PScore, 6> QUEEN_KING_RING = {
    CS(0,0), S(2,50), S(95,31), S(267,-36), S(552,-117), S(801,-239),
};

const std::array<PScore, 48> PAWN_PSQT = {
    S(82,297),      S(-25,424),     S(270,305),     S(249,212),     S(273,144),     S(359,188),     S(172,292),     S(239,273),
    S(4,124),       S(127,132),     S(89,65),       S(86,43),       S(70,25),       S(45,37),       S(-29,94),      S(-136,127),
    S(-38,-22),     S(10,-34),      S(-14,-76),     S(-3,-104),     S(-16,-98),     S(-81,-79),     S(-96,-27),     S(-161,16),
    S(-41,-92),     S(33,-94),      S(-27,-126),    S(-24,-132),    S(-36,-138),    S(-92,-117),    S(-113,-63),    S(-183,-46),
    S(7,-129),      S(100,-136),    S(-15,-104),    S(-63,-108),    S(-75,-115),    S(-120,-111),   S(-126,-73),    S(-178,-70),
    S(-28,-121),    S(131,-135),    S(13,-101),     S(-104,-78),    S(-136,-118),   S(-144,-91),    S(-146,-55),    S(-194,-46),
};
const std::array<PScore, 64> KNIGHT_PSQT = {
    S(-394,-27),    S(-86,25),      S(-417,192),    S(146,-13),     S(-93,95),      S(-158,110),    S(-315,68),     S(-478,42),
    S(9,-66),       S(25,53),       S(245,-35),     S(131,61),      S(181,68),      S(96,-2),       S(27,2),        S(1,-48),
    S(60,43),       S(193,56),      S(272,34),      S(255,16),      S(192,38),      S(127,47),      S(100,21),      S(17,-8),
    S(137,0),       S(62,68),       S(143,56),      S(79,76),       S(84,89),       S(77,74),       S(33,38),       S(28,-11),
    S(19,-10),      S(77,19),       S(29,61),       S(59,60),       S(12,79),       S(37,69),       S(34,15),       S(-24,-46),
    S(-58,-60),     S(28,-22),      S(8,-31),       S(33,28),       S(30,31),       S(-48,-2),      S(-37,-15),     S(-77,-85),
    S(-8,-49),      S(-25,3),       S(-6,-28),      S(-10,-16),     S(-3,-35),      S(-38,-55),     S(-71,-28),     S(-89,-156),
    S(-202,27),     S(-41,-144),    S(-55,-74),     S(-4,-74),      S(-70,-47),     S(-77,-116),    S(-61,-161),    S(-247,-124),
};
const std::array<PScore, 64> BISHOP_PSQT = {
    S(-109,50),     S(-124,61),     S(-136,33),     S(-167,56),     S(-239,93),     S(-302,109),    S(-152,110),    S(-55,71),
    S(-65,1),       S(-99,78),      S(21,31),       S(34,18),       S(-83,82),      S(-6,46),       S(-13,32),      S(-70,17),
    S(42,40),       S(154,18),      S(287,-27),     S(148,-8),      S(101,-4),      S(70,25),       S(90,21),       S(25,2),
    S(30,-33),      S(9,37),        S(97,4),        S(84,8),        S(109,2),       S(47,22),       S(-5,55),       S(-17,5),
    S(33,-49),      S(40,-7),       S(-11,35),      S(63,-1),       S(48,29),       S(-21,50),      S(10,-3),       S(5,-46),
    S(49,-34),      S(29,-15),      S(38,-26),      S(-6,27),       S(-7,29),       S(-13,22),      S(21,-40),      S(-8,-40),
    S(28,-103),     S(84,-54),      S(48,-50),      S(-1,-24),      S(-31,-26),     S(25,-57),      S(-1,-51),      S(40,-123),
    S(-24,-79),     S(-135,23),     S(-66,-36),     S(-46,-43),     S(-59,-53),     S(-21,-98),     S(-16,-75),     S(-50,-43),
};
const std::array<PScore, 64> ROOK_PSQT = {
    S(310,-50),     S(250,-1),      S(51,87),       S(107,59),      S(163,8),       S(165,-3),      S(130,4),       S(110,15),
    S(216,-2),      S(147,28),      S(258,17),      S(186,45),      S(231,6),       S(144,25),      S(70,32),       S(75,33),
    S(44,62),       S(174,29),      S(196,28),      S(160,28),      S(98,21),       S(30,47),       S(71,26),       S(-18,63),
    S(-33,39),      S(-6,40),       S(4,46),        S(24,11),       S(21,13),       S(-35,46),      S(-45,32),      S(-82,35),
    S(-158,7),      S(-61,-4),      S(-107,32),     S(-82,16),      S(-103,22),     S(-140,53),     S(-154,48),     S(-184,41),
    S(-159,-21),    S(-42,-62),     S(-99,-41),     S(-111,-39),    S(-110,-30),    S(-157,-3),     S(-145,-29),    S(-186,-19),
    S(-125,-43),    S(-57,-73),     S(-70,-62),     S(-90,-52),     S(-85,-58),     S(-108,-50),    S(-145,-58),    S(-192,-58),
    S(-129,-60),    S(-102,-15),    S(-66,-32),     S(-25,-67),     S(-45,-47),     S(-67,-39),     S(-85,-61),     S(-112,-49),
};
const std::array<PScore, 64> QUEEN_PSQT = {
    S(125,79),      S(135,71),      S(158,125),     S(65,193),      S(51,150),      S(-33,193),     S(-14,153),     S(-22,84),
    S(110,95),      S(-88,250),     S(83,207),      S(-64,310),     S(-59,250),     S(-71,208),     S(-113,152),    S(0,39),
    S(89,151),      S(155,155),     S(127,167),     S(31,212),      S(-53,229),     S(-55,149),     S(-11,69),      S(8,18),
    S(55,104),      S(-18,195),     S(-23,161),     S(-75,188),     S(-99,189),     S(-61,95),      S(-70,70),      S(-11,-8),
    S(-29,109),     S(6,38),        S(-66,137),     S(-55,54),      S(-91,112),     S(-86,48),      S(-65,3),       S(-42,-56),
    S(-4,-58),      S(11,-60),      S(-22,-2),      S(-53,-42),     S(-53,-84),     S(-79,-25),     S(-35,-121),    S(-28,-84),
    S(33,-304),     S(48,-388),     S(36,-263),     S(20,-201),     S(-6,-178),     S(12,-252),     S(-13,-162),    S(-7,-151),
    S(104,-563),    S(-9,-574),     S(-38,-362),    S(53,-362),     S(47,-256),     S(50,-339),     S(8,-226),      S(-34,-116),
};
const std::array<PScore, 64> KING_PSQT = {
    S(1369,-722),   S(188,28),      S(-135,159),    S(-365,101),    S(0,0), S(0,0), S(0,0), S(0,0),
    S(569,-180),    S(-69,130),     S(-241,154),    S(182,52),      S(0,0), S(0,0), S(0,0), S(0,0),
    S(139,31),      S(-22,73),      S(133,47),      S(68,31),       S(0,0), S(0,0), S(0,0), S(0,0),
    S(0,16),        S(62,39),       S(99,32),       S(93,29),       S(0,0), S(0,0), S(0,0), S(0,0),
    S(-184,18),     S(-150,42),     S(-102,57),     S(-136,75),     S(0,0), S(0,0), S(0,0), S(0,0),
    S(-158,-9),     S(-96,-10),     S(-226,53),     S(-261,86),     S(0,0), S(0,0), S(0,0), S(0,0),
    S(53,-88),      S(0,-71),       S(-102,-18),    S(-196,13),     S(0,0), S(0,0), S(0,0), S(0,0),
    S(127,-186),    S(115,-133),    S(-10,-94),     S(54,-160),     S(0,0), S(0,0), S(0,0), S(0,0),
};
// clang-format on

std::array<Bitboard, 64> king_ring_table = []() {
    std::array<Bitboard, 64> king_ring_table{};
    for (u8 sq_idx = 0; sq_idx < 64; sq_idx++) {
        Bitboard sq_bb     = Bitboard::from_square(Square{sq_idx});
        Bitboard king_ring = sq_bb;
        king_ring |= sq_bb.shift(Direction::North);
        king_ring |= sq_bb.shift(Direction::South);
        king_ring |= sq_bb.shift(Direction::East);
        king_ring |= sq_bb.shift(Direction::West);
        king_ring |= sq_bb.shift(Direction::NorthEast);
        king_ring |= sq_bb.shift(Direction::SouthEast);
        king_ring |= sq_bb.shift(Direction::NorthWest);
        king_ring |= sq_bb.shift(Direction::SouthWest);
        king_ring_table[sq_idx] = king_ring;
    }
    return king_ring_table;
}();

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

    phase = std::clamp<i32>(phase, 4, 24);

    PScore mobility    = PSCORE_ZERO;
    PScore king_attack = PSCORE_ZERO;

    auto add_mobility = [&](Color c, PScore& mob_count, PScore& k_attack) {
        Bitboard bb = pos.bitboard_for(c, PieceType::Pawn) | pos.attacked_by(~c, PieceType::Pawn);
        Bitboard king_ring = king_ring_table[pos.king_sq(~c).raw];
        for (PieceId id : pos.get_piece_mask(c, PieceType::Knight)) {
            mobility += KNIGHT_MOBILITY[pos.mobility_of(c, id, ~bb)];
            k_attack += KNIGHT_KING_RING[pos.mobility_of(c, id, king_ring)];
        }
        for (PieceId id : pos.get_piece_mask(c, PieceType::Bishop)) {
            mobility += BISHOP_MOBILITY[pos.mobility_of(c, id, ~bb)];
            k_attack += BISHOP_KING_RING[pos.mobility_of(c, id, king_ring)];
        }
        for (PieceId id : pos.get_piece_mask(c, PieceType::Rook)) {
            mobility += ROOK_MOBILITY[pos.mobility_of(c, id, ~bb)];
            k_attack += ROOK_KING_RING[pos.mobility_of(c, id, king_ring)];
        }
        for (PieceId id : pos.get_piece_mask(c, PieceType::Queen)) {
            mobility += QUEEN_MOBILITY[pos.mobility_of(c, id, ~bb)];
            k_attack += QUEEN_KING_RING[pos.mobility_of(c, id, king_ring)];
        }
        mobility += KING_MOBILITY[pos.mobility_of(c, PieceId::king(), ~bb)];
    };

    add_mobility(Color::Black, mobility, king_attack);
    mobility *= -1;  // Persy trick
    king_attack *= -1;
    add_mobility(Color::White, mobility, king_attack);

    const std::array<Bitboard, 2> pawns = {pos.board().bitboard_for(Color::White, PieceType::Pawn),
                                           pos.board().bitboard_for(Color::Black, PieceType::Pawn)};

    PScore doubled_pawns_bonus = DOUBLED_PAWN_VAL
                               * ((pawns[0] & pawns[0].shift(Direction::North)).popcount()
                                  - (pawns[1] & pawns[1].shift(Direction::South)).popcount());

    PScore bishop_pair_bonus = BISHOP_PAIR_VAL
                             * ((pos.piece_count(Color::White, PieceType::Bishop) >= 2)
                                - (pos.piece_count(Color::Black, PieceType::Bishop) >= 2));

    PScore tempo = (us == Color::White) ? TEMPO_VAL : -TEMPO_VAL;
    PScore sum =
      psqt_state.score() + mobility + king_attack + tempo + bishop_pair_bonus + doubled_pawns_bonus;
    return sum->phase<24>(phase);
};

Score evaluate_stm_pov(const Position& pos, const PsqtState& psqt_state) {
    const Color us = pos.active_color();
    return (us == Color::White) ? evaluate_white_pov(pos, psqt_state)
                                : -evaluate_white_pov(pos, psqt_state);
}

}  // namespace Clockwork
