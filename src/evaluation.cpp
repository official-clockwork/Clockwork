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
const PScore PAWN_MAT     = S(264,463);
const PScore KNIGHT_MAT   = S(739,953);
const PScore BISHOP_MAT   = S(759,1002);
const PScore ROOK_MAT     = S(1168,1705);
const PScore QUEEN_MAT    = S(2555,3088);
const PScore MOBILITY_VAL = S(21,8);
const PScore TEMPO_VAL    = S(30,13);

const PScore BISHOP_PAIR_VAL = S(53,209);
const PScore DOUBLED_PAWN_VAL = S(-72,-118);

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
    S(-191,536),    S(-39,500),     S(52,519),      S(246,251),     S(206,258),     S(282,331),     S(105,394),     S(201,383),
    S(-21,158),     S(102,217),     S(66,98),       S(179,11),      S(112,17),      S(23,84),       S(-59,142),     S(-109,115),
    S(-35,-16),     S(0,-24),       S(-28,-69),     S(-15,-110),    S(-29,-88),     S(-87,-89),     S(-120,-18),    S(-166,20),
    S(-24,-118),    S(36,-96),      S(-37,-127),    S(-50,-119),    S(-49,-146),    S(-113,-127),   S(-148,-50),    S(-202,-43),
    S(28,-160),     S(116,-150),    S(9,-111),      S(-66,-93),     S(-76,-110),    S(-132,-114),   S(-146,-70),    S(-195,-68),
    S(-18,-150),    S(177,-167),    S(50,-112),     S(-96,-57),     S(-148,-119),   S(-156,-97),    S(-174,-48),    S(-208,-53),
};
const std::array<PScore, 64> KNIGHT_PSQT = {
    S(-375,-216),   S(-14,-100),    S(-469,264),    S(113,49),      S(-157,129),    S(-229,171),    S(-209,-32),    S(-472,-42),
    S(77,-90),      S(85,91),       S(280,-28),     S(135,59),      S(218,63),      S(157,-32),     S(22,-25),      S(75,-96),
    S(112,12),      S(166,120),     S(321,62),      S(264,32),      S(223,57),      S(120,60),      S(50,110),      S(-63,28),
    S(179,-12),     S(102,96),      S(197,72),      S(108,105),     S(143,102),     S(74,96),       S(4,76),        S(-7,49),
    S(10,5),        S(142,51),      S(48,85),       S(51,85),       S(5,117),       S(13,102),      S(-27,93),      S(-58,-48),
    S(-65,-33),     S(14,0),        S(-15,3),       S(27,57),       S(35,48),       S(-89,44),      S(-55,12),      S(-112,-31),
    S(-16,-6),      S(0,2),         S(-32,15),      S(-37,28),      S(-39,-3),      S(-104,23),     S(-113,-21),    S(-167,-119),
    S(-238,-19),    S(-47,-106),    S(-30,-69),     S(-16,-39),     S(-44,-45),     S(-49,-137),    S(-117,-70),    S(-231,-71),
};
const std::array<PScore, 64> BISHOP_PSQT = {
    S(-180,103),    S(-83,36),      S(-539,160),    S(-255,119),    S(-381,189),    S(-463,159),    S(-80,123),     S(0,57),
    S(-42,-37),     S(-45,108),     S(33,47),       S(30,20),       S(-46,85),      S(-23,74),      S(-16,47),      S(-136,45),
    S(70,43),       S(84,65),       S(279,15),      S(121,36),      S(69,56),       S(-4,88),       S(98,27),       S(19,42),
    S(83,-39),      S(-6,73),       S(113,16),      S(94,39),       S(135,14),      S(37,56),       S(0,56),        S(-93,108),
    S(-3,-29),      S(11,26),       S(0,37),        S(57,33),       S(31,71),       S(-19,54),      S(-17,24),      S(-39,-38),
    S(58,-37),      S(28,-16),      S(68,-21),      S(-20,48),      S(-26,60),      S(-41,69),      S(7,-27),       S(17,-51),
    S(-15,-103),    S(103,-57),     S(49,-37),      S(-8,-5),       S(-28,-24),     S(6,-21),       S(-37,0),       S(26,-172),
    S(-51,0),       S(-65,16),      S(-74,21),      S(-36,-50),     S(-56,-52),     S(-31,-52),     S(-36,-80),     S(-87,-22),
};
const std::array<PScore, 64> ROOK_PSQT = {
    S(221,-24),     S(365,-25),     S(91,69),       S(124,46),      S(200,-9),      S(63,48),       S(130,25),      S(119,36),
    S(271,-16),     S(174,56),      S(341,-9),      S(184,70),      S(194,63),      S(80,97),       S(47,88),       S(87,75),
    S(84,48),       S(296,-18),     S(218,7),       S(187,11),      S(88,37),       S(22,95),       S(39,62),       S(-37,84),
    S(-7,58),       S(12,43),       S(41,45),       S(55,1),        S(15,22),       S(-51,113),     S(-57,73),      S(-142,97),
    S(-127,-27),    S(6,-44),       S(-80,38),      S(-78,21),      S(-118,32),     S(-153,86),     S(-191,83),     S(-195,65),
    S(-123,-54),    S(14,-122),     S(-81,-31),     S(-109,-12),    S(-96,-35),     S(-175,37),     S(-187,20),     S(-202,18),
    S(-143,-4),     S(-45,-99),     S(-48,-68),     S(-83,-37),     S(-91,-36),     S(-124,-10),    S(-180,-22),    S(-199,-32),
    S(-138,-16),    S(-72,6),       S(-68,-11),     S(-39,-29),     S(-69,-3),      S(-81,-13),     S(-98,-33),     S(-135,-20),
};
const std::array<PScore, 64> QUEEN_PSQT = {
    S(169,52),      S(152,40),      S(161,165),     S(158,132),     S(134,104),     S(-11,225),     S(67,84),       S(-6,61),
    S(162,108),     S(-75,299),     S(102,252),     S(-80,399),     S(-55,295),     S(-126,296),    S(-96,179),     S(-6,92),
    S(152,160),     S(211,114),     S(214,175),     S(67,232),      S(-67,280),     S(-81,219),     S(-19,97),      S(21,9),
    S(68,143),      S(-3,217),      S(-30,203),     S(-74,251),     S(-83,188),     S(-54,86),      S(-63,60),      S(-51,73),
    S(-25,124),     S(11,81),       S(-92,206),     S(-89,138),     S(-129,148),    S(-128,105),    S(-60,-40),     S(-80,-15),
    S(12,-104),     S(-11,-19),     S(-43,45),      S(-109,57),     S(-78,-27),     S(-79,-51),     S(-60,-71),     S(-66,-20),
    S(33,-228),     S(22,-297),     S(-10,-157),    S(3,-144),      S(-55,-91),     S(-13,-195),    S(-51,-111),    S(-82,-22),
    S(-69,-164),    S(-36,-404),    S(-50,-253),    S(-7,-271),     S(43,-235),     S(24,-264),     S(-53,-115),    S(-105,3),
};
const std::array<PScore, 64> KING_PSQT = {
    S(1191,-767),   S(302,-88),     S(-502,228),    S(-532,205),    S(0,0), S(0,0), S(0,0), S(0,0),
    S(382,-98),     S(-301,183),    S(-275,194),    S(-111,123),    S(0,0), S(0,0), S(0,0), S(0,0),
    S(-138,117),    S(-136,124),    S(-47,98),      S(70,2),        S(0,0), S(0,0), S(0,0), S(0,0),
    S(-395,157),    S(-332,180),    S(-139,105),    S(-168,71),     S(0,0), S(0,0), S(0,0), S(0,0),
    S(-282,59),     S(-317,108),    S(-263,107),    S(-452,176),    S(0,0), S(0,0), S(0,0), S(0,0),
    S(-147,-19),    S(-100,-18),    S(-275,68),     S(-406,144),    S(0,0), S(0,0), S(0,0), S(0,0),
    S(152,-134),    S(73,-119),     S(-73,-44),     S(-227,27),     S(0,0), S(0,0), S(0,0), S(0,0),
    S(227,-254),    S(196,-195),    S(20,-138),     S(43,-189),     S(0,0), S(0,0), S(0,0), S(0,0),
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
