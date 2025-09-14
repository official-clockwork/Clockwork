#include <ranges>

#include "common.hpp"
#include "position.hpp"
#include "psqt_state.hpp"

#include "evaluation.hpp"

#include "eval_types.hpp"
#include "square.hpp"

namespace Clockwork {

// clang-format off
const PScore PAWN_MAT   = S(271,548);
const PScore KNIGHT_MAT = S(834,995);
const PScore BISHOP_MAT = S(857,1074);
const PScore ROOK_MAT   = S(1282,1784);
const PScore QUEEN_MAT  = S(2761,3320);
const PScore TEMPO_VAL  = S(29,15);

const PScore BISHOP_PAIR_VAL  = S(51,217);
const PScore DOUBLED_PAWN_VAL = S(-62,-120);

const std::array<PScore, 9> KNIGHT_MOBILITY = {
    S(-416,82), S(-151,-239), S(-100,-73), S(-39,-10), S(6,1), S(15,56), S(57,54), S(97,47), S(154,9),
};
const std::array<PScore, 14> BISHOP_MOBILITY = {
    S(-145,-742), S(-244,-297), S(-118,-133), S(-65,-68), S(-20,-29), S(16,-19), S(39,12), S(70,11), S(70,38), S(97,27), S(99,42), S(185,-9), S(293,-88), S(298,-110),
};
const std::array<PScore, 15> ROOK_MOBILITY = {
    S(-298,123), S(-14,-286), S(-87,-84), S(-69,-14), S(-49,2), S(-25,3), S(-18,27), S(13,31), S(45,16), S(78,26), S(107,35), S(135,36), S(203,15), S(172,35), S(303,-71),
};
const std::array<PScore, 28> QUEEN_MOBILITY = {
    S(-218,-45), S(-102,-493), S(137,-452), S(-337,-480), S(-300,-75), S(-199,-274), S(-131,-357), S(-119,-134), S(-101,-129), S(-87,-86), S(-75,-12), S(-66,22), S(-49,21), S(-31,49), S(-27,70), S(1,53), S(-8,93), S(15,73), S(34,80), S(58,44), S(64,19), S(112,-8), S(118,26), S(282,-121), S(354,-277), S(917,-500), S(564,-209), S(875,-520),
};
const std::array<PScore, 9> KING_MOBILITY = {
    S(233,283), S(135,1048), S(-32,628), S(-18,482), S(-19,414), S(20,324), S(-51,345), S(-3,313), S(47,237),
};

const std::array<PScore, 48> PAWN_PSQT = {
    S(-185,463),    S(-18,433),     S(27,474),      S(161,241),     S(123,237),     S(206,308),     S(65,346),      S(158,321),     //
    S(-49,120),     S(96,188),      S(23,93),       S(121,10),      S(61,0),        S(-30,58),      S(-78,97),      S(-131,57),     //
    S(-60,-63),     S(-47,-53),     S(-69,-95),     S(-75,-135),    S(-82,-121),    S(-121,-132),   S(-156,-61),    S(-187,-41),    //
    S(-37,-176),    S(4,-131),      S(-58,-170),    S(-89,-161),    S(-89,-189),    S(-130,-179),   S(-175,-97),    S(-208,-113),   //
    S(4,-214),      S(110,-201),    S(-18,-149),    S(-73,-144),    S(-92,-155),    S(-152,-162),   S(-150,-127),   S(-213,-132),   //
    S(-19,-219),    S(181,-217),    S(61,-170),     S(-79,-111),    S(-120,-169),   S(-142,-158),   S(-163,-109),   S(-207,-127),   //
};
const std::array<PScore, 64> KNIGHT_PSQT = {
    S(-270,-85),    S(36,-113),     S(-484,263),    S(109,36),      S(-191,138),    S(-257,172),    S(-195,-41),    S(-350,39),     //
    S(122,-68),     S(108,80),      S(284,-50),     S(150,27),      S(211,40),      S(164,-54),     S(-1,-5),       S(113,-79),     //
    S(120,18),      S(145,92),      S(283,74),      S(219,34),      S(192,54),      S(74,60),       S(29,82),       S(-67,34),      //
    S(174,8),       S(88,71),       S(161,75),      S(126,79),      S(152,78),      S(52,88),       S(-11,46),      S(-6,59),       //
    S(44,40),       S(135,28),      S(51,74),       S(31,82),       S(8,100),       S(-7,94),       S(-36,67),      S(-40,-30),     //
    S(-66,2),       S(-2,-10),      S(-69,15),      S(-20,65),      S(-26,64),      S(-128,40),     S(-80,-4),      S(-110,-6),     //
    S(0,54),        S(6,25),        S(-52,0),       S(-59,15),      S(-53,-12),     S(-121,0),      S(-103,-9),     S(-135,-85),    //
    S(-154,51),     S(-48,-44),     S(-50,-54),     S(-21,-26),     S(-59,-32),     S(-69,-116),    S(-118,-11),    S(-221,45),     //
};
const std::array<PScore, 64> BISHOP_PSQT = {
    S(-146,88),     S(-66,14),      S(-519,115),    S(-232,80),     S(-332,143),    S(-447,123),    S(-68,93),      S(13,72),       //
    S(-10,-23),     S(-50,89),      S(29,30),       S(33,2),        S(-38,63),      S(-24,55),      S(-17,27),      S(-98,46),      //
    S(54,56),       S(74,52),       S(260,21),      S(106,37),      S(59,49),       S(-5,74),       S(78,15),       S(18,38),       //
    S(81,-27),      S(7,57),        S(121,10),      S(90,56),       S(139,25),      S(36,46),       S(13,36),       S(-95,98),      //
    S(34,-34),      S(12,21),       S(9,32),        S(60,51),       S(26,84),       S(-1,42),       S(-23,9),       S(-18,-46),     //
    S(79,-9),       S(45,-21),      S(66,-12),      S(-8,48),       S(-13,60),      S(-50,68),      S(22,-30),      S(37,-45),      //
    S(43,-38),      S(106,-47),     S(52,-36),      S(-8,3),        S(-26,-17),     S(16,-14),      S(-37,2),       S(99,-119),     //
    S(57,77),       S(-63,42),      S(-47,32),      S(-15,-51),     S(-44,-50),     S(-13,-35),     S(18,-66),      S(-19,23),      //
};
const std::array<PScore, 64> ROOK_PSQT = {
    S(203,-12),     S(380,-36),     S(99,58),       S(115,53),      S(165,20),      S(54,60),       S(115,39),      S(96,55),       //
    S(252,-5),      S(192,44),      S(332,-1),      S(171,81),      S(182,71),      S(72,103),      S(42,93),       S(71,80),       //
    S(109,44),      S(336,-24),     S(272,0),       S(225,16),      S(141,31),      S(61,92),       S(76,58),       S(-20,82),      //
    S(26,50),       S(59,40),       S(71,46),       S(118,-2),      S(57,23),       S(-11,113),     S(-17,72),      S(-117,90),     //
    S(-98,-31),     S(48,-53),      S(-45,35),      S(-55,27),      S(-95,37),      S(-122,85),     S(-155,80),     S(-174,59),     //
    S(-116,-53),    S(36,-121),     S(-66,-31),     S(-96,-13),     S(-92,-32),     S(-160,34),     S(-167,13),     S(-194,14),     //
    S(-147,1),      S(-47,-86),     S(-49,-68),     S(-83,-39),     S(-87,-38),     S(-127,-10),    S(-170,-28),    S(-205,-33),    //
    S(-144,-27),    S(-72,2),       S(-66,-24),     S(-35,-36),     S(-65,-10),     S(-77,-20),     S(-96,-39),     S(-142,-30),    //
};
const std::array<PScore, 64> QUEEN_PSQT = {
    S(185,6),       S(160,0),       S(155,140),     S(170,102),     S(142,87),      S(-1,202),      S(67,61),       S(-6,34),       //
    S(163,91),      S(-76,282),     S(96,249),      S(-72,395),     S(-47,293),     S(-123,288),    S(-87,159),     S(-13,71),      //
    S(155,164),     S(226,116),     S(251,186),     S(99,256),      S(-29,294),     S(-38,218),     S(1,79),        S(34,-10),      //
    S(81,134),      S(45,195),      S(15,210),      S(-32,297),     S(-37,230),     S(-16,98),      S(-21,51),      S(-39,56),      //
    S(-12,125),     S(43,70),       S(-47,210),     S(-47,175),     S(-90,183),     S(-83,114),     S(-33,-46),     S(-66,-31),     //
    S(11,-96),      S(-4,-15),      S(-24,54),      S(-85,74),      S(-54,-4),      S(-67,-33),     S(-49,-71),     S(-70,-17),     //
    S(25,-231),     S(16,-286),     S(-19,-147),    S(-2,-128),     S(-54,-89),     S(-24,-168),    S(-55,-104),    S(-88,-34),     //
    S(-115,-59),    S(-71,-370),    S(-59,-255),    S(-22,-265),    S(8,-190),      S(14,-257),     S(-50,-121),    S(-124,7),      //
};
const std::array<PScore, 64> KING_PSQT = {
    S(506,-715),    S(341,-139),    S(-518,178),    S(-594,185),    S(0,0),         S(0,0),         S(0,0),         S(0,0),         //
    S(183,-61),     S(-212,209),    S(-177,227),    S(-125,204),    S(0,0),         S(0,0),         S(0,0),         S(0,0),         //
    S(-197,72),     S(-107,184),    S(-21,156),     S(-106,128),    S(0,0),         S(0,0),         S(0,0),         S(0,0),         //
    S(-379,70),     S(-238,200),    S(-96,153),     S(-202,134),    S(0,0),         S(0,0),         S(0,0),         S(0,0),         //
    S(-245,-33),    S(-209,124),    S(-165,120),    S(-331,183),    S(0,0),         S(0,0),         S(0,0),         S(0,0),         //
    S(-139,-90),    S(-48,26),      S(-206,98),     S(-325,168),    S(0,0),         S(0,0),         S(0,0),         S(0,0),         //
    S(135,-167),    S(97,-38),      S(-48,27),      S(-194,94),     S(0,0),         S(0,0),         S(0,0),         S(0,0),         //
    S(195,-445),    S(170,-213),    S(3,-163),      S(27,-217),     S(0,0),         S(0,0),         S(0,0),         S(0,0),         //
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
        Bitboard pawn_danger = pos.attacked_by(~c, PieceType::Pawn);
        for (PieceId id : pos.get_piece_mask(c, PieceType::Knight)) {
            mobility += KNIGHT_MOBILITY[pos.mobility_of(c, id, ~pawn_danger)];
        }
        for (PieceId id : pos.get_piece_mask(c, PieceType::Bishop)) {
            mobility += BISHOP_MOBILITY[pos.mobility_of(c, id, ~pawn_danger)];
        }
        for (PieceId id : pos.get_piece_mask(c, PieceType::Rook)) {
            mobility += ROOK_MOBILITY[pos.mobility_of(c, id, ~pawn_danger)];
        }
        for (PieceId id : pos.get_piece_mask(c, PieceType::Queen)) {
            mobility += QUEEN_MOBILITY[pos.mobility_of(c, id, ~pawn_danger)];
        }
        mobility += KING_MOBILITY[pos.mobility_of(c, PieceId::king(), ~pawn_danger)];
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
