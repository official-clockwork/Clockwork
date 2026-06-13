#pragma once

#include "eval_types.hpp"

namespace Clockwork {

// clang-format off
inline const PParam PAWN_MAT   = S(197, 498);
inline const PParam KNIGHT_MAT = S(820, 1538);
inline const PParam BISHOP_MAT = S(867, 1625);
inline const PParam ROOK_MAT   = S(1032, 2551);
inline const PParam QUEEN_MAT  = S(2449, 3879);

inline const PParam TEMPO_VAL  = S(69, 51);

inline const PParam BISHOP_XRAY_PAWNS   = S(-13, -3);
inline const PParam BISHOP_PAIR_VAL   = S(64, 231);
inline const PParam ROOK_OPEN_VAL     = S(111, -6);
inline const PParam ROOK_SEMIOPEN_VAL = S(43, 12);
inline const PParam MINOR_BEHIND_PAWN = S(15, 39);
inline const PParam RESTRICTED_SQUARES = S(19, 7);

inline const PParam DOUBLED_PAWN_VAL = S(-21, -80);
inline const PParam ISOLATED_PAWN_VAL = S(-13, -38);

inline const PParam POTENTIAL_CHECKER_VAL = S(-45, -41);
inline const PParam OUTPOST_KNIGHT_VAL    = S(51, 42);
inline const PParam OUTPOST_BISHOP_VAL    = S(41, 39);

inline const PParam PAWN_PUSH_THREAT_KNIGHT = S(38, 48);
inline const PParam PAWN_PUSH_THREAT_BISHOP = S(41, 4);
inline const PParam PAWN_PUSH_THREAT_ROOK   = S(18, 67);
inline const PParam PAWN_PUSH_THREAT_QUEEN  = S(62, -49);

inline const std::array<PParam, 6> PAWN_PHALANX = {
    S(14, -2), S(31, 35), S(60, 64), S(121, 211), S(387, 386), S(618, 743),
};
inline const std::array<PParam, 5> DEFENDED_PAWN = {
    S(46, 33), S(38, 25), S(55, 55), S(96, 177), S(411, 103),
};
inline const std::array<PParam, 6> PASSED_PAWN = {
    S(-91, -249), S(-93, -209), S(-67, -80), S(-21, 42), S(88, 201), S(220, 406),
};
inline const std::array<PParam, 6> PASSED_CLEAR_STOPPERS = {
    S(-85, 50), S(-91, 34), S(-60, 107), S(-92, 229), S(-54, 331), S(53, 207),
};
inline const std::array<PParam, 6> PASSED_CLEAR_FORWARD = {
    S(-21, 71), S(-36, 61), S(-55, 103), S(-29, 132), S(74, 156), S(39, 177),
};
inline const std::array<PParam, 6> DEFENDED_PASSED_PUSH = {
    S(24, -40), S(25, -4), S(17, 35), S(21, 105), S(67, 276), S(327, 224),
};
inline const std::array<PParam, 6> BLOCKED_PASSED_PAWN = {
    S(13, -36), S(2, 7), S(-3, -28), S(-1, -66), S(-11, -175), S(-160, -440),
};

inline const std::array<PParam, 8> FRIENDLY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(18, 231), S(6, 179), S(2, 103), S(11, 65), S(22, 68), S(63, 62), S(61, 33),
};
inline const std::array<PParam, 8> ENEMY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-140, -45), S(17, -5), S(4, 93), S(30, 141), S(44, 176), S(54, 185), S(39, 167),
};

inline const std::array<PParam, 9> KNIGHT_MOBILITY = {
    S(-85, -373), S(-25, -132), S(3, -14), S(28, 32), S(56, 63), S(70, 99), S(88, 106), S(107, 114), S(127, 68),
};
inline const std::array<PParam, 14> BISHOP_MOBILITY = {
    S(-63, -285), S(-15, -105), S(33, -30), S(56, 20), S(77, 60), S(88, 90), S(93, 112), S(96, 132), S(100, 144), S(107, 149), S(114, 143), S(134, 124), S(139, 117), S(119, 93),
};
inline const std::array<PParam, 15> ROOK_MOBILITY = {
    S(109, -181), S(24, 11), S(44, 43), S(60, 60), S(71, 78), S(74, 92), S(79, 107), S(85, 112), S(89, 124), S(95, 131), S(100, 136), S(101, 145), S(104, 149), S(107, 139), S(132, 93),
};
inline const std::array<PParam, 28> QUEEN_MOBILITY = {
    S(-206, -162), S(-52, 7), S(-28, 134), S(-6, 259), S(10, 298), S(17, 351), S(23, 384), S(30, 399), S(34, 424), S(37, 441), S(43, 449), S(47, 457), S(50, 463), S(52, 469), S(53, 470), S(52, 474), S(48, 475), S(53, 465), S(54, 463), S(58, 453), S(55, 436), S(61, 415), S(31, 439), S(-16, 429), S(-43, 426), S(-75, 446), S(-119, 455), S(-90, 396),
};

inline const PParam PAWN_THREAT_KNIGHT = S(193, 149);
inline const PParam PAWN_THREAT_BISHOP = S(169, 215);
inline const PParam PAWN_THREAT_ROOK   = S(189, 151);
inline const PParam PAWN_THREAT_QUEEN  = S(154, 21);

inline const std::array<PParam, 6> MINOR_THREAT = {
    S(7, 54), S(91, 92), S(102, 124), S(202, 125), S(159, 10), S(0, 0),
};
inline const std::array<PParam, 6> ROOK_THREAT = {
    S(8, 55), S(52, 93), S(74, 90), S(16, 13), S(148, -49), S(0, 0),
};
inline const PParam KING_THREAT  = S(-18, 155);
inline const PParam HANGING_PAWN  = S(31, 90);
inline const PParam HANGING_NON_PAWN  = S(68, 27);

inline const std::array<PParam, 2> KNIGHT_ON_QUEEN = {
    S(21, -18), S(30, -7),
};
inline const std::array<PParam, 2> BISHOP_ON_QUEEN = {
    S(39, 47), S(121, -119),
};
inline const std::array<PParam, 2> ROOK_ON_QUEEN = {
    S(37, 11), S(50, -8),
};

inline const std::array<PParam, 9> BISHOP_PAWNS = {
    S(3, -15), S(4, -14), S(-0, -20), S(-6, -28), S(-12, -36), S(-17, -43), S(-18, -55), S(-24, -58), S(-28, -73),
};

inline const PParam ROOK_LINEUP = S(17, 78);

inline const std::array<PParam, 48> PAWN_PSQT = {
    S(181, 266),    S(39, 396),     S(71, 371),     S(144, 265),    S(199, 156),    S(135, 213),    S(98, 249),     S(235, 172),    //
    S(41, 60),      S(53, 106),     S(29, 59),      S(42, -9),      S(33, -42),     S(10, -16),     S(10, 26),      S(-18, 53),     //
    S(37, -13),     S(12, -3),      S(41, -46),     S(27, -68),     S(17, -86),     S(-1, -73),     S(-38, -40),    S(-39, -2),     //
    S(-7, -71),     S(-31, -41),    S(2, -61),      S(-6, -74),     S(-28, -83),    S(-37, -72),    S(-84, -52),    S(-77, -50),    //
    S(-11, -108),   S(22, -106),    S(7, -45),      S(-8, -50),     S(-36, -60),    S(-50, -70),    S(-79, -68),    S(-81, -72),    //
    S(11, -105),    S(85, -94),     S(70, -40),     S(27, -14),     S(-4, -35),     S(-23, -60),    S(-48, -56),    S(-61, -54),    //
};
inline const std::array<PParam, 64> KNIGHT_PSQT = {
    S(-255, -497),  S(-250, 52),    S(-269, -69),   S(30, 22),      S(-94, 43),     S(-263, 79),    S(-362, 112),   S(-347, -388),  //
    S(-5, -7),      S(1, 35),       S(91, 29),      S(80, 57),      S(85, 49),      S(54, 25),      S(-26, 30),     S(-31, 21),     //
    S(48, -26),     S(37, 32),      S(67, 55),      S(70, 74),      S(59, 72),      S(21, 59),      S(8, 36),       S(-41, 19),     //
    S(96, 25),      S(89, 51),      S(101, 69),     S(99, 113),     S(104, 113),    S(69, 76),      S(50, 43),      S(38, 35),      //
    S(77, 18),      S(103, 8),      S(98, 51),      S(91, 84),      S(78, 84),      S(74, 65),      S(63, 11),      S(24, 32),      //
    S(19, -36),     S(46, -18),     S(50, 22),      S(63, 56),      S(58, 53),      S(31, 16),      S(16, -8),      S(-23, -33),    //
    S(42, -23),     S(43, -8),      S(29, -14),     S(36, 14),      S(33, 10),      S(7, -41),      S(-29, 2),      S(-40, -78),    //
    S(-40, -74),    S(10, -22),     S(29, -41),     S(47, -29),     S(27, -9),      S(-9, -47),     S(-17, -21),    S(-61, -129),   //
};
inline const std::array<PParam, 64> BISHOP_PSQT = {
    S(-123, 20),    S(-176, 106),   S(-424, 188),   S(-259, 92),    S(-297, 149),   S(-298, 154),   S(-212, 129),   S(-121, 71),    //
    S(-28, -14),    S(-73, 92),     S(-33, 55),     S(-66, 69),     S(-67, 80),     S(-35, 49),     S(-6, 42),      S(-44, 27),     //
    S(38, 26),      S(16, 65),      S(34, 74),      S(28, 64),      S(22, 52),      S(17, 57),      S(15, 49),      S(17, 20),      //
    S(16, 5),       S(52, 28),      S(63, 44),      S(76, 63),      S(102, 48),     S(45, 28),      S(51, 2),       S(2, 5),        //
    S(44, -40),     S(46, 11),      S(79, 19),      S(94, 32),      S(78, 46),      S(69, 29),      S(22, 13),      S(20, -30),     //
    S(60, -23),     S(85, -21),     S(99, 9),       S(66, 37),      S(64, 20),      S(60, 22),      S(66, 0),       S(20, -13),     //
    S(31, -72),     S(116, -47),    S(71, -12),     S(50, 6),       S(32, 7),       S(53, -38),     S(51, -47),     S(43, -45),     //
    S(52, -74),     S(22, -10),     S(26, -0),      S(49, -26),     S(31, -15),     S(37, 20),      S(46, -13),     S(43, -53),     //
};
inline const std::array<PParam, 64> ROOK_PSQT = {
    S(177, 121),    S(204, 140),    S(149, 167),    S(140, 140),    S(205, 92),     S(168, 118),    S(173, 135),    S(150, 139),    //
    S(85, 170),     S(138, 168),    S(196, 129),    S(143, 125),    S(185, 107),    S(145, 134),    S(93, 168),     S(90, 169),     //
    S(54, 159),     S(174, 105),    S(202, 81),     S(180, 62),     S(177, 86),     S(117, 127),    S(120, 134),    S(64, 180),     //
    S(32, 121),     S(93, 125),     S(121, 96),     S(95, 101),     S(124, 86),     S(85, 130),     S(76, 142),     S(20, 168),     //
    S(2, 57),       S(66, 65),      S(60, 80),      S(29, 91),      S(39, 96),      S(27, 122),     S(10, 120),     S(-11, 123),    //
    S(19, -27),     S(85, 0),       S(75, 22),      S(55, 32),      S(65, 38),      S(38, 66),      S(39, 49),      S(-2, 53),      //
    S(-61, -31),    S(66, -67),     S(72, -31),     S(60, 2),       S(64, 0),       S(47, 18),      S(37, 1),       S(9, 3),        //
    S(-1, -52),     S(15, -14),     S(86, -30),     S(96, -33),     S(95, -25),     S(72, -1),      S(70, -12),     S(51, -6),      //
};
inline const std::array<PParam, 64> QUEEN_PSQT = {
    S(73, 181),     S(145, 164),    S(57, 300),     S(-5, 403),     S(39, 348),     S(71, 260),     S(64, 188),     S(2, 238),      //
    S(56, 202),     S(75, 251),     S(44, 316),     S(-92, 450),    S(-43, 416),    S(6, 315),      S(49, 188),     S(24, 179),     //
    S(17, 247),     S(51, 287),     S(18, 360),     S(-20, 397),    S(1, 377),      S(46, 269),     S(78, 168),     S(59, 122),     //
    S(17, 178),     S(19, 259),     S(-14, 311),    S(-25, 379),    S(-10, 381),    S(19, 250),     S(56, 163),     S(27, 137),     //
    S(10, 166),     S(7, 186),      S(-3, 234),     S(-30, 304),    S(-18, 324),    S(6, 252),      S(9, 189),      S(33, 95),      //
    S(3, 67),       S(33, 79),      S(28, 154),     S(7, 172),      S(15, 180),     S(16, 205),     S(37, 135),     S(17, 110),     //
    S(-25, -74),    S(5, -72),      S(12, -2),      S(35, 38),      S(31, 76),      S(33, 52),      S(7, 84),       S(21, 55),      //
    S(-36, -60),    S(-19, -209),   S(11, -211),    S(25, -99),     S(33, -23),     S(32, -56),     S(30, -36),     S(1, 14),       //
};
inline const std::array<PParam, 64> KING_PSQT = {
    S(109, -512),   S(453, 82),     S(284, 216),    S(-18, 199),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(338, -166),   S(347, 197),    S(192, 211),    S(10, 161),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(187, 48),     S(218, 182),    S(112, 204),    S(-48, 183),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(14, 17),      S(90, 124),     S(-25, 174),    S(-110, 199),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-120, 2),     S(-34, 81),     S(-98, 149),    S(-183, 200),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-51, -37),    S(14, 29),      S(-74, 114),    S(-129, 160),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(27, -95),     S(33, -17),     S(-32, 46),     S(-109, 103),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-14, -213),   S(8, -94),      S(-75, -28),    S(-72, -49),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
};

inline const PParam KS_NO_QUEEN = S(-84, -438);

inline const std::array<PParam, 5> PT_INNER_RING_ATTACKS = {
    S(7, -4), S(10, 12), S(10, 5), S(5, 1), S(3, -10),
};
inline const std::array<PParam, 5> PT_OUTER_RING_ATTACKS = {
    S(4, -7), S(5, 5), S(2, 2), S(3, -0), S(4, -1),
};

inline const PParam KS_FLANK_ATTACK = S(3, -2);
inline const PParam KS_FLANK_DEFENSE = S(-3, -0);
inline const PParam KS_FLANK_DOUBLE_ATTACK   = S(3, -1);
inline const PParam KS_FLANK_DOUBLE_DEFENSE  = S(-2, 2);

inline const std::array<std::array<PParam, 7>, 4> KING_SHELTER = {{
  {{ S(17, -20), S(-6, 5), S(-3, -10), S(8, -17), S(12, -29), S(7, -58), S(-5, -50), }},
  {{ S(2, 1), S(-21, -1), S(-17, 7), S(-10, 16), S(-7, 6), S(-9, -15), S(-21, -23), }},
  {{ S(-4, -1), S(-12, -10), S(-14, 18), S(-9, 18), S(-7, 10), S(-8, -18), S(-25, -43), }},
  {{ S(7, 8), S(-10, 18), S(-7, 40), S(-2, 43), S(-2, 39), S(4, 17), S(8, -15), }},
}};
inline const std::array<PParam, 7> BLOCKED_SHELTER_STORM = {
    S(0, 0), S(0, 0), S(11, 23), S(-5, 2), S(-7, 10), S(-5, 28), S(1, 51),
};
inline const std::array<std::array<PParam, 7>, 4> SHELTER_STORM = {{
  {{ S(6, 10), S(-43, -128), S(-8, -43), S(0, 3), S(-2, 3), S(-5, 10), S(-5, 8), }},
  {{ S(10, -1), S(-23, -125), S(-5, -32), S(-3, -1), S(-0, -3), S(-7, 4), S(2, 2), }},
  {{ S(0, 10), S(-9, -81), S(8, -19), S(1, 8), S(-3, 10), S(-9, 18), S(-5, 20), }},
  {{ S(2, -1), S(6, -65), S(-0, 27), S(-2, 25), S(-5, 8), S(-12, 11), S(-9, 19), }},
}};
inline TunableSigmoid<32> KING_SAFETY_ACTIVATION(
        1361, 846, -21, 0
);

inline VParam WINNABLE_PAWNS = V(-19);
inline VParam WINNABLE_SYM = V(101);
inline VParam WINNABLE_ASYM = V(84);
inline VParam WINNABLE_PAWN_ENDGAME = V(88);
inline VParam WINNABLE_BIAS = V(-387);

// Epoch duration: 6.36129s
// clang-format on
}  // namespace Clockwork
