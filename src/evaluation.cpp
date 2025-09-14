#include <ranges>

#include "common.hpp"
#include "position.hpp"
#include "psqt_state.hpp"

#include "evaluation.hpp"

#include "eval_types.hpp"
#include "square.hpp"

namespace Clockwork {

// clang-format off
const PScore PAWN_MAT   = S(294,536);
const PScore KNIGHT_MAT = S(862,990);
const PScore BISHOP_MAT = S(889,1078);
const PScore ROOK_MAT   = S(1323,1792);
const PScore QUEEN_MAT  = S(2782,3351);
const PScore TEMPO_VAL  = S(30,16);

const PScore BISHOP_PAIR_VAL  = S(47,208);
const PScore DOUBLED_PAWN_VAL = S(-67,-121);

const std::array<PScore, 9> KNIGHT_MOBILITY = {
    S(-152,-200), S(-71,-110), S(-29,-24), S(-18,15), S(11,37), S(35,53), S(55,53), S(86,17), S(133,-58),
};
const std::array<PScore, 14> BISHOP_MOBILITY = {
    S(-149,-192), S(-109,-124), S(-59,-84), S(-30,-55), S(-9,-8), S(14,0), S(33,5), S(38,24), S(57,25), S(66,15), S(100,-1), S(208,-85), S(185,-58), S(287,-149),
};
const std::array<PScore, 15> ROOK_MOBILITY = {
    S(-131,-68), S(-101,-23), S(-74,-17), S(-61,-23), S(-57,5), S(-24,17), S(0,11), S(27,12), S(46,32), S(82,27), S(105,25), S(135,24), S(155,17), S(160,15), S(297,-95),
};
const std::array<PScore, 28> QUEEN_MOBILITY = {
    S(-219,-46), S(-103,-494), S(-101,-493), S(-121,-327), S(-117,-224), S(-106,-148), S(-93,-152), S(-104,-49), S(-88,-58), S(-74,-50), S(-83,40), S(-66,49), S(-53,29), S(-44,55), S(-36,64), S(-41,96), S(-46,92), S(-20,65), S(15,41), S(56,-5), S(139,-59), S(153,-90), S(187,-102), S(466,-379), S(434,-282), S(915,-498), S(311,-157), S(711,-499),
};
const std::array<PScore, 9> KING_MOBILITY = {
    S(233,283), S(114,374), S(96,368), S(70,399), S(30,394), S(-10,386), S(-26,388), S(-37,350), S(22,251),
};

const std::array<PScore, 48> PAWN_PSQT = {
    S(-179,455),    S(3,418),       S(66,451),      S(226,211),     S(176,222),     S(229,313),     S(84,347),      S(182,326),     //
    S(-22,92),      S(129,155),     S(79,43),       S(183,-30),     S(104,-19),     S(6,40),        S(-60,91),      S(-115,50),     //
    S(-53,-71),     S(-19,-65),     S(-42,-110),    S(-42,-149),    S(-56,-129),    S(-108,-129),   S(-143,-59),    S(-182,-38),    //
    S(-38,-178),    S(16,-139),     S(-45,-177),    S(-69,-166),    S(-72,-190),    S(-123,-174),   S(-171,-92),    S(-209,-107),   //
    S(2,-219),      S(101,-205),    S(-8,-166),     S(-75,-145),    S(-95,-152),    S(-149,-156),   S(-155,-116),   S(-208,-128),   //
    S(-49,-213),    S(148,-215),    S(27,-167),     S(-94,-102),    S(-131,-152),   S(-154,-144),   S(-170,-99),    S(-211,-122),   //
};
const std::array<PScore, 64> KNIGHT_PSQT = {
    S(-379,-189),   S(1,-140),      S(-500,228),    S(81,16),       S(-205,108),    S(-256,141),    S(-257,-59),    S(-490,-10),    //
    S(50,-88),      S(70,53),       S(258,-52),     S(116,37),      S(192,46),      S(138,-53),     S(-28,-33),     S(43,-91),      //
    S(75,3),        S(125,93),      S(295,85),      S(240,51),      S(200,76),      S(85,85),       S(15,84),       S(-91,13),      //
    S(146,-10),     S(80,68),       S(182,78),      S(121,96),      S(148,100),     S(62,103),      S(-19,49),      S(-40,44),      //
    S(9,35),        S(142,30),      S(70,77),       S(58,88),       S(26,109),      S(12,106),      S(-34,67),      S(-71,-36),     //
    S(-61,4),       S(-1,-21),      S(5,-12),       S(30,53),       S(28,57),       S(-84,38),      S(-67,-4),      S(-136,-20),    //
    S(-70,35),      S(-34,14),      S(-76,5),       S(-27,16),      S(-39,-4),      S(-106,6),      S(-119,-22),    S(-188,-99),    //
    S(-238,21),     S(-67,-76),     S(-58,-60),     S(-9,-31),      S(-41,-29),     S(-53,-129),    S(-147,-13),    S(-263,-18),    //
};
const std::array<PScore, 64> BISHOP_PSQT = {
    S(-154,68),     S(-71,16),      S(-518,108),    S(-243,76),     S(-347,142),    S(-458,121),    S(-87,102),     S(-23,69),      //
    S(-24,-40),     S(-49,95),      S(36,32),       S(39,7),        S(-34,69),      S(-16,56),      S(-16,36),      S(-132,43),     //
    S(46,43),       S(75,56),       S(263,38),      S(123,44),      S(81,59),       S(0,94),        S(94,20),       S(7,35),        //
    S(68,-37),      S(8,60),        S(129,21),      S(118,61),      S(159,36),      S(50,64),       S(15,48),       S(-102,94),     //
    S(17,-39),      S(17,26),       S(26,37),       S(86,58),       S(63,90),       S(12,55),       S(-16,21),      S(-38,-47),     //
    S(53,-22),      S(63,-13),      S(90,-18),      S(14,51),       S(1,66),        S(-22,76),      S(31,-17),      S(8,-48),       //
    S(-5,-61),      S(83,-44),      S(59,-41),      S(14,-7),       S(-7,-12),      S(20,-11),      S(-26,5),       S(43,-155),     //
    S(-57,56),      S(-67,33),      S(-70,39),      S(-16,-43),     S(-50,-41),     S(-22,-13),     S(3,-66),       S(-60,5),       //
};
const std::array<PScore, 64> ROOK_PSQT = {
    S(169,-8),      S(332,-22),     S(82,59),       S(87,60),       S(136,27),      S(26,67),       S(73,55),       S(57,66),       //
    S(228,-1),      S(174,50),      S(323,-3),      S(168,78),      S(167,75),      S(54,106),      S(17,101),      S(42,90),       //
    S(85,43),       S(318,-24),     S(248,0),       S(204,17),      S(122,31),      S(35,96),       S(51,59),       S(-47,84),      //
    S(1,48),        S(47,33),       S(59,41),       S(98,-3),       S(42,21),       S(-31,115),     S(-38,73),      S(-140,94),     //
    S(-106,-38),    S(39,-54),      S(-45,30),      S(-54,26),      S(-93,38),      S(-127,89),     S(-166,81),     S(-184,62),     //
    S(-121,-53),    S(42,-131),     S(-49,-42),     S(-81,-18),     S(-76,-33),     S(-146,38),     S(-164,16),     S(-194,17),     //
    S(-167,2),      S(-53,-96),     S(-58,-71),     S(-64,-41),     S(-64,-38),     S(-111,-7),     S(-161,-28),    S(-199,-35),    //
    S(-142,-26),    S(-86,3),       S(-63,-28),     S(-16,-43),     S(-46,-18),     S(-57,-24),     S(-79,-45),     S(-124,-35),    //
};
const std::array<PScore, 64> QUEEN_PSQT = {
    S(95,54),       S(112,10),      S(118,148),     S(127,117),     S(102,102),     S(-65,232),     S(8,87),        S(-78,59),      //
    S(106,111),     S(-102,295),    S(89,248),      S(-89,404),     S(-67,303),     S(-144,301),    S(-110,164),    S(-53,77),      //
    S(123,155),     S(206,124),     S(233,191),     S(97,247),      S(-31,288),     S(-53,222),     S(-18,90),      S(4,-19),       //
    S(52,135),      S(30,202),      S(16,207),      S(-14,282),     S(-33,228),     S(-15,98),      S(-34,48),      S(-61,51),      //
    S(-22,120),     S(39,79),       S(-28,208),     S(-23,163),     S(-67,177),     S(-72,113),     S(-36,-47),     S(-77,-41),     //
    S(2,-99),       S(16,-15),      S(6,44),        S(-49,62),      S(-29,-13),     S(-34,-49),     S(-38,-74),     S(-77,-21),     //
    S(-4,-216),     S(9,-297),      S(-13,-146),    S(24,-131),     S(-23,-92),     S(-5,-174),     S(-39,-118),    S(-111,-11),    //
    S(-186,-28),    S(-69,-402),    S(-64,-250),    S(-9,-263),     S(5,-162),      S(30,-255),     S(-47,-118),    S(-111,-11),    //
};
const std::array<PScore, 64> KING_PSQT = {
    S(495,-614),    S(352,-172),    S(-489,143),    S(-583,151),    S(0,0),         S(0,0),         S(0,0),         S(0,0),         //
    S(189,-84),     S(-194,198),    S(-193,226),    S(-115,203),    S(0,0),         S(0,0),         S(0,0),         S(0,0),         //
    S(-195,69),     S(-74,158),     S(18,136),      S(-99,119),     S(0,0),         S(0,0),         S(0,0),         S(0,0),         //
    S(-358,73),     S(-212,176),    S(-91,136),     S(-185,118),    S(0,0),         S(0,0),         S(0,0),         S(0,0),         //
    S(-282,0),      S(-210,102),    S(-158,102),    S(-331,167),    S(0,0),         S(0,0),         S(0,0),         S(0,0),         //
    S(-186,-54),    S(-44,-1),      S(-201,75),     S(-325,151),    S(0,0),         S(0,0),         S(0,0),         S(0,0),         //
    S(110,-151),    S(140,-80),     S(-21,0),       S(-150,64),     S(0,0),         S(0,0),         S(0,0),         S(0,0),         //
    S(126,-286),    S(173,-215),    S(-4,-152),     S(17,-203),     S(0,0),         S(0,0),         S(0,0),         S(0,0),         //
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

    PScore mobility = PSCORE_ZERO;

    auto add_mobility = [&](Color c, PScore& mob_count) {
        Bitboard bb = pos.board().get_color_bitboard(c) | pos.attacked_by(~c, PieceType::Pawn);
        for (PieceId id : pos.get_piece_mask(c, PieceType::Knight)) {
            mobility += KNIGHT_MOBILITY[pos.mobility_of(c, id, ~bb)];
        }
        for (PieceId id : pos.get_piece_mask(c, PieceType::Bishop)) {
            mobility += BISHOP_MOBILITY[pos.mobility_of(c, id, ~bb)];
        }
        for (PieceId id : pos.get_piece_mask(c, PieceType::Rook)) {
            mobility += ROOK_MOBILITY[pos.mobility_of(c, id, ~bb)];
        }
        for (PieceId id : pos.get_piece_mask(c, PieceType::Queen)) {
            mobility += QUEEN_MOBILITY[pos.mobility_of(c, id, ~bb)];
        }
        mobility += KING_MOBILITY[pos.mobility_of(c, PieceId::king(), ~bb)];
    };

    add_mobility(Color::Black, mobility);
    mobility *= -1;  // Persy trick
    add_mobility(Color::White, mobility);


    const std::array<Bitboard, 2> pawns = {pos.board().bitboard_for(Color::White, PieceType::Pawn),
                                           pos.board().bitboard_for(Color::Black, PieceType::Pawn)};

    PScore doubled_pawns_bonus = DOUBLED_PAWN_VAL
                               * ((pawns[0] & pawns[0].shift(Direction::North)).popcount()
                                  - (pawns[1] & pawns[1].shift(Direction::South)).popcount());

    PScore bishop_pair_bonus = BISHOP_PAIR_VAL
                             * ((pos.piece_count(Color::White, PieceType::Bishop) >= 2)
                                - (pos.piece_count(Color::Black, PieceType::Bishop) >= 2));

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
