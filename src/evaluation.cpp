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
const PScore PAWN_MAT   = S(288,529);
const PScore KNIGHT_MAT = S(848,1002);
const PScore BISHOP_MAT = S(869,1099);
const PScore ROOK_MAT   = S(1287,1797);
const PScore QUEEN_MAT  = S(2800,3285);
const PScore TEMPO_VAL  = S(28,15);

const PScore BISHOP_PAIR_VAL  = S(54,216);
const PScore DOUBLED_PAWN_VAL = S(-72,-117);

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
    S(-165,443),    S(50,402),      S(118,434),     S(254,212),     S(193,226),     S(249,317),     S(102,347),     S(182,326),     //
    S(-29,101),     S(137,158),     S(92,51),       S(187,-15),     S(108,-6),      S(12,48),       S(-45,95),      S(-94,52),      //
    S(-71,-59),     S(-37,-52),     S(-41,-103),    S(-38,-142),    S(-50,-122),    S(-96,-125),    S(-128,-56),    S(-162,-37),    //
    S(-57,-164),    S(0,-128),      S(-44,-172),    S(-63,-162),    S(-61,-185),    S(-108,-169),   S(-151,-90),    S(-186,-107),   //
    S(-20,-201),    S(86,-194),     S(-8,-162),     S(-61,-143),    S(-81,-149),    S(-131,-154),   S(-135,-114),   S(-185,-127),   //
    S(-69,-199),    S(134,-210),    S(36,-173),     S(-76,-107),    S(-94,-157),    S(-135,-144),   S(-143,-99),    S(-187,-122),   //
};
const std::array<PScore, 64> KNIGHT_PSQT = {
    S(-330,-160),   S(5,-122),      S(-531,265),    S(11,62),       S(-245,142),    S(-286,160),    S(-253,-39),    S(-409,-4),     //
    S(65,-64),      S(57,84),       S(216,-32),     S(67,55),       S(147,62),      S(133,-49),     S(-8,-16),      S(65,-78),      //
    S(24,53),       S(63,127),      S(234,101),     S(145,80),      S(131,87),      S(63,67),       S(11,77),       S(-92,42),      //
    S(114,27),      S(12,113),      S(118,97),      S(45,121),      S(125,89),      S(52,85),       S(-20,39),      S(-12,55),      //
    S(51,34),       S(149,30),      S(63,64),       S(56,68),       S(21,90),       S(20,77),       S(-20,59),      S(-20,-40),     //
    S(-43,-2),      S(-6,-24),      S(-33,-13),     S(4,53),        S(8,43),        S(-88,15),      S(-69,-16),     S(-100,-19),    //
    S(-1,31),       S(11,20),       S(-45,-2),      S(-45,13),      S(-50,-11),     S(-104,-8),     S(-86,-29),     S(-139,-106),   //
    S(-117,3),      S(-27,-52),     S(6,-78),       S(23,-34),      S(8,-43),       S(-42,-128),    S(-89,-15),     S(-229,-5),     //
};
const std::array<PScore, 64> BISHOP_PSQT = {
    S(-135,58),     S(-70,12),      S(-576,118),    S(-282,87),     S(-345,134),    S(-455,114),    S(-53,88),      S(10,65),       //
    S(-56,-15),     S(-48,86),      S(15,34),       S(12,5),        S(-11,52),      S(-8,43),       S(0,24),        S(-118,40),     //
    S(-29,80),      S(28,70),       S(210,46),      S(79,50),       S(43,58),       S(-5,81),       S(92,13),       S(-9,46),       //
    S(38,-28),      S(16,48),       S(96,21),       S(71,66),       S(120,36),      S(26,55),       S(20,36),       S(-98,89),      //
    S(36,-48),      S(21,16),       S(33,23),       S(57,58),       S(35,87),       S(6,41),        S(-19,8),       S(-19,-53),     //
    S(60,-23),      S(83,-17),      S(85,-17),      S(15,46),       S(-3,58),       S(-29,67),      S(48,-24),      S(16,-51),      //
    S(-10,-48),     S(107,-48),     S(57,-38),      S(2,4),         S(-12,-15),     S(23,-19),      S(-20,-7),      S(51,-137),     //
    S(0,53),        S(-14,19),      S(-18,34),      S(7,-52),       S(-32,-48),     S(8,-7),        S(3,-59),       S(-43,9),       //
};
const std::array<PScore, 64> ROOK_PSQT = {
    S(112,9),       S(302,-12),     S(11,85),       S(26,85),       S(88,45),       S(0,77),        S(65,52),       S(65,59),       //
    S(157,30),      S(126,69),      S(241,30),      S(94,115),      S(118,97),      S(13,127),      S(-17,118),     S(18,105),      //
    S(36,66),       S(262,4),       S(207,27),      S(202,20),      S(126,33),      S(38,98),       S(64,61),       S(-38,86),      //
    S(-18,62),      S(19,55),       S(41,61),       S(113,-3),      S(60,18),       S(-10,112),     S(-14,71),      S(-110,88),     //
    S(-106,-25),    S(28,-38),      S(-65,52),      S(-43,25),      S(-72,30),      S(-103,83),     S(-135,73),     S(-148,52),     //
    S(-115,-47),    S(25,-113),     S(-65,-23),     S(-79,-12),     S(-60,-39),     S(-136,36),     S(-144,12),     S(-166,11),     //
    S(-146,-1),     S(-47,-95),     S(-36,-71),     S(-51,-46),     S(-56,-43),     S(-95,-15),     S(-146,-33),    S(-165,-42),    //
    S(-127,-28),    S(-85,-2),      S(-56,-34),     S(-30,-46),     S(-57,-20),     S(-67,-30),     S(-86,-46),     S(-115,-37),    //
};
const std::array<PScore, 64> QUEEN_PSQT = {
    S(51,0),        S(54,-15),      S(3,145),       S(-23,120),     S(-39,123),     S(-98,205),     S(-15,66),      S(-61,8),       //
    S(82,67),       S(-142,295),    S(-4,259),      S(-195,411),    S(-90,269),     S(-171,298),    S(-109,146),    S(-41,66),      //
    S(-61,211),     S(72,172),      S(77,230),      S(35,240),      S(-61,287),     S(-64,229),     S(-17,101),     S(-23,28),      //
    S(42,77),       S(43,142),      S(5,162),       S(-30,268),     S(-40,241),     S(-5,98),       S(-3,47),       S(-35,57),      //
    S(13,81),       S(73,36),       S(-16,188),     S(-17,165),     S(-60,184),     S(-45,112),     S(3,-58),       S(-35,-28),     //
    S(49,-130),     S(46,-31),      S(20,42),       S(-45,92),      S(-14,6),       S(-18,-26),     S(-1,-70),      S(-27,-34),     //
    S(37,-246),     S(54,-315),     S(15,-148),     S(33,-97),      S(-19,-48),     S(21,-162),     S(-12,-103),    S(-62,-24),     //
    S(-136,-40),    S(-7,-448),     S(-31,-266),    S(-8,-226),     S(23,-155),     S(37,-230),     S(-22,-125),    S(-88,-24),     //
};
const std::array<PScore, 64> KING_PSQT = {
    S(479,-614),    S(422,-181),    S(-506,163),    S(-520,154),    S(0,0),         S(0,0),         S(0,0),         S(0,0),         //
    S(200,-72),     S(-155,199),    S(-135,225),    S(-31,200),     S(0,0),         S(0,0),         S(0,0),         S(0,0),         //
    S(-150,64),     S(11,149),      S(53,135),      S(-28,110),     S(0,0),         S(0,0),         S(0,0),         S(0,0),         //
    S(-297,65),     S(-137,164),    S(17,120),      S(-79,105),     S(0,0),         S(0,0),         S(0,0),         S(0,0),         //
    S(-259,-1),     S(-107,83),     S(-26,77),      S(-202,141),    S(0,0),         S(0,0),         S(0,0),         S(0,0),         //
    S(-175,-52),    S(37,-15),      S(-77,48),      S(-170,117),    S(0,0),         S(0,0),         S(0,0),         S(0,0),         //
    S(100,-137),    S(156,-68),     S(29,0),        S(-103,70),     S(0,0),         S(0,0),         S(0,0),         S(0,0),         //
    S(30,-240),     S(124,-177),    S(-18,-124),    S(11,-159),     S(0,0),         S(0,0),         S(0,0),         S(0,0),         //
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

template<Color color>
PScore evaluate_pawns(const Position& pos) {
    Bitboard pawns = pos.board().bitboard_for(color, PieceType::Pawn);
    PScore eval = PSCORE_ZERO;
    eval += DOUBLED_PAWN_VAL * (pawns & pawns.shift(Direction::North)).popcount();

    return eval;
}

template<Color color>
PScore evaluate_pieces(const Position& pos) {
    constexpr Color opp = ~color;
    PScore eval = PSCORE_ZERO;
    Bitboard bb = pos.bitboard_for(color, PieceType::Pawn) | pos.attacked_by(opp, PieceType::Pawn);
    Bitboard opp_king_ring = king_ring_table[pos.king_sq(opp).raw];
    for (PieceId id : pos.get_piece_mask(color, PieceType::Knight)) {
        eval += KNIGHT_MOBILITY[pos.mobility_of(color, id, ~bb)];
        eval += KNIGHT_KING_RING[pos.mobility_of(color, id, opp_king_ring)];
    }
    for (PieceId id : pos.get_piece_mask(color, PieceType::Bishop)) {
        eval += BISHOP_MOBILITY[pos.mobility_of(color, id, ~bb)];
        eval += BISHOP_KING_RING[pos.mobility_of(color, id, opp_king_ring)];
    }
    for (PieceId id : pos.get_piece_mask(color, PieceType::Rook)) {
        eval += ROOK_MOBILITY[pos.mobility_of(color, id, ~bb)];
        eval += ROOK_KING_RING[pos.mobility_of(color, id, opp_king_ring)];
    }
    for (PieceId id : pos.get_piece_mask(color, PieceType::Queen)) {
        eval += QUEEN_MOBILITY[pos.mobility_of(color, id, ~bb)];
        eval += QUEEN_KING_RING[pos.mobility_of(color, id, opp_king_ring)];
    }
    eval += KING_MOBILITY[pos.mobility_of(color, PieceId::king(), ~bb)];

    if (pos.piece_count(color, PieceType::Bishop) >= 2) {
        eval += BISHOP_PAIR_VAL;
    }

    return eval;
}

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

    PScore mobility    = PSCORE_ZERO;
    PScore king_attack = PSCORE_ZERO;

    PScore eval = psqt_state.score();
    eval += evaluate_pieces<Color::White>(pos) - evaluate_pieces<Color::Black>(pos);
    eval += evaluate_pawns<Color::White>(pos) - evaluate_pawns<Color::Black>(pos);
    eval += (us == Color::White) ? TEMPO_VAL : -TEMPO_VAL;
    return eval->phase<24>(phase);
};

Score evaluate_stm_pov(const Position& pos, const PsqtState& psqt_state) {
    const Color us = pos.active_color();
    return (us == Color::White) ? evaluate_white_pov(pos, psqt_state)
                                : -evaluate_white_pov(pos, psqt_state);
}

}  // namespace Clockwork
