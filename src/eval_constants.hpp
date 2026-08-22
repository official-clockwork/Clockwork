#pragma once

#include "eval_types.hpp"

namespace Clockwork {

// clang-format off
inline const PParam PAWN_MAT   = S(202, 507);
inline const PParam KNIGHT_MAT = S(821, 1596);
inline const PParam BISHOP_MAT = S(872, 1682);
inline const PParam ROOK_MAT   = S(1041, 2670);
inline const PParam QUEEN_MAT  = S(2419, 4123);

inline const PParam TEMPO_VAL  = S(69, 58);

inline const PParam BISHOP_XRAY_PAWNS   = S(-13, -2);
inline const PParam BISHOP_PAIR_VAL   = S(62, 240);
inline const PParam ROOK_OPEN_VAL     = S(109, -4);
inline const PParam ROOK_SEMIOPEN_VAL = S(44, 12);
inline const PParam MINOR_BEHIND_PAWN = S(15, 42);
inline const PParam RESTRICTED_SQUARES = S(19, 7);

inline const PParam DOUBLED_PAWN_VAL = S(-22, -81);
inline const PParam ISOLATED_PAWN_VAL = S(-13, -39);

inline const PParam POTENTIAL_CHECKER_VAL = S(-45, -42);
inline const PParam OUTPOST_KNIGHT_VAL    = S(49, 43);
inline const PParam OUTPOST_BISHOP_VAL    = S(42, 39);

inline const PParam PAWN_PUSH_THREAT_KNIGHT = S(39, 51);
inline const PParam PAWN_PUSH_THREAT_BISHOP = S(42, 3);
inline const PParam PAWN_PUSH_THREAT_ROOK   = S(18, 70);
inline const PParam PAWN_PUSH_THREAT_QUEEN  = S(62, -51);

inline const std::array<PParam, 6> PAWN_PHALANX = {
    S(15, -2), S(33, 36), S(60, 66), S(122, 214), S(390, 405), S(680, 823),
};
inline const std::array<PParam, 5> DEFENDED_PAWN = {
    S(48, 33), S(39, 26), S(54, 57), S(91, 181), S(387, 122),
};
inline const std::array<PParam, 6> PASSED_PAWN = {
    S(-94, -263), S(-95, -221), S(-68, -88), S(-23, 38), S(87, 198), S(230, 407),
};
inline const std::array<PParam, 6> PASSED_CLEAR_STOPPERS = {
    S(-81, 54), S(-92, 36), S(-59, 105), S(-89, 230), S(-54, 341), S(51, 224),
};
inline const std::array<PParam, 6> PASSED_CLEAR_FORWARD = {
    S(-18, 69), S(-29, 60), S(-60, 108), S(-33, 135), S(73, 160), S(34, 195),
};
inline const std::array<PParam, 6> DEFENDED_PASSED_PUSH = {
    S(24, -40), S(24, -2), S(16, 35), S(22, 103), S(71, 272), S(335, 219),
};
inline const std::array<PParam, 6> BLOCKED_PASSED_PAWN = {
    S(14, -36), S(2, 8), S(-3, -26), S(-0, -68), S(-14, -176), S(-170, -457),
};

inline const std::array<PParam, 8> FRIENDLY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(20, 233), S(9, 181), S(3, 105), S(11, 66), S(19, 71), S(57, 66), S(61, 36),
};
inline const std::array<PParam, 8> ENEMY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-131, -46), S(15, 0), S(3, 103), S(29, 153), S(45, 188), S(56, 196), S(42, 177),
};

inline const std::array<PParam, 9> KNIGHT_MOBILITY = {
    S(-95, -396), S(-27, -142), S(4, -27), S(27, 22), S(54, 55), S(70, 92), S(88, 100), S(107, 108), S(127, 61),
};
inline const std::array<PParam, 14> BISHOP_MOBILITY = {
    S(-74, -352), S(-20, -130), S(32, -40), S(57, 14), S(77, 57), S(89, 88), S(94, 111), S(96, 132), S(101, 143), S(107, 150), S(116, 144), S(130, 127), S(130, 132), S(115, 96),
};
inline const std::array<PParam, 15> ROOK_MOBILITY = {
    S(99, -211), S(24, -11), S(45, 23), S(60, 42), S(71, 60), S(75, 74), S(80, 90), S(86, 96), S(90, 108), S(96, 116), S(100, 123), S(100, 132), S(103, 136), S(108, 123), S(128, 85),
};
inline const std::array<PParam, 28> QUEEN_MOBILITY = {
    S(-193, -204), S(-38, -90), S(-14, 51), S(5, 193), S(22, 236), S(30, 289), S(36, 327), S(43, 345), S(48, 368), S(51, 385), S(57, 393), S(62, 400), S(65, 407), S(67, 413), S(68, 415), S(67, 418), S(64, 420), S(66, 412), S(67, 408), S(71, 400), S(61, 391), S(73, 364), S(37, 393), S(-7, 383), S(-40, 382), S(-75, 405), S(-135, 433), S(-100, 368),
};

inline const PParam PAWN_THREAT_KNIGHT = S(196, 154);
inline const PParam PAWN_THREAT_BISHOP = S(171, 221);
inline const PParam PAWN_THREAT_ROOK   = S(190, 153);
inline const PParam PAWN_THREAT_QUEEN  = S(160, 21);

inline const std::array<std::array<PParam, 5>, 2> MINOR_THREAT = {{
  {{ S(8, 57), S(98, 96), S(104, 125), S(178, 94), S(164, 10), }},
  {{ S(6, 58), S(87, 94), S(103, 124), S(219, 614), S(149, 49), }},
}};
inline const std::array<std::array<PParam, 5>, 2> ROOK_THREAT = {{
  {{ S(10, 64), S(44, 107), S(59, 112), S(12, 12), S(151, -76), }},
  {{ S(4, 49), S(62, 82), S(95, 72), S(23, 12), S(365, 898), }},
}};
inline const PParam KING_THREAT  = S(-10, 157);
inline const PParam HANGING_PAWN  = S(32, 92);
inline const PParam HANGING_NON_PAWN  = S(72, 31);

inline const std::array<PParam, 2> KNIGHT_ON_QUEEN = {
    S(21, -17), S(22, 8),
};
inline const std::array<PParam, 2> BISHOP_ON_QUEEN = {
    S(42, 45), S(112, -112),
};
inline const std::array<PParam, 2> ROOK_ON_QUEEN = {
    S(37, 13), S(49, -7),
};

inline const std::array<PParam, 9> BISHOP_PAWNS = {
    S(4, -17), S(4, -15), S(-1, -21), S(-6, -29), S(-12, -37), S(-17, -43), S(-19, -55), S(-25, -57), S(-31, -74),
};

inline const PParam ROOK_LINEUP = S(17, 81);

inline const std::array<PParam, 48> PAWN_PSQT = {
    S(184, 277),    S(63, 399),     S(69, 387),     S(158, 271),    S(193, 168),    S(144, 211),    S(123, 240),    S(222, 181),    //
    S(42, 64),      S(55, 110),     S(35, 62),      S(48, -4),      S(40, -41),     S(13, -12),     S(15, 27),      S(-20, 54),     //
    S(39, -13),     S(15, -4),      S(44, -45),     S(26, -69),     S(18, -87),     S(4, -75),      S(-36, -44),    S(-37, -5),     //
    S(-6, -73),     S(-30, -43),    S(1, -61),      S(-10, -77),    S(-31, -87),    S(-35, -75),    S(-82, -57),    S(-76, -52),    //
    S(-13, -111),   S(18, -107),    S(3, -45),      S(-13, -50),    S(-40, -60),    S(-53, -71),    S(-79, -72),    S(-83, -74),    //
    S(9, -107),     S(84, -95),     S(72, -40),     S(25, -11),     S(-4, -32),     S(-24, -60),    S(-49, -59),    S(-62, -57),    //
};
inline const std::array<PParam, 64> KNIGHT_PSQT = {
    S(-261, -501),  S(-258, 51),    S(-264, -30),   S(1, 42),       S(-89, 41),     S(-265, 81),    S(-346, 100),   S(-348, -406),  //
    S(-0, -11),     S(4, 27),       S(86, 24),      S(84, 60),      S(90, 48),      S(52, 27),      S(-15, 26),     S(-41, 20),     //
    S(41, -25),     S(43, 28),      S(56, 63),      S(75, 71),      S(58, 72),      S(21, 59),      S(10, 35),      S(-34, 11),     //
    S(90, 23),      S(92, 47),      S(102, 69),     S(105, 110),    S(109, 108),    S(73, 73),      S(52, 38),      S(33, 32),      //
    S(79, 15),      S(102, 3),      S(100, 50),     S(95, 81),      S(81, 83),      S(73, 64),      S(58, 19),      S(24, 25),      //
    S(18, -41),     S(44, -25),     S(51, 20),      S(63, 52),      S(59, 51),      S(31, 14),      S(15, -18),     S(-24, -41),    //
    S(36, -23),     S(39, -10),     S(27, -19),     S(37, 9),       S(34, 7),       S(5, -44),      S(-31, 0),      S(-49, -84),    //
    S(-44, -93),    S(10, -29),     S(28, -43),     S(44, -32),     S(28, -18),     S(-9, -51),     S(-21, -27),    S(-71, -141),   //
};
inline const std::array<PParam, 64> BISHOP_PSQT = {
    S(-131, 21),    S(-191, 102),   S(-400, 166),   S(-272, 97),    S(-303, 138),   S(-304, 152),   S(-226, 127),   S(-129, 72),    //
    S(-27, -20),    S(-64, 87),     S(-46, 61),     S(-68, 59),     S(-66, 74),     S(-40, 48),     S(-9, 41),      S(-51, 24),     //
    S(34, 17),      S(15, 59),      S(29, 68),      S(26, 54),      S(22, 42),      S(20, 46),      S(9, 42),       S(11, 12),      //
    S(15, -7),      S(48, 20),      S(60, 35),      S(79, 54),      S(103, 39),     S(41, 20),      S(44, -4),      S(-2, -3),      //
    S(36, -44),     S(44, 2),       S(76, 13),      S(91, 26),      S(74, 37),      S(64, 24),      S(19, 6),       S(9, -39),      //
    S(56, -30),     S(79, -26),     S(95, 4),       S(63, 28),      S(59, 12),      S(56, 13),      S(58, -4),      S(15, -23),     //
    S(25, -74),     S(112, -58),    S(65, -20),     S(46, -2),      S(29, -2),      S(44, -44),     S(44, -58),     S(37, -51),     //
    S(42, -73),     S(22, -25),     S(29, -11),     S(43, -33),     S(27, -20),     S(36, 14),      S(42, -17),     S(38, -62),     //
};
inline const std::array<PParam, 64> ROOK_PSQT = {
    S(175, 109),    S(204, 129),    S(157, 159),    S(143, 131),    S(204, 86),     S(162, 113),    S(168, 131),    S(152, 128),    //
    S(87, 151),     S(145, 155),    S(188, 127),    S(142, 113),    S(179, 99),     S(146, 125),    S(99, 157),     S(89, 159),     //
    S(57, 145),     S(178, 88),     S(205, 71),     S(176, 57),     S(180, 74),     S(121, 115),    S(115, 132),    S(63, 170),     //
    S(30, 111),     S(93, 120),     S(113, 94),     S(97, 94),      S(125, 76),     S(85, 118),     S(75, 133),     S(21, 158),     //
    S(5, 45),       S(63, 55),      S(57, 77),      S(26, 86),      S(35, 91),      S(25, 115),     S(8, 112),      S(-14, 117),    //
    S(17, -34),     S(85, -10),     S(73, 16),      S(51, 29),      S(61, 35),      S(38, 58),      S(34, 43),      S(-4, 48),      //
    S(-62, -41),    S(63, -75),     S(68, -38),     S(56, -5),      S(60, -4),      S(46, 9),       S(37, -8),      S(5, -4),       //
    S(1, -62),      S(24, -30),     S(87, -42),     S(96, -42),     S(98, -37),     S(75, -15),     S(69, -21),     S(48, -16),     //
};
inline const std::array<PParam, 64> QUEEN_PSQT = {
    S(89, 154),     S(146, 154),    S(56, 299),     S(-2, 396),     S(35, 349),     S(71, 258),     S(72, 175),     S(25, 208),     //
    S(58, 184),     S(78, 239),     S(37, 322),     S(-82, 438),    S(-39, 406),    S(20, 288),     S(58, 175),     S(40, 146),     //
    S(20, 235),     S(60, 261),     S(29, 338),     S(-8, 373),     S(11, 362),     S(51, 259),     S(82, 155),     S(63, 119),     //
    S(16, 172),     S(28, 238),     S(-13, 307),    S(-12, 360),    S(5, 353),      S(21, 238),     S(60, 155),     S(35, 117),     //
    S(13, 145),     S(10, 169),     S(3, 218),      S(-23, 287),    S(-11, 303),    S(11, 238),     S(16, 175),     S(36, 81),      //
    S(4, 51),       S(35, 60),      S(31, 135),     S(10, 157),     S(20, 164),     S(22, 187),     S(41, 116),     S(22, 95),      //
    S(-24, -95),    S(4, -93),      S(13, -25),     S(40, 14),      S(37, 54),      S(39, 26),      S(12, 65),      S(25, 37),      //
    S(-42, -80),    S(-10, -240),   S(21, -245),    S(33, -133),    S(40, -55),     S(38, -74),     S(36, -60),     S(5, -5),       //
};
inline const std::array<PParam, 64> KING_PSQT = {
    S(164, -555),   S(473, 84),     S(313, 195),    S(18, 191),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(367, -183),   S(363, 189),    S(210, 204),    S(25, 156),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(197, 45),     S(230, 175),    S(114, 199),    S(-51, 179),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(37, 9),       S(112, 114),    S(-22, 172),    S(-118, 201),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-114, -1),    S(-22, 77),     S(-86, 146),    S(-182, 198),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-55, -35),    S(11, 34),      S(-69, 115),    S(-125, 160),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(22, -93),     S(28, -12),     S(-34, 50),     S(-110, 107),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-24, -211),   S(2, -91),      S(-75, -27),    S(-84, -40),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
};

inline const PParam KS_NO_QUEEN = S(-80, -449);

inline const std::array<PParam, 5> PT_INNER_RING_ATTACKS = {
    S(7, -4), S(10, 12), S(10, 5), S(4, 1), S(3, -9),
};
inline const std::array<PParam, 5> PT_OUTER_RING_ATTACKS = {
    S(4, -7), S(5, 5), S(2, 2), S(3, -0), S(4, -2),
};

inline const PParam KS_FLANK_ATTACK = S(3, -2);
inline const PParam KS_FLANK_DEFENSE = S(-3, -0);
inline const PParam KS_FLANK_DOUBLE_ATTACK   = S(3, -1);
inline const PParam KS_FLANK_DOUBLE_DEFENSE  = S(-2, 2);

inline const std::array<std::array<PParam, 7>, 4> KING_SHELTER = {{
  {{ S(16, -19), S(-7, 5), S(-3, -10), S(6, -16), S(12, -29), S(6, -54), S(-4, -46), }},
  {{ S(2, 3), S(-21, 2), S(-17, 10), S(-10, 17), S(-7, 8), S(-10, -13), S(-19, -21), }},
  {{ S(-3, -1), S(-12, -9), S(-12, 17), S(-8, 18), S(-7, 9), S(-6, -17), S(-22, -38), }},
  {{ S(6, 8), S(-11, 18), S(-7, 38), S(-2, 43), S(-2, 38), S(3, 19), S(9, -14), }},
}};
inline const std::array<PParam, 7> BLOCKED_SHELTER_STORM = {
    S(0, 0), S(0, 0), S(9, 24), S(-6, 3), S(-8, 10), S(-6, 29), S(1, 49),
};
inline const std::array<std::array<PParam, 7>, 4> SHELTER_STORM = {{
  {{ S(5, 12), S(-42, -121), S(-9, -39), S(-1, 5), S(-3, 4), S(-6, 11), S(-6, 10), }},
  {{ S(8, -1), S(-25, -124), S(-7, -29), S(-5, 1), S(-2, -2), S(-9, 5), S(0, 4), }},
  {{ S(-0, 12), S(-8, -69), S(6, -15), S(-0, 9), S(-4, 12), S(-10, 19), S(-6, 22), }},
  {{ S(0, 1), S(2, -62), S(-1, 25), S(-3, 26), S(-6, 9), S(-12, 11), S(-9, 19), }},
}};
inline TunableSigmoid<32> KING_SAFETY_ACTIVATION(
        1427, 910, -21, -4
);

inline VParam WINNABLE_PAWNS = V(-20);
inline VParam WINNABLE_SYM = V(102);
inline VParam WINNABLE_ASYM = V(87);
inline VParam WINNABLE_PAWN_ENDGAME = V(75);
inline VParam WINNABLE_BIAS = V(-389);

// Epoch duration: 9.56388s
// Epoch duration: 7.65336s
// clang-format on
}  // namespace Clockwork
