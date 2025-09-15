#include <ranges>

#include "common.hpp"
#include "position.hpp"
#include "psqt_state.hpp"

#include "evaluation.hpp"

#include "eval_types.hpp"
#include "square.hpp"

namespace Clockwork {

// clang-format off
const PScore PAWN_MAT   = S(294,530);
const PScore KNIGHT_MAT = S(854,996);
const PScore BISHOP_MAT = S(883,1105);
const PScore ROOK_MAT   = S(1289,1785);
const PScore QUEEN_MAT  = S(2784,3339);
const PScore TEMPO_VAL  = S(31,15);

const PScore BISHOP_PAIR_VAL  = S(54,214);
const PScore DOUBLED_PAWN_VAL = S(-71,-118);

const std::array<PScore, 9> KNIGHT_MOBILITY = {
    S(-223,-269), S(-139,-135), S(-85,-46), S(-45,-7), S(1,10), S(24,55), S(52,57), S(87,47), S(136,-30),
};
const std::array<PScore, 14> BISHOP_MOBILITY = {
    S(-211,-289), S(-153,-187), S(-90,-144), S(-66,-82), S(-25,-43), S(-2,-19), S(29,-19), S(43,-7), S(55,8), S(58,2), S(88,-7), S(190,-80), S(235,-123), S(356,-189),
};
const std::array<PScore, 15> ROOK_MOBILITY = {
    S(-297,-305), S(-126,-94), S(-82,-38), S(-66,-23), S(-45,-2), S(-29,4), S(-7,23), S(26,16), S(61,16), S(88,34), S(122,24), S(143,30), S(207,12), S(186,23), S(314,-86),
};
const std::array<PScore, 28> QUEEN_MOBILITY = {
    S(-189,-29), S(-202,-405), S(-205,-596), S(-195,-302), S(-147,-363), S(-125,-287), S(-157,-75), S(-125,-93), S(-106,-68), S(-89,-50), S(-94,5), S(-82,34), S(-52,28), S(-47,43), S(-26,49), S(-26,66), S(-32,105), S(-4,71), S(14,59), S(58,3), S(96,-21), S(159,-91), S(145,-59), S(433,-341), S(425,-349), S(958,-576), S(520,-308), S(1000,-686),
};
const std::array<PScore, 9> KING_MOBILITY = {
    S(491,672), S(158,434), S(40,471), S(31,495), S(-3,481), S(-41,457), S(-58,471), S(-66,445), S(10,318),
};

const std::array<PScore, 48> PAWN_PSQT = {
    S(-178,454),    S(14,417),      S(82,447),      S(202,228),     S(146,241),     S(205,331),     S(71,358),      S(158,337),     //
    S(-21,100),     S(126,162),     S(68,56),       S(168,-11),     S(91,-4),       S(-7,55),       S(-65,102),     S(-115,57),     //
    S(-52,-67),     S(-27,-56),     S(-50,-102),    S(-50,-139),    S(-67,-118),    S(-114,-121),   S(-149,-51),    S(-182,-31),    //
    S(-37,-173),    S(14,-134),     S(-41,-173),    S(-70,-161),    S(-73,-184),    S(-123,-167),   S(-173,-84),    S(-204,-103),   //
    S(2,-212),      S(102,-202),    S(1,-168),      S(-60,-145),    S(-86,-150),    S(-145,-151),   S(-153,-112),   S(-203,-124),   //
    S(-46,-212),    S(152,-218),    S(41,-175),     S(-80,-107),    S(-102,-157),   S(-146,-143),   S(-161,-96),    S(-203,-119),   //
};
const std::array<PScore, 64> KNIGHT_PSQT = {
    S(-318,-159),   S(38,-133),     S(-508,248),    S(71,40),       S(-211,130),    S(-267,161),    S(-235,-47),    S(-398,-11),    //
    S(80,-73),      S(84,72),       S(258,-56),     S(130,21),      S(198,35),      S(142,-55),     S(-18,-11),     S(66,-76),      //
    S(93,16),       S(120,97),      S(281,73),      S(219,38),      S(184,59),      S(72,64),       S(10,80),       S(-81,32),      //
    S(168,7),       S(80,69),       S(171,70),      S(111,85),      S(142,83),      S(55,85),       S(-18,42),      S(-15,58),      //
    S(46,44),       S(158,27),      S(61,71),       S(46,81),       S(10,102),      S(1,90),        S(-28,65),      S(-35,-27),     //
    S(-58,11),      S(-13,-10),     S(-42,3),       S(-3,62),       S(-1,56),       S(-106,30),     S(-89,-2),      S(-120,-4),     //
    S(-6,41),       S(11,35),       S(-51,14),      S(-56,29),      S(-65,5),       S(-120,5),      S(-104,-14),    S(-155,-91),    //
    S(-161,27),     S(-36,-42),     S(-9,-52),      S(13,-18),      S(-5,-28),      S(-41,-123),    S(-101,-1),     S(-234,5),      //
};
const std::array<PScore, 64> BISHOP_PSQT = {
    S(-149,67),     S(-81,13),      S(-532,113),    S(-240,73),     S(-358,144),    S(-450,116),    S(-68,90),      S(7,62),        //
    S(-40,-24),     S(-48,85),      S(23,34),       S(33,1),        S(-32,60),      S(-25,51),      S(-14,26),      S(-123,41),     //
    S(43,48),       S(67,55),       S(258,31),      S(118,37),      S(76,48),       S(-6,80),       S(85,15),       S(0,37),        //
    S(66,-35),      S(6,58),        S(122,16),      S(106,58),      S(152,26),      S(47,50),       S(8,40),        S(-111,94),     //
    S(32,-43),      S(9,24),        S(21,32),       S(66,60),       S(41,89),       S(6,43),        S(-24,14),      S(-31,-49),     //
    S(59,-19),      S(81,-14),      S(77,-12),      S(5,51),        S(-10,62),      S(-41,70),      S(44,-21),      S(15,-49),      //
    S(-9,-49),      S(104,-44),     S(54,-33),      S(-14,9),       S(-30,-8),      S(12,-14),      S(-35,0),       S(48,-137),     //
    S(-5,58),       S(-26,23),      S(-26,42),      S(-10,-40),     S(-40,-46),     S(2,-5),        S(10,-60),      S(-45,9),       //
};
const std::array<PScore, 64> ROOK_PSQT = {
    S(182,-7),      S(361,-29),     S(77,64),       S(95,60),       S(143,26),      S(40,64),       S(102,44),      S(86,58),       //
    S(232,1),       S(173,52),      S(312,0),       S(159,83),      S(165,75),      S(54,105),      S(19,100),      S(46,90),       //
    S(96,43),       S(317,-18),     S(250,5),       S(206,17),      S(123,34),      S(43,94),       S(60,60),       S(-33,82),      //
    S(21,49),       S(55,41),       S(61,50),       S(101,1),       S(42,26),       S(-23,116),     S(-28,76),      S(-124,95),     //
    S(-87,-32),     S(60,-51),      S(-38,40),      S(-53,32),      S(-91,41),      S(-115,88),     S(-151,83),     S(-166,63),     //
    S(-106,-48),    S(55,-121),     S(-46,-27),     S(-85,-7),      S(-81,-27),     S(-149,43),     S(-158,20),     S(-182,21),     //
    S(-128,-3),     S(-23,-96),     S(-23,-72),     S(-62,-35),     S(-70,-34),     S(-111,-8),     S(-161,-26),    S(-184,-33),    //
    S(-128,-26),    S(-76,0),       S(-65,-25),     S(-43,-39),     S(-74,-12),     S(-83,-21),     S(-101,-38),    S(-135,-29),    //
};
const std::array<PScore, 64> QUEEN_PSQT = {
    S(155,13),      S(145,-4),      S(144,134),     S(153,98),      S(120,91),      S(-28,206),     S(50,59),       S(-28,26),      //
    S(129,100),     S(-100,292),    S(81,250),      S(-90,400),     S(-62,294),     S(-146,296),    S(-103,155),    S(-37,71),      //
    S(137,153),     S(210,119),     S(237,190),     S(89,257),      S(-35,292),     S(-54,219),     S(-6,78),       S(11,-13),      //
    S(68,132),      S(38,197),      S(16,206),      S(-34,297),     S(-45,237),     S(-21,101),     S(-28,43),      S(-49,46),      //
    S(-8,125),      S(44,76),       S(-36,214),     S(-47,180),     S(-84,183),     S(-76,111),     S(-32,-52),     S(-66,-34),     //
    S(22,-100),     S(23,-15),      S(-2,54),       S(-73,78),      S(-43,-9),      S(-49,-42),     S(-34,-73),     S(-59,-33),     //
    S(21,-233),     S(38,-306),     S(-4,-147),     S(8,-122),      S(-46,-76),     S(-12,-175),    S(-44,-116),    S(-97,-27),     //
    S(-149,-59),    S(-48,-395),    S(-58,-259),    S(-26,-261),    S(0,-186),      S(17,-267),     S(-47,-138),    S(-133,0),      //
};
const std::array<PScore, 64> KING_PSQT = {
    S(536,-631),    S(372,-162),    S(-520,169),    S(-565,169),    S(0,0),         S(0,0),         S(0,0),         S(0,0),         //
    S(188,-70),     S(-193,214),    S(-167,238),    S(-104,219),    S(0,0),         S(0,0),         S(0,0),         S(0,0),         //
    S(-173,73),     S(-69,170),     S(19,148),      S(-87,127),     S(0,0),         S(0,0),         S(0,0),         S(0,0),         //
    S(-358,79),     S(-211,185),    S(-90,147),     S(-178,130),    S(0,0),         S(0,0),         S(0,0),         S(0,0),         //
    S(-273,-2),     S(-200,109),    S(-151,111),    S(-318,175),    S(0,0),         S(0,0),         S(0,0),         S(0,0),         //
    S(-178,-57),    S(-32,3),       S(-188,81),     S(-303,159),    S(0,0),         S(0,0),         S(0,0),         S(0,0),         //
    S(116,-149),    S(148,-72),     S(-1,7),        S(-146,84),     S(0,0),         S(0,0),         S(0,0),         S(0,0),         //
    S(94,-277),     S(170,-211),    S(12,-153),     S(47,-195),     S(0,0),         S(0,0),         S(0,0),         S(0,0),         //
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
        Bitboard bb = pos.bitboard_for(c, PieceType::Pawn) | pos.attacked_by(~c, PieceType::Pawn);
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
