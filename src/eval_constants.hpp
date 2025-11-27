#pragma once

#include "eval_types.hpp"

namespace Clockwork {

// clang-format off
inline const PParam PAWN_MAT   = S(291,312);
inline const PParam KNIGHT_MAT = S(1137,913);
inline const PParam BISHOP_MAT = S(1226,947);
inline const PParam ROOK_MAT   = S(1712,1682);
inline const PParam QUEEN_MAT  = S(3665,2886);
inline const PParam TEMPO_VAL  = S(59,15);

inline const PParam BISHOP_PAIR_VAL   = S(80,178);
inline const PParam ROOK_OPEN_VAL     = S(104,-29);
inline const PParam ROOK_SEMIOPEN_VAL = S(39,13);

inline const PParam DOUBLED_PAWN_VAL = S(-37,-78);

inline const PParam POTENTIAL_CHECKER_VAL = S(-75,-3);
inline const PParam OUTPOST_KNIGHT_VAL    = S(7,51);
inline const PParam OUTPOST_BISHOP_VAL    = S(29,27);

inline const PParam PAWN_PUSH_THREAT_KNIGHT = S(54,18);
inline const PParam PAWN_PUSH_THREAT_BISHOP = S(56,-14);
inline const PParam PAWN_PUSH_THREAT_ROOK   = S(33,33);
inline const PParam PAWN_PUSH_THREAT_QUEEN  = S(55,-53);

inline const std::array<PParam, 6> PAWN_PHALANX = {
    S(20,20), S(63,31), S(74,70), S(190,140), S(561,241), S(925,1154),
};
inline const std::array<PParam, 5> DEFENDED_PAWN = {
    S(64,43), S(61,31), S(67,57), S(147,120), S(688,-90),
};
inline const std::array<PParam, 6> PASSED_PAWN = {
    S(-71,-104), S(-61,-85), S(-35,-9), S(20,70), S(42,211), S(307,314),
};
inline const std::array<PParam, 6> DEFENDED_PASSED_PUSH = {
    S(50,-44), S(36,-6), S(21,27), S(23,76), S(92,152), S(144,296),
};
inline const std::array<PParam, 6> BLOCKED_PASSED_PAWN = {
    S(15,-46), S(4,3), S(1,-27), S(7,-47), S(5,-96), S(-192,-145),
};

inline const std::array<PParam, 8> FRIENDLY_KING_PASSED_PAWN_DISTANCE = {
    CS(0,0), S(13,101), S(-22,89), S(-14,38), S(-1,8), S(9,13), S(39,10), S(18,-1),
};
inline const std::array<PParam, 8> ENEMY_KING_PASSED_PAWN_DISTANCE = {
    CS(0,0), S(-185,-49), S(32,-3), S(-11,46), S(12,76), S(17,101), S(37,100), S(-12,120),
};

inline const std::array<PParam, 9> KNIGHT_MOBILITY = {
    S(-227,-224), S(-120,-61), S(-63,-8), S(-19,25), S(28,40), S(54,78), S(91,75), S(125,77), S(171,20),
};
inline const std::array<PParam, 14> BISHOP_MOBILITY = {
    S(-236,-283), S(-162,-99), S(-87,-37), S(-51,13), S(-20,44), S(-2,65), S(17,80), S(37,86), S(56,90), S(70,86), S(96,74), S(161,28), S(191,6), S(251,-28),
};
inline const std::array<PParam, 15> ROOK_MOBILITY = {
    S(-306,-230), S(-152,-82), S(-100,-18), S(-68,-8), S(-41,15), S(-26,37), S(-8,49), S(11,55), S(28,67), S(46,76), S(64,78), S(77,80), S(98,83), S(109,69), S(255,-58),
};
inline const std::array<PParam, 28> QUEEN_MOBILITY = {
    S(-966,-877), S(-255,-659), S(-159,-521), S(-104,-314), S(-95,-106), S(-57,-1), S(-53,109), S(-28,123), S(-23,177), S(-11,203), S(-2,228), S(4,243), S(22,235), S(33,247), S(40,241), S(53,237), S(60,228), S(59,235), S(85,191), S(107,154), S(122,135), S(165,70), S(178,61), S(338,-112), S(372,-155), S(616,-312), S(485,-255), S(857,-453),
};
inline const std::array<PParam, 9> KING_MOBILITY = {
    S(448,-4), S(99,-121), S(-5,-32), S(-20,6), S(-48,10), S(-82,15), S(-61,17), S(-70,11), S(-71,-38),
};

inline const std::array<PParam, 3> KNIGHT_KING_RING = {
    CS(0,0), S(88,-32), S(158,-79),
};
inline const std::array<PParam, 3> BISHOP_KING_RING = {
    CS(0,0), S(36,-7), S(136,-45),
};
inline const std::array<PParam, 5> ROOK_KING_RING = {
    CS(0,0), S(68,-50), S(53,-65), S(104,-64), S(162,-130),
};
inline const std::array<PParam, 6> QUEEN_KING_RING = {
    CS(0,0), S(-39,27), S(-56,38), S(-1,-9), S(162,-98), S(357,-238),
};

inline const PParam PAWN_THREAT_KNIGHT = S(240,58);
inline const PParam PAWN_THREAT_BISHOP = S(208,102);
inline const PParam PAWN_THREAT_ROOK   = S(199,56);
inline const PParam PAWN_THREAT_QUEEN  = S(179,-64);

inline const PParam KNIGHT_THREAT_BISHOP = S(106,73);
inline const PParam KNIGHT_THREAT_ROOK   = S(244,5);
inline const PParam KNIGHT_THREAT_QUEEN  = S(156,-68);

inline const PParam BISHOP_THREAT_KNIGHT = S(111,35);
inline const PParam BISHOP_THREAT_ROOK   = S(244,55);
inline const PParam BISHOP_THREAT_QUEEN  = S(192,49);

inline const std::array<PParam, 9> BISHOP_PAWNS = {
    S(-1,-11), S(-3,-5), S(-3,-15), S(-8,-26), S(-14,-31), S(-19,-38), S(-21,-45), S(-27,-40), S(-37,-44),
};

inline const std::array<PParam, 48> PAWN_PSQT = {
    S(110,161),     S(101,208),     S(170,174),     S(233,55),      S(178,50),      S(164,115),     S(58,138),      S(117,114),     //
    S(81,46),       S(194,73),      S(170,16),      S(172,-42),     S(125,-58),     S(69,-10),      S(30,37),       S(-21,41),      //
    S(-0,13),       S(20,17),       S(37,-27),      S(24,-41),      S(6,-45),       S(-36,-39),     S(-77,8),       S(-103,30),     //
    S(-26,-35),     S(-8,-7),       S(-12,-40),     S(-30,-37),     S(-56,-46),     S(-76,-38),     S(-128,12),     S(-147,-0),     //
    S(-29,-65),     S(35,-62),      S(-14,-18),     S(-45,-16),     S(-66,-25),     S(-105,-27),    S(-125,-13),    S(-149,-19),    //
    S(-19,-58),     S(112,-54),     S(67,-18),      S(9,0),         S(-28,-12),     S(-64,-17),     S(-91,6),       S(-128,-6),     //
};
inline const std::array<PParam, 64> KNIGHT_PSQT = {
    S(-403,-162),   S(-355,58),     S(-465,233),    S(-127,64),     S(-258,90),     S(-342,96),     S(-576,83),     S(-547,-20),    //
    S(-1,-6),       S(65,6),        S(166,-61),     S(112,5),       S(114,12),      S(51,-14),      S(-9,7),        S(-28,-39),     //
    S(55,-32),      S(98,13),       S(189,4),       S(141,27),      S(140,17),      S(59,25),       S(48,-0),       S(-48,7),       //
    S(110,2),       S(100,24),      S(130,29),      S(108,57),      S(117,44),      S(84,37),       S(59,-5),       S(34,2),        //
    S(99,-18),      S(125,-21),     S(119,5),       S(91,22),       S(82,30),       S(79,25),       S(52,-3),       S(38,-58),      //
    S(11,-27),      S(42,-39),      S(37,-18),      S(48,25),       S(54,22),       S(-3,0),        S(2,-39),       S(-38,-45),     //
    S(13,-12),      S(34,-40),      S(18,-34),      S(18,-15),      S(5,-21),       S(-23,-42),     S(-12,-56),     S(-69,-124),    //
    S(-36,-63),     S(2,-20),       S(20,-43),      S(29,-36),      S(21,-28),      S(-27,-59),     S(-39,-35),     S(-88,-87),     //
};
inline const std::array<PParam, 64> BISHOP_PSQT = {
    S(-162,84),     S(-183,64),     S(-426,92),     S(-305,105),    S(-256,108),    S(-425,134),    S(-166,111),    S(-118,87),     //
    S(10,-28),      S(-8,47),       S(12,29),       S(-9,34),       S(-34,49),      S(4,40),        S(-19,30),      S(-53,33),      //
    S(40,29),       S(86,19),       S(163,26),      S(93,26),       S(68,29),       S(43,41),       S(101,13),      S(-0,30),       //
    S(57,-16),      S(70,16),       S(105,18),      S(103,43),      S(110,43),      S(48,41),       S(37,18),       S(-11,25),      //
    S(58,-44),      S(65,-6),       S(72,10),       S(72,34),       S(64,50),       S(24,41),       S(8,4),         S(4,-38),       //
    S(70,-35),      S(110,-21),     S(115,-7),      S(55,33),       S(37,39),       S(39,34),       S(67,-10),      S(35,-32),      //
    S(57,-70),      S(107,-46),     S(77,-34),      S(49,-4),       S(40,-19),      S(41,-30),      S(23,-15),      S(43,-79),      //
    S(53,-51),      S(39,-5),       S(44,2),        S(55,-32),      S(63,-43),      S(60,-1),       S(52,-29),      S(34,-32),      //
};
inline const std::array<PParam, 64> ROOK_PSQT = {
    S(106,12),      S(171,10),      S(100,41),      S(100,35),      S(108,24),      S(56,37),       S(64,40),       S(72,45),       //
    S(14,70),       S(101,45),      S(176,22),      S(101,66),      S(117,53),      S(64,63),       S(4,81),        S(-5,87),       //
    S(3,47),        S(151,6),       S(181,-0),      S(182,-4),      S(136,6),       S(61,47),       S(79,34),       S(-41,85),      //
    S(-28,41),      S(48,34),       S(80,25),       S(102,-11),     S(70,13),       S(9,61),        S(-9,60),       S(-80,68),      //
    S(-93,-8),      S(-11,-3),      S(-26,14),      S(-43,15),      S(-48,12),      S(-66,52),      S(-96,48),      S(-115,38),     //
    S(-116,-29),    S(-41,-55),     S(-47,-25),     S(-66,-24),     S(-50,-43),     S(-99,12),      S(-102,-6),     S(-124,-9),     //
    S(-178,-18),    S(-78,-78),     S(-54,-63),     S(-49,-61),     S(-56,-56),     S(-75,-40),     S(-96,-62),     S(-127,-46),    //
    S(-143,-17),    S(-112,-11),    S(-59,-47),     S(-34,-64),     S(-47,-49),     S(-60,-37),     S(-76,-46),     S(-93,-29),     //
};
inline const std::array<PParam, 64> QUEEN_PSQT = {
    S(38,43),       S(77,8),        S(81,14),       S(-38,143),     S(33,62),       S(-23,89),      S(42,4),        S(-22,29),      //
    S(24,81),       S(-49,176),     S(-53,231),     S(-141,263),    S(-113,209),    S(-123,207),    S(-74,113),     S(-39,53),      //
    S(-18,113),     S(72,109),      S(12,186),      S(-5,197),      S(-46,179),     S(-78,178),     S(-7,72),       S(-42,45),      //
    S(50,26),       S(54,95),       S(18,130),      S(6,195),       S(-16,180),     S(-25,114),     S(13,20),       S(-1,-12),      //
    S(15,53),       S(50,15),       S(24,85),       S(-16,142),     S(-29,129),     S(-22,86),      S(-11,8),       S(-9,-42),      //
    S(25,-105),     S(50,-62),      S(51,7),        S(-3,34),       S(9,-9),        S(12,-4),       S(23,-74),      S(-1,-68),      //
    S(10,-212),     S(44,-311),     S(31,-175),     S(48,-103),     S(22,-83),      S(39,-154),     S(13,-92),      S(-5,-90),      //
    S(-46,-132),    S(15,-382),     S(12,-369),     S(39,-275),     S(41,-193),     S(45,-238),     S(32,-198),     S(-22,-122),    //
};
inline const std::array<PParam, 64> KING_PSQT = {
    S(-136,-382),   S(93,-23),      S(-65,29),      S(-173,53),     S(20,-11),      S(20,-11),      S(20,-11),      S(20,-11),      //
    S(190,-116),    S(5,142),       S(22,127),      S(127,58),      S(20,-11),      S(20,-11),      S(20,-11),      S(20,-11),      //
    S(-37,55),      S(66,134),      S(106,102),     S(91,57),       S(20,-11),      S(20,-11),      S(20,-11),      S(20,-11),      //
    S(-257,79),     S(31,94),       S(24,91),       S(-30,76),      S(20,-11),      S(20,-11),      S(20,-11),      S(20,-11),      //
    S(-230,36),     S(-47,67),      S(-30,72),      S(-113,108),    S(20,-11),      S(20,-11),      S(20,-11),      S(20,-11),      //
    S(-136,6),      S(56,9),        S(-34,60),      S(-78,86),      S(20,-11),      S(20,-11),      S(20,-11),      S(20,-11),      //
    S(80,-85),      S(134,-45),     S(45,2),        S(-37,46),      S(20,-11),      S(20,-11),      S(20,-11),      S(20,-11),      //
    S(-44,-114),    S(87,-104),     S(-14,-68),     S(-31,-67),     S(20,-11),      S(20,-11),      S(20,-11),      S(20,-11),      //
};
// Epoch duration: 61.8411s
// clang-format on
}  // namespace Clockwork
