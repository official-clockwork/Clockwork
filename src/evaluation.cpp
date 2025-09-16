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
const PScore PAWN_MAT   = S(290, 529);
const PScore KNIGHT_MAT = S(850, 1006);
const PScore BISHOP_MAT = S(869, 1094);
const PScore ROOK_MAT   = S(1290, 1795);
const PScore QUEEN_MAT  = S(2797, 3278);
const PScore TEMPO_VAL  = S(28, 15);

const PScore BISHOP_PAIR_VAL  = S(53, 216);
const PScore DOUBLED_PAWN_VAL = S(-72, -117);

const std::array<PScore, 9> KNIGHT_MOBILITY = {
    S(-216, -268), S(-135, -135), S(-81, -44), S(-42, -6), S(3, 12), S(26, 58), S(54, 61), S(88, 53), S(135, -21),
};
const std::array<PScore, 14> BISHOP_MOBILITY = {
    S(-188, -284), S(-133, -183), S(-71, -139), S(-49, -78), S(-9, -40), S(8, -14), S(35, -13), S(45, 0), S(53, 17), S(49, 14), S(78, 5), S(168, -63), S(198, -101), S(293, -161),
};
const std::array<PScore, 15> ROOK_MOBILITY = {
    S(-272, -359), S(-108, -103), S(-67, -44), S(-53, -29), S(-36, -7), S(-23, 0), S(-4, 18), S(26, 12), S(58, 13), S(80, 33), S(108, 25), S(120, 36), S(178, 20), S(140, 35), S(264, -76),
};
const std::array<PScore, 28> QUEEN_MOBILITY = {
    S(-110, 12), S(-274, -75), S(-198, -472), S(-173, -255), S(-126, -320), S(-109, -239), S(-139, -38), S(-109, -65), S(-91, -39), S(-76, -22), S(-82, 30), S(-72, 57), S(-41, 44), S(-39, 56), S(-19, 52), S(-23, 73), S(-29, 104), S(-10, 72), S(0, 58), S(50, -6), S(85, -43), S(151, -118), S(137, -99), S(455, -414), S(409, -397), S(1029, -700), S(623, -421), S(1451, -1043),
};
const std::array<PScore, 9> KING_MOBILITY = {
    S(426, 752), S(151, 439), S(33, 481), S(23, 508), S(-7, 494), S(-37, 471), S(-44, 483), S(-36, 453), S(61, 322),
};

const std::array<PScore, 3> KNIGHT_KING_RING = {
    CS(0, 0), S(89, -43), S(124, -89),
};
const std::array<PScore, 3> BISHOP_KING_RING = {
    CS(0, 0), S(65, -10), S(146, -52),
};
const std::array<PScore, 5> ROOK_KING_RING = {
    CS(0, 0), S(78, -57), S(176, -93), S(233, -94), S(324, -147),
};
const std::array<PScore, 6> QUEEN_KING_RING = {
    CS(0, 0), S(3, 49), S(96, 29), S(269, -39), S(555, -121), S(815, -252),
};

const std::array<PScore, 48> PAWN_PSQT = {
    S(-166, 443),   S(49, 402),     S(116, 433),    S(252, 212),    S(191, 225),    S(247, 317),    S(100, 346),    S(180, 326),    //
    S(-31, 101),    S(135, 158),    S(90, 51),      S(185, -15),    S(106, -6),     S(10, 48),      S(-47, 95),     S(-96, 52),     //
    S(-73, -59),    S(-39, -52),    S(-43, -103),   S(-40, -142),   S(-51, -122),   S(-98, -125),   S(-130, -56),   S(-164, -37),   //
    S(-59, -164),   S(-1, -129),    S(-46, -172),   S(-65, -162),   S(-63, -186),   S(-110, -169),  S(-153, -90),   S(-188, -107),  //
    S(-22, -201),   S(84, -195),    S(-10, -163),   S(-63, -143),   S(-83, -150),   S(-133, -154),  S(-137, -115),  S(-186, -128),  //
    S(-71, -199),   S(132, -210),   S(34, -173),    S(-78, -108),   S(-96, -156),   S(-137, -144),  S(-145, -100),  S(-189, -122),  //
};
const std::array<PScore, 64> KNIGHT_PSQT = {
    S(-332, -161),  S(0, -121),     S(-539, 268),   S(7, 63),       S(-246, 140),   S(-288, 159),   S(-255, -41),   S(-411, -5),    //
    S(62, -65),     S(56, 82),      S(215, -34),    S(66, 54),      S(147, 60),     S(131, -51),    S(-9, -18),     S(63, -79),     //
    S(22, 52),      S(61, 126),     S(232, 99),     S(144, 78),     S(129, 85),     S(62, 65),      S(9, 76),       S(-93, 39),     //
    S(112, 26),     S(10, 111),     S(117, 95),     S(43, 119),     S(124, 87),     S(50, 83),      S(-22, 38),     S(-14, 54),     //
    S(49, 32),      S(147, 29),     S(61, 63),      S(54, 67),      S(19, 89),      S(18, 75),      S(-22, 58),     S(-21, -42),    //
    S(-44, -4),     S(-8, -25),     S(-34, -15),    S(2, 51),       S(6, 42),       S(-90, 13),     S(-71, -17),    S(-102, -21),   //
    S(-3, 29),      S(9, 18),       S(-47, -3),     S(-47, 12),     S(-52, -13),    S(-106, -10),   S(-87, -31),    S(-141, -106),  //
    S(-123, 6),     S(-29, -53),    S(5, -79),      S(21, -36),     S(6, -45),      S(-43, -130),   S(-90, -16),    S(-231, -5),    //
};
const std::array<PScore, 64> BISHOP_PSQT = {
    S(-139, 62),    S(-72, 14),     S(-577, 119),   S(-284, 88),    S(-346, 136),   S(-457, 117),   S(-55, 90),     S(8, 66),       //
    S(-58, -14),    S(-50, 88),     S(14, 35),      S(11, 7),       S(-11, 53),     S(-9, 44),      S(-1, 25),      S(-119, 41),    //
    S(-30, 81),     S(26, 72),      S(210, 47),     S(77, 52),      S(41, 59),      S(-7, 82),      S(91, 15),      S(-10, 47),     //
    S(37, -26),     S(14, 50),      S(95, 22),      S(70, 68),      S(118, 37),     S(24, 57),      S(18, 38),      S(-99, 90),     //
    S(34, -47),     S(20, 17),      S(31, 24),      S(55, 60),      S(34, 88),      S(5, 42),       S(-20, 10),     S(-19, -52),    //
    S(59, -22),     S(82, -16),     S(83, -15),     S(13, 47),      S(-4, 60),      S(-30, 68),     S(47, -23),     S(15, -51),     //
    S(-11, -46),    S(105, -47),    S(55, -37),     S(0, 5),        S(-13, -14),    S(22, -18),     S(-21, -6),     S(49, -136),    //
    S(0, 55),       S(-16, 22),     S(-19, 36),     S(6, -50),      S(-33, -47),    S(6, -5),       S(2, -58),      S(-44, 9),      //
};
const std::array<PScore, 64> ROOK_PSQT = {
    S(113, 8),      S(304, -13),    S(12, 84),      S(27, 85),      S(88, 46),      S(1, 76),       S(66, 52),      S(64, 59),      //
    S(157, 30),     S(127, 69),     S(242, 30),     S(95, 115),     S(119, 97),     S(14, 126),     S(-17, 118),    S(19, 105),     //
    S(36, 66),      S(263, 4),      S(207, 27),     S(202, 20),     S(125, 33),     S(38, 98),      S(64, 61),      S(-38, 86),     //
    S(-17, 62),     S(20, 55),      S(42, 60),      S(113, -3),     S(60, 18),      S(-10, 112),    S(-13, 71),     S(-110, 88),    //
    S(-105, -25),   S(28, -38),     S(-65, 52),     S(-42, 24),     S(-71, 29),     S(-103, 83),    S(-135, 73),    S(-147, 52),    //
    S(-115, -48),   S(26, -113),    S(-65, -23),    S(-79, -12),    S(-59, -39),    S(-135, 36),    S(-144, 11),    S(-165, 11),    //
    S(-145, -1),    S(-46, -95),    S(-35, -71),    S(-51, -46),    S(-56, -43),    S(-95, -15),    S(-146, -33),   S(-165, -42),   //
    S(-127, -28),   S(-85, -3),     S(-56, -34),    S(-30, -46),    S(-57, -20),    S(-67, -30),    S(-85, -46),    S(-114, -37),   //
};
const std::array<PScore, 64> QUEEN_PSQT = {
    S(49, 2),       S(52, -12),     S(-4, 154),     S(-34, 132),    S(-48, 132),    S(-102, 210),   S(-19, 71),     S(-62, 9),      //
    S(80, 70),      S(-143, 297),   S(-6, 262),     S(-196, 413),   S(-92, 272),    S(-172, 300),   S(-109, 146),   S(-41, 67),     //
    S(-63, 215),    S(70, 176),     S(76, 233),     S(34, 242),     S(-62, 289),    S(-64, 230),    S(-18, 102),    S(-24, 30),     //
    S(42, 79),      S(42, 143),     S(4, 164),      S(-30, 270),    S(-40, 242),    S(-5, 98),      S(-3, 48),      S(-35, 58),     //
    S(13, 81),      S(72, 37),      S(-16, 189),    S(-17, 166),    S(-60, 183),    S(-45, 113),    S(3, -58),      S(-35, -29),    //
    S(49, -130),    S(46, -31),     S(20, 43),      S(-45, 92),     S(-14, 6),      S(-18, -25),    S(-1, -70),     S(-26, -34),    //
    S(37, -247),    S(54, -316),    S(15, -148),    S(33, -98),     S(-18, -49),    S(21, -163),    S(-12, -103),   S(-61, -26),    //
    S(-137, -38),   S(-7, -449),    S(-31, -266),   S(-7, -226),    S(23, -155),    S(38, -230),    S(-21, -127),   S(-88, -23),    //
};
const std::array<PScore, 64> KING_PSQT = {
    S(476, -612),   S(434, -182),   S(-496, 162),   S(-508, 152),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(208, -72),    S(-152, 200),   S(-136, 227),   S(-29, 202),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-143, 65),    S(13, 150),     S(54, 137),     S(-27, 112),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-292, 65),    S(-133, 165),   S(18, 122),     S(-76, 106),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-258, 0),     S(-105, 85),    S(-25, 78),     S(-200, 142),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-174, -51),   S(38, -14),     S(-76, 49),     S(-168, 118),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(101, -135),   S(158, -67),    S(30, 2),       S(-102, 72),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(31, -238),    S(125, -175),   S(-17, -123),   S(12, -157),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
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
    PScore   eval  = PSCORE_ZERO;
    eval += DOUBLED_PAWN_VAL * (pawns & pawns.shift(Direction::North)).popcount();

    return eval;
}

template<Color color>
PScore evaluate_pieces(const Position& pos) {
    constexpr Color opp  = ~color;
    PScore          eval = PSCORE_ZERO;
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
